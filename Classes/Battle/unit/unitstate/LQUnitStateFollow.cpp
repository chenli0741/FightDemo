#include "LQUnitStateFollow.h"

#include "LQUnitAIControl.h"

//战斗单元跟随状态
void LQUnitStateFollow::enter()
{
    FSMState::enter();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	LQCombatUnit* soldier = getCombatUnit();
	//士兵跟随武将
    soldier->_updateFollowCommand();
    //if(soldier->getIndex()==_DEBUG_INDEX)
    //    CCLOG("LQUnitStateFollow::enter... %d", soldier->getIndex());
};

//---------------------------------------------------------
void LQUnitStateFollow::update(float dt, Layer* layer)
{
    FSMState::update(dt, layer);
    //turn towards closest asteroid's future position, and then fire
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    //LQCombatUnit* soldier = getCombatUnit();
    
    
    //CCLOG("LQUnitStateFollow::update...");
}


int LQUnitStateFollow::checkTransitions()
{
    LQCombatUnit* soldier = getCombatUnit();
    
    //if(soldier->getIndex()==45)
    //    CCLOG("LQUnitStateFollow update.. %p %d", soldier, soldier->getIndex());
    
    //死亡判断逻辑
    if (!soldier->getIsDie())
        return FUSM_STATE_DIE;
    
    if (soldier->isSkilling())  //技能攻击中，保持攻击
        return FUSM_STATE_ATTACK;
    
    //if (soldier->getSquadState()==FUSM_SQSTATE_RUN)
    //    CCLOG("FUSM_SQSTATE_RUN...");
    //集团军团等待
    if (soldier->getSquadState()==FUSM_SQSTATE_WAIT && soldier->isArrayed())
        return FUSM_STATE_WAIT;
    //集团军团攻击，开始单元索敌
    if (soldier->getSquadState()==FUSM_SQSTATE_ATTACK)
        return FUSM_STATE_APPROACH;
    
    return FUSM_STATE_FOLLOW;
}

/*
//---------------------------------------------------------
float LQUnitStateFollow::calculateActivation()
{
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    if (soldier->getSquadState()==FUSM_SQSTATE_RUN)      //集团行军
        m_activationLevel = 1;
    else m_activationLevel = -1;

    return m_activationLevel;
}*/

void LQUnitStateFollow::exit()
{
    FSMState::exit();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	//LQCombatUnit* soldier = getCombatUnit();
    
};
