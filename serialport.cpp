//#include "czigbee.h"
#include "serialport.h"
#include "common.h"
#include <limits.h>
#include <pthread.h>

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
    /* allow the process to receive SIGIO */
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

int CSerialPort::SendBuffer(std::vector<uint8_t> __vct)
{
    std::vector<uint8_t> vctTmp = __vct; // this because  the thread kill the memory of the data
    return write(fdSP,&(vctTmp[0]),vctTmp.size());
}

void signal_handler_IO(int _status)
{
    static uint8_t buffer[PIPE_BUF];
    int buffersize, fd_sp;
    static std::vector<uint8_t> vctBuffer;
    switch (_status) {
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

            for(int k=0;k<buffersize;k++)
            {
                if( (buffer[k] == 0x7E) && !vctBuffer.empty() )
                {
                    SerialPort.addDataVector(vctBuffer);
                    vctBuffer.clear();
                }
                vctBuffer.push_back(buffer[k]);
            }

        }catch(CSerialPort::SerialPortRuntimeError &e)
        {
            perror(e.what());
        }
        //SerialPort.serialPortBufferEmpty(true);

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


