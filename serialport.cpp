//#include "czigbee.h"
#include "serialport.h"
#include "common.h"
#include <limits.h>
#include <pthread.h>
#include "czblog.hpp"

CSerialPort SerialPort;

bool CSerialPort::OpenSerialPort()
{
    fdSP = open(sDevice.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY );
    if (fdSP == -1)
    {
        perror(sDevice.c_str());
        ConnectionStatusCallBack(false);
        bIsConnected = false;
        return false;
    }
    bIsConnected = true;

    saio.sa_handler = signal_handler_IO;
    sigemptyset(&saio.sa_mask);
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO, &saio, NULL);
    sigaction(SIGPIPE, &saio, NULL);
    sigaction(SIGINT, &saio, NULL);
    sigaction(SIGSEGV, &saio, NULL);
    /* allow the process to receive SIGIO*/
    fcntl(fdSP, F_SETOWN, getpid());
    /* Make the file descriptor asynchronous (the manual page says only that F_SETFL only work with O_APPEND and O_NONBLOCK) */
    fcntl(fdSP, F_SETFL, FASYNC);
    tcgetattr(fdSP, &oldtio); /* save current port settings */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | /*ICRNL | */IGNCR;
    newtio.c_oflag = ~OPOST; // for raw flow
    newtio.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);
    newtio.c_cc[VMIN] = 0;
    newtio.c_cc[VTIME] = 0;
    tcflush(fdSP,TCIOFLUSH);
    tcsetattr(fdSP,TCSANOW,&newtio);

    ConnectionStatusCallBack(true);

    return true;
}

size_t CSerialPort::SendBuffer(data_vct_ptr _vct)
{
    return write(fdSP,(void *)_vct->at(0),_vct->size());
}

/*int CSerialPort::SendBuffer(std::vector<boost::uint8_t> &_vct)
{
    return write(fdSP,(void *)_vct.at(0),_vct.size());
}*/

void signal_handler_IO(int _status)
{
    static uint8_t buffer[PIPE_BUF];
    int buffersize, fd_sp;
    data_vct_ptr vctBuffer( new std::vector<boost::uint8_t>() );
    switch (_status)
    {
    case SIGIO:
        // READ
        memset(buffer, 0, PIPE_BUF);
        buffersize = 0;
        fd_sp = SerialPort.GetFileDescriptor();
	
        if( (buffersize = read(fd_sp,buffer,PIPE_BUF))==0)
        {
            break;
        }

        try{

            for(ssize_t k=0;k<buffersize;k++)
            {
		//printf("%02x:",buffer[k]);
                if( (buffer[k] == 0x7E) && !vctBuffer->empty() )
                {
		    LOG.LOGDATA(vctBuffer);
                    //SerialPort.addDataVector(vctBuffer);
                    vctBuffer = data_vct_ptr(new std::vector<boost::uint8_t>()); // get a new vector to store data incoming from serial port
		    vctBuffer->clear();
		}
                vctBuffer->push_back(buffer[k]);
            }

        }catch(CSerialPort::SerialPortRuntimeError &e)
        {
            perror(e.what());
        }

        break;
    case SIGINT: // Interactive attention
        printf("SIGINT serial port !!!\n");
        break;
    case SIGSEGV:
        printf("Segmentation Fault !!!!\n");
        //close(fdSP);
        //pthread_exit(0x00);
        break;
    default:
        break;
    }
}


