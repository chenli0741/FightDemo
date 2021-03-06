//
// FSMAIController.h
// SuperWarriors
// FSM AI的控制类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#ifndef __Lequworld_SuperWarriors_FSMAIController_H__
#define __Lequworld_SuperWarriors_FSMAIController_H__


#include "cocos2d.h"
#include "AIObj.h"
#include "FSMMachine.h"
#include "AIController.h"

USING_NS_CC;

// FSM AI控制器类
class FSMAIController : public AIController
{
public:
	//constructor/functions
	FSMAIController(AIObj* obj);
	~FSMAIController();
	// AI的统一环境数据计算方法，有动作就有updatePerceptions
	virtual void updatePerceptions(float dt){};
	virtual void update(float dt, Layer* layer);
	virtual bool init();
	
	//AI控制器的状态机
	CC_SYNTHESIZE_READONLY(FSMMachine*, m_machine, Machine);
	//AI控制的对象数据
	CC_SYNTHESIZE_READONLY(AIObj*, m_ctrlObj, ControlObj)
	void setControlObj(AIObj* obj)  {m_ctrlObj = obj; m_ctrlObj->setController(this);}
    
};
#endif