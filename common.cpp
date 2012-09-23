
#include <algorithm>
#include <vector>
#include <map>

#include "common.h"

void ShowUINT8(uint8_t _d )
{
    printf("%02x:",_d);
    //std::cout << std::hex << (unsigned short)_d;
}

void ShowVectorUINT8(std::vector<uint8_t> &_vct)
{
    std::for_each(_vct.begin(),_vct.end(),ShowUINT8);
}

void ShowVectorUINT8(data_vct_ptr _vct)
{
    std::for_each(_vct->begin(),_vct->end(),ShowUINT8);
    //std::cout << std::endl;
}

std::ostream& operator<<(std::ostream &_output, std::vector<uint8_t> &_vector)
{
    BOOST_FOREACH(uint8_t ui,_vector)
    {
        _output << std::hex << (unsigned short)ui << ":";
    }
    return _output;
}

std::ostream& operator<<(std::ostream &_output, data_vct_ptr _vector)
{
    std::vector<uint8_t>::iterator it = (*_vector).begin();
    for(;it!=(*_vector).end();++it)
    {
	_output << (unsigned short)(*it);
    }
    return _output;
}

std::ostream& operator<<(std::ostream &_output, data_unit_ptr _data_unit)
{
    std::vector<uint8_t>::iterator it = _data_unit->databuffer_ptr->begin();
    for( ; it != _data_unit->databuffer_ptr->end() ; ++it )
    {
	_output << (unsigned short)(*it);
    }
    return _output;
}

std::vector<std::string> tokenize(const std::string &_line,const char *_delimeter)
{
    std::vector<std::string> Tokens;

    if(_line.empty()) return Tokens;

    std::string str;

    BOOST_FOREACH(char c,_line){
        if(c==*_delimeter)
        {
            Tokens.push_back(str);
            str.clear();
            continue;
        }
        str += c;
    }

    if(!str.empty())
        Tokens.push_back(str);

    return Tokens;
}


