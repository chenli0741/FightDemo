//
//  BattleReturnLayer.h
//  SuperWarriors
//
//  Created by lequdev1 on 15-4-16.
//
//

#ifndef __SuperWarriors__BattleReturnLayer__
#define __SuperWarriors__BattleReturnLayer__
#include "cocos2d.h"
#include "../extensions/cocos-ext.h"

#if !defined(LQ_DEMO_FIGHT)
#include "LoadBattlefield.h"
#endif


USING_NS_CC;
USING_NS_CC_EXT;

#include <iostream>
#include "LQConfig.h"
using namespace std;
class BattleReturnLayer:public Layer
{
    public:
    static BattleReturnLayer* create();
    bool init();
    float PI = 3.1415926;
    bool battonTouchBegan();
    void onBtnClickBack(Ref * sender, Control::EventType controlEvent);
    void onBtnClickRePlay(Ref * sender, Control::EventType controlEvent);
    virtual bool onTouchBegan(Touch* touch, Event  *event) override;
    virtual void onTouchMoved(Touch* touch, Event  *event) override;
    virtual void onTouchEnded(Touch* touch, Event  *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
};



#endif /* defined(__SuperWarriors__BattleReturnLayer__) */
