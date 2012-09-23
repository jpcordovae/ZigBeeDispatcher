/*
 * File:   czbframe.cpp
 * Author: jpcordovae
 *
 * Created on March 6, 2011, 1:53 PM
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "czbframe.h"
#include "common.h"

CZBFrame::CZBFrame() {
}

CZBFrame::CZBFrame(const CZBFrame& orig) {
}

CZBFrame::~CZBFrame() {
}

CZBFrame::CZBFrame(data_vct_ptr _dbuffer)
{
    setFromRawFrame( _dbuffer );
}

bool CZBFrame::setFromRawFrame(data_vct_ptr _dbuffer)
{

    if( ! CheckRawFrame(_dbuffer) )
    {
	// TODO: log this
        return false;
    }

    Delimeter = _dbuffer->at(0);
    MSB = _dbuffer->at(1);
    LSB = _dbuffer->at(2);

    Lenght = ((short) LSB) + (((short) MSB) << 8); // TODO: check endianes issue

    for (size_t k = 3; k < _dbuffer->size()-1; k++) // -1 por que no hay que considerar el checksum
    {
        vctFrameData->push_back(_dbuffer->at(k));
    }

    CheckSum = getCheckSum(_dbuffer);

//    std::cout << std::endl <<  "setFromRawFrame verification buffer: " << std::endl;
//    ShowVectorUINT8(_vctBuffer);
//    std::cout << std::endl;
//    std::cout << "Delimeter : " << std::hex << (unsigned short)Delimeter << std::endl;
//    std::cout << "MSB       : " << std::hex << (unsigned short)MSB << std::endl;
//    std::cout << "LSB       : " << std::hex << (unsigned short)LSB << std::endl;
//    std::cout << "Lenght    : " << std::hex << (unsigned int) Lenght << std::endl;
//    std::cout << "FrameData : " ; ShowVectorUINT8(vctFrameData); std::cout << std::endl;
//    std::cout << "Checksum buffer    : " << std::hex << (unsigned short)_vctBuffer[_vctBuffer.size()-1] << std::endl;
//    std::cout << "calculated checksum: " << std::hex << (unsigned short)CheckSum << std::endl;
//    std::cout << "difference checksums : " << std::hex << (unsigned short)CheckSum - (unsigned short)_vctBuffer[_vctBuffer.size()-1] << std::endl;

    if(CheckSum != _dbuffer->at(_dbuffer->size()-1))
    {
        //throw bad_rf_frame("setFromRawFrame:checksum verification failed");
        //std::cerr << "setFromRawFrame:checksum verification failed" << std::endl;
        return false;
    }

    if(_dbuffer->at(3)==0x97)
    {
#ifdef ZB_DEBUG_FRAME
        std::cout << "CZBFrame: AT remote command response found " << _dbuffer << std::endl;
#endif
    }

    return true;
}

uint8_t CZBFrame::getAPIIdentifierFromRFDataFrame(data_vct_ptr _vctRFDataFrame)
{
    if(_vctRFDataFrame->empty())
    {
        //throw bad_rf_frame("getCommandFromRFDataFrame: _vctDataFrame empty !!!");
	//TODO: log this
        return 0; // TODO: change this !!
    }
    return _vctRFDataFrame->at(0);
}

data_vct_ptr CZBFrame::getcmdDataFromRFDataFrame(data_vct_ptr _vctFrameData)
{
    data_vct_ptr vctRet = data_vct_ptr(new std::vector<boost::uint8_t>());
    vctRet->resize(_vctFrameData->size()-1);
    std::copy(_vctFrameData->begin()+1,_vctFrameData->end(),vctRet->begin());
    return vctRet;
}

data_vct_ptr CZBFrame::get64bAddressFromCmdData( data_vct_ptr _vctCmdData)
{
    data_vct_ptr vctRet = data_vct_ptr(new std::vector<boost::uint8_t>());
    vctRet->assign(_vctCmdData->begin(),_vctCmdData->begin()+8);
    return vctRet;
}

uint8_t CZBFrame::getCheckSum(data_vct_ptr _vctBuffer)
{
    std::vector<uint8_t>::iterator it;
    uint8_t cs; // checksum
    for(it=_vctBuffer->begin()+3;it!=_vctBuffer->end()-1;++it){
        cs += (*it);
    }
    cs = 0xFF - cs;
    return cs;
}

bool CZBFrame::CheckRawFrame(data_vct_ptr _vctBuffer)
{
    if(_vctBuffer->empty())
    {
        return false;
    }

    if(_vctBuffer->at(0) != 0x7E){
        return false;
    }

    if(_vctBuffer->size() <= 4 ) // no data in frame, suppose to not happend ever... throw it ??
    { 
        return false;
    }

    return true;
}



