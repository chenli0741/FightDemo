//
//  LQSquadUserData.h
//  SuperWarriors
//
//  Created by lequdev1 on 14-1-24.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//
#ifndef __SuperWarriors_LQSquadUserData_H__
#define __SuperWarriors_LQSquadUserData_H__

#include "cocos2d.h"
#include "LQBaseUserData.h"
#include "LQCardInfoData.h"
#include "LQArmsInfoData.h"
#include "LQArmsUserData.h"
#include "LQSkillUserData.h"
#include "LQAttackCalculator.h"

static const int HERO_RATE   = 100; //武将血缩小系数

class LQSquadUserData:public LQBaseUserData, public LQAttackCalculator
{
private:
    LQHeroUserData* m_herodata;
    //LQArmsUserData* m_soldierdata;
    
    int m_soldierHpValue; //兵的血量
    int m_soldierApValue; //兵的攻击
    int m_soldierDpValue; //兵的防御
public:
    LQSquadUserData();
    ~LQSquadUserData();
    bool initData(LQHeroUserData* heroData);
    
    //返回配置数据类，子类覆盖
    LQHeroUserData* getHeroData(){  return m_herodata; };
    
    CC_SYNTHESIZE_READONLY(int, m_HpValueMax, HpValueMax );         //军团血量
    int changeSquadHp(int value);//value 为伤害值，减血时正值；返回实际伤血量，中间扣除了护盾
    int getSquadHpValue();
    int getPower();
    
    //决定采取的攻击方式，复合攻击方式读取两个攻击力数值\r\n1：近战\r\n2：远程
    CC_SYNTHESIZE_READONLY(LQAttackType, m_attackType, AttackType );
    CC_SYNTHESIZE_READONLY(std::string, m_critPic, CritPic );	//暴击使用的特效图片，如步兵是“奋起”、骑兵是“突击”、弓兵是“齐射”等等
    CC_SYNTHESIZE_READONLY(std::string, m_critFont, CritFont );
    CC_SYNTHESIZE_READONLY(std::string, m_spPic, SpPic );	// 特殊效果图片
    CC_SYNTHESIZE_READONLY(std::string, m_spFont, SpFont );	// 特殊效果字体
    
    //LQArmDataInterface 计算攻击值
    float getAttackSpeed();     //有效攻击速度
    
};
#endif