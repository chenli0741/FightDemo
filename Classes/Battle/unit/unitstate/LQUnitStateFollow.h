#ifndef __SuperWarriors__LQUnitStateFollow__
#define __SuperWarriors__LQUnitStateFollow__

#include "../LQCombatUnit.h"

//战斗单元跟随状态
class LQUnitStateFollow : public FSMState
{
public:
    //constructor/functions
    LQUnitStateFollow(AIController* parent):FSMState(FUSM_STATE_FOLLOW,parent){}
	void enter();       //进入状态时运行
    void exit();        //离开状态时运行
    void update(float dt, Layer* layer);  //如果该状态是当前状态，在AI每次处理中都会调用
    int  checkTransitions(); //用于处理决定状态结束的逻辑，将返回运行状态的枚举，如果不需改变，返回原状态
    
	LQCombatUnit* getCombatUnit(){ return (LQCombatUnit*)(m_parent->getControlObj()); }
private:
    float m_attack_dist;                //攻击范围
};
#endif