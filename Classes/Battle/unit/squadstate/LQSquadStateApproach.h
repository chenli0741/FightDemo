#ifndef __SuperWarriors__LQSquadStateApproach__
#define __SuperWarriors__LQSquadStateApproach__

#include "../LQCombatSquad.h"


//军团单元索敌状态
//  索敌状态，加速冲向目标
class LQSquadStateApproach : public FuSMState
{
public:
    //constructor/functions
    LQSquadStateApproach(AIController* parent):FuSMState(FUSM_SQSTATE_APPROACH,parent)
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