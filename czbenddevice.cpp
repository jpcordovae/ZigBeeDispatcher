/*
 * File:   czbenddevice.cpp
 * Author: jpcordovae
 *
 * Created on 3 de enero de 2009, 07:18 PM
 */

#include "czbenddevice.h"
#include <iostream>
#include <ctime>

CZBEndDevice::CZBEndDevice()
{
    /* Special */
    mapConfig[C_WR] = ""; // C_WR	"WR" // Write
    mapConfig[C_RE] = ""; // C_RE	"RE" // Restore Defaults
    mapConfig[C_FR] = ""; // C_FR	"FR" // Software Reset
    /* Networking & secutiry */
    mapConfig[C_CH] = ""; // C_CH	"CH" // Channel
    mapConfig[C_ID] = ""; // C_ID	"ID" // PAN ID
    mapConfig[C_DH] = ""; // C_DH	"DH" // Destination Address High
    mapConfig[C_DL] = ""; // C_DL	"DL" // Destination Address Low
    mapConfig[C_MY] = ""; // C_MY	"MY" // 16-bit Source Address
    mapConfig[C_SH] = ""; // C_SH	"SH" // Serial Number High
    mapConfig[C_SL] = ""; // C_SL	"SL" // Serial Number Low
    mapConfig[C_RR] = ""; // C_RR	"RR" // XBee Retries
    mapConfig[C_RN] = ""; // C_RN	"RN" // Random Delay Slots
    mapConfig[C_MM] = ""; // C_MM	"MM" // MAC Mode
    mapConfig[C_NI] = ""; // C_NI	"NI" // Node Identifier
    mapConfig[C_ND] = ""; // C_ND	"ND" // Node Discover
    mapConfig[C_NT] = ""; // C_NT	"NT" // Node Discover Time
    mapConfig[C_DN] = ""; // C_DN	"DN" // Destination Node
    mapConfig[C_CE] = ""; // C_CE	"CE" // Coordinator Enable
    mapConfig[C_SC] = ""; // C_SC	"SC" // Scan Channels
    mapConfig[C_SD] = ""; // C_SD	"SD" // Scan Duration
    mapConfig[C_A1] = ""; // C_A1	"A1" // End Device Association
    mapConfig[C_A2] = ""; // C_A2	"A2" // Coordinator Association
    mapConfig[C_AI] = ""; // C_AI	"AI" // Association Indication
    mapConfig[C_DA] = ""; // C_DA	"DA" // Force Disassociation
    mapConfig[C_FP] = ""; // C_FP	"FP" // Force Poll
    mapConfig[C_AS] = ""; // C_AS	"AS" // Active Scan
    mapConfig[C_ED] = ""; // C_ED	"ED" // Energy Scan
    mapConfig[C_EE] = ""; // C_EE	"EE" // AES Encryption Enable
    mapConfig[C_KY] = ""; // C_KY	"KY" // AES Encryption Key
    /* RF Interfacing */
    mapConfig[C_PL] = ""; // C_PL	"PL" // Power Level
    mapConfig[C_CA] = ""; // C_CA	"CA" // CCA Threshold
    mapConfig[C_SM] = ""; // C_SM	"SM" // Sleep
    mapConfig[C_ST] = ""; // C_ST	"ST" // Time Before Sleep
    mapConfig[C_SP] = ""; // C_SP	"SP" // Cyclic Sleep Period
    mapConfig[C_DP] = ""; // C_DP	"DP" // Dissasociated Cyclic Sleep Period
    /* Serial Interfacing */
    mapConfig[C_BD] = ""; // C_BD	"BD" // Interface Data Rate
    mapConfig[C_RO] = ""; // C_RO	"RO" // Paquetization Timeout
    mapConfig[C_AP] = ""; // C_AP	"AP" // API Enable
    mapConfig[C_NB] = ""; // C_NB	"NB" // Parity
    mapConfig[C_PR] = ""; // C_PR	"PR" // Pull-Up Resistor
    /* I/O Settings */
    mapConfig[C_D8] = ""; // C_D8	"D8" // DI8 Conf.
    mapConfig[C_D7] = ""; // C_D7	"D7" // DIO7 Conf.
    mapConfig[C_D6] = ""; // C_D6	"D6" // DIO6 Conf.
    mapConfig[C_D5] = ""; // C_D5	"D5" // DIO5 Conf.
    mapConfig[C_D4] = ""; // C_D4	"D4" // DIO4 Conf
    mapConfig[C_D3] = ""; // C_D3	"D3" // DIO3 Conf.
    mapConfig[C_D2] = ""; // C_D2	"D2" // DIO2 Conf.
    mapConfig[C_D1] = ""; // C_D1	"D1" // DIO1 Conf.
    mapConfig[C_D0] = ""; // C_D0	"D0" // DIO0 Conf.
    mapConfig[C_IU] = ""; // C_IU	"IU" // I/O Output Enable
    mapConfig[C_IT] = ""; // C_IT	"IT" // Samples Before TX
    mapConfig[C_IS] = ""; // C_IS	"IS" // Force Sample
    mapConfig[C_IO] = ""; // C_IO	"IO" // Digital Output Level
    mapConfig[C_IC] = ""; // C_IC	"IC" // DIO Change Detect
    mapConfig[C_IR] = ""; // C_IR	"IR" // Sample Rate
    mapConfig[C_AV] = ""; // C_AV	"AV" // ADC Voltage Reference
    mapConfig[C_IA] = ""; // C_IA	"IA" // I/O Input Address
    mapConfig[C_T0] = ""; // C_T0	"T0" // D0 Output timeout
    mapConfig[C_T1] = ""; // C_T1	"T1" // D1 Output timeout
    mapConfig[C_T2] = ""; // C_T2	"T2" // D2 Output timeout
    mapConfig[C_T3] = ""; // C_T3	"T3" // D3 Output timeout
    mapConfig[C_T4] = ""; // C_T4	"T4" // D4 Output timeout
    mapConfig[C_T5] = ""; // C_T5	"T5" // D5 Output timeout
    mapConfig[C_T6] = ""; // C_T6	"T6" // D6 Output timeout
    mapConfig[C_T7] = ""; // C_T7	"T7" // D7 Output timeout
    mapConfig[C_P0] = ""; // C_P0	"P0" // PWM0 Conf.
    mapConfig[C_P1] = ""; // C_P1	"P1" // PWM1 Conf.
    mapConfig[C_M0] = ""; // C_M0	"M0" // PWM0 Output Level
    mapConfig[C_M1] = ""; // C_M1	"M1" // PWM1 Output Level
    mapConfig[C_PT] = ""; // C_PT	"PT" // PWM Output Timeout
    mapConfig[C_RP] = ""; // C_RP	"RP" // RSSI PWM Timer
    /* Diagnostic */
    mapConfig[C_VR] = ""; // C_VR	"VR" // Firmware Version
    //mapUpdate[C_VL] = false; // C_VL	"VL" // Firmware Version - Verbose ----> DEPRECATED since firmware version 10C8
    mapConfig[C_HV] = ""; // C_HV	"HV" // Hardware Version
    mapConfig[C_DB] = ""; // C_DB	"DB" // Received Signal Strength
    mapConfig[C_EC] = ""; // C_EC	"EC" // CCA Failure
    mapConfig[C_EA] = ""; // C_EA	"EA" // ACK Failures
    mapConfig[C_ED] = ""; // C_ED	"ED" // Energy Scan
    /* AT Command Options */
    mapConfig[C_CT] = ""; // C_CT	"CT" // Command Mode Timeout
    mapConfig[C_CN] = ""; // C_CN	"CN" // Exit Command Mode
    mapConfig[C_AC] = ""; // C_AC	"AC" // Apply Changes
    mapConfig[C_GT] = ""; // C_GT	"GT" // Guard Times
    mapConfig[C_CC] = ""; // C_CC	"CC" // Command Sequence Character

}

CZBEndDevice::CZBEndDevice(const std::string &name, const std::vector<uint8_t> &_vctAddress)
{
    CZBEndDevice();
    SetName(name);
    SetAddress(_vctAddress);
}

CZBEndDevice::CZBEndDevice(const CZBEndDevice& orig) : CEndDevice(orig)
{

}

CZBEndDevice::~CZBEndDevice()
{
    //pthread_mutex_destroy(&tx_mutex);
}

void CZBEndDevice::UpdateConfig(std::string _sItem, uint8_t *_ucValue, size_t _buffersize)
{
    //NOTE: the data is saves in ASCII heximal representation, this for compatibility with easy TCP/IP stream data... easy to program the client later
    if (mapConfig.find(_sItem) == mapConfig.end())
    {
            printf("UpdateConfig: error trying to found command %s in mapConfig\n",_sItem.c_str());
            return;
    }

    char cTmp[200];
    bzero(cTmp,200);
    mapConfig[_sItem] = "";

    for(size_t k=0;k<_buffersize;k++)
    {
            sprintf(cTmp,"%02x",_ucValue[k]);
    }

    mapConfig[_sItem] = std::string(cTmp);

}

std::string CZBEndDevice::GetConfigValue(std::string _sItem)
{    
    std::map<std::string,bool>::iterator it;

    if(mapConfig.find(_sItem)==mapConfig.end())
    {
            printf("CZBEndDevice::GetConfigValue: _sItem %s not found in mapConfig\n",_sItem.c_str());
            return std::string("error");
    }

    return mapConfig[_sItem];
}

void CZBEndDevice::freeFrameID(uint8_t frameID)
{
    boost::mutex::scoped_lock scoped_lock(mutex);
    mapTXBuffer.erase(frameID);
}

// return true if found a frame with the same address, command and data
bool CZBEndDevice::searchFrame(std::vector<uint8_t> __vctToSearch)
{
    boost::mutex::scoped_lock scoped_lock(mutex);
    typedef std::map<uint8_t,stTXFrame> mp;
    BOOST_FOREACH(const mp::value_type &pair, mapTXBuffer){
        stTXFrame txf = pair.second;
        std::vector<uint8_t> vctTmp;
        vctTmp.assign(txf.vctBuffer.begin()+5,txf.vctBuffer.end()+18);
        if(vctTmp==__vctToSearch){
            //TODO: log this
            return true;
        }
    }
    return false;
}

// return true if found a frame ID key inside mapTXBuffer
bool CZBEndDevice::checkFrameID(uint8_t _frameID){
    boost::mutex::scoped_lock scoped_lock(mutex);
    if(mapTXBuffer.find(_frameID)==mapTXBuffer.end()){
        return false;
    }
    return true;
}

uint8_t CZBEndDevice::getReserveFrameID(void)
{
    uint8_t uiMAX = uint8_t(0)-uint8_t(1); // this should give us 0xFF, TODO: check this with gdb
    boost::mutex::scoped_lock scoped_lock(mutex);

    if(mapTXBuffer.size()>=uiMAX){
        //throw CZBEndDeviceRuntimeError("getReserveFrameID:: max size reached");
        return 0;
    }
    // carefull using frame ID with value of '0', that means NO RETURN FRAME ACK... that's bad for us
    for( uint8_t ui=1 ; ui < uiMAX ; ui++ ){
        if(mapTXBuffer.find(ui)==mapTXBuffer.end())
        {
            stTXFrame stxf;
            stxf.enFS = DO_NOTHING;
            mapTXBuffer[ui] = stxf;
            return ui;
        }
    }
    return 0;
}

void CZBEndDevice::addTXBuffer(uint8_t _uc, stTXFrame _txf)
{
    //
    boost::mutex::scoped_lock scoped_lock(mutex);
    mapTXBuffer[_uc] = _txf;
}

void CZBEndDevice::setEnFrameStatus(uint8_t __key,enFrameStatus __enFS)
{
    boost::mutex::scoped_lock scoped_lock(mutex);
    std::map<uint8_t,stTXFrame>::iterator it;
    for(it=mapTXBuffer.begin();it!=mapTXBuffer.end();++it)
    {
        if(it->first!=__key){
            std::cerr << "setEnFrameStatus: __key " << (unsigned short)__key << " with __enFS value to set " << __enFS << " (first = " << (unsigned short) it->first << " )" << std::endl;
            continue;
        }
        stTXFrame txf = it->second;
        txf.enFS = __enFS;
        mapTXBuffer[__key] = txf;
        return;
    }
    std::cerr << "setEnFrameStatus: __key value of mapTXBuffer not found...   wtf !! ???";
}

void CZBEndDevice::setStTxFrame(uint8_t __key,stTXFrame __txf)
{
    boost::mutex::scoped_lock scoped_lock(mutex);
    mapTXBuffer[__key] = __txf;
}

std::vector<uint8_t> CZBEndDevice::getFrameIDVector(void)
{
    std::vector<uint8_t> vct;
    std::map<uint8_t,stTXFrame>::iterator it;
    boost::mutex::scoped_lock scoped_lock(mutex);
    for(it=mapTXBuffer.begin();it!=mapTXBuffer.end();++it)
    {
        vct.push_back(it->first);
    }

    return vct;
}

stTXFrame CZBEndDevice::get_stTXFrame(size_t __frameID)
{
    stTXFrame stxf;
    boost::mutex::scoped_lock scoped_lock(mutex);
//    if(!checkFrameID(__frameID))
//    {
//        std::cerr << "get_stTXFrame: frame not found";
//        return stxf;
//    }
    return mapTXBuffer[__frameID];
}
