//
//  LQHitAnimation.cpp
//  SuperWarriors
//
//  Created by ByronSong on 13-4-20.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#include "LQHitAnimation.h"

LQ_NAMESPACE

LQHitAnimation::LQHitAnimation() : LQ::LQCreatureBaseAnimation() {
    
}

LQHitAnimation::~LQHitAnimation() {
    
}

LQHitAnimation* LQHitAnimation::create(Sprite *creature) {
    LQHitAnimation* pRet = new LQHitAnimation();
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

string LQHitAnimation::getSpriteFrameName()
{
    return "hit1_effect";
}

uint LQHitAnimation::getSpriteFrameSize()
{
    return 4;
}

Point LQHitAnimation::getSpriteDefaultPositionFactor()
{
    return Point(0, 0.3333f);
}

LQ_NAMESPACE_END