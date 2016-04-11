//
//  LQZoneInfoData.h
//  SuperWarriors
//
//  区域地图数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQZONEINFODATA_H_
#define __LQZONEINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQZoneInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQZoneInfoData)
	
public:
    LQZoneInfoData();
    virtual ~LQZoneInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(int, m_fatherID, FatherID );	//该区域属于\r\n\r\n哪个大区域
    LQ_SYNTHESIZE_READONLY(int, m_useType, UseType );	//地图类别
    LQ_SYNTHESIZE_READONLY(int, m_areaType, AreaType );	//区域的类别\r\n\r\n1、世界地图区域\r\n\r\n2、子地图区域
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Point, Point, PointID );	//区域的备注说明
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Name, Name, NameID );	//区域名称
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Description, Description, DescriptionID );	//
    LQ_SYNTHESIZE_READONLY(int, m_levelDifficulty, LevelDifficulty );	//1、普通\r\n2、精英\r\n
    LQ_SYNTHESIZE_READONLY(int, m_onetime, Onetime );	//1.一次性副本
    LQ_SYNTHESIZE_READONLY(int, m_unlockAera, UnlockAera );	//通关后\r\n\r\n解锁的下一区域ID\r\n\r\n默认开启第一条记录
    LQ_SYNTHESIZE_READONLY(int, m_deblockinglv, Deblockinglv );	//解锁本章节等级
    LQ_SYNTHESIZE_READONLY(int, m_unlockChapter, UnlockChapter );	//通关后解锁的下一章节ID
    LQ_SYNTHESIZE_READONLY(int, m_battleMapType, BattleMapType );	//战斗地图类别\r\n1、任务地图\r\n2、PVE活动地图\r\n3、PVP活动地图
    LQ_SYNTHESIZE_READONLY(std::string, m_chapterPicture, ChapterPicture );	//用于章节/关卡选择界面
    LQ_SYNTHESIZE_READONLY(std::string, m_areaBg, AreaBg );	//用于区域的背景图片
    LQ_SYNTHESIZE_READONLY(std::string, m_roadMap, RoadMap );	//用于区域上的路线
    LQ_SYNTHESIZE_READONLY(std::string, m_battleCity, BattleCity );	//用于每章地图上关卡点的图标
    LQ_SYNTHESIZE_READONLY(std::string, m_iconCoords, IconCoords );	//用于城池在本地图上的坐标显示，格式为x,y
    LQ_SYNTHESIZE_READONLY(std::string, m_battlaMap, BattlaMap );	//战斗场景的背景地图
    LQ_SYNTHESIZE_READONLY(int, m_smokeEffect, SmokeEffect );	//烟尘特效ID
    LQ_SYNTHESIZE_READONLY(int, m_physicalConsume, PhysicalConsume );	//打一次的体力消耗
    LQ_SYNTHESIZE_READONLY(int, m_timeLimited, TimeLimited );	//每天挑战次数
    LQ_SYNTHESIZE_READONLY(int, m_battleMonster, BattleMonster );	//城池中的怪物组ID
    LQ_SYNTHESIZE_READONLY(std::string, m_visiableAward, VisiableAward );	//关卡信息页面可看到的关卡掉落物品
    LQ_SYNTHESIZE_READONLY(std::string, m_oddsAward, OddsAward );	//char:300
    LQ_SYNTHESIZE_READONLY(int, m_moneyAward, MoneyAward );	//通关后固定的金钱奖励
    LQ_SYNTHESIZE_READONLY(int, m_soulAward, SoulAward );	//通关后固定的将魂奖励
    LQ_SYNTHESIZE_READONLY(std::string, m_biographies, Biographies );	//通关该关卡开放的列传
    LQ_SYNTHESIZE_READONLY(int, m_readyStoryID, ReadyStoryID );	//在章节开始的地图上可能触发的剧情
    LQ_SYNTHESIZE_READONLY(int, m_startStoryID, StartStoryID );	//场景开始可能触发的剧情ID\r\n可以为空
    LQ_SYNTHESIZE_READONLY(int, m_endStoryID, EndStoryID );	//场景结束可能触发的剧情ID\r\n可以为空
    LQ_SYNTHESIZE_READONLY(std::string, m_triggerStoryID, TriggerStoryID );	//触发剧情\r\n格式（类型,剧情ID,值）\r\n类型：1 怒气满\r\n2 血值\r\n3 时间\r\n4 死亡
    LQ_SYNTHESIZE_READONLY(std::string, m_actStoryID, ActStoryID );	//触发剧情的演员格式（类型，演员D）
    LQ_SYNTHESIZE_READONLY(std::string, m_wav1, Wav1 );	//场景音乐
    LQ_SYNTHESIZE_READONLY(std::string, m_wav2, Wav2 );	//环境音效
    LQ_SYNTHESIZE_READONLY(std::string, m_completecopy, Completecopy );	//完成副本图标
    LQ_SYNTHESIZE_READONLY(std::string, m_fightPic, FightPic );	//战前准备教程图片（不填表示不出现教程）
    LQ_SYNTHESIZE_READONLY(int, m_fightTextID, FightTextID );	//战前准备教程文字\r\n（不填表示没有小女孩）
    LQ_SYNTHESIZE_READONLY(std::string, m_oneTimeAward, OneTimeAward );	//char:300
    //返回类字符串
    static const char* getAliasName();
};

#endif
