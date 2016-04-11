#include "LQSquadStateWait.h"

#include "LQSquadAIControl.h"

void LQSquadStateWait::enter()
{
	//LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
	//LQCombatSquad* squad = getCombatSquad();
	//soldier->attack();
};

//---------------------------------------------------------
void LQSquadStateWait::update(float dt, Layer* layer)
{
    //turn towards closest asteroid's future position, and then fire
    //LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
    LQCombatSquad* squad = getCombatSquad();
    squad->updateWaitCommand();
}

//计算状态激活水平值 >=0 激活状态 <0 状态消除
float LQSquadStateWait::calculateActivation()
{
    //LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
    LQCombatSquad* squad = getCombatSquad();
    m_activationLevel = -1;
    
    //非冲锋状态的军团没有了目标，集结等待
    if(!squad->isHasTarget() && squad->getState()!=FUSM_SQSTATE_RUN){
        m_activationLevel = 1;
    }
    return m_activationLevel;
}
