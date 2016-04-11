//
// FSMAIController.cpp
// SuperWarriors
// AI的控制类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#include "FSMAIController.h"

//FSM AI控制器类
FSMAIController::FSMAIController(AIObj* obj):AIController(obj), m_ctrlObj(obj), m_machine(NULL)
{
	m_machine = new FSMMachine(0,this);
}

FSMAIController::~FSMAIController()
{
	CC_SAFE_DELETE(m_machine);
}

bool FSMAIController::init()
{
	return true;
}

//确保控制对象存在，更新感知和状态机
void FSMAIController::update(float dt, Layer* layer)
{
	if(!m_ctrlObj)
    {
        m_machine->reset();
        return;
    }
    
    updatePerceptions(dt);
    m_machine->updateMachine(dt,layer);
}
