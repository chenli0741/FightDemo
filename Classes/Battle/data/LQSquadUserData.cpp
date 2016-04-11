//
//  LQHeroUserData.cpp
//  LostEmpire
//
//  Created by lequdev1 on 14-1-24.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include "LQSquadUserData.h"
#include "LQData.h"

LQSquadUserData::LQSquadUserData()
{
    m_soldierHpValue = 0;
    m_soldierApValue = 0;
    m_soldierDpValue = 0;
    
    m_HpValueMax = 0;
}

LQSquadUserData::~LQSquadUserData()
{
    CCLOG("~LQSquadUserData %p", this);
    //CC_SAFE_DELETE(m_soldierdata);
}

bool LQSquadUserData::initData(LQHeroUserData* heroData)
{
    m_resIDC = heroData->getResIDC();
    m_herodata = heroData;
    m_herodata->setSquadData(this);
    m_occupation = heroData->getOccupation();
    m_armsType = heroData->getArmsType();
    m_attackType = (LQAttackType)heroData->getArmsCfgdata()->getAttackType();
    m_critPic = heroData->getArmsCfgdata()->getCritPic();
    m_critFont = heroData->getArmsCfgdata()->getCritFont();
    m_spPic = heroData->getArmsCfgdata()->getSpPic();
    m_spFont = heroData->getArmsCfgdata()->getSpFont();
    string m_soldierRestrain = heroData->getSoldierRestrain();
    this->splitValue(m_soldierRestrain, ';', ',', false);
    //m_isMuseCrit = m_armsType==Combat_ArmsType_Cavalry;
    
    if(m_herodata->getSoldiers()->size()>0){
        LQArmsUserData* m_soldierdata = m_herodata->getSoldiers()->at(0);
        m_attackType = m_soldierdata->getAttackType();
        //string m_soldierRestrain = m_soldierdata->getArmsCfgdata()->getSoldierRestrain();
        //this->splitValue(m_soldierRestrain, ';', ',', false);
        //CCLOG("LQSquadUserData :%d %d %d %d %d", this->getCurrStdAtr(ATTR_STRIKE1), this->getCurrStdAtr(ATTR_STRIKE2), this->getCurrStdAtr(ATTR_STRIKE3), this->getCurrStdAtr(ATTR_STRIKE4), this->getCurrStdAtr(ATTR_STRIKE5));
    }
    
    m_rageBeAttack = heroData->getRageBeAttack();	//被攻击怒气
    m_rageAttack = heroData->getRageAttack();       //攻击怒气
    m_furyMaxValue = heroData->getFuryMaxValue();	//当前等级的怒气上限
    
    //m_spatkFacter = heroData->getSpatkFacter();
    
    m_soldierHpValue = heroData->getSoldHpValue();
    m_soldierApValue = heroData->getSoldApValue();
    m_soldierDpValue = heroData->getSoldDpValue();
    
    //将武将上得值复制过来
    ValueMap attrmap = heroData->getAttrMap();
    for (auto iter = attrmap.cbegin(); iter != attrmap.cend(); ++iter)
    {
        string skey = iter->first;
        if(skey.length()<=0) continue;
        m_attrMap[ skey ] = ((Value)(iter->second)).asInt();
    }
    setCurrStdAtr( ATTR_HP, m_soldierHpValue);  //生命
    setCurrStdAtr( ATTR_ATK, m_soldierApValue); //攻击
    setCurrStdAtr( ATTR_DEF, m_soldierDpValue); //防御
    
    //统计军团的生命
    m_HpValueMax = m_herodata->getSoldHpValue();
    m_soldierHpValue = m_herodata->getCurrSoldHpValue();
    setCurrStdAtr( ATTR_CURRHP, heroData->getCurrSoldHpValue());  //生命
    
    CCLOG("LQSquadUserData: SquadHP=%d = soldierHP=%d)", m_HpValueMax ,m_soldierHpValue);
        
    return true;

}
//value 为伤害值，减血时正值，士兵的值
int LQSquadUserData::changeSquadHp(int value)
{
    //if(value>10000)
    //    CCLOG(" changeSquadHp error ...");
    if (this->getCurrShValue()>0 && value>0){  //伤血时存在护盾，
        int v = this->getCurrShValue();
        if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
            CCLOG(" %d changeSquadHp getCurrShValue: %d", m_resIDC, v);
        if(v>=value){  //护盾足够
            this->setCurrStdAtr(ATTR_SHIELD,v-value);
            value = 0;
        }else{
            this->setCurrStdAtr(ATTR_SHIELD,0);  //护盾被打掉了
            value = value-v;
        }
    }
    
    if (value<0)  //负数是加血
    {
        int val = m_HpValue - this->getHpMaxValue();
        if (value<val) //暴血了
            value = val;
    }
    
    if( m_herodata->getResIDC()==90041930 ){ // ||_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
        CCLOG("==== %d changeSquadHp m_HpValue %d - value %d = %d", m_resIDC, m_soldierHpValue, value, m_soldierHpValue-value);
        if (value<0 )
            CCLOG("m_herodata is add HP");
        if (m_soldierHpValue-value<0 )
            CCLOG("m_herodata is dead");
    }

    m_soldierHpValue = m_soldierHpValue + value*(-1);
    if(m_soldierHpValue<0)
        m_soldierHpValue = 0;
    m_HpValue = m_soldierHpValue;
    
    return value;
}

//获取当前军团的血量
int LQSquadUserData::getSquadHpValue()
{
    int sqhpValue = m_HpValue;  //12.18修改去掉武将伤害 + m_herodata->getCurrHpValue()/HERO_RATE;
    if(sqhpValue<0)
        CCLOG("ERROR getSquadHpValue: %d %d", m_HpValue, m_herodata->getCurrHpValue()/HERO_RATE);
    return sqhpValue;
}

//获取当前军团战力
int LQSquadUserData::getPower()
{
    return m_soldierHpValue*(this->getCurrStdAtr(ATTR_ATK)+this->getCurrStdAtr(ATTR_DEF))*(this->getCurrStdAtr(ATTR_HURTADD)/RATE_PERCENT+this->getCurrStdAtr(ATTR_HURTSUB)/RATE_PERCENT+1)*(this->getCurrStdAtr(ATTR_ARMOR)/RATE_PERCENT+this->getCurrStdAtr(ATTR_ARMOR_DE)/RATE_PERCENT+1);
}

#pragma mark LQArmDataInterface
//有效攻击速度
float LQSquadUserData::getAttackSpeed()
{
    return 1;
}
