//
//  LQSocketBuffer.h
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#ifndef MyCocos2d_LQSocketBuffer_h
#define MyCocos2d_LQSocketBuffer_h

#include <stdint.h>
#include <string>

#include <iostream>
//#include "BaseSocketBuffer.h"

//#include "../Utils/LQCore.h"

using namespace std;

//NL_NAMESPACE_USE

//LQ_NAMESPACE

class LQSocketBuffer{
public:
    inline int size() { return m_size; }
    inline int capacity() {return m_capacity; }
    inline char* bodyData() { return m_data; }
    inline int position() { return m_position; }
    
    static LQSocketBuffer* create();
    static LQSocketBuffer* createWithCapacity(size_t capacity);
    //static LQSocketBuffer* createWithSocketBuffer(BaseSocketBuffer *buffer);
    static LQSocketBuffer* createWithCharArray(const char *charArray, size_t length);
    virtual ~LQSocketBuffer();
    
    
    // write int value
    void writeInt(int value);
    // write short value
    void writeShort(short value);
    // write byte value. return short class, Because class type "byte" is not defined int c language
    void writeByte(char value);
    // write String value
    void writeString(const string &value);
    // write float value
    void writeFloat(float value);
    // write long long value
    void writeLong(long long value);
    // write double value
    void writeDouble(double value);
    // write bool value
    void writeBoolean(bool value);
    // write byte array, class type of the NSData length is short
//    void writeBytes(BaseSocketBuffer* request);
    void writeBytes(LQSocketBuffer *request);
    void writeBytes(const char* bytes, int length);
    
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
    // read byte array, the return buffer should delete manually
    // first, get the NSData length, class type is short
    // and, get the short length data
//    BaseSocketBuffer* readBytes();
    // read fixed length array
//    BaseSocketBuffer* readBytes(size_t length);
    
    LQSocketBuffer* readBytes();
    LQSocketBuffer* readBytes(size_t length);
    
    // reset position to zero
    void markPosition();
    void resetPosition();
    void flip();
        
//private: // 下面的不要调用  只是lua所有不能private的了  
    char* m_data;   // 内容
    int m_size;       // 当前内容长度
    int m_capacity;   // 容量
    int m_position;     // 当前已读取位置

    LQSocketBuffer();
    LQSocketBuffer(size_t capacity);
//    LQSocketBuffer(BaseSocketBuffer *buffer);
    LQSocketBuffer(const char *charArray, size_t length);

    // 检测数据长度
    void checkCapacity(size_t valueLength);

    // write value
    void writeValue(const void *, size_t);
    void readValue(void *, size_t);
};

//LQ_NAMESPACE_END

#endif
