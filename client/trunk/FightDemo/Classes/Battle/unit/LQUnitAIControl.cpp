#include "LQUnitAIControl.h"
#include "LQBattleSession.h"

//#include "LQCombatUnit.h"
//#include "LQCombatSquad.h"
#include "unitstate/LQUnitStateAttack.h"
#include "unitstate/LQUnitStateApproach.h"
#include "unitstate/LQUnitStateFollow.h"
#include "unitstate/LQUnitStateEvade.h"
#include "unitstate/LQUnitStateDeath.h"
//#include "./unitstate/LQUnitStatePatrol.h"
#include "unitstate/LQUnitStateWait.h"

//---------------------------------------------------------
LQUnitAIControl::LQUnitAIControl(AIObj* obj):
AIController(obj), m_lockedUnit(NULL), m_lockedUnitDist(0)
//m_nearestUnit(NULL),m_safetyRadius(0),m_willCollide(false), m_nearestUnitDist(0),
{
    //construct the state machine and add the necessary states
    m_unit = (LQCombatUnit*)m_ctrlObj;
    m_squad = m_unit->getSquad();
    
	//基类已经创建，这里只是加状态
    m_machine = new FSMMachine(FUSM_MACH_SAUCER,this);
    //m_machine->setType(FUSM_MACH_SAUCER);
    m_machine->addState(new LQUnitStateApproach(this));
    FSMState *state = new LQUnitStateFollow(this);
    m_machine->addState(state);
    m_machine->addState(new LQUnitStateEvade(this));
    m_machine->addState(new LQUnitStateAttack(this));
    m_machine->addState(new LQUnitStateDeath(this));
    //m_machine->addState(new LQUnitStatePatrol(this));
    m_machine->addState(new LQUnitStateWait(this));
    ((FSMMachine*)m_machine)->setDefaultState(state);
	m_machine->reset();
}

//---------------------------------------------------------
bool LQUnitAIControl::init()
{
	AIController::init();
    
    //m_willCollide  = false;
    //m_safetyRadius    = 15.0f;
    
	/*
     if(!m_target)
     {
     m_target = new Target;
     m_target->m_size = 1;
     Game.PostGameObj(m_target);
     }*/
	return true;
}

//---------------------------------------------------------
void LQUnitAIControl::update(float dt, Layer* layer)
{
	AIController::update(dt,layer);
}

//--感知-------------------------------------------------------
void LQUnitAIControl::updatePerceptions(float dt)
{
    if(m_ctrlObj->getType()==klqLQGameObjHero){
        //if(!m_ctrlObj->getTarget() && m_ctrlObj->getIndex()==96)
        //    CCLOG("LQUnitAIControl.. %d %d", m_ctrlObj->getIndex(), m_ctrlObj->getTargetIndex());
        //CCLOG("LQUnitAIControl.. %p %s", m_ctrlObj, m_ctrlObj->getName().c_str());
    }
    
    //if(m_ctrlObj->getIndex()==11)
    //    CCLOG("updatePerceptions.. %p %d", m_ctrlObj, m_ctrlObj->getIndex());
    
    m_ctrlObj->isDied();
       
    if(m_ctrlObj->getTarget() && !m_ctrlObj->getTarget()->getActive())  //目标死了
    {
        m_ctrlObj->setTarget(NULL);
    }
    
    //存储锁定的目标
    if(!m_ctrlObj->getTarget()){
        m_lockedUnit = LQBattleSession::getInstance()->getLockedUnit(m_ctrlObj, m_squad);
    }
    
    if(m_lockedUnit)
    {
        Point pt1 = m_lockedUnit->getLQPosition();
        Point pt2 = m_ctrlObj->getLQPosition();
        m_lockedUnitDist = getDistance(pt1,pt2);
    }else{
        //没有锁定到敌人,军团死光了
    }
    
    if(m_ctrlObj->getTarget())
    {
        
        Point pt1 = m_ctrlObj->getTarget()->getLQPosition();
        Point pt2 = m_ctrlObj->getLQPosition();
        m_targetDist = getDistance(pt1,pt2);
    }
    
}


