#include "LQUnitStateWait.h"

#include "LQUnitAIControl.h"
#include "../../LQBattleSession.h"

//战斗单元集结巡逻状态
void LQUnitStateWait::enter()
{
    FSMState::enter();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	LQCombatUnit* soldier = getCombatUnit();
	soldier->_updateWaitCommand();
    if(soldier->getIndex()==_DEBUG_INDEX)
        CCLOG("LQUnitStateWait::enter... %d", soldier->getIndex());
};

//---------------------------------------------------------
void LQUnitStateWait::update(float dt, Layer* layer)
{
    FSMState::update(dt, layer);
    //turn towards closest asteroid's future position, and then fire
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    //LQCombatUnit* soldier = getCombatUnit();
    
    //CCLOG("LQUnitStateWait::update...");
}

int LQUnitStateWait::checkTransitions()
{
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    //if(soldier->getIndex()==45)
    //    CCLOG("LQUnitStateWait update.. %p %d", soldier, soldier->getIndex());
    
    //死亡判断逻辑
    if (!soldier->getIsDie())
        return FUSM_STATE_DIE;
    
    if (soldier->isSkilling())  //技能攻击中，保持攻击
        return FUSM_STATE_ATTACK;
    
    //集团军团攻击
    if (soldier->getSquadState()==FUSM_SQSTATE_ATTACK)
        return FUSM_STATE_APPROACH;
    //集团军团索敌, 军团阵型
    if (soldier->getSquadState()==FUSM_SQSTATE_APPROACH || soldier->getSquadState()==FUSM_SQSTATE_RUN){
        if( soldier->isBannerman() )
            return FUSM_STATE_APPROACH;
        else return FUSM_STATE_FOLLOW;
    }
    
    //攻击范围
    m_attack_dist = soldier->getAttackDistMax(); //FU_ATTACK_DIST*MAP_LENGTH;
    m_attack_dist_min = soldier->getAttackDistMin();
    //攻击状态转换逻辑
    if(soldier->getTarget())
    {
        float _distance = soldier->getTileDistance(soldier->getTarget());
        if(_distance <= m_attack_dist){
            if(_distance>m_attack_dist_min)  //有效范围内
                return FUSM_STATE_ATTACK;
#if USE_STATE_EVADE == 1
            else if(m_attack_dist_min>0)     //逃跑范围
                return FUSM_STATE_EVADE;  //;
#else
            else if(m_attack_dist_min>0)     //近战范围
                return FUSM_STATE_ATTACK;
#endif
        }
    }
    
    return FUSM_STATE_WAIT;
}

/*
//---------------------------------------------------------
float LQUnitStateWait::calculateActivation()
{
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    if(LQBattleSession::getInstance()->getStatus()==klqBattleStatusEnd)
        return m_activationLevel = 1;
    
    m_activationLevel = -1;
    if (soldier->getSquadState()==FUSM_SQSTATE_PATROL)      //集团集结巡逻
        m_activationLevel = 1;
    else if (soldier->getSquadState()==FUSM_SQSTATE_WAIT)   //集团军团等待
        m_activationLevel = 1;
    else if(!soldier->isHasTarget() &&                      //没有了目标
            (soldier->getState()==FUSM_STATE_ATTACK || soldier->getState()==FUSM_STATE_PAREOL))         m_activationLevel = 1;
    else m_activationLevel = -1;
    
    return m_activationLevel;
}*/

void LQUnitStateWait::exit()
{
    FSMState::exit();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	//LQCombatUnit* soldier = getCombatUnit();

};
