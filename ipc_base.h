/* 
 * File:   ipc_base.h
 * Author: jpcordovae
 *
 * Created on 5 de enero de 2009, 12:34 PM
 */

#ifndef _IPC_BASE_H
#define	_IPC_BASE_H

// system includes
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ZB_FIFO "/tmp/zb_fifo" // Zig bee fifo
#define SP_FIFO "/tmp/sp_fifo" // Serial Port Fifo
#define WS_FIFO "/tmp/ws_fifo" // web service Fifo

typedef enum TYPE_IPC_MSG {
                    ED,     // msg come from EndDevice
                    CTRL,   // is a control msg
                    COMM,   // msg come from CommPort
                    ZB,     // msg come from ZB app
                    NO_TYPE // no type defined yet
                    }TYPE_IPC_MSG;

typedef enum { SP_SEND, SP_CONTROL } TYPE_IPC_MSG_SERIALPORT;

typedef struct IPC_MSG{
    unsigned int PID;
    TYPE_IPC_MSG enIpcMsgType;
    char pipe[PATH_MAX];
    unsigned char msg[PIPE_BUF/2];
    size_t msglenght;
} IPC_MSG, *p_IPC_MSG;

#ifdef	__cplusplus
extern "C" {
#endif

p_IPC_MSG GetNewIpcMsg(void);
void CleanIpcMsg(p_IPC_MSG _p_IM);
p_IPC_MSG SetIpcMsg(p_IPC_MSG _p_IM , TYPE_IPC_MSG _enTIM, char *_cPipePath, unsigned char *_msg, size_t _msgSize );
void PrintFifoError(int _errno);

#ifdef	__cplusplus
}
#endif

#endif	/* _IPC_BASE_H */

