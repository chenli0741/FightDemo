//
//  LQCreatureBaseAnimation.cpp
//  SuperWarriors
//
//  Created by ByronSong on 13-4-20.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#include "LQCreatureBaseAnimation.h"

LQ_NAMESPACE

LQCreatureBaseAnimation::LQCreatureBaseAnimation() :
m_sprite(NULL), m_curAction(NULL), m_finishCall(NULL), m_curAnimation(NULL), m_repeatCounter(1){
    
}

LQCreatureBaseAnimation::~LQCreatureBaseAnimation() {
    if(m_sprite)
        m_sprite->removeFromParentAndCleanup(true);
    
    CC_SAFE_RELEASE(m_curAction);
    CC_SAFE_RELEASE(m_curAnimation);
    CC_SAFE_RELEASE(m_finishCall);
    CC_SAFE_RELEASE(m_sprite);
}

bool LQCreatureBaseAnimation::initWithCreature(Sprite *creature) {
    string fileName;
    stringf(fileName, "%s%03d.png", getSpriteFrameName().c_str(), 0);
    Sprite *sprite = CCSprite::createWithSpriteFrameName(fileName.c_str());
    
    Point anchorPoint = Point(0.5f, 0.5f);
    Animation* animation = getSpriteAnimationFromSpriteFrameName(getSpriteFrameName());
    //CCLOG("Animation %p %d", animation, animation->getReferenceCount());
    setCurAnimation(animation);
    setAnchorPoint(anchorPoint);
    // 设置相对坐标
    Point positionFactor = getSpriteDefaultPositionFactor();
    const Size& spriteSize = creature->getTextureRect().size;
    float posX = creature->getContentSize().width * creature->getAnchorPoint().x + spriteSize.width * positionFactor.x;
    float posY = creature->getContentSize().height * creature->getAnchorPoint().y + spriteSize.height * positionFactor.y;
    sprite->setPosition(Point(posX, posY));
    sprite->setAnchorPoint(anchorPoint);
    creature->addChild(sprite);
    return true;
}
/*
void LQCreatureBaseAnimation::runxxx(cocos2d::Animation *animation, int repeatCounter) {
    // 停止之前动作
    m_sprite->stopActionByTag(SPRITE_SPEDD_ACTION_TAG);
    
    Animate* animAction = CCAnimate::create(animation);
    if (m_repeatCounter <= 0) {
        // 无限循环
        setCurAction(CCSpeed::create(RepeatForever::create(animAction), LQBattleManager::getInstance()->getGameSpeed()));
    }else {
        //setFinishCall(CCCallFunc::create(this, callfunc_selector(LQRasterAnimation::animationDidStop)));
        Repeat *repeatAction = Repeat::create(animAction, m_repeatCounter);
        Sequence *sequenceAction = Sequence::create(repeatAction, m_finishCall, nil);
        Speed *speedAction = Speed::create(sequenceAction, LQBattleManager::getInstance()->getGameSpeed());
        setCurAction(speedAction);
    }
    //m_sprite->setAnchorPoint(m_anchorPoint);
    m_curAction->setTag(SPRITE_SPEDD_ACTION_TAG);
    m_sprite->runAction(m_curAction);
}*/


void LQCreatureBaseAnimation::display()
{
    restart();
}

void LQCreatureBaseAnimation::restart()
{
    m_sprite->setVisible(true);
}

void LQCreatureBaseAnimation::finishTween()
{
    m_sprite->setVisible(false);
}

Animation* LQCreatureBaseAnimation::getSpriteAnimationFromSpriteFrameName(const string& fileName)
{
    Animation *animation = Animation::create();
    
    int size = getSpriteFrameSize();
    const char* cFileName = fileName.c_str();
    for(int i = 0; i < size; ++i) {
        string file;
        stringf(file, "%s%03d.png", cFileName, i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(file.c_str());
        animation->addSpriteFrame(frame);
    }
    // delayPerUnit means the delay to add to each frame (exactly the same as the old delay)
    animation->setDelayPerUnit(0.08);
    
    return animation;
}


LQ_NAMESPACE_END