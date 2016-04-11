#ifndef __SuperWarriors__LQSquadAIControl__
#define __SuperWarriors__LQSquadAIControl__

#include "../fusm/AIController.h"
#include "../fusm/GameObj.h"
#include "../fusm/AIObj.h"
#include "../fusm/FuSMMachine.h"

#include "../LQBattleSession.h"

#include "LQCombatSquad.h"

USING_NS_CC;
using namespace std;


//军团的AI控制器
class LQSquadAIControl: public AIController
{
public:
    //constructor/functions
    LQSquadAIControl(AIObj* obj);
    virtual void update(float dt, Layer* layer);
    virtual void updatePerceptions(float dt);
    virtual bool init();
    
    // 战斗状态 0 准备 1 战斗 2 结束
    LQBattleStatusEnum getStatus();
    
    // 锁定的军团
    CC_SYNTHESIZE_READONLY(LQCombatSquad*, m_lockedSquad, LockedSquad);
    // 锁定的军团距离
    CC_SYNTHESIZE_READONLY(float, m_lockedSquadDist, LockedSquadDist);
    
    // 最近的军团
    CC_SYNTHESIZE_READONLY(GameObj*, m_nearestSquad, NearestSquad);
    // 最近的军团距离
    CC_SYNTHESIZE_READONLY(float, m_nearestSquadDist, NearestSquadDist);
    //float m_nearestSquadDist1;
private:
    LQCombatSquad* m_squad;
};
#endif