//
//  LQMissile.cpp
//  game
//
//  Created by lequdev2 on 14-11-3.
//
//

#include "LQMissile.h"


LQ_NAMESPACE

//static int aaaa = 1;

LQMissile::LQMissile():m_fromPoint(Point::ZERO),m_toPoint(Point::ZERO),m_isDispose(false),m_currentTime(0),m_isShoot(false)
{
    //ttt = aaaa++;
}

LQMissile::~LQMissile()
{
    //CCLOG("~LQMissile %d", ttt);
}

LQMissile* LQMissile::create(LQMissileFlyType type, string missileRes, int frameCount)
{
    //string missileRes = beginUnit->getMissileRes().c_str();
    if(missileRes.length()<=0){
        type = Missile_Fly_ZX;
        frameCount = 0;
        missileRes = "B_Jian";
    }
    string frameName;
    stringf(frameName, "missile/%s_%d.png", missileRes.c_str(), frameCount);
    LQMissile* object = LQMissileManager::sharedMissileManager()->getReuseMissileFromSpriteFrameName(frameName);
    if (object == nil) {
        object = new LQMissile();
        if (object != nil && object->initWithSpriteFrameName(frameName)) {
            //object->setScale(0.8);
            object->initWithMissile(frameName.c_str(), type, missileRes, frameCount);
            object->autorelease();
        }else {
            delete object;
            object = nil;
        }
        return object;
    }else {
        //log("复用弓箭了哦！%p",object);
        object->setVisible(true);
        return object;
    }
}

LQMissile* LQMissile::create(LQMissileFlyType type, Sprite* missileSp)
{
    //string missileRes = beginUnit->getMissileRes().c_str();
    string frameName;
    stringf(frameName, "missile_%d.png", missileSp->getTag());
    LQMissile* object = LQMissileManager::sharedMissileManager()->getReuseMissileFromSpriteFrameName(frameName);
    if (object == nil) {
        object = new LQMissile();
        if (object != nil && object->initWithFile("res/blank.png")) {
            object->setTextureRect(missileSp->getTextureRect());
            object->initWithSprite(frameName.c_str(), type, missileSp);
            object->autorelease();
        }else {
            delete object;
            object = nil;
        }
        return object;
    }else {
        //log("复用弓箭了哦！%p",object);
        object->setVisible(true);
        return object;
    }
}

bool LQMissile::initWithSprite(string frameName, LQMissileFlyType type, Sprite* missileSp)
{
    m_type = type;
    m_fileName = frameName;
    m_hitEffect = 0;
    //missileSp->setPosition(this->getPositionX()/2,this->getPositionY()/2);
    this->addChild(missileSp);
    
    return  true;
}

bool LQMissile::initWithMissile(string frameName, LQMissileFlyType type, string missileRes, int frameCount)
{
    m_type = type;
    m_frameCount = frameCount;
    m_fileName = frameName;
    m_missileRes = missileRes;
    m_hitEffect = 0;
    
    if(m_frameCount>0){
        ActionInterval *action = this->createAnimation();
        RepeatForever* forever1 = RepeatForever::create(action);
        this->runAction(forever1);
    }
    
    return  true;
}

//创建系列帧动画
ActionInterval* LQMissile::createAnimation()
{
    std::string numstr;
    Vector<SpriteFrame*>* allFrames = new Vector<SpriteFrame*>();
    SpriteFrameCache* sharedSFCache = SpriteFrameCache::getInstance();
    string file = m_fileName;
    Animation *animation = AnimationCache::getInstance()->getAnimation(file);
    
    float fps = 0.07;
    
    if (!animation) {
        string oldframe = "";
        for(int i = 1; i <= m_frameCount ; i++) {
            std::string fname;
            LQ::stringf( fname, "missile/%s_%d.png", m_missileRes.c_str(), i );
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
    animate = RepeatForever::create(animate);

    CC_SAFE_DELETE(allFrames);
    return animate;
}

//void LQMissile::retain()
//{
//    Ref::retain();
//    if (ttt==1)
//    CCLOG("%p retain %d: %d", this, ttt, getReferenceCount());
//}
//void LQMissile::release()
//{
//    Ref::release();
//    if (ttt==1)
//    CCLOG("%p release %d: %d", this, ttt, getReferenceCount());
//}

void LQMissile::shoot(Point fromPoint, Point toPoint, CallFunc* callback,int hitEffect)
{
    m_callback = callback;
    m_hitEffect = hitEffect;
    m_callback->retain();
    this->runAction(FadeIn::create(1));
    
    m_isDispose = false;
    m_currentTime = 0;
    
    m_fromPoint = fromPoint;
    m_toPoint = toPoint;
    Point controlPoint = m_fromPoint;
    
    float xDistance =   m_fromPoint.x -m_toPoint.x; //两点之间X方向的距离
    float yDiatance =   m_fromPoint.y -m_toPoint.y; //两点之间Y方向的距离
    int timeMultiNum =  sqrt(xDistance*xDistance+yDiatance*yDiatance)/100;//总花费时间是基础时间的多少倍
    m_totalTime = missileType[m_type].baseTime * timeMultiNum;//计算出本次射箭花费的总时间
    if(m_totalTime < missileType[m_type].minTime){  //保证飞行的最小时间
        m_totalTime = missileType[m_type].minTime;
    }
    
    m_speedX = (m_fromPoint.x -m_toPoint.x)/m_totalTime;  //每秒钟X方向的位移
    m_speedY = (m_fromPoint.y -m_toPoint.y)/m_totalTime;  //每秒钟Y方向的位移

    m_isShoot = true;
    //schedule(schedule_selector(LQMissile::customUpdate));
    LQMissileManager::sharedMissileManager()->shootMissile(this);
}

void LQMissile::dispose() {
    Sequence* act;
    if(m_callback!=nullptr){
        act = Sequence::create(m_callback, FadeOut::create(1), NULL);
        m_callback->release();
    }
    else act = Sequence::create(FadeOut::create(1), NULL);
    this->runAction(act);
    //this->unschedule(schedule_selector(LQMissile::customUpdate));
    m_isShoot = false;
    LQMissileManager::sharedMissileManager()->addMissileToReuse(this);
}

void LQMissile::customUpdate(float dt)
{
    //CCLOG("missile time is %f",dt);
    if(!m_isShoot)
        return;
    float flytimes = missileType[m_type].flyTime;
    if (m_currentTime == 0) {
        this->runAction(FadeIn::create(0));
    }
    //位置加1
    m_currentTime=m_currentTime+dt;
    if (m_currentTime <= m_totalTime && !m_isDispose) {//还没消失，继续飞
        float midStation = m_totalTime/2;//中间位置步数
        float xChange =m_speedX*dt;         //x坐标的该变量
        float yChange =m_speedY*dt+(midStation-m_currentTime)*flytimes;  //Y坐标的该变量
       
        
        if(m_type == Missile_Fly_TS){//投石车特殊处理
            if(m_currentTime <m_totalTime/4){//前半段，往高飞的过程
                yChange = m_speedY*dt + m_totalTime/4*flytimes*3-m_currentTime*flytimes*3;
            }else{//后半段，往下掉的过程
                yChange = m_speedY*dt -m_currentTime*flytimes/3+m_totalTime/4*flytimes/3;
            }
            
        }
        
        float x0 = this->getPositionX()-xChange;
        float y0 = this->getPositionY()-yChange;
        
        //CCLOG("LQMissile::customUpdate x0 = %.3f y0 = %.3f", x0, y0);
        this->setPosition(x0,y0); //设置偏移量
        float radians = atan2f(-yChange, -xChange);
        // // 设置旋转了多少度
        // OpenGLES 的旋转角度和cocos2d sprite的旋转角度相反，所以要乘以-1.
        float degrees = CC_RADIANS_TO_DEGREES(radians * -1); //radians * 180 / M_PI;
        this->setRotation(degrees);
    }else{
        dispose();
    }
    
}

static LQMissileManager *_instance = nil;

LQMissileManager::LQMissileManager() {
    
}

LQMissileManager::~LQMissileManager() {
    //CCLOG("~LQMissileManager");
    m_missileDict.clear();
    while( !m_reuseMissileDict.empty() ) {
//        for( LQMissile* missile : m_reuseMissileDict.begin()->second){
//           if (missile->ttt==1)
//           CCLOG("%p missile free: %d", missile, missile->getReferenceCount());
//        }
        m_reuseMissileDict.begin()->second.clear();
        m_reuseMissileDict.erase( m_reuseMissileDict.begin() );
    }
}

LQMissileManager* LQMissileManager::sharedMissileManager() {
    if (_instance == nil) {
        _instance = new LQMissileManager();
    }
    return _instance;
}

LQMissile* LQMissileManager::getReuseMissileFromSpriteFrameName(const string &frameName)
{
    if (m_reuseMissileDict.size() > 0) {
        Vector<LQMissile*>& MissileList = m_reuseMissileDict[frameName];
        if (MissileList.size() <= 0 ) {
            return nil;
        }
        LQMissile* object = *m_reuseMissileDict[frameName].begin();
        //object->retain();
        
        m_reuseMissileDict[frameName].erase(m_reuseMissileDict[frameName].begin());
        return object;
    }
    return nil;
}

void LQMissileManager::addMissileToReuse(LQ::LQMissile *missile)
{
    //移除射击队列
    m_missileDict.eraseObject(missile);
    
    //log("加入到循环中");
    Vector<LQMissile*>& missileList = m_reuseMissileDict[missile->getFileName()];
    if (missileList.size() < 0) {
        m_reuseMissileDict[missile->getFileName()] = missileList;
    }
    missile->setVisible(false);
    m_reuseMissileDict[missile->getFileName()].pushBack(missile);
}

//加入射击队列
void LQMissileManager::shootMissile(LQMissile* missile)
{
    m_missileDict.pushBack(missile);
}

void LQMissileManager::clear()
{
    CC_SAFE_DELETE(_instance);
    _instance = NULL;
}

//
void LQMissileManager::update(float dt)
{
    LQMissile* missile;
    for (int i=m_missileDict.size()-1;i >=0 ;--i)
    //for (auto iter = m_missileDict.begin(); iter != m_missileDict.end(); iter++)
    {
        missile = m_missileDict.at(i);
        missile->customUpdate(dt);  //(*iter)
    }
}

LQ_NAMESPACE_END