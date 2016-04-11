//
//  LQArmsInfoData.cpp
//  SuperWarriors
//
//兵种数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQArmsInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQArmsInfoData)

LQArmsInfoData::LQArmsInfoData():LQBaseData()
{

}
LQArmsInfoData::~LQArmsInfoData()
{
    //释放对象资源

};

const char* LQArmsInfoData::getAliasName()
{
    return "ArmsInfo";
};


