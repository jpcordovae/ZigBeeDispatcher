/*
 * File:   serialport.h
 * Author: Juan Pablo Cordova Echeverria
 * email: jpcordovae@gmail.com
 * Created on December 14, 2008, 2:30 AM
 */

#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <map>
#include <vector>

#include "common.h"
//#include "CDispatcher.h"

void *ThreadTX(void *_arg);

#define BAUDRATE B57600

void signal_handler_IO (int _status); // definition of signal handler

class CSerialPort
{

public:

    typedef boost::signals2::signal< void ( data_vct_ptr )> signalDataCallBack;
    typedef boost::signals2::signal< void (bool) > signalConnectionStatus;
    typedef boost::shared_ptr<CSerialPort> SerialPortPtr;

    struct SerialPortRuntimeError : std::runtime_error
    {
    SerialPortRuntimeError() : runtime_error("serial port runtime error")
        {        }
    };

CSerialPort() : bIsConnected(false)
    {}

CSerialPort(const std::string _sDev) : sDevice(_sDev) {  }

    bool OpenSerialPort(void);

    bool OpenSerialPort(const std::string &_sDev)
    {
        SetDevice(_sDev);
        return OpenSerialPort();
    }

    void addData(uint8_t *buffer, std::size_t buffersize)
    {
        if(buffersize<=0) return;
        for( size_t i=0;i<buffersize;i++)
        {
            vctBuffer->push_back(buffer[i]);
        }
        DataCallBack(vctBuffer);
    }

    void addDataVector(data_vct_ptr _vctBuffer)
    {
        DataCallBack(_vctBuffer);
    }

    void SetDevice(std::string _sDev) { sDevice = _sDev; }

    void addDataCallBack( signalDataCallBack::slot_type subscriber)
    {
        DataCallBack.connect(subscriber);
    }

    void addConnectionStatusCallBack( signalConnectionStatus::slot_type subscriber )
    {
        ConnectionStatusCallBack.connect(subscriber);
    }

    int GetFileDescriptor(void) { return fdSP; };

    size_t SendBuffer(data_vct_ptr __vct);
    //int SendBuffer(std::vector<boost::uint8_t> &_vct);
    
    void CloseSerialPort(void)
    {
        close(fdSP);
        bIsConnected = false;
        ConnectionStatusCallBack(false);
    }

    ~CSerialPort()
    {
        vctBuffer->clear();
        ConnectionStatusCallBack(false); // be carefull with sw shutdown
        close(fdSP);
    }

private:
    std::string sDevice; // device to open
    std::string sName;   // name of the device, to show

    data_vct_ptr vctBuffer; // raw data buffer
    signalDataCallBack DataCallBack; // new data arrive
    signalConnectionStatus ConnectionStatusCallBack; // change in connection status
    int fdSP; // serial port file descriptor
    struct sigaction saio; // definition of signal action
    bool bIsConnected;
    bool bThreadExecute;
    struct termios oldtio, newtio;

};

typedef CSerialPort::SerialPortPtr SerialPortPtr;

extern CSerialPort SerialPort;

#endif  /* _SERIALPORT_H */
