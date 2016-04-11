//
//  LQMonsterInfoData.cpp
//  SuperWarriors
//
//怪物数据数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQMonsterInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQMonsterInfoData)

LQMonsterInfoData::LQMonsterInfoData():LQBaseData()
{

}
LQMonsterInfoData::~LQMonsterInfoData()
{
    //释放对象资源

};

const char* LQMonsterInfoData::getAliasName()
{
    return "MonsterInfo";
};


