#ifndef __Lequworld_SuperWarriors_FState__
#define __Lequworld_SuperWarriors_FState__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class AIController;

//状态
class FState : public Ref
{
public:
	//constructor/functions
	FState(int type = 0,AIController* parent = NULL){m_type = type;m_parent = parent; m_interval=-1; m_ctrlinterval = 0; }
    virtual void enter()   { m_interval = 0; }      //进入状态时运行
	virtual void exit()    { }      //离开状态时运行
	virtual void update(float dt, Layer* layer)   { m_interval += dt; }      //如果该状态是当前状态，在AI每次处理中都会调用
	virtual bool init()    { return true; }      //对状态的复位
	
    //AI控制的对象数据
	CC_SYNTHESIZE_READONLY(AIController*, m_parent, AIParent)
	//状态类型
	CC_SYNTHESIZE(int, m_type, Type)
    //AI状态控制间隔
	virtual bool hasInterval(){
        if (m_ctrlinterval>0 && m_interval>=0){
            return m_interval>m_ctrlinterval;
        }
        return true;
    };     //间隔合适为true
protected:
    float m_ctrlinterval;
    float m_interval;
};


#endif