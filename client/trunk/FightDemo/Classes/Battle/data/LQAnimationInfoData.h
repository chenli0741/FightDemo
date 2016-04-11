//
//  LQAnimationInfoData.h
//  SuperWarriors
//
//  特效动画数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQANIMATIONINFODATA_H_
#define __LQANIMATIONINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQAnimationInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQAnimationInfoData)
	
public:
    LQAnimationInfoData();
    virtual ~LQAnimationInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(int, m_resID, ResID );	//特效资源ID
    LQ_SYNTHESIZE_READONLY(int, m_actionType, ActionType );	//特效类型\r\n（填7则无法连续播放）
    LQ_SYNTHESIZE_READONLY(int, m_hangingPoint, HangingPoint );	//特效的挂点位置(身体),1代表上,2代表中,3代表下,4代表底；同时代表层级
    LQ_SYNTHESIZE_READONLY(int, m_scaleFactor, ScaleFactor );	//特效资源的缩放系数，千分之
    LQ_SYNTHESIZE_READONLY(int, m_duplication, Duplication );	//“-1”代表Buff，不断重复直到Buff结束
    LQ_SYNTHESIZE_READONLY(int, m_frameInterval, FrameInterval );	//时间，单位：毫秒
    LQ_SYNTHESIZE_READONLY(std::string, m_position, Position );	//特效位置
    LQ_SYNTHESIZE_READONLY(int, m_frameDelay, FrameDelay );	//触发动画后，延迟多少帧开始播放动画
    LQ_SYNTHESIZE_READONLY(int, m_frameHit, FrameHit );	//攻击前奏帧数\r\n(开始播放声音的帧数）
    LQ_SYNTHESIZE_READONLY(int, m_frameReady, FrameReady );	//攻击帧数\r\n特效单位播放输出的帧数，也是显示伤害数据的帧数；并决定后续动画的播放时机
    LQ_SYNTHESIZE_READONLY(int, m_frameEnd, FrameEnd );	//单个特效播放结束的帧数\r\n（必须等同美术资源总帧数）
    LQ_SYNTHESIZE_READONLY(int, m_nextAnimation, NextAnimation );	//接下来的动作
    LQ_SYNTHESIZE_READONLY(int, m_breakType, BreakType );	//振动类型\r\n1 震屏\r\n2 红光\r\n3 持续声音
    LQ_SYNTHESIZE_READONLY(std::string, m_audio, Audio );	//对应音效
    LQ_SYNTHESIZE_READONLY(std::string, m_hitAudio, HitAudio );	//打击对应音效
    LQ_SYNTHESIZE_READONLY(int, m_opacity, Opacity );	//特效的透明度，值填写0~255，\r\n不填是255
    LQ_SYNTHESIZE_READONLY(std::string, m_shade, Shade );	//色差蒙层\r\n填写R,G,B值
    //返回类字符串
    static const char* getAliasName();
};

#endif
