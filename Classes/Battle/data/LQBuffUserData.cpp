//
//  LQBuffUserData.cpp
//  SuperWarriors
//
//  Created by lequdev1 on 14-7-15.
//
//

#include "LQBuffUserData.h"

LQBuffUserData::LQBuffUserData(GameObj* hero, LQEffectUserData* effect, float bufftime, float buffvalue):m_hero(hero), m_target(NULL), onBufferCallback(nullptr), onBufferEndTime(nullptr)
{
    aliasName = "EffectInfo";
    m_objectType = effect->getObjectType();
    m_outputType = effect->getCfgdata()->getOutputType();
    m_dottime = effect->getCfgdata()->getDotTime()/RATE_PERCENT;
    m_splashArea = effect->getCfgdata()->getSplashArea();        //溅射范围
    m_priority = 0;
    m_scalefactor = 0;
    m_bufftime = bufftime;      //特效BuffTime
    m_buffvalue = buffvalue;	//特效BuffValue
    m_dotvalue = buffvalue;
    m_flashicon = "";
    m_buffDelayTime = 0;
    m_buffIntervelDelayTime = 0;
    m_active = true;
    m_tagId  = Buff_Tag_Id++;
    
    initWithData(effect->getCfgdata());
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
        CCLOG("new LQBuffUserData second:%f  id:%d  buf %.3f dot %.3f val %.3f", getCurrentMillSecond(), m_resIDC , m_bufftime, m_dottime, m_buffvalue);
}

LQBuffUserData::~LQBuffUserData()
{
    //CCLOG("free LQBuffUserData %p", this);
}
// 初始化数据,子类重写
const char* LQBuffUserData::getAliasName()
{
    return aliasName;
}

// 返回配置数据子类覆盖
LQEffectInfoData*  LQBuffUserData::getCfgdata()
{
    return (LQEffectInfoData*)m_cfgdata;
}

// 返回特效配置数据
LQAnimationInfoData* LQBuffUserData::getAnimationCfgData()
{
    return (LQAnimationInfoData*)this->m_animationCfgData;
}

// 返回FlashID图标
string LQBuffUserData::getFlashID()
{
    return m_flashicon;
}

// 返回Scalefactor
float LQBuffUserData::getScalefactor()
{
    return m_scalefactor;
}

// 返回Buff图标
string LQBuffUserData::getBufferIcon()
{
    return m_buffericon;
}

// 返回Buff旗帜上的图标
string LQBuffUserData::getFlagsIcon()
{
    return m_flagsicon;
}

// 初始化数据,子类重写
void LQBuffUserData::putData()
{
    m_isdot = false;
    m_isDoEffect = false;
    m_buffType = getCfgdata()->getBuffSubType();
    m_animationType = (KLQAnimationDisplayType)getCfgdata()->getAniDisplayType();
    m_animationID = getCfgdata()->getAnimationID();
    m_priority = getCfgdata()->getPriority();
    m_buffFlag = getCfgdata()->getBuffFlag();
    m_dominantAttribute = getCfgdata()->getDominantAttribute();
    m_isdot = (m_buffType==5|| m_buffType==6 || m_buffType==15|| m_buffType==16|| m_buffType==56|| m_buffType==66) && m_dottime>0;
    if (m_isdot)
        m_buffvalue = 0;
    m_animationCfgData = NULL;
    m_scalefactor = getCfgdata()->getScaleFactor()*0.001;
    m_flashicon = getCfgdata()->getFlashID();
    m_flagsicon = "";
    if(getCfgdata()->getSquadStatusID()>0)
        stringf(m_flagsicon, "iconf/%d.png", getCfgdata()->getSquadStatusID());   //40021001.png
    //获取特效数据
    if(m_animationID>0){
        if(m_animationType==klqAnimationDisplayTypeAnimation){  //动画
            m_animationCfgData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), m_animationID);
            m_effectPosition = PointFromString(m_animationCfgData->getPosition());
        }else if(m_animationType==klqAnimationDisplayTypeIcon){  //图标
            stringf(m_buffericon, "iconh/%d.png", m_animationID);   //40021001.png
        }else if(m_animationType==klqAnimationDisplayTypeBmp){  //图标
            stringf(m_buffericon, "iconb/%d.png", m_animationID);  //40021001b.png
        }else if(m_animationType==klqAnimationDisplayTypeEffect){  //特效
            m_animationCfgData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), m_animationID);
            m_effectPosition = PointFromString(m_animationCfgData->getPosition());
        }
    }
}

// update
void LQBuffUserData::update(float dt)
{
    if(!m_active)
        return;
    m_buffDelayTime += dt;
    m_buffIntervelDelayTime += dt;
    //CCLOG("%p BuffUserData::update.... %.3f; %.3f", this, m_buffDelayTime, m_buffIntervelDelayTime);
    
    if( m_isdot && m_buffIntervelDelayTime>m_dottime)  //触发DOT技能效果
    {
        this->doBufferCallback();
        m_buffvalue += m_dotvalue;  //累计DOT Value
    }else if( m_buffType!=1 && m_buffType!=2 && !m_isDoEffect )  //除了属性加减值后，其他都是一次触发
    {
        m_isDoEffect = true;
        this->doBufferCallback();
    }
    
    if(m_buffDelayTime>m_bufftime)
        this->doBufferEndTime();
}

//打断BUFF
void LQBuffUserData::isBreak()
{
    m_buffDelayTime = m_bufftime+0.01;
}

void LQBuffUserData::doBufferCallback()
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
        CCLOG("%p BuffUserData::Callback.... ", this);
    if (this->onBufferCallback)
        this->onBufferCallback(this);
    m_buffIntervelDelayTime = 0;
}

void LQBuffUserData::doBufferEndTime()
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
        CCLOG("%p BuffUserData::endtime.... ", this);
    m_active = false;
    if (this->onBufferEndTime)
        this->onBufferEndTime(this);
}
