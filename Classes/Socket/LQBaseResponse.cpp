//
//  LQBaseResponse.cpp
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#include "LQBaseResponse.h"
#include "LQSocketBuffer.h"

LQ_NAMESPACE

LQBaseResponse::LQBaseResponse()
{
    m_buffer = nil;
}

LQBaseResponse::LQBaseResponse(LQSocketBuffer* buffer)
{
    m_buffer = buffer;
}

LQBaseResponse::~LQBaseResponse()
{
    delete m_buffer;
}

void LQBaseResponse::readMessage(const char *data, size_t length)
{
    m_buffer = LQSocketBuffer::createWithCharArray(data, length);
}

// get int value
int LQBaseResponse::readInt()
{
    return m_buffer->readInt();
}

// get short value
short LQBaseResponse::readShort()
{
    return m_buffer->readShort();
}

// get byte value. return short class, Because class type is not defined int c language
short LQBaseResponse::readByte()
{
    return m_buffer->readByte();
}

// get String value. NSString will auto release.
string LQBaseResponse::readString()
{
    return m_buffer->readString();
}

// read float value
float LQBaseResponse::readFloat()
{
    return m_buffer->readFloat();
}

// read long value
long long LQBaseResponse::readLong()
{
    return m_buffer->readLong();
}

// read double value
double LQBaseResponse::readDouble()
{
    return m_buffer->readDouble();
}

// read bool value
bool LQBaseResponse::readBoolean()
{
    return m_buffer->readBoolean();
}

// read byte array, the return buffer should delete manually
// first, get the NSData length, class type is short
// and, get the short length data
LQSocketBuffer* LQBaseResponse::readBytes()
{
    return m_buffer->readBytes();
}

LQ_NAMESPACE_END