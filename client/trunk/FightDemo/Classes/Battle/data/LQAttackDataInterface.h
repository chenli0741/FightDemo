//
//  LQAttackDataInterface.h
//  SuperWarriors
//
//  Created by chenli on 14-6-19.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//
#ifndef __SuperWarriors_LQAttackDataInterface_H__
#define __SuperWarriors_LQAttackDataInterface_H__

#include "cocos2d.h"
#include "../unit/LQCombatUnitEnums.h"
#include "LQBaseUserData.h"
#include "LQArmsInfoData.h"
#include "LQActionInfoData.h"

static const int ATTR_ADDVALUE  = 500; //增加值统一序号加500
static const int ATTR_MAIN  = 10; //英雄主属性减去10 生命5 攻击7 防御8

static const int ATTR_CURRHP    = 1; //当前生命
static const int ATTR_LOSTHP    = 2; //缺失生命

static const int ATTR_HEROHP    = 5; //英雄生命
static const int ATTR_HEROATK   = 7; //英雄攻击
static const int ATTR_HERODEF   = 8; //英雄防御

static const int ATTR_SHIELD   = 100;//护盾血条
static const int ATTR_ATKDIST  = 101;//攻击距离
static const int ATTR_FACTER   = 102;//固定系数值 恒等于1  cl-add4.1
static const int ATTR_SQUEHP   = 103;//当前军团生命
static const int ATTR_SPATKFAC = 104;//突击增加额外伤害系数 cl-add4.10

static const int ATTR_DIE1  = 105;   //死亡敌方军团 cl-add9.14
static const int ATTR_DIE2  = 106;   //死亡我方军团 cl-add9.14
static const int ATTR_LIVE1 = 107;   //存活敌方军团 cl-add9.14
static const int ATTR_LIVE2 = 108;   //存活我方军团 cl-add9.14
static const int ATTR_LIVE3 = 109;   //存活军团差 cl-add9.14

static const int ATTR_FOR   = 12; //武勇
static const int ATTR_INT   = 13; //智力
static const int ATTR_COM   = 14; //统帅
static const int ATTR_HP    = 15; //生命
static const int ATTR_ATK   = 17; //攻击
static const int ATTR_DEF   = 18; //防御

static const int ATTR_HURTADD   = 25; //伤害加深
static const int ATTR_HURTSUB   = 26; //伤害减免
static const int ATTR_ARMOR     = 27; //破甲
static const int ATTR_ARMOR_DE  = 28; //固甲
static const int ATTR_RAGE      = 31; //怒气
static const int ATTR_RAGEMAX   = 32; //怒气上限
static const int ATTR_RAGERATE  = 33; //怒气%

static const int ATTR_STRIKE1    = 41; //步兵压制
static const int ATTR_STRIKE_DE1 = 42; //步兵韧性
static const int ATTR_STHURT1    = 43; //压步伤害
static const int ATTR_STHURT_DE1 = 44; //压步免伤
static const int ATTR_STRIKE2    = 45; //弓兵压制
static const int ATTR_STRIKE_DE2 = 46; //弓兵韧性
static const int ATTR_STHURT2    = 47; //压弓伤害
static const int ATTR_STHURT_DE2 = 48; //压弓免伤
static const int ATTR_STRIKE3    = 49; //骑兵压制
static const int ATTR_STRIKE_DE3 = 50; //骑兵韧性
static const int ATTR_STHURT3    = 51; //压骑伤害
static const int ATTR_STHURT_DE3 = 52; //压骑免伤
static const int ATTR_STRIKE4    = 53; //策士压制
static const int ATTR_STRIKE_DE4 = 54; //策士韧性
static const int ATTR_STHURT4    = 55; //压策伤害
static const int ATTR_STHURT_DE4 = 56; //压策免伤
static const int ATTR_STRIKE5    = 57; //机械压制
static const int ATTR_STRIKE_DE5 = 58; //机械韧性
static const int ATTR_STHURT5    = 59; //压机伤害
static const int ATTR_STHURT_DE5 = 60; //压机免伤

static const int ATTR_STRIKEALL    = 71; //全兵种压制
static const int ATTR_STRIKE_DEALL = 72; //全兵种韧性
static const int ATTR_STHURTALL    = 73; //压全兵种伤害
static const int ATTR_STHURT_DEALL = 74; //压全兵种免伤

static const int ATTR_AFFECT_TREAT = 80; //影响治疗%

//add 4.10需求 正数提高，负数减少
static const int ATTR_HURTRATE1 = 121; //对步兵伤害提高或减少%
static const int ATTR_HURTRATE2 = 122; //对弓兵伤害提高或减少%
static const int ATTR_HURTRATE3 = 123; //对骑兵伤害提高或减少%
static const int ATTR_HURTRATE4 = 124; //对策士伤害提高或减少%
static const int ATTR_HURTRATE5 = 125; //对机械伤害提高或减少%

static const int ATTR_CRITHURTRATE1 = 126; //对步兵暴击伤害提高或减少%
static const int ATTR_CRITHURTRATE2 = 127; //对弓兵暴击伤害提高或减少%
static const int ATTR_CRITHURTRATE3 = 128; //对骑兵暴击伤害提高或减少%
static const int ATTR_CRITHURTRATE4 = 129; //对策士暴击伤害提高或减少%
static const int ATTR_CRITHURTRATE5 = 130; //对机械暴击伤害提高或减少%

static const int ATTR_DEHURTRATE1 = 131; //对步兵伤害减免提高或减少%
static const int ATTR_DEHURTRATE2 = 132; //对弓兵伤害减免提高或减少%
static const int ATTR_DEHURTRATE3 = 133; //对骑兵伤害减免提高或减少%
static const int ATTR_DEHURTRATE4 = 134; //对策士伤害减免提高或减少%
static const int ATTR_DEHURTRATE5 = 135; //对机械伤害减免提高或减少%

static const int ATTR_HURTRATEALL   = 141;   //对全兵种伤害提高或减少%
static const int ATTR_CRITHURTRATEALL = 142; //对全兵种暴击伤害提高或减少%
static const int ATTR_DEHURTRATEALL = 143;   //对全兵种伤害减免提高或减少%

//战斗数据接口
class LQAttackDataInterface
{
protected:
    bool m_isCrit;      //是否暴击
    bool m_isMuseCrit;  //必暴标志，用于技能控制产生暴击
    bool m_isForward;   //是否突击
    bool m_isMuseNotCrit;  //必不暴标志，用于技能控制产生暴击
public:
    LQAttackDataInterface() {
        m_occupation = Combat_Occupation_None;
        m_attackObjtype = Combat_AttackObjectType_Squad;
    };
    virtual ~LQAttackDataInterface() {};
    
    //军团兵种\r\n1、步兵\r\n2、远程\r\n3、骑兵\r\n4、机械
    CC_SYNTHESIZE(LQArmsTypeEnum, m_armsType, ArmsType );
    //武将职业\r\n1、普通\r\n2、策士
    CC_SYNTHESIZE(LQOccupationEnum, m_occupation, Occupation );
    //攻击计算方式
    CC_SYNTHESIZE_READONLY(LQAttackObjectTypeEnum, m_attackObjtype, AttackObjectType );
    
    
    virtual float getAttackSpeed()= 0;     //有效攻击速度？？？？
    
    virtual float getCurrentDefense(LQAttackDataInterface* armsdata)= 0;//有效防御值
    virtual float getCurrentDPS(LQAttackDataInterface* armsdata) = 0;   //总伤害输出
    virtual bool isCrit(LQAttackDataInterface* armsdata)= 0;            //是否暴击
    bool getIsCrit(){ return m_isCrit; };                               //用来获取本次是否暴击标志
    void setIsMustCrit(){ m_isMuseCrit = true; };                       //本次必爆，不影响暴击系数
    void setIsMustNotCrit(){ m_isMuseNotCrit = true; };                 //本次必不爆，不影响暴击系数
    void setIsMustForward(){
        m_isForward = true; m_isMuseCrit = true;
    }; //本次突击,加必爆
    bool getIsForward(){ return m_isForward; };                         //获取突击效果的额外伤害
    
    virtual int getHpMaxValue() = 0; //当前总生命
    virtual int getCurrHpValue() = 0; //当前生命
    virtual int getCurrApValue() = 0; //当前攻击
    virtual int getCurrDpValue() = 0;  //当前防御
    virtual float getCurrSpatkFacter() = 0; //当前突击增加额外伤害
    
    virtual int getForInitComValue()= 0;  //武勇+智力统率
    
    // 属性
    virtual float getCurrStdAtr(int attrtype)= 0;             //当前属性
    virtual void setCurrStdAtr(int attrtype, int vlaue)= 0; //当前属性
    virtual void addCurrStdAtr(int attrtype, int vlaue)= 0; //添加当前属性
    // 暴击 克制
    virtual float getCurrStrike(LQArmsTypeEnum armsType)= 0;   //当前暴击
    virtual float getCurrDeStrike(LQArmsTypeEnum armsType)= 0; //当前韧性
    virtual float getCurrDamage(LQArmsTypeEnum armsType)= 0;   //当前暴伤
    virtual float getCurrDeDamage(LQArmsTypeEnum armsType)= 0; //当前免暴伤
    // 兵种伤害百分比 4.10
    virtual float getCurrHurtRate(LQArmsTypeEnum armsType)= 0; //对兵种伤害提高或者降低%
    // 兵种暴击伤害百分比 4.10
    virtual float getCritHurtRate(LQArmsTypeEnum armsType)= 0; //对兵种暴击伤害的百分比%
    // 兵种伤害减免百分比 4.11
    virtual float getCurrDeHurtRate(LQArmsTypeEnum armsType)= 0; //对兵种伤害减免提高或者降低%
    
    // buffer update
    virtual void update(float dt) = 0;
};

#endif