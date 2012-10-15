#include <iostream>
//#include "CDispatcher.h"
//#include "serialport.h"
#include "common.h"
#include "czblog.hpp"
#include "czbdb.hpp"
#include "cenddevice.h"
#include "czbenddevice.h"
#include "czbed_usbweather.hpp"
#include "czbed_corec_ext_0001.hpp"
#include "czbed_corec_ext_0002.hpp"

bool bExit=false;
//CDispatcher *CD=NULL;
extern CZBDataBase *DB;
extern std::list<CEndDevicePtr> *end_devices_list;

void LoadEndDevicesFromDB(void)
{
    sql::ResultSet *rs = DB->getSQLQuery("SELECT * FROM end_devices");
    while(rs->next())
    {
	std::string name = rs->getString("name");
	std::string address = rs->getString("address");
	int model = rs->getInt("model");
	std::string description = rs->getString("description");
	std::cout << name << ", " << model << ", " << address << ", " << description << std::endl;
	CEndDevicePtr edptr;
	
	switch(model)
	{
	case 1: // USB-Weather-v3
	    edptr = CEndDevicePtr(new USBWeather(name,address));
	    break;
	case 2: // COREC_EXT_0001
	    edptr = CEndDevicePtr(new corec_ext_0001(name,address));
	    break;
	case 3: // COREC_EXT_0002
	    edptr = CEndDevicePtr(new corec_ext_0002(name,address));
	    break;
	case 4: // COREC_INT_0001
	    //edptr = CEndDevicePtr(new corec_int_0001(name,address));
	    break;
	case 5: // COREC_INT_0002
	    //edptr = CEndDevicePtr(new corec_int_0002(name,address));
	    break;
	}
	// get variables from model and setup end device
    }
}

int main(void)
{
    DB = new CZBDataBase();
    DB->set_hostname("localhost");
    DB->set_username("telemetria");
    DB->set_password("telemetria.");
    DB->set_database("corec_telemetry"); // for schema
    DB->connect();
    std::cout << "connected to database corec_telemetry" << std::endl;

    LoadEndDevicesFromDB();
    //LOG = new CZBLog();
    //LOG.set_log_file("log/log.txt");
    //LOG.set_data_file("log/data.txt");
    //LOG.set_save_data(true);
    //LOG.set_save_log(true);
    
    //std::list<string_ptr> address_list = DB->get_end_devices_address();
    //std::list<string_ptr>::iterator it = address_list.begin();

    //for(;it!=address_list.end();++it){
	//std::cout << (*it)->c_str() << std::endl;
    //}
    
    //SerialPort.SetDevice("/dev/ttyUSB0");
    //CD = new CDispatcher();
    //SerialPort.addDataCallBack(boost::bind(&CDispatcher::CheckAndAddRawFrames,CD,_1));
    //SerialPort.addDataCallBack(boost::bind(&CZBLog::LOGDATA,&LOG,_1));
    //SerialPort.addDataCallBack(boost::bind(ShowVectorUINT8,_1));
    //SerialPort.OpenSerialPort();
    //char line[1024];
    
    while(!bExit)
    {
        usleep(10000);
    }
    
    //multithread safe delete :)
    
    exit(EXIT_SUCCESS);
    
}


/*void ShowEndDevices(void)
{
    if(!CD) return;
    std::cout << "EndDevice List" << std::endl;
    std::vector<std::string> vctED= CD->getZBEDListNames();
    BOOST_FOREACH(std::string str,vctED){
        std::cout << str << std::endl;
    }
    std::cout << std::endl;
}

void ShowChannels(void)
{
    std::vector<std::string> edNameList = CD->getZBEDListNames();
    BOOST_FOREACH(std::string str, edNameList){
        CZBEndDevicePtr czbed = CD->getZBEndDeviceByName(str);
        std::vector<std::string> channels = czbed->getChannelNames();
        BOOST_FOREACH(std::string str_channel,channels){
            std::cout << str << ":" << str_channel << std::endl;
        }
    }
}

void ProcessLine(const std::string &_line)
{
    std::vector<std::string> token = tokenize(_line," ");
    if(token.empty()) return;
    if(token.size()>1)
    {
        std::cout << "tokenized command detected:" << std::endl;
        BOOST_FOREACH(std::string str, token){
            std::cout << str << std::endl;
        }
    }
    if(token[0].compare("exit")==0)     bExit=true;
    if(token[0].compare("show_ed")==0)  ShowEndDevices();
    if(token[0].compare("show_channels")==0) ShowChannels();
}
*/


