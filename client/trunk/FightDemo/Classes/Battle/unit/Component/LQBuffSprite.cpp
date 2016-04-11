//
//  LQBuffSprite.cpp
//  LostEmpire
//
//  Created by chenli on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "LQBuffSprite.h"
#include "LQCombatHero.h"
#include "ResourceManage.h"
#include "AudioManager.h"
#include "LQSkillEffectListener.h"

using namespace cocos2d;
LQ_NAMESPACE

#define LQBuffActionTagNumber 500   //用于标识动作ID

LQBuffSprite::LQBuffSprite()
{
    m_ownscale = 1;
    m_time = 0;
    m_audioid = 0;
    islaoadingRes = false;
    m_sp = NULL;
    m_nextAnimationData = NULL;
    m_skillAllOutputTime = 0;
}

LQBuffSprite::~LQBuffSprite()
{
    //if (m_audioid>0)
    //    AudioManager::getInstance()->stopEffect(m_audioid);
    //onFreeResource();
    //CCLOG("~~~~~~LQBuffSprite::~LQBuffSprite  type = %d",m_showType);
}

LQBuffSprite* LQBuffSprite::createWithData(LQAnimationInfoData* data, float scale,KLQBuffShowType showType)
{
    LQBuffSprite* sp = new LQBuffSprite();
    if (sp && sp->initWithFile("res/blank.png")) {
        sp->initWithData(data,scale,showType);
        sp->autorelease();
    }else{
        CC_SAFE_DELETE(sp);
    }
    
    return sp;
}

LQBuffSprite* LQBuffSprite::createWithId(int buffid, float scale)
{
    LQBuffSprite* sp = new LQBuffSprite();
    if (sp && sp->initWithFile("res/blank.png")) {
        sp->initWithId(buffid,scale);
        sp->autorelease();
    }else{
        CC_SAFE_DELETE(sp);
    }
    
    return sp;
}

bool LQBuffSprite::initWithId(int buffid, float scale)
{
    LQAnimationInfoData* m_animationData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), buffid);
    return initWithData(m_animationData,scale);
}

bool LQBuffSprite::initWithData(LQAnimationInfoData* data,float scale,KLQBuffShowType showType)
{
    m_skillCount = 1;
    m_showType = showType;
    m_ownscale = scale;

    if(data){
        LQAnimationSprite::initWithData(data);
        if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL3 )
            CCLOG("    --LQBuffSprite::initWithData Animation:%d", data->getResID());
    }
    
    /*
    m_data = data;
    m_imageFileID = m_data->getResID();
    m_showType = showType;// (KLQBuffShowType)m_data->getShowType();
    Point desPoint = Point::ZERO;
    
    m_nextAnimation = m_data->getNextAnimation();
    m_scaleFactor = m_data->getScaleFactor()*0.001;
    m_repeatTime = m_data->getDuplication();
    m_frameInterval = m_data->getFrameInterval()*0.001;
    //上 1 中 2 下 3 4 地
    m_hangingPoint = m_data->getHangingPoint();
    m_buffAnchPosition = PointFromString(m_data->getPosition());
    this->setAnchorPoint(m_buffAnchPosition);
    m_frameReady = m_data->getFrameReady();
    m_frameEnd = m_data->getFrameEnd();
    m_frameNum = m_frameReady+m_frameEnd;
    m_allTime = -1;
    
    //可能会卡TODO
    if(m_showType == kqBuffShowType_Buff_1 || m_showType == kqBuffShowType_Buff_2)
        this->onLoadResource();
    
    string fname = "";
    LQ::stringf( fname, "%d/1.png", m_imageFileID);
    
    m_sp = Sprite::createWithSpriteFrameName(fname);
    this->setTextureRect(Rect(0,0,m_sp->getContentSize().width,m_sp->getContentSize().height));
//    //上 1 中 2 下 3 4 地
//    if (m_hangingPoint==1)
//        desPoint = Point(0, m_unitHeight);
//    else if (m_hangingPoint==2)
//        desPoint = Point(0, m_unitHeight*0.5 );
    m_sp->setPosition(desPoint+Point(this->getContentSize()*0.5));
    m_sp->setScale(m_scaleFactor);
    this->addChild(m_sp);
    
    this->setAnchorPoint(m_buffAnchPosition);*/
    
    return true;
}
/*
//主要装载资源--缓存的资源
void LQBuffSprite::onLoadResource()
{
    string filename;
    stringf(filename, "res/skill/%d.plist",m_imageFileID);
    string pvrName;
    stringf(pvrName, "res/skill/%d.pvr.ccz",m_imageFileID);
    if(_DEBUG_TYPE<10) CCLOG("Skill addResFile: %s ", filename.c_str());
    ResourceManage::getInstance()->addResFile(filename, pvrName);
    islaoadingRes = true;
}

//主要用于回收资源--缓存的资源
void LQBuffSprite::onFreeResource()
{
    if(!islaoadingRes)
        return;
    string filename;
    stringf(filename, "res/skill/%d.plist",m_imageFileID);
    string pvrName;
    stringf(pvrName, "res/skill/%d.pvr.ccz",m_imageFileID);
    ResourceManage::getInstance()->removeResFile(filename);
}

//创建系列帧动画
ActionInterval* LQBuffSprite::createAnimation()
{
    std::string numstr;
    Vector<SpriteFrame*>* allFrames = new Vector<SpriteFrame*>();
    SpriteFrameCache* sharedSFCache = SpriteFrameCache::getInstance();
    string file;
    stringf(file, "%d",m_imageFileID);
    Animation *animation = AnimationCache::getInstance()->getAnimation(file);
    
    float fps = m_frameInterval ;
    
    if (!animation) {
        string oldframe = "";
        for(int i = 1; i <= m_frameNum ; i++) {
            std::string fname;
            LQ::stringf( fname, "%d/%d.png", m_imageFileID, i );
            SpriteFrame* frame = sharedSFCache->getSpriteFrameByName(fname.c_str());
            if(!frame){
                frame = sharedSFCache->getSpriteFrameByName(oldframe.c_str());
            }else{
                oldframe = fname;
            }
            allFrames->pushBack(frame);
        }
        
        animation=Animation::createWithSpriteFrames(*allFrames,fps);
        
        AnimationCache::getInstance()->addAnimation(animation, file);
    }
    ActionInterval* animate = Animate::create(animation);
    if(m_repeatTime > 0)
    {
        animate = Repeat::create(animate, m_repeatTime);
    }
    CC_SAFE_DELETE(allFrames);
    
    
    return animate;
}
*/
//技能方向
void LQBuffSprite::setDirction(LQUnitDirectionType direction)
{
    if (direction == klqUnitAnimPhaseNone)
        return;
    
    if (m_showType == kqBuffShowType_Buff_1 || m_showType == kqBuffShowType_Buff_2 || m_showType == kqBuffShowType_Buff_3 || m_showType == KkqBuffShowType_Smoke)
    {
        this->setRotation(Def_Buff_Rotate[direction]);
    }
}

//
void LQBuffSprite::play()
{
    m_time = 0;
    m_skillOutputTime = m_frameReady * m_frameInterval * m_repeatTime;
    m_skillHitTime = m_frameHit * m_frameInterval * m_repeatTime;
    m_skillAllOutputTime = m_skillOutputTime;
    if(m_nextAnimationData!=NULL)
        m_skillAllOutputTime += m_nextAnimationData->getFrameReady() * m_frameInterval;
    
    if(m_repeatTime>0)
        m_allTime = m_frameInterval*m_frameNum * m_repeatTime + m_frameInterval*(m_skillCount - 1);
    if(!m_sp) return;
    
    if (m_shadeColor!=Color3B::WHITE)
        m_sp->setColor(m_shadeColor);
    ActionInterval *action = this->createAnimation();
    RepeatForever* forever1 = RepeatForever::create(action);
    forever1->setTag(LQBuffActionTagNumber);
    m_sp->runAction(forever1);
    
    if (m_data->getAudio() != "" && m_data->getBreakType()==3) {  //BreakType==3 是 特效持续放声音
        string filePath;
        stringf(filePath, "res/audio/%s", m_data->getAudio().c_str());
        m_audio = filePath;
        if (_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
            CCLOG("LQBuffSprite playEffect:%s", filePath.c_str());
        //m_audioid = AudioManager::getInstance()->playEffect(filePath.c_str(), true);
        auto action1 = Sequence::create(CallFunc::create([this](){
            LQBuffSprite* sp = (LQBuffSprite*)this;
            AudioManager::getInstance()->playEffect(sp->getAudio().c_str());
        }),DelayTime::create(1.0), NULL);
        this->runAction(RepeatForever::create(action1));
    }
}

//
void LQBuffSprite::play1(int dt)
{
    m_time = 0;
    float m_delayTime = dt;
    m_skillOutputTime = (m_delayTime+m_frameReady) * m_frameInterval;
    m_skillHitTime = (m_delayTime+m_frameHit) * m_frameInterval;
    m_skillAllOutputTime = m_skillOutputTime;
    if(m_nextAnimationData!=NULL)
        m_skillAllOutputTime += m_nextAnimationData->getFrameReady() * m_frameInterval;
    
    if(!m_sp) return;
    //skillOutputCallBack();
    
    ActionInterval* spAct;
    ActionInterval* spAct1 = this->createAnimation();
    auto callBack0 = CallFunc::create(CC_CALLBACK_0(LQBuffSprite::skillHitOutputCallBack,this));
    auto callBack1 = CallFunc::create(CC_CALLBACK_0(LQBuffSprite::skillOutputCallBack,this));
    ActionInterval* spAct2 = Sequence::create( DelayTime::create(m_skillHitTime), callBack0, NULL);
    ActionInterval* spAct3 = Sequence::create( DelayTime::create(m_skillOutputTime), callBack1, NULL);
    ActionInterval* spAct4 = Spawn::create(spAct2, spAct3, NULL);
    if(m_repeatTime>0) {
        m_skillOutputTime *= m_repeatTime;
        m_skillAllOutputTime = m_skillOutputTime;
        if(m_nextAnimationData!=NULL)
            m_skillAllOutputTime += m_nextAnimationData->getFrameReady() * m_frameInterval;
        Repeat* forever1 = Repeat::create(spAct1,m_repeatTime);
        spAct = Spawn::create(forever1, spAct4, NULL);
    }else
        spAct = Spawn::create(spAct1, spAct4, NULL);
    
    if (m_shadeColor!=Color3B::WHITE)
        m_sp->setColor(m_shadeColor);
    if(m_delayTime>0)
        m_sp->runAction(Sequence::create( DelayTime::create(m_delayTime*m_frameInterval), spAct, RemoveSelf::create(), NULL));
    else
        m_sp->runAction(Sequence::create( spAct, RemoveSelf::create(), NULL));
}

void LQBuffSprite::playLine(int left)
{
    m_time = 0;
    m_skillOutputTime = 1.0f;  //固定走2秒 m_frameReady * m_frameInterval;
    m_skillHitTime = m_skillOutputTime;
    m_skillAllOutputTime = m_skillOutputTime;
    if(m_nextAnimationData!=NULL)
        m_skillAllOutputTime += m_nextAnimationData->getFrameReady() * m_frameInterval;
    
    if(!m_sp) return;
    
    int dd = left ? -1: 1;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    if (m_data->getAudio() != "" && m_data->getBreakType()==4) {
        string filePath;
        stringf(filePath, "res/audio/%s", m_data->getAudio().c_str());
        if (_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3)
            CCLOG("LQBuffSprite playLine:%s  %ld", filePath.c_str(), getCurrentMillSecond());
        AudioManager::getInstance()->playEffect(filePath.c_str());
    }
    
    //auto callBack = CallFunc::create(CC_CALLBACK_0(LQBuffSprite::skillOutputCallBack,this));
    auto callBack0 = CallFunc::create(CC_CALLBACK_0(LQBuffSprite::skillHitOutputCallBack,this));
    auto callBack1 = CallFunc::create(CC_CALLBACK_0(LQBuffSprite::skillOutputCallBack,this));
    ActionInterval* spAct2 = Sequence::create( DelayTime::create(m_skillHitTime), callBack0, NULL);
    ActionInterval* spAct3 = Sequence::create( DelayTime::create(m_skillOutputTime), callBack1, NULL);
    ActionInterval* spAct4 = Spawn::create(spAct2, spAct3, NULL);
    
    Repeat* forever1 = Repeat::create(this->createAnimation(),m_repeatTime);
    ActionInterval* spAct1 = MoveBy::create(m_skillOutputTime*1.6, Point(dd*visibleSize.height*0.9, dd*visibleSize.height*0.9*0.5));
    ActionInterval* spAct0 = Sequence::create( spAct1, RemoveSelf::create(), NULL);
    //ActionInterval* spAct2 = Sequence::create( DelayTime::create(m_skillOutputTime), callBack, NULL);
    ActionInterval* spAct = Spawn::create(forever1, spAct0, spAct4, NULL);
    
    m_sp->runAction(spAct);
}

void LQBuffSprite::playWithTimes(unsigned int times,Sequence* sequ)
{
    m_time = 0;
    m_skillOutputTime = m_frameNum * m_frameInterval;
    m_skillHitTime = m_skillOutputTime;
    
    m_repeatTime = times;
    ActionInterval* spAct;
    ActionInterval* spAct1 = this->createAnimation();
    if(!sequ){
        //CallFunc* callBack = CallFunc::create(CC_CALLBACK_0(LQBuffSprite::skillOutputCallBack,this));
        //sequ = Sequence::create(callBack,NULL);
        auto callBack0 = CallFunc::create(CC_CALLBACK_0(LQBuffSprite::skillHitOutputCallBack,this));
        auto callBack1 = CallFunc::create(CC_CALLBACK_0(LQBuffSprite::skillOutputCallBack,this));
        sequ = Sequence::create(Spawn::create(callBack0,callBack1,NULL),NULL);
    }
    sequ->retain();
    m_skillOutputTime *= m_repeatTime;
    m_skillAllOutputTime = m_skillOutputTime;
    if(m_nextAnimationData!=NULL)
        m_skillAllOutputTime += m_nextAnimationData->getFrameReady() * m_frameInterval;
    ActionInterval* spAct2 = Sequence::create( DelayTime::create(m_skillOutputTime), sequ, NULL);
    Repeat* forever1 = Repeat::create(spAct1,m_repeatTime);
    spAct = Spawn::create(forever1, spAct2, NULL);
    
    m_sp->runAction(Sequence::create( spAct, RemoveSelf::create(), NULL));
    sequ->release();
}

//打击声音
void LQBuffSprite::skillHitOutputCallBack()
{
    if (m_data->getAudio() != "" && m_data->getBreakType()!=3) {
        string filePath;
        stringf(filePath, "res/audio/%s", m_data->getAudio().c_str());
        if (_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3)
            CCLOG("LQBuffSprite skillHitOutputCallBack:%s  %f", filePath.c_str(), getCurrentMillSecond());
        AudioManager::getInstance()->playEffect(filePath.c_str());
    }
}

//伤害输出
void LQBuffSprite::skillOutputCallBack()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3)
        CCLOG("LQBuffSprite skillOutputCallBack: %f", getCurrentMillSecond());
    
//    if (m_data->getHitAudio() != "") {
//        string filePath;
//        stringf(filePath, "res/audio/%s", m_data->getHitAudio().c_str());
//        AudioManager::getInstance()->playEffect(filePath.c_str());
//    }
    
    if(m_nextAnimationData!=NULL){
        LQBuffSprite* _nextSp = LQBuffSprite::createWithData(m_nextAnimationData);
        _nextSp->setPosition(this->getPosition());
        int zOrder = 1;
        if ( _nextSp->getHangingPoint()!=4 )  //底部
            zOrder = 3001;
        
        this->getParent()->addChild(_nextSp, zOrder);
        _nextSp->play1();
    }
    
    //LQEventSkillEffect* event=new LQEventSkillEffect(NULL,NULL);
    LQEventSkillEffect event(NULL,NULL);
    //event.setUserData(this);
    _eventDispatcher->dispatchEvent(&event);
}

//在update中计算技能的持续时间
void LQBuffSprite::update(float dt)
{
    m_time += dt;
    m_isShowEnd = false;
    
    if (m_time >= m_skillOutputTime) {
        m_skillOutputTime += m_frameInterval+m_frameInterval* m_frameNum;
        m_isOutput = true;
    }
    if (m_time >= m_skillHitTime) {
        m_skillHitTime += m_frameInterval+m_frameInterval* m_frameNum;
        m_isHitOutput = true;
    }
    
    if (m_time >= m_allTime && m_allTime>0) {
        m_isShowEnd = true;
    }
}

LQ_NAMESPACE_END