//
//  LQFormulaParamData.cpp
//  SuperWarriors
//
//公式参数表数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQFormulaParamData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQFormulaParamData)

LQFormulaParamData::LQFormulaParamData():LQBaseData()
{

}
LQFormulaParamData::~LQFormulaParamData()
{
    //释放对象资源

};

const char* LQFormulaParamData::getAliasName()
{
    return "FormulaParam";
};


