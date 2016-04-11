//
// AIObj.h
// SuperWarriors
//
// Created by chenli on 14-6-16.
// Copyright lequworld.com
// AI控制器
//

#ifndef __Lequworld_SuperWarriors_ControlObj_H_
#define __Lequworld_SuperWarriors_ControlObj_H_

#include "GameObj.h"

class AIController;

//AI控制器
class AIObj : public GameObj
{
public:
	//constructor/functions
	AIObj(AIController* controller):GameObj(){ m_controller = controller; };

	//AI控制器
	CC_SYNTHESIZE(AIController*, m_controller, Controller)
};

#endif