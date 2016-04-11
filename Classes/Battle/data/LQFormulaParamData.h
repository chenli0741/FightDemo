//
//  LQFormulaParamData.h
//  SuperWarriors
//
//  公式参数表数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQFORMULAPARAMDATA_H_
#define __LQFORMULAPARAMDATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQFormulaParamData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQFormulaParamData)
	
public:
    LQFormulaParamData();
    virtual ~LQFormulaParamData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(int, m_belongTo, BelongTo );	//属于哪个公式
    LQ_SYNTHESIZE_READONLY(std::string, m_parameter, Parameter );	//在公式中代表哪个参数
    LQ_SYNTHESIZE_READONLY(int, m_factor, Factor );	//万分之
    //返回类字符串
    static const char* getAliasName();
};

#endif
