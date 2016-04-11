//
//  LQMonsterInfoData.h
//  SuperWarriors
//
//  怪物数据数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQMONSTERINFODATA_H_
#define __LQMONSTERINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQMonsterInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQMonsterInfoData)
	
public:
    LQMonsterInfoData();
    virtual ~LQMonsterInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Name, Name, NameID );	//卡牌名称
    LQ_SYNTHESIZE_READONLY(std::string, m_shortName, ShortName );	//短名称
    LQ_SYNTHESIZE_READONLY(int, m_shapeID, ShapeID );	//卡牌将领的外形ID
    LQ_SYNTHESIZE_READONLY(std::string, m_headID, HeadID );	//卡牌将领的头像ID
    LQ_SYNTHESIZE_READONLY(std::string, m_cardID, CardID );	//卡牌将领的卡牌ID
    LQ_SYNTHESIZE_READONLY(int, m_useType, UseType );	//1、关卡1-4章\r\n2、关卡5-8章\r\n3、关卡9-12章\r\n4、关卡13-16章\r\n5、关卡17-20章\r\n11、经验关、良驹、13列传\r\n16、新征战用NPC
    LQ_SYNTHESIZE_READONLY(int, m_quality, Quality );	//卡牌将领的品质
    LQ_SYNTHESIZE_READONLY(int, m_starLevel, StarLevel );	//星级
    LQ_SYNTHESIZE_READONLY(int, m_grade, Grade );	//卡牌资质
    LQ_SYNTHESIZE_READONLY(int, m_levelDefault, LevelDefault );	//初始化等级，默认为1
    LQ_SYNTHESIZE_READONLY(int, m_battlePower, BattlePower );	//抽出的1级武将卡牌以及关卡敌人的战斗力
    LQ_SYNTHESIZE_READONLY(int, m_armsType, ArmsType );	//军团兵种\r\n1、枪兵\r\n2、远程\r\n3、骑兵\r\n4、剑兵\r\n5、机械
    LQ_SYNTHESIZE_READONLY(int, m_occupation, Occupation );	//武将职业\r\n1、普通\r\n2、策士
    LQ_SYNTHESIZE_READONLY(std::string, m_heroDefAtr, HeroDefAtr );	//属性类型,值;(值/100)
    LQ_SYNTHESIZE_READONLY(std::string, m_defAtr, DefAtr );	//属性类型,值;(值/100)
    LQ_SYNTHESIZE_READONLY(int, m_rageDefault, RageDefault );	//初始化怒气
    LQ_SYNTHESIZE_READONLY(int, m_rageMaxDefault, RageMaxDefault );	//初始化怒气上限
    LQ_SYNTHESIZE_READONLY(int, m_rageMin, RageMin );	//攻击时会产生的最小怒气值
    LQ_SYNTHESIZE_READONLY(int, m_rageBeAttack, RageBeAttack );	//被攻击怒气
    LQ_SYNTHESIZE_READONLY(int, m_rageAttack, RageAttack );	//攻击怒气
    LQ_SYNTHESIZE_READONLY(int, m_stuntEffect, StuntEffect );	//突出武将技能施放时的效果
    LQ_SYNTHESIZE_READONLY(int, m_activeSkillID, ActiveSkillID );	//主动技能
    LQ_SYNTHESIZE_READONLY(int, m_passiveSkill1ID, PassiveSkill1ID );	//被动技能1
    LQ_SYNTHESIZE_READONLY(int, m_passiveSkill2ID, PassiveSkill2ID );	//被动技能2
    LQ_SYNTHESIZE_READONLY(int, m_defaultSoldier, DefaultSoldier );	//初始化携带兵种
    LQ_SYNTHESIZE_READONLY(int, m_patrolSpeed, PatrolSpeed );	//巡逻移动速度（毫秒/格子)
    LQ_SYNTHESIZE_READONLY(int, m_speed, Speed );	//移动速度（毫秒/格子)
    LQ_SYNTHESIZE_READONLY(int, m_view, View );	//作战单位视野（单位：格子）
    LQ_SYNTHESIZE_READONLY(int, m_radius, Radius );	//军团最大半径（单位：格子）
    LQ_SYNTHESIZE_READONLY(int, m_attackType, AttackType );	//决定采取的攻击方式\r\n1：近战\r\n2：远程
    LQ_SYNTHESIZE_READONLY(int, m_attackSpeed, AttackSpeed );	//以毫秒为单位，兵种从发动攻击指令到攻击动作完成，每帧播放时间，万分之一秒
    LQ_SYNTHESIZE_READONLY(int, m_attackGap, AttackGap );	//攻击方式1攻击间隔时间，万分之一秒
    LQ_SYNTHESIZE_READONLY(int, m_attackDistMin, AttackDistMin );	//决定攻击方式1的有效范围
    LQ_SYNTHESIZE_READONLY(int, m_attackDistMax, AttackDistMax );	//决定攻击方式1的有效范围
    LQ_SYNTHESIZE_READONLY(int, m_aIType, AIType );	//决定AI类型
    LQ_SYNTHESIZE_READONLY(std::string, m_atkSound, AtkSound );	//%d表示通配数字号，两位数字
    LQ_SYNTHESIZE_READONLY(int, m_atkSNum, AtkSNum );	//攻击配音文件个数
    LQ_SYNTHESIZE_READONLY(std::string, m_dthSound, DthSound );	//%d表示通配数字号，两位数字
    LQ_SYNTHESIZE_READONLY(int, m_dthSNum, DthSNum );	//攻击配音文件个数
    LQ_SYNTHESIZE_READONLY(int, m_tacticianSkillID, TacticianSkillID );	//策士技能
    LQ_SYNTHESIZE_READONLY(std::string, m_soldierRestrain, SoldierRestrain );	//属性,值;（值/10000)\r\n49，骑兵压制；45,弓兵压制；41,步兵压制\r\n43压步伤；47压弓伤；51压骑伤
    LQ_SYNTHESIZE_READONLY(int, m_spatkFacter, SpatkFacter );	//冲锋、迎击的基础伤害（无视对手防御力）
    LQ_SYNTHESIZE_READONLY(std::string, m_starSkillID, StarSkillID );	//星级,技能ID;星级,技能ID;\r\n
    //返回类字符串
    static const char* getAliasName();
};

#endif
