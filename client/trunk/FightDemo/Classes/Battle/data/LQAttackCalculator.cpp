//
//  LQAttackCalculator.cpp
//  SuperWarriors
//
//  Created by LequWorld Studio on 2014
//  Copyright (c) 2013年 LequGame. All rights reserved.	
//
#include "LQAttackCalculator.h"
#include "LQData.h"

using namespace cocos2d;


LQAttackCalculator::LQAttackCalculator()
{
    m_dttime = -0.5;
    m_rageAttack = 0;
    m_rageBeAttack = 0;
    m_furyMaxValue = 0;
    m_parent = NULL;
    m_stopCrit = false;
    m_isForward = false;
    //当前暴击系数值
    m_CurrCrit = 0;
    m_isCrit = false;
    m_isMuseCrit = false;
    m_isMuseNotCrit = false;
    m_armsType = Combat_ArmsType_None;
    m_occupation = Combat_Occupation_None;
    //暴击伤害系数
    _critDamageCoefficient = getFormulaParamFactor(14,"e");
    m_HpValue = 0;
    //Buffers
    m_buffers = new LQBuffUserDataVector();
};

LQAttackCalculator::~LQAttackCalculator()
{
    //释放对象资源
    if (m_buffers) {
        deleteInVector(*m_buffers);
    }
    CC_SAFE_DELETE(m_buffers);
};

//拆分属性值的数据
void LQAttackCalculator::splitValue(const string& valuestr, char splitch1, char splitch2, bool maintype)
{
    std::vector<std::string> attrvals = split(valuestr.c_str(), splitch1);  //';'
    int k=0;
    while (k<attrvals.size())
    {
        string attrstr = attrvals.at(k);
        std::vector<std::string> attrvalue = split(attrstr.c_str(), splitch2);  //','
        if(attrvalue.size()==2){
            string attrname = attrvalue[0].c_str();
            if(maintype){
                attrname = intToString(atoi(attrvalue[0].c_str())-ATTR_MAIN);
            }
            int value = atoi(attrvalue[1].c_str());
            if (atoi(attrname.c_str())==ATTR_CURRHP)
                m_HpValue = value;       //当前生命
            m_attrMap[attrname.c_str()] = value;
        }
        k++;
    }
}

void LQAttackCalculator::splitValue(const string& valuestr, ValueMap& attrMap)
{
    std::vector<std::string> attrvals = split(valuestr.c_str(), ';');
    int k=0;
    while (k<attrvals.size())
    {
        string attrstr = attrvals.at(k);
        std::vector<std::string> attrvalue = split(attrstr.c_str(), ',');
        if(attrvalue.size()==2){
            string attrname = attrvalue[0].c_str();
            int value = atoi(attrvalue[1].c_str());
            if (atoi(attrname.c_str())==ATTR_CURRHP)
                m_HpValue = value;       //当前生命
            attrMap[attrname.c_str()] = value;
        }
        k++;
    }
}

//获取当前属性
float LQAttackCalculator::getCurrStdAtr(int attrtype)
{
    //if (attrtype==ATTR_AFFECT_TREAT)
    //    CCLOG("%p getCurrStdAtr attrtype = %d ", this, attrtype);
    if (attrtype==ATTR_CURRHP)   //获取当前HP
        return this->getCurrHpValue()*0.01f;
    if (attrtype==ATTR_LOSTHP)   //获取缺失生命
        return (this->getCurrStdAtr(ATTR_HP)-this->getCurrHpValue())*0.01f;
    if (attrtype==ATTR_FACTER)   //固定系数值 恒等于1 cl-add4.1
        return 1;
    if (attrtype==ATTR_RAGEMAX)   //获取怒气上限
        return m_furyMaxValue;
    int a = m_attrMap[ intToString(attrtype).c_str() ].asInt();
    int b = m_attrMap[ intToString(attrtype+ATTR_ADDVALUE).c_str() ].asInt();
    //if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
    //    CCLOG("%p getCurrStdAtr attrtype = %d  a = %d b= %d", this, attrtype, a, b);
    return a+b;
}

//获取当前属性
void LQAttackCalculator::setCurrStdAtr(int attrtype, int value)
{
    //if (attrtype==ATTR_AFFECT_TREAT)
    //    CCLOG("%p setCurrStdAtr attrtype = %d value= %d", this, attrtype, value);
    if (attrtype==ATTR_CURRHP)
        m_HpValue = value;       //当前生命
    else if (attrtype==ATTR_RAGEMAX)
        m_furyMaxValue = value;
    m_attrMap[intToString(attrtype).c_str()] = value;
    if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
        CCLOG("%p setCurrStdAtr attrtype = %d value= %d", this, attrtype, value);
}

//添加当前属性
void LQAttackCalculator::addCurrStdAtr(int attrtype, int value)
{
    //if (attrtype==ATTR_AFFECT_TREAT)
    //    CCLOG("%p addCurrStdAtr attrtype = %d value= %d", this, attrtype, value);

    const char* key = intToString(attrtype+ATTR_ADDVALUE).c_str();
    int newvalue = 0;
    if (attrtype==ATTR_SHIELD){ //护盾
        key = intToString(attrtype).c_str();
        newvalue = MAX(value,m_attrMap[key].asInt());
    }else if (attrtype==ATTR_RAGE){ //怒气
        key = intToString(attrtype).c_str();
    }else if (attrtype>=ATTR_STRIKEALL && attrtype<=ATTR_STHURT_DEALL){ //全兵种暴击等
        int _t = attrtype-70;  // 属性种类 1
        LQArmsTypeEnum tarr = Combat_ArmsType_None; //1 步，2 弓，3 骑，4 策，5机
        switch (m_armsType) {
            case Combat_ArmsType_Remote: //远程
                tarr = Combat_ArmsType_Cavalry;
                break;
            case Combat_ArmsType_Cavalry: //骑兵
                tarr = Combat_ArmsType_Melee;
                break;
            case Combat_ArmsType_Infantry: //步兵
                return;
                break;
            case Combat_ArmsType_Machine: //机械
                tarr = Combat_ArmsType_None;
                break;
            default: //枪兵 Combat_ArmsType_Melee = 1,
                tarr = Combat_ArmsType_Remote;
                break;
        }
        for (int i=0; i<5; i++) {
            //3-1）所有A【不克制】的兵种的压制值，增加值X。
            //3-2）所有A【克制】的兵种（通常只有一个）的压制值，增加（X-2500）。
            newvalue = abs(value);
            if ((int)tarr!=(i+1) && (int)m_armsType!=(i+1)){  //克制
                newvalue -= 2500;
                if (newvalue<0) newvalue = 0;
            }
            if (newvalue==0) continue;
            newvalue = newvalue*(value>0?1:-1);
            int m = 40+i*4+_t;
            key = intToString(m).c_str();
            newvalue = newvalue + m_attrMap[key].asInt();
            if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
                CCLOG("%p 全兵种 addCurrStdAtr attrtype = %d value %d + %d = %d", this, m, m_attrMap[key].asInt(), newvalue-m_attrMap[key].asInt(), newvalue);
            m_attrMap[key] = newvalue;
        }
        return;
    }else
        newvalue = value + m_attrMap[key].asInt();
    
    if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
        CCLOG("%p addCurrStdAtr attrtype = %d value %d + %d = %d", this, attrtype, m_attrMap[key].asInt(), value, newvalue);
    m_attrMap[key] = newvalue;
}

//当前生命比率
float LQAttackCalculator::getCurrHpRate()
{
    return m_HpValue/this->getCurrStdAtr(ATTR_HP);
}

//设置当前的死活军团数
void LQAttackCalculator::setCurrLiveAtr(int v1, int v2, int v3, int v4)
{
    this->setCurrStdAtr(ATTR_DIE1, v1); //死亡敌方军团
    this->setCurrStdAtr(ATTR_DIE2, v2); //死亡我方军团
    this->setCurrStdAtr(ATTR_LIVE1, v3); //存活敌方军团
    this->setCurrStdAtr(ATTR_LIVE2, v4); //存活我方军团
    
    this->setCurrStdAtr(ATTR_LIVE3, MAX(0,v4-v3)+1); //存活我方军团
}

// 独立属性
//当前总生命
int LQAttackCalculator::getHpMaxValue()
{
    return this->getCurrStdAtr(ATTR_HP);
}

//当前生命
int LQAttackCalculator::getCurrHpValue()
{
    return m_HpValue;
}

//当前攻击
int LQAttackCalculator::getCurrApValue()
{
   return this->getCurrStdAtr(ATTR_ATK);
}

//当前防御
int LQAttackCalculator::getCurrDpValue()
{
   return this->getCurrStdAtr(ATTR_DEF);
}

//当前护盾
int LQAttackCalculator::getCurrShValue()
{
    int _hdValue = this->getCurrStdAtr(ATTR_SHIELD);
    if (_hdValue<0){
        this->setCurrStdAtr(ATTR_SHIELD,0);
        _hdValue = 0;
    }
    return _hdValue;
}

//当前突击增加额外伤害
float LQAttackCalculator::getCurrSpatkFacter()
{
    return this->getCurrStdAtr(ATTR_SPATKFAC)*0.0001;
}

//静止暴击
void LQAttackCalculator::stopCrit(bool value)
{
    m_stopCrit = value;
}

//当前暴击
float LQAttackCalculator::getCurrStrike(LQArmsTypeEnum armsType)
{
    //CCLOG("getCurrStrike:%d %d %d %d %d", this->getCurrStdAtr(ATTR_STRIKE1), this->getCurrStdAtr(ATTR_STRIKE2), this->getCurrStdAtr(ATTR_STRIKE3), this->getCurrStdAtr(ATTR_STRIKE4), this->getCurrStdAtr(ATTR_STRIKE5));
    if(armsType==Combat_ArmsType_Remote) //弓将
        return this->getCurrStdAtr(ATTR_STRIKE2);
    else if(armsType==Combat_ArmsType_Cavalry) //骑将
        return this->getCurrStdAtr(ATTR_STRIKE3);
    else if(armsType==Combat_ArmsType_Machine)  //机械
        return this->getCurrStdAtr(ATTR_STRIKE5);
    //Combat_Occupation_Melee 步将
    return this->getCurrStdAtr(ATTR_STRIKE1);
}

//当前韧性
float LQAttackCalculator::getCurrDeStrike(LQArmsTypeEnum armsType)
{
    //CCLOG("getCurrDeStrike:%d %d %d %d %d", this->getCurrStdAtr(ATTR_STRIKE_DE1), this->getCurrStdAtr(ATTR_STRIKE_DE2), this->getCurrStdAtr(ATTR_STRIKE_DE3), this->getCurrStdAtr(ATTR_STRIKE_DE4), this->getCurrStdAtr(ATTR_STRIKE_DE5));
    if(armsType==Combat_ArmsType_Remote) //弓将
        return this->getCurrStdAtr(ATTR_STRIKE_DE2);
    else if(armsType==Combat_ArmsType_Cavalry) //骑将
        return this->getCurrStdAtr(ATTR_STRIKE_DE3);
    else if(armsType==Combat_ArmsType_Machine)  //机械
        return this->getCurrStdAtr(ATTR_STRIKE_DE5);
    //Combat_Occupation_Melee 步将
    return this->getCurrStdAtr(ATTR_STRIKE_DE1);
}

//当前暴伤
float LQAttackCalculator::getCurrDamage(LQArmsTypeEnum armsType)
{
    if(armsType==Combat_ArmsType_Remote) //弓将
        return this->getCurrStdAtr(ATTR_STHURT2)/RATE_PERCENT;
    else if(armsType==Combat_ArmsType_Cavalry) //骑将
        return this->getCurrStdAtr(ATTR_STHURT3)/RATE_PERCENT;
    else if(armsType==Combat_ArmsType_Machine)  //机械
        return this->getCurrStdAtr(ATTR_STHURT5)/RATE_PERCENT;
    //Combat_Occupation_Melee 步将
    return this->getCurrStdAtr(ATTR_STHURT1)/RATE_PERCENT;
}

//当前免暴伤
float LQAttackCalculator::getCurrDeDamage(LQArmsTypeEnum armsType)
{
    if(armsType==Combat_ArmsType_Remote) //弓将
        return this->getCurrStdAtr(ATTR_STHURT_DE2)/RATE_PERCENT;
    else if(armsType==Combat_ArmsType_Cavalry) //骑将
        return this->getCurrStdAtr(ATTR_STHURT_DE3)/RATE_PERCENT;
    else if(armsType==Combat_ArmsType_Machine)  //机械
        return this->getCurrStdAtr(ATTR_STHURT_DE5)/RATE_PERCENT;
    //Combat_Occupation_Melee 步将
    return this->getCurrStdAtr(ATTR_STHURT_DE1)/RATE_PERCENT;
}

//兵种伤害百分比 4.10
//对兵种伤害提高或者降低%
float LQAttackCalculator::getCurrHurtRate(LQArmsTypeEnum armsType)
{
    float val = 0;
    //对全兵种伤害提高或减少%
    float val0 = this->getCurrStdAtr(ATTR_HURTRATEALL)/CENTI_PERCENT;
    if(armsType==Combat_ArmsType_Remote) //弓将
        val = this->getCurrStdAtr(ATTR_HURTRATE2)/CENTI_PERCENT;
    else if(armsType==Combat_ArmsType_Cavalry) //骑将
        val = this->getCurrStdAtr(ATTR_HURTRATE3)/CENTI_PERCENT;
    else if(armsType==Combat_ArmsType_Machine)  //机械
        val = this->getCurrStdAtr(ATTR_HURTRATE5)/CENTI_PERCENT;
    else //Combat_Occupation_Melee 步将
        val = this->getCurrStdAtr(ATTR_HURTRATE1)/CENTI_PERCENT;
    return MAX(val,val0);
}

//对兵种暴击伤害的百分比%
float LQAttackCalculator::getCritHurtRate(LQArmsTypeEnum armsType)
{
    float val = 0;
    //对全兵种暴击伤害提高或减少%
    float val0 = this->getCurrStdAtr(ATTR_CRITHURTRATEALL)/CENTI_PERCENT;
    
    if(armsType==Combat_ArmsType_Remote) //弓将
        val = this->getCurrStdAtr(ATTR_CRITHURTRATE2)/CENTI_PERCENT;
    else if(armsType==Combat_ArmsType_Cavalry) //骑将
        val = this->getCurrStdAtr(ATTR_CRITHURTRATE3)/CENTI_PERCENT;
    else if(armsType==Combat_ArmsType_Machine)  //机械
        val = this->getCurrStdAtr(ATTR_CRITHURTRATE5)/CENTI_PERCENT;
    else //Combat_Occupation_Melee 步将
        val = this->getCurrStdAtr(ATTR_CRITHURTRATE1)/CENTI_PERCENT;
    return MAX(val,val0);
}

//对兵种伤害减免提高或者降低%
float LQAttackCalculator::getCurrDeHurtRate(LQArmsTypeEnum armsType)
{
    float val = 0;
    //对全兵种伤害减免提高或减少%
    float val0 = this->getCurrStdAtr(ATTR_DEHURTRATEALL)/CENTI_PERCENT;
    
    if(armsType==Combat_ArmsType_Remote) //弓将
        val = this->getCurrStdAtr(ATTR_DEHURTRATE2)/CENTI_PERCENT;
    else if(armsType==Combat_ArmsType_Cavalry) //骑将
        val = this->getCurrStdAtr(ATTR_DEHURTRATE3)/CENTI_PERCENT;
    else if(armsType==Combat_ArmsType_Machine)  //机械
        val = this->getCurrStdAtr(ATTR_DEHURTRATE5)/CENTI_PERCENT;
    else //Combat_Occupation_Melee 步将
        val = this->getCurrStdAtr(ATTR_DEHURTRATE1)/CENTI_PERCENT;
    return MAX(val,val0);
}

//已方有效防御值
float LQAttackCalculator::getCurrentDefense(LQAttackDataInterface* armsdata)
{
    return getCurrDpValue();
}

//武勇+智力统率
int LQAttackCalculator::getForInitComValue()
{
    //CCLOG("getForInitComValue: %d %d %d",  this->getCurrStdAtr(ATTR_FOR), this->getCurrStdAtr(ATTR_INT), this->getCurrStdAtr(ATTR_COM));
    
    return this->getCurrStdAtr(ATTR_FOR)+this->getCurrStdAtr(ATTR_INT)+this->getCurrStdAtr(ATTR_COM);
}

//不破防输出 = (int)max（1，己方有效攻击^2*破防系数/[敌方有效防御*(1+破防系数)]*((己方伤害加深+a)/(敌方伤害减免+b)) * ((已方破甲值+c)/(敌方固甲值+d))＊if（暴击成立，暴击伤害系数，１）
//破防输出 = (int)max（1，(己方有效攻击-敌方有效防御)*((己方伤害加深+a)/(敌方伤害减免+b)) * ((已方破甲值+c)/(敌方固甲值+d))
//不破防输出=max⁡（int((破防系数*（己方有效攻击）^2)/(（1+破防系数）^2 敌方有效防御))*min⁡(max⁡(（（己方伤害加深+a）/(敌方伤害减免+b)）,e),f)*min⁡(max（（己方破甲+c）/(敌方固甲+d),g）,h),**if（暴击成立，暴击伤害系数，1），1）
//破防输出=max⁡（（己方有效攻击-敌方有效防御）*min⁡(max⁡(（（己方伤害加深+a）/(敌方伤害减免+b)）,e),f)*min⁡(max（（己方破甲+c）/(敌方固甲+d),g）,h)*if（暴击成立，暴击伤害系数，1），1）
float LQAttackCalculator::getCurrentDPS(LQAttackDataInterface* armsdata)
{
    if(armsdata->getCurrHpValue()<=0)
        return 0;
    
    float value = MIN_DPS;
    float valueTemp1 = 0;
    float critrate = 1.0f;
    
    //武将打兵 兵打武将时走此算法
    if(this->getAttackObjectType()!=armsdata->getAttackObjectType())
    {
        //系数
        float aparam = 0;
        if (this->getAttackObjectType()==Combat_AttackObjectType_Squad)
            aparam = getFormulaParamFactor(26,"a1");  //兵团对将
        else aparam = getFormulaParamFactor(26,"a2");  //将对兵团
        float x1 = aparam - (this->getForInitComValue()-armsdata->getForInitComValue())/10000;
        valueTemp1 = armsdata->getHpMaxValue()/x1;
    }else{
        //系数
        //float aparam = getFormulaParamFactor(23,"a");  //
        //float bparam = getFormulaParamFactor(23,"b");  //
        //float cparam = getFormulaParamFactor(23,"c");  //
        //float dparam = getFormulaParamFactor(23,"d");  //
        //float eparam = getFormulaParamFactor(23,"e");  //
        //float fparam = getFormulaParamFactor(23,"f");  //
        //float gparam = getFormulaParamFactor(23,"g");  //
        //float hparam = getFormulaParamFactor(23,"h");  //
        //破防系数
        float defenceParam = getFormulaParamFactor(19,"x");
        
        //CCLOG("LQAttackCalculator::getCurrentDPS....");
        //CCLOG("LQAttackCalculator:%s  %d", this->getName().c_str(), m_resIDC);
        if (getCurrApValue() < armsdata->getCurrentDefense(this)* (1 + defenceParam)) {
            //CCLOG(" LQAttackCalculator.. 不破防");
            valueTemp1 = (getCurrApValue()*getCurrApValue()*defenceParam) / ((1 + defenceParam)*(1 + defenceParam)*armsdata->getCurrentDefense(this));
            if (_DEBUG_TYPE_VALUE==_DEBUG_TYPE || _DEBUG_TYPE_SKILL==_DEBUG_TYPE)
                CCLOG(" LQAttackCalculator.. 不破防 Ap %d DPa %.3f Def %.3f = %.3f", getCurrApValue(), defenceParam, armsdata->getCurrentDefense(this), valueTemp1);
        }else{
            //CCLOG(" LQAttackCalculator.. 破防");
            valueTemp1 = getCurrApValue() - armsdata->getCurrentDefense(this);
        }
        //伤害加深减免
        //float temp1 = MIN(fparam, MAX(eparam, ((this->getCurrStdAtr(ATTR_HURTADD)/100+aparam)/(armsdata->getCurrStdAtr(ATTR_HURTSUB)/100+bparam))));
        float temp1 = 1 + (this->getCurrStdAtr(ATTR_HURTADD)*0.0001) - (armsdata->getCurrStdAtr(ATTR_HURTSUB)*0.0001);
        valueTemp1 *= temp1;
        //破甲固甲
        //temp1 = MIN(hparam, MAX(gparam, ((this->getCurrStdAtr(ATTR_ARMOR)/100+cparam)/(armsdata->getCurrStdAtr(ATTR_ARMOR_DE)/100+dparam))));
        temp1 = 1 + (this->getCurrStdAtr(ATTR_ARMOR)*0.0001) - (armsdata->getCurrStdAtr(ATTR_ARMOR_DE)*0.0001);
        valueTemp1 *= temp1;
        if (_DEBUG_TYPE_VALUE==_DEBUG_TYPE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL) {
            //CCLOG("  伤害加深减免： (%d + %.3f) / (%d + %.3f)", this->getCurrStdAtr(ATTR_HURTADD), aparam, armsdata->getCurrStdAtr(ATTR_HURTSUB), bparam);
            //CCLOG("  破甲固甲： (%d + %.3f) / (%d + %.3f)", this->getCurrStdAtr(ATTR_ARMOR), cparam, armsdata->getCurrStdAtr(ATTR_ARMOR_DE), dparam);
            CCLOG(" 伤害加深减免：1 + %.3f - %.3f", this->getCurrStdAtr(ATTR_HURTADD)*0.0001, armsdata->getCurrStdAtr(ATTR_HURTSUB)*0.0001);
            CCLOG(" 破甲固甲：1 + %.3f - %.3f", this->getCurrStdAtr(ATTR_ARMOR)*0.0001, armsdata->getCurrStdAtr(ATTR_ARMOR_DE)*0.0001);
        }
        
        //暴击
        if( this->isCrit(armsdata) ){
            _critDamageCoefficient = MAX(getCurrDamage(armsdata->getArmsType())-armsdata->getCurrDeDamage(this->getArmsType()),1.0f);
            critrate = _critDamageCoefficient ;
            //CCLOG("HeroUser.. 暴击 = %.2f", critrate);
        }
        //CCLOG("HeroUser.. ApValue = %d,",m_ApValue);
        valueTemp1 *=  critrate;
    }
    
    value = MAX(value, valueTemp1);
    
    //3.13增加:  突击增加额外伤害
    if(this->getIsForward()){
        float val = getCurrApValue()*getCurrSpatkFacter();
        m_isForward = false;
        if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL)
            CCLOG("  突击增加额外伤害 %d*%.3f = %.3f", getCurrApValue(), getCurrSpatkFacter(), val);
        value += val;
    }

    //4.10增加：对兵种伤害提高或者降低百分比%
    float hurtRate = this->getCurrHurtRate(armsdata->getArmsType());
    if(hurtRate!=0){
        value = value*(1+hurtRate);
        if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL)
            CCLOG("  A 对兵种[%d]伤害提高或者降低了%.3f 结果 value = %.3f", armsdata->getArmsType(), hurtRate, value);
    }
    //4.10增加：对兵种暴击伤害的百分比%
    if(critrate>1){
        float critHurtRate = armsdata->getCritHurtRate(this->getArmsType());
        if (critHurtRate>0 && critHurtRate<1){
            value = value*critHurtRate;
            if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL)
                CCLOG("  B 对兵种[%d]暴击伤害的百分比%.3f 结果 value = %.3f", this->getArmsType(), critHurtRate, value);
        }
    }else{
        //4.11增加：对兵种普通伤害的百分比%
        float dehurtRate = armsdata->getCurrDeHurtRate(this->getArmsType());
        if (dehurtRate>0 && dehurtRate<1){
            value = value*(1-dehurtRate);
            if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL)
                CCLOG("  C 对兵种[%d]普通伤害的百分比%.3f 结果 value = %.3f", this->getArmsType(), dehurtRate, value);
        }
    }
    
    if (_DEBUG_TYPE_VALUE==_DEBUG_TYPE || _DEBUG_TYPE_SKILL==_DEBUG_TYPE) {
        if(critrate>1) CCLOG("  !!!暴击 = %.2f  ",critrate ); // [%.2f] m_CurrCrit
        CCLOG("  攻击方%d 数据： 攻击 = %d, 防御 = %.3f", this->getAttackObjectType(), getCurrApValue(), this->getCurrentDefense(armsdata));
        CCLOG("  被攻击%d 数据： 攻击 = %d, 防御 = %.3f", this->getAttackObjectType(), armsdata->getCurrApValue(), armsdata->getCurrentDefense(this));
        CCLOG("  有效输出 = %.2f",value);
    }
    return value;
}

//是否暴击
//	暴击机率=Min(Max(（已方暴击值-敌方韧性值）,0)，暴击率修正)*0.01
//	在此机率范围内判定暴击是否成立
bool LQAttackCalculator::isCrit(LQAttackDataInterface* armsdata)
{
    //武将不能对武将暴击
    if (armsdata->getAttackObjectType()==Combat_AttackObjectType_Hero)
        return false;
    
    //CCLOG("_critBaseRate %d %d", getCurrStrike(armsdata->getOccupation()), armsdata->getCurrDeStrike(this->getOccupation()));
    int _critBaseRate = MAX(getCurrStrike(armsdata->getArmsType())-armsdata->getCurrDeStrike(this->getArmsType()),0);  //去掉修正
    m_CurrCrit += _critBaseRate;
    //m_isMuseCrit = true;
    if (m_isMuseCrit){  //必爆控制
        m_isCrit = true;
        m_isMuseCrit = false;
    }else if (m_isMuseNotCrit){
        m_isCrit = false;
        m_isMuseNotCrit = false;
    }else if (m_CurrCrit >= RATE_PERCENT) {
        m_isCrit = true;
        m_CurrCrit -= RATE_PERCENT;
    }else{
        m_isCrit = false;
    }
    return m_isCrit;
}

// buffer update
void LQAttackCalculator::update(float dt)
{
    m_dttime += dt;
    
    LQBuffUserData* buff;
    for(int i=0; i<m_buffers->size(); i++ ){
        buff = m_buffers->at(i);
        if(buff){
            buff->update(dt);
        }
    }
}

//只有武将释放技能时，才会对军团和将造成buff，  判断是否产生buff的 计算公式在heroUserdata内。
//添加buff ，优先级高，则剔除之前的buff 状态； 如果优先级相同则叠加
void LQAttackCalculator::addBufferData(LQBuffUserData* buffData)
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_BUFF)
        CCLOG("Data::addBuffer....%p %d BuffType:%d", buffData, buffData->getResIDC(), buffData->getBuffType());
    
    if (_DEBUG_TYPE==_DEBUG_TYPE_BUFF && buffData->getResIDC()==40041259)
        CCLOG("Data::addBuffer 40041259 size:%lu",m_buffers->size());
    
    LQBuffUserDataVector m_removebuffers;
    int flag = 0;//0---大于所有的之前的buff，1有相同buff优先级， 2 级别太低，不构成添加状态 3 有相同的
    for (int i  = 0; i <  m_buffers->size(); i++) {
        LQBuffUserData* tempdata = m_buffers->at(i);
        if(_DEBUG_TYPE==_DEBUG_TYPE_BUFF){
            CCLOG("  >>BUFF:ResIDC temp %d, buff %d",tempdata->getResIDC(),buffData->getResIDC());
            CCLOG("  >>BUFF:Priority temp %d, buff %d",tempdata->getPriority(),buffData->getPriority());
            CCLOG("  >>BUFF:BuffFlag temp %d, buff %d",tempdata->getBuffFlag(),buffData->getBuffFlag());
        }
        if(tempdata->getResIDC() == buffData->getResIDC())
        {
            flag = 3;
            break;
        }
        if (tempdata->getPriority() == buffData->getPriority() &&
            (buffData->getBuffFlag()==0 || tempdata->getBuffFlag()!= buffData->getBuffFlag()) ) {
            flag = 1;
            break;
        }
        else if (buffData->getBuffFlag()>0 && tempdata->getBuffFlag() == buffData->getBuffFlag()){
            m_removebuffers.push_back(tempdata);
            flag = 1;
            break;
        }else if(tempdata->getPriority() > buffData->getPriority()){
            flag = 2;
            break;
        }
    }
    
    if (flag == 0) {  //清除所有老的，添加新的
        for (int i  = 0; i <  m_buffers->size(); i++) {
            LQBuffUserData* tempdata = m_buffers->at(i);
            m_removebuffers.push_back(tempdata);
        }
        m_buffers->clear();
        m_buffers->push_back(buffData);
        if(m_parent){
            buffData->onBufferCallback = CC_CALLBACK_1(GameObj::bufferCallback,m_parent);
            buffData->onBufferEndTime = CC_CALLBACK_1(GameObj::bufferEndCallback,m_parent);
        }
    }else if (flag == 1){ //添加新的
        if(m_parent){
            buffData->onBufferCallback = CC_CALLBACK_1(GameObj::bufferCallback,m_parent);
            buffData->onBufferEndTime = CC_CALLBACK_1(GameObj::bufferEndCallback,m_parent);
        }
        m_buffers->push_back(buffData);
    }else if (flag == 2){ //现在级别高于新的BUF，跳过
        
    }else if (flag == 3){ //现在级别高于新的BUF，跳过
        for (int i  = 0; i <  m_buffers->size(); i++) {
            LQBuffUserData* tempdata = m_buffers->at(i);
            if(tempdata->getResIDC() == buffData->getResIDC())
                m_removebuffers.push_back(tempdata);
        }
        m_buffers->push_back(buffData);
        if(m_parent){
            buffData->onBufferCallback = CC_CALLBACK_1(GameObj::bufferCallback,m_parent);
            buffData->onBufferEndTime = CC_CALLBACK_1(GameObj::bufferEndCallback,m_parent);
        }
    }
    
    for (int i = 0; i < m_removebuffers.size(); i++) {
        LQBuffUserData* buffdata1 = m_removebuffers.at(i);
        buffdata1->setActive(false); //Buff作废
        if (buffdata1->onBufferEndTime)
            buffdata1->onBufferEndTime(buffdata1);
    }
    
    return;
}

//移除buff，移除buff状态时，某些buff数据需要恢复
void LQAttackCalculator::removeBufferData(LQBuffUserData* bufferData)   //删除BUFF
{
    if (!m_buffers)
        return;
    
    if (bufferData->getResIDC()==40061440 || bufferData->getResIDC()==40061441)
        CCLOG("Data::removeBuffer.... %p", bufferData);
    
    LQBuffUserDataVector::iterator iter = remove(m_buffers->begin(), m_buffers->end(), bufferData);
    if (iter != m_buffers->end()) {
        if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_BUFF)
            CCLOG("Data::removeBuffer.... %p", bufferData);
        m_buffers->erase(iter);
        CC_SAFE_DELETE(bufferData);
    }
    
}

//清除所有BUFF
void LQAttackCalculator::cleareBuffer()
{
    while(!m_buffers->empty()) {
        LQBuffUserData* bufferData = m_buffers->back();
        m_buffers->pop_back();
        if (bufferData->onBufferEndTime)
            bufferData->onBufferEndTime(bufferData);
        delete bufferData;
    }
}

//怒气变化值
//  (伤害血量/本方武将总血量)*1000*(本方武将怒气上限/10000)*本方武将怒气回复系数
//  (伤害血量/本方小兵军团总血量)*1000*(本方武将怒气上限/10000)*本方武将怒气回复系数
int LQAttackCalculator::getFuriesValue(float dps,LQAttackCalculator* data, float rageAttack)
{
    if(dps<=0 || data->getCurrHpValue()<=0)
        return 0;
    float _furies = (dps/data->getHpMaxValue()) * 100.00f;
    _furies = MIN(100,_furies);
    //if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
    //    CCLOG(" getFuriesValue %.3f, %d, %.3f %.3f", dps, data->getHpMaxValue(), _furies, rageAttack);
    //_furies *= (data->getFuryMaxValue() * 0.0001f);
    _furies *= rageAttack;
    return (int)_furies;
}