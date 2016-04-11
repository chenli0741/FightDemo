//
//  LQCombatUnitEnums.h
//  SuperWarriors
//
//  Created by ByronSong on 13-4-19.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#ifndef SuperWarriors_LQCombatUnitEnums_h
#define SuperWarriors_LQCombatUnitEnums_h

#include "LQCore.h"

LQ_NAMESPACE

#define klqAttackStartValue 10
#define klqAttackFinishValue 20
#define klqEndingStartValue 30
#define klqEndingFinishValue 40
#define AnimationFPS 0.09   //动作的帧间隔


// BUFF
const int TAG_BUFFER_ID = 45555;


typedef enum
{
    Missile_Fly_ZX = 0,   //直线
    Missile_Fly_GJ = 1,   // 弓箭
    Missile_Fly_TS = 2,   // 投石
    Missile_Fly_BQ = 3,   // 标枪
    Missile_Fly_FS = 4,   // 法术
    Missile_Fly_1FX = 7,  // 单方向直线
    Missile_Fly_8FX = 8   // 八方向直线
}LQMissileFlyType;

typedef enum
{
    klqSkillShowType_None = 0,      //无回调
    //
    //klqSkillShowType_ActiveSkill,   //主动技能
    //klqSkillShowType_Passive,       //被动技能
    //buff效果
    klqSkillShowType_Buff_1,    //1.静态图片
    klqSkillShowType_Buff_2,    //2.序列帧，有方向
    klqSkillShowType_Buff_3,    //3.人物叠加颜色，闪烁
    klqSkillShowType_Buff_4,    //4.序列帧，不分方向
    klqSkillShowType_Buff_5,    //5.图片，360°旋转
    //
    KLQSkillShowType_Stunte,        //武将增强效果播放，只传radius= 0；count = 1
    KLQSkillSHowType_Smoke
   
} KLQSkillShowType;

typedef enum{
    klqAnimationDisplayTypeNone = 0,
    klqAnimationDisplayTypeAnimation = 1,       //动画
    klqAnimationDisplayTypeIcon = 2,            //图标
    klqAnimationDisplayTypeBmp = 3,             //图片
    klqAnimationDisplayTypeEffect = 4           //特效
} KLQAnimationDisplayType;

//动作动画类型
enum LQUnitAnimPhaseEnum {
    klqUnitAnimPhaseNone = 0,
    klqUnitAnimPhaseStand = 1,
    klqUnitAnimPhaseMove = 2,
    klqUnitAnimPhaseFallDown = 3,
    klqUnitAnimPhaseAttack = 4,
    klqUnitAnimPhaseSkillAttack = 5
};


typedef enum{
    klqUnitDirectionTypeNone = -1,      //无
    klqUnitDirectionTypeDown = 0,       //下S
    klqUnitDirectionTypeLeft = 1,       //左W
    klqUnitDirectionTypeUp = 2,         //上N
    klqUnitDirectionTypeRight = 3,      //右E
    klqUnitDirectionTypeLeftDown = 4,   //左下SW
    klqUnitDirectionTypeLeftUp = 5,     //左上NW
    klqUnitDirectionTypeRightDown = 6,  //右下SE
    klqUnitDirectionTypeRightUp = 7     //右上NE
}LQUnitDirectionType;

static const char* UnitDIR[] = {"Down", "Left", "Up", "Right", "LeftDown", "LeftUp", "RightDown", "RightUp"};

static LQUnitDirectionType UnitDirection[] = {
    klqUnitDirectionTypeUp,
    klqUnitDirectionTypeRightUp,
    klqUnitDirectionTypeRight,
    klqUnitDirectionTypeRightDown,
    klqUnitDirectionTypeDown,
    klqUnitDirectionTypeLeftDown,
    klqUnitDirectionTypeLeft,
    klqUnitDirectionTypeLeftUp
};

typedef enum{
    klqClockDirectionNone = 0,
    klqClockDirectionClockWise = 1,         // 顺时针方向
    klqClockDirectionCounterClockWise = 2   // 逆时针方向
}LQClockType;


typedef enum{
    klqCombatSquadDirectionLeft  = 0,   // 进攻方 左边
    klqCombatSquadDirectionRight = 1    // 防守方 右边
}LQCombatSquadDirectionEnum;

typedef enum{
    klqCombatSquadRowNone  = 0,
    klqCombatSquadRowFront  = 1,   // 军团位置 前排
    klqCombatSquadRowMiddle = 2,   // 军团位置 中排
    klqCombatSquadRowBack = 3      // 军团位置 后排
}LQCombatSquadRowEnum;

typedef enum
{
    Combat_People_None = 0,
    Combat_People_Infantry = 1,     // 剑兵
    Combat_People_Spearman = 2,     // 枪兵
    Combat_People_Cavalry = 3,      // 骑兵
    Combat_People_Halberdier = 4,   // 戟兵
    Combat_People_Archers = 5,      // 弩兵
    Combat_People_Elephants = 6,    // 象兵
    Combat_People_CavalryArc = 7,   // 弓骑兵
    Combat_People_Rattan = 8,       // 藤甲
    Combat_People_Women = 9         // 女兵
}LQCombatUnitType;

typedef enum
{
    Combat_ArmsType_None = 0,
    Combat_ArmsType_Melee = 1,    //枪兵
    Combat_ArmsType_Remote = 2,   //远程
    Combat_ArmsType_Cavalry = 3,  //骑兵
    Combat_ArmsType_Infantry = 4, //步兵
    Combat_ArmsType_Machine = 5   //机械
}LQArmsTypeEnum;
//1、步兵\r\n2、远程\r\n3、骑兵\r\n4、机械

typedef enum
{
    Combat_Occupation_None = 0,
    Combat_Occupation_Normal = 1,   //普通
    Combat_Occupation_Tactician = 2 //策士
}LQOccupationEnum;

typedef enum
{
    Combat_AttackType_Melee = 1,    // 近战
    Combat_AttackType_Remote = 2    // 远程
}LQAttackType;

typedef enum
{
    Combat_AttackObjectType_Hero  = 1,   // 英雄
    Combat_AttackObjectType_Squad = 2    // 兵团
}LQAttackObjectTypeEnum;

//兵种效果类型
typedef enum
{
    Combat_ShowEffect_None  = 0,   // 无
    Combat_ShowEffect_Forward = 1, // 骑兵冲锋
    Combat_ShowEffect_Assault = 2, // 步兵突击
    Combat_ShowEffect_Attack  = 10 // 无
}LQShowEffectTypeEnum;

//获取direction的数组index，因为UnitDirection和枚举的不对应造成。Why not！
static int getUnitDirectionIndex(LQUnitDirectionType direction)
{
    int m = -1;  //当前位置,起始位置
    for (int i = 0; i<8; i++) {
        if (direction == UnitDirection[i]){
            m = i;
            break;
        }
    }
    return m;
}

//方向取反
static LQUnitDirectionType getNotDirection(LQUnitDirectionType direction)
{
    switch (direction) {
        case klqUnitDirectionTypeDown:
            return klqUnitDirectionTypeUp;
        case klqUnitDirectionTypeUp:
            return klqUnitDirectionTypeDown;
        case klqUnitDirectionTypeLeft:
            return klqUnitDirectionTypeRight;
        case klqUnitDirectionTypeRight:
            return klqUnitDirectionTypeLeft;
        case klqUnitDirectionTypeLeftDown:
            return klqUnitDirectionTypeRightUp;
        case klqUnitDirectionTypeRightUp:
            return klqUnitDirectionTypeLeftDown;
        case klqUnitDirectionTypeLeftUp:
            return klqUnitDirectionTypeRightDown;
        case klqUnitDirectionTypeRightDown:
            return klqUnitDirectionTypeLeftUp;
        default:
            return klqUnitDirectionTypeNone;
    }
}
/*
//获取帧数量
static int getAnimationPhaseCount(LQUnitAnimPhaseEnum type)
{
    //LQUnitAnimPhaseEnum animPhase = (LQUnitAnimPhaseEnum)modifierNum;
    switch (type) {
        case klqUnitAnimPhaseFallDown:
            return 1;
            break;
        case klqUnitAnimPhaseMove:
            return 8;
            break;
        case klqUnitAnimPhaseStand:
            return 5;
            break;
        case klqUnitAnimPhaseAttack:
            return 8;
            break;
        case klqUnitAnimPhaseSkillAttack:
            return 8;
            break;
        default:
            break;
    }
    return -1;
}*/

LQ_NAMESPACE_END

#endif
