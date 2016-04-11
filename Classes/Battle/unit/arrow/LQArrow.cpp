//
//  LQArrow.cpp
//  game
//
//  Created by lequdev1 on 14-7-9.
//
//

#include "LQArrow.h"
LQ_NAMESPACE
//static const uint FALL_OFF_STEP = 8;
//static const uint DISAPPEAR_STEP = FALL_OFF_STEP + 5;

LQArrow::LQArrow():m_fromPoint(Point::ZERO),m_toPoint(Point::ZERO), m_bezier(nil),m_isDispose(false),m_currentStep(0)
{
    m_bezier = new LQBezier();
    m_isShoot = false;
}

LQArrow::~LQArrow()
{
    
    CC_SAFE_DELETE(m_bezier);
    
}



LQArrow* LQArrow::create(LQCombatSoldier* beginUnit, LQCombatSoldier* endUnit, float speed, LQUnitDirectionType direction, int type){
    string missileRes = beginUnit->getMissileRes().c_str();
    string frameName;
    stringf(frameName, "%s_000.png", missileRes.c_str());
    LQArrow* object = LQArrowManager::sharedArrowManager()->getReuseArrowFromSpriteFrameName(frameName);
    if (object == nil) {
        object = new LQArrow();
        if (object != nil && object->initWithSpriteFrameName(frameName)) {
            object->setScale(0.8);
            object->initArrow(beginUnit, endUnit, speed, frameName, direction, type);
            object->autorelease();
        }else {
            delete object;
            object = nil;
        }
        return object;
    }else {
        //log("复用弓箭了哦！%p",object);
        object->initArrow(beginUnit, endUnit, speed, frameName, direction, type);
        return object;
    }
}

bool LQArrow::initArrow(LQCombatSoldier* beginUnit, LQCombatSoldier* endUnit, float speed, string frameName,LQUnitDirectionType direction, int type){
    
    {
        this->runAction(FadeIn::create(1));
        setDirection(direction);
        m_type = type;
        m_fileName = frameName;
        m_isDispose = false;
        m_currentStep = 0;
        m_fromPoint = beginUnit->getPosition();
        m_toPoint = endUnit->getPosition();
        m_speed = speed;
        m_fromUnit = beginUnit;
        m_toUnit = endUnit;
        
        Point controlPoint = getControlPoint(m_fromPoint,m_toPoint);
        m_totalStep = m_bezier->calcStep(m_fromPoint, controlPoint, m_toPoint, m_speed);
        
        return  true;
    }
    return false;
}
void LQArrow::setDirection(LQUnitDirectionType direction)
{
    m_direction = direction;
}
void LQArrow::shoot()
{
    m_isShoot = true;
    //schedule(schedule_selector(LQArrow::customUpdate));
}
Point LQArrow::getControlPoint(const Point &from, const Point &to)
{
    float x1 = (from.x + to.x)/2;
    float y1 = (from.y + to.y)/2;
    
    float disX = 0;
    float disY = 0;
    float dist_ft = from.getDistance(to);
    float angle_ft = to.getAngle(from);
    
    float ratio = 1;
    switch (m_direction) {
        case klqUnitDirectionTypeDown://下
        {
            ratio = 0.5;
        }
            break;
        case klqUnitDirectionTypeLeft://左
        {
            
        }
            break;
        case klqUnitDirectionTypeUp://上
        {
            ratio = 0.5;
        }
            break;
        case klqUnitDirectionTypeRight://右
        {
        
        }
            break;
        case klqUnitDirectionTypeLeftDown://左下
        {
            
        }
            break;
        case klqUnitDirectionTypeLeftUp://左上
        {
            
        }
            break;
        case klqUnitDirectionTypeRightDown://右下
        {
            
        }
            break;
        case klqUnitDirectionTypeRightUp://右上
        {
            
        }
            break;
            
        default:
            break;
    }
    
    disX = dist_ft* ratio *sin(angle_ft);
    disY = dist_ft* ratio *cos(angle_ft);
    
    return Point(x1 + disX, y1 + disY);
    
    
}
void LQArrow::dispose() {
    //CallFunc::create([this](){
//    if(this->gettoUnit() && this->gettoUnit()->getPosition().getDistance(m_toPoint) < 20)
//        //            this->getFromUnit()->shootEnd();
//        }),
    this->runAction(Sequence::create(FadeOut::create(1), NULL));
    m_isShoot = false;
    //this->unschedule(schedule_selector(LQArrow::customUpdate));
    LQArrowManager::sharedArrowManager()->addArrowToReuse(this);
}
void LQArrow::customUpdate(float dt)
{
    if(!m_isShoot)
        return;
    //bool isEnd = true;
    if (m_currentStep <= m_totalStep && !m_isDispose) {
        if (m_currentStep == 0) {
            this->runAction(FadeIn::create(0));
        }
        //isEnd = false;
        // 还没击中目标
        Point controlPosition = getControlPoint(m_fromPoint, m_toPoint);
        
        vector<float> temp;
        m_bezier->getAnchorPoint(temp, m_currentStep, m_fromPoint, controlPosition, m_toPoint, m_speed);
        if (temp.size() == 0) {
            // 没有飞行轨迹了
            //isEnd = true;
            log("隐藏弓箭");
            dispose();
        }else {
            float x = temp[0];
            float y = temp[1];
            float degrees = temp[2];
            // 新坐标
            this->setPosition(Point(x, y));
            // 朝向
            this->setRotation(degrees);
            // 下一步
//            if (m_currentStep > m_totalStep/2) {
//                // 加速，带来比较强的视觉冲击感
//                m_currentStep += 2;
//            }else
            {
                m_currentStep++;
            }
            
        }
        
    }else{
        dispose();
    }
   
}

static LQArrowManager *_instance = nil;

LQArrowManager::LQArrowManager() {
//    m_reuseArrowDict = CCDictionary::create();
//    m_reuseArrowDict->retain();
}

LQArrowManager::~LQArrowManager() {
//    CC_SAFE_RELEASE(m_reuseArrowDict);
}


LQArrowManager* LQArrowManager::sharedArrowManager() {
    if (_instance == nil) {
        _instance = new LQArrowManager();
    }
    return _instance;
}

LQArrow* LQArrowManager::getReuseArrowFromSpriteFrameName(const string &frameName)
{
    if (m_reuseArrowDict.size() > 0) {
        Vector<LQArrow*>& arrowList = m_reuseArrowDict[frameName];
        if (arrowList.size() <= 0 ) {
            return nil;
        }
        LQArrow* object = *m_reuseArrowDict[frameName].begin();
        object->retain();
        
        m_reuseArrowDict[frameName].erase(m_reuseArrowDict[frameName].begin());
        return object;
    }
    return nil;
}

void LQArrowManager::addArrowToReuse(LQ::LQArrow *arrow)
{
    //移除射击队列
    m_missileDict.eraseObject(arrow);
   
    //log("加入到循环中");
    Vector<LQArrow*>& arrowList = m_reuseArrowDict[arrow->getFileName()];
    if (arrowList.size() < 0) {
        m_reuseArrowDict[arrow->getFileName()] = arrowList;
    }
    m_reuseArrowDict[arrow->getFileName()].pushBack(arrow);
}

//加入射击队列
void LQArrowManager::shootMissile(LQ::LQArrow* arrow)
{
    m_missileDict.pushBack(arrow);
}

void LQArrowManager::clear()
{
    delete _instance;
    _instance = nil;
}

//
void LQArrowManager::update(float dt)
{
    for (auto iter = m_missileDict.begin(); iter != m_missileDict.end(); iter++)
    {
        (*iter)->customUpdate(dt);
    }
}
LQ_NAMESPACE_END