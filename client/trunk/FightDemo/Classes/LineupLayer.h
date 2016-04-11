#ifndef __LineupLayer_Layer_H__
#define __LineupLayer_Layer_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LQCombatHero.h"
#include "LQCore.h"

USING_NS_CC;
USING_NS_CC_EXT;
LQ_NAMESPACE_USE;


//游戏数据类
class LQTestSession : public Node
{
protected:
    int attackHeroList[15] = {-1};
    int defenceHeroList[15] = {-1};
    
public:
    LQTestSession();
    ~LQTestSession();
    
    int getAttackHeroList(int i){ return attackHeroList[i]; }
    int getDefenceHeroList(int i){ return defenceHeroList[i]; }
    void setAttackHeroList(int i,int value){ attackHeroList[i] = value; }
    void setDefenceHeroList(int i,int value){ defenceHeroList[i] = value; }
    void clear(){
        for (int i=0 ; i<15; i++) {
            defenceHeroList[i] = -1;
            attackHeroList[i] = -1;
        }
    }
    
    static LQTestSession* getInstance();
};


class LineupLayer : public cocos2d::LayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    ~LineupLayer();
    
    void onBackButtonClicked(cocos2d::Ref *sender, Control::EventType controlEvent);
    void onButtonClicked(cocos2d::Ref *sender, Control::EventType controlEvent);

    // implement the "static create()" method manually
    CREATE_FUNC(LineupLayer);
    
    bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchMoved(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
    
    void selectSpriteForTouch(Point touchLocation);
    Point boundLayerPos(Point newPos);
    void panForTranslation(Point translation);
    
    // update per frame
    //void update(float dt);
private:
    Vector<LQCombatHero*> movableSprites;
    vector<Sprite*> boxSprites;
    LQCombatHero* selSprite;
    Point selSpritePostion;
    ActionInterval* createAnimation();
    void loadHeroResource(string soldierColor, string  soldierCode);
//    void updata();
};

#endif // __HELLOWORLD_SCENE_H__
