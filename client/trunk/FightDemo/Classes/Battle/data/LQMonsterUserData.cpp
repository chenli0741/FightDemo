//
//  LQHeroUserData.cpp
//  LostEmpire
//
//  Created by lequdev1 on 14-1-24.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include "LQMonsterUserData.h"
#include "LQData.h"

LQMonsterUserData::LQMonsterUserData(int pkey, int resIDC, int soldierIDC, int count)
{
    m_pkey = pkey;
    m_soldierIDC = soldierIDC;
    m_soldierCount = count;
    m_stuntEffect = -1;
    aliasName = "MonsterInfo";
    m_soldiers = new LQArmsUserDataVector();
    m_dpsTotal = 0;     // 攻击的伤害统计
    m_killTotal = 0;    // 杀死敌人数量统计
    m_stopSkill = false;
    //m_heroFuries= 0;
    _skillData = NULL;
    _skillData0 = NULL;
    _skillData1 = NULL;
    _skillData2 = NULL;
    m_activeSkillID = -1;
    m_passiveSkill1ID = -1;
    m_passiveSkill2ID = -1;
    m_tacticianSkillID = -1;
    this->initWithIDC(resIDC);
}

LQMonsterUserData::~LQMonsterUserData()//宝物
{
    CCLOG("~LQMonsterUserData %p", this);
    //if (m_soldiers) {
    //    deleteInVector(*m_soldiers);
    //}
    //CC_SAFE_DELETE(m_soldiers);
}

// 初始化数据
bool LQMonsterUserData::init()
{
    if(!LQArmsUserData::init())
        return false;
    
    //CCX_PROPERTY_IMPLEMENT_INT_INIT(level,m_level)
    //CCX_PROPERTY_IMPLEMENT_STR_INIT(cityid,m_cityId)
    
    return true;
}
/*
LQArmsInfoData* LQMonsterUserData::getArmsCfgdata()
{
    return this->m_armsCfgdata;
}*/

// 返回配置数据子类覆盖
LQMonsterInfoData* LQMonsterUserData::getMonsterCfgdata()
{
    return (LQMonsterInfoData*)this->m_cfgdata;
}

//子类重写
const char* LQMonsterUserData::getAliasName()
{
    return this->aliasName;
}

// 初始化数据,子类重写
void LQMonsterUserData::putData()
{
    LQHeroUserData::putData();
    
    this->getMonsterCfgdata()->getValue("NameID", &m_name);          //卡牌名称
    this->getMonsterCfgdata()->getValue("ShortName", &m_shortname);//卡牌简称
    this->getMonsterCfgdata()->getValue("HeadID", &m_headID);      //卡牌将领的头像ID
    this->getMonsterCfgdata()->getValue("CardID", &m_cardID);      //卡牌将领的卡牌ID
    this->getMonsterCfgdata()->getValue("Quality", &m_quality);    //卡牌将领的品质
    this->getMonsterCfgdata()->getValue("DefaultSoldier", &m_defaultSoldier); 	//初始化携带兵种
    this->getMonsterCfgdata()->getValue("ActiveSkillID", &m_activeSkillID);    //主动技能
    this->getMonsterCfgdata()->getValue("PassiveSkill1ID", &m_passiveSkill1ID);    //被动技能1
    this->getMonsterCfgdata()->getValue("PassiveSkill2ID", &m_passiveSkill2ID);    //被动技能2
    this->getMonsterCfgdata()->getValue("StarLevel", &m_star);     //初始化星级
    this->getMonsterCfgdata()->getValue("BattlePower", &m_power);  //初始化战斗力
    
    m_speed = getMonsterCfgdata()->getSpeed()*0.001f;
    m_patrolSpeed = getMonsterCfgdata()->getPatrolSpeed();
    
    if(m_soldierIDC<=0)
        m_soldierIDC = m_defaultSoldier;

    m_stuntEffect = getMonsterCfgdata()->getStuntEffect();
    
    //初始数据
    this->defaultData();
}

//设置初始值，我的英雄将由相同传入
void LQMonsterUserData::defaultData()
{
    LQHeroUserData::defaultData();
}
