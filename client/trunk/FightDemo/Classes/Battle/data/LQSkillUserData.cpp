//
//  LQSkillUserData.cpp
//  SuperWarriors
//
//  Created by lequdev1 on 14-7-14.
//
//

#include "LQSkillUserData.h"
#include "LQCore.h"
#include "LQData.h"

LQSkillUserData::LQSkillUserData(int resIDC, KLQSkillType skillType)
{
    aliasName = "SkillInfo";
    m_time = 0;
    m_skillType = skillType;
    m_effectDatas = new LQEffectUserDataVector();
    this->initWithIDC(resIDC);
}

LQSkillUserData::~LQSkillUserData()
{
    if (m_effectDatas) {
        deleteInVector(*m_effectDatas);
    }
    CC_SAFE_DELETE(m_effectDatas);
}

// 初始化数据
bool LQSkillUserData::init()
{
    if(!LQBaseUserData::init())
        return false;
    
    return true;
}

LQSkillInfoData* LQSkillUserData::getSkillCfgData()
{
    return (LQSkillInfoData*)this->m_cfgdata;
}

// 返回配置数据子类覆盖
//LQAnimationInfoData* LQSkillUserData::getAnimationCfgData()
//{
//    return (LQAnimationInfoData*)this->m_animationCfgData;
//}

//子类重写
const char* LQSkillUserData::getAliasName()
{
    return this->aliasName;
}

// 初始化数据,子类重写
void LQSkillUserData::putData()
{
    m_mainEffectData = NULL;
    //m_animationCfgData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), m_hitEffect);
    
    m_name = getSkillCfgData()->getName();
    m_duplication = 0;//m_animationCfgData->getDuplication();//技能表中得重复次数
    m_radius = 0;
    m_count = 0;
    m_skillDistance = 0;
    m_mutiEffectType = getSkillCfgData()->getMutiEffectType();
    m_isDoPassive = false;
    
    m_count = 1;
    //将技能效果加载，现在最多有4个技能效果
    int effectCount = 4;
    int _objectType;
    int _effectUnit;
    for (int i =1; i<=effectCount; i++) {
        char* str1 = new char[100];
        sprintf(str1, "ObjectType%d", i);
        this->getCfgdata()->getValue(str1, &_objectType);
        char* str2 = new char[100];
        sprintf(str2, "EffectUnit%d", i);
        this->getCfgdata()->getValue(str2, &_effectUnit);
        if(_objectType>0 && _effectUnit>0){
            //效果
            LQEffectUserData* effectUserData = new LQEffectUserData(_effectUnit, _objectType);
            if(effectUserData->getCfgdata()->getIsEffective()>=0){
                m_effectDatas->push_back(effectUserData);
                if(m_mainEffectData == NULL)
                    m_mainEffectData = effectUserData;
                effectUserData->setSkillInfo(getSkillCfgData());
            }
            else CC_SAFE_DELETE(effectUserData);  //无效的数据
        }
        delete [] str1;  str1 = NULL;
        delete [] str2;  str2 = NULL;
    }
    
    restore();
}

//获取瞬时伤害
int LQSkillUserData::getPMPOValue(LQHeroUserData* heroData)
{
    return 0;
}

//是否先手可放技能，设置了起手时间，PVE敌人使用
bool LQSkillUserData::isFirstSkill(float time)
{
    float rtime = this->getSkillCfgData()->getReleaseTime()*0.001;
    //if (m_skillType==klqSkillShowType_Passive)
    //    rtime = 1;
    if (rtime>0)  //存在先手技能 起手技能时间，只有在没有目标的冲锋有效
    {
        //CCLOG("isFirstSkill %.3f %.3f",time,rtime);
        if (time>rtime)
            return true;
        return false;
    }
    
    if (m_skillType!=klqSkillShowType_ActiveSkill)
        return true;
    //自动技能释放需要存在针对敌人的效果单元
    for(LQEffectUserData* effectdata : *m_effectDatas)
    {
        int n = effectdata->getObjectType()%2;
        if (n==0)
            return true;
    }
    return false;
}

//
void LQSkillUserData::restore()
{
    m_time = 0;
    _isDoSkillReady = false; //技能施放特效中段
    _isDoSkillEnd = false;   //技能施放特效结束
    _isDoHitReady = false; //技能命中特效中段
    _isDoHitEnd = false;   //技能命中特效结束，技能结束
    
    //用于表示施放特效结束的时间，现在延生到命中特效开始时（攻击特效结束 或者 弹道命中）
    m_skillOutEndTime = 0.2;// + (m_missileType>=0 ? 0.2 : 0.8);
    m_hitOutputTime = m_skillOutEndTime + 0.8;    //用于命中特效的时间
    m_skillLastTime = m_hitOutputTime + 0.2;      //整个技能动作的时间
    
    m_isSkillEnd = false;   //技能施放特效结束
    m_isHitReady = false; //技能命中特效中段
    m_isHitEnd = false;   //技能命中特效结束，技能结束
    
    //CCLOG("new LQSkillUserData %ld id:%d%s  %.3f %.3f %.3f %.3f", getCurrentMillSecond(), m_resIDC , m_name.c_str(), m_skillOutputTime, m_skillOutEndTime, m_hitOutputTime, m_skillLastTime);
}

//在update中计算技能的持续时间
void LQSkillUserData::update(float dt)
{
    m_time += dt;
    
    if (m_time >= m_skillLastTime && !_isDoHitEnd) {
        m_isHitEnd = true;
        _isDoHitEnd = true;
    }else if (m_time >= m_hitOutputTime && !_isDoHitReady) {
        _isDoHitReady = true;
        m_isHitReady = true;
    }else if (m_time >= m_skillOutEndTime && !_isDoSkillEnd) {
        m_isSkillEnd = true;
        _isDoSkillEnd = true;
    }
}
