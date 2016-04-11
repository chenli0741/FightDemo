//
//  LQCardInfoData.h
//  SuperWarriors
//
//  卡牌数据数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQCARDINFODATA_H_
#define __LQCARDINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQCardInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQCardInfoData)
	
public:
    LQCardInfoData();
    virtual ~LQCardInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Name, Name, NameID );	//卡牌名称
    LQ_SYNTHESIZE_READONLY(int, m_charactorID, CharactorID );	//角色的ID，用于判定不同名称、但其实是同一人物的武将。（例如马超与西凉锦马超）
    LQ_SYNTHESIZE_READONLY(std::string, m_shortName, ShortName );	//短名称
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Description, Description, DescriptionID );	//卡牌介绍的文本内容
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Intro, Intro, IntroID );	//介绍文本
    LQ_SYNTHESIZE_READONLY(int, m_shapeID, ShapeID );	//卡牌将领的外形ID
    LQ_SYNTHESIZE_READONLY(std::string, m_headID, HeadID );	//卡牌将领的头像ID
    LQ_SYNTHESIZE_READONLY(std::string, m_cardID, CardID );	//卡牌将领的卡牌ID
    LQ_SYNTHESIZE_READONLY(int, m_influenceID, InfluenceID );	//1、魏\r\n2、蜀\r\n3、吴\r\n4、群
    LQ_SYNTHESIZE_READONLY(int, m_quality, Quality );	//卡牌将领的品质
    LQ_SYNTHESIZE_READONLY(int, m_starLevel, StarLevel );	//星级
    LQ_SYNTHESIZE_READONLY(int, m_maxStar, MaxStar );	//卡牌的星级上限
    LQ_SYNTHESIZE_READONLY(int, m_grade, Grade );	//D:7500\r\nC:7800\r\nB-:8200\r\nB+:8600\r\nA-:9000\r\nA+:9500\r\nS:10000\r\n/10000
    LQ_SYNTHESIZE_READONLY(int, m_levelDefault, LevelDefault );	//初始化等级，默认为1
    LQ_SYNTHESIZE_READONLY(std::string, m_mainProperty, MainProperty );	//决定兵种的主要显示属性，兵种强化时也只针对此主加成属性进行强化。字串拼接，类别见分类说明表
    LQ_SYNTHESIZE_READONLY(int, m_battlePower, BattlePower );	//抽出的1级武将卡牌以及关卡敌人的战斗力
    LQ_SYNTHESIZE_READONLY(int, m_armsType, ArmsType );	//军团兵种\r\n1、步兵\r\n2、远程\r\n3、骑兵\r\n4、剑兵\r\n5、机械
    LQ_SYNTHESIZE_READONLY(int, m_occupation, Occupation );	//武将职业\r\n1、普通\r\n2、策士
    LQ_SYNTHESIZE_READONLY(std::string, m_heroDefAtr, HeroDefAtr );	//属性类型,值;(值/100\r\n武将生命，武将攻击，武将防御)
    LQ_SYNTHESIZE_READONLY(std::string, m_defAtr, DefAtr );	//属性类型,值;(值/100，武智统攻防血，伤害加深，伤害减免，破甲，固甲)
    LQ_SYNTHESIZE_READONLY(std::string, m_defGrow, DefGrow );	//生命增量，攻击增量，防御增量
    LQ_SYNTHESIZE_READONLY(std::string, m_starPower3, StarPower3 );	//A+3星吞卡时的固定成长
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
    LQ_SYNTHESIZE_READONLY(int, m_spatkFacter, SpatkFacter );	//冲锋、迎击的基础伤害（无视对手防御力）\r\n（值/10000）
    LQ_SYNTHESIZE_READONLY(int, m_type, Type );	//武将类型：1枪、2弓、3骑\r\n4孟获（紫，大象）\r\n5祝融（紫，投掷）\r\n6张飞、华雄、孙坚（紫，刀盾）
    LQ_SYNTHESIZE_READONLY(std::string, m_starSkillID, StarSkillID );	//星级,技能ID;星级,技能ID;\r\n
    //返回类字符串
    static const char* getAliasName();
};

#endif
