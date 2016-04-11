//
//  LQBuffSprite.h
//  LostEmpire
//  Buff动画
//  Created by chenli on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#ifndef __LQBuffSprite_H__
#define __LQBuffSprite_H__

#include "cocos2d.h"
#include "LQData.h"
#include "LQAnimationSprite.h"

USING_NS_CC;

using namespace std;

//初始buff的图片以右为起始方向
static int Def_Buff_Rotate[8] = {90,180,-90,0,135,-135,45,-45};

LQ_NAMESPACE
class LQBuffSprite : public LQAnimationSprite
{
protected:
    int m_skillCount;
    unsigned int m_audioid;
    
    CC_SYNTHESIZE_READONLY(bool, m_isOutput, IsOutPut);          //技能输出
    CC_SYNTHESIZE_READONLY(bool, m_isHitOutput, IsHitOutPut);    //技能输出
    CC_SYNTHESIZE_READONLY(bool, m_isShowEnd, IsShowEnd);        //技能效果显示结束
    
    CC_SYNTHESIZE(float, m_skillHitTime, SkillHitTime);//用于表示瞬时输出的时间
    CC_SYNTHESIZE(float, m_skillOutputTime, SkillOutputTime);//用于表示瞬时输出的时间
    CC_SYNTHESIZE(int, m_heroLocalZOrder, HeroLocalZOrder);//英雄的层级
    CC_SYNTHESIZE_READONLY(float, m_skillAllOutputTime, SkillAllOutputTime);//用于表示瞬时输出的时间
    CC_SYNTHESIZE_READONLY(std::string, m_audio, Audio );	//对应音效
    
    void skillHitOutputCallBack();  //打击声音
    void skillOutputCallBack();     //伤害输出
public:
    LQBuffSprite();
    ~LQBuffSprite();
    
    static LQBuffSprite* createWithId(int buffid, float scale=1);
    static LQBuffSprite* createWithData(LQAnimationInfoData* data, float scale=1, KLQBuffShowType showType=kqBuffShowType_Buff_1);
    bool initWithId(int buffid, float scale);
    bool initWithData(LQAnimationInfoData* data,float scale,KLQBuffShowType showType=kqBuffShowType_Buff_1);
    
    void setDirction(LQUnitDirectionType direction);//buff状态改变方向
    
    void update(float dt);
    //
    void play();
    void play1(int dt=0);
    void playLine(int left);
    void playWithTimes(unsigned int times,Sequence* sequ=NULL);
};

LQ_NAMESPACE_END
#endif