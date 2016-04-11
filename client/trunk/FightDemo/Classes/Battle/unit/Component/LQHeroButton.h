//
//  LQHeroButton.h
//  game
//
//  Created by lequdev1 on 14-7-21.
//
//

#ifndef __game__LQHeroButton__
#define __game__LQHeroButton__

#include <iostream>
#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
#include "LQStringExtend.h"

#include "LQData.h"
#include "LQCombatHero.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LQHeroButton:public ControlButton
{
public:
    LQHeroButton();
    ~LQHeroButton();
    static LQHeroButton* createHeroButtonWithData(LQCombatHero* hero);
    virtual bool init();
    bool initWithHeroButtonData(LQCombatHero* hero);
    
    CC_SYNTHESIZE(LQCombatHero*, m_hero, Hero);//对应的英雄
    CC_SYNTHESIZE(LQHeroUserData*, m_heroData, HeroData);    // 对应的英雄的数据
    CC_SYNTHESIZE(LQSquadUserData*, m_squadData, SquadData); // 对应的兵团的数据
    CC_SYNTHESIZE(bool, m_isHeroDie, IsHeroDie);    //英雄是否已经阵亡

    void updataButton();//用于刷新英雄的血量和怒气值
    
    Label* hpLabel;
    Sprite* m_headiconSp;
    Sprite* m_selectedSprite;
    Sprite* m_freeSprite;
    Sprite* m_hurtsSprite;
    CC_SYNTHESIZE_READONLY(bool, m_isSelected, IsSelected);
    void setSelected(bool isSelected);
    CC_SYNTHESIZE_READONLY(bool, m_isCanFreeSkill, IsCanFreeSkill)   //是否可以释放技能
    void setIsCanFreeSkill(bool isCanFreeSkill);
    
    void setHeroDie();
private:
    ControlSlider* m_hpSlider;//血条
    ControlSlider* m_furySlider;//怒气条
    ActionInterval* createAnimation();
    
    void onEnter();
    void onExit();
    
    //将获取英雄血提出，好修改取数方法12.18
    int getCurrHpValue();

};
#endif /* defined(__game__LQHeroButton__) */
