//
//  LQInnerClassResponse.h
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#ifndef __MyCocos2d__LQInnerClassResponse__
#define __MyCocos2d__LQInnerClassResponse__

#include "LQBaseResponse.h"

LQ_NAMESPACE
class LQInnerClassResponse : public LQBaseResponse {
    
public:
    LQInnerClassResponse(LQSocketBuffer* buffer);
    virtual ~LQInnerClassResponse();
    
    // read message from socket
    virtual void readMessage();
};

LQ_NAMESPACE_END

#endif /* defined(__MyCocos2d__LQInnerClassResponse__) */
