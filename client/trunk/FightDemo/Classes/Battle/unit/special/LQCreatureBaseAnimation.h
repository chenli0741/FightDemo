//
//  LQCreatureBaseAnimation.h
//  SuperWarriors
//
//  Created by ByronSong on 13-4-20.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#ifndef __SuperWarriors__LQCreatureBaseAnimation__
#define __SuperWarriors__LQCreatureBaseAnimation__

#include "LQStringExtend.h"
#include "LQCombatUnitDefine.h"

LQ_NAMESPACE

class LQCreatureBaseAnimation : public Sprite {
    CC_SYNTHESIZE_PASS_BY_REF(Point, m_adjustPosition, AdjustPosition);
    CC_SYNTHESIZE_RETAIN(Sprite*, m_sprite, Sprite);
    CC_SYNTHESIZE_RETAIN(Action*, m_curAction, CurAction);
    CC_SYNTHESIZE_RETAIN(CallFunc*, m_finishCall, FinishCall);
    CC_SYNTHESIZE_RETAIN(Animation*, m_curAnimation, CurAnimation);
    CC_SYNTHESIZE_READONLY(int, m_repeatCounter, RepeatCounter);
public:
    LQCreatureBaseAnimation();
    virtual ~LQCreatureBaseAnimation();
    
    virtual bool initWithCreature(Sprite* creature);
    
    virtual void display();

    // override
    virtual void restart();
    virtual void finishTween();
    
    // 动画文件名
    virtual string getSpriteFrameName() = 0;
    // 动画文件总数
    virtual uint getSpriteFrameSize() = 0;
    // 默认相对坐标参数
    virtual Point getSpriteDefaultPositionFactor() = 0;
    
protected:
    //void runxxx(Animation *animation, int repeatCounter);
    virtual Animation* getSpriteAnimationFromSpriteFrameName(const string& fileName);
    
};


LQ_NAMESPACE_END

#endif /* defined(__sgmz__LQCreatureBaseAnimation__) */
