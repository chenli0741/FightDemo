//
//  BattleGuideLayer.cpp
//  SuperWarriors
//
//  Created by lequdev1 on 15-4-16.
//
//

#include "BattleGuideLayer.h"
#include <math.h>
BattleGuideLayer* BattleGuideLayer::create()
{
    BattleGuideLayer* layer = new BattleGuideLayer();
    if (layer && layer->init()) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    
    return layer;
}
bool BattleGuideLayer::init()
{
    
    string LTJM_PLIST = "files/ccbiui/ccbires/resv4/V4JUQING.plist";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST.c_str());
    auto touchListener = EventListenerTouchOneByOne::create();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    touchListener->onTouchBegan = CC_CALLBACK_2(BattleGuideLayer::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(BattleGuideLayer::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(BattleGuideLayer::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto backLayer = LayerColor::create(Color4B(0,0,0,100),visibleSize.width,visibleSize.height);
    
    
    float disX = visibleSize.width*(1-0.05) - 46.5;
    float disY = visibleSize.height*0.1;
    auto point = Point(disX,disY);
    /*
    auto child = Sprite::create("res/level/rHeadStencil.png");
    child->setPosition(point);
    this->addChild(child);
     */
    TouchPoint = Point(disX,disY);
    touchLayer = LayerColor::create(Color4B(0, 0, 0, 0), 100, 100);
    touchLayer->setAnchorPoint(Point(0.5,0.5));
    touchLayer->ignoreAnchorPointForPosition(false);
    touchLayer->setPosition(point);
    this->addChild(touchLayer,5);
    
    //自动战斗
    pStencil = Sprite::create("res/level/rHeadStencil.png");
    pStencil->setPosition(point);
    pStencil->setAnchorPoint(Point(0.5, 0.5));
    pStencil->setScale(1.4f);
    auto pClip = ClippingNode::create();
    pClip->setStencil(pStencil);
    pClip->setInverted(true);
    pClip->setAlphaThreshold(0);
    pClip->addChild(backLayer);
    this->addChild(pClip);
    
    Sprite* sp1 = Sprite::createWithSpriteFrameName("yd_quanquan.png");
    
    sp1->setPosition(Point(50,50));
    sp1->setAnchorPoint(Point(0.5, 0.5));
    touchLayer->addChild(sp1,1);
    
    Sprite* sp2 = Sprite::createWithSpriteFrameName("yd_shouzhi.png");
    sp2->setAnchorPoint(Point(0.3, 1.0));
    sp2->setPosition(Point(50,50));
    RepeatForever* act1 = RepeatForever::create(Sequence::create(ScaleTo::create(0.2, 0.9),
                                                                 DelayTime::create(0.1),
                                                                 ScaleTo::create(0.28, 1),
                                                                 DelayTime::create(0.1),
                                                                 ScaleTo::create(0.32, 0.9),
                                                                 DelayTime::create(0.1),
                                                                 NULL));
    sp1->runAction(act1);
    RepeatForever* act2 = RepeatForever::create(Sequence::create(ScaleTo::create(0.2, 0.8),
                                                                 DelayTime::create(0.1),
                                                                 ScaleTo::create(0.28, 0.6),
                                                                 DelayTime::create(0.1),
                                                                 ScaleTo::create(0.32, 0.8),
                                                                 DelayTime::create(0.1),
                                                                 NULL));
    sp2->runAction(act2);
    touchLayer->addChild(sp2,2);
    //妹子
    Point point2 = Point(visibleSize.width/7 * 5,visibleSize.height/6);
    point2 = Point(150,point2.y+50);
    
    sp0 = Sprite::createWithSpriteFrameName("xinshouzhiyin.png");
    sp0->setPosition(point2);
    sp0->setAnchorPoint(Point(0.0, 0.0));
    this->addChild(sp0,4);
    talklab = Label::createWithSystemFont("点击这里可以自动战斗", "Helvetica", 24);
    talklab->enableShadow();
    talklab->setAnchorPoint(Point(0.0,0.5));
    talklab->setPosition(Point(sp0->getContentSize().width*0.5, sp0->getContentSize().height*0.22));
    sp0->addChild(talklab);
    
     
    this->setTouchEnabled(true);
    return true;
}
bool BattleGuideLayer::onTouchBegan(Touch* touch, Event  *event){
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float disX = TouchPoint.x;   //visibleSize.width*(1-0.05);
    float disY = TouchPoint.y;   //visibleSize.height*0.1;
    Point position = touch->getLocation();
    if(position.x > (disX - 50) && position.x < (disX + 50) && position.y > (disY - 50) && position.y < (disY + 50))
    {
        CCLOG("Positoinx=%f:y=%f",position.x,position.y);
        return false;
    }
    return true;
}
void BattleGuideLayer::setDataToLayer(string value,Point touchPoint,Point textPoint)
{
    TouchPoint = touchPoint;
    talklab->setString(value);
    pStencil->setPosition(touchPoint);
    touchLayer->setPosition(touchPoint);
    sp0->setPosition(textPoint);
}
void BattleGuideLayer::onTouchMoved(Touch* touch, Event  *event){}
void BattleGuideLayer::onTouchEnded(Touch* touch, Event  *event){}
void BattleGuideLayer::onTouchCancelled(Touch *touch, Event *event){}
