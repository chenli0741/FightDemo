#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LQHeroUserData.h"
#include "LQCombatHero.h"
#include "LQMissile.h"

USING_NS_CC_EXT;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void OnAutoButtonClicked(cocos2d::Ref *sender, Control::EventType controlEvent);
    void OnAutoButton2Clicked(cocos2d::Ref *sender, Control::EventType controlEvent);
    void OnSkillButtonClicked(cocos2d::Ref *sender, Control::EventType controlEvent);
    void OnLineupButtonClicked(cocos2d::Ref *sender, Control::EventType controlEvent);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    // update per frame
    void update(float dt);
private:
    LQHeroUserData* m_herodata;
    LQHeroUserData* m_targetdata;
    LQSkillUserData* m_skilldata;
    LQCombatHero* m_hero;
    LQCombatHero* m_target;
    Sprite* shootsp;
    LQMissile* missileSp;
    int resid;
    int skillid;
    int skillidadd;
    int skillid0;
    Label* ttf0;
    
    int _testHeroId;
    int _animPhaseEnum;
    int _directionType;
    int _testsize;
    int _stestsize;
    int m_soldierIDC;
    //
    Vector<Node*> _anodes;
    
    ActionInterval* createAnimation();
    
    void shootEnd(Node * node);
    void skillHitEffect();
    
    void loadHeroResource(string soldierColor, string  soldierCode);
//    void updata();
};

#endif // __HELLOWORLD_SCENE_H__
