#include <vector>
#include <map>
#include <iostream>

#include "common.h"
#include "CDispatcher.h"
#include "serialport.h"
#include "czbframe.h"
#include "DomBase.h"
#include "czbconfiguration.h"
#include "czblog.hpp"

CDispatcher Dispatcher;

CDispatcher::CDispatcher()
{
    std::string edname = "local";
    data_vct_ptr vctAddress(new std::vector<boost::uint8_t>(8,0x00));
    //AddNewEndDevice(edname, vctAddress); // local end device
    
    vctAddress = data_vct_ptr(new std::vector<boost::uint8_t>());
    vctAddress->push_back(0x00);
    vctAddress->push_back(0x13);
    vctAddress->push_back(0xa2);
    vctAddress->push_back(0x00);
    vctAddress->push_back(0x40);
    vctAddress->push_back(0x54);
    vctAddress->push_back(0xd3);
    vctAddress->push_back(0x1b);
    edname = "ED02";
    //AddNewEndDevice(edname, vctAddress);

    vctAddress = data_vct_ptr(new std::vector<boost::uint8_t>());
    vctAddress->push_back(0x00);
    vctAddress->push_back(0x13);
    vctAddress->push_back(0xa2);
    vctAddress->push_back(0x00);
    vctAddress->push_back(0x40);
    vctAddress->push_back(0x2c);
    vctAddress->push_back(0x53);
    vctAddress->push_back(0x28);
    edname = "ED01";
    //AddNewEndDevice(edname, vctAddress);

    bTXThreadExit = false;
    TXThread = boost::thread(&CDispatcher::processTX, this);

}

void CDispatcher::AddFrameToTransmit(data_vct_ptr newframe)
{
    data_vct_ptr toCompare;
    data_vct_ptr address;

    // find frame in memory, to avoid resend a repeated frame
    if(CZBFrame::getAPIIdentifierFromRFDataFrame(newframe)!=API_AT_REMOTE_COMMAND_REQUEST)
    {
        std::cerr << "AddFrameToTransmit: this is not a API_AT_REMOTE_COMMAND_REQUEST API identifier";
        //LOG->ERROR_WARNING_LOG("AddFrameToTransmit: this is not a API_AT_REMOTE_COMMAND_REQUEST API identifier");
        return;
    }

    address->assign(newframe->begin()+5,newframe->end()+12);

    CZBEndDevicePtr ptr = getZBEndDeviceByAddress(address);

    if(ptr.get()==NULL){
        //TODO: throw this, and log it too
        std::cerr << "AddFrameToTransmit: end device not found with address " << address;

        return;
    }

    toCompare->assign(newframe->begin()+5,newframe->begin()+18);
    // search frame return a true if found a frame with the same data
    //    if(ptr->searchFrame(toCompare))
    //        return;

    // after all test behind, add data to transmit
}

void CDispatcher::processTX(void)
{
    while(!bTXThreadExit)
    {

        std::map<std::string,CZBEndDevicePtr>::iterator it_ed;
        for(it_ed=mapEndDevices.begin();it_ed!=mapEndDevices.end();++it_ed)
        {
            CZBEndDevicePtr zbptr = it_ed->second;
            // CHECK DATA TO SEND
            try{
                data_vct_ptr vctFrameID = zbptr->getFrameIDVector();
                for(size_t i=0;i<vctFrameID->size();i++)
                {

                    stTXFrame stxf = zbptr->get_stTXFrame(vctFrameID->at(i));

                    switch (stxf.enFS)
                    {

                    case OK: // sended data is OK, must delete from the list
                        zbptr->setEnFrameStatus(vctFrameID->at(i),F_DELETE);
                        break;

                    case F_ERROR: // data error
                        zbptr->setEnFrameStatus(vctFrameID->at(i),F_DELETE);
                        break;

                    case INVALID_COMMAND:// frame with an invalid command
                        zbptr->setEnFrameStatus(vctFrameID->at(i),F_DELETE);
                        break;

                    case INVALID_PARAMETER: // frame send with a invalid parameter
                        std::cerr << "WARNING: invalid parameter for frame " << stxf.vctBuffer << zbptr->GetName();
                        zbptr->setEnFrameStatus(vctFrameID->at(i),F_DELETE);
                        break;

                    case NO_RESPONSE:// this is the response of the ZigBee device...
                        zbptr->setEnFrameStatus(vctFrameID->at(i),WAITING_RESPONSE);
                        break;

                    case NONE:// do nothing
                        break;

                    case SEND: // send data
                        if(stxf.uiTryCounter >= 5)
                        {
                            std::cerr << "WARNING: max sen data try reached for end device " << zbptr->GetName();
                            zbptr->setEnFrameStatus(vctFrameID->at(i),F_DELETE);
                            break;
                        }
                        stxf.tLastTime = boost::posix_time::microsec_clock::universal_time();
                        stxf.tTime = stxf.tLastTime;
                        SerialPort.SendBuffer(stxf.vctBuffer);
                        zbptr->setStTxFrame(vctFrameID->at(i),stxf);
                        zbptr->setEnFrameStatus(vctFrameID->at(i),WAITING_RESPONSE);
                        break;

                    case DO_NOTHING: // do nothing, this come from a new frameID request
                        break;

                    case WAITING_RESPONSE:
                    {
                        stxf.tTime = boost::posix_time::microsec_clock::universal_time();
                        boost::posix_time::time_duration duration_miliseconds = stxf.tTime-stxf.tLastTime;
                        if ( duration_miliseconds.total_milliseconds() > 2000 )
                        {
                            stxf.uiTryCounter++;
                            stxf.enFS = SEND;
                            zbptr->setStTxFrame(vctFrameID->at(i),stxf);
                            std::cout << "resending frame " << (unsigned short) stxf.frameID << " for " << stxf.uiTryCounter << " times" << std::endl;
                            break;
                        }
                    }
                    break;

                    case F_DELETE:
                        std::cout << "processTX: deleting mapTXbuffer at __key " << (unsigned short)vctFrameID->at(i) << std::endl;
                        zbptr->freeFrameID(vctFrameID->at(i));
                        break;

                    default:
                        break;

                    }

                }
            }catch(std::exception &e)
            {
                std::cerr << e.what();
                continue;
            }

        }

        try{
            std::vector<CZBFramePtr>::iterator it;
            boost::mutex::scoped_lock scoped_lock(rfbuffer_mutex);
            for(it=RFFrameBuffer.begin();it!=RFFrameBuffer.end();++it)
            {
                addRFFrame((*it)->getDataVector());
            }
            RFFrameBuffer.clear();
        }
        catch(std::exception &e)
        {
            std::cerr << "CheckAndAddRawFrames exception catched: " << e.what();
            return;
        }
        catch(boost::exception &b)
        {

        }

        usleep(10000);
    }
}

void CDispatcher::AddNewEndDevice(const std::string &name, data_vct_ptr vctAddress)
{
    this->mapEndDevices[name] = CZBEndDevicePtr(new CZBEndDevice(name,vctAddress));
    CZBEndDevicePtr ptr = mapEndDevices[name];
    ptr->AddDataChannel("D0");
    ptr->AddDataChannel("D1");
    ptr->AddDataChannel("D2");
    ptr->AddDataChannel("D3");
    ptr->AddDataChannel("D4");
    ptr->AddDataChannel("D5");
    ptr->AddDataChannel("D6");
    ptr->AddDataChannel("D7");
    ptr->AddDataChannel("D8");
    ptr->AddDataChannel("RSSI");
    EDListChangedCallBack(getZBEDListNames());
}

void CDispatcher::addRFFrame(data_vct_ptr _vctRFData)
{
    switch( CZBFrame::getAPIIdentifierFromRFDataFrame(_vctRFData) )
    {
    case API_AT_REMOTE_COMMAND_REQUEST: // comando remoto
        std::cout << "API_AT_REMOTE_COMMAND_REQUEST" << std::endl;
        break;
    case API_AT_REMOTE_COMMAND_RESPONSE: // respuesta a comando remoto
        std::cout << "API_AT_REMOTE_COMMAND_RESPONSE : ";
        ShowVectorUINT8(_vctRFData);
        std::cout << std::endl;
        Function_API_AT_REMOTE_COMMAND_RESPONSE(CZBFrame::getcmdDataFromRFDataFrame(_vctRFData));
        break;
    case API_AT_COMMAND: // comando local
        std::cout << "API_AT_COMMAND" << std::endl;
        break;
    case API_AT_COMMAND_RESPONSE: // respuesta a un comando AT enviado
        std::cout << "API_AT_COMMAND_RESPONSE : ";
        ShowVectorUINT8(_vctRFData); std::cout << std::endl;
        break;
    case API_MODEM_STATUS: // status del modem
        std::cout << "API_MODEM_STATUS" << std::endl;
        break;
    case API_QUEUE_PARAMETER_VALUE:
        std::cout << "API_QUEUE_PARAMETER_VALUE" << std::endl;
        break;
    case API_TX_REQUEST_64bit_ADDRESS:
        std::cout << "API_TX_REQUEST_64bit_ADDRESS" << std::endl;
        break;
    case API_TX_REQUEST_16bit_ADDRESS:
        std::cout << "API_TX_REQUEST_16bit_ADDRESS" << std::endl;
        break;
    case API_TX_STATUS:
        std::cout << "API_TX_STATUS";
        ShowVectorUINT8(_vctRFData); std::cout << std::endl;
        break;
    case API_RX_PACKET_64bit_ADDRESS:
        std::cout << "API_RX_PACKET_64bit_ADDRESS" << std::endl;
        break;
    case API_RX_PACKET_16bit_ADDRESS:
        std::cout << "API_RX_PACKET_16bit_ADDRESS" << std::endl;
        break;
    case API_RX_IO_DATA_16bit_ADDRESS:
        std::cout << "API_RX_IO_DATA_16bit_ADDRESS" << std::endl;
        break;
    case API_RX_IO_DATA_64bit_ADDRESS:
        //std::cout << "CmdID:0x" << std::hex << API_RX_IO_DATA_64bit_ADDRESS << " : API_RX_IO_DATA_64bit_ADDRESS" << std::endl;
        Function_API_RX_IO_DATA_64bit_ADDRESS(CZBFrame::getcmdDataFromRFDataFrame(_vctRFData));
        break;
    default:
        break;
    }
}

void CDispatcher::Function_API_AT_REMOTE_COMMAND_RESPONSE(data_vct_ptr _vctRFData)
{
    uint8_t frameID = _vctRFData->at(0);

    std::cout << "frameID: " << (unsigned short) frameID << std::endl;

    if(frameID==0)
    {
        std::cerr << "Function_API_AT_REMOTE_COMMAND_RESPONSE: error with frameID of " << (unsigned short) frameID << std::endl;
        return;
        //TODO: maybe broadcast message !!!!
    }

    data_vct_ptr address( new std::vector<boost::uint8_t>() );
    address->assign(_vctRFData->begin()+1,_vctRFData->begin()+9);

    std::cout << "ED address : " << std::hex << address << std::endl;

    data_vct_ptr NA; // network address
    NA->push_back(_vctRFData->at(9));
    NA->push_back(_vctRFData->at(10));

    std::cout << "Network address: " << std::hex << NA << std::endl;

    data_vct_ptr command;
    uint8_t uiCommand[2];
    uiCommand[0] = _vctRFData->at(11);
    uiCommand[1] = _vctRFData->at(12);

    std::cout << "command: " << uiCommand[0] << uiCommand[1] << std::endl;

    command->push_back(_vctRFData->at(11));
    command->push_back(_vctRFData->at(12));

    CZBEndDevicePtr czbptr = getZBEndDeviceByAddress(address);

    if(czbptr.get()==NULL)
    {
        std::cerr << "Function_API_AT_REMOTE_COMMAND_RESPONSE: end device not found, with address " << address << std::endl;
        return;
    }

    if(!czbptr->checkFrameID(frameID)){
        std::cerr << "Function_API_AT_REMOTE_COMMAND_RESPONSE: frame ID not found" << std::endl;
        return;
    }

    enFrameStatus status = (enFrameStatus)_vctRFData->at(13);

    data_vct_ptr vctCommandData;

    switch(status)
    {
    case OK:
        std::cout << "status: " << "OK" << std::endl;
        // delete frame from frame stack
        czbptr->setEnFrameStatus(frameID,OK);
        break;
    case F_ERROR:
        std::cout << "status: " << "F_ERROR" << std::endl;
        // send to cerr and log it
        czbptr->setEnFrameStatus(frameID,F_ERROR);
        break;
    case INVALID_COMMAND:
        std::cout << "status: " << "INVALID_COMMAND" << std::endl;
        // hummm, I dont know what to do
        czbptr->setEnFrameStatus(frameID,INVALID_COMMAND);
        break;
    case INVALID_PARAMETER:
        std::cout << "status: " << "INVALID_PARAMETER" << std::endl;
        // idem that before
        czbptr->setEnFrameStatus(frameID,INVALID_PARAMETER);
        break;
    case NO_RESPONSE:
        std::cout << "status: " << "NO_RESPONSE" << std::endl;
        // invelidate end device
        czbptr->setEnFrameStatus(frameID,NO_RESPONSE);
        break;
    default:
        break;
    }

    for(size_t k=14;k<_vctRFData->size();k++)
    {
        vctCommandData->push_back(_vctRFData->at(k));
    }

    std::cout << "command data: " << vctCommandData << std::endl;

}

uint8_t CDispatcher::calculateChechSumUINT8(data_vct_ptr _vctBuffer)
{
    uint8_t var_return=0;

    std::vector<uint8_t>::iterator it;
    for(it=_vctBuffer->begin();it!=_vctBuffer->end();++it)
    {
        var_return += (*it);
    }

    var_return = 0xFF - var_return;
    return var_return;
}

void CDispatcher::Function_API_AT_REMOTE_COMMAND_REQUEST(const std::string &_sEDName, data_vct_ptr _vctCommand, data_vct_ptr _vctRFData)
{
    CZBEndDevicePtr czbed = getZBEndDeviceByName(_sEDName);
    if(czbed.get()==NULL)
    {
        std::cerr << "Function_API_AT_REMOTE_COMMAND_REQUEST: end device " << czbed->GetAddress() << " not found." << std::endl;
        //TODO: throw this ??
        return;
    }
    data_vct_ptr address(new std::vector<boost::uint8_t>());
    address = czbed->GetAddress();
    Function_API_AT_REMOTE_COMMAND_REQUEST(address,_vctCommand,_vctRFData);
}

void CDispatcher::Function_API_AT_REMOTE_COMMAND_REQUEST(data_vct_ptr address, data_vct_ptr _vctCommand, data_vct_ptr _vctCommandData)
{

    uint8_t delimeter = 0x7E;
    uint8_t MSB;
    uint8_t LSB;
    uint8_t API_ID = 0x17;
    data_vct_ptr vctNA(new std::vector<boost::uint8_t>());
    vctNA->push_back(0xFF);
    vctNA->push_back(0xFE);
    //command options
    uint8_t command_options = 0x02; // apply changes
    uint8_t frame_id = getZBEndDeviceByAddress(address)->getReserveFrameID();

    if(frame_id==0){
        std::cerr << "can't reserve new frame ID in end device tx map buffer of " << getZBEndDeviceByAddress(address)->GetName();
        return;
    }

    // join all together
    data_vct_ptr frame(new std::vector<boost::uint8_t>());
    frame->push_back(delimeter);
    frame->push_back(0x00); // MSB
    frame->push_back(0x00); // LSB
    frame->push_back(API_ID);
    frame->push_back(frame_id);

    for(size_t i=0;i<address->size();i++)
    {
        frame->push_back(address->at(i));
    }

    frame->push_back(vctNA->at(0));
    frame->push_back(vctNA->at(1));
    frame->push_back(command_options);

    frame->insert(frame->end(),_vctCommand->begin(),_vctCommand->end());
    frame->insert(frame->end(),_vctCommandData->begin(),_vctCommandData->end());

    data_vct_ptr newFrame(new std::vector<boost::uint8_t>());

    newFrame->insert(newFrame->begin(),frame->begin()+3,frame->end());
    frame->push_back(calculateChechSumUINT8(newFrame));

    uint16_t SIZE = frame->size()-4;

    MSB = uint8_t( ( SIZE & 0xFF00 ) >> 8 );
    LSB = uint8_t( SIZE & 0x00FF );

    frame->at(1) = MSB;
    frame->at(2) = LSB;

    std::cout << "Function_API_AT_REMOTE_COMMAND_REQUEST: ";
    std::cout << std::endl;

    stTXFrame stxf;
    stxf.vctBuffer = frame;
    stxf.buffersize = stxf.vctBuffer->size();
    stxf.uiTryCounter = 0;
    stxf.enFS = SEND;
    stxf.frameID = frame_id;

    getZBEndDeviceByAddress(address)->addTXBuffer(frame_id,stxf);
//    SerialPort.SendBuffer(frame);
}

void CDispatcher::SendCommand(const std::string &_sEDName, data_vct_ptr _vctCommand, data_vct_ptr _vctRFData)
{
    if(_sEDName.compare("local")==0)
    {
        return; // not local commands in this moment
    }
    Function_API_AT_REMOTE_COMMAND_REQUEST(_sEDName, _vctCommand, _vctRFData);
}

/*void CDispatcher::Function_API_TX_REQUEST_64bit_ADDRESS( data_vct_ptr _vctRFFrame) // complete rf frame
  {
  //
  }*/

void CDispatcher::Function_API_RX_IO_DATA_64bit_ADDRESS(data_vct_ptr _vctCmdData)
{
    data_vct_ptr vctAddress = CZBFrame::get64bAddressFromCmdData(_vctCmdData);
    uint8_t RSSI = _vctCmdData->at(8);

    if(vctAddress->size()!=0)
    {
        AddDataToZBED(getZBEndDeviceByAddress(vctAddress),std::string("RSSI"),RSSI,boost::posix_time::microsec_clock::universal_time());
    }
    //uint8_t OPTIONS = _vctCmdData[9]; // unused
    data_vct_ptr vctRFData;
    vctRFData->assign(_vctCmdData->begin()+10,_vctCmdData->end());
    ReadSample(vctRFData,vctAddress);
}

void CDispatcher::AddDataToZBED( CZBEndDevicePtr zbptr, const std::string & sDataChannel, const uint16_t u16d , const ptime &_ptime )
{
    if(zbptr.get()!=NULL)
    {
        zbptr->AddData( sDataChannel,u16d,_ptime);
    }
}

void CDispatcher::ReadSample(data_vct_ptr _vctRFData, data_vct_ptr _vctAddress)
{
    uint8_t samples = _vctRFData->at(0);
    uint16_t sCI = ( ((unsigned short)(_vctRFData->at(1))) << 8 ) + ((unsigned short)(_vctRFData->at(2)))  ; // ChannelIndicator
    uint16_t k = 0;

    data_vct_ptr vctSamples( new std::vector<boost::uint8_t>() );
    vctSamples->assign(_vctRFData->begin()+3,_vctRFData->end());

    unsigned int sample_size = vctSamples->size()/samples;

    CZBEndDevicePtr zbptr = getZBEndDeviceByAddress(_vctAddress);

    if(!zbptr)
    {
        std::cerr << "EndDevice not found";
        // TODO: warning this with a exception ??
        return;
    }

    ptime mytime = boost::posix_time::microsec_clock::universal_time();

    for(size_t i=0;i<samples;i++)
    {

        if( (sCI & CI_D0) || (sCI & CI_D1) ||
            (sCI & CI_D2) || (sCI & CI_D3) ||
            (sCI & CI_D4) || (sCI & CI_D5) ||
            (sCI & CI_D6) || (sCI & CI_D7) ||
            (sCI & CI_D8) )
        {
            uint16_t DIOLineData = ( uint16_t(vctSamples->at(sample_size*i+(k*2))) << 8 ) + uint16_t(vctSamples->at(sample_size*i+1+(k*2)));
            k++;
            if ((sCI & CI_D0) != 0x0000) AddDataToZBED(zbptr,"D0",API_CI_D0(DIOLineData),mytime);
            if ((sCI & CI_D1) != 0x0000) AddDataToZBED(zbptr,"D1",API_CI_D1(DIOLineData),mytime);
            if ((sCI & CI_D2) != 0x0000) AddDataToZBED(zbptr,"D2",API_CI_D2(DIOLineData),mytime);
            if ((sCI & CI_D3) != 0x0000) AddDataToZBED(zbptr,"D3",API_CI_D3(DIOLineData),mytime);
            if ((sCI & CI_D4) != 0x0000) AddDataToZBED(zbptr,"D4",API_CI_D4(DIOLineData),mytime);
            if ((sCI & CI_D5) != 0x0000) AddDataToZBED(zbptr,"D5",API_CI_D5(DIOLineData),mytime);
            if ((sCI & CI_D6) != 0x0000) AddDataToZBED(zbptr,"D6",API_CI_D6(DIOLineData),mytime);
            if ((sCI & CI_D7) != 0x0000) AddDataToZBED(zbptr,"D7",API_CI_D7(DIOLineData),mytime);
            if ((sCI & CI_D8) != 0x0000) AddDataToZBED(zbptr,"D8",API_CI_D8(DIOLineData),mytime);
        }

        if( (sCI & CI_ADC0) || (sCI & CI_ADC1) ||
            (sCI & CI_ADC2) || (sCI & CI_ADC3) ||
            (sCI & CI_ADC4)  || (sCI & CI_ADC5) )
        {
            //unit16_t dat;
            if ((sCI & CI_ADC0) != 0x0000) {
                //dat = (uint16_t(vctSamples[sample_size*i+k*2]) << 8 ) + uint16_t(vctSamples[sample_size*i+1+k*2]);
                AddDataToZBED(zbptr,"D0", (uint16_t(vctSamples->at(sample_size*i+k*2)) << 8 ) + uint16_t(vctSamples->at(sample_size*i+1+k*2)),mytime);
                k++;
            }
            if ((sCI & CI_ADC1) != 0x0000) {
                AddDataToZBED(zbptr,"D1", (uint16_t(vctSamples->at(sample_size*i+k*2)) << 8 ) + uint16_t(vctSamples->at(sample_size*i+1+k*2)),mytime);
                k++;
            }
            if ((sCI & CI_ADC2) != 0x0000) {
                AddDataToZBED(zbptr,"D2", (uint16_t(vctSamples->at(sample_size*i+k*2)) << 8 ) + uint16_t(vctSamples->at(sample_size*i+1+k*2)),mytime);
                k++;
            }
            if ((sCI & CI_ADC3) != 0x0000) {
                AddDataToZBED(zbptr,"D3", (uint16_t(vctSamples->at(sample_size*i+k*2)) << 8 ) + uint16_t(vctSamples->at(sample_size*i+1+k*2)),mytime);
                k++;
            }
            if ((sCI & CI_ADC4) != 0x0000) {
                AddDataToZBED(zbptr,"D4", (uint16_t(vctSamples->at(sample_size*i+k*2)) << 8 ) + uint16_t(vctSamples->at(sample_size*i+1+k*2)),mytime);
                k++;
            }
            if ((sCI & CI_ADC5) != 0x0000) {
                AddDataToZBED(zbptr,"D5", (uint16_t(vctSamples->at(sample_size*i+k*2)) << 8 ) + uint16_t(vctSamples->at(sample_size*i+1+k*2)),mytime);
                k++;
            }
        }
        k=0;
    }
}

CZBEndDevicePtr CDispatcher::getZBEndDeviceByAddress(data_vct_ptr _vctAddress)
{
    std::map<std::string,CZBEndDevicePtr>::iterator it;

    for(it=mapEndDevices.begin();it!=mapEndDevices.end();++it)
    {
        CZBEndDevicePtr ptr = it->second;
        if(ptr->CompareAddress(_vctAddress))
	{
            return ptr;
	}
    }

    return CZBEndDevicePtr();
}

CZBEndDevicePtr CDispatcher::getZBEndDeviceByName(const std::string &_name)
{
    std::map<std::string,CZBEndDevicePtr>::iterator it;
    for(it=mapEndDevices.begin();it!=mapEndDevices.end();++it)
    {
        std::string str = it->first;
        if(str.compare(_name)==0)
	{
            return it->second;
        }
    }

    return CZBEndDevicePtr();
}

void CDispatcher::CheckAndAddRawFrames(data_vct_ptr _vctRawBuffer)
{
    CZBFramePtr ptr;

    try
    {
        boost::mutex::scoped_lock scoped_lock(rfbuffer_mutex);
        RFFrameBuffer.push_back(CZBFramePtr(new CZBFrame(_vctRawBuffer)));
    }catch( bad_rf_frame &e )
    {
        std::cerr << "CheckAndAddRawFrames: bad_rf_frame throw";
        return;
    }
    
}

std::vector<std::string> CDispatcher::getZBEDListNames(void)
{
    std::vector<std::string> vctEDNames;
    std::map<std::string,CZBEndDevicePtr>::iterator it;

    for(it=mapEndDevices.begin();it!=mapEndDevices.end();++it)
    {
        vctEDNames.push_back((*it).first);
    }

    return vctEDNames;
}

data_vct_ptr CDispatcher::GetEndDeviceAddressByName(std::string &_sName)
{
    std::map<std::string,CZBEndDevicePtr>::iterator it;
    data_vct_ptr vctUINT8_t;

    for(it=mapEndDevices.begin();it!=mapEndDevices.end();++it)
    {
        CZBEndDevicePtr ptr = it->second;
        std::string edName = it->first;
        if(edName.compare(_sName)==0)
        {
            vctUINT8_t = ptr->GetAddress();
        }
    }

    return vctUINT8_t;
}

boost::signals2::connection CDispatcher::setDataCallback(const std::string &_sName, CEndDevice::signalChannel::slot_type &_subscriber)
{
    CZBEndDevicePtr edptr = getZBEndDeviceByName(_sName);

    if(edptr.get()==NULL){
        // throw this
        return boost::signals2::connection();
    }

    return edptr->connectChannelToFunction(_subscriber);
}


