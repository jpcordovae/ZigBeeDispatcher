#include "czbed_corec_ext_0002.hpp"

corec_ext_0002::corec_ext_0002()
{

}

corec_ext_0002::corec_ext_0002(const std::string &_name, const std::string &_address)
{
    ::CEndDevice(_name,_address);
    //SetName(_name);
    //SetAddress(string2hex(_address));
}
