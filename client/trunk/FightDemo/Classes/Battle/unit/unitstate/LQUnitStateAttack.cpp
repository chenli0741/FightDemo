#include "LQUnitStateAttack.h"

#include "LQUnitAIControl.h"
#include "../../LQBattleSession.h"

//战斗单元攻击状态
void LQUnitStateAttack::enter()
{
    FSMState::enter();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	LQCombatUnit* soldier = getCombatUnit();
	/*
    if(m_activationLevel==0){  //有目标，不在范围, 追踪靠近目标
        //CCLOG("Attack updateApprochCommand()..."); //
        soldier->updateApprochCommand(soldier->getTarget());
    }else*/
    soldier->_updateAttackCommand();
    //if(soldier->getIndex()==_DEBUG_INDEX)
    //    CCLOG("LQUnitStateAttack::enter... %d", soldier->getIndex());
};

//---------------------------------------------------------
void LQUnitStateAttack::update(float dt, Layer* layer)
{
    FSMState::update(dt, layer);
    //turn towards closest asteroid's future position, and then fire
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    //LQCombatUnit* soldier = getCombatUnit();
    //CCLOG("LQUnitStateAttack::update...");
}

int LQUnitStateAttack::checkTransitions()
{
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    //死亡判断逻辑
    if (!soldier->getIsDie())
        return FUSM_STATE_DIE;
    
    if (soldier->isSkilling())  //技能攻击中，保持攻击
        return FUSM_STATE_ATTACK;
    
    //索敌判断逻辑
    if (soldier->getTarget()==NULL)  //没有目标了
    {
        return FUSM_STATE_APPROACH;
    }
    //集团军团等待
    if (soldier->getSquadState()==FUSM_SQSTATE_WAIT){
        return FUSM_STATE_WAIT;
    }
    //攻击中出现的集团军团索敌, 军团阵型（特殊操作）
    if (soldier->getSquadState()==FUSM_SQSTATE_APPROACH || soldier->getSquadState()==FUSM_SQSTATE_RUN){
        CCLOG("Soldier%d: 攻击中出现的集团军团索敌, 军团阵型（特殊操作）", soldier->getIndex());
        soldier->setTarget(NULL);  //放弃进攻对象
        if( soldier->isBannerman() )
            return FUSM_STATE_APPROACH;
        else return FUSM_STATE_FOLLOW;
    }
    
    //攻击范围
    m_attack_dist = soldier->getAttackDistMax();
    //逃跑判断逻辑
    m_attack_dist_min = soldier->getAttackDistMin()*MAP_LENGTH;  //只有弓箭手有值
    //当弓箭手被锁敌满数量 或者 是军团的旗手，不能再跑了。
    if(soldier->getLockedCount()>=7 || soldier->isBannerman())
        m_attack_dist_min = 0;
    if(soldier->getTarget())
    {
#if USE_STATE_EVADE == 1
        if(m_attack_dist_min>0){
            if (parent->getTargetDist()<m_attack_dist_min)
            return FUSM_STATE_EVADE;
        }
#endif
        float _distance = soldier->getTileDistance(soldier->getTarget());
        if (_distance > m_attack_dist)
            return FUSM_STATE_APPROACH;
    }
    
    return FUSM_STATE_ATTACK;
}

/*
//---------------------------------------------------------
float LQUnitStateAttack::calculateActivation()
{
    //LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    //没有目标，目标已经死了
    if(!soldier->isHasTarget())
        return m_activationLevel = -1;
    
    if(LQBattleSession::getInstance()->getStatus()==klqBattleStatusEnd)
        return m_activationLevel = -1;
    
    if(soldier->getType()==klqLQGameObjHero){
        if(soldier->getSquad()->getSquadDirection()==klqCombatSquadDirectionLeft)
            CCLOG("LQUnitStateAttack.. %p %s", soldier, soldier->getName().c_str());
        if(soldier->getSquad()->getIndex()==4)
            CCLOG("LQUnitStateAttack.. %p %s", soldier, soldier->getName().c_str());
        CCLOG("LQUnitStateAttack.. %p %s", soldier, soldier->getName().c_str());
    }
    //攻击范围
    m_attack_dist = soldier->getAttackDistMax(); //FU_ATTACK_DIST*MAP_LENGTH;
    m_attack_dist_min = soldier->getAttackDistMin();
    if (soldier->getSquadState()!=FUSM_SQSTATE_ATTACK)        //集团进攻
        m_activationLevel = -1;
    else{
        if(!soldier->getTarget())       //还没有目标
            m_activationLevel = -1;
        else{
            float _distance = soldier->getTileDistance(soldier->getTarget());
            if(_distance <= m_attack_dist){
                if(_distance>m_attack_dist_min)  //有效范围内
                    m_activationLevel = 1;
                else    //需要退后
                    m_activationLevel = -1;
            }else
                m_activationLevel = 0;
        }
    }
    
//    if(soldier->getSquadState()==FUSM_SQSTATE_ATTACK && soldier->getType()!=klqLQGameObjHero)
//    {
//        CCLOG("LQUnitStateAttack calculateActivation:--- %f", parent->getLockedUnitDist());
//    }

    return m_activationLevel;
}*/

void LQUnitStateAttack::exit()
{
    FSMState::exit();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	//LQCombatUnit* soldier = getCombatUnit();
    //if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE && soldier->getIndex()==96)
    //    CCLOG("LQUnitStateAttack::exit 1");
    
};