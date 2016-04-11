//
//  LQInnerClassResponse.cpp
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#include "LQInnerClassResponse.h"

LQ_NAMESPACE

LQInnerClassResponse::LQInnerClassResponse(LQSocketBuffer *buffer) : LQBaseResponse(buffer)
{
    readMessage();
}

LQInnerClassResponse::~LQInnerClassResponse()
{
    
}

void LQInnerClassResponse::readMessage()
{
    
}

LQ_NAMESPACE_END