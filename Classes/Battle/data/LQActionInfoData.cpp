//
//  LQActionInfoData.cpp
//  SuperWarriors
//
//动作时间数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQActionInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQActionInfoData)

LQActionInfoData::LQActionInfoData():LQBaseData()
{

}
LQActionInfoData::~LQActionInfoData()
{
    //释放对象资源

};

const char* LQActionInfoData::getAliasName()
{
    return "ActionInfo";
};


