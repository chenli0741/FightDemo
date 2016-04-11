//
//  LQSkillShowingLayer.h
//  SuperWarriors
//
//  Created by lequ on 15/4/1.
//
//

#ifndef __SuperWarriors__LQSkillShowingLayer__
#define __SuperWarriors__LQSkillShowingLayer__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "../../data/LQHeroUserData.h"
#include "LQCombatHero.h"
using namespace std;
class LQSkillShowingLayer:public Layer
{
public:
    static LQSkillShowingLayer* create();
    bool init();
    
    void onEnter();
    void onExit();

    void show(LQCombatHero* herodata,LQBaseUserData* data,int type, FiniteTimeAction* endcall);
    void removeSelf();
    void removeSpriteSelf(Node* node);
    void showLabel(Node* node);
    Node* nodeSkillLable;
    virtual bool onTouchBegan(Touch* touch, Event  *event) override;
    virtual void onTouchMoved(Touch* touch, Event  *event) override;
    virtual void onTouchEnded(Touch* touch, Event  *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
};

#endif /* defined(__SuperWarriors__LQSkillShowingLayer__) */
