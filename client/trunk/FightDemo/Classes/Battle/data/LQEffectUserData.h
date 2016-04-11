//
//  LQEffectUserData.h
//  SuperWarriors
//  效果单元数据
//  Created by lequdev1 on 14-7-15.
//
//

#ifndef __SuperWarriors__LQEffectUserData__
#define __SuperWarriors__LQEffectUserData__

#include "LQAnimationInfoData.h"
#include "LQEffectInfoData.h"
#include "LQSkillInfoData.h"
#include "LQBaseUserData.h"
#include "LQDataCache.h"
#include "GameObj.h"

class LQEffectUserData:public LQBaseUserData
{
private:
    //技能特效
    LQAnimationInfoData* m_animationCfgData;
    
    //1号公式的X为可选的属性代码 特殊规则
    int m_SPFMarker;     //取值方向
    int m_SPFMarkerAttr; //取值主属性
public:
    LQEffectUserData(int resIDC, int objectType);
    ~LQEffectUserData();
    // 初始化数据,子类重写
    const char* getAliasName();
    LQEffectInfoData*  getCfgdata();
    // 返回特效配置数据
    LQAnimationInfoData* getAnimationCfgData();
    
    // 初始化数据,子类重写
    void putData();
    
    //CC_SYNTHESIZE(GameObj *, m_hero, Hero );        //释放技能的武将数据
    CC_SYNTHESIZE(int, m_objectType, ObjectType );  //效果实施的对象类型
    CC_SYNTHESIZE(LQSkillInfoData*, m_skillInfo, SkillInfo );  //效果实施的对象类型
    
    CC_SYNTHESIZE_READONLY(int, m_missileType, MissileType );	//飞行类型\r\n0、直线\r\n1、弓箭\r\n2、投石\r\n3、标枪\r\n4、法术\r\n7、单方向直线\r\n8、八发行直线
    CC_SYNTHESIZE_READONLY(int, m_missileEffect, MissileEffect );	//飞行特效资源
    CC_SYNTHESIZE_READONLY(int, m_hitType, HitType );	//命中特效类型\r\n1 点命中\r\n2 圆命中\r\n7 单方向直线\r\n8 八发行直线
    CC_SYNTHESIZE_READONLY(int, m_hitEffect, HitEffect );	//命中特效
    
    CC_SYNTHESIZE_READONLY(int, m_effectType, EffectType );	//1、瞬间加值或减值\r\n2、Buff状态\r\n3、清空DeBuff和Dot\r\n4、策反\r\n5、援兵？
    CC_SYNTHESIZE_READONLY(int, m_formulaType, FormulaType );	//详见后页公式类型表
    
    CC_SYNTHESIZE_READONLY(KLQAnimationDisplayType, m_animationType, AnimationType );	//特效显示类型
    CC_SYNTHESIZE_READONLY(int, m_animationID, AnimationID );	//特效ID
    CC_SYNTHESIZE_READONLY(Point, m_skillPosition, SkillPosition );	//特效位置

    CC_SYNTHESIZE(float, m_outputTime, OutputTime );	//控制外面的伤害飘字时间
    
    
    CC_SYNTHESIZE_READONLY(int, m_hurtSum, HurtSum );  //效果单元伤血的总值，用来做类似吸血效果
    CC_SYNTHESIZE_READONLY(int, m_numFactor, NumFactor );  //数量因子
    
    //效果计算公式  1、普通伤害 2、攻击输出  xType <0 伤害 >0取属性
    //效果计算公式  7、Debuff的属性伤害计算
    void calculatorValue(GameObj* hero, GameObjectVector* targetObjects, LQAttackDataInterface* herodata, LQAttackDataInterface* squadata, int mainAttr, int times = 0, float outtime = 0); //float bufftime=0,
    // value 当值为0时是取值DPS
    static float calculatorValue(LQEffectInfoData* cfgdata, GameObj* hero, GameObj* target, LQAttackDataInterface* herodata, LQAttackDataInterface* squadata, int mainAttr,int value = 0);
    //效果计算公式  3、属性变动计算
    void calculatorPropValue(GameObj* hero, GameObjectVector* targetObjects, LQAttackDataInterface* herodata, int mainAttr,int sign, int times = 0, float outtime = 0); //float bufftime=0,
    int calculatorPropValue(LQAttackDataInterface* herodata, LQAttackDataInterface* targetdata, int mainAttr,int xValue);
    //效果计算公式  5、Debuff持续时间的结果计算
    float calculatorBuffTime(LQAttackDataInterface* herodata, LQAttackDataInterface* targetdata, int mainAttr);
    //效果计算公式  6、Buff持续时间的结果计算
    float calculatorBuffTime(LQAttackDataInterface* herodata, int mainAttr);
};

typedef vector<LQEffectUserData *> LQEffectUserDataVector;
#endif
