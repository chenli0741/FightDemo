//
//  LQClientRequest.h
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#ifndef MyCocos2d_LQClientRequest_h
#define MyCocos2d_LQClientRequest_h

#include "../Battle/core/LQCore.h"
#include "LQBaseRequest.h"


LQ_NAMESPACE

// 客户端请求消息基类
class LQClientRequest : public LQBaseRequest {
    
public:
    LQClientRequest();
    virtual ~LQClientRequest();
    
    // the message data = length(short) + body(NSData*)
    // the message data should delete manually
    virtual LQSocketBuffer* toMessage();
    virtual LQSocketBuffer* toWsMessage();
    
    inline int msgCode() const { return m_msgCode; }
    inline void setMsgCode(int msgCode) { m_msgCode = msgCode; writeInt(msgCode); }
    inline int msgLength() const { return m_buffer->size(); }
    
private:
    int m_msgCode;
};

LQ_NAMESPACE_END

#endif
