/*
 * File:   DomBase.h
 * Author: jpcordovae
 *
 * Created on December 13, 2008, 4:33 PM
 */

#ifndef _DOMBASE_H
#define _DOMBASE_H

#include <map>

// system includes
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// local includes
#include "zb_frame.h"
#include "ipc_base.h"

/////////////////////////////
// Comandos
/////////////////////////////

/* Special */
#define C_WR    "WR" // Write
#define C_RE    "RE" // Restore Defaults
#define C_FR    "FR" // Software Reset

/* Networking & secutiry */
#define C_CH    "CH" // Channel
#define C_ID    "ID" // PAN ID
#define C_DH    "DH" // Destination Address High
#define C_DL    "DL" // Destination Address Low
#define C_MY    "MY" // 16-bit Source Address
#define C_SH    "SH" // Serial Number High
#define C_SL    "SL" // Serial Number Low
#define C_RR    "RR" // XBee Retries
#define C_RN    "RN" // Random Delay Slots
#define C_MM    "MM" // MAC Mode
#define C_NI    "NI" // Node Identifier
#define C_ND    "ND" // Node Discover
#define C_NT    "NT" // Node Discover Time
#define C_DN    "DN" // Destination Node
#define C_CE    "CE" // Coordinator Enable
#define C_SC    "SC" // Scan Channels
#define C_SD    "SD" // Scan Duration
#define C_A1    "A1" // End Device Association
#define C_A2    "A2" // Coordinator Association
#define C_AI    "AI" // Association Indication
#define C_DA    "DA" // Force Disassociation
#define C_FP    "FP" // Force Poll
#define C_AS    "AS" // Active Scan
#define C_ED    "ED" // Energy Scan
#define C_EE    "EE" // AES Encryption Enable
#define C_KY    "KY" // AES Encryption Key

/* RF Interfacing */
#define C_PL    "PL" // Power Level
#define C_CA    "CA" // CCA Threshold
#define C_SM    "SM" // Sleep
#define C_ST    "ST" // Time Before Sleep
#define C_SP    "SP" // Cyclic Sleep Period
#define C_DP    "DP" // Dissasociated Cyclic Sleep Period

/* Serial Interfacing */
#define C_BD    "BD" // Interface Data Rate
#define C_RO    "RO" // Paquetization Timeout
#define C_AP    "AP" // API Enable
#define C_NB    "NB" // Parity
#define C_PR    "PR" // Pull-Up Resistor

/* I/O Settings */
#define C_D8    "D8" // DI8 Conf.
#define C_D7    "D7" // DIO7 Conf.
#define C_D6    "D6" // DIO6 Conf.
#define C_D5    "D5" // DIO5 Conf.
#define C_D4    "D4" // DIO4 Conf
#define C_D3    "D3" // DIO3 Conf.
#define C_D2    "D2" // DIO2 Conf.
#define C_D1    "D1" // DIO1 Conf.
#define C_D0    "D0" // DIO0 Conf.
#define C_IU    "IU" // I/O Output Enable
#define C_IT    "IT" // Samples Before TX
#define C_IS    "IS" // Force Sample
#define C_IO    "IO" // Digital Output Level
#define C_IC    "IC" // DIO Change Detect
#define C_IR    "IR" // Sample Rate
#define C_AV    "AV" // ADC Voltage Reference
#define C_IA    "IA" // I/O Input Address
#define C_T0    "T0" // D0 Output timeout
#define C_T1    "T1" // D1 Output timeout
#define C_T2    "T2" // D2 Output timeout
#define C_T3    "T3" // D3 Output timeout
#define C_T4    "T4" // D4 Output timeout
#define C_T5    "T5" // D5 Output timeout
#define C_T6    "T6" // D6 Output timeout
#define C_T7    "T7" // D7 Output timeout
#define C_P0    "P0" // PWM0 Conf.
#define C_P1    "P1" // PWM1 Conf.
#define C_M0    "M0" // PWM0 Output Level
#define C_M1    "M1" // PWM1 Output Level
#define C_PT    "PT" // PWM Output Timeout
#define C_RP    "RP" // RSSI PWM Timer

/* Diagnostic */
#define C_VR    "VR" // Firmware Version
#define C_VL    "VL" // Firmware Version - Verbose
#define C_HV    "HV" // Hardware Version
#define C_DB    "DB" // Received Signal Strength
#define C_EC    "EC" // CCA Failure
#define C_EA    "EA" // ACK Failures
#define C_ED    "ED" // Energy Scan

/* AT Command Options */
#define C_CT    "CT" // Command Mode Timeout
#define C_CN    "CN" // Exit Command Mode
#define C_AC    "AC" // Apply Changes
#define C_GT    "GT" // Guard Times
#define C_CC    "CC" // Command Sequence Character

/* cmdID for API mode */
#define API_AT_REMOTE_COMMAND_REQUEST   0x17 // comando remoto
#define API_AT_REMOTE_COMMAND_RESPONSE  0x97 // respuesta a comando remoto
#define API_AT_COMMAND                  0x08 // comando local
#define API_AT_COMMAND_RESPONSE         0x88 // respuesta a un comando AT enviado
#define API_MODEM_STATUS                0x8A // status del modem
#define API_QUEUE_PARAMETER_VALUE       0x09
#define API_TX_REQUEST_64bit_ADDRESS    0x00
#define API_TX_REQUEST_16bit_ADDRESS    0x01
#define API_TX_STATUS                   0x89
#define API_RX_PACKET_64bit_ADDRESS     0x80
#define API_RX_PACKET_16bit_ADDRESS     0x81
#define API_RX_IO_DATA_16bit_ADDRESS    0x83
#define API_RX_IO_DATA_64bit_ADDRESS    0x82

// MSB (uint8_t 2 del I/O header )
#define API_CI_A5(x) ((x>>14)&0x01)
#define API_CI_A4(x) ((x>>13)&0x01)
#define API_CI_A3(x) ((x>>12)&0x01)
#define API_CI_A2(x) ((x>>11)&0x01)
#define API_CI_A1(x) ((x>>10)&0x01)
#define API_CI_A0(x) ((x>>9) &0x01)
#define API_CI_D8(x) ((x>>8) &0x01)
// LSB (uint8_t 1 del I/O header )
#define API_CI_D7(x) ((x>>7)&0x01)
#define API_CI_D6(x) ((x>>6)&0x01)
#define API_CI_D5(x) ((x>>5)&0x01)
#define API_CI_D4(x) ((x>>4)&0x01)
#define API_CI_D3(x) ((x>>3)&0x01)
#define API_CI_D2(x) ((x>>2)&0x01)
#define API_CI_D1(x) ((x>>1)&0x01)
#define API_CI_D0(x) (  (x) &0x01)

// Channel indicator is defined by a 'short' type
#define CI_ADCX 0x8E00
#define CI_DX   0x01FF

#define CI_ADC5 0x4000
#define CI_ADC4 0x2000
#define CI_ADC3 0x1000
#define CI_ADC2 0x0800
#define CI_ADC1 0x0400
#define CI_ADC0 0x0200
#define CI_D8   0x0100
#define CI_D7   0x0080
#define CI_D6   0x0040
#define CI_D5   0x0020
#define CI_D4   0x0010
#define CI_D3   0x0008
#define CI_D2   0x0004
#define CI_D1   0x0002
#define CI_D0   0x0001

#define CI_ADC_TEST(x) (CI_ADCX & x)

// PIN configuration of ZigBee
#define XB_IO_ADC   0x02
#define XB_IO_DI    0x03
#define XB_IO_DOL   0x04
#define XB_IO_DOH   0x05

// Buffer size
#define MAX_ED_TX_BUFFER    100
#define MAX_ED_TX_TRY       10  // max retransmition.
#define COMM_BUFFER_SIZE    1024*4 // 4Kb for inet comms...

// Modem Status; API: 0x8A
enum enModemStatus
{
    HR = 0, // Hardware Reset
    WTR, // Watchdog timer reset
    ASS, // Associated
    DIS, // Dissasociated
    SYN_LOST, // Synchronization lost
    COORDINATOR_REALIGNMENT, // Coordinator realignment
    COORDINATOR_STARTED
}; // Coordinator started

enum enOptionRXPacket
{
    ADDRESS_BROADCAST = 1, PAN_BROADCAST
};

enum enTXStatus
{
    SUCCESS = 0, NO_ACK, CCA_FAILURE, PURGED
};

enum enRXOptions
{
    DISABLE_ACK = 0x01, BROADCAST_PAN_ID = 0x04
};

// 0:OK, 1:Error, 2:Invalid Command, 3:Invalid Parameter, 4:No Response.
enum enFrameStatus
{
    OK = 0, F_ERROR = 1, INVALID_COMMAND = 2, INVALID_PARAMETER = 3, NO_RESPONSE = 4, NONE = 15, SEND, DO_NOTHING, F_DELETE, WAITING_RESPONSE
};

enum enIOType
{
    DIGITAL_INPUT, DIGITAL_OUTPUT, ANALOG_INPUT, ANALOG_OUTPUT
};

typedef struct XBEE_Struct
{
    uint8_t CH;
    uint8_t ID[4]; // PAN ID
    uint8_t DH[4]; // Destination address high
    uint8_t DL[4]; // Destination address low
    uint8_t MY[4]; // 16-bit address
    uint8_t SH[4];
    uint8_t SL[4];
    uint8_t RR;
    uint8_t RN;
    uint8_t MM;
    uint8_t NT;
    uint8_t CE;
    uint8_t SC[4];
    uint8_t SD;
    uint8_t A1;
    uint8_t A2;
    uint8_t AI;
    uint8_t EE;
    uint8_t KY;
    uint8_t NI;
    // RF Interfacing
    uint8_t CA;
    uint8_t PL;
    // Sleep modes (NonBeacon)
    uint8_t SM;
    uint8_t ST[4];
    uint8_t SP;
    uint8_t DP[4];
    // Serial interfacing
    uint8_t BD;
    uint8_t RO;
    uint8_t AP;
    uint8_t PR;
    // I/O seting
    uint8_t D8, D7, D6, D5, D4, D3, D2, D1, D0;
    uint8_t IU;
    uint8_t IT;
    uint8_t IC;
    uint8_t IR;
    uint8_t IA[16];
    uint8_t T0, T1, T2, T3, T4, T5, T6, T7;
    uint8_t P0;
    uint8_t P1;
    uint8_t PT;
    uint8_t RP;
    // Diagnostic
    uint8_t VR[4];
    uint8_t HV[4];
    uint8_t DB;
    uint8_t EC;
    uint8_t EA;
    // AT Commmand options
    uint8_t CT;
    uint8_t GT;
    uint8_t CC;
} XBStruct;

typedef struct stTXFrame
{
    stTXFrame()
    {
	vctBuffer = data_vct_ptr( new std::vector<boost::uint8_t>() );
    }
    uint8_t frameID;
    size_t uiTryCounter;
    ptime tTime;
    ptime tLastTime;
    data_vct_ptr vctBuffer;
    size_t buffersize;
    enum enFrameStatus enFS;
} stTXFrame;

uint8_t Char2Hex(char *_c); // just take 2 chars (or 1) uint8_t to convert

extern std::map<std::string,std::string> mapATCommandDescription;

void setMapATCommandDescription(void);

#endif  /* _DOMBASE_H */
