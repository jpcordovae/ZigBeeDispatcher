/*
 * File:   czbenddevice.h
 * Author: jpcordovae
 *
 * Created on 3 de enero de 2009, 07:18 PM
 */

#ifndef _CZBENDDEVICE_H
#define	_CZBENDDEVICE_H

#include <boost/shared_ptr.hpp>


#include "common.h"
#include "cenddevice.h"
#include "DomBase.h"
#include "cevent.h"
#include <pthread.h>
#include <deque>

class CZBEndDevice: public CEndDevice {

public:

    typedef boost::shared_ptr<CZBEndDevice> CZBEndDevicePtr;

    CZBEndDevice();
    CZBEndDevice(const CZBEndDevice& orig);
    CZBEndDevice(const std::string &name, data_vct_ptr _vctAddress);
    virtual ~CZBEndDevice();
    void UpdateConfig(std::string _sItem, uint8_t *_Value, size_t _buffersize);
    std::string GetConfigValue(std::string); // get the value in the configuration
    void SetUpdated(bool _bValue);
    void setRSSI( const uint8_t &_RSSI ) { RSSI = _RSSI; }
    bool IsDataChannelUpdated(std::string _sDataChannel);

    // TX buffer
    void freeFrameID(uint8_t frameID);
    bool searchFrame(data_vct_ptr __vctToSearch);
    bool checkFrameID(uint8_t _frameID);
    uint8_t getReserveFrameID(void);
    void addTXBuffer(uint8_t,stTXFrame);
    void setEnFrameStatus(uint8_t,enFrameStatus);
    void setStTxFrame(uint8_t,stTXFrame);
    data_vct_ptr getFrameIDVector(void);
    stTXFrame get_stTXFrame(size_t);

    // enable / disable
    void ZBEDdisable(void) { enable = false; };
    void ZBEDenable(void) { enable = true; };
    bool IsEnabled(void) { return enable; };

    //  alias
//    void setAlias(std::string &, std::string);
//    std::string getAlias(std::string &__channel);
//    std::string getChannel(std::string &__alias);
//    uint16_t getAliasLastValue(std::string &__alias);

private:
    uint8_t RSSI;
    std::map<std::string, std::string> mapConfig;
    std::map<uint8_t,stTXFrame> mapTXBuffer; // transmition buffer
    std::map<std::string,std::string> mapChannelAlias;
    boost::mutex mutex;
    bool enable;

//    CEvent<> NotResponse;
//    CEvent<> SincronizationLost;

};

typedef CZBEndDevice::CZBEndDevicePtr CZBEndDevicePtr;

#endif	/* _CZBENDDEVICE_H */

