//
//  LQAttackCalculator.h
//  SuperWarriors
//
//  Created by chenli on 14-6-19.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//
#ifndef __SuperWarriors_LQAttackCalculator_H__
#define __SuperWarriors_LQAttackCalculator_H__

#include "cocos2d.h"
#include "../unit/LQCombatUnitEnums.h"
#include "LQAttackDataInterface.h"
#include "LQBaseUserData.h"
#include "LQArmsInfoData.h"
#include "LQActionInfoData.h"
#include "LQBuffUserData.h"

//战斗计算
class LQAttackCalculator : public LQAttackDataInterface
{
protected:
    float m_dttime;                 //记录时间
    bool m_stopCrit;                //禁止暴击
    int m_HpValue;                  //生命数值
    ValueMap m_attrMap;             //属性数值
    float _critDamageCoefficient;   //暴击伤害系数
    
    CC_SYNTHESIZE(GameObj*, m_parent, Parent );    //对象的所属者 Parent
    
    //Buffer
    CC_SYNTHESIZE_READONLY(LQBuffUserDataVector*, m_buffers, Buffers );    //暴击值 >10000触发
    
    CC_SYNTHESIZE(int, m_CurrCrit, CurrCrit );    //暴击值 >10000触发
    
    void splitValue(const string& valuestr, char splitch1 = ';', char splitch2 = ',', bool maintype = false); //拆分属性值的数据
    void splitValue(const string& valuestr, ValueMap& attrMap);
    ValueMap getAttrMap(){ return m_attrMap; };
public:
    LQAttackCalculator();
    ~LQAttackCalculator();
    
    //有效防御值
    virtual float getCurrentDefense(LQAttackDataInterface* armsdata);
    //总伤害输出
    virtual float getCurrentDPS(LQAttackDataInterface* armsdata);
    //是否暴击
    virtual bool isCrit(LQAttackDataInterface* armsdata);
    
    virtual float getCurrStdAtr(int attrtype); //当前属性
    virtual void setCurrStdAtr(int attrtype, int value);  //当前属性
    virtual void addCurrStdAtr(int attrtype, int value);  //添加当前属性

    void setCurrLiveAtr(int v1, int v2, int v3, int v4); //设置当前的死活军团数
    
    float getCurrHpRate(); //当前生命比率
    
    virtual int getHpMaxValue(); //当前总生命
    virtual int getCurrHpValue(); //当前生命
    virtual int getCurrApValue(); //当前攻击
    virtual int getCurrDpValue(); //当前防御
    virtual int getCurrShValue(); //当前护盾
    virtual float getCurrSpatkFacter(); //当前突击增加额外伤害 冲锋、迎击的基础伤害（无视对手防御力）4.10
    
    virtual int getForInitComValue();  //武勇+智力统率
    
    // 暴击 克制
    virtual float getCurrStrike(LQArmsTypeEnum armsType);   //当前暴击
    virtual float getCurrDeStrike(LQArmsTypeEnum armsType); //当前韧性
    virtual float getCurrDamage(LQArmsTypeEnum armsType);   //当前暴伤
    virtual float getCurrDeDamage(LQArmsTypeEnum armsType); //当前免暴伤
    // 兵种伤害百分比 4.10
    virtual float getCurrHurtRate(LQArmsTypeEnum armsType); //对兵种伤害提高 降低%
    // 兵种暴击伤害百分比 4.10
    virtual float getCritHurtRate(LQArmsTypeEnum armsType); //对兵种暴击伤害的百分比%
    // 兵种伤害减免百分比 4.11
    virtual float getCurrDeHurtRate(LQArmsTypeEnum armsType); //对兵种伤害减免提高或者降低%
    
    // buffer update
    virtual void update(float dt);
    
    // BUFF
    void addBufferData(LQBuffUserData* buffData);      //添加BUFF
    void removeBufferData(LQBuffUserData* bufferData); //删除BUFF
    void cleareBuffer();
    
    //静止暴击
    void stopCrit(bool value);
    
    CC_SYNTHESIZE(int, m_rageBeAttack, RageBeAttack );	//被攻击怒气
    CC_SYNTHESIZE(int, m_rageAttack, RageAttack );	//攻击怒气
    CC_SYNTHESIZE_READONLY(int, m_furyMaxValue, FuryMaxValue );	//当前等级的怒气上限
    int getFuriesValue(float dps,LQAttackCalculator* data, float rageAttack);
    
    //CC_SYNTHESIZE(float, m_spatkFacter, SpatkFacter );	//冲锋、迎击的基础伤害（无视对手防御力） ATTR_SPATKFAC
};

#endif