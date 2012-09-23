#include "czblog.hpp"

CZBLog LOG;

void CZBLog::log(enum LOG_LEVEL _log_level,message_unit_ptr _msg_ptr)
{
    switch(_log_level)
    {
	/*case DATA:
    {
        {
            boost::mutex::scoped_lock k(_config_mutex);
            if( !_save_data_to_file || !_save_data ) return;
        }
        boost::mutex::scoped_lock l(_data_mutex);
        _data_queue.push(_msg_ptr);
    }
    break;*/
    default:
    {
        {
            boost::mutex::scoped_lock m(_config_mutex);
            if( !_save_log_to_file || !_save_log ) return;
        }
        boost::mutex::scoped_lock n(_data_mutex);
        _log_queue.push(_msg_ptr);
    }
    break;
    }
}

void CZBLog::set_data_file(std::string _data_file_name)
{
    boost::mutex::scoped_lock l(_data_mutex);

    if(_data_file.is_open())
    {
        _data_file.close();
    }

    _data_file.open(_data_file_name,std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary );

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

void CZBLog::set_log_file(std::string _log_file_name)
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

void CZBLog::set_save_data(bool _b)
{
    boost::mutex::scoped_lock l(_config_mutex);
    _save_data = _b;
}

void CZBLog::set_save_log(bool _b)
{
    boost::mutex::scoped_lock l(_config_mutex);
    _save_log = _b;
}

void CZBLog::threaded_method(void)
{
    while(!_thread_exit)
    {
        boost::mutex::scoped_lock l(_config_mutex);
        if(_save_log && _save_log_to_file)
        {
            boost::mutex::scoped_lock k(_log_mutex);
            if(_log_queue.empty())
	    {
		continue;
	    }
            _log_file << _log_queue.front()->msg << std::endl;
            _log_queue.pop();
        }

        if(_save_data && _save_data_to_file)
        {
            boost::mutex::scoped_lock k(_data_mutex);
            if(_data_queue.empty())
	    {
		continue;
	    }
            _data_file << _data_queue.front()->databuffer_ptr << std::endl;
            _data_queue.pop();
        }
        usleep(10000);
    }
}


CZBLog::CZBLog():_thread_exit(false)
{
    _log_thread = boost::thread(boost::bind(&CZBLog::threaded_method,this));

};
CZBLog::~CZBLog()
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
}

void CZBLog::LOGDATA(data_vct_ptr _data)
{
    std::vector<uint8_t>::iterator it = _data->begin();
    for(;it!=_data->end();++it)
    {
	std::cout << (unsigned short)(*it);
    }
    data_unit_ptr du(new data_unit());
    du->databuffer_ptr = _data;
    {
	boost::mutex::scoped_lock k(_config_mutex);
	if( !_save_data_to_file || !_save_data ) return;
    }
    boost::mutex::scoped_lock l(_data_mutex);
    _data_queue.push(du);
    
    //printf("HERE !!");
    //for( ssize_t k = 0; k < _data->size() ; k++ )
    //{
    //    printf( "%02x:" , _data->at(k) );
    //}
    //log(LOG_LEVEL::DATA,msg_ptr);
}

void CZBLog::ERROR_WARNING_LOG(char *_msg)
{
    message_unit_ptr msg_ptr( new message_unit() );
    memcpy(msg_ptr->msg,_msg,strlen(_msg));
    log(LOG_LEVEL::ERROR_WARNING,msg_ptr);
}

void CZBLog::WARNING_LOG(char *_msg)
{
    message_unit_ptr msg_ptr( new message_unit() );
    memcpy(msg_ptr->msg,_msg,strlen(_msg));
    log(LOG_LEVEL::WARNING,msg_ptr);
}

void CZBLog::INFO_LOG(char *_msg)
{
    message_unit_ptr msg_ptr( new message_unit() );
    memcpy(msg_ptr->msg,_msg,strlen(_msg));
    log(LOG_LEVEL::INFO,msg_ptr);
}
