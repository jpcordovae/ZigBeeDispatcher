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
    CZBFrame(data_vct_ptr);
    virtual ~CZBFrame();

    bool setFromRawFrame( data_vct_ptr ); // return false in any problem
    static bool CheckRawFrame( data_vct_ptr );
    static uint8_t getAPIIdentifierFromRFDataFrame( data_vct_ptr );
    static data_vct_ptr getcmdDataFromRFDataFrame( data_vct_ptr );
    static data_vct_ptr get64bAddressFromCmdData(data_vct_ptr);
    uint8_t getFrameID(void) { return FrameID; }
    uint8_t getDelimeter(void) { return Delimeter; }
    uint8_t getMSB(void) { return MSB; }
    uint8_t getLSB(void) { return LSB; }
    size_t getTotalLenght(void){ return vctFrameData->size() + 4; } // including checksum
    size_t getDataLenght(void) { return vctFrameData->size(); }
    uint8_t getCheckSum(data_vct_ptr);
    uint8_t getCheckSum(void) { return CheckSum; }
    data_vct_ptr getDataVector(void) { return vctFrameData; }

private:
    uint8_t FrameID;
    uint8_t Delimeter;
    uint8_t MSB;
    uint8_t LSB;
    //uint8_t CmdID;
    uint16_t Lenght; // two bytes size, is the concatenation of LSB and LSB, be carefull with procesor endianess
    data_vct_ptr vctFrameData;// data buffer
    uint8_t CheckSum;
};

typedef CZBFrame::CZBFramePtr CZBFramePtr;

//extern std::vector<CZBFramePtr> RFFrameBuffer; // working buffer, shared pointers

#endif	/* _CZBFRAME_H */

