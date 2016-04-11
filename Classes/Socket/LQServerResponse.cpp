//
//  LQServerResponse.cpp
//  MyCocos2d
//
//  Created by ByronSong on 13-4-1.
//
//

#include "LQServerResponse.h"
//#include "../Battle/Utils/LQHelper.h"


LQ_NAMESPACE

LQServerResponse::LQServerResponse() : LQBaseResponse(), m_processTimes(0), m_isManual(false)
{
    
}

LQServerResponse::~LQServerResponse()
{
    
}

void LQServerResponse::readMessage(const char *data, size_t length)
{
    LQBaseResponse::readMessage(data, length);
    
    m_receiveTime = currentTimeMillis();
    m_msgCode = readInt();
}

LQ_NAMESPACE_END