//
//  LQBaseRequest.cpp
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#include "LQBaseRequest.h"
#include "LQSocketBuffer.h"

#pragma mark LQBaseRequest create&init

LQ_NAMESPACE

LQBaseRequest::LQBaseRequest()
{
    m_buffer = LQSocketBuffer::create();
}

LQBaseRequest::~LQBaseRequest()
{
    delete m_buffer;
}

// write int value
void LQBaseRequest::writeInt(int value)
{
    m_buffer->writeInt(value);
}

// write short value
void LQBaseRequest::writeShort(short value)
{
    m_buffer->writeShort(value);
}

// write byte value. return short class, Because class type "byte" is not defined int c language
void LQBaseRequest::writeByte(int value)
{
    m_buffer->writeByte(value);
}

// write String value
void LQBaseRequest::writeString(const std::string& value)
{
    m_buffer->writeString(value);
}

// write float value
void LQBaseRequest::writeFloat(float value)
{
    m_buffer->writeFloat(value);
}

// write long long value
void LQBaseRequest::writeLong(long long value)
{
    m_buffer->writeLong(value);
}

// write double value
void LQBaseRequest::writeDouble(double value)
{
    m_buffer->writeDouble(value);
}

// write bool value
void LQBaseRequest::writeBoolean(bool value)
{
    m_buffer->writeBoolean(value);
}

// write byte array, class type of the NSData length is short
void LQBaseRequest::writeBytes(const LQBaseRequest* request)
{
    // 先长度
    m_buffer->writeShort(request->m_buffer->size());
    // 后内容
    m_buffer->writeBytes(request->m_buffer);
}

void LQBaseRequest::writeBytes(const char* bytes, size_t length)
{
    m_buffer->writeBytes(bytes, length);
}

LQ_NAMESPACE_END