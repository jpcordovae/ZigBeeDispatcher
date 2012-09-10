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

CZBFrame::CZBFrame(std::vector<uint8_t> _vctBuffer)
{
    setFromRawFrame(_vctBuffer);
}

bool CZBFrame::setFromRawFrame(std::vector<uint8_t> _vctBuffer)
{

    if( ! CheckRawFrame(_vctBuffer) ){
        return false;
    }
    
    Delimeter = _vctBuffer[0];
    MSB = _vctBuffer[1];
    LSB = _vctBuffer[2];

    Lenght = ((short) LSB) + (((short) MSB) << 8); // TODO: check endianes issue

    for (size_t k = 3; k < _vctBuffer.size()-1; k++) { // -1 por que no hay que considerar el checksum
            vctFrameData.push_back(_vctBuffer[k]);
    }

    CheckSum = getCheckSum(_vctBuffer);

    {
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
}

    if(CheckSum != _vctBuffer[_vctBuffer.size()-1]){
        //throw bad_rf_frame("setFromRawFrame:checksum verification failed");
        //std::cerr << "setFromRawFrame:checksum verification failed" << std::endl;
        return false;
    }

    if(_vctBuffer[3]==0x97){
	#ifdef ZB_DEBUG_FRAME
        std::cout << "CZBFrame: AT remote command response found " << _vctBuffer << std::endl;
	#endif
    }

    return true;
}

uint8_t CZBFrame::getAPIIdentifierFromRFDataFrame(const std::vector<uint8_t> _vctRFDataFrame)
{
    if(_vctRFDataFrame.empty())
    {
        //throw bad_rf_frame("getCommandFromRFDataFrame: _vctDataFrame empty !!!");
        return 0; // TODO: change this !!
    }
    return _vctRFDataFrame[0];
}

std::vector<uint8_t> CZBFrame::getcmdDataFromRFDataFrame(const std::vector<uint8_t> _vctFrameData)
{
    std::vector<uint8_t> vctRet;
    vctRet.resize(_vctFrameData.size()-1);
    std::copy(_vctFrameData.begin()+1,_vctFrameData.end(),vctRet.begin());
    return vctRet;
}

std::vector<uint8_t> CZBFrame::get64bAddressFromCmdData( const std::vector<uint8_t> &_vctCmdData)
{
    std::vector<uint8_t> vctRet;
    vctRet.assign(_vctCmdData.begin(),_vctCmdData.begin()+8);
    return vctRet;
}

uint8_t CZBFrame::getCheckSum(std::vector<uint8_t> &_vctBuffer)
{
    std::vector<uint8_t>::iterator it;
    uint8_t cs; // checksum
    for(it=_vctBuffer.begin()+3;it!=_vctBuffer.end()-1;++it){
        cs += (*it);
    }
    cs = 0xFF - cs;
    return cs;
}

bool CZBFrame::CheckRawFrame(std::vector<uint8_t> &_vctBuffer)
{
    if(_vctBuffer.empty()){
        return false;
    }

    if (_vctBuffer[0] != 0x7E){
        return false;
    }

    if( _vctBuffer.size() <= 4 ) { // no data in frame, suppose not happend ever... throw it ??
        return false;
    }

    return true;
}



