//
//  LQAdsInfoData.h
//  SuperWarriors
//
//  Loading页面的文字和武将数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQADSINFODATA_H_
#define __LQADSINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQAdsInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQAdsInfoData)
	
public:
    LQAdsInfoData();
    virtual ~LQAdsInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(int, m_cardID, CardID );	//卡牌ID
    LQ_SYNTHESIZE_READONLY(std::string, m_cardIcon, CardIcon );	//卡牌形象
    LQ_SYNTHESIZE_READONLY(std::string, m_adWords, AdWords );	//广告词
    //返回类字符串
    static const char* getAliasName();
};

#endif
