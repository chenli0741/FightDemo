//
//  LQMissAnimation.cpp
//  SuperWarriors
//
//  Created by ByronSong on 13-4-20.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#include "LQMissAnimation.h"


LQ_NAMESPACE

LQMissAnimation::LQMissAnimation() : LQCreatureBaseAnimation() {
    
}

LQMissAnimation::~LQMissAnimation() {
    
}

LQMissAnimation* LQMissAnimation::create(Sprite *creature) {
    LQMissAnimation* pRet = new LQMissAnimation();
    if (pRet && pRet->initWithCreature(creature))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

string LQMissAnimation::getSpriteFrameName()
{
    return "miss";
}

uint LQMissAnimation::getSpriteFrameSize()
{
    return 3;
}

Point LQMissAnimation::getSpriteDefaultPositionFactor()
{
    return Point(0, 0.5f);
}

LQ_NAMESPACE_END