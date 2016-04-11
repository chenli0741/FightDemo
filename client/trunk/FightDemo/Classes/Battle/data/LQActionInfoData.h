//
//  LQActionInfoData.h
//  SuperWarriors
//
//  动作时间数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQACTIONINFODATA_H_
#define __LQACTIONINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQActionInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQActionInfoData)
	
public:
    LQActionInfoData();
    virtual ~LQActionInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(std::string, m_resName, ResName );	//卡牌将领的外型文件夹名称
    LQ_SYNTHESIZE_READONLY(int, m_unitType, UnitType );	//兵种的类别\r\n1、剑兵\r\n2、枪兵\r\n3、骑兵\r\n4、戟兵\r\n5、弩兵\r\n6、象兵\r\n7、骑射\r\n8、藤甲\r\n9、女兵
    LQ_SYNTHESIZE_READONLY(std::string, m_centralCoordinate, CentralCoordinate );	//图片的重心坐标，格式为(x,y)
    LQ_SYNTHESIZE_READONLY(int, m_imgWidth, ImgWidth );	//图片的宽度
    LQ_SYNTHESIZE_READONLY(int, m_imgHeight, ImgHeight );	//图片的高度
    LQ_SYNTHESIZE_READONLY(int, m_standBy, StandBy );	//待命动作序列
    LQ_SYNTHESIZE_READONLY(int, m_move, Move );	//移动动作序列
    LQ_SYNTHESIZE_READONLY(int, m_death, Death );	//死亡动作序列
    LQ_SYNTHESIZE_READONLY(int, m_melATKR, MelATKR );	//近战攻击准备动作序列，攻击准备完成后，即命中目标。－1表示无此动作
    LQ_SYNTHESIZE_READONLY(int, m_melATKF, MelATKF );	//近战攻击完成动作序列，攻击准备完成后，回复到再次可以准备攻击的待命状态的动作过程。－1表示无此动作
    LQ_SYNTHESIZE_READONLY(int, m_stATKT, StATKT );	//技能攻击重复次数
    LQ_SYNTHESIZE_READONLY(int, m_stATKR, StATKR );	//技能攻击准备动作序列，攻击准备完成后，即放出技能。－1表示无此动作
    LQ_SYNTHESIZE_READONLY(int, m_stATKN, StATKN );	//技能攻击动作序列总帧数；\r\nStATKT*StATKN
    LQ_SYNTHESIZE_READONLY(int, m_missileType, MissileType );	//箭矢类型：\r\n0、直线\r\n1、弓箭\r\n2、投石\r\n3、标枪\r\n4、法术\r\n7、单方向直线\r\n8、八发行直线\r\n纯近战兵种，填-1
    LQ_SYNTHESIZE_READONLY(std::string, m_missileRes, MissileRes );	//箭矢应用的资源文件，与模型资源绑定
    LQ_SYNTHESIZE_READONLY(std::string, m_missileResPostion, MissileResPostion );	//特效位置
    LQ_SYNTHESIZE_READONLY(int, m_hitEffect, HitEffect );	//命中爆点特效
    //返回类字符串
    static const char* getAliasName();
};

#endif
