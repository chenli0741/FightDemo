//
//  LQHitAnimation.h
//  SuperWarriors
//
//  Created by ByronSong on 13-4-20.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#ifndef __SuperWarriors__LQHitAnimation__
#define __SuperWarriors__LQHitAnimation__

#include "LQCreatureBaseAnimation.h"

LQ_NAMESPACE

class LQHitAnimation : public LQCreatureBaseAnimation {
    
public:
    LQHitAnimation();
    ~LQHitAnimation();
    
    static LQHitAnimation* create(Sprite* creature);
    
    // 动画文件名
    string getSpriteFrameName();
    // 动画文件总数
    uint getSpriteFrameSize();
    // 默认相对坐标参数
    Point getSpriteDefaultPositionFactor();
    
};
LQ_NAMESPACE_END

#endif /* defined(__sgmz__LQHitAnimation__) */
