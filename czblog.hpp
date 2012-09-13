#ifndef __CZBLOG__H__
#define __CZBLOG__H__

#define LOG_MSG_BUFFER_SIZE 2048 // max size of message

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

class CZBLog
{
    
public:
    enum LOG_LEVEL { ERROR_CRITICAL, ERROR_WARNING, WARNING, DATA , INFO };

    struct message_unit
    {
	boost::uint8_t msg[LOG_MSG_BUFFER_SIZE]; //TODO: check if this make a memory leack
	
	message_unit()
	{
	    bzero(msg,LOG_MSG_BUFFER_SIZE);
	}
	
	void operator <<(const char *)
	{
	    //
	}
	
	boost::posix_time::ptime timestamp;
    };
    
    typedef boost::shared_ptr<message_unit> message_unit_ptr;
    typedef boost::shared_ptr<std::string> string_ptr;
    
    void log(enum LOG_LEVEL _log_level,message_unit_ptr _msg_ptr)
    {
	switch(_log_level)
	{
	case DATA:
	    {
		{
		    boost::mutex::scoped_lock k(_config_mutex);
		    if( !_save_data_to_file || !_save_data ) return;
		}
		boost::mutex::scoped_lock l(_data_mutex);
		_data_queue.push(_msg_ptr);
	    }
	    break;
	default:
	    {
		//{
		    boost::mutex::scoped_lock m(_config_mutex);
		    if( !_save_log_to_file || !_save_log ) return;
		    //}
		boost::mutex::scoped_lock n(_data_mutex);
		_log_queue.push(_msg_ptr);
	    }
	    break;
	}
    }

    void set_data_file(std::string _data_file_name)
    {
	boost::mutex::scoped_lock l(_data_mutex);

	if(_data_file.is_open())
	{
	    _data_file.close();
	}

	_data_file.open(_data_file_name,std::fstream::in | std::fstream::out | std::fstream::app);

	if(!_data_file.is_open())
	{
	    //throw this
	    std::cerr << "error abriendo archivo de log de datos" << std::endl;
	    boost::mutex::scoped_lock m(_config_mutex);
	    _save_data_to_file = false;
	    return;
	}
	
	boost::mutex::scoped_lock n(_config_mutex);

	_save_data_to_file = true;

	return;
    }

    void set_log_file(std::string _log_file_name)
    {
	boost::mutex::scoped_lock l(_log_mutex);
	
	if(_log_file.is_open())
	{
	    _log_file.close();
	}
	
	_log_file.open(_log_file_name,std::fstream::in | std::fstream::out | std::fstream::app);
	
	if(!_log_file.is_open())
	{
	    //throw this
	    std::cerr << "error abriendo archivo de log de eventos" << std::endl;
	    boost::mutex::scoped_lock m(_config_mutex);
	    _save_log_to_file = false;
	    return;
	}
	
	boost::mutex::scoped_lock n(_config_mutex);
	
	_save_log_to_file = true;
	
	return;
    }

    void set_save_data(bool _b)
    {
	boost::mutex::scoped_lock l(_config_mutex);
	_save_data = _b;
    }

    void set_save_log(bool _b)
    {
	boost::mutex::scoped_lock l(_config_mutex);
	_save_log = _b;
    }

    void threaded_method(void)
    {
	while(!_thread_exit)
	{
	    boost::mutex::scoped_lock l(_config_mutex);
	    if(_save_log && _save_log_to_file)
	    {
		boost::mutex::scoped_lock k(_log_mutex);
		if(_log_queue.empty()) continue;
		_log_file << _log_queue.front()->msg << std::endl;
		_log_queue.pop();
	    }
	    
	    if(_save_data && _save_data_to_file)
	    {
		boost::mutex::scoped_lock k(_data_mutex);
		if(_data_queue.empty()) continue;
		_data_file << _data_queue.front()->msg << std::endl;
		_data_queue.pop();
	    }
	    
	    usleep(10000);
	}
    }
    
    CZBLog():_thread_exit(false)
    {
	_log_thread = boost::thread(boost::bind(&CZBLog::threaded_method,this));
	
    };
    
    ~CZBLog()
    {
	//this = NULL; // safe delete, must be done outside of this class
	boost::mutex::scoped_lock l(_log_mutex);
	boost::mutex::scoped_lock k(_data_mutex);
	boost::mutex::scoped_lock m(_config_mutex);
	
	_save_data = false;
	_save_log = false;
	_thread_exit = true;

	usleep(10000); // give time to finish some pending job
	
	_log_thread.interrupt(); // just in case
	
	if(_data_file.is_open()) _data_file.close(); // this flush buffer and close file
	if(_log_file.is_open()) _log_file.close(); // flush buffer and close file
	
	delete this; // good bye !!
    };

    void LOGDATA(std::vector<boost::uint8_t> _data)
    {
	CZBLog::message_unit_ptr msg_ptr( new CZBLog::message_unit() );
	std::string sTmp;
	unsigned int k = 0;
	while( _data.size() > k )
	{
	    try{
	    sTmp += boost::lexical_cast<std::string>(_data[k++]);
	    }catch(std::exception &e)
	    {
		std::cerr << e.what() << std::endl;
		continue;
	    }
	}
	memcpy(msg_ptr->msg,sTmp.c_str(),_data.size());// this can be done because the vector store the data in a contiguous memory area
	log(LOG_LEVEL::DATA,msg_ptr);
    }

    void ERROR_WARNING_LOG(char *_msg)
    {
	message_unit_ptr msg_ptr( new message_unit() );
	memcpy(msg_ptr->msg,_msg,strlen(_msg));
	log(LOG_LEVEL::ERROR_WARNING,msg_ptr);
    }

    void WARNING_LOG(char *_msg)
    {
	message_unit_ptr msg_ptr( new message_unit() );
	memcpy(msg_ptr->msg,_msg,strlen(_msg));
	log(LOG_LEVEL::WARNING,msg_ptr);
    }

    void INFO_LOG(char *_msg)
    {
	message_unit_ptr msg_ptr( new message_unit() );
	memcpy(msg_ptr->msg,_msg,strlen(_msg));
	log(LOG_LEVEL::INFO,msg_ptr);
    }
    
private:
    boost::thread _log_thread;
    boost::mutex _log_mutex;
    boost::mutex _data_mutex;
    boost::mutex _config_mutex; // to change the configuration on the fly
    std::queue<message_unit_ptr> _data_queue;
    std::queue<message_unit_ptr> _log_queue;
    bool _save_data_to_file;
    bool _save_log_to_file;
    bool _save_data;
    bool _save_log;
    std::fstream _data_file;
    std::fstream _log_file;
    bool _thread_exit;
    //boost::posix_time::milliseconds _sleep_time_milliseconds;

};

extern CZBLog *LOG;


#endif
