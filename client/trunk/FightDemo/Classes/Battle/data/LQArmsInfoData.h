//
//  LQArmsInfoData.h
//  SuperWarriors
//
//  兵种数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQARMSINFODATA_H_
#define __LQARMSINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQArmsInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQArmsInfoData)
	
public:
    LQArmsInfoData();
    virtual ~LQArmsInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(int, m_quality, Quality );	//1、白\r\n2、绿\r\n3、蓝\r\n4、紫\r\n5、橙\r\n6、金
    LQ_SYNTHESIZE_READONLY(int, m_starLevel, StarLevel );	//1、1星\r\n2、2星\r\n3、3星\r\n4、4星\r\n5、5星
    LQ_SYNTHESIZE_READONLY(int, m_grade, Grade );	//装备、坐骑、技能等的品级，代表成长潜质
    LQ_SYNTHESIZE_READONLY(bool, m_isSale, IsSale );	//0、不可出售\r\n1、可出售
    LQ_SYNTHESIZE_READONLY(int, m_price, Price );	//卖出价格
    LQ_SYNTHESIZE_READONLY(int, m_rebornNum, RebornNum );	//炼化获得魂玉、洗练石的数量
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Name, Name, NameID );	//兵种名称
    LQ_SYNTHESIZE_READONLY(int, m_unitArea, UnitArea );	//
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Description, Description, DescriptionID );	//描述文本
    LQ_SYNTHESIZE_READONLY(std::string, m_iconID, IconID );	//图标的资源ID
    LQ_SYNTHESIZE_READONLY(std::string, m_cardID, CardID );	//卡牌的资源ID
    LQ_SYNTHESIZE_READONLY(int, m_shapeID, ShapeID );	//序列帧的资源文件
    LQ_SYNTHESIZE_READONLY(std::string, m_flashID, FlashID );	//光影资源ID，用于普通攻击的刀光
    LQ_SYNTHESIZE_READONLY(std::string, m_smallIcon, SmallIcon );	//布阵和战前准备需要的小图标
    LQ_SYNTHESIZE_READONLY(std::string, m_critPic, CritPic );	//暴击使用的特效图片，如步兵是“奋起”、骑兵是“突击”、弓兵是“齐射”等等
    LQ_SYNTHESIZE_READONLY(std::string, m_critFont, CritFont );	//暴击字体
    LQ_SYNTHESIZE_READONLY(std::string, m_spPic, SpPic );	// 特殊效果图片
    LQ_SYNTHESIZE_READONLY(std::string, m_spFont, SpFont );	// 特殊效果字体
    LQ_SYNTHESIZE_READONLY(int, m_speed, Speed );	//兵种的移动速度（毫秒/格子)
    LQ_SYNTHESIZE_READONLY(int, m_view, View );	//作战单位视野（单位：格子）
    LQ_SYNTHESIZE_READONLY(int, m_attackType, AttackType );	//决定采取的攻击方式\r\n1：近战\r\n2：远程
    LQ_SYNTHESIZE_READONLY(int, m_attackSpeed, AttackSpeed );	//以毫秒为单位，兵种从发动攻击指令到攻击动作完成，每帧播放时间，万分之一秒
    LQ_SYNTHESIZE_READONLY(int, m_attackGap, AttackGap );	//攻击方式1攻击间隔时间，万分之一秒
    LQ_SYNTHESIZE_READONLY(int, m_attackDistMin, AttackDistMin );	//决定攻击方式1的有效范围
    LQ_SYNTHESIZE_READONLY(int, m_attackDistMax, AttackDistMax );	//决定攻击方式1的有效范围
    LQ_SYNTHESIZE_READONLY(int, m_aIType, AIType );	//决定兵种的AI类型
    LQ_SYNTHESIZE_READONLY(std::string, m_soldierRestrain, SoldierRestrain );	//属性,值;（值/10000)
    //返回类字符串
    static const char* getAliasName();
};

#endif
