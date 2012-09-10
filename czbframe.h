/* 
 * File:   czbframe.h
 * Author: jpcordovae
 *
 * Created on March 6, 2011, 1:53 PM
 */

#ifndef _CZBFRAME_H
#define	_CZBFRAME_H

#include "common.h"
#include <vector>


class CZBFrame {
public:

    typedef boost::shared_ptr<CZBFrame> CZBFramePtr;

    CZBFrame();
    CZBFrame(const CZBFrame& orig);
    CZBFrame(std::vector<uint8_t>);
    virtual ~CZBFrame();

    bool setFromRawFrame(std::vector<uint8_t> ); // return false in any problem
    static bool CheckRawFrame(std::vector<uint8_t> &);
    static uint8_t getAPIIdentifierFromRFDataFrame(const std::vector<uint8_t> );
    static std::vector<uint8_t> getcmdDataFromRFDataFrame(const std::vector<uint8_t> );
    static std::vector<uint8_t> get64bAddressFromCmdData( const std::vector<uint8_t> &);
    uint8_t getFrameID(void) { return FrameID; }
    uint8_t getDelimeter(void) { return Delimeter; }
    uint8_t getMSB(void) { return MSB; }
    uint8_t getLSB(void) { return LSB; }
    size_t getTotalLenght(void){ return vctFrameData.size() + 4; } // including checksum
    size_t getDataLenght(void) { return vctFrameData.size(); }
    uint8_t getCheckSum(std::vector<uint8_t> &);
    uint8_t getCheckSum(void) { return CheckSum; }
    std::vector<uint8_t> &getDataVector(void) { return vctFrameData; }

private:
    uint8_t FrameID;
    uint8_t Delimeter;
    uint8_t MSB;
    uint8_t LSB;
    //uint8_t CmdID;
    uint16_t Lenght; // two bytes size, is the concatenation of LSB and LSB, be carefull with procesor endianess
    std::vector<uint8_t> vctFrameData;// data buffer
    uint8_t CheckSum;
};

typedef CZBFrame::CZBFramePtr CZBFramePtr;

//extern std::vector<CZBFramePtr> RFFrameBuffer; // working buffer, shared pointers

#endif	/* _CZBFRAME_H */

