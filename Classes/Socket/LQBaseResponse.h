//
//  LQBaseResponse.h
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#ifndef __MyCocos2d__LQBaseResponse__
#define __MyCocos2d__LQBaseResponse__

#include "LQSocketBuffer.h"

#include "../Battle/core/LQCore.h"
#include "../Battle/core/LQTemplate.h"
#include "../Battle/core/DynBase.h"

STD_NAMESPACE_USE
//NL_NAMESPACE_USE

LQ_NAMESPACE

class LQBaseResponse : public UObject {
    
public:
    LQBaseResponse();
    LQBaseResponse(LQSocketBuffer* buffer);
    virtual ~LQBaseResponse();
    
    // read message from socket
    virtual void readMessage(const char* data, size_t length);
    
protected:
    
    // get int value
    int readInt();
    // get short value
    short readShort();
    // get byte value. return short class, Because class type is not defined int c language
    short readByte();
    // get String value. NSString will auto release.
    string readString();
    // read float value
    float readFloat();
    // read long value
    long long readLong();
    // read double value
    double readDouble();
    // read bool value
    bool readBoolean();
    // read byte array, the return NSData will auto release
    // first, get the NSData length, class type is short
    // and, get the short length data
    LQSocketBuffer* readBytes();
    
protected:
    LQSocketBuffer* m_buffer;
    
};

LQ_NAMESPACE_END

#endif /* defined(__MyCocos2d__LQBaseResponse__) */
