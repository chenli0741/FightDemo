#include "LQUnitStateEvade.h"

#include "LQUnitAIControl.h"

//战斗单元撤退状态
void LQUnitStateEvade::enter()
{
    FSMState::enter();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	LQCombatUnit* soldier = getCombatUnit();
	soldier->_updateEvadeCommand();
    if(soldier->getIndex()==_DEBUG_INDEX)
        CCLOG("LQUnitStateEvade::enter...%d", soldier->getIndex());
};

//---------------------------------------------------------
void LQUnitStateEvade::update(float dt, Layer* layer)
{
    FSMState::update(dt, layer);
    //turn towards closest asteroid's future position, and then fire
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    //LQCombatUnit* soldier = getCombatUnit();
    
    //CCLOG("LQUnitStateEvade::update...");
}


int LQUnitStateEvade::checkTransitions()
{
    LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    //死亡判断逻辑
    if (!soldier->getIsDie())
        return FUSM_STATE_DIE;
    
    //逃跑中出现的集团军团索敌, 军团阵型
    if (soldier->getSquadState()==FUSM_SQSTATE_APPROACH || soldier->getSquadState()==FUSM_SQSTATE_RUN){
        CCLOG("Soldier%d: 逃跑中出现的集团军团索敌, 军团阵型（特殊操作）", soldier->getIndex());
        soldier->setTarget(NULL);  //放弃进攻对象
        if( soldier->isBannerman() )
            return FUSM_STATE_APPROACH;
        else return FUSM_STATE_FOLLOW;
    }
    
    //逃跑判断逻辑
    m_attack_dist_min = soldier->getAttackDistMin()*MAP_LENGTH;  //只有弓箭手有值
    //当弓箭手被锁敌满数量 或者 是军团的旗手，不能再跑了。
    if(soldier->getLockedCount()>=7 || soldier->isBannerman())
        m_attack_dist_min = 0;
    if(soldier->getTarget() && soldier->getTarget()->getActive())
    {
        if (parent->getTargetDist()>=m_attack_dist_min)
            return FUSM_STATE_ATTACK;
    }else
        return FUSM_STATE_APPROACH;
    
    
    return FUSM_STATE_EVADE;
}

/*
//---------------------------------------------------------
float LQUnitStateEvade::calculateActivation()
{
    LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    m_activationLevel = -1;
    m_attack_dist_min = soldier->getAttackDistMin()*MAP_LENGTH;  //只有弓箭手有值
    
    //当弓箭手被锁敌满数量 或者 是军团的旗手，不能再跑了。
    if(soldier->getLockedCount()>=7 || soldier->isBannerman())
        m_attack_dist_min = 0;
    
    
    //逃跑范围
    if(parent->getLockedUnit() && m_attack_dist_min>0)
    {
        if (parent->getLockedUnitDist()<m_attack_dist_min){
            m_activationLevel = 1;
        }
    }
    return m_activationLevel;
}
*/
void LQUnitStateEvade::exit()
{
    FSMState::exit();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	//LQCombatUnit* soldier = getCombatUnit();
    
};
