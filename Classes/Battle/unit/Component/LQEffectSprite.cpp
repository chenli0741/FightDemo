//
//  LQEffectSprite.cpp
//  LostEmpire
//
//  Created by chenli on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "LQEffectSprite.h"
#include "LQCombatHero.h"
#include "ResourceManage.h"
//#include "SmokeManager.h"
using namespace cocos2d;
LQ_NAMESPACE

LQEffectSprite::LQEffectSprite():m_radius(0),m_skillCount(1),m_animationData(NULL),m_imageFileID(0),m_scaleFactor(1),m_frameInterval(60),m_frameNum(1),m_skillAnchPosition(Point(0.5, 0.5)),m_frameReady(0),m_skillType(2),m_own(NULL),m_repeatTime(1),m_time(0),m_speed(1),m_isEnd(0)
{
    
}
LQEffectSprite::~LQEffectSprite()
{
    //log("~~~~~~LQEffectSprite::~LQEffectSprite  type = %d",m_skillShowType);
}

LQEffectSprite* LQEffectSprite::createSkillWithIDAndType(int skillID, LQCombatUnit* own, KLQSkillShowType skillShowtype)
{
    LQEffectSprite* sp = new LQEffectSprite();
    if (sp && sp->initWithFile("res/blank.png")) {
        sp->initWithIDAndType(skillID, own, skillShowtype);
        sp->autorelease();
    }else{
        CC_SAFE_DELETE(sp);
    }
    
    return sp;
}
//    klqSkillShowType_None = 0,      //无回调
//    klqSkillShowType_ActiveSkill,   //主动技能
//    klqSkillShowType_Passive,       //被动技能
//    klqSkillShowType_Buff,           //buff效果
//    KLQSkillShowType_Stunte
bool LQEffectSprite::initWithIDAndType(int effectID, LQCombatUnit* own, KLQSkillShowType skillShowtype)
{
    m_skillShowType = skillShowtype;
    m_own = own;
    m_skillID = effectID;
    switch (skillShowtype) {
        case klqSkillShowType_None:
        {
            initWithEffectID(effectID);
        }
            break;
//        case klqSkillShowType_ActiveSkill:
//        {
//            initWithSkillID(effectID);
//        }
//            break;
//        case klqSkillShowType_Passive:
//        {
//            initWithSkillID(effectID);
//        }
//            break;
        case KLQSkillShowType_Stunte:
        {
            initWithEffectID(effectID);
        }
            break;
        case klqSkillShowType_Buff_1:
        case klqSkillShowType_Buff_2:
        case klqSkillShowType_Buff_3:
        case klqSkillShowType_Buff_4:
        case klqSkillShowType_Buff_5:
        {
            initWithBuffID(effectID);
        }
            break;
        case KLQSkillSHowType_Smoke:
        {
            initWithEffectID(effectID);
        }
            break;
        default:
            break;
    }
    
    if (m_skillShowType == klqSkillShowType_ActiveSkill || m_skillShowType == klqSkillShowType_Passive || m_skillShowType == KLQSkillShowType_Stunte) {
        string filename;
        stringf(filename, "res/skill/%d.plist",m_imageFileID);
        string pvrName;
        stringf(pvrName, "res/skill/%d.pvr.ccz",m_imageFileID);
        if(_DEBUG_TYPE==_DEBUG_TYPE_SAFE) CCLOG("Skill addResFile: %s ", filename.c_str());
        ResourceManage::getInstance()->addResFile(filename, pvrName);
    }
    
    
    Sprite* sp = createSprite();
    this->setTextureRect(Rect(0,0,sp->getContentSize().width,sp->getContentSize().height));
    if (m_skillShowType == klqSkillShowType_Buff_3) {
        setDirction(m_own->getDirection());
    }
    m_frameInterval = m_frameInterval/1000.0f;
    caculateTime();
    return true;
}

//bool LQEffectSprite::initWithSkillID(int skillId)
//{
//    LQSkillUserData* m_skillUserData = new LQSkillUserData(skillId);
//    initWithEffectID(m_skillUserData->getHitEffect());
//    m_repeatTime =  m_skillUserData->getDuplication();
//    m_radius = m_skillUserData->getRadius();
//    m_skillCount = m_skillUserData->getCount();
//    CC_SAFE_DELETE(m_skillUserData);
//    
//    return true;
//}

bool LQEffectSprite::initWithBuffID(int buffID)
{
    LQEffectUserData* buffUserData = new LQEffectUserData(buffID, 1);
    if (m_skillShowType == klqSkillShowType_Buff_4  && buffUserData->getAnimationID() > 0) {
        initWithEffectID(buffUserData->getAnimationID());
    }else{
        //特殊buff  播放的内容为buffinfo   文件中的图片
        m_frameNum = 1;
    }
    m_imageFileID = buffUserData->getAnimationID();
    m_skillCount = 1;
    m_skillAnchPosition = buffUserData->getSkillPosition();
    CC_SAFE_DELETE(buffUserData);
    
    return true;
}

bool LQEffectSprite::initWithEffectID(int effectID)
{
    if (effectID<=0)
        return false;
    m_animationData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), effectID);
    m_imageFileID = m_animationData->getResID();
    
    
    m_repeatTime = m_animationData->getDuplication();
    m_skillType = m_animationData->getHangingPoint();
    m_scaleFactor = m_animationData->getScaleFactor()*0.001;
    m_frameInterval = m_animationData->getFrameInterval();
    m_skillAnchPosition = PointFromString(m_animationData->getPosition());
    m_frameHit = m_animationData->getFrameHit();
    m_frameReady = m_animationData->getFrameReady();
    m_frameEnd = m_animationData->getFrameEnd();
    m_frameNum = m_frameReady+m_frameEnd;
    return true;
}

//计算技能的时间
void LQEffectSprite::caculateTime()
{
    m_skillLastTime = m_frameInterval*m_frameNum * m_repeatTime + m_frameInterval* (m_skillCount - 1);
    m_skillOutputTime = m_frameReady * m_frameInterval * m_repeatTime;
    if (m_frameHit>0)
        m_skillOutputTime = m_frameHit * m_frameInterval * m_repeatTime;
    
    if (m_skillLastTime>1000)
        CCLOG("LQEffectSprite........ caculateTime");
}

void LQEffectSprite::play()
{
    if (m_skillShowType == klqSkillShowType_Buff_5 ) {
        
        playBuffSkill5();
        return;
    }
    if (m_imageFileID <= 0) {
        return;
    }

    std::vector<Point> pointVector;
    pointVector.push_back(Point(0, 0));
    
    float beginA = 0;
    
    float dis = MAP_LENGTH * m_radius;
    
    for (int i = 0; i < m_skillCount - 1; i++) {
        float x = dis * cos(beginA);
        float y = dis * sin(beginA);
        
        CCLOG("LQEffectSprite point = (%f,%f), Angle = %f",x,y,beginA);
        pointVector.push_back(Point(x, y));
        
        beginA += CC_DEGREES_TO_RADIANS(360/(m_skillCount - 1));//
        
    }
    
    for (int i = 0; i < m_skillCount; i++) {
       
        float tempScale = getScale() - rand()%3*0.1 + 0.1;
        
        Point desPoint = pointVector.at(i);
        this->runAction(Sequence::create(DelayTime::create(m_frameInterval*i),CallFunc::create([this,desPoint,tempScale](){
            this->setIsEnd(this->getIsEnd() + 1);
            this->playSigle(desPoint, tempScale);
        }), NULL));
        
    }
}

#pragma mark - buff 播放特殊处理
//播放特殊处理的buff效果
//m_skillShowType == klqSkillShowType_Buff_5
void LQEffectSprite::playBuffSkill5()
{
    Sprite* sp = createSprite();
    sp->setPosition(Point(this->getContentSize()*0.5));
    this->addChild(sp);
    RotateBy* totateby = RotateBy::create(1, 360);
    RepeatForever* buffrep = RepeatForever::create(totateby);
    sp->runAction(buffrep);
}

void LQEffectSprite::playSigle(Point desPoint, float scale)
{
    string fname = "";
    LQ::stringf( fname, "%d/1.png", m_imageFileID);

    Sprite* sp = Sprite::createWithSpriteFrameName(fname);
    sp->setPosition(desPoint+Point(this->getContentSize()*0.5));
    sp->setScale(scale*1.5);
    this->addChild(sp);
    
    ActionInterval *action = this->createAnimation();
    if (m_skillShowType == klqSkillShowType_ActiveSkill || m_skillShowType == klqSkillShowType_Passive || m_skillShowType == KLQSkillShowType_Stunte || m_skillShowType == klqSkillShowType_None /*|| m_skillShowType == KLQSkillSHowType_Smoke */) {
        
        Sequence* seq = Sequence::create(action, CallFunc::create([this,sp](){
            sp->stopAllActions();
            sp->removeFromParentAndCleanup(true);
//            if (this->getSkillShowType() == KLQSkillSHowType_Smoke) {
//                if (this->getIsEnd() == this->getSkillCount()) {
//                    this->skillEnd();
//                }
//            }
        }), NULL);
        seq->setTag(LQSkillActionTagNumber);
        sp->runAction(seq);
    }else{
        RepeatForever* forever1 = RepeatForever::create(action);
        forever1->setTag(LQSkillActionTagNumber);
        sp->runAction(forever1);
    }
}

//创建系列帧动画
ActionInterval* LQEffectSprite::createAnimation()
{
//    CCAnimation *animation = CCAnimation::create();
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
Sprite* LQEffectSprite::createSprite()
{
    std::string fname;
    stringf( fname, "%d/1.png", m_imageFileID);
    Sprite *sprite = Sprite::createWithSpriteFrameName(fname.c_str());
    return sprite;
}

//技能方向
void LQEffectSprite::setDirction(LQUnitDirectionType direction)
{
    if (!(m_skillShowType == klqSkillShowType_Buff_2 || m_skillShowType == KLQSkillSHowType_Smoke) && direction == klqUnitDirectionTypeNone) {
        return;
    }
    if (direction == klqUnitAnimPhaseNone) {
        return;
    }
    
    if (m_skillShowType == klqSkillShowType_Buff_2 || m_skillShowType == klqSkillShowType_Buff_3 || m_skillShowType == KLQSkillSHowType_Smoke) {
        this->setRotation(EffectRotate[direction]);
    }
}

//技能速度
void LQEffectSprite::setSpeed(int speed)
{
    if (m_speed == speed) {
        return;
    }
    m_speed = speed;
    
    Vector<Node*>& skillVector = this->getChildren();
    for (int i = 0; i < skillVector.size(); i++) {
        Node* skillSp = skillVector.at(i);
        Scheduler * sche = skillSp->getScheduler();
        sche->setTimeScale(speed);
    }
}
//主要用于回收资源--主动技能和被动技能时缓存的资源
void LQEffectSprite::skillEnd()
{
    if (m_skillShowType == klqSkillShowType_ActiveSkill || m_skillShowType == klqSkillShowType_Passive) {
        string filename;
        stringf(filename, "res/skill/%d.plist",m_imageFileID);
        string pvrName;
        stringf(pvrName, "res/skill/%d.pvr.ccz",m_imageFileID);
        ResourceManage::getInstance()->removeResFile(filename);
    }
    
    if (m_skillShowType == KLQSkillSHowType_Smoke) {
        this->setVisible(false);
    }else
        this->removeFromParent();

}

//在update中计算技能的持续时间
void LQEffectSprite::updateSkill(float dt)
{
    m_time += dt;
    m_isOutput = false;
    m_isShowEnd = false;
    if (m_time >= m_skillOutputTime) {
        m_skillOutputTime += m_frameInterval+m_frameInterval* m_frameNum;
        m_isOutput = true;
    }
    
    if (m_time >= m_skillLastTime) {
        m_isShowEnd = true;
    }
}

LQ_NAMESPACE_END