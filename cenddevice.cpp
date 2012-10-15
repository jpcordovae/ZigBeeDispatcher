/*
 * File:   cenddevice.cpp
 * Author: jpcordovae
 *
 * Created on 23 de diciembre de 2008, 04:18 AM
 */

#include <vector>
#include <map>
#include <iostream>
#include "cenddevice.h"

std::list<CEndDevicePtr> *end_devices_list;

CEndDevice::CEndDevice() {
    //memset(ucAddress, 0, 8);
}

CEndDevice::CEndDevice(const CEndDevice& orig) {
}

CEndDevice::~CEndDevice( ){
}

void CEndDevice::SetName(const std::string &_sName) {
    sName = _sName;
}

CEndDevice::CEndDevice(const std::string &_name, const std::string &_address)
{
    sName = _name;
    SetAddress(string2hex(_address));
}

std::string CEndDevice::GetName(void) {
    return sName;
}

void CEndDevice::AddDataChannel(const std::string &_sDataChannel) {
    mapDataContainer.insert(std::pair<std::string,ContainerPtr>(_sDataChannel,ContainerPtr(new std::vector<EDDATA>)));
    std::cout << "adding channel " << _sDataChannel << std::endl;
}

size_t CEndDevice::GetDataChannelCount(std::string _sDataChannel)
{
    return mapDataContainer[_sDataChannel]->size();
}

//void *CEndDevice::GetDataValue(std::string _sDataChannel, size_t _stPosition) {
////    std::vector<void *> *vTmp = this->map_DataContainer[_sDataChannel];
////    return vTmp->at(_stPosition);
//}

size_t CEndDevice::getChannelCount(void)
{
    return mapDataContainer.size();
}

std::vector<std::string> CEndDevice::getChannelNames(void)
{
    std::vector<std::string> vctNames;
    std::map<std::string,ContainerPtr >::iterator it;

    for(it=mapDataContainer.begin();it!=mapDataContainer.end();++it)
    {
        vctNames.push_back(it->first);
    }

    return vctNames;
}

void CEndDevice::getSaveData(const bool _bSaveData) { // save data ?
    bSaveData = _bSaveData;
}

bool CEndDevice::getSaveData(void) { // it's saving data?
    return bSaveData;
}

uint16_t CEndDevice::GetLastData(const std::string &_sDataChannel) {
    
    std::map<std::string,ContainerPtr>::iterator it;
    it = mapDataContainer.find(_sDataChannel);

    if( it==mapDataContainer.end() || mapDataContainer.empty() )
        return 0x0000;

    ContainerPtr cptr = it->second;

    if(cptr->empty())
        return 0x0000;

    return cptr->at(cptr->size()-1).data;
}

bool CEndDevice::AddData(const std::string &_sDataChannel, const uint16_t &_data, const ptime &_ptime)
{
    std::map<std::string,ContainerPtr>::iterator it;
    it = mapDataContainer.find( _sDataChannel );

    if( it == mapDataContainer.end() )
        return false;

    ContainerPtr cptr = it->second;

    EDDATA Data;
    Data.data = _data;
    Data.ptimestamp = _ptime;

    if( GetLastData(_sDataChannel) != _data )
    {
        DataChangeCallBack(sName,_sDataChannel,Data);
    }

    cptr->push_back(Data);

    return true;
}

std::vector<uint8_t> CEndDevice::GetAddress(void) {
    return vctAddress;
}

void CEndDevice::SetAddress(const std::vector<uint8_t> &_vctAddress)
{
    vctAddress.clear();
    vctAddress.assign(_vctAddress.begin(),_vctAddress.end());
}

bool CEndDevice::CompareAddress(const std::vector<uint8_t> &_vctAddress)
{
    return vctAddress==_vctAddress;
}

bool CEndDevice::CompareName(const std::string &_sName) {
    if (sName.compare(_sName) == 0)
        return true;
    return false;
}

//void CEndDevice::sendSignalDataChange(std::string _edName, std::string _channels, EDDATA _edData)
//{
    // 
//}
