#ifndef __CZBED__COREC_EXT_0001__HPP__
#define __CZBED__COREC_EXT_0001__HPP__

#include "../czbenddevice.h"
#include "../common.h"

class corec_ext_0001 : public CZBEndDevice {
public:
    corec_ext_0001();
    corec_ext_0001(const std::string &_name, const std::string &_address);
};


#endif
