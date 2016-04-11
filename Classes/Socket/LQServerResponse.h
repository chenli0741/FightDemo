//
//  LQServerResponse.h
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#ifndef __MyCocos2d__LQServerResponse__
#define __MyCocos2d__LQServerResponse__

#include "LQBaseResponse.h"

using namespace std;

LQ_NAMESPACE

class LQServerResponse : public LQBaseResponse {
    
public:
    LQServerResponse();
    virtual ~LQServerResponse();
    
    // the millisecond time at the message received
    inline long long receiveTime() { return m_receiveTime; };
    
    // read message from socket
    virtual void readMessage(const char* data, size_t length);
    
    // the message code
    // even number: server response
    inline int msgCode() { return m_msgCode; };
    
    /** 是否已经处理过 */
    inline bool isProcessed() { return m_processTimes > 0; }
    
    // 增加处理次数
    inline void processMessage() { m_processTimes++; }
    
    // 是否手工维护，默认为false，需要要设置为true，需要调用 manualDelete()
    inline bool isManualDelete() { return m_isManual; }
    
    // 转为手工维护，即process之后不再自动删除，自行处理完成后必须显式delete
    inline void identifiedAsManualDelete() { m_isManual = true; }
    
protected:
    int m_msgCode;
    long long m_receiveTime;
    
    /** 处理次数 */
    int m_processTimes;
        
    /** 是否手工维护 */
    bool m_isManual;
};
LQ_NAMESPACE_END
#endif /* defined(__MyCocos2d__LQServerResponse__) */
