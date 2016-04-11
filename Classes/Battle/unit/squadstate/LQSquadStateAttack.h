#ifndef __SuperWarriors__LQSquadStateAttack__
#define __SuperWarriors__LQSquadStateAttack__


#include "../LQCombatSquad.h"

//军团单元攻击状态
//  攻击状态，发布进攻命令
class LQSquadStateAttack : public FuSMState
{
public:
    //constructor/functions
    LQSquadStateAttack(AIController* parent):FuSMState(FUSM_STATE_ATTACK,parent){}
	void enter();
    void update(float dt, Layer* layer);
    float calculateActivation();        //计算状态激活水平值 >=0 激活状态 <0 状态消除
    
	LQCombatSquad* getCombatSquad(){ return (LQCombatSquad*)(m_parent->getControlObj()); }
private:
    LQCombatSquad* m_lockedSquad;
    float m_attack_dist;                //攻击范围
};
#endif