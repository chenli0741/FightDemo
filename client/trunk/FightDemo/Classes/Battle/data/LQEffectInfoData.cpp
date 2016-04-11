//
//  LQEffectInfoData.cpp
//  SuperWarriors
//
//特效数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQEffectInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQEffectInfoData)

LQEffectInfoData::LQEffectInfoData():LQBaseData()
{

}
LQEffectInfoData::~LQEffectInfoData()
{
    //释放对象资源

};

const char* LQEffectInfoData::getAliasName()
{
    return "EffectInfo";
};


