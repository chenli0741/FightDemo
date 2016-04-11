#ifndef __FSMMACHINE
#define __FSMMACHINE

#include "FSMState.h"
#include "FMachine.h"
#pragma warning(disable: 4786)
#include <vector>


class FSMMachine: public FSMState, public FMachine
{
public:
    //constructor/functions
    FSMMachine(int type = FUSM_MACH_NONE,AIController* parent = NULL);
    virtual void updateMachine(float dt, Layer* layer);
    //virtual void addState(FSMState* state);
    virtual void setDefaultState(FSMState* state) {m_defaultState = state;}
    //virtual void setGoalID(int goal) {m_goalID = goal;}
    virtual bool transitionState(int goal);
    virtual void reset();
    
    //data
    FSMState* getStates(int i);
    FSMState* getCurrentState(){ return m_currentState; };
protected:
    //std::vector<FSMState*> m_states;
    FSMState* m_currentState;
    FSMState* m_defaultState;
    FSMState* m_goalState;
    //int		  m_goalID;
};
#endif