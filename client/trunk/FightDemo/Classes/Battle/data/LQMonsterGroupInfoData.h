//
//  LQMonsterGroupInfoData.h
//  SuperWarriors
//
//  怪物组表数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQMONSTERGROUPINFODATA_H_
#define __LQMONSTERGROUPINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQMonsterGroupInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQMonsterGroupInfoData)
	
public:
    LQMonsterGroupInfoData();
    virtual ~LQMonsterGroupInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(int, m_picID, PicID );	//关卡信息中可以看到的头像
    LQ_SYNTHESIZE_READONLY(int, m_useType, UseType );	//1、关卡1-4章\r\n2、关卡5-8章\r\n3、关卡9-12章\r\n4、关卡13-16章\r\n5、关卡17-20章\r\n11/试炼关\r\n12/良驹\r\n13/列传\r\n14/南蛮\r\n15/征战
    LQ_SYNTHESIZE_READONLY(int, m_groupLevel, GroupLevel );	//怪物组等级
    LQ_SYNTHESIZE_READONLY(int, m_capacity, Capacity );	//怪物组战斗力
    LQ_SYNTHESIZE_READONLY(std::string, m_victoryCond, VictoryCond );	//1.特定NPC被击溃\r\n2.任意我军部队被击溃\r\n3.我军总生命降低到Y%或更少\r\n4.任意我军部队生命降低到Y%或更少\r\n5.开战X秒后未能达成胜利条件（相当于调整单场战斗的时间上限）\r\n6.在特定秒后，只要我军仍有部队存活，则战斗胜利。（而非通常的失败）\r\n10.将敌方特定NPC的血量降低到X%或更少\r\n//”1,2,0” [类型,NPC索引,哪方] ;0是我方；1是敌方。\r\n”3,2000” [类型,Y万分] ”4,2000” [类型,Y万分] ”5,50” [类型,秒] ”6,50”[类型，秒]”10,2,2000” [类型,NPC索引,X万分]
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_CondDesc, CondDesc, CondDescID );	//胜利条件描述
    LQ_SYNTHESIZE_READONLY(std::string, m_formatTip, FormatTip );	//阵型提示\r\n阵位，类型；阵位，类型\r\n类型：1步兵、2远程、3骑兵
    LQ_SYNTHESIZE_READONLY(int, m_type, Type );	//1、普通小怪\r\n2、精英怪物\r\n3、Boss\r\n4、强力Boss
    LQ_SYNTHESIZE_READONLY(int, m_monster1, Monster1 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster2, Monster2 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster3, Monster3 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster4, Monster4 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster5, Monster5 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster6, Monster6 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster7, Monster7 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster8, Monster8 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster9, Monster9 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster10, Monster10 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster11, Monster11 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster12, Monster12 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster13, Monster13 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster14, Monster14 );	//怪物ID
    LQ_SYNTHESIZE_READONLY(int, m_monster15, Monster15 );	//怪物ID
    //返回类字符串
    static const char* getAliasName();
};

#endif
