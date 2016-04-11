//
//  LQSocketBuffer.cpp
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#include "LQSocketBuffer.h"
#include <assert.h>

//LQ_NAMESPACE
// 默认buffer长度 256
#define DEFAULT_BUFFER_LENGTH 512

#pragma mark LQSocketBuffer create&init

LQSocketBuffer::LQSocketBuffer()
{
    m_capacity = DEFAULT_BUFFER_LENGTH;
    m_data = (char*)malloc(m_capacity);
    m_size = 0;
    m_position = 0;
}

LQSocketBuffer::LQSocketBuffer(size_t capacity)
{
    m_capacity = capacity;
    m_data = (char*)malloc(m_capacity);
    m_size = 0;
    m_position = 0;
}

//LQSocketBuffer::LQSocketBuffer(BaseSocketBuffer *buffer)
//{
//    if (buffer) {
//        m_capacity = buffer->capacity();
//        m_data = (char*)malloc(m_capacity);
//        m_size = buffer->size();
//        m_position = 0;
//        memcpy(m_data, buffer->bodyData(), m_size);
//    }else{
//        // 构造函数之间调用存在问题，参考http://www.cnblogs.com/chio/archive/2007/10/20/931043.html
//        new (this)LQSocketBuffer();
//    }
//}

LQSocketBuffer::~LQSocketBuffer()
{
    free(m_data);
}

LQSocketBuffer::LQSocketBuffer(const char *charArray, size_t length)
{
    m_capacity = length;
    m_data = (char*)malloc(m_capacity);
    m_size = length;
    m_position = 0;
    memcpy(m_data, charArray, length);
}

LQSocketBuffer* LQSocketBuffer::create()
{
    return new LQSocketBuffer();
}

//LQSocketBuffer* LQSocketBuffer::createWithSocketBuffer(BaseSocketBuffer *buffer)
//{
//    return new LQSocketBuffer(buffer);
//}

LQSocketBuffer* LQSocketBuffer::createWithCharArray(const char *charArray, size_t length)
{
    return new LQSocketBuffer(charArray, length);
}

LQSocketBuffer* LQSocketBuffer::createWithCapacity(size_t capacity)
{
    return new LQSocketBuffer(capacity);
}

#pragma mark LQSocketBuffer write message

void LQSocketBuffer::writeBoolean(bool value)
{
    writeValue(&value, sizeof(bool));
}

void LQSocketBuffer::writeByte(char value)
{
    writeValue(&value, sizeof(char));
}

void LQSocketBuffer::writeShort(short value)
{
    writeValue(&value, sizeof(short));
}

void LQSocketBuffer::writeInt(int value)
{
    writeValue(&value, sizeof(int));
}

void LQSocketBuffer::writeLong(long long value)
{
    writeValue(&value, sizeof(long long));
}

void LQSocketBuffer::writeFloat(float value)
{
    writeValue(&value, sizeof(float));
}

void LQSocketBuffer::writeDouble(double value)
{
    writeValue(&value, sizeof(double));
}

void LQSocketBuffer::writeString(const string &value)
{
    // write the length(short) to data cache
    // 去掉最后的结束符
    writeShort(value.length());
    writeBytes(value.c_str(), value.length());
}

void LQSocketBuffer::writeBytes(const char *bytes, int length)
{
    writeValue(bytes, length);
}

void LQSocketBuffer::writeBytes(LQSocketBuffer *request)
{
    writeBytes(request->bodyData(), request->size());
}

void LQSocketBuffer::writeValue(const void* value, size_t valueLength)
{
    // 检查容量
    checkCapacity(valueLength);
    // 复制内容
    memcpy(m_data + m_position, value, valueLength);
    // 变更位置
    m_position += valueLength;
    // 同步size，保证一致性
    m_size = m_position;
}

void LQSocketBuffer::checkCapacity(size_t valueLength)
{
    int newLength = m_position + valueLength;
    int newCapacity = m_capacity;
    // 判断是否需要扩容
    while (newLength > newCapacity) {
        // 增加一倍容量
        newCapacity = newCapacity << 1;
    }
    
    if (newCapacity != m_capacity) {
        // 如果容量发生变化了
        m_capacity = newCapacity;
        // 创建新的数组
        char *newData = (char*)malloc(m_capacity);
        // 如果内存分配失败
        //CCAssert(newData, "读取消息过程中，内存分配失败");
        
        // 复制原有数据
        memcpy(newData, m_data, m_size);
        // 释放原来分配的内存
        free(m_data);
        // 赋值
        m_data = newData;
    }
}

#pragma mark LQSocketBuffer read message

bool LQSocketBuffer::readBoolean()
{
    size_t valueLength = sizeof(bool);
    
    //char buffer[valueLength];
    char* buffer = new char[valueLength];
    readValue(buffer, valueLength);
    
    bool *value;
    value = (bool*)buffer;
    return *value;
}

short LQSocketBuffer::readByte()
{
    size_t valueLength = sizeof(char);
    
    //char buffer[valueLength];
    char* buffer = new char[valueLength];
    readValue(buffer, valueLength);
    
    char *value;
    value = (char*)buffer;
    short shortValue = *value;
    return shortValue;
}

short LQSocketBuffer::readShort()
{
    size_t valueLength = sizeof(short);
    
    //char buffer[valueLength];
    char* buffer = new char[valueLength];
    readValue(buffer, valueLength);
    
    short *value;
    value = (short*)buffer;
    return *value;
}

int LQSocketBuffer::readInt()
{
    size_t valueLength = sizeof(int32_t);
    
    //char buffer[valueLength];
    char* buffer = new char[valueLength];
    readValue(buffer, valueLength);
    
//    printf("%x %x %x %x\n", (unsigned char)buffer[0],
//           (unsigned char)buffer[1],
//           (unsigned char)buffer[2],
//           (unsigned char)buffer[3]);
    
    int *value;
    value = (int*)buffer;
    return *value;
}

long long LQSocketBuffer::readLong()
{
    size_t valueLength = sizeof(long long);
    
    //char buffer[valueLength];
    char* buffer = new char[valueLength];
    readValue(buffer, valueLength);
    
    long long *value;
    value = (long long*)buffer;
    return *value;
}

float LQSocketBuffer::readFloat()
{
    size_t valueLength = sizeof(float);
    
    //char buffer[valueLength];
    char* buffer = new char[valueLength];
    readValue(buffer, valueLength);
    
    float *value;
    value = (float*)buffer;
    return *value;
}

double LQSocketBuffer::readDouble()
{
    size_t valueLength = sizeof(double);
    
    //char buffer[valueLength];
    char* buffer = new char[valueLength];
    readValue(buffer, valueLength);
    
    double *value;
    value = (double*)buffer;
    return *value;
}

string LQSocketBuffer::readString()
{
    short strLength = readShort();
    
    //char buffer[strLength+1];
    char* buffer = new char[strLength+1];
    readValue(buffer, strLength);
    buffer[strLength] = 0;
    // the second parameter is the string length
    // not the length of the char array 
    return string(reinterpret_cast<char const*>(buffer), strLength);
}

LQSocketBuffer* LQSocketBuffer::readBytes()
{
    short length = readShort();
    
    char buffer[length];
    readValue(buffer, length);
    
    return new LQSocketBuffer(buffer, length);
}

LQSocketBuffer* LQSocketBuffer::readBytes(size_t length)
{    
    char buffer[length];
    readValue(buffer, length);
    
    return new LQSocketBuffer(buffer, length);
}

void LQSocketBuffer::readValue(void * buffer, size_t valueLength)
{
    assert((valueLength + m_position) <= m_size);
    
    memcpy(buffer, m_data + m_position, valueLength);
    m_position += valueLength;
}

#pragma mark LQSocketBuffer position

// reset position to zero
void LQSocketBuffer::resetPosition()
{
    m_position = 0;
}

void LQSocketBuffer::markPosition()
{
    m_position = m_size;
}

void LQSocketBuffer::flip()
{
    // 还没读取数据，无需flip
    if (m_position == 0) {
        return;
    }
    
    // 创建新的数组
    char *newData = (char*)malloc(m_capacity);
    // 复制原有数据
    memcpy(newData, m_data + m_position, m_size - m_position);
    // 释放原来分配的内存
    free(m_data);
    // 赋值
    m_data = newData;
    
    m_size -= m_position;
    resetPosition();
}

//LQ_NAMESPACE_END