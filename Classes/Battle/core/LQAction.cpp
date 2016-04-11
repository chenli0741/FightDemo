//
//  LQAction.cpp
//  LostEmpire
//
//  Created by chenli on 13-7-7.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "LQAction.h"

using namespace cocos2d;


//
// ZorderTo
//
ZorderTo* ZorderTo::create(int zorder)
{
    ZorderTo* ret = new ZorderTo();
    
    if (ret) {
        ret->initWithZorder(zorder);
        ret->autorelease();
    }
    
    return ret;
}

void ZorderTo::update(float time) {
    CC_UNUSED_PARAM(time);
    _target->getParent()->reorderChild(_target,m_zorder);
}

ActionInstant* ZorderTo::reverse() const
{
    return NULL;
}

//用zorder初始化动作
bool ZorderTo::initWithZorder(int zorder)
{
    m_zorder = zorder;
    return true;
}

ZorderTo * ZorderTo::clone() const
{
	// no copy constructor
	auto a = new ZorderTo();
    a->initWithZorder(m_zorder);
	a->autorelease();
	return a;
}

//
// Zorder
//
Zorder* Zorder::create()
{
    Zorder* ret = new Zorder();
    
    if (ret) {
        ret->autorelease();
    }
    
    return ret;
}

void Zorder::update(float time) {
    CC_UNUSED_PARAM(time);
    _target->getParent()->reorderChild(_target,900-_target->getPositionY());
}

ActionInstant* Zorder::reverse() const
{
    return NULL;
}

Zorder * Zorder::clone() const
{
	// no copy constructor
	auto a = new Zorder();
	a->autorelease();
	return a;
}

/*
//
// LQCardinalSplineTo
//
LQCardinalSplineTo* LQCardinalSplineTo::create(float duration, cocos2d::CCPointArray *points, float tension)
{
    LQCardinalSplineTo *ret = new LQCardinalSplineTo();
    if (ret)
    {
        if (ret->initWithDuration(duration, points, tension))
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }
    
    return ret;
}

void LQCardinalSplineTo::update(float time) {
    CC_UNUSED_PARAM(time);
    CCCardinalSplineTo::update(time);
    m_pTarget->setZOrder(900-m_pTarget->getPositionY());
}


//
//LQEllipseBy
//
LQEllipseBy* LQEllipseBy::create(float d, const LQEllipseConfig& c)//利用之前定义的椭圆的三个参数初始化椭圆
{
    LQEllipseBy *pTuoyuanBy = new LQEllipseBy();
    pTuoyuanBy->initWithDuration(d, c);
    pTuoyuanBy->autorelease();
    
    return pTuoyuanBy;
}

bool LQEllipseBy::initWithDuration(float d, const LQEllipseConfig& c)
{
    if (CCActionInterval::initWithDuration(d))
    {
        m_sConfig = c;
        return true;
    }
    
    return false;
}

void LQEllipseBy::update(float time)
{
    if (m_pTarget)
    {
        CCPoint s_startPosition =m_sConfig.centerPosition;//中心点坐标
        float x = m_sConfig.aLength*sin(2*3.1415926*time);
        float y = m_sConfig.cLength*cos(2*3.1415926*time);
        m_pTarget->setPosition(ccpAdd(s_startPosition, ccp(x, y)));
        m_pTarget->setUserData(CCFloat::create(time));
        m_pTarget->setZOrder(900-m_pTarget->getPositionY());
    }
}

//
//LQEllipseTo
//
LQEllipseTo* LQEllipseTo::create(float d, float round)//利用之前定义的椭圆的三个参数初始化椭圆
{
    LQEllipseTo *pTuoyuanTo = new LQEllipseTo();
    pTuoyuanTo->initWithDuration(d,round);
    pTuoyuanTo->autorelease();
    
    return pTuoyuanTo;
}

bool LQEllipseTo::initWithDuration(float d, float round)
{
    if (CCActionInterval::initWithDuration(d))
    {
        m_round = round;
        return true;
    }
    
    return false;
}

void LQEllipseTo::update(float time)
{
    if (m_pTarget)
    {
        ((LQEllipseDelegate*)m_pTarget)->roundWithRound(time);
    }
}

*/