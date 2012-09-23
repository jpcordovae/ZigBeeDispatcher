#include <iostream>
#include "CDispatcher.h"
#include "serialport.h"
#include "common.h"
#include "czblog.hpp"

bool bExit=false;
CDispatcher *CD=NULL;

void ShowEndDevices(void)
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

int main(int argv, char *argc[])
{
    LOG.set_log_file("log/log.txt");
    LOG.set_data_file("log/data.txt");
    LOG.set_save_data(true);
    LOG.set_save_log(true);

    SerialPort.SetDevice(argc[1]);
    //CD = new CDispatcher();
    //SerialPort.addDataCallBack(boost::bind(&CDispatcher::CheckAndAddRawFrames,CD,_1));
    //SerialPort.addDataCallBack(boost::bind(&CZBLog::LOGDATA,&LOG,_1));
    //SerialPort.addDataCallBack(LogData);
    //SerialPort.addDataCallBack(&ShowVectorUINT8);
    SerialPort.OpenSerialPort();

    //char line[1024];
    while(!bExit)
    {
        //memset(line,0,1024);
        //std::cout << ">";
        //std::cin.getline(line,1024);
        //std::cout << "command: " << std::string(line) << std::endl;
        //ProcessLine(std::string(line));
        usleep(10000);
    }

    //multithread safe delete :)

    exit(EXIT_SUCCESS);
}


