//
//  LQCardInfoData.cpp
//  SuperWarriors
//
//卡牌数据数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQCardInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQCardInfoData)

LQCardInfoData::LQCardInfoData():LQBaseData()
{

}
LQCardInfoData::~LQCardInfoData()
{
    //释放对象资源

};

const char* LQCardInfoData::getAliasName()
{
    return "CardInfo";
};


