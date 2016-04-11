//
//  LQHeroUserData.cpp
//  LostEmpire
//
//  Created by lequdev1 on 14-1-24.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include "LQHeroUserData.h"
#include "LQData.h"

//soldierIDC<=0读取默认的兵种数据（怪物组使用）
LQHeroUserData::LQHeroUserData(int resIDC, int soldierIDC, int count):m_soldierIDC(soldierIDC), m_soldiers(NULL), m_soldierCount(count), m_armsCfgdata(NULL),m_passiveSkillID(0),m_nodeParent(NULL),m_squadData(NULL)
{
    m_index = -1;
    m_stopSkill = false;
    _skillData = NULL;
    _skillData0 = NULL;
    _skillData1 = NULL;
    _skillData2 = NULL;
    m_stuntEffect = -1;
    aliasName = "CardInfo";
    m_soldiers = new LQArmsUserDataVector();
    m_formationIndex = -1;
    m_dpsTotal = 0;     // 攻击的伤害统计
    m_killTotal = 0;    // 杀死敌人数量统计
    //m_heroFuries= 0;
    m_star = 0;
    m_assaultStaus = false;
    m_soldierRestrain = "";
    this->initWithIDC(resIDC);
    //CCLOG("LQHeroUserData %p new IDC:%d", this, resIDC);
}

LQHeroUserData::~LQHeroUserData()
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SAFE) CCLOG("~LQHeroUserData %p", this);
    if (m_soldiers) {
        deleteInVector(*m_soldiers);
    }
    CC_SAFE_DELETE(m_soldiers);
    
    CC_SAFE_DELETE(_skillData);
    CC_SAFE_DELETE(_skillData0);
    CC_SAFE_DELETE(_skillData1);
    CC_SAFE_DELETE(_skillData2);
}

// 初始化数据
bool LQHeroUserData::init()
{
    if(!LQArmsUserData::init())
        return false;
    
    //CCX_PROPERTY_IMPLEMENT_INT_INIT(level,m_level)
    //CCX_PROPERTY_IMPLEMENT_STR_INIT(cityid,m_cityId)
    
    return true;
}

LQArmsInfoData* LQHeroUserData::getArmsCfgdata()
{
    return this->m_armsCfgdata;
}

// 返回配置数据子类覆盖
LQCardInfoData* LQHeroUserData::getCardCfgdata()
{
    return (LQCardInfoData*)this->m_cfgdata;
}

//子类重写
const char* LQHeroUserData::getAliasName()
{
    return this->aliasName;
}

// 初始化数据,子类重写
void LQHeroUserData::putData()
{
    LQArmsUserData::putData();
    
    if(!this->getCfgdata())
        return;
    
    string m_herDefAttr;
    this->getCfgdata()->getValue("HeroDefAtr", &m_herDefAttr);
    this->splitValue(m_herDefAttr, ';', ',', true);
    m_heroHp = m_attrMap[ intToString(ATTR_HEROHP).c_str() ].asInt();	//生命
    m_heroAtk = m_attrMap[ intToString(ATTR_HEROATK).c_str() ].asInt();	//攻击
    m_heroDef = m_attrMap[ intToString(ATTR_HERODEF).c_str() ].asInt();	//防御
    
    this->getCardCfgdata()->getValue("DefAtr", &m_herDefAttr);
    this->splitValue( m_herDefAttr );
    
    this->getCardCfgdata()->getValue("NameID", &m_name);          //卡牌名称
    this->getCardCfgdata()->getValue("ShortName", &m_shortname);     //卡牌名称
    this->getCardCfgdata()->getValue("HeadID", &m_headID);      //卡牌将领的头像ID
    this->getCardCfgdata()->getValue("CardID", &m_cardID);      //卡牌将领的卡牌ID
    this->getCardCfgdata()->getValue("Quality", &m_quality);    //卡牌将领的品质
    this->getCardCfgdata()->getValue("Occupation", &m_occupation); //武将职业\r\n1、步将\r\n2、骑将\r\n3、弓将\r\n4、策士v
    this->getCardCfgdata()->getValue("DefaultSoldier", &m_defaultSoldier); 	//初始化携带兵种
    this->getCardCfgdata()->getValue("ActiveSkillID", &m_activeSkillID);        //主动技能
    this->getCardCfgdata()->getValue("PassiveSkill1ID", &m_passiveSkill1ID);    //被动技能1
    this->getCardCfgdata()->getValue("PassiveSkill2ID", &m_passiveSkill2ID);    //被动技能2
    this->getCardCfgdata()->getValue("TacticianSkillID", &m_tacticianSkillID);  //被动技能2
    this->getCardCfgdata()->getValue("StuntEffect", &m_stuntEffect);  //武将释放技能时强化效果的特效
    int spatkFacter;
    this->getCardCfgdata()->getValue("SpatkFacter", &spatkFacter);  //冲锋、迎击的基础伤害（无视对手防御力）
    //m_spatkFacter = val*0.0001;
    this->setCurrStdAtr(ATTR_SPATKFAC, spatkFacter);
    
    
    int heroFuries;
    this->getCardCfgdata()->getValue("RageMin", &m_rageMin);  //冲锋、迎击的基础伤害（无视对手防御力）
    this->getCardCfgdata()->getValue("RageDefault", &heroFuries);         //初始化怒气
    this->setCurrStdAtr(ATTR_RAGE, heroFuries);
    
    this->getCardCfgdata()->getValue("StarLevel", &m_star);            //初始化星级
    this->getCardCfgdata()->getValue("BattlePower", &m_power);         //初始化战斗力
    
    this->getCardCfgdata()->getValue("AtkSound", &m_atkSound);
    this->getCardCfgdata()->getValue("AtkSNum", &m_atkSNum);
    this->getCardCfgdata()->getValue("DthSound", &m_dthSound);
    this->getCardCfgdata()->getValue("DthSNum", &m_dthSNum);
    
    this->getCardCfgdata()->getValue("SoldierRestrain", &m_soldierRestrain);
    
    this->getCardCfgdata()->getValue("RageBeAttack", &m_rageBeAttack);  //每次被攻击怒气
    this->getCardCfgdata()->getValue("RageAttack", &m_rageAttack);      //每次攻击怒气
    //m_rageBeAttack = m_rageBeAttack*0.7;
    //m_rageAttack = m_rageAttack*0.7;
    this->getCardCfgdata()->getValue("ArmsType", &m_armsType);          //军团兵种\r\n1、步兵\r\n2、远程\r\n3、骑兵\r\n4、机械
    
    m_speed = getCardCfgdata()->getSpeed()*0.001f;
    m_patrolSpeed = getCardCfgdata()->getPatrolSpeed();
    
    if(m_soldierIDC<=0){
        m_soldierIDC = this->getCardCfgdata()->getDefaultSoldier();
    }
    
    if(m_soldierIDC>0){
        m_armsCfgdata = (LQArmsInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQArmsInfoData::getAliasName(),m_soldierIDC);
        m_flashID = m_armsCfgdata->getFlashID().c_str();
    }
    
    //初始数据
    this->defaultData();
    //计算类型
    m_attackObjtype = Combat_AttackObjectType_Hero;
    m_assaultStaus = m_armsType==Combat_ArmsType_Cavalry;
    m_isMuseCrit = m_armsType==Combat_ArmsType_Cavalry;
}

// 当数据都传入后，分配和计算数值
void LQHeroUserData::calcData()
{
    _LQDEBUG("Hero:%d %d", m_resIDC, m_soldierIDC);
    
    m_forValue = m_attrMap[ intToString(ATTR_FOR).c_str() ].asInt();	//武勇
    m_intValue = m_attrMap[ intToString(ATTR_INT).c_str() ].asInt();	//智力
    m_comValue = m_attrMap[ intToString(ATTR_COM).c_str() ].asInt();	//统率
    m_sHpValue = m_attrMap[ intToString(ATTR_HP).c_str() ].asInt();     //生命
    m_sApValue = m_attrMap[ intToString(ATTR_ATK).c_str() ].asInt();	//攻击
    m_sDpValue = m_attrMap[ intToString(ATTR_DEF).c_str() ].asInt();	//防御
    //if (m_sHpValue>0 && m_sHpValue<500)
    //    m_sHpValue = 500;
    
    switch (m_quality) {
        case 1:
            m_qualityId = 1;
            break;
        case 2:
        case 3:
            m_qualityId = 2;
            break;
        case 4:
        case 5:
        case 6:
            m_qualityId = 3;
            break;
        case 7:
        case 8:
        case 9:
            m_qualityId = 4;
            break;
        case 10:
            m_qualityId = 5;
            break;
        default:
            m_qualityId = 1;
            break;
    }
    
    //武将生命=武将兵力*兵力折算系数
    //武将普通攻击的有效攻击值 = 武将武勇*武勇折算系数
    //武将的有效防御 = 武将统率*统率折算系数
    float aparam = getFormulaParamFactor(26,"a");  //
    float bparam = getFormulaParamFactor(26,"b");  //
    float cparam = getFormulaParamFactor(26,"c");  //
    int sumfic = m_forValue+m_intValue+m_comValue-30000;
    int _HpValue = m_heroHp + (sumfic<=0 ? 0 : (sumfic/10000*aparam));   //生命
    int m_ApValue = m_heroAtk+(sumfic<=0 ? 0 : (sumfic/10000*bparam));//攻击
    int m_DpValue = m_heroDef+(sumfic<=0 ? 0 : (sumfic/10000*cparam));//防御
    setCurrStdAtr( ATTR_HEROHP, _HpValue);  //生命
    setCurrStdAtr( ATTR_HEROATK, m_ApValue); //攻击
    setCurrStdAtr( ATTR_HERODEF, m_DpValue); //防御
    m_HpValue = _HpValue;
    if( getCurrStdAtr(ATTR_SQUEHP)<0 )
        setCurrStdAtr( ATTR_SQUEHP, m_sHpValue);  //军团生命
    
    //兵种生命 = 武将兵力 * 兵力折算系数 + 生命基值+生命成长*（武将等级 - 1）+其他系统养成生命
    //兵种的有效防御 = 武将统率*统率折算系数 + 防御基值 + 防御步长*（武将等级-1）++其他系统养成防御
    //兵种普通攻击的有效攻击值＝武将武勇*武勇折算系数+攻击基值+攻击步长*（武将等级 - 1）++其他系统养成攻击
    //********后端传入数据**********
    int _squeHp = getCurrStdAtr(ATTR_SQUEHP);
    if (m_soldiers) {
        deleteInVector(*m_soldiers);
    }
    if( m_soldierIDC>0 && m_soldierCount>0 && _squeHp>0) {
        m_defaultSoldierHP = m_soldierCount==0 ? 0: m_sHpValue/m_soldierCount;	//生命
        m_defaultSoldierAP = m_soldierCount==0 ? 0: m_sApValue/m_soldierCount;	//攻击
        m_defaultSoldierDP = m_soldierCount==0 ? 0: m_sDpValue/m_soldierCount; //防御
        m_soldierCount = _squeHp/m_defaultSoldierHP;
        if(m_soldierCount<15)
            CCLOG("m_soldierCount:%d",m_soldierCount);
        if(m_soldierCount<=0) m_soldierCount = 1;
        m_defaultSoldierHP = _squeHp/m_soldierCount;
        //创建单兵数据
        int i=0;
        //m_soldierCount = 0;
        while(i<m_soldierCount)
        {
            if(m_soldierIDC==90001020 && i%3==2)  //象兵
            {
                i++;
                continue;
            }
            LQArmsUserData* m_soldier = new LQArmsUserData();
            m_soldier->initWithIDC(m_soldierIDC);
            //m_soldier->setHeroData(this);
            m_soldier->setSquadDirection(m_squadDirection);
            m_soldier->setPatrolSpeed(m_patrolSpeed);  //巡逻速度为武将的巡逻速度
            m_soldier->setArmsType(m_armsType);
            m_soldier->setOccupation(m_occupation);
            m_soldier->setHpValue(m_defaultSoldierHP);

            
            m_soldierUnitType = LQCombatUnitType(m_soldier->getUnitType());
            m_soldiers->push_back(m_soldier);
            i++;
        }
    }
    
    m_furyMaxValue = getCardCfgdata()->getRageMaxDefault();
    int _rageRate = m_attrMap[ intToString(ATTR_RAGERATE).c_str() ].asInt();	//怒气%
    if(_rageRate>0){
        int _rage = this->getCurrStdAtr(ATTR_RAGE);
        _rage += this->getCurrStdAtr(ATTR_RAGEMAX)*(_rageRate/RATE_PERCENT);
        this->setCurrStdAtr(ATTR_RAGE,_rage);
    }    
    //m_armyTotalHP =m_HpValue+m_sHpValue;//军团初始时候的总生命，用来计算怒气时时用
    
    _LQDEBUG("LQHeroUserData Hero [%d] HpValue: %d", m_resIDC, m_HpValue);
    _LQDEBUG("               Soldier   HpValue: %d", m_defaultSoldierHP);
    
    //技能数据
    if(m_activeSkillID>0){
        _skillData = new LQSkillUserData(m_activeSkillID);
    }
    
    if(m_passiveSkill1ID>0){
        _skillData1 = new LQSkillUserData(m_passiveSkill1ID,klqSkillShowType_Passive);
    }
    
    if(m_passiveSkill2ID>0){
        _skillData2 = new LQSkillUserData(m_passiveSkill2ID,klqSkillShowType_Passive);
    }
    
    if(m_tacticianSkillID>0){
        _skillData0 = new LQSkillUserData(m_tacticianSkillID,klqSkillShowType_Tactician);
    }
    
    return;
}

string LQHeroUserData::getAtkMusic()
{
    if (m_atkSound == "" ) {  //|| m_atkSNum<=0
        return "";
    }
    
//    int index = rand()%m_atkSNum + 1;
//    string atkmusic;
//    stringf(atkmusic, m_atkSound.c_str(), index);
    
    string filePath;
    stringf(filePath, "res/audio/%s", m_atkSound.c_str());
    return filePath;
}

int LQHeroUserData::getCurrHpValue()
{
    return getSquadHpValue();
}

//当前军团血量
int LQHeroUserData::getSquadHpValue()
{
    if(!m_squadData)
        return 0;
    return m_squadData->getCurrHpValue();
}

//当前军团伤血量
int LQHeroUserData::getSquadHurtValue()
{
    if(!m_squadData)
        return 0;
    return m_squadData->getHpMaxValue()-m_squadData->getCurrHpValue();
}

//本场当前军团伤血量
int LQHeroUserData::getSquadCurrHurtValue()
{
    if(!m_squadData)
        return 0;
    return this->getCurrSoldHpValue()-m_squadData->getCurrHpValue();
}

//变更生命HP
void LQHeroUserData::changeHpValue(int value)
{
    //if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
    //    CCLOG("%d changeHpValue m_HpValue %d + value %d = %d", m_resIDC, m_HpValue, value, m_HpValue+value);
    setHpValue(m_HpValue+value);
}

//当前总生命
int LQHeroUserData::getHpMaxValue()
{
    return this->getCurrStdAtr(ATTR_SQUEHP);
}

//当前攻击
int LQHeroUserData::getCurrApValue()
{
    return this->getCurrStdAtr(ATTR_HEROATK);
}

//当前防御
int LQHeroUserData::getCurrDpValue()
{
    return this->getCurrStdAtr(ATTR_HERODEF);
}

//兵的完整生命值，用于BOSS
int LQHeroUserData::getCurrSoldHpValue()
{
    int val = this->getCurrStdAtr(ATTR_SQUEHP);
    if (val<=0)
        val = this->getSoldHpValue();
    return val;
}

string LQHeroUserData::getDthMusic()
{
    if (m_dthSound == "" ) {  //|| m_dthSNum<=0
        return "";
    }
    
    //int index = rand()%m_dthSNum + 1;
    //string dthmusic;
    //stringf(dthmusic, m_dthSound.c_str(), index);
    string filePath;
    stringf(filePath, "res/audio/%s", m_dthSound.c_str());
    return filePath;
}

//变更英雄伤害统计
void LQHeroUserData::changeHeroDpsTotal(int value)
{
    if(value>=0)
        return;
    
    this->setDpsTotal(m_dpsTotal-value);
}

//变更Kill
void LQHeroUserData::changeKillTotal(int value)
{
    this->setKillTotal(m_killTotal+value);
}

//变更怒气 最大不得超过最大怒气上限
void LQHeroUserData::changeHeroFuries(int value)
{
    //if (value <= 0)
    //    return;
    int m_heroFuries = getHeroFuries();
    //if (m_resIDC==90042087)
    //    CCLOG("%p herodata->changeHeroFuries = %d + %d", this, m_heroFuries, value);
    setHeroFuries(m_heroFuries + value);
    
}

//增涨怒气比率
void LQHeroUserData::addHeroFuriesRadio(float radio)
{
    int m_heroFuries = getHeroFuries();
    setHeroFuries(m_heroFuries + m_furyMaxValue*radio);
}

/*获取此次攻击被激活的被动技能
 当skillid 为0 时，为普通攻击*/
int LQHeroUserData::getPassiveSkillID()
{
    if (m_passiveSkillID > 0) {
        m_passiveSkillID = 0;
        return  0;
    }
    m_passiveSkillID = 0;

    return m_passiveSkillID;
}

//是否先手技能
bool LQHeroUserData::isFirstSkill()
{
    if (!_skillData)
        return false;
    return _skillData->isFirstSkill(m_dttime);
}

//是否可以释放主动技能
bool LQHeroUserData::isCanFreeActiveSkill()
{
    if (m_activeSkillID <= 0 || m_stopSkill) {
        return false;
    }
    //CCLOG("%p herodata->isCanFreeActiveSkill = %d",this, m_heroFuries);
    return m_furyMaxValue <= this->getHeroFuries();
}

//是否可以释放被动技能
bool LQHeroUserData::isCanFreePassiveSkill(LQSkillUserData* passiveSkill)
{
    if (passiveSkill && m_stopSkill) {
        return false;
    }
    //现在只判断出手时间
    return passiveSkill->isFirstSkill(m_dttime);
}

//设置初始值，我的英雄将由相同传入
void LQHeroUserData::defaultData()
{
    //
    m_attrMap[ intToString(ATTR_SQUEHP).c_str() ] = -1;
}
