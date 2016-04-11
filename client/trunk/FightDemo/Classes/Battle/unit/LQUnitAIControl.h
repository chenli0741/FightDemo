#ifndef __SuperWarriors__LQUnitAIControl__
#define __SuperWarriors__LQUnitAIControl__

#include "../fusm/AIController.h"
#include "../fusm/GameObj.h"
#include "../fusm/AIObj.h"
#include "../fusm/FSMMachine.h"

USING_NS_CC;
using namespace std;

#include "LQCombatUnit.h"
#include "LQCombatSquad.h"


//#define FU_APPROACH_DIST     10.0f     //索敌范围
//#define FU_ATTACK_DIST       1.2f     //攻击范围

class LQUnitAIControl: public AIController
{
public:
    //constructor/functions
    LQUnitAIControl(AIObj* obj);
    virtual void update(float dt, Layer* layer);
    virtual void updatePerceptions(float dt);
    virtual bool init();
    
    //perception data 
    // 锁定的军团
    CC_SYNTHESIZE_READONLY(LQCombatUnit*, m_lockedUnit, LockedUnit);
    // 锁定的军团距离
    CC_SYNTHESIZE_READONLY(float, m_lockedUnitDist, LockedUnitDist);
    // 锁定的军团距离
    CC_SYNTHESIZE_READONLY(float, m_targetDist, TargetDist);
    
    // 最近的军团
    //CC_SYNTHESIZE_READONLY(GameObj*, m_nearestUnit, NearestUnit);
    // 最近的军团距离
    //CC_SYNTHESIZE_READONLY(float, m_nearestUnitDist, NearestUnitDist);
    // 将碰撞
    //CC_SYNTHESIZE_READONLY(bool, m_willCollide, WillCollide);
    // 安全距离
    //CC_SYNTHESIZE_READONLY(float, m_safetyRadius, SafetyRadius);
    
    void setSquad(LQCombatSquad* squad){ m_squad = squad; };
private:
    LQCombatUnit*  m_unit;          //当前战斗单元
    LQCombatSquad* m_squad;         //当前军团
};
#endif