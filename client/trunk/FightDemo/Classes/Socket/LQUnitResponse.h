//
//  LQUnitResponse.h
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#ifndef __MyCocos2d__LQUnitResponse__
#define __MyCocos2d__LQUnitResponse__

#include "LQBaseResponse.h"

LQ_NAMESPACE

class LQUnitResponse : public LQBaseResponse {
    
public:
    LQUnitResponse(LQSocketBuffer* buffer);
    virtual ~LQUnitResponse();
    
    // read message from socket
    virtual void readMessage();
};

LQ_NAMESPACE_END

#endif /* defined(__MyCocos2d__LQUnitResponse__) */
