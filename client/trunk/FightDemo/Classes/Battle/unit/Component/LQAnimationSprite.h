//
//  LQAnimationSprite.h
//  LostEmpire
//  Buff动画
//  Created by chenli on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#ifndef __LQAnimationSprite_H__
#define __LQAnimationSprite_H__

#include "cocos2d.h"
#include "LQData.h"

USING_NS_CC;

using namespace std;

LQ_NAMESPACE

typedef enum
{
    kqBuffShowType_None = 0,      //无回调
    //buff效果
    kqBuffShowType_Buff_1 = 1,    //1.序列帧，不分方向
    kqBuffShowType_Buff_2 = 2,    //2.序列帧，有方向
    kqBuffShowType_Buff_3 = 3,    //3.人物叠加颜色，闪烁
    kqBuffShowType_Buff_4 = 4,    //4.图片，360°旋转
    
    KkqBuffShowType_Smoke = 10    //烟尘
    
} KLQBuffShowType;

class LQAnimationSprite : public Sprite
{
protected:
    Sprite* m_sp;
    LQAnimationInfoData* m_data;
    float m_ownscale;
    int m_imageFileID;
    int m_nextAnimation;
    LQAnimationInfoData* m_nextAnimationData;
    float m_frameInterval;  //时间，单位：毫秒
    int m_frameNum;     //帧数
    int m_repeatTime;   //“-1”代表Buff，不断重复直到Buff结束
    int m_frameHit;	//攻击帧数
    int m_frameReady;	//从单个特效单位播放开始，到显示伤害数据的帧数
    int m_frameEnd;	//从显示伤害数据开始，到单个特效播放结束的帧数
    float m_scaleFactor;	//特效资源的缩放系数，千分之
    Color3B m_shadeColor;
    
    float m_time;
    bool islaoadingRes;  //加载资源
    
    CC_SYNTHESIZE_READONLY(KLQBuffShowType, m_showType, ShowType);//特效的展示形式
    CC_SYNTHESIZE_READONLY(Point, m_buffAnchPosition, SkillAnchPosition );	//特效位置
    CC_SYNTHESIZE(float, m_allTime, AllTime); //整个动作的时间
    CC_SYNTHESIZE_READONLY(int, m_frameDelay, FrameDelay );	//特效位置
    
    CC_SYNTHESIZE_READONLY(int, m_hangingPoint, HangingPoint );	//特效的挂点位置(身体),1代表上,2代表中,3代表下,4代表底
 
    //创建系列帧动画
    ActionInterval* createAnimation();

public:
    LQAnimationSprite();
    ~LQAnimationSprite();
    
    static LQAnimationSprite* createWithId(int buffid);
    static LQAnimationSprite* createWithData(LQAnimationInfoData* data);
    bool initWithId(int buffid);
    bool initWithData(LQAnimationInfoData* data);
    
//    void setDirction(LQUnitDirectionType direction);//buff状态改变方向
    
//    void update(float dt);
    
    //主要装载资源--缓存的资源
    void onLoadResource();
    //主要用于回收资源--缓存的资源
    void onFreeResource();
    //
    virtual void play();
//    void play1();
};

LQ_NAMESPACE_END
#endif