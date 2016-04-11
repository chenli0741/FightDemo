#ifndef __Lequworld_SuperWarriors_FSMEnum__
#define __Lequworld_SuperWarriors_FSMEnum__

#include "cocos2d.h"
#include "../unit/LQCombatUnitEnums.h"
#include "../core/LQCore.h"

USING_NS_CC;
using namespace std;

//FSMEnum state types
//状态机的状态种类
typedef enum
{
    FUSM_STATE_NONE = -1,
	FUSM_STATE_WAIT = 0,        //等待
	FUSM_STATE_ATTACK = 1,      //攻击
	FUSM_STATE_DIE = 2,         //死亡
    FUSM_STATE_APPROACH = 3,    //索敌，追踪敌人
	FUSM_STATE_EVADE = 4,       //逃跑或者后撤
	FUSM_STATE_PATROL = 5,      //巡逻，漫无目的Patrol
    FUSM_STATE_ARRAY = 6,       //列队
    FUSM_STATE_FOLLOW = 7,      //跟随武将，RUN
	FUSM_STATE_COUNT = 8        //统计
} LQUnitStateEnum;

#define USE_STATE_EVADE 0        //使用逃跑策略 1

//军团状态机的状态种类
typedef enum
{
    FUSM_SQSTATE_NONE = -1,
	FUSM_SQSTATE_WAIT = 0,      //军团等待，单位都原地不动，（可能调整队型站位）
	FUSM_SQSTATE_RUN = 1,       //阵型行军，（单元跟随武将），（目标点移动和路径移动）
    FUSM_SQSTATE_APPROACH = 2,  //军团索敌，军团追踪敌人, 开视野
	FUSM_SQSTATE_ATTACK = 3     //军团攻击命令，单位可以攻击了
	//FUSM_SQSTATE_EVADE,     //军团逃跑或者后撤（现在没有处理）
    //FUSM_SQSTATE_ARRAY,     //列队，军团队伍集结（单元靠近英雄，并整理成队形）
	//FUSM_SQSTATE_PATROL     //巡逻，军团自由行走（单元在英雄范围内活动）
} LQSquadStateEnum;

//FuSM machine types
//状态机的类型
enum
{
	FUSM_MACH_NONE,         //无
	FUSM_MACH_SAUCER,       //AI控制
    FUSM_MACH_COUNT
};


#endif