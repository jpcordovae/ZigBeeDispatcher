/*
 * File:   zb_frame.h
 * Author: jpcordovae
 *
 * Created on 5 de enero de 2009, 12:28 PM
 */

#ifndef _ZB_FRAME_H
#define	_ZB_FRAME_H

#define FRAME_BUFFER 100 // 100 bytes
#define ZB_DELIMETER 0x7E

#include "common.h"
#include <string.h>
#include <stdlib.h>

typedef struct stFame {
	uint8_t ucFrameID;
	uint8_t ucDelimeter;
	uint8_t ucMSB;
	uint8_t ucLSB;
	uint8_t ucCmdID;
	unsigned short usLenght;
	uint8_t ucAddress[8];
	uint8_t buffer[FRAME_BUFFER]; // data buffer
	unsigned short sizebuffer;
	uint8_t ucCheckSum;
} stFrame;

// frame manage
int NewStFrameFromRawFrame(uint8_t *ucBuffer, unsigned short buffersize,stFrame *_stF);
int NewRawFrameFromStFrame(uint8_t *ucBuffer, stFrame *_p_stFrame); // return the buffer lenght. Warning !!!, we mus be SURE that the buffer have reverved space to keep the data
uint8_t CheckSumBuffer(uint8_t *Buffer,int iBufferSize);

#endif	/* _ZB_FRAME_H */
