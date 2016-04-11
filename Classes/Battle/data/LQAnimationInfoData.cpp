//
//  LQAnimationInfoData.cpp
//  SuperWarriors
//
//特效动画数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQAnimationInfoData.h"

using namespace cocos2d;

IMPLEMENT_CLASS(LQAnimationInfoData)

LQAnimationInfoData::LQAnimationInfoData():LQBaseData()
{

}
LQAnimationInfoData::~LQAnimationInfoData()
{
    //释放对象资源

};

const char* LQAnimationInfoData::getAliasName()
{
    return "AnimationInfo";
};


