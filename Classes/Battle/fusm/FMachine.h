#ifndef __Lequworld_SuperWarriors_FMachine__
#define __Lequworld_SuperWarriors_FMachine__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

//状态机
class FMachine
{
public:
    FMachine(){};
    virtual ~FMachine(){
        while(!m_states.empty()) {
            delete m_states.back();
            m_states.pop_back();
        }
    };
    virtual void updateMachine(float dt, Layer* layer){};		//运行模糊状态机
    virtual void addState(FState* state){
        m_states.push_back(state);
    };	//增加状态
	virtual void reset(){};						//重置状态
protected:
    std::vector<FState*> m_states;
};
#endif