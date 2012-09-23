#ifndef CDISTPATCHER_H
#define CDISTPATCHER_H

#include "common.h"
#include "cenddevice.h"
#include "czbenddevice.h"
#include "cevent.h"
#include "czbframe.h"
#include "czbconfiguration.h"

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

    void AddFrameToTransmit(data_vct_ptr new_frame);
    void processTX(void);
    void AddNewEndDevice(const std::string &name, data_vct_ptr vctAddress);
    void addRFFrame(data_vct_ptr);
    void Function_API_RX_IO_DATA_64bit_ADDRESS( data_vct_ptr );
    void Function_API_AT_REMOTE_COMMAND_RESPONSE(data_vct_ptr _vctRFData);
    void SendCommand(const std::string &_sEDName, data_vct_ptr _vctCommand, data_vct_ptr _vctRFData);
    uint8_t calculateChechSumUINT8(data_vct_ptr _vctBuffer);
    void Function_API_AT_REMOTE_COMMAND_REQUEST(const std::string &_sEDName, data_vct_ptr _vctCommand, data_vct_ptr _vctRFData);
    void Function_API_AT_REMOTE_COMMAND_REQUEST(data_vct_ptr address, data_vct_ptr _vctCommand, data_vct_ptr _vctRFData);
    void Function_API_TX_REQUEST_64bit_ADDRESS( data_vct_ptr );
    void ReadSample(data_vct_ptr, data_vct_ptr _vctAddress/* ZBED address */);
    void AddDataToZBED( CZBEndDevicePtr zbptr, const std::string & sDataChannel, const uint16_t u16d, const ptime &_ptime);
    void CheckAndAddRawFrames(data_vct_ptr _vctRawBuffer);
    CZBEndDevicePtr getZBEndDeviceByAddress(data_vct_ptr _vctAddress);
    CZBEndDevicePtr getZBEndDeviceByName(const std::string &_name);
    std::vector<std::string> getZBEDListNames(void);
    data_vct_ptr GetEndDeviceAddressByName(std::string &_sName);
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
    CZBConfiguration *Config;
};

typedef CDispatcher::CDispatcherPtr CDispatcherPtr;

extern CDispatcher Dispatcher;

#endif // CDISTPATCHER_H
