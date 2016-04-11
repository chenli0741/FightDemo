#ifndef __SuperWarriors__LQUnitStateWait__
#define __SuperWarriors__LQUnitStateWait__

#include "../LQCombatUnit.h"

//战斗单元集结巡逻状态
class LQUnitStateWait : public FSMState
{
public:
    //constructor/functions
    LQUnitStateWait(AIController* parent):FSMState(FUSM_STATE_WAIT,parent){}
	void enter();       //进入状态时运行
    void exit();        //离开状态时运行
    void update(float dt, Layer* layer);  //如果该状态是当前状态，在AI每次处理中都会调用
    int  checkTransitions(); //用于处理决定状态结束的逻辑，将返回运行状态的枚举，如果不需改变，返回原状态
    
	LQCombatUnit* getCombatUnit(){ return (LQCombatUnit*)(m_parent->getControlObj()); }
private:
    float m_attack_dist;                //攻击范围
    float m_attack_dist_min;
};
#endif