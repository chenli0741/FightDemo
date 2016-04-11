#include "FSMMachine.h"

#include "AIController.h"
//---------------------------------------------------------
FSMMachine::FSMMachine(int type,AIController* parent):
FSMState(type,parent), FMachine()
{
	m_currentState = NULL;
	m_defaultState = NULL;
	m_goalState    = NULL;
}

//---------------------------------------------------------
void FSMMachine::updateMachine(float dt, Layer* layer)
{
	//don't do anything if you have no states
	if(m_states.size() == 0 )
		return;

	//don't do anything if there's no current 
	//state, and no default state
	if(!m_currentState)
		m_currentState = m_defaultState;
	if(!m_currentState)
		return;

    if(m_currentState->getAIParent()->getControlObj()->getIndex()==0)
    {
        
    }
        
	//check for transitions, and then update
	int oldStateID = m_currentState->getType();
	int m_goalID = m_currentState->checkTransitions();
	
	//switch if there was a transition
	if(m_goalID != oldStateID)
	{
		if(transitionState(m_goalID))
		{
			m_currentState->exit();
			m_currentState = m_goalState;
			m_currentState->enter();
		}
	}
	m_currentState->update(dt,layer);
	
}

//---------------------------------------------------------
//void FSMMachine::addState(FSMState* state)
//{
//	m_states.push_back(state);
//}

FSMState* FSMMachine::getStates(int i)
{
    return (FSMState*)m_states[i];
}

//---------------------------------------------------------
bool FSMMachine::transitionState(int goal)
{
	//don't do anything if you have no states
	if(m_states.size() == 0 )
		return false;
	
	//determine if we have state of type 'goal'
	//in the list, and switch to it, otherwise, quit out
	for(int i =0;i<m_states.size();i++)
	{
		if(m_states[i]->getType() == goal && m_currentState->hasInterval())
		{
			m_goalState = getStates(i);
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------
void FSMMachine::reset()
{
	exit();
	if(m_currentState)
		m_currentState->exit();
	m_currentState = m_defaultState;

	//init all the states
	for(int i =0;i<m_states.size();i++)
		m_states[i]->init();

    //and now enter the m_defaultState, if any
    if(m_currentState)
        m_currentState->enter();
    
}