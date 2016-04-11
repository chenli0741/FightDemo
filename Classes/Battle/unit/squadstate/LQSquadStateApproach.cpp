#include "LQSquadStateApproach.h"

#include "LQSquadAIControl.h"

void LQSquadStateApproach::enter()
{
	//LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
	//LQCombatSquad* squad = getCombatSquad();
	//soldier->attack();
};

//---------------------------------------------------------
void LQSquadStateApproach::update(float dt, Layer* layer)
{
    //turn towards closest asteroid's future position, and then fire
    //LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
    LQCombatSquad* squad = getCombatSquad();

    if(m_activationLevel==1)
        squad->updateApprochCommand(m_lockedSquad);
    else if(m_activationLevel==2 && squad->getTarget()!=m_lockedSquad)
        //攻击范围内而且攻击目标发生变化了，原来的死了，还没来得及更新
        squad->updateAttackCommand(m_lockedSquad);
    else if(squad->getTarget()==NULL || (squad->getTarget() && !squad->getTarget()->getActive()))
        squad->setTarget(m_lockedSquad);
}

//计算状态激活水平值 >=0 激活状态 <0 状态消除
float LQSquadStateApproach::calculateActivation()
{
    LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
    LQCombatSquad* squad = getCombatSquad();
    
    if(LQBattleSession::getInstance()->getStatus()==klqBattleStatusEnd)
        return m_activationLevel = -1;
    
//    if(squad->getIndex()==12 && squad->getSquadDirection()==klqCombatSquadDirectionRight)
//    {
//        CCLOG("LQSquadStateApproach::calculateActivation %s", squad->getName().c_str());
//    }
    
    //军团存在攻击目标，将不能改变目标
    if(squad->isHasTarget() && squad->getState()!=FUSM_SQSTATE_WAIT)
        return m_activationLevel = -1;
    
    m_approach_dist = MAP_ROW*MAP_LENGTH;      //最大的索敌范围 MAP_ROW
    m_attack_dist = squad->getCurrAtkView()*MAP_LENGTH;  //攻击范围 SQ_ATTACK_DIST*
    
    m_lockedSquad = NULL;
    if (!squad->getIsOpenView()){  //还没有打开视野
        float m_nearestSquadDist = parent->getNearestSquadDist();
        if(m_nearestSquadDist>=0 && m_nearestSquadDist<m_attack_dist+MAP_LENGTH*0.5)  //已经在攻击视野内
        {
            if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("SIDX[%s]:撞见了 [%s] 及时打开视野。。。%f", squad->getName().c_str(), parent->getNearestSquad()->getName().c_str(), m_nearestSquadDist);
            squad->setIsOpenView(true);
        }
    }
    
    m_activationLevel = -1;
    //未开战（冲锋的军团碰上开战）或者 没有开视野
    if (parent->getStatus()<klqBattleStatusRun || !squad->getIsOpenView())
        m_activationLevel = -1;
    else{
        if(squad->isOnMustWays())
            return -1;
        
        m_lockedSquad = parent->getLockedSquad();
        float m_lockedSquadDist = parent->getLockedSquadDist();
        if(!m_lockedSquad)           //没有索到目标
            m_activationLevel = 0.0f;
        else if(m_lockedSquad){
            if(m_lockedSquadDist<m_attack_dist)  //已经在攻击视野内
                m_activationLevel = 2;
            else if (m_lockedSquadDist < m_approach_dist) //在索敌视野内
                m_activationLevel =  1;
            else
                CCLOG("SINX[%s]:目标 %s 跑到哪里去了，%f", squad->getName().c_str(), m_lockedSquad->getName().c_str(), m_lockedSquadDist);
        }
        
        //if(m_activationLevel>0){
        //    if (squad->getIndex()==3)
        //        CCLOG("squad........");
        //    CCLOG("calculateActivation %s->%s   %s->%s", squad->getName().c_str(), m_lockedSquad->getName().c_str(), PointString(squad->getPosition()).c_str(), PointString(m_lockedSquad->getPosition()).c_str());
        //    CCLOG("  Dist %.3f,%.3f ", m_lockedSquadDist, m_attack_dist);
        //}
    }
    return m_activationLevel;
}
