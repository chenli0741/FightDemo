//
//  LQCastListInfoData.cpp
//  SuperWarriors
//
//演员表数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQCastListInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQCastListInfoData)

LQCastListInfoData::LQCastListInfoData():LQBaseData()
{

}
LQCastListInfoData::~LQCastListInfoData()
{
    //释放对象资源

};

const char* LQCastListInfoData::getAliasName()
{
    return "CastListInfo";
};


