//
//  LQSkillInfoData.cpp
//  SuperWarriors
//
//技能数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQSkillInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQSkillInfoData)

LQSkillInfoData::LQSkillInfoData():LQBaseData()
{

}
LQSkillInfoData::~LQSkillInfoData()
{
    //释放对象资源

};

const char* LQSkillInfoData::getAliasName()
{
    return "SkillInfo";
};


