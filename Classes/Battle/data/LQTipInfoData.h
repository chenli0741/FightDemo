//
//  LQTipInfoData.h
//  SuperWarriors
//
//  Tips属性数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQTIPINFODATA_H_
#define __LQTIPINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQTipInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQTipInfoData)
	
public:
    LQTipInfoData();
    virtual ~LQTipInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(std::string, m_word, Word );	//如果存在替代字符：/s\r\n例如：%1打败了%2，赢得了奖品%3
    LQ_SYNTHESIZE_READONLY(int, m_isPlist, IsPlist );	//Plist
    //返回类字符串
    static const char* getAliasName();
};

#endif
