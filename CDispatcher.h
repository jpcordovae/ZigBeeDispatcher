#ifndef CDISTPATCHER_H
#define CDISTPATCHER_H

#include "common.h"
#include "cenddevice.h"
#include "czbenddevice.h"
#include "cevent.h"
#include "czbframe.h"

#include <vector>
#include <stdint.h>

struct stVariable{
    std::string EndDevice;
    std::string Channel;
    std::string VariableName;
};

class CDispatcher
{
public:

    typedef boost::shared_ptr<CDispatcher> CDispatcherPtr;
    typedef boost::signals2::signal< void (std::vector<std::string>) > signalEDListChanged;

    CDispatcher();

    struct DispatcherRuntimeError : public std::runtime_error
    {
        DispatcherRuntimeError() : std::runtime_error("Dispatcher unable to dispatch message")
        {}
    };

    void AddFrameToTransmit(std::vector<uint8_t> new_frame);
    void processTX(void);
    void AddNewEndDevice(const std::string &name, const std::vector<uint8_t> &vctAddress);
    void addRFFrame(const std::vector<uint8_t> );
    void Function_API_RX_IO_DATA_64bit_ADDRESS( std::vector<uint8_t> );
    void Function_API_AT_REMOTE_COMMAND_RESPONSE(std::vector<uint8_t> _vctRFData);
    void SendCommand(const std::string &_sEDName, const std::vector<uint8_t> _vctCommand, const std::vector<uint8_t> _vctRFData);
    uint8_t calculateChechSumUINT8(std::vector<uint8_t> &_vctBuffer);
    void Function_API_AT_REMOTE_COMMAND_REQUEST(const std::string &_sEDName, const std::vector<uint8_t> _vctCommand, const std::vector<uint8_t> _vctRFData);
    void Function_API_AT_REMOTE_COMMAND_REQUEST(const std::vector<uint8_t> &address, const std::vector<uint8_t> &_vctCommand, const std::vector<uint8_t> &_vctRFData);
    void Function_API_TX_REQUEST_64bit_ADDRESS( std::vector<uint8_t> );
    void ReadSample(std::vector<uint8_t> &, std::vector<uint8_t> &_vctAddress/* ZBED address */);
    void AddDataToZBED( CZBEndDevicePtr zbptr, const std::string & sDataChannel, const uint16_t u16d, const ptime &_ptime);
    void CheckAndAddRawFrames(std::vector<uint8_t> _vctRawBuffer);
    CZBEndDevicePtr getZBEndDeviceByAddress(const std::vector<uint8_t> &_vctAddress);
    CZBEndDevicePtr getZBEndDeviceByName(const std::string &_name);
    std::vector<std::string> getZBEDListNames(void);
    std::vector<uint8_t> GetEndDeviceAddressByName(std::string &_sName);
    boost::signals2::connection setDataCallback(const std::string &_sName, CEndDevice::signalChannel::slot_type &_subscriber);

    // signal connections
    boost::signals2::connection connectEDlistChanged(const signalEDListChanged::slot_type & subscriber)
    {
        return EDListChangedCallBack.connect(subscriber);
    }

    boost::signals2::connection connectEDDataChanged(const std::string &_ed, const CEndDevice::signalChannel::slot_type & subscriber)
    {
        CZBEndDevicePtr czbed = getZBEndDeviceByName(_ed);
        if(czbed.get()==NULL)
        {
            std::cerr << "connectEDDataChanged: end device " << _ed << " not found";
            return boost::signals2::connection();
        }
        return czbed->connectChannelToFunction(subscriber);
    }

private:
    std::map<std::string,CEndDevice> mapModelsEndDevices;
    std::map<std::string,CZBEndDevicePtr> mapEndDevices;
    signalEDListChanged EDListChangedCallBack;
    boost::thread TXThread;
    bool bTXThreadExit;
    //CEvent<int> fail_frames(0,5);
    std::vector<CZBFramePtr> RFFrameBuffer;
    boost::mutex rfbuffer_mutex;
    //CZBConfiguration *Config;
};

typedef CDispatcher::CDispatcherPtr CDispatcherPtr;

extern CDispatcher Dispatcher;

#endif // CDISTPATCHER_H
