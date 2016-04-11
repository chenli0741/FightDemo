//
//  LQAction.h
//  LostEmpire
//
//  Created by chenli on 13-7-7.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#ifndef __LQAction_H__
#define __LQAction_H__

#include "cocos2d.h"
using namespace cocos2d;

class LQEllipseDelegate {
    
public:
    virtual void roundWithRound(float round){};
    virtual void roundToRound(float round){};
};

/** @brief ZorderTo the node
 */
class CC_DLL ZorderTo : public ActionInstant
{
public:
    /** Allocates and initializes the action */
    static ZorderTo * create(int zorder);
    
    
    //
    // Overrides
    //
    virtual void update(float time) override;
    virtual ActionInstant* reverse() const override;
	virtual ZorderTo* clone() const override;
    
protected:
    ZorderTo(){}
    virtual ~ZorderTo(){}
    bool initWithZorder(int zorder);
    
private:
    int m_zorder;
    CC_DISALLOW_COPY_AND_ASSIGN(ZorderTo);
};

/** @brief Zorder the node
 */
class CC_DLL Zorder : public ActionInstant
{
public:
    /** Allocates and initializes the action */
    static Zorder * create();
    
    
    //
    // Overrides
    //
    virtual void update(float time) override;
    virtual ActionInstant* reverse() const override;
	virtual Zorder* clone() const override;
    
protected:
    Zorder(){}
    virtual ~Zorder(){}
private:
    CC_DISALLOW_COPY_AND_ASSIGN(Zorder);
};

/*
// 定义一个结构来包含确定椭圆的参数
typedef struct lqEllipseConfig {
    //中心点坐标
    CCPoint curPosition;
    //中心点坐标
    CCPoint centerPosition;
    //椭圆a长，三角斜边
    float aLength;
    //椭圆c长，三角底边
    float cLength;
public:
    lqEllipseConfig(){};
    lqEllipseConfig(CCPoint pt,float a,float c):curPosition(pt),centerPosition(pt),aLength(a),cLength(c){};
} LQEllipseConfig;

/ **
 @LQEllipseBy  path.
 * /
class LQEllipseBy : public CCActionInterval
{
protected:
    LQEllipseConfig m_sConfig;
    CCPoint m_startPosition;
    CCPoint s_startPosition;

public:
    / ** creates an action with a Cardinal Spline array of points and tension * /
    static LQEllipseBy* create(float d, const LQEllipseConfig& c);
    //用“动作持续时间”和“椭圆控制参数”初始化动作
    bool initWithDuration(float d, const LQEllipseConfig& c);
    
    virtual void update(float time);
};

/ **
 @LQEllipseTo  path.
 * /
class LQEllipseTo : public CCActionInterval
{
protected:
    float m_round;
public:
    / ** creates an action with a Cardinal Spline array of points and tension * /
    static LQEllipseTo* create(float d, float round);
    //用“动作持续时间”和“椭圆控制参数”初始化动作
    bool initWithDuration(float d, float round);
    
    virtual void update(float time);
};
*/

#endif