#include "LQSquadStateAttack.h"

#include "LQSquadAIControl.h"

void LQSquadStateAttack::enter()
{
	//LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
	//LQCombatSquad* squad = getCombatSquad();
	//soldier->attack();
};

//---------------------------------------------------------
void LQSquadStateAttack::update(float dt, Layer* layer)
{
    //turn towards closest asteroid's future position, and then fire
    //LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
    LQCombatSquad* squad = getCombatSquad();
    
    if(m_activationLevel>0)
        squad->updateAttackCommand();
    else if(m_activationLevel==0)
        squad->_updateMoveCommand();
}

//计算状态激活水平值 >=0 激活状态 <0 状态消除
float LQSquadStateAttack::calculateActivation()
{
    LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
    LQCombatSquad* squad = getCombatSquad();
    
    if(LQBattleSession::getInstance()->getStatus()==klqBattleStatusEnd)
        return m_activationLevel = -1;
    
    if(squad->getIndex()==8 && squad->getSquadDirection()==klqCombatSquadDirectionLeft)
    {
        //CCLOG("LQSquadStateAttack::calculateActivation %s", squad->getName().c_str());
    }
    
    //军团没有目标，目标已经死了
    if(!squad->isHasTarget())
        return m_activationLevel = -1;
    
    m_attack_dist = squad->getCurrAtkView()*MAP_LENGTH;  //攻击范围 SQ_ATTACK_DIST
    m_lockedSquad = (LQCombatSquad*)squad->getTarget();
    Point pt1 = m_lockedSquad->getLQPosition();
    Point pt2 = squad->getLQPosition();
    float m_lockedSquadDist = getDistance(pt1,pt2);
    float m_nearestSquadDist = parent->getNearestSquadDist();  //最近的目标
    
    //if (squad->getIndex()==8 && parent->getNearestSquad())
    //    CCLOG("1 squad.......%s Dist %.3f,%.3f ", parent->getNearestSquad()->getName().c_str(), m_nearestSquadDist, m_attack_dist);
    
    //最近的目标出现在攻击范围内,变更攻击目标
    if (m_nearestSquadDist <= m_attack_dist && parent->getNearestSquad()!=m_lockedSquad )
    {
        //CCLOG("2 squad.......Dist %.3f,%.3f  %s %s", m_nearestSquadDist, m_attack_dist, PointString(squad->getTileCoord()).c_str(), PointString(parent->getNearestSquad()->getTileCoord()).c_str());
        if (!squad->isOnWays() && squad->getTaunter()==NULL){ //增加嘲讽者
            m_activationLevel = 1;
            squad->setTarget(parent->getNearestSquad());
            return m_activationLevel;
        }
    }
    
    //if(squad->getIndex()==13 || squad->getHeroData()->getResIDC()==90041926)
        //CCLOG(" calculateActivation= %.3f %.3f", m_attack_dist, m_lockedSquadDist);
    
    
    if (parent->getStatus()<klqBattleStatusRun)
        m_activationLevel = -1;
    else{
        //存在攻击目标，且目标在攻击范围之内
        m_activationLevel = m_lockedSquadDist <= m_attack_dist;
        if(m_activationLevel>0){
            //if (squad->getIndex()==3)
            //    CCLOG("3 squad ........");
            //CCLOG("%s->%s   %s->%s", squad->getName().c_str(), m_lockedSquad->getName().c_str(), PointString(squad->getPosition()).c_str(), PointString(m_lockedSquad->getPosition()).c_str());
            //CCLOG("  Dist %.3f,%.3f ", m_lockedSquadDist, m_attack_dist);
        }
    }
    return m_activationLevel;
}
