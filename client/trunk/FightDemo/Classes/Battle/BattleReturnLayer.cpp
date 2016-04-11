//
//  BattleReturnLayer.cpp
//  SuperWarriors
//
//  Created by lequdev1 on 15-4-16.
//
//

#include "BattleReturnLayer.h"
#include <math.h>
#include "LQBattleSession.h"
BattleReturnLayer* BattleReturnLayer::create()
{
    BattleReturnLayer* layer = new BattleReturnLayer();
    if (layer && layer->init()) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    
    return layer;
}
bool BattleReturnLayer::init()
{
    string LTJM_PLIST = "files/ccbiui/ccbires/resv4/V4UNIVERSAL.plist";
    // 半透明背景
    auto visibleSize = Director::getInstance()->getVisibleSize();
    LayerColor *cbg = LayerColor::create(Color4B(0, 0, 0, 175), visibleSize.width*2, visibleSize.height*2);
    cbg->setAnchorPoint(Point(0,0));
    cbg->setPosition(0, 0);
    this->addChild(cbg);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST.c_str());
    auto touchListener = EventListenerTouchOneByOne::create();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    touchListener->onTouchBegan = CC_CALLBACK_2(BattleReturnLayer::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(BattleReturnLayer::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(BattleReturnLayer::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    this->setTouchEnabled(true);
    
    SpriteFrame *frame =SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_continue.png");
    auto autobutton = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame));
    autobutton->setPreferredSize(Size(96,93));
    autobutton->setPosition(Point(visibleSize.width/ 2,visibleSize.height/2));
    autobutton->setAnchorPoint(Point(0.5, 0.5));
    autobutton->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleReturnLayer::onBtnClickBack),Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(autobutton);
    /*
    SpriteFrame *frame2 =SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_continue.png");
    auto autobutton2 = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame2));
    autobutton2->setPreferredSize(Size(96,93));
    autobutton2->setPosition(Point(visibleSize.width/3,visibleSize.height/2));
    autobutton2->setAnchorPoint(Point(0.5, 0.5));
    autobutton2->addTargetWithActionForControlEvents(this, cccontrol_selector(BattleReturnLayer::onBtnClickRePlay),Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(autobutton2);
    */
    return true;
}
void BattleReturnLayer::onBtnClickBack(Ref * sender, Control::EventType controlEvent)
{
#if !defined(LQ_DEMO_FIGHT)
    // 退出游戏
    LoadBattlefield::getInstance()->combatBack();
#endif
}
void BattleReturnLayer::onBtnClickRePlay(Ref * sender, Control::EventType controlEvent)
{
#if !defined(LQ_DEMO_FIGHT)
    std::string str = LQBattleSession::getInstance()->getBattleValueStr();
    //LoadBattlefield::getInstance()->combatBack();
    //LoadBattlefield::getInstance()->startLoadBattle(str);
    LoadBattlefield::getInstance()->replayBattle(str);
#endif
}
bool BattleReturnLayer::onTouchBegan(Touch* touch, Event  *event){
    return true;
}
void BattleReturnLayer::onTouchMoved(Touch* touch, Event  *event){}
void BattleReturnLayer::onTouchEnded(Touch* touch, Event  *event){}
void BattleReturnLayer::onTouchCancelled(Touch *touch, Event *event){}
