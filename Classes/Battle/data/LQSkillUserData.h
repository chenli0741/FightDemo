//
//  LQSkillUserData.h
//  SuperWarriors
//
//  Created by lequdev1 on 14-7-14.
//
//

#ifndef __SuperWarriors__LQSkillUserData__
#define __SuperWarriors__LQSkillUserData__

#include "LQBaseUserData.h"
#include "LQDataCache.h"
#include "LQSkillInfoData.h"
#include "LQAnimationInfoData.h"
#include "LQEffectUserData.h"
#include "GameObj.h"


class LQArmsUserData;
class LQHeroUserData;

typedef enum
{
    klqSkillShowType_ActiveSkill = 1,   //主动技能
    klqSkillShowType_Passive = 2,      //被动技能
    klqSkillShowType_Tactician= 3      //策士技能
} KLQSkillType;

class LQSkillUserData:public LQBaseUserData
{
private:
    float m_time;
    
    bool _isDoSkillReady; //技能施放特效中段
    bool _isDoSkillEnd;   //技能施放特效结束
    bool _isDoHitReady; //技能命中特效中段
    bool _isDoHitEnd;   //技能命中特效结束，技能结束
    //技能特效
    //LQAnimationInfoData* m_animationCfgData;
public:
    LQSkillUserData(int resIDC, KLQSkillType skillType = klqSkillShowType_ActiveSkill);
    ~LQSkillUserData();
    
    virtual bool init();
    
    // 初始化数据,子类重写
    const char* getAliasName();
    
    LQSkillInfoData* getSkillCfgData();         // 返回技能配置数据
    //LQAnimationInfoData* getAnimationCfgData(); // 返回特效配置数据
    
    // 初始化数据,子类重写
    void putData();
    
    //
    void restore();
    
    CC_SYNTHESIZE_READONLY(int, m_mutiEffectType, MutiEffectType );	//多效果单元处理\r\n0 多回合释放\r\n1 一回合释放
    CC_SYNTHESIZE_READONLY(KLQSkillType, m_skillType, SkillType );	//1 主动技能\r\n2 变动技能
    
    CC_SYNTHESIZE_READONLY(std::string, m_name, Name );	//卡牌名称
    CC_SYNTHESIZE_READONLY(LQEffectUserDataVector*, m_effectDatas, EffectDatas ); //技能的效果
    CC_SYNTHESIZE_READONLY(LQEffectUserData*, m_mainEffectData, MainEffectData ); //技能的效果
    
    CC_SYNTHESIZE(bool, m_isDoPassive, IsDoPassive );	//被动技能已经触发
    //OLD
    //获取瞬时伤害
    int getPMPOValue(LQHeroUserData* heroData);
    //是否先手可放技能
    bool isFirstSkill(float time);
    
    CC_SYNTHESIZE_READONLY(int, m_duplication, Duplication );	//“-1”代表Buff，不断重复直到Buff结束
    CC_SYNTHESIZE_READONLY(int, m_radius, Radius );	//特效的播放半径
    CC_SYNTHESIZE_READONLY(int, m_count, Count );	//个数
    CC_SYNTHESIZE_READONLY(float, m_skillDistance, SkillDistance);//攻击范围
    
    //在update中计算技能的持续时间
    void update(float dt);
    
    CC_SYNTHESIZE(float, m_skillOutEndTime, SkillOutEndTime);  //用于表示施放特效结束的时间
    CC_SYNTHESIZE(float, m_hitOutputTime, HitOutputTime);      //用于命中特效的时间
    CC_SYNTHESIZE(float, m_skillLastTime, SkillLastTime);      //整个技能动作的时间
    
    
    CC_SYNTHESIZE(bool, m_isSkillEnd, IsSkillEnd);        //技能施放特效结束
    CC_SYNTHESIZE(bool, m_isHitReady, IsHitReady);        //技能命中特效中段
    CC_SYNTHESIZE(bool, m_isHitEnd, IsHitEnd);            //技能命中特效结束，技能结束
};

typedef vector<LQSkillUserData *> LQSkillUserDataVector;
    /*
    CC_SYNTHESIZE_READONLY(int, m_effectID, EffectID);//对应的特效ID
    CC_SYNTHESIZE_READONLY(bool, m_fireMode, FireMode );	//0、被动\r\n1、主动
    CC_SYNTHESIZE_READONLY(int, m_aOEType, AOEType );	//技能的作用范围\r\n1.玩家点选范围内友方\r\n2.玩家点选范围内敌方\r\n3.随机选择交战的友军\r\n4.随机选择交战的敌军\r\n5.施法者为中心友军\r\n6.施法者为中心敌军\r\n7.我方武将单体\r\n8.敌方武将单体
    CC_SYNTHESIZE_READONLY(std::string, m_description, Description );	//特效名称、描述
    CC_SYNTHESIZE_READONLY(int, m_hangingPoint, HangingPoint );	//特效的挂点位置,1代表头顶,2代表身上
    CC_SYNTHESIZE_READONLY(Point, m_targetPostion, TargetPostion );	//目标单位身上的特效挂点
    CC_SYNTHESIZE_READONLY(int, m_scaleFactor, ScaleFactor );	//特效资源的缩放系数，千分之
    CC_SYNTHESIZE_READONLY(int, m_radius, Radius );	//特效的播放半径
    CC_SYNTHESIZE_READONLY(int, m_duplication, Duplication );	//“-1”代表Buff，不断重复直到Buff结束
    CC_SYNTHESIZE_READONLY(int, m_frameInterval, FrameInterval );	//时间，单位：毫秒
    CC_SYNTHESIZE_READONLY(int, m_frameNum, FrameNum );	//帧数
    CC_SYNTHESIZE_READONLY(int, m_count, Count );	//个数
    CC_SYNTHESIZE_READONLY(std::string, m_position, Position );	//特效位置
    CC_SYNTHESIZE_READONLY(int, m_frameReady, FrameReady );	//从单个特效单位播放开始，到显示伤害数据的帧数
    CC_SYNTHESIZE_READONLY(int, m_frameEnd, FrameEnd );	//从显示伤害数据开始，到单个特效播放结束的帧数
    
    //瞬伤的参数
    
    CC_SYNTHESIZE_READONLY(bool, m_isPMPO, IsPMPO );	//是否有瞬间输出
    CC_SYNTHESIZE_READONLY(int, m_pMPOType, PMPOType );	//瞬间输出数值作用于何种属性，见分类代码说明
    CC_SYNTHESIZE_READONLY(int, m_pMPOFrom, PMPOFrom );	//决定技能瞬间输出的属性类型，见属性分类
    CC_SYNTHESIZE_READONLY(int, m_pMPOBase, PMPOBase );	//瞬间输出基值
    CC_SYNTHESIZE_READONLY(int, m_pMPOFactor, PMPOFactor );	//用于支配属性对瞬间输出的叠加计算，/10000
    
    CC_SYNTHESIZE_READONLY(bool, m_isBuff, IsBuff );
    CC_SYNTHESIZE_READONLY(int, m_buffID, BuffID );	//格式：状态参数中ID，状态参数中ID
    CC_SYNTHESIZE_READONLY(int, m_buffProbability, BuffProbability );	//Buff触发机率,/10000
    CC_SYNTHESIZE_READONLY(int, m_buffTime, BuffTime );	//Buff持续的时间，单位：毫秒
    CC_SYNTHESIZE_READONLY(int, m_buffInterval, BuffInterval );	//Buff生效的间隔时间，单位：毫秒，有的Buff不需要此时间，设为0
    CC_SYNTHESIZE_READONLY(int, m_buffFrom, BuffFrom );	//支配Buff的属性
    CC_SYNTHESIZE_READONLY(int, m_buffBase, BuffBase );	//Buff状态的基值
    CC_SYNTHESIZE_READONLY(int, m_buffFactor, BuffFactor );	//Buff状态中需要改变的属性参数
    
    
    CC_SYNTHESIZE(float, m_skillTime, SkillTime);           //整个技能的播放时间
    CC_SYNTHESIZE(float, m_currSkillReadyTime, CurrSkillReadyTime);//下一个到达ready时间
    CC_SYNTHESIZE(float, m_skillDelayTime,SkillDelayTime);  //从开始 技能播放的到现在的持续时间
    CC_SYNTHESIZE(float, m_skillReadyDelayTime,SkillReadyDelayTime);//从开始 技能播放的到现在的持续时间
    CC_SYNTHESIZE(float, m_currSkillIndex,CurrSkillIndex);//下一个到达ready时间的特效的index
    
    int m_pMPOValue;
    int getPMPOValue(LQHeroUserData* heroData);//获取技能瞬时输出值
    
    CC_SYNTHESIZE(float, m_skillDistance, SkillDistance);//攻击范围
    CC_SYNTHESIZE(LQAnimationInfoData*, m_animationData, AnimationData);//攻击范围
    
    //CC_SYNTHESIZE(LQBuffInfoData*, m_buffData, BuffData);//攻击范围
    
    //是否buff产生，如果返回值为0，则没有产生Buff
    CC_SYNTHESIZE(float, m_buffRevised, BuffRevised);//buff修正参数
    int getIsBufferHappened(LQHeroUserData* heroData,LQArmsUserData* targetData,LQGameObjType targetType = klqLQGameObjSoldier);//buff 的间隔伤害
    
};
*/


#endif /* defined(__game__LQSkillUserData__) */
