#include "LQSquadAIControl.h"
#include "../LQBattleSession.h"

//#include "FStateApproach.h"
#include "squadstate/LQSquadStateAttack.h"
#include "squadstate/LQSquadStateApproach.h"
#include "squadstate/LQSquadStateWait.h"
//#include "./squadstate/LQSquadStatePatrol.h"
#include "./squadstate/LQSquadStateArray.h"
//#include "FStateEvade.h"

//#include "Target.h"

/*
FUSM_SQSTATE_WAIT,      //等
FUSM_SQSTATE_ATTACK,    //攻击
FUSM_SQSTATE_APPROACH,  //索敌，追踪敌人
FUSM_SQSTATE_EVADE,     //逃跑或者后撤
FUSM_SQSTATE_PAREOL     //巡逻，漫无目的Patrol
 */

//---------------------------------------------------------
LQSquadAIControl::LQSquadAIControl(AIObj* obj):
AIController(obj), m_nearestSquad(NULL),  m_nearestSquadDist(0)
{
    //construct the state machine and add the necessary states
    m_squad = (LQCombatSquad*)m_ctrlObj;
    
	//基类已经创建，这里只是加状态
    m_machine = new FuSMMachine(FUSM_MACH_SAUCER,this);
	//m_machine->setType(FUSM_MACH_SAUCER);
    //m_machine->addState(new LQSquadStatePatrol(this));
    m_machine->addState(new LQSquadStateApproach(this));
    m_machine->addState(new LQSquadStateWait(this));
    m_machine->addState(new LQSquadStateAttack(this));
    m_machine->addState(new LQSquadStateArray(this));  //暂时不用
    m_machine->reset();
}

//---------------------------------------------------------
bool LQSquadAIControl::init()
{
	AIController::init();
    
    m_nearestSquad = NULL;
    m_nearestSquadDist = -1.0f;
    m_lockedSquad = NULL;
    m_lockedSquadDist = -1.0f;
	
	return true;
}

//---------------------------------------------------------
void LQSquadAIControl::update(float dt, Layer* layer)
{
    //当军团有目标时军团不能变更AI
    if(m_squad && !m_squad->isOnChangeAI())
        return;
	AIController::update(dt,layer);
}

//--感知-------------------------------------------------------
void LQSquadAIControl::updatePerceptions(float dt)
{
    m_nearestSquad = NULL;
    
    if(!m_ctrlObj->getIsOpenView())
        m_lockedSquad = NULL;
    // 敌人军团完蛋了
    if(m_lockedSquad){
        if(!m_lockedSquad->getActive()){
            if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL) CCLOG("SIDX:%d 敌人军团%d完蛋了", m_squad->getIndex(), m_lockedSquad->getIndex() );
            //存储最近的目标
            if( m_squad->getArmsType()==Combat_ArmsType_Cavalry )  //骑兵阵型有弓兵优先的嗜好  && m_squad->getAtkCount()<1 
                m_lockedSquad = LQBattleSession::getInstance()->getClosestCombatSquad(m_squad, Combat_ArmsType_Remote);
            else m_lockedSquad = NULL;

            m_nearestSquad  = LQBattleSession::getInstance()->getClosestCombatSquad(m_squad);
            if(m_lockedSquad == NULL)
                m_lockedSquad  = (LQCombatSquad*)m_nearestSquad;
        }
    }else {
        //存储锁定的目标
        if( m_squad->getArmsType()==Combat_ArmsType_Cavalry )  //骑兵阵型第一次攻击钱索敌有弓兵优先的嗜好 && m_squad->getAtkCount()<1
            m_lockedSquad = LQBattleSession::getInstance()->getClosestCombatSquad(m_squad, Combat_ArmsType_Remote);
        else m_lockedSquad = NULL;
        
        //最近锁定
        m_nearestSquad  = LQBattleSession::getInstance()->getClosestCombatSquad(m_squad);
        
        if(m_lockedSquad == NULL)
            m_lockedSquad  = (LQCombatSquad*)m_nearestSquad;
        
        if(m_lockedSquad && !m_lockedSquad->getActive()) m_lockedSquad = NULL;
        if(m_nearestSquad && !m_nearestSquad->getActive()) m_nearestSquad = NULL;
    }
    
    //asteroid collision determination
    if(m_lockedSquad)
    {
        Point pt1 = m_lockedSquad->getLQPosition();
        Point pt2 = m_ctrlObj->getLQPosition();
        m_lockedSquadDist = getDistance(pt1,pt2);//pt1.getDistance(pt2);
    }
    
    //存储最近的目标
    if(!m_nearestSquad)
        m_nearestSquad  = LQBattleSession::getInstance()->getClosestCombatSquad(m_squad);
    if(m_nearestSquad)
    {
		//m_nearestSquadDist1 = m_nearestSquad->getLQPosition().getDistance(m_ctrlObj->getLQPosition());
        m_nearestSquadDist = getDistance(m_nearestSquad->getLQPosition(),m_ctrlObj->getLQPosition());
    }
    
}

LQBattleStatusEnum LQSquadAIControl::getStatus()
{
    return LQBattleSession::getInstance()->getStatus();
}


