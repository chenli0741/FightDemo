//
// FSMState.h
// SuperWarriors
//
// 状态基类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#ifndef __Lequworld_SuperWarriors__FSMSTATE__
#define __Lequworld_SuperWarriors__FSMSTATE__

#include "FSMEnum.h"
#include "FState.h"
#include <stdlib.h>

class AIController;

class FSMState : public FState
{
public:
	//constructor/functions
	FSMState(int type = 0,AIController* parent = NULL){m_type = type;m_parent = parent; m_interval = -1; m_ctrlinterval = 0; }
    //virtual void enter()            {}      //进入状态时运行
	//virtual void exit()             {}      //离开状态时运行
	//virtual void update(float dt, Layer* layer)   {}      //如果该状态是当前状态，在AI每次处理中都会调用
	//virtual bool init()             {return true;}      //对状态的复位
	virtual int  checkTransitions() {return 0;} //用于处理决定状态结束的逻辑，将返回运行状态的枚举，如果不需改变，返回原状态

    //AI控制的对象数据
	//CC_SYNTHESIZE_READONLY(AIController*, m_parent, Parent)
	//状态类型
	//CC_SYNTHESIZE(int, m_type, Type)
};
#endif