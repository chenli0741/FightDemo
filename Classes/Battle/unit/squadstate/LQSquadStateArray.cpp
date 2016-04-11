#include "LQSquadStateArray.h"

#include "LQSquadAIControl.h"

void LQSquadStateArray::enter()
{
	//LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
	//LQCombatSquad* squad = getCombatSquad();
	//soldier->attack();
};

//---------------------------------------------------------
void LQSquadStateArray::update(float dt, Layer* layer)
{
    //turn towards closest asteroid's future position, and then fire
    //LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
    LQCombatSquad* squad = getCombatSquad();
    LQCombatSquad* target = (LQCombatSquad*)squad->getTarget();
    
    if(m_activationLevel>0){
        Point _targettd = target->getBannerman()->getTargetAttackTileCoord(squad->getBannerman());
        Point _targetpt = convertPoint(_targettd);
        Point _squadpt = squad->getBannerman()->getPosition();
        Point _pt = _targetpt-_squadpt;
        squad->trunFormation( _squadpt+ Point(_pt.x/2,_pt.y/2) ); //target->getPosition()
    }
}

//计算状态激活水平值 >=0 激活状态 <0 状态消除
float LQSquadStateArray::calculateActivation()
{
    //LQSquadAIControl* parent = (LQSquadAIControl*)m_parent;
    LQCombatSquad* squad = getCombatSquad();
    
    if(LQBattleSession::getInstance()->getStatus()==klqBattleStatusEnd)
        return m_activationLevel = -1;
    
    //军团没有目标，目标已经死了
    if(!squad->isHasTarget())
        return m_activationLevel = -1;
    
    m_attack_dist = squad->getCurrAtkView()*MAP_LENGTH;  //攻击范围 SQ_ATTACK_DIST
    m_lockedSquad = (LQCombatSquad*)squad->getTarget();
    Point pt1 = m_lockedSquad->getLQPosition();
    Point pt2 = squad->getLQPosition();
    float m_lockedSquadDist = pt1.getDistance(pt2);
    
    //存在攻击目标，且目标在攻击范围之内
    m_activationLevel = m_lockedSquadDist <= m_attack_dist;
    
    return m_activationLevel;
}
