#ifndef __Lequworld_SuperWarriors_FUSMMACHINE__
#define __Lequworld_SuperWarriors_FUSMMACHINE__

#include "FuSMState.h"
#include "FMachine.h"
#include "cocos2d.h"

USING_NS_CC;

//模糊状态机
class FuSMMachine: public FuSMState, public FMachine
{
public:
    //constructor/functions
    FuSMMachine(int type = FUSM_MACH_NONE, AIController* parent = NULL);
    virtual void updateMachine(float dt, Layer* layer);		//运行模糊状态机
    //virtual void addState(FuSMState* state);	//增加状态
	virtual bool isActive(FuSMState *state);	//判断状态是否激活中
    virtual void reset();						//重置状态
    
    //
    FuSMState* getStates(int i);
protected:
    //std::vector<FuSMState*> m_states;
    std::vector<FuSMState*> m_activatedStates;
};
#endif