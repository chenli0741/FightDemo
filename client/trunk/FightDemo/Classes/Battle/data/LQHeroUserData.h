//
//  LQPayInfoUserData.h
//  LostEmpire
//
//  Created by lequdev1 on 14-1-24.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//
#ifndef __LQCardUserData_H__
#define __LQCardUserData_H__

#include "cocos2d.h"
#include "LQBaseUserData.h"
#include "LQCardInfoData.h"
#include "LQArmsInfoData.h"
#include "LQArmsUserData.h"
#include "LQSkillUserData.h"

class LQSquadUserData;

class LQHeroUserData : public LQArmsUserData
{
private:
    LQArmsInfoData* m_armsCfgdata;
    
protected:
    virtual bool init();
public:
    //const char* aliasName;
    LQHeroUserData(){}
    LQHeroUserData(int resIDC, int soldierIDC, int count);
    ~LQHeroUserData();
    
    //返回配置数据类，子类覆盖
    LQArmsInfoData* getArmsCfgdata();
    
    // 初始化数据,子类重写
    const char* getAliasName();
    void putData();
    // 当数据都传入后，分配和计算数值
    void calcData();
    
    CC_SYNTHESIZE(int,  m_index,  Index);
    
    CC_SYNTHESIZE(LQSquadUserData*, m_squadData, SquadData); // 对应的兵团的数据
    
    CC_SYNTHESIZE_READONLY(int , m_soldierIDC, SoldierIDC );//武将士兵IDC
    CC_SYNTHESIZE_READONLY(std::string, m_name, Name );	//卡牌名称
    CC_SYNTHESIZE_READONLY(std::string, m_shortname, ShortName );	//卡牌名称
    CC_SYNTHESIZE_READONLY(std::string, m_headID, HeadID );	//卡牌将领的头像ID
    CC_SYNTHESIZE_READONLY(std::string, m_cardID, CardID );	//卡牌将领的卡牌ID
    CC_SYNTHESIZE(int, m_quality, Quality );	//卡牌将领的品质
    CC_SYNTHESIZE(int, m_qualityId, QualityId );	//卡牌将领的品质
    CC_SYNTHESIZE(int, m_star, Star );	//卡牌将领的星级
    CC_SYNTHESIZE_READONLY(int, m_activeSkillID, ActiveSkillID );	    //主动技能
    CC_SYNTHESIZE_READONLY(int, m_passiveSkill1ID, PassiveSkill1ID );	//被动技能1
    CC_SYNTHESIZE_READONLY(int, m_passiveSkill2ID, PassiveSkill2ID );	//被动技能2
    CC_SYNTHESIZE_READONLY(int, m_tacticianSkillID, TacticianSkillID );	//策士技能
    CC_SYNTHESIZE_READONLY(int, m_defaultSoldier, DefaultSoldier );	//初始化携带兵种
    
    CC_SYNTHESIZE_READONLY(int, m_stuntEffect, StuntEffect);//武将释放技能时强化效果的特效
    
    CC_SYNTHESIZE_READONLY(std::string, m_atkSound, AtkSound );	//%d表示通配数字号，两位数字
    CC_SYNTHESIZE_READONLY(int, m_atkSNum, AtkSNum );	//攻击配音文件个数
    CC_SYNTHESIZE_READONLY(std::string, m_dthSound, DthSound );	//%d表示通配数字号，两位数字
    CC_SYNTHESIZE_READONLY(int, m_dthSNum, DthSNum );	//死亡配音文件个数
    
    //将所带兵的数据
    CC_SYNTHESIZE_READONLY(int, m_defaultSoldierAP, DefaultSoldierATK );	//初始化兵种的攻击
    CC_SYNTHESIZE_READONLY(int, m_defaultSoldierHP, DefaultSoldierHP );     //初始化兵种的生命
    CC_SYNTHESIZE_READONLY(int, m_defaultSoldierDP, DefaultSoldierDP );     //初始化兵种的防御
    CC_SYNTHESIZE_READONLY(LQCombatUnitType, m_soldierUnitType, SoldierUnitType );     //武将所带兵种的类型
    CC_SYNTHESIZE_READONLY(std::string, m_soldierRestrain, SoldierRestrain );	//属性,值;（值/10000)
    
    string getAtkMusic();
    string getDthMusic();
    
    // 阵型Index
    CC_SYNTHESIZE(int, m_formationIndex, FormationIndex );
    //携带兵种
    CC_SYNTHESIZE(LQArmsUserDataVector*, m_soldiers, Soldiers)
    //携带兵种数量
    CC_SYNTHESIZE(int, m_soldierCount, SoldierCount)
    // 移动速度（毫秒/格子)
    CC_SYNTHESIZE(float, m_speed, Speed )
    // 巡逻移动速度（毫秒/格子)
    CC_SYNTHESIZE(float, m_patrolSpeed, PatrolSpeed );
    CC_SYNTHESIZE(cocos2d::Node*, m_nodeParent, NodeParent);//获取武将的数据
    
    // 战斗数值相关属性
    // 武将基本数值
    CC_SYNTHESIZE_READONLY(int, m_heroHp, HeroHp );      //武将生命
    CC_SYNTHESIZE_READONLY(int, m_heroAtk, HeroAtk );	//武将攻击
    CC_SYNTHESIZE_READONLY(int, m_heroDef, HeroDef );	//武将防御
    
    CC_SYNTHESIZE_READONLY(int, m_forValue, ForValue );	//初始武勇
    CC_SYNTHESIZE_READONLY(int, m_intValue, IntValue );	//初始智力
    CC_SYNTHESIZE_READONLY(int, m_comValue, ComValue );	//初始统率
    // 兵基本数值
    CC_SYNTHESIZE_READONLY(int, m_sHpValue, SoldHpValue ); //兵的生命
    CC_SYNTHESIZE_READONLY(int, m_sApValue, SoldApValue );	//兵的攻击
    CC_SYNTHESIZE_READONLY(int, m_sDpValue, SoldDpValue );	//兵的防御
    int getCurrSoldHpValue();    //兵的生命
    
    CC_SYNTHESIZE(int, m_rageMin, RageMin );	//当前等级的怒气上限
    //CC_SYNTHESIZE(int, m_armyTotalHP, ArmyTotalHP );	//武将和军团总生命值（初始时候的),计算怒气时候用的
    
    CC_SYNTHESIZE(bool, m_stopSkill, StopSkill);        //静止技能
    
    CC_SYNTHESIZE_READONLY(bool, m_assaultStaus, IsAssaultStaus);  //突袭状态
    
    //将的技能
    CC_SYNTHESIZE_READONLY(LQSkillUserData*, _skillData, ActiveSkillData); //主动技能
    CC_SYNTHESIZE_READONLY(LQSkillUserData*, _skillData1, SkillData1); //被动技能1
    CC_SYNTHESIZE_READONLY(LQSkillUserData*, _skillData2, SkillData2); //被动技能2
    CC_SYNTHESIZE_READONLY(LQSkillUserData*, _skillData0, TacticianSkillData); //主动技能
    bool isFirstSkill();  //是否先手技能
    
    virtual void changeHpValue(int value);
    virtual int getHpMaxValue(); //当前总生命
    virtual int getCurrApValue();//当前攻击
    virtual int getCurrDpValue();//当前防御
    
    // 怒气
    //CC_SYNTHESIZE(int, m_heroFuries, HeroFuries);
    void setHeroFuries(int value){
        if (value > m_furyMaxValue) {
            value = m_furyMaxValue;
        }else if (value<0) value = 0;
        //if (m_resIDC==90042087)
        //    CCLOG("ATTR_RAGE %f", this->getCurrStdAtr(ATTR_RAGE));
        this->setCurrStdAtr(ATTR_RAGE, value);
    };
    int getHeroFuries(){
        //if (m_resIDC==90042087)
        //    CCLOG("ATTR_RAGE %f", this->getCurrStdAtr(ATTR_RAGE));
        return this->getCurrStdAtr(ATTR_RAGE);
    };
    // 是否使用技能攻击（毫秒/格子)
    virtual int getPassiveSkillID();//当skillid 为0 时，为普通攻击
    bool isCanFreeActiveSkill();//是否可以释放主动技能
    bool isCanFreePassiveSkill(LQSkillUserData* passiveSkill); //是否可以释放被动技能
    // Kill统计
    void changeKillTotal(int value);
    //变更英雄伤害统计
    void changeHeroDpsTotal(int value);
    //变更怒气 最大不得超过最大怒气上限
    void changeHeroFuries(int value);
    //怒气比率
    bool getHeroFuriesRadio(){ return (getHeroFuries()*1.0f)/m_furyMaxValue>0.95f; };
    //增涨怒气比率
    void addHeroFuriesRadio(float radio);
    //血比率
    int getHPRate();
    int getCurrHpValue();
    //当前军团血量
    int getSquadHpValue();
    //当前军团伤血量
    int getSquadHurtValue();
    //本场当前军团伤血量
    int getSquadCurrHurtValue();
    
    // 攻击的伤害统计
    CC_SYNTHESIZE(int, m_dpsTotal, DpsTotal);
    // 杀死敌人数量统计
    CC_SYNTHESIZE(int, m_killTotal, KillTotal);
    
    //设置初始值，我的英雄将由相同传入
    virtual void defaultData();
private:
    LQCardInfoData* getCardCfgdata();

protected:
    int m_passiveSkillID;

};

typedef vector<LQHeroUserData*> LQHeroUserDataVector;
#endif