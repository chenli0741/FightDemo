//
//  LQSkillInfoData.h
//  SuperWarriors
//
//  技能数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQSKILLINFODATA_H_
#define __LQSKILLINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQSkillInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQSkillInfoData)
	
public:
    LQSkillInfoData();
    virtual ~LQSkillInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Name, Name, NameID );	//技能名称
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_Description, Description, DescriptionID );	//技能描述（战斗中展示，短）
    LQ_SYNTHESIZE_LANG_READONLY(std::string, m_InterfaceDes, InterfaceDes, InterfaceDesID );	//英雄界面展示的详细技能描述
    LQ_SYNTHESIZE_READONLY(std::string, m_typePic, TypePic );	//图标的资源ID
    LQ_SYNTHESIZE_READONLY(std::string, m_iconID, IconID );	//图标的资源ID
    LQ_SYNTHESIZE_READONLY(int, m_mutiEffectType, MutiEffectType );	//多效果单元处理\r\n0/不填、多回合释放\r\n1、 一回合释放
    LQ_SYNTHESIZE_READONLY(int, m_skillTime, SkillTime );	//动画持续时间\r\n（技能多个效果单元，完成时间不可控，这个控制本技能动画什么时候算结束，不填默认2秒）\r\n 單位：毫秒
    LQ_SYNTHESIZE_READONLY(int, m_objectType1, ObjectType1 );	//见目标对象ID表
    LQ_SYNTHESIZE_READONLY(int, m_targetRange, TargetRange );	//索敌范围\r\n（这里指的是目标周围范围）
    LQ_SYNTHESIZE_READONLY(int, m_effectUnit1, EffectUnit1 );	//效果单元索引
    LQ_SYNTHESIZE_READONLY(int, m_objectType2, ObjectType2 );	//见目标对象ID表\r\n
    LQ_SYNTHESIZE_READONLY(int, m_effectUnit2, EffectUnit2 );	//效果单元索引
    LQ_SYNTHESIZE_READONLY(int, m_objectType3, ObjectType3 );	//见目标对象ID表
    LQ_SYNTHESIZE_READONLY(int, m_effectUnit3, EffectUnit3 );	//效果单元索引
    LQ_SYNTHESIZE_READONLY(int, m_objectType4, ObjectType4 );	//见目标对象ID表
    LQ_SYNTHESIZE_READONLY(int, m_effectUnit4, EffectUnit4 );	//效果单元索引
    LQ_SYNTHESIZE_READONLY(int, m_releaseTime, ReleaseTime );	//释放时间（毫秒，\r\n1000为1秒）
    //返回类字符串
    static const char* getAliasName();
};

#endif
