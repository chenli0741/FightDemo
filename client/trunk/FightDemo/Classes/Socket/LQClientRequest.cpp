//
//  LQClientRequest.cpp
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#include "LQClientRequest.h"
#include "LQSocketBuffer.h"

LQ_NAMESPACE

LQClientRequest::LQClientRequest() : LQBaseRequest()
{
    
}

LQClientRequest::~LQClientRequest()
{
    
}

// the message data = length(short) + body(NSData*)
// the message data should delete manually
LQSocketBuffer* LQClientRequest::toMessage()
{
    // body length
    short length = m_buffer->size() + sizeof(short);
    // message data
    LQSocketBuffer *buffer = LQSocketBuffer::createWithCapacity(length);
    // append length
    buffer->writeShort(length);
    // append message body
    buffer->writeBytes(m_buffer);
    
    // message data auto release
    return buffer;
}

LQSocketBuffer* LQClientRequest::toWsMessage()
{
    // body length

    // message data auto release
    return NULL;
}

LQ_NAMESPACE_END