//
//  LQAnimationSprite.cpp
//  LostEmpire
//
//  Created by chenli on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "LQAnimationSprite.h"
#include "LQCombatHero.h"
#include "ResourceManage.h"
using namespace cocos2d;
LQ_NAMESPACE

#define LQBuffActionTagNumber 500   //用于标识动作ID

LQAnimationSprite::LQAnimationSprite():islaoadingRes(false),m_showType(kqBuffShowType_Buff_1)
{
    m_time = 0;
    m_ownscale = 1;
}

LQAnimationSprite::~LQAnimationSprite()
{
    onFreeResource();
    //CCLOG("~~~~~~LQAnimationSprite::~LQAnimationSprite  type = %d",m_showType);
}

LQAnimationSprite* LQAnimationSprite::createWithData(LQAnimationInfoData* data)
{
    LQAnimationSprite* sp = new LQAnimationSprite();
    if (sp && sp->initWithFile("res/blank.png")) {
        sp->initWithData(data);
        sp->autorelease();
    }else{
        CC_SAFE_DELETE(sp);
    }
    
    return sp;
}

LQAnimationSprite* LQAnimationSprite::createWithId(int buffid)
{
    LQAnimationSprite* sp = new LQAnimationSprite();
    if (sp && sp->initWithFile("res/blank.png")) {
        sp->initWithId(buffid);
        sp->autorelease();
    }else{
        CC_SAFE_DELETE(sp);
    }
    
    return sp;
}

bool LQAnimationSprite::initWithId(int buffid)
{
    LQAnimationInfoData* m_animationData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), buffid);
    return initWithData(m_animationData);
}

bool LQAnimationSprite::initWithData(LQAnimationInfoData* data)
{
    m_data = data;
    
    //m_skillCount = 1;
    m_imageFileID = m_data->getResID();
    //m_showType = showType;// (KLQBuffShowType)m_data->getShowType();
    Point desPoint = Point::ZERO;
    
    m_nextAnimation = m_data->getNextAnimation();
    if(m_nextAnimation>0)
        m_nextAnimationData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), m_nextAnimation);
    m_scaleFactor = m_data->getScaleFactor()*0.001*m_ownscale;
    m_repeatTime = m_data->getDuplication();
    m_frameInterval = m_data->getFrameInterval()*0.001;
    //上 1 中 2 下 3 4 地
    m_hangingPoint = m_data->getHangingPoint();
    m_buffAnchPosition = PointFromString(m_data->getPosition());
    this->setAnchorPoint(m_buffAnchPosition);
    m_frameHit = m_data->getFrameHit();
    m_frameDelay = m_data->getFrameDelay();
    m_frameReady = m_data->getFrameReady();  //5.19更改 frameReady的含义 改成伤害触发事件
    m_frameEnd = m_data->getFrameEnd();
    m_frameNum = m_frameEnd;   //m_frameReady+m_frameEnd;
    m_allTime = -1;
    m_shadeColor = Color3BFromString(m_data->getShade().c_str());
    //;
    
    //可能会卡TODO
    if(m_showType == kqBuffShowType_Buff_1 || m_showType == kqBuffShowType_Buff_2)
        this->onLoadResource();
    
    string fname = "";
    LQ::stringf( fname, "%d/1.png", m_imageFileID);
    
    m_sp = Sprite::createWithSpriteFrameName(fname);
    this->setTextureRect(Rect(0,0,m_sp->getContentSize().width,m_sp->getContentSize().height));
    m_sp->setPosition(desPoint+Point(this->getContentSize()*0.5));
    m_sp->setScale(m_scaleFactor);
    this->addChild(m_sp);
    if(data->getOpacity()>0)
        m_sp->setOpacity(data->getOpacity());

    this->setAnchorPoint(m_buffAnchPosition);
    
    return true;
}

//主要装载资源--缓存的资源
void LQAnimationSprite::onLoadResource()
{
    string filename;
    stringf(filename, "res/skill/%d.plist",m_imageFileID);
    string pvrName;
    stringf(pvrName, "res/skill/%d.pvr.ccz",m_imageFileID);
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL) CCLOG("Skill addResFile: %s ", filename.c_str());
    ResourceManage::getInstance()->addResFile(filename, pvrName);
    islaoadingRes = true;
}

//主要用于回收资源--缓存的资源
void LQAnimationSprite::onFreeResource()
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
ActionInterval* LQAnimationSprite::createAnimation()
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
    //有具体播放者控制次数
    //if(m_repeatTime > 0)
    //{
    //    animate = Repeat::create(animate, 1);
    //}
    CC_SAFE_DELETE(allFrames);
    
    
    return animate;
}

//
void LQAnimationSprite::play()
{
//    m_time = 0;
//    m_skillOutputTime = m_frameReady * m_frameInterval * m_repeatTime;
//    if(m_repeatTime>0)
//        m_allTime = m_frameInterval*m_frameNum * m_repeatTime + m_frameInterval*(m_skillCount - 1);
    ActionInterval *action = this->createAnimation();
    RepeatForever* forever1 = RepeatForever::create(action);
    forever1->setTag(LQBuffActionTagNumber);
    m_sp->runAction(forever1);
}


LQ_NAMESPACE_END