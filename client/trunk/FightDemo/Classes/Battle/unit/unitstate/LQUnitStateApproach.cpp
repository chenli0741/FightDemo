#include "LQUnitStateApproach.h"

#include "LQUnitAIControl.h"
#include "../../LQBattleSession.h"

//战斗单元索敌状态
void LQUnitStateApproach::enter()
{
    FSMState::enter();
	LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	LQCombatUnit* soldier = getCombatUnit();
    
    m_lockedunit = parent->getLockedUnit();
    //索敌
    soldier->_updateApprochCommand(m_lockedunit);
    if(soldier->getIndex()==_DEBUG_INDEX)
        CCLOG("LQUnitStateApproach::enter... %d", soldier->getIndex());
};

//---------------------------------------------------------
void LQUnitStateApproach::update(float dt, Layer* layer)
{
    FSMState::update(dt, layer);
    //turn towards closest asteroid's future position, and then fire
    LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    //if(soldier->getIndex()==45)
    //    CCLOG("LQUnitStateApproach update.. %p %d", soldier, soldier->getIndex());
    //没有敌人索敌
    if(!soldier->getTarget()){
        m_lockedunit = parent->getLockedUnit();
        soldier->_updateApprochCommand(m_lockedunit);
        //soldier->setTarget(m_lockedunit);
        //soldier->setIsStand(false);
    }else{
        //CCLOG("IDX%d: 靠近敌人 %d %s->%s", soldier->getIndex(), soldier->getTarget()->getIndex(), PointString(soldier->getPosition()).c_str(),
        //      PointString(soldier->getTarget()->getPosition()).c_str() );
    }
    
    //存在追踪目标点 或者 存在追踪路径
    if( soldier->getTargetPoint()!=Point::ZERO || soldier->getMustPoints().size()>0 || soldier->getWayPoints().size()>0 )
    {
        //目标点得前进，不管后面索敌
        soldier->_updateMoveCommand();
        return;
    }
    
    //CCLOG("LQUnitStateApproach::update...");
}

int LQUnitStateApproach::checkTransitions()
{
    LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    //死亡判断逻辑
    if (!soldier->getIsDie())
        return FUSM_STATE_DIE;
    
    if (soldier->isSkilling())  //技能攻击中，保持攻击
        return FUSM_STATE_ATTACK;
    
    //集团军团等待
    if (soldier->getSquadState()==FUSM_SQSTATE_WAIT){
        if (soldier->isSkilling())  //技能攻击中，保持攻击
            return FUSM_STATE_ATTACK;
        return FUSM_STATE_WAIT;
    }
    //集团军团索敌, 军团阵型
    if (soldier->getSquadState()==FUSM_SQSTATE_APPROACH || soldier->getSquadState()==FUSM_SQSTATE_RUN){
        if( !soldier->isBannerman() )
            return FUSM_STATE_FOLLOW;
        return FUSM_STATE_APPROACH;
    }
    
    if(soldier->getIndex()==_DEBUG_INDEX)
        CCLOG("LQUnitStateApproach::checkTransitions... %d", soldier->getIndex());
    
    //攻击范围
    m_attack_dist = soldier->getAttackDistMax(); //FU_ATTACK_DIST*MAP_LENGTH;
    m_attack_dist_min = soldier->getAttackDistMin();
    //攻击状态转换逻辑
    if(soldier->getTarget())
    {
        float _distance = soldier->getTileDistance(soldier->getTarget());
        float _adjust_dist = 0;
        //if( !soldier->getTarget()->isStand() )
        //    _adjust_dist = 1;
        if(_distance <= m_attack_dist + _adjust_dist){
            //if(soldier->getUnitType()==5 && soldier->getSquadDirection()==klqCombatSquadDirectionLeft)
            //    CCLOG("弓箭兵 1.... _distance:%f %p", _distance, soldier->getTarget());
            
            if(_distance>m_attack_dist_min)  //有效范围内
            {
                if( soldier->doStand(true) && soldier->getIsCanAttack() )  //站住了才能攻击
                {
                    //if(soldier->getUnitType()==5 && soldier->getSquadDirection()==klqCombatSquadDirectionLeft)
                    //    CCLOG("弓箭兵 2.... _distance:%f", _distance);
                    return FUSM_STATE_ATTACK;
                }
                //可以攻击了但不能站住，怎么办？
                //???
                //CCLOG("IDX%d:可以攻击了但不能站住，怎么办 UnitType:%d ", soldier->getIndex(), soldier->getUnitType());
            }
#if USE_STATE_EVADE == 1
            else if(m_attack_dist_min>0)     //逃跑范围
                return FUSM_STATE_EVADE;
#else
            else if(m_attack_dist_min>0)     //近战范围
                return FUSM_STATE_ATTACK;
#endif
        }
    }else{
        //索敌对象是在索定军团中找的
        m_lockedunit = parent->getLockedUnit();
        //没有敌人,什么情况？
        if(m_lockedunit==NULL){
            if(soldier->getMustPoints().size()<=0 && soldier->getWayPoints().size()<=0)  //又没有路径，只能停止等待了
                return FUSM_STATE_WAIT;
            //CCLOG("IDX%d:没有索到敌人,什么情况？", soldier->getIndex());
        }
    }
    
    return FUSM_STATE_APPROACH;
}

/*
//---------------------------------------------------------
float LQUnitStateApproach::calculateActivation()
{
    LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
    LQCombatUnit* soldier = getCombatUnit();
    
    if(soldier->getIndex() == 135){
        CCLOG("LQUnitStateApproach 1.. %p %s", soldier, soldier->getName().c_str());
    }
    
    if(LQBattleSession::getInstance()->getStatus()==klqBattleStatusEnd)
        return m_activationLevel = -1;
    
    if(soldier->getType()==klqLQGameObjHero){
        if(soldier->getSquad()->getSquadDirection()==klqCombatSquadDirectionLeft)
            CCLOG("LQUnitStateAttack.. 2 %p %s", soldier, soldier->getName().c_str());
        if(soldier->getSquad()->getIndex()==4)
            CCLOG("LQUnitStateApproach.. %p %d", soldier, soldier->getIndex());
        CCLOG("LQUnitStateApproach.. 3 %p %s", soldier, soldier->getName().c_str());
    }
    
    //存在目标，将不能改变目标
    if(soldier->isHasTarget())
        return m_activationLevel = -1;
    
    m_lockedunit = parent->getLockedUnit();  //索敌对象是在索定军团中找的
    //if(m_lockedunit)
    //    CCLOG("LQUnitStateApproach.. %p", m_lockedunit);
    
    if (soldier->getSquadState()==FUSM_SQSTATE_APPROACH || soldier->getSquadState()==FUSM_SQSTATE_ATTACK)      //集团索敌
        m_activationLevel = 1;
    else if (soldier->getSquadState()!=FUSM_SQSTATE_ATTACK)   //集团进攻
        m_activationLevel = -1;
    else
        m_activationLevel = 0.0f;
    return m_activationLevel;
}*/

void LQUnitStateApproach::exit()
{
    FSMState::exit();
	//LQUnitAIControl* parent = (LQUnitAIControl*)m_parent;
	//LQCombatUnit* soldier = getCombatUnit();    
};
