//
//  LQEffectUserData.cpp
//  SuperWarriors
//
//  Created by lequdev1 on 14-7-15.
//
//

#include "LQEffectUserData.h"
#include "LQAttackCalculator.h"

LQEffectUserData::LQEffectUserData(int resIDC, int objectType)
{
    m_skillInfo = NULL;
    aliasName = "EffectInfo";
    m_formulaType = 0;
    m_objectType = objectType;
    m_missileType=Missile_Fly_GJ;
    m_missileEffect = 0;
    m_hitType = 0;
    m_hitEffect = 0;
    m_SPFMarker = -1;     //取值方向
    m_SPFMarkerAttr = -1; //取值主属性
    m_hurtSum = 0;
    m_numFactor = 0;
    
    initWithIDC(resIDC);
}

LQEffectUserData::~LQEffectUserData()
{

}
// 初始化数据,子类重写
const char* LQEffectUserData::getAliasName()
{
    return aliasName;
}

// 返回配置数据子类覆盖
LQEffectInfoData*  LQEffectUserData::getCfgdata()
{
    return (LQEffectInfoData*)m_cfgdata;
}

// 返回特效配置数据
LQAnimationInfoData* LQEffectUserData::getAnimationCfgData()
{
    return (LQAnimationInfoData*)this->m_animationCfgData;
}

// 初始化数据,子类重写
void LQEffectUserData::putData()
{
    m_missileType = (LQMissileFlyType)getCfgdata()->getMissileType();
    m_missileEffect = getCfgdata()->getMissileEffect();
    m_hitType = getCfgdata()->getHitType();
    m_hitEffect = getCfgdata()->getHitEffect();
    
    m_formulaType = getCfgdata()->getFormulaType();
    m_animationType = (KLQAnimationDisplayType)getCfgdata()->getAniDisplayType();
    m_animationID = getCfgdata()->getAnimationID();
    m_effectType = getCfgdata()->getEffectType();
    m_numFactor = getCfgdata()->getNumFacter();
    
    string _SPFMarker = getCfgdata()->getSPFMarker(); //定义1号公式的X为可选的属性代码。拼串，1代表施法者，2代表目标。\r\n（例：\r\n1,15，代表X取施法者的15）
    if (_SPFMarker.length()>0){
        std::vector<std::string> attrvalue = split(_SPFMarker.c_str(), ',');
        if(attrvalue.size()>=2){
            m_SPFMarker = atoi(attrvalue[0].c_str());    //取值方向
            m_SPFMarkerAttr = atoi(attrvalue[1].c_str());; //取值主属性
        }
    }
    //获取特效数据
    if(m_animationType==1 && m_animationID>0){
        m_animationCfgData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), m_animationID);
        m_skillPosition = PointFromString(m_animationCfgData->getPosition());
    }else m_skillPosition = Point::ZERO;
    m_outputTime = -1;
    m_hurtSum = 0;
}

//原有技能公式1、2、7变更为如下公式：
//y=Min（Max（（DominantAttribute攻/DominantAttribute防）*EffectFactor，EffectBase），MaxLimit）*x+b
//所有Buff、Debuff、Dot和控制时间为固定值，不再对时间进行操作，即去掉公式5和6
//原表格结构中Valuea不再需要，可以删除该列
//
//保留公式统一如下：
//y=Min（Max（（DominantAttribute攻/DominantAttribute防）*EffectFactor，EffectBase），MaxLimit）*x+b
void LQEffectUserData::calculatorValue(GameObj* hero, GameObjectVector* targetObjects, LQAttackDataInterface* herodata, LQAttackDataInterface* squadata, int mainAttr, int times, float outtime)
{
    m_hurtSum = 0;
    int _outputType = this->getCfgdata()->getOutputType();        //输出类型
    //int _effectType = this->getCfgdata()->getEffectType();            //输出类型
    //if(_outputType!=15)  //生命
    //    CCLOG("CalculatorValue OutputType %d is ERROR.", _outputType);
    float _basevalue = this->getCfgdata()->getEffectBase()/RATE_PERCENT;  //校准值
    float _factor = this->getCfgdata()->getEffectFactor()/RATE_PERCENT;   //输出系数
    float _maxLimit = this->getCfgdata()->getMaxLimit()/RATE_PERCENT;     //输出上限校正系数
    //int _splashArea = this->getCfgdata()->getSplashArea();        //溅射范围
    int bparam = this->getCfgdata()->getValueb();
    int _unittype = this->getCfgdata()->getEffectType();
    LQFlyNumberTypeEnum _numbertype = _unittype==1 ? klq_flaytype_skill : klq_flaytype_normal;
    LQAttackCalculator* targetdata;
    float hurt1 = 0;
    for (int i=0; i<=times; i++) {
        for (GameObj* targetobj : *targetObjects ) {
            targetdata = (LQAttackCalculator*)targetobj->getCalculator();
            //if (hero->getIndex()==80)
            //    CCLOG("hero->getIndex()");
            hurt1 = (herodata->getCurrStdAtr(mainAttr)/targetdata->getCurrStdAtr(mainAttr))*_factor;
            hurt1 = MAX(hurt1,_basevalue);
            hurt1 = MIN(hurt1,_maxLimit);
            if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
                CCLOG("   ->mainAttr=%d atr my = %0.3f target =%0.3f _factor=%0.3f basevalue=%0.3f maxLimit=%0.3f", mainAttr, herodata->getCurrStdAtr(mainAttr), targetdata->getCurrStdAtr(mainAttr), _factor, _basevalue, _maxLimit);
            if(_outputType==ATTR_HP){  //生命 UnitType == 1 公式1
                if (this->getFormulaType()==2){  //强制暴击
                    herodata->setIsMustCrit();
                    squadata->setIsMustCrit();
                }else{  //其他公式（现为的所有效果） 技能必不爆
                    herodata->setIsMustNotCrit();
                    squadata->setIsMustNotCrit();
                    if(this->getCfgdata()->getSkillCrit()>0)  //强制技能暴击
                    {
                        herodata->setIsMustCrit();
                        squadata->setIsMustCrit();
                    }
                }
                
                //公式中的伤害值x
                float hurt = 0;
                if (m_SPFMarker<0){  //伤害值x
                    if (targetdata->getAttackObjectType()==Combat_AttackObjectType_Hero)
                        hurt = herodata->getCurrentDPS(targetdata);
                    else
                        hurt = squadata->getCurrentDPS(targetdata);
                }else{  //特殊值代替x
                    if (m_SPFMarker==0)
                        hurt = herodata->getCurrStdAtr(m_SPFMarkerAttr);
                    else
                        hurt = targetdata->getCurrStdAtr(m_SPFMarkerAttr);
                }
                
                if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
                    CCLOG("   ->基础伤害 %d.... hurt = %0.3f 放大倍数=%0.3f bparam=%d", hero->getIndex(), hurt, hurt1, bparam);
                hurt1 *= hurt;
                hurt1 += bparam;
                
                if (m_numFactor>0){
                    float df = herodata->getCurrStdAtr(m_numFactor)*0.1;
                    hurt1 *= df;
                    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3 || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
                        CCLOG(" %d 技能输出*数量系数 %f.... hurt1 = %0.3f", m_resIDC, df, hurt1);
                }
                                
                if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3 || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
                    CCLOG(" %d 技能伤害输出 %d.... 次:%d hurt1 = %0.3f  %f", m_resIDC, hero->getIndex(), times>0 ? i+1 : 0, hurt1, getCurrentMillSecond());
                
                float _bufftime = this->calculatorBuffTime(herodata, (this->getCfgdata()->getTFType()==5) ?targetdata:NULL , mainAttr); //NewCL 4.6
                if (_bufftime>0){
                    LQBuffUserData* newbuf = new LQBuffUserData(hero, this, _bufftime, hurt1);
                    targetobj->addBuffer( newbuf );
                }else{
                    m_hurtSum += hurt1;
                    hero->changeTargetDps(targetobj, hurt1, _numbertype, this->getCfgdata(), times>0 ? i+1 : 0, outtime);  //伤害符号是反着的
                }
            }else{      //生命 UnitType == 2 公式7
                hurt1 *= -herodata->getCurrStdAtr(_outputType);
                //目标属性发生变化
                float _bufftime = this->getCfgdata()->getBuffTime()/RATE_PERCENT;  //持续时间
                if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL  || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
                    CCLOG(" 技能改变属性 %d.... hurt1 = %0.3f bufftime = %0.3f", hero->getIndex(), hurt1, _bufftime);
                LQBuffUserData* newbuf = new LQBuffUserData(hero, this, _bufftime, hurt1);
                targetobj->addBuffer( newbuf );
            }
        }
    }
    
    if (this->getCfgdata()->getSuckBlood()>0 && m_hurtSum!=0) //吸血
    {
        hero->changeTargetDps(hero, -m_hurtSum, klq_flaytype_skill);
    }
    //CC_SAFE_DELETE(targetObjects);
    //CCLOG(" Hero::主动技能[%s] 伤害 公式类型：%d  hurt = %.3f", skillUserData->getName().c_str(), this->getFormulaType(), hurt1);
}

float LQEffectUserData::calculatorValue(LQEffectInfoData* cfgdata, GameObj* hero, GameObj* target, LQAttackDataInterface* herodata, LQAttackDataInterface* squadata, int mainAttr,int value)
{
    float _basevalue = cfgdata->getEffectBase()/RATE_PERCENT;  //校准值
    float _factor = cfgdata->getEffectFactor()/RATE_PERCENT;   //输出系数
    float _maxLimit = cfgdata->getMaxLimit()/RATE_PERCENT;     //输出上限校正系数
    int bparam = cfgdata->getValueb();
    //int _unittype = cfgdata->getEffectType();
    int m_SPFMarker = -1;
    int m_SPFMarkerAttr = 0;
    string _SPFMarker = cfgdata->getSPFMarker(); //定义1号公式的X为可选的属性代码。拼串，1代表施法者，2代表目标。\r\n（例：\r\n1,15，代表X取施法者的15）
    if (_SPFMarker.length()>0){
        std::vector<std::string> attrvalue = split(_SPFMarker.c_str(), ',');
        if(attrvalue.size()>=2){
            m_SPFMarker = atoi(attrvalue[0].c_str());    //取值方向
            m_SPFMarkerAttr = atoi(attrvalue[1].c_str());; //取值主属性
        }
    }
    
    LQAttackCalculator* targetdata;
    float hurt1 = 0;

    targetdata = (LQAttackCalculator*)target->getCalculator();
    hurt1 = (herodata->getCurrStdAtr(mainAttr)/targetdata->getCurrStdAtr(mainAttr))*_factor;
    hurt1 = MAX(hurt1,_basevalue);
    hurt1 = MIN(hurt1,_maxLimit);
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
        CCLOG("   ->mainAttr=%d atr my = %0.3f target =%0.3f _factor=%0.3f basevalue=%0.3f maxLimit=%0.3f", mainAttr, herodata->getCurrStdAtr(mainAttr), targetdata->getCurrStdAtr(mainAttr), _factor, _basevalue, _maxLimit);
    float hurt = value;
    if (value<=0){
        if (m_SPFMarker<0){  //伤害值x
            if (targetdata->getAttackObjectType()==Combat_AttackObjectType_Hero)
                hurt = herodata->getCurrentDPS(targetdata);
            else
                hurt = squadata->getCurrentDPS(targetdata);
        }else{  //特殊值代替x
            if (m_SPFMarker==0)
                hurt = herodata->getCurrStdAtr(m_SPFMarkerAttr);
            else
                hurt = targetdata->getCurrStdAtr(m_SPFMarkerAttr);
        }
        
        if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
            CCLOG("   ->基础伤害 %d.... hurt = %0.3f 放大倍数=%0.3f bparam=%d", hero->getIndex(), hurt, hurt1, bparam);
        hurt1 *= hurt;
        int hurt2 = hurt1 + bparam;
        hurt1 = hurt2;
    }else
    {
        hurt1 += bparam;
    }
    
    return hurt1;
}

//效果计算公式  3、属性变动计算 4、属性变动计算,自身属性为基值
//y=Min（Max（DominantAttribute施法者*EffectFactor，EffectBase），MaxLimit）*x
//每点施法者属性转换*系数
void LQEffectUserData::calculatorPropValue(GameObj* hero, GameObjectVector* targetObjects, LQAttackDataInterface* herodata, int mainAttr,int sign, int times, float outtime)
{
    m_hurtSum = 0;
    int _sourceType = this->getCfgdata()->getSourceType();            //输出类型
    int _outputType = this->getCfgdata()->getOutputType();            //输出类型
    if(_sourceType<=0) _sourceType = _outputType;
    int _formulaType = this->getCfgdata()->getFormulaType();            //公式类型
    float _basevalue = this->getCfgdata()->getEffectBase()/RATE_PERCENT;  //校准值
    float _factor = this->getCfgdata()->getEffectFactor()/RATE_PERCENT;   //输出系数
    float _maxlimit = this->getCfgdata()->getMaxLimit()/RATE_PERCENT;     //输出上限校正系数
    //int _splashArea = this->getCfgdata()->getSplashArea();        //溅射范围
    int bparam = this->getCfgdata()->getValueb();
    float value1 = 0;
    LQAttackDataInterface* targetdata;
    for (int i=0; i<=times; i++) {
        for (GameObj* targetobj : *targetObjects ) {
            targetdata = targetobj->getCalculator();
            value1 = MIN( MAX(herodata->getCurrStdAtr(mainAttr)*0.01*_factor,_basevalue), _maxlimit);
            float keyvalue = 1; //cl-modi4.1
            if (_formulaType==3){ //3号公式
                if(_sourceType==ATTR_HP)
                    keyvalue = targetdata->getHpMaxValue();
                else
                    keyvalue = targetdata->getCurrStdAtr(_sourceType);
            }else{  //4号公式
                if(_sourceType==ATTR_HP)
                    keyvalue = herodata->getHpMaxValue();
                else
                    keyvalue = herodata->getCurrStdAtr(_sourceType);
            }
            int value= value1*keyvalue*sign;  //增益和减益
            value += bparam;
            if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL  || _DEBUG_TYPE==_DEBUG_TYPE_VALUE){
                CCLOG(" 技能改变属性 (3,4公式) %d.... value = %d", hero->getIndex(), value);
                CCLOG("  1) MIN( MAX(herodata->getCurrStdAtr(mainAttr)*_factor,_basevalue), _maxlimit)");
                CCLOG("   _basevalue %.3f _maxlimit %.3f sign %d", _basevalue, _maxlimit, sign);
                CCLOG("   value1*=  mainAttr %.3f *_factor %.3f = %.3f", herodata->getCurrStdAtr(mainAttr)*0.01, _factor, herodata->getCurrStdAtr(mainAttr)*0.01*_factor);
                CCLOG("  2) value1 *= targetdata->getCurrStdAtr(_sourceType)*sign  ---_sourceType可能没有");
                CCLOG("   _outputType [%d] = %.3f", _outputType, targetdata->getCurrStdAtr(_outputType));
                CCLOG("   _sourceType [%d] = %.3f", _sourceType, targetdata->getCurrStdAtr(_sourceType));
                CCLOG("   value = %.3f * %.3f * %d", value1, keyvalue, sign);
                CCLOG("   value = %d + %.3f", value, targetdata->getCurrStdAtr(_outputType));
            }
            
            if (m_numFactor>0){
                float df = herodata->getCurrStdAtr(m_numFactor)*0.1;
                value *= df;
                if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3 || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
                    CCLOG(" 技能改变属性*数量系数 %f.... value = %d", df, value);
            }
            
            //加血
            if(_outputType==ATTR_HP){
                float _bufftime = this->calculatorBuffTime(herodata, (this->getCfgdata()->getTFType()==5) ?targetdata:NULL , mainAttr); //NewCL 4.6
                if (_bufftime>0){
                    LQBuffUserData* newbuf = new LQBuffUserData(hero, this, _bufftime, -value);
                    targetobj->addBuffer( newbuf );
                }else{
                    m_hurtSum += -value;
                    hero->changeTargetDps(targetobj, -value, klq_flaytype_skill, this->getCfgdata(), times>0 ? i+1 : 0, outtime);  //伤害符号是反着的
                }
            }else if (_outputType==ATTR_RAGE){  //直接加怒气属性
                float _bufftime = this->calculatorBuffTime(herodata, (this->getCfgdata()->getTFType()==5) ?targetdata:NULL , mainAttr); //NewCL 4.6
                if (_bufftime>0){
                    LQBuffUserData* newbuf = new LQBuffUserData(hero, this, _bufftime, value);
                    targetobj->addBuffer( newbuf );
                }else
                    targetobj->changeHeroFuries(value);
            }else{
                //目标属性发生变化
                if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL  || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
                    CCLOG("   TFType = %d ", this->getCfgdata()->getTFType());
                float _bufftime = this->calculatorBuffTime(herodata, (this->getCfgdata()->getTFType()==5) ?targetdata:NULL , mainAttr); //NewCL 4.6
                if(_bufftime>0){
                    LQBuffUserData* newbuf = new LQBuffUserData(hero, this, _bufftime, value);
                    if(this->getCfgdata()->getBuffSubType()==1 || this->getCfgdata()->getBuffSubType()==2 || this->getCfgdata()->getBuffSubType()==62 || this->getCfgdata()->getBuffSubType()==13){
                        if(this->getCfgdata()->getBuffSubType()==13)
                            targetdata->addCurrStdAtr(_outputType, value);  //4.12不得已改的，白刃改出的问题
                        targetobj->bufferCallback( newbuf );
                    }
                    targetobj->addBuffer( newbuf );
                }else{
                    //直接加属性
                    targetdata->addCurrStdAtr(_outputType, value);
                }
            }
            
            //targetdata->addCurrStdAtr(_outputType, value1);
        }
    }
    
    if (this->getCfgdata()->getSuckBlood()>0 && m_hurtSum!=0) //吸血
    {
        hero->changeTargetDps(hero, -m_hurtSum, klq_flaytype_skill);
    }
}

//4	"Buff对属性改变的结果=Min（Max（DominantAttribute施法者*EffectFactor，EffectBase），MaxLimit）*BuffValue
//Buff持续时间=BuffTime"	Buff属性改变=每点施法者属性转换*系数，持续时间是固定值
int LQEffectUserData::calculatorPropValue(LQAttackDataInterface* herodata, LQAttackDataInterface* targetdata, int mainAttr,int xValue)
{
    //int _outputType = this->getCfgdata()->getOutputType();            //输出类型
    float _basevalue = this->getCfgdata()->getEffectBase()/RATE_PERCENT;  //校准值
    float _factor = this->getCfgdata()->getEffectFactor()/RATE_PERCENT;   //输出系数
    float _maxlimit = this->getCfgdata()->getMaxLimit()/RATE_PERCENT;     //输出上限校正系数
    float value1 = MIN( MAX(herodata->getCurrStdAtr(mainAttr)*0.01*_factor,_basevalue), _maxlimit);
    int bparam = this->getCfgdata()->getValueb();
    //value1 *= sign;  //增益和减益
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL  || _DEBUG_TYPE==_DEBUG_TYPE_VALUE){
        CCLOG(" 技能Debuff持续时间的结果计算（5号公式）） ");
        CCLOG("  N0.4) herodata->getCurrStdAtr(mainAttr)*_facter");
        CCLOG("   _basevalue  %.3f _factor %.3f _maxlimit %.3f", _basevalue, _factor, _maxlimit);
        CCLOG("   value1 = mainAttr1 %.3f*_factor %.3f = %.3f", herodata->getCurrStdAtr(mainAttr)*0.01, _factor, value1);
        CCLOG("   value = %.3f * %d = %.3f", value1, xValue, value1*xValue);
    }
    
    value1 *= xValue;
    value1 += bparam;
    return value1;
}

//效果计算公式  5、Debuff持续时间的结果计算
// Debuff:
// y=BuffTime+Min(DominantAttribute攻/DominantAttribute防*timefacter, timelimit)
float LQEffectUserData::calculatorBuffTime(LQAttackDataInterface* herodata, LQAttackDataInterface* targetdata, int mainAttr)
{
    if (targetdata==NULL){
        return calculatorBuffTime(herodata, mainAttr); //6号公式
    }
    
    float _timefacter = this->getCfgdata()->getTimeFacter()/RATE_PERCENT;   //输出系数
    float _timelimit = this->getCfgdata()->getTimelimit()/RATE_PERCENT;     //输出上限校正系数
    float _bufftime = this->getCfgdata()->getBuffTime()/RATE_PERCENT;
    if (_timefacter<=0 || _timelimit<=0)
        return _bufftime;
    
    float value1 = (herodata->getCurrStdAtr(mainAttr)/targetdata->getCurrStdAtr(mainAttr))*_timefacter;
    value1 = MIN(value1,_timelimit);
    float value = value1+_bufftime;
    
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL  || _DEBUG_TYPE==_DEBUG_TYPE_VALUE){
        CCLOG(" 技能Debuff持续时间的结果计算（5号公式）） ");
        CCLOG("  N0.5) (herodata->getCurrStdAtr(mainAttr)/targetdata->getCurrStdAtr(mainAttr))*_timefacter");
        CCLOG("   _timefacter %.3f _maxlimit %.3f", _timefacter, _timelimit);
        CCLOG("   value1 =  (mainAttr1 %.3f / mainAttr2 %.3f) *_factor %.3f = %.3f", herodata->getCurrStdAtr(mainAttr)*0.01, targetdata->getCurrStdAtr(mainAttr)*0.01, _timefacter, value1);
        CCLOG("   value = %.3f + %.3f", value1, _bufftime);
    }
    
    return value;
}

//效果计算公式  6、Buff持续时间的结果计算
// buff:
// y=BuffTime+Min(DominantAttribute施法者*timefacter, timelimit)
float LQEffectUserData::calculatorBuffTime(LQAttackDataInterface* herodata, int mainAttr)
{
    float _timefacter = this->getCfgdata()->getTimeFacter()/RATE_PERCENT;   //输出系数
    float _timelimit = this->getCfgdata()->getTimelimit()/RATE_PERCENT;     //输出上限校正系数
    float _bufftime = this->getCfgdata()->getBuffTime()/RATE_PERCENT;
    if (_timefacter<=0 || _timelimit<=0)
        return _bufftime;
    
    float value1 = herodata->getCurrStdAtr(mainAttr)*_timefacter;
    value1 = MIN(value1,_timelimit);
    float value = value1+_bufftime;
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL  || _DEBUG_TYPE==_DEBUG_TYPE_VALUE){
        CCLOG(" 技能Buff持续时间的结果计算（6号公式）） ");
        CCLOG("  N0.6) herodata->getCurrStdAtr(mainAttr)*_timefacter");
        CCLOG("   _timefacter %.3f _maxlimit %.3f", _timefacter, _timelimit);
        CCLOG("   value1 = mainAttr1 %.3f *_factor %.3f = %.3f", herodata->getCurrStdAtr(mainAttr)*0.01, _timefacter, value1);
        CCLOG("   value = %.3f + %.3f", value1, _bufftime);
    }
    
    return value;
}


/*
//效果计算公式  1、普通伤害 2、暴击伤害
//效果计算公式  7、Debuff的属性伤害计算
//1	技能伤害=Min（Max（（DominantAttribute攻+a）/（DominantAttribute防+b），EffectBase）,MaxLimit）*EffectFactor*基本伤害输出	瞬间伤害公式
//2	技能伤害=Min（Max（（DominantAttribute攻+a）/（DominantAttribute防+b），EffectBase）,MaxLimit）*EffectFactor*暴击伤害输出	暴击伤害公式
void LQEffectUserData::calculatorValue(GameObj* hero, GameObjectVector* targetObjects, LQAttackDataInterface* herodata, LQAttackDataInterface* squadata, int mainAttr, int sign)
{
    int _outputType = this->getCfgdata()->getOutputType();            //输出类型
    //if(_outputType!=15)  //生命
    //    CCLOG("CalculatorValue OutputType %d is ERROR.", _outputType);
    float _basevalue = this->getCfgdata()->getEffectBase()/RATE_PERCENT;  //校准值
    float _factor = this->getCfgdata()->getEffectFactor()/RATE_PERCENT;   //输出系数
    int aparam = 0;
    int bparam = 0;
    float hurt1 = 0;
    LQAttackCalculator* targetdata;
    for (GameObj* targetobj : *targetObjects ) {
        targetdata = (LQAttackCalculator*)targetobj->getCalculator();
        //if (hero->getIndex()==80)
        //    CCLOG("hero->getIndex()");
        hurt1 = ( (herodata->getCurrStdAtr(mainAttr)+aparam)/(targetdata->getCurrStdAtr(mainAttr)+bparam) );
        hurt1 = MAX(hurt1,_basevalue);
        hurt1 *= _factor;
        hurt1 *= sign;  //增益和减益
        if (this->getFormulaType()==2){  //强制暴击
            herodata->setIsMustCrit();
        }
        if (targetdata->getAttackObjectType()==Combat_AttackObjectType_Hero)
           hurt1 *= herodata->getCurrentDPS(targetdata);
        else
           hurt1 *= squadata->getCurrentDPS(targetdata);
        
        if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
            CCLOG(" 技能伤害 %d.... hurt1 = %0.3f", hero->getIndex(), hurt1);
        
        if(_outputType==ATTR_HP)  //生命 UnitType == 1 公式1,2
            hero->changeTargetDps (targetobj, -hurt1, klq_flaytype_skill);  //伤害符号是反着的
        else {  //生命 UnitType == 2 公式7
            //目标属性发生变化
            float _bufftime = this->getCfgdata()->getBuffTime()/RATE_PERCENT;  //持续时间
            LQBuffUserData* newbuf = new LQBuffUserData(hero, this, _bufftime, hurt1);
            targetobj->addBuffer( newbuf );
        }
    }
    CC_SAFE_DELETE(targetObjects);
    //CCLOG(" Hero::主动技能[%s] 伤害 公式类型：%d  hurt = %.3f", skillUserData->getName().c_str(), this->getFormulaType(), hurt1);
}

//效果计算公式  3、属性变动计算
//3	属性变动值=Min（Max（DominantAttribute施法者*EffectFactor，EffectBase），MaxLimit）*OutType属性值
//每点施法者属性转换*系数
void LQEffectUserData::calculatorPropValue(GameObj* hero, GameObjectVector* targetObjects, LQAttackDataInterface* herodata, int mainAttr,int sign)
{
    int _outputType = this->getCfgdata()->getOutputType();            //输出类型
    float _basevalue = this->getCfgdata()->getEffectBase()/RATE_PERCENT;  //校准值
    float _factor = this->getCfgdata()->getEffectFactor()/RATE_PERCENT;   //输出系数
    float _maxlimit = this->getCfgdata()->getMaxLimit()/RATE_PERCENT;     //输出上限校正系数
    int value1 = 0;
    LQAttackDataInterface* targetdata;
    for (GameObj* targetobj : *targetObjects ) {
        targetdata = targetobj->getCalculator();
        value1 = MIN( MAX(herodata->getCurrStdAtr(mainAttr)*_factor,_basevalue), _maxlimit);
        value1 *= targetdata->getCurrStdAtr(_outputType);
        value1 *= sign;  //增益和减益
        //目标属性发生变化
        value1 += targetdata->getCurrStdAtr(_outputType);
        targetdata->setCurrStdAtr(_outputType, value1);
        //targetdata->addCurrStdAtr(_outputType, value1);
    }
}

//4	"Buff对属性改变的结果=Min（Max（DominantAttribute施法者*EffectFactor，EffectBase），MaxLimit）*BuffValue
//Buff持续时间=BuffTime"	Buff属性改变=每点施法者属性转换*系数，持续时间是固定值
int LQEffectUserData::calculatorBuffValue(LQAttackDataInterface* herodata, int mainAttr)
{
    float _buffvalue = this->getCfgdata()->getBuffValue()/RATE_PERCENT;   //输出类型
    float _basevalue = this->getCfgdata()->getEffectBase()/RATE_PERCENT;  //校准值
    float _factor = this->getCfgdata()->getEffectFactor()/RATE_PERCENT;   //输出系数
    float _maxlimit = this->getCfgdata()->getMaxLimit()/RATE_PERCENT;     //输出上限校正系数
    int value1 = MIN( MAX(herodata->getCurrStdAtr(mainAttr)*_factor,_basevalue), _maxlimit)*_buffvalue;
    
    return value1;
}

//5	"Buff持续时间=BuffTime+Min（Max（DominantAttribute施法者*EffectFactor，EffectBase），MaxLimit）
//Buff对属性改变的结果=OutType受攻击者属性值*BuffValue"	每x点属性，Buff、Debuff时间增加1秒，有上限，Buff、Debuff按固定百分比对目标作用
float LQEffectUserData::calculatorBuffTime(LQAttackDataInterface* herodata, int mainAttr)
{
    float _bufftime = this->getCfgdata()->getBuffTime()/RATE_PERCENT;     //Buff·Debuff持续时间基值（万分之一秒）
    float _basevalue = this->getCfgdata()->getEffectBase()/RATE_PERCENT;  //校准值
    float _factor = this->getCfgdata()->getEffectFactor()/RATE_PERCENT;   //输出系数
    float _maxlimit = this->getCfgdata()->getMaxLimit()/RATE_PERCENT;     //输出上限校正系数
    float value1 = _bufftime + MIN( MAX(herodata->getCurrStdAtr(mainAttr)*_factor,_basevalue), _maxlimit);
    
    return value1;
}
 */
