//
//  LQArmsUserData.h
//  SuperWarriors
//
//  Created by chenli on 14-6-19.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//
#ifndef __SuperWarriors_LQArmsUserData_H__
#define __SuperWarriors_LQArmsUserData_H__

#include "cocos2d.h"
#include "LQBaseUserData.h"
#include "LQArmsInfoData.h"
#include "../unit/LQCombatUnitEnums.h"
#include "LQActionInfoData.h"
#include "LQEffectUserData.h"
#include "LQAttackCalculator.h"

class LQHeroUserData;

class LQArmsUserData : public LQBaseUserData, public LQAttackCalculator
{
public:
    LQArmsUserData();
    ~LQArmsUserData();
    
    /**返回配置数据类，子类覆盖
     */
    //LQArmsInfoData* getCfgdata();
    LQArmsInfoData* getArmsCfgdata();
    // 初始化数据,子类重写
    void putData();
    //void putDataWithMessage(LQServerResponse* resp);
    
    //create()
    CREATEDATA_FUNC(LQArmsUserData);
    
    //CC_SYNTHESIZE(LQHeroUserData*, m_herodata, HeroData );	//英雄
    
    //队列方向(对型方向，站的方向)
    CC_SYNTHESIZE(LQCombatSquadDirectionEnum, m_squadDirection, SquadDirection) //队列方向
    
    // 移动速度（毫秒/格子)
    CC_SYNTHESIZE(float, m_speed, Speed );
    // 军团攻击视野（格子)
    CC_SYNTHESIZE(float, m_atkview, AtkView );
    // 巡逻移动速度（毫秒/格子)
    CC_SYNTHESIZE(float, m_patrolSpeed, PatrolSpeed );
    //决定采取的攻击方式，复合攻击方式读取两个攻击力数值\r\n1：近战\r\n2：远程\r\n
    CC_SYNTHESIZE_READONLY(LQAttackType, m_attackType, AttackType );
    
    //兵种的类别\r\n1、剑兵\r\n2、枪兵\r\n3、骑兵\r\n4、戟兵\r\n5、弩兵\r\n6、象兵\r\n7、藤甲\r\n8、女兵
    CC_SYNTHESIZE(int, m_unitType, UnitType );
    // 战斗数值相关属性
    CC_SYNTHESIZE(int, m_level, Level );        //等级
    CC_SYNTHESIZE(int, m_power, Power );        //战斗力
    //
    CC_SYNTHESIZE_READONLY(std::string, m_flashID, FlashID );	//光影资源ID，用于普通攻击的刀光
    
    // 一级属性
    void setHpValue(int value);
    virtual void changeHpValue(int value);

    // 动作时间数据
    CC_SYNTHESIZE_READONLY(LQActionInfoData*, m_actionInfo, ActionInfo );
    CC_SYNTHESIZE_READONLY(int, m_shapeID, ShapeID );	//序列帧的资源文件
    CC_SYNTHESIZE_READONLY(string, m_baseName, BaseName );	//资源文件名
    
    CC_SYNTHESIZE_READONLY(int, m_standBy, StandBy );	//待命动作序列
    CC_SYNTHESIZE_READONLY(int, m_move, Move );	//移动动作序列
    CC_SYNTHESIZE_READONLY(int, m_death, Death );	//死亡动作序列
    CC_SYNTHESIZE_READONLY(int, m_melATKR, MelATKR );	//近战攻击准备动作序列，攻击准备完成后，即命中目标。－1表示无此动作
    CC_SYNTHESIZE_READONLY(int, m_melATKF, MelATKF );	//近战攻击完成动作序列，攻击准备完成后，回复到再次可以准备攻击的待命状态的动作过程。－1表示无此动作
    CC_SYNTHESIZE_READONLY(int, m_stATKT, StATKT );	//技能攻击重复次数
    CC_SYNTHESIZE_READONLY(int, m_stATKR, StATKR );	//技能攻击准备动作序列，攻击准备完成后，即放出技能。－1表示无此动作
    CC_SYNTHESIZE_READONLY(int, m_stATKN, StATKN );	//技能攻击完成动作序列，攻击准备完成后，回复到再次可以准备攻击的待命状态的动作过程。－1表示无此动作

    
    //buff增加属性
    CC_SYNTHESIZE(float, m_SpeedBuffAdd, SpeedBuffAdd );	//速度+速度*SpeedBuffAdd
    
    //特殊buff状态
    CC_SYNTHESIZE(bool, m_isInvincible, IsInvincible );	//是否无敌
    CC_SYNTHESIZE(bool, m_isCanMove, IsCanMove );	//是否可以移动
    CC_SYNTHESIZE(int, m_isCanAttack, IsCanAttack );	//固甲+Buff
   
    //LQArmDataInterface 计算攻击值
    float getAttackSpeed();     //有效攻击速度
    float getCurrSpeedValue();//当前速度
    
    // 动作攻击持续时间
    CC_SYNTHESIZE(float, m_attackAnimTime, AttackAnimTime);
    int getCurrAnimationCount(LQUnitAnimPhaseEnum phaseType);
    int getCurrAnimationPhaseCount(LQUnitAnimPhaseEnum phaseType);

protected:
    int m_attackSpeed;	//以毫秒为单位，兵种从发动攻击指令到攻击动作完成所需要的时间
    int m_attackGap;	//攻击方式1攻击间隔时间，万分之一秒
    int m_attackDistMax;  //攻击方式的有效范围
    
    void testData();
};

typedef vector<LQArmsUserData*> LQArmsUserDataVector;

#endif