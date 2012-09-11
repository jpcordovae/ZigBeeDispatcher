#ifndef CZBLOG__H
#define CZBLOG__H

#define LOG_MSG_BUFFER_SIZE 2048 // max size of message

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <iostream>
#include <list>

class CZBLog
{
public:
    enum LOG_LEVEL { ERROR_CRITICAL, ERROR_WARNING, WARNING, DATA };

    struct message_unit
    {
	char msg[LOG_MSG_BUFFER_SIZE]; //TODO: check if this make a memory leack
	message_unit()
	{
	    bzero(msg,LOG_MSG_BUFFER_SIZE);
	}
	//message_unit()
	boost::posix_time::ptime timestamp;
    };
    
    typedef boost::shared_ptr<message_unit> message_unit_ptr;
    typedef boost::shared_ptr<std::string> string_ptr;
    
    void log(enum LOG_LEVEL _log_level,message_unit_ptr _msg_ptr){
	
    }

    void set_data_file(std::string _data_file_name)
    {
	if(_data_file.is_open()){
	    _data_file.close();
	}
	_data_file.open(_data_file_name);
	if(!_data_file.is_open()){
	    //throw this
	    std::cerr << "error abriendo archivo de log de datos" << std::endl;
	    _save_data_to_file = false;
	    return;
	}
	_save_data_to_file = true;
	return;
    }

    void set_log_file(std::string _log_file_name){
		if(_log_file.is_open()){
	    _log_file.close();
	}
	_log_file.open(_log_file_name);
	if(!_log_file.is_open()){
	    //throw this
	    std::cerr << "error abriendo archivo de log de datos" << std::endl;
	    _save_log_to_file = false;
	    return;
	}
	_save_log_to_file = true;
	return;
    }
    
    CZBLog(){};
    ~CZBLog(){};
    
private:
    boost::mutex _log_mutex;
    std::list<message_unit_ptr> _message_list;
    bool _save_data_to_file;
    bool _save_log_to_file;
    std::fstream _data_file;
    std::fstream _log_file;
    //boost::posix_time::milliseconds _sleep_time_milliseconds;
};

extern CZBLog LOG;


#endif
