//
//  LQZoneInfoData.cpp
//  SuperWarriors
//
//区域地图数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQZoneInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQZoneInfoData)

LQZoneInfoData::LQZoneInfoData():LQBaseData()
{

}
LQZoneInfoData::~LQZoneInfoData()
{
    //释放对象资源

};

const char* LQZoneInfoData::getAliasName()
{
    return "ZoneInfo";
};


