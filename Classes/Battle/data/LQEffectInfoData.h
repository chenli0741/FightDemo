//
//  LQEffectInfoData.h
//  SuperWarriors
//
//  特效数据文件
//  Created by LequWorld Studio on 2016
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#ifndef __LQEFFECTINFODATA_H_
#define __LQEFFECTINFODATA_H_
	
#include "LQBaseData.h"
	
using namespace cocos2d;
	
//
class LQEffectInfoData: public LQBaseData
{
protected:
	DECLARE_CLASS(LQEffectInfoData)
	
public:
    LQEffectInfoData();
    virtual ~LQEffectInfoData(void);
	
    /** 数据值 */
    LQ_SYNTHESIZE_READONLY(std::string, m_effectText, EffectText );	//在头像上飘\r\nat攻击\r\ndf防御\r\nmv移动\r\ncr暴击\r\nts统率\r\nwy武勇\r\nzl智力\r\nup上升\r\ndown下降
    LQ_SYNTHESIZE_READONLY(int, m_missileType, MissileType );	//飞行类型\r\n0、直线\r\n1、弓箭\r\n2、投石\r\n3、标枪\r\n4、法术\r\n7、单方向直线\r\n8、八发行直线
    LQ_SYNTHESIZE_READONLY(int, m_missileEffect, MissileEffect );	//飞行特效资源
    LQ_SYNTHESIZE_READONLY(int, m_hitType, HitType );	//命中特效类型\r\n1 点命中\r\n2 圆命中\r\n7 单方向直线\r\n8 八发行直线
    LQ_SYNTHESIZE_READONLY(int, m_hitEffect, HitEffect );	//命中特效\r\n（这里同时决定了播放什么声音）
    LQ_SYNTHESIZE_READONLY(int, m_dominantAttribute, DominantAttribute );	//技能由何种属性支配，属性分类见分类代码说明，技能的输出判定与对应属性差有关\r\n（12武勇、13智力、14统率）
    LQ_SYNTHESIZE_READONLY(int, m_effectType, EffectType );	//1、瞬间加值或减值\r\n2、Buff状态\r\n3、清空DeBuff和Dot\r\n4、策反\r\n5、援兵？
    LQ_SYNTHESIZE_READONLY(int, m_buffSubType, BuffSubType );	//1、属性加值\r\n2、属性减值\r\n3、加速\r\n4、减速\r\n5、间隔一定时间加值\r\n6、间隔一定时间减值\r\n7、禁止移动（眩晕）\r\n8、禁止暴击\r\n9、禁止技能\r\n10、无视防御\r\n11、无视攻击\r\n12、禁锢(冰冻等)\r\n13、护盾\r\n14、白刃\r\n15、间隔一定时间加变值\r\n16、间隔一定时间减变值
    LQ_SYNTHESIZE_READONLY(int, m_buffFlag, BuffFlag );	//标志相同Buff\r\n（BUFF大类，两个数不一样则叠加；两个数一样，则比较Priority，数大的盖掉数小的；如果Priority相同，则后一个盖掉前一个）
    LQ_SYNTHESIZE_READONLY(int, m_priority, Priority );	//Buff等级，相同或更大数字的优先级顶替之前的小优先级Buff，小优先级Buff不能顶替大优先级的Buff\r\n\r\n百位数代号： \r\n1、全兵暴击\r\n2、武勇\r\n3、智力\r\n4、统率\r\n5、生命\r\n6、保留\r\n7、攻击\r\n8、防御                                                   
    LQ_SYNTHESIZE_READONLY(int, m_squadStatusID, SquadStatusID );	//军团状态ID：\r\n显示在军团旗帜出的状态图标，可以显示多个
    LQ_SYNTHESIZE_READONLY(int, m_aniDisplayType, AniDisplayType );	//1、动画（全军）\r\n2、图标（头顶）\r\n3、图片（身上）\r\n4、特效（武将）
    LQ_SYNTHESIZE_READONLY(int, m_animationID, AnimationID );	//（BUFF）\r\n特效ID
    LQ_SYNTHESIZE_READONLY(int, m_animationDef, AnimationDef );	//特效动作描述\r\n1:静止\r\n2：旋转呼吸
    LQ_SYNTHESIZE_READONLY(int, m_atkTimes, AtkTimes );	//攻击输出次数，默认为一次
    LQ_SYNTHESIZE_READONLY(int, m_splashArea, SplashArea );	//溅射范围
    LQ_SYNTHESIZE_READONLY(std::string, m_sPFMarker, SPFMarker );	//定义1号公式的X为可选的属性代码。拼串，0代表施法者，1代表目标。\r\n（例：\r\n1,15，代表X取目标的15）
    LQ_SYNTHESIZE_READONLY(int, m_formulaType, FormulaType );	//详见后页公式类型表
    LQ_SYNTHESIZE_READONLY(int, m_sourceType, SourceType );	//源数值取于何种属性，见分类代码说明\r\n(填写102时，3、4号公式中X取值恒定为1）
    LQ_SYNTHESIZE_READONLY(int, m_outputType, OutputType );	//输出数值作用于何种属性，见分类代码说明\r\n(17攻击、18防御）
    LQ_SYNTHESIZE_READONLY(int, m_effectBase, EffectBase );	//输出基值，输出下限校正系数，最小也要放大这么多倍,/10000
    LQ_SYNTHESIZE_READONLY(int, m_maxLimit, MaxLimit );	//输出上限校正系数，最大不能放大这么多倍,/10000
    LQ_SYNTHESIZE_READONLY(int, m_effectFactor, EffectFactor );	//用于支配属性对输出的叠加计算，/10000\r\n（如果以生命为基数恢复生命，则这里只能填写个位数。）\r\n（3、4号公式中这里如果智力100，则计算值就认为是乘100）
    LQ_SYNTHESIZE_READONLY(int, m_buffValue, BuffValue );	//Buff、Debuff、Dot对属性改变的基值或百分比，/10000\r\n（对移动速度而言，100*10000的意思是速度提高100%）
    LQ_SYNTHESIZE_READONLY(int, m_buffTime, BuffTime );	//Buff·Debuff和Dot的持续时间基值\r\n（万分之一秒）
    LQ_SYNTHESIZE_READONLY(int, m_dotTime, DotTime );	//Dot的间隔时间（万分之一秒）
    LQ_SYNTHESIZE_READONLY(int, m_isEffective, IsEffective );	//是否存在特效\r\n=0
    LQ_SYNTHESIZE_READONLY(int, m_valueb, Valueb );	//仅对公式1,2,7有效\r\n(值/100）
    LQ_SYNTHESIZE_READONLY(int, m_valueC, ValueC );	//公式为+XC，X是武将的等级\r\n(值/100）
    LQ_SYNTHESIZE_READONLY(int, m_suckBlood, SuckBlood );	//吸血值
    LQ_SYNTHESIZE_READONLY(int, m_skillCrit, SkillCrit );	//技能是否会暴击，0或不填为不暴击，填1为可能暴击
    LQ_SYNTHESIZE_READONLY(int, m_numFacter, NumFacter );	//在场部队数量对公式效果的影响\r\n公式结果*facter\r\n（不填则默认为无影响）
    LQ_SYNTHESIZE_READONLY(std::string, m_flashID, FlashID );	//光影资源ID，用于普通攻击的刀光
    LQ_SYNTHESIZE_READONLY(int, m_scaleFactor, ScaleFactor );	//特效资源的缩放系数，千分之
    LQ_SYNTHESIZE_READONLY(int, m_timeFacter, TimeFacter );	//Debuff/buff类效果的额外持续时间系数。值/10000\r\n（注意：在使用buff 6号公式时，取的表中属性值是填表值，面板智力70的实际值是7000。因此6号公式中，这里通常填写个位数即可。\r\ndebuff5号公式由于求出的属性比值是个位数，因此这里通常填写5位数）
    LQ_SYNTHESIZE_READONLY(int, m_timelimit, Timelimit );	//时间调整值的上限，最多不能超过这么多秒。值/10000
    LQ_SYNTHESIZE_READONLY(int, m_tFType, TFType );	//使用的时间公式类型\r\n6为buff公式\r\n5为debuff公式\r\n不填默认为6
    //返回类字符串
    static const char* getAliasName();
};

#endif
