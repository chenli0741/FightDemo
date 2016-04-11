#include "LQUnitStateDeath.h"

#include "LQUnitAIControl.h"

//战斗单元死亡状态
void LQUnitStateDeath::enter()
{
    FSMState::enter();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	LQCombatUnit* soldier = getCombatUnit();
    
    soldier->_updateDieCommand();
    
    if(soldier->getIndex()==_DEBUG_INDEX)
         CCLOG("LQUnitStateDeath::enter... %d", soldier->getIndex());
};

//---------------------------------------------------------
void LQUnitStateDeath::update(float dt, Layer* layer)
{
    FSMState::update(dt, layer);
    //turn towards closest asteroid's future position, and then fire
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    //LQCombatUnit* soldier = getCombatUnit();
    
    //soldier->_updateDieCommand();
    
    //CCLOG("LQUnitStateDeath::update...");
}

int LQUnitStateDeath::checkTransitions()
{
    return FUSM_STATE_DIE;
}

void LQUnitStateDeath::exit()
{
    FSMState::exit();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	//LQCombatUnit* soldier = getCombatUnit();
    
};
