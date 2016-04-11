//
//  LQTFormatinfoData.h
//  SuperWarriors
//
//  数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQTFORMATINFODATA_H_
#define __LQTFORMATINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQTFormatinfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQTFormatinfoData)
	
public:
    LQTFormatinfoData();
    virtual ~LQTFormatinfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(std::string, m_name, Name );	//名称
    LQ_SYNTHESIZE_READONLY(int, m_used, Used );	//启用
    LQ_SYNTHESIZE_READONLY(int, m_idx, Idx );	//敌我（1我 2 敌）
    LQ_SYNTHESIZE_READONLY(int, m_zoneId, ZoneId );	//地图
    LQ_SYNTHESIZE_READONLY(std::string, m_victoryCond, VictoryCond );	//1.特定NPC被击溃\r\n2.任意我军部队被击溃\r\n3.我军总生命降低到Y%或更少\r\n4.任意我军部队生命降低到Y%或更少\r\n5.开战X秒后未能达成胜利条件（相当于调整单场战斗的时间上限）\r\n6.在N秒后，只要我军仍有部队存活，则战斗胜利。（而非通常的失败）\r\n10.将敌方特定NPC的血量降低到X%或更少\r\n//”1,2,0” [类型,NPC索引,哪方] ”3,2000” [类型,Y万分] ”4,2000” [类型,Y万分] ”5,50” [类型,秒] ”10,2,2000” [类型,NPC索引,X万分]\r\n////1.特
    LQ_SYNTHESIZE_READONLY(int, m_iDX1, IDX1 );	//位置1
    LQ_SYNTHESIZE_READONLY(int, m_iDX2, IDX2 );	//位置2
    LQ_SYNTHESIZE_READONLY(int, m_iDX3, IDX3 );	//位置3
    LQ_SYNTHESIZE_READONLY(int, m_iDX4, IDX4 );	//位置4
    LQ_SYNTHESIZE_READONLY(int, m_iDX5, IDX5 );	//位置5
    LQ_SYNTHESIZE_READONLY(int, m_iDX6, IDX6 );	//位置6
    LQ_SYNTHESIZE_READONLY(int, m_iDX7, IDX7 );	//位置7
    LQ_SYNTHESIZE_READONLY(int, m_iDX8, IDX8 );	//位置8
    LQ_SYNTHESIZE_READONLY(int, m_iDX9, IDX9 );	//位置9
    LQ_SYNTHESIZE_READONLY(int, m_iDX10, IDX10 );	//位置10
    LQ_SYNTHESIZE_READONLY(int, m_iDX11, IDX11 );	//位置11
    LQ_SYNTHESIZE_READONLY(int, m_iDX12, IDX12 );	//位置12
    LQ_SYNTHESIZE_READONLY(int, m_iDX13, IDX13 );	//位置13
    LQ_SYNTHESIZE_READONLY(int, m_iDX14, IDX14 );	//位置14
    LQ_SYNTHESIZE_READONLY(int, m_iDX15, IDX15 );	//位置15
    //返回类字符串
    static const char* getAliasName();
};

#endif
