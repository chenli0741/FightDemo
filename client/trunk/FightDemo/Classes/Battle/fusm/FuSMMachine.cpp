//
// FuSMMachine.cpp
// SuperWarriors
//   AI的控制类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#include "FuSMMachine.h"


//---------------------------------------------------------
FuSMMachine::FuSMMachine(int type,AIController* parent):
FuSMState(type,parent), FMachine()
{
	
}

FuSMState* FuSMMachine::getStates(int i)
{
    return (FuSMState*)m_states[i];
}

//---------------------------------------------------------
void FuSMMachine::updateMachine(float dt, Layer* layer)
{
    //don't do anything if you have no states
    if(m_states.size() == 0)
        return;
    
    //check for activations, and then update
    m_activatedStates.clear();
    std::vector<FuSMState*> nonActiveStates;
    for(int i =0;i<m_states.size();i++)
    {
        if(getStates(i)->calculateActivation() >= 0){
            m_activatedStates.push_back(getStates(i));
		}else
            nonActiveStates.push_back(getStates(i));
    }
    
    //Exit all non active states for cleanup
    if(nonActiveStates.size() != 0)
    {
        for(int i =0;i<nonActiveStates.size();i++)
            nonActiveStates[i]->exit();
    }
    
    //Update all activated states
    if(m_activatedStates.size() != 0)
    {
        for(int i =0;i<m_activatedStates.size();i++)
            m_activatedStates[i]->update(dt,layer);
    }
}

//---------------------------------------------------------
//void FuSMMachine::addState(FuSMState* state)
//{
//	m_states.push_back(state);
//}

//---------------------------------------------------------
bool FuSMMachine::isActive(FuSMState *state)
{
    if(m_activatedStates.size() != 0)
    {
        for(int i =0;i<m_activatedStates.size();i++)
            if(m_activatedStates[i] == state)
				return true;
    }
	return false;
}

//---------------------------------------------------------
void FuSMMachine::reset()
{
	//exit and init all the states
	for(int i =0;i<m_states.size();i++)
    {
        m_states[i]->exit();
        m_states[i]->init();
    }
}