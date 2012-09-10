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

void ShowUINT8(uint8_t _d );
void ShowVectorUINT8(std::vector<uint8_t> );
void ShowBufferOfVectorsUINT8( std::vector<std::vector<uint8_t> > &_vct );
std::ostream& operator<<(std::ostream &_output, const std::vector<uint8_t> &_vector);
std::vector<std::string> tokenize(const std::string &_line, const char *_delimeter);
typedef boost::error_info<struct tag_errmsg, std::string> errmsg_info;
typedef boost::error_info<struct tag_errno,int> errno_info;

typedef boost::posix_time::ptime ptime;

class zb_exception_base : public boost::exception , public std::exception { };

class CEndDeviceException : public zb_exception_base { };

class bad_rf_frame : public zb_exception_base { };

#endif	/* _COMMON_H */

