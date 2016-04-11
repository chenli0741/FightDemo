//
// FuSMState.h
// SuperWarriors
//
// 状态基类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#ifndef __Lequworld_SuperWarriors__FUSMSTATE__
#define __Lequworld_SuperWarriors__FUSMSTATE__

#include "cocos2d.h"
#include "FSMEnum.h"
#include "FState.h"
#include "FMachine.h"

USING_NS_CC;

class AIController;

//状态基类
class FuSMState : public FState
{
public:
    //constructor/functions
    FuSMState(int type = 0,AIController* parent = NULL){
		m_type = type;
		m_parent = parent;
		m_activationLevel = 0.0f;
	};
    //virtual void enter()            {};
    //virtual void exit()             {};
    //virtual void update(float dt, Layer* layer)   {};
    virtual bool init()             {m_activationLevel = 0.0f; return true;};
	//计算状态激活水平值 >=0 激活状态 <0 状态消除
    virtual float calculateActivation() {return m_activationLevel;};

	//FuM状态机是通过m_activationLevel值来控制状态变化
    virtual void checkLowerBound(float lbound = 0.0f) {if(m_activationLevel < lbound) m_activationLevel = lbound;};
    virtual void checkUpperBound(float ubound = 1.0f) {if(m_activationLevel > ubound) m_activationLevel = ubound;};
    virtual void checkBounds(float lb = 0.0f,float ub = 1.0f) {checkLowerBound(lb);checkUpperBound(ub);};

    //AI控制的对象数据
	//CC_SYNTHESIZE_READONLY(AIController*, m_parent, Parent)
	//状态类型
	//CC_SYNTHESIZE(int, m_type, Type)
	//状态激活水平
	CC_SYNTHESIZE_READONLY(float, m_activationLevel, ActivationLevel)
private:

};

#endif