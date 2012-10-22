#ifndef __CZBED__USBWEATHER__HPP__
#define __CZBED__USBWEATHER__HPP__

#include "../czbenddevice.h"
#include "../common.h"

class USBWeather : public CZBEndDevice {
public:
    USBWeather();
    USBWeather(const std::string &_name, const std::string &_address);
};


#endif
