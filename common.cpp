//
// Juan Pablo Cordova Echeverria 
//
#include <algorithm>
#include <vector>
#include <map>

#include "common.h"

void ShowUINT8(uint8_t _d )
{
    printf("%02x:",_d);
    //std::cout << std::hex << (unsigned short)_d;
}

//void ShowVectorUINT8(std::vector<uint8_t> &_vct)
//{
//    std::for_each(_vct.begin(),_vct.end(),ShowUINT8);
//}

void ShowVectorUINT8(std::vector<uint8_t> _vct)
{
    std::for_each(_vct.begin(),_vct.end(),ShowUINT8);
    std::cout << std::endl;
}

void ShowBufferOfVectorsUINT8( std::vector<std::vector<uint8_t> > &_vct )
{
    printf("Bufferef vector of vectors : \n");
    std::for_each(_vct.begin(),_vct.end(),ShowVectorUINT8);
    //TODO: incomplete implementation !!!
}

std::ostream& operator<<(std::ostream &_output, const std::vector<uint8_t> &_vector)
{
    BOOST_FOREACH(uint8_t ui,_vector){
        _output << std::hex << (unsigned short)ui << ":";
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

boost::uint8_t char2hexvalue(const char _data)
{
    boost::uint8_t ret;
    switch(_data)
    {
    case '0': ret = 0;	break;
    case '1': ret = 1;	break;
    case '2': ret = 2;	break;
    case '3': ret = 3;	break;
    case '4': ret = 4;	break;
    case '5': ret = 5;	break;
    case '6': ret = 6;	break;
    case '7': ret = 7;	break;
    case '8': ret = 8;	break;
    case '9': ret = 9;	break;
    case 'A':
    case 'a': ret = 10;	break;
    case 'B':
    case 'b': ret = 11;	break;
    case 'C':
    case 'c': ret = 12;	break;
    case 'D':
    case 'd': ret = 13;	break;
    case 'E':
    case 'e': ret = 14; break;
    case 'F':
    case 'f': ret = 15;	break;
    default:
	break;	
    }
    return ret;
}

boost::uint8_t byte2unit8_t(const char _data[])
{
    return char2hexvalue(_data[0]) * 16 + char2hexvalue(_data[1]);
}


std::vector<boost::uint8_t> string2hex(std::string _str)
{
    std::vector<boost::uint8_t> ret;
    for(unsigned int k=0;k<_str.length();k++)
    {
	ret.push_back(char2hexvalue(_str[k]));
    }
    //ShowVectorUINT8(ret);
    return ret;
}

