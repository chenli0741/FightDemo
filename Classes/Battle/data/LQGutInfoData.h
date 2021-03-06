//
//  LQGutInfoData.h
//  SuperWarriors
//
//  剧情数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQGUTINFODATA_H_
#define __LQGUTINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQGutInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQGutInfoData)
	
public:
    LQGutInfoData();
    virtual ~LQGutInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(std::string, m_playText, PlayText );	//不同的角色，剧情ID不同，ID尾数1－8分别对应同尾数的英雄，尾数9表示通用剧情。\r\n类型 0 普通信息 1 警告 2 对话 3 自定义图片对话\r\n文本\r\n卡牌ID(主角ID为41009999）\r\n对话方向：0 无  1 左 2 右
    LQ_SYNTHESIZE_READONLY(std::string, m_triggerEvent, TriggerEvent );	//剧情触发动作
    //返回类字符串
    static const char* getAliasName();
};

#endif
