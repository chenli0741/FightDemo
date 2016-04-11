//
// AIController.cpp
// SuperWarriors
// AI的控制类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#include "AIController.h"

//AI控制器类
AIController::AIController(AIObj* obj):m_ctrlObj(obj), m_machine(NULL)
{
	//m_machine = new FuSMMachine(0,this);
}

AIController::~AIController()
{
	CC_SAFE_DELETE(m_machine);
}

bool AIController::init()
{
	return true;
}

//确保控制对象存在，更新感知和状态机
void AIController::update(float dt, Layer* layer)
{
	if(!m_ctrlObj)
    {
        m_machine->reset();
        return;
    }
    
    updatePerceptions(dt);
    m_machine->updateMachine(dt,layer);
}
