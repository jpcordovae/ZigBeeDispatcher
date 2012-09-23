/* 
 * File:   common.h
 * Author: jpcordovae
 *
 * Created on February 28, 2011, 8:29 PM
 */

#ifndef _COMMON_H
#define	_COMMON_H

#define _LIBCPP_SHARED_LOCK

#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/exception/all.hpp>
#include <boost/date_time.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/any.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <stdlib.h>
#include <iostream>
#include <vector>

#define LOG_MSG_BUFFER_SIZE 2048 // max size of message

typedef boost::shared_ptr<std::vector<boost::uint8_t> > data_vct_ptr;

struct message_unit{
    boost::uint8_t msg[LOG_MSG_BUFFER_SIZE]; //TODO: check if this make a memory leack
    message_unit( ) { bzero(msg,LOG_MSG_BUFFER_SIZE); }
};

struct data_unit{
    data_vct_ptr databuffer_ptr;
    boost::posix_time::ptime timestamp;
};

typedef boost::shared_ptr<message_unit> message_unit_ptr;
typedef boost::shared_ptr<data_unit> data_unit_ptr;
typedef boost::shared_ptr<std::string> string_ptr;

void ShowUINT8(uint8_t _d );
void ShowVectorUINT8(data_vct_ptr);
//void ShowVectorUINT8( std::vector<uint8_t> & );
//void ShowBufferOfVectorsUINT8( std::vector<data_vct_ptr> &_vct );

std::ostream& operator<<(std::ostream &_output, data_vct_ptr _vector);
std::ostream& operator<<(std::ostream &_output, std::vector<uint8_t> &_vector);
std::ostream& operator<<(std::ostream &_output, data_unit_ptr _data_unit);

std::vector<std::string> tokenize(const std::string &_line, const char *_delimeter);

typedef boost::error_info<struct tag_errmsg, std::string> errmsg_info;
typedef boost::error_info<struct tag_errno,int> errno_info;
typedef boost::posix_time::ptime ptime;

class zb_exception_base : public boost::exception , public std::exception { };

class CEndDeviceException : public zb_exception_base { };

class bad_rf_frame : public zb_exception_base { };



#endif	/* _COMMON_H */

