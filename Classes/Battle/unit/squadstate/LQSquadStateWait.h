#ifndef __SuperWarriors__LQSquadStateWait__
#define __SuperWarriors__LQSquadStateWait__

#include "../LQCombatSquad.h"

//军团单元巡逻状态
//  巡逻状态，加速冲向目标
class LQSquadStateWait : public FuSMState
{
public:
    //constructor/functions
    LQSquadStateWait(AIController* parent):FuSMState(FUSM_SQSTATE_WAIT,parent)
    {
        m_lockedSquad = NULL;
    }
	void enter();
    void update(float dt, Layer* layer);
    float calculateActivation(); //计算状态激活水平值 >=0 激活状态 <0 状态消除
    
	LQCombatSquad* getCombatSquad(){ return (LQCombatSquad*)(m_parent->getControlObj()); }
private:
    LQCombatSquad* m_lockedSquad;
    float m_approach_dist;              //索敌范围
    float m_attack_dist;                //攻击范围
};
#endif