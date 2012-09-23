#ifndef __CZBLOG__H__
#define __CZBLOG__H__

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
#include "common.h"

class CZBLog
{
    
public:
    enum LOG_LEVEL { ERROR_CRITICAL, ERROR_WARNING, WARNING, DATA , INFO };
    
    void log(enum LOG_LEVEL _log_level,message_unit_ptr _msg_ptr);
    void set_data_file(std::string _data_file_name);
    void set_log_file(std::string _log_file_name);
    void set_save_data(bool _b);
    void set_save_log(bool _b);
    void threaded_method(void);
    
    CZBLog();
  
    ~CZBLog();

    void LOGDATA(data_vct_ptr _data);
    void ERROR_WARNING_LOG(char *_msg);
    void WARNING_LOG(char *_msg);
    void INFO_LOG(char *_msg);
    
private:
    boost::thread _log_thread;
    boost::mutex _log_mutex;
    boost::mutex _data_mutex;
    boost::mutex _config_mutex; // to change the configuration on the fly
    std::queue<data_unit_ptr> _data_queue;
    std::queue<message_unit_ptr> _log_queue;
    bool _save_data_to_file;
    bool _save_log_to_file;
    bool _save_data;
    bool _save_log;
    std::fstream _data_file;
    std::fstream _log_file;
    bool _thread_exit;
};

extern CZBLog LOG;

#endif
