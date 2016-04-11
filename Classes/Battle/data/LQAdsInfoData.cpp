//
//  LQAdsInfoData.cpp
//  SuperWarriors
//
//Loading页面的文字和武将数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQAdsInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQAdsInfoData)

LQAdsInfoData::LQAdsInfoData():LQBaseData()
{

}
LQAdsInfoData::~LQAdsInfoData()
{
    //释放对象资源

};

const char* LQAdsInfoData::getAliasName()
{
    return "AdsInfo";
};


