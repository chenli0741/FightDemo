//
//  LQBaseSocketBuffer.h
//  sgmz
//
//  Created by ByronSong on 13-4-7.
//
//

#ifndef SuperWarriors_BaseSocketBuffer_h
#define SuperWarriors_BaseSocketBuffer_h

#include <stdint.h>
#include <string>

//#include <netlink/netlink.h>
//#include "../Battle/core/LQCore.h"

using namespace std;

//LQ_NAMESPACE

class BaseSocketBuffer {
    
public:
    virtual int size() = 0;
    virtual int capacity() = 0;
    virtual char* bodyData() = 0;
    virtual int position()  = 0;
    
    BaseSocketBuffer(){};
    virtual ~BaseSocketBuffer(){};
    
    // write int value
    virtual void writeInt(int value) = 0;
    // write short value
    virtual void writeShort(short value) = 0;
    // write byte value. return short class, Because class type "byte" is not defined int c language
    virtual void writeByte(char value) = 0;
    // write String value
    virtual void writeString(const std::string &value) = 0;
    // write float value
    virtual void writeFloat(float value) = 0;
    // write long long value
    virtual void writeLong(long long value) = 0;
    // write double value
    virtual void writeDouble(double value) = 0;
    // write bool value
    virtual void writeBoolean(bool value) = 0;
    // write byte array, class type of the NSData length is short
    virtual void writeBytes(BaseSocketBuffer* request) = 0;
    virtual void writeBytes(const char* bytes, int length) = 0;
    
    // get int value
    virtual int readInt() = 0;
    // get short value
    virtual short readShort() = 0;
    // get byte value. return short class, Because class type is not defined int c language
    virtual short readByte() = 0;
    // get String value. NSString will auto release.
    virtual std::string readString() = 0;
    // read float value
    virtual float readFloat() = 0;
    // read long value
    virtual long long readLong() = 0;
    // read double value
    virtual double readDouble() = 0;
    // read bool value
    virtual bool readBoolean() = 0;
    // read byte array, the return buffer should delete manually
    // first, get the NSData length, class type is short
    // and, get the short length data
    virtual BaseSocketBuffer* readBytes() = 0;
    // read fixed length array
    virtual BaseSocketBuffer* readBytes(size_t length) = 0;
    
    // reset position to zero
    virtual void markPosition() = 0;
    virtual void resetPosition() = 0;
    virtual void flip() = 0;
};

//LQ_NAMESPACE_END

#endif
