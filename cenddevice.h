/*
 * File:   cenddevice.h
 * Author: jpcordovae
 *
 * Created on 23 de diciembre de 2008, 04:18 AM
 */

#ifndef _CENDDEVICE_H
#define	_CENDDEVICE_H

#include <time.h>
#include <pthread.h>
#include <map>
#include <vector>
#include <string>
#include "common.h"

enum enAddressLenght { _32bits=32, _64bits=64 };

// frame manage
uint8_t *GetAddressFromFrame(uint8_t *ucBuffer);
uint8_t *GetDataFromFrame(uint8_t *ucBuffer);

struct EDDATA {
        uint16_t data;
        ptime ptimestamp;
    };

class CEndDevice {
public:
    CEndDevice();
    CEndDevice(const CEndDevice& orig);
    CEndDevice(const std::string &_name, const std::string &_address);

    virtual ~CEndDevice();

    // definitions
    typedef boost::shared_ptr<CEndDevice> CEndDevicePtr;
    typedef std::vector<EDDATA> Container;
    typedef boost::shared_ptr<std::vector<EDDATA> > ContainerPtr;
    typedef boost::signals2::signal< void (std::string, std::string , EDDATA ) > signalChannel;
    typedef boost::shared_ptr<signalChannel>  signalChannelPtr;

    // properties methods
    void SetName(const std::string &_sName);
    std::string GetName(void);

    // Address
    std::vector<uint8_t> GetAddress(void);
    void SetAddress(const std::vector<uint8_t> &);
    bool CompareAddress(const std::vector<uint8_t> &_vctAddress);
    bool CompareName(const std::string &_sName);

    // channels
    void AddDataChannel(const std::string &_sDataChannel);
    size_t GetDataChannelCount(std::string _sDataChannel);
    std::size_t getChannelCount(void);
    std::vector<std::string> getChannelNames(void);

    //data
    void getSaveData(const bool _bSaveData); // save data ?
    bool getSaveData(void); // it's saving data?
    uint16_t GetLastData(const std::string &_sDataChannel);
    bool AddData(const std::string &_sDataChannel, const uint16_t &, const ptime &);

    // config
    std::map<std::string,ContainerPtr> mapDataContainer;

    // connections
    boost::signals2::connection connectChannelToFunction(const signalChannel::slot_type &_subscriber)
    {
        return DataChangeCallBack.connect(_subscriber);
    }

    void sendSignalDataChange(std::string _edName, std::string _channels, EDDATA _edData);

    virtual void 

private:
    std::string sName; // ordinary name of the device.
    bool bSaveData; // the ED will save the data?
    std::vector<uint8_t> vctAddress;
    enAddressLenght enAL;
    signalChannel DataChangeCallBack;
    boost::mutex mutex;
};

typedef CEndDevice::CEndDevicePtr CEndDevicePtr;
typedef CEndDevice::signalChannelPtr signalChannelPtr;

extern std::list<CEndDevicePtr> *end_devices_list;

#endif	/* _CENDDEVICE_H */
