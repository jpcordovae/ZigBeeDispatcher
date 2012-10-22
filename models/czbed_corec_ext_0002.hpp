#ifndef __CZBED__COREC_EXT_0002__HPP__
#define __CZBED__COREC_EXT_0002__HPP__

#include "../czbenddevice.h"
#include "../common.h"

class corec_ext_0002 : public CZBEndDevice {
public:
    corec_ext_0002();
    corec_ext_0002(const std::string &_name, const std::string &_address);
};


#endif
