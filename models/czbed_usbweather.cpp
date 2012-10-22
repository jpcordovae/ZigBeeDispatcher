#include "czbed_usbweather.hpp"

USBWeather::USBWeather()
{

}

USBWeather::USBWeather(const std::string &_name, const std::string &_address)
{
    ::CEndDevice(_name,_address);
    //SetName(_name);
    //SetAddress(string2hex(_address));
}
