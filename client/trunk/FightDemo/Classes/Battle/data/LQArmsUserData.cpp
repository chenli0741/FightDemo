//
//  LQArmsUserData.cpp
//  SuperWarriors
//
//  Created by chenli on 14-6-19.
//  Copyright (c) 2014年 __LQArmsUserData__. All rights reserved.
//

#include "LQArmsUserData.h"
#include "LQData.h"

LQArmsUserData::LQArmsUserData(): m_speed(0), m_patrolSpeed(0), m_attackType(Combat_AttackType_Melee),m_SpeedBuffAdd(0),m_isInvincible(false),m_isCanMove(true),m_isCanAttack(true)
{
    aliasName = "ArmsInfo";
    m_actionInfo = NULL;
    
    //m_herodata = NULL;
    m_atkview = 1;
    m_armsType = Combat_ArmsType_None;
    m_occupation = Combat_Occupation_None;
    m_flashID = "";
    m_power = 0;
}

LQArmsUserData::~LQArmsUserData()
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SAFE) CCLOG("~LQArmsUserData %p", this);
}


// 返回配置数据子类覆盖
LQArmsInfoData* LQArmsUserData::getArmsCfgdata()
{
    return (LQArmsInfoData*)this->m_cfgdata;
}

// 初始化数据,子类重写
void LQArmsUserData::putData()
{
    if(!this->getCfgdata())
        return;
    //TODO
    this->getCfgdata()->getValue("AttackType", &m_attackType);
    this->getCfgdata()->getValue("AttackSpeed", &m_attackSpeed);
    this->getCfgdata()->getValue("AttackGap", &m_attackGap);
    this->getCfgdata()->getValue("AttackDistMax", &m_attackDistMax);
    
    m_atkview = m_attackDistMax;
    
    if(getArmsCfgdata()){
        m_speed = getArmsCfgdata()->getSpeed()*0.001f;
        m_patrolSpeed = getArmsCfgdata()->getSpeed()*0.001f;
    }
    if( strcmp(aliasName,"ArmsInfo")==0 )
        m_flashID = getArmsCfgdata()->getFlashID().c_str();
    
    //动作资源
    this->getCfgdata()->getValue("ShapeID", &m_shapeID);
    m_baseName = "";
    if(m_shapeID>0){
        m_actionInfo = (LQActionInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQActionInfoData::getAliasName(),m_shapeID);
        m_baseName = m_actionInfo->getResName().c_str();
        m_unitType = m_actionInfo->getUnitType();
        m_melATKR = m_actionInfo->getMelATKR();
        m_melATKF = m_actionInfo->getMelATKF();
        m_stATKT = m_actionInfo->getStATKT();
        m_stATKR = m_actionInfo->getStATKR();
        m_stATKN = m_actionInfo->getStATKN();
        m_standBy = m_actionInfo->getStandBy();
        m_move = m_actionInfo->getMove();
        m_death = m_actionInfo->getDeath();
    }
    
    m_level = 1;    //等级
    m_HpValue = 0;	//生命
    
    //buff增加属性
    m_SpeedBuffAdd =0;	//速度+Buff
    
    //攻击间隔，用于攻击的输出频率，现在不想做出变化，就不要加入m_attackGap
    //m_attackAnimTime = AnimationFPS * getCurrAnimationPhaseCount(klqUnitAnimPhaseAttack)+m_attackGap*0.001f;
    m_attackAnimTime = AnimationFPS * getCurrAnimationPhaseCount(klqUnitAnimPhaseAttack)+0.2;  //0.2降速度
    
    //testData();
    return;
}

int LQArmsUserData::getCurrAnimationCount(LQUnitAnimPhaseEnum phaseType)
{
    switch (phaseType) {
        case klqUnitAnimPhaseFallDown:
            return m_death;
            break;
        case klqUnitAnimPhaseMove:
            return m_move;
            break;
        case klqUnitAnimPhaseStand:
            return m_standBy;
            break;
        case klqUnitAnimPhaseAttack:
            return m_melATKR + m_melATKF;
            break;
        case klqUnitAnimPhaseSkillAttack:
            return m_stATKN;
            break;
        default:
            break;
    }
    return -1;
}

int LQArmsUserData::getCurrAnimationPhaseCount(LQUnitAnimPhaseEnum phaseType)
{
    switch (phaseType) {
        case klqUnitAnimPhaseFallDown:
            return m_death;
            break;
        case klqUnitAnimPhaseMove:
            return m_move;
            break;
        case klqUnitAnimPhaseStand:
            return m_standBy;
            break;
        case klqUnitAnimPhaseAttack:
            return m_melATKR + m_melATKF;
            break;
        case klqUnitAnimPhaseSkillAttack:
            return m_stATKN*(m_stATKT>0?m_stATKT:1);
            break;
        default:
            break;
    }
    return -1;
}

void LQArmsUserData::testData()
{
    
}

//生命HP

void LQArmsUserData::setHpValue(int value)
{
    //不让出现负的生命值
    if(value<=0) value = 0;
    //setCurrStdAtr(ATTR_HP, value);
    m_HpValue = value;
}

//变更生命HP
void LQArmsUserData::changeHpValue(int value)
{
    //setCurrStdAtr(ATTR_HP, getCurrStdAtr(ATTR_HP)+value);
    setHpValue(m_HpValue+value);
}

#pragma mark LQArmDataInterface
//有效攻击速度 ？？？？
float LQArmsUserData::getAttackSpeed()
{
    return 1;
}


//当前速度
float LQArmsUserData::getCurrSpeedValue()
{
    //if(m_SpeedBuffAdd!=0)
    //    CCLOG("m_SpeedBuffAdd:%.3f", m_SpeedBuffAdd);
    return m_speed + m_speed*m_SpeedBuffAdd;
}

