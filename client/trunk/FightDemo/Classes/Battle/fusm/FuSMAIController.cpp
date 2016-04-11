//
// FuSMAIController.cpp
// SuperWarriors
// AI的控制类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#include "FuSMAIController.h"

//FuSM AI控制器类
FuSMAIController::FuSMAIController(AIObj* obj): AIController(obj), m_ctrlObj(obj), m_machine(NULL)
{
	m_machine = new FuSMMachine(0,this);
}

FuSMAIController::~FuSMAIController()
{
	CC_SAFE_DELETE(m_machine);
}

bool FuSMAIController::init()
{
	return true;
}

//确保控制对象存在，更新感知和状态机
void FuSMAIController::update(float dt, Layer* layer)
{
	if(!m_ctrlObj)
    {
        m_machine->reset();
        return;
    }
    
    updatePerceptions(dt);
    m_machine->updateMachine(dt,layer);
}
