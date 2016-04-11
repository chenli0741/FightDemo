//
//  LQBaseRequest.h
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#ifndef MyCocos2d_LQBaseRequest_h
#define MyCocos2d_LQBaseRequest_h

#include "LQSocketBuffer.h"

#include "../Battle/core/LQCore.h"
#include "../Battle/core/LQTemplate.h"


STD_NAMESPACE_USE
//NL_NAMESPACE_USE

LQ_NAMESPACE


class LQBaseRequest {
    
public:
    LQBaseRequest();
    virtual ~LQBaseRequest();
    
    // the message body
    inline LQSocketBuffer* bodyData() { return m_buffer; };
    
protected:

    // write int value
    void writeInt(int value);
    // write short value
    void writeShort(short value);
    // write byte value. return short class, Because class type "byte" is not defined int c language
    void writeByte(int value);
    // write String value
    void writeString(const std::string& value);
    // write float value
    void writeFloat(float value);
    // write long long value
    void writeLong(long long value);
    // write double value
    void writeDouble(double value);
    // write bool value
    void writeBoolean(bool value);
    // write byte array, class type of the NSData length is short
    void writeBytes(const LQBaseRequest* request);
    void writeBytes(const char* bytes, size_t length);
    
protected:
    LQSocketBuffer *m_buffer;
};

LQ_NAMESPACE_END

#endif
