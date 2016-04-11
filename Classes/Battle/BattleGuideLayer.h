//
//  BattleGuideLayer.h
//  SuperWarriors
//
//  Created by lequdev1 on 15-4-16.
//
//

#ifndef __SuperWarriors__BattleGuideLayer__
#define __SuperWarriors__BattleGuideLayer__
#include "cocos2d.h"
#include "../extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include <iostream>
#include "LQConfig.h"
using namespace std;
class BattleGuideLayer:public Layer
{
    public:
    static BattleGuideLayer* create();
    bool init();
    float PI = 3.1415926;
    bool battonTouchBegan();
    Label* talklab;
    Layer* touchLayer;
    Sprite* sp0;
    Sprite* pStencil;
    Point TouchPoint;
    void setDataToLayer(string value,Point touchPoint,Point textPoint);
    virtual bool onTouchBegan(Touch* touch, Event  *event) override;
    virtual void onTouchMoved(Touch* touch, Event  *event) override;
    virtual void onTouchEnded(Touch* touch, Event  *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
};



#endif /* defined(__SuperWarriors__BattleGuideLayer__) */
