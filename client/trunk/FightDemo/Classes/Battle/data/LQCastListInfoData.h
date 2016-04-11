//
//  LQCastListInfoData.h
//  SuperWarriors
//
//  演员表数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQCASTLISTINFODATA_H_
#define __LQCASTLISTINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQCastListInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQCastListInfoData)
	
public:
    LQCastListInfoData();
    virtual ~LQCastListInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Name, Name, NameID );	//演员名称
    LQ_SYNTHESIZE_READONLY(std::string, m_cardID, CardID );	//演员形象
    //返回类字符串
    static const char* getAliasName();
};

#endif
