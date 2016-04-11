//
//  LQMonsterGroupInfoData.cpp
//  SuperWarriors
//
//怪物组表数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQMonsterGroupInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQMonsterGroupInfoData)

LQMonsterGroupInfoData::LQMonsterGroupInfoData():LQBaseData()
{

}
LQMonsterGroupInfoData::~LQMonsterGroupInfoData()
{
    //释放对象资源

};

const char* LQMonsterGroupInfoData::getAliasName()
{
    return "MonsterGroupInfo";
};


