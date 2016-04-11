//
//  LQMissAnimation.h
//  SuperWarriors
//
//  Created by ByronSong on 13-4-20.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#ifndef __SuperWarriors__LQMissAnimation__
#define __SuperWarriors__LQMissAnimation__

#include "LQCreatureBaseAnimation.h"

LQ_NAMESPACE

class LQMissAnimation : public LQCreatureBaseAnimation {
    
public:
    LQMissAnimation();
    ~LQMissAnimation();
    
    static LQMissAnimation* create(Sprite* creature);
    
    // 动画文件名
    string getSpriteFrameName();
    // 动画文件总数
    uint getSpriteFrameSize();
    // 默认相对坐标参数
    Point getSpriteDefaultPositionFactor();
    
};
LQ_NAMESPACE_END

#endif /* defined(__sgmz__LQMissAnimation__) */
