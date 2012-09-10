
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

std::vector<std::string> tokenize(const std::string &_line,const char *_delimeter){
	std::vector<std::string> Tokens;
	
	if(_line.empty()) return Tokens;
	
	std::string str;

	BOOST_FOREACH(char c,_line)
	{
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


