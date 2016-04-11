//
//  LQEffectSprite.h
//  LostEmpire
//  技能动画
//  Created by chenli on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#ifndef __LQEffectSprite_H__
#define __LQEffectSprite_H__

#include "cocos2d.h"
#include "LQStringExtend.h"
#include "LQData.h"

USING_NS_CC;
using namespace std;
#include "LQCombatUnitEnums.h"
#include "LQCombatUnit.h"


//1.静态图片
//2.序列帧，有方向
//3.人物叠加颜色，闪烁
//4.序列帧，不分方向
//5.图片，360°旋转

//初始buff的图片以右为起始方向
static int EffectRotate[8] = {90,180,-90,0,135,-135,45,-45};
#define LQSkillActionTagNumber 996 

LQ_NAMESPACE
class LQEffectSprite : public Sprite
{
private:
    Sprite* createSprite();
    //创建系列帧动画
    ActionInterval* createAnimation();
public:
    LQEffectSprite();
    ~LQEffectSprite();
    
    static LQEffectSprite* createSkillWithIDAndType(int skillID, LQCombatUnit* own, KLQSkillShowType skillShowtype = klqSkillShowType_None);
    //static LQEffectSprite* createWithData(int skillID, LQCombatUnit* own, KLQSkillShowType skillShowtype = klqSkillShowType_None);
    bool initWithIDAndType(int effectID, LQCombatUnit* own, KLQSkillShowType skillShowtype);
    bool initWithBuffID(int buffID);
    //bool initWithSkillID(int skillID);
    bool initWithEffectID(int effectID);
    
    CC_SYNTHESIZE(LQCombatUnit*, m_own, Own);
    CC_SYNTHESIZE_READONLY(int, m_imageFileID, ImageFileID);//特效的图片资源文件夹名
    CC_SYNTHESIZE(int, m_skillID, SkillID);
    CC_SYNTHESIZE_READONLY(KLQSkillShowType, m_skillShowType, SkillShowType);//特效的展示形式
    CC_SYNTHESIZE_READONLY(int, m_repeatTime,RepeatTime);
    
    CC_SYNTHESIZE(LQAnimationInfoData*, m_animationData, AnimationData);
    CC_SYNTHESIZE_READONLY(int , m_skillType, SkillType);//1 下， 2 上
    
    CC_SYNTHESIZE_READONLY(float, m_scaleFactor, ScaleFactor );	//特效资源的缩放系数，千分之
    
    CC_SYNTHESIZE_READONLY(float, m_frameInterval, FrameInterval );	//时间，单位：毫秒
    CC_SYNTHESIZE_READONLY(int, m_frameNum, FrameNum );	//帧数
    CC_SYNTHESIZE_READONLY(Point, m_skillAnchPosition, SkillAnchPosition );	//特效位置
    CC_SYNTHESIZE_READONLY(int, m_frameHit, FrameHit );	//攻击帧数
    CC_SYNTHESIZE_READONLY(int, m_frameReady, FrameReady );	//从单个特效单位播放开始，到显示伤害数据的帧数
    CC_SYNTHESIZE_READONLY(int, m_frameEnd, FrameEnd );	//从显示伤害数据开始，到单个特效播放结束的帧数
    CC_SYNTHESIZE_READONLY(int, m_radius, Radius );//释放半径
    CC_SYNTHESIZE_READONLY(int, m_skillCount, SkillCount );//释放个数
    
    CC_SYNTHESIZE(float, m_time, Time);
    CC_SYNTHESIZE(float, m_skillLastTime, SkillLastTime); //整个动作的时间
    CC_SYNTHESIZE(float, m_skillOutputTime, SkillOutputTime);//用于表示瞬时输出的时间
    CC_SYNTHESIZE(bool, m_isOutput, IsOutPut);          //技能输出
    CC_SYNTHESIZE(bool, m_isShowEnd, IsShowEnd);        //技能效果显示结束
    CC_SYNTHESIZE(int, m_isEnd, IsEnd);
    
    CC_SYNTHESIZE(float, m_speed, Speed);
    
    void setDirction(LQUnitDirectionType direction);//buff状态改变方向
    void setSpeed(int speed);//改变技能播放速度 1-正常 大于1 加速  小于1减速
    
    void play();
    void playBuffSkill5();  //m_skillShowType == klqSkillShowType_Buff_5
    
    void playSigle(Point desPoint,float scale);
    void skillEnd();//技能播放结束收尾
    
    void caculateTime();
    void updateSkill(float dt);
};



LQ_NAMESPACE_END
#endif