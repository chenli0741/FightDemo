//
// GameObj.cpp
// SuperWarriors
// 控制对象
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#include "CCConsole.h"
#include "GameObj.h"
#include "../core/LQCore.h"


//---------------------------------------------------------
GameObj::GameObj():m_objectUID(0), m_type(klqLQGameObjNone), m_direction(klqUnitDirectionTypeNone), m_target(NULL), m_attacker(NULL),m_targetIndex(-1), m_active(true), m_collisionFlags(0),m_tileCoord(Point(-1000, -1000)), m_name(""), m_index(-1), m_isOpenView(false),m_moveMileage(0)
{
    m_oldtarget = NULL;
    m_taunter = NULL;
	m_directionOffset = Point::ZERO;
    m_animationInterval = 0;
    m_targetPoint = Point::ZERO;
    for (int i=0; i<8; i++) {
        m_lockedObjs[i] = NULL;
    }
    m_isCanAttack = false;
}

//---------------------------------------------------------
GameObj::~GameObj()
{
	setTarget(NULL);
    m_mustPoints.clear();
    m_wayPoints.clear();
}

bool GameObj::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    return true;
}

static void _log(int index, const char *format, va_list args)
{
    char buf[MAX_LOG_LENGTH];
    
    vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
    strcat(buf, "\n");
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    __android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info",  "%s", buf);
    
#elif CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WP8
    WCHAR wszBuf[MAX_LOG_LENGTH] = {0};
    MultiByteToWideChar(CP_UTF8, 0, buf, -1, wszBuf, sizeof(wszBuf));
    OutputDebugStringW(wszBuf);
    WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, buf, sizeof(buf), NULL, FALSE);
    printf("%s", buf);
    fflush(stdout);
#else
    // Linux, Mac, iOS, etc
    fprintf(stdout, "IDX%d: %s", index, buf);
    fflush(stdout);
#endif
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
    Director::getInstance()->getConsole()->log(buf);
#endif
    
}

void GameObj::_LQDEBUG(const char * format, ...)
{
    if(m_index!=_DEBUG_INDEX)
        return;
    va_list args;
    va_start(args, format);
    _log(m_index, format, args);
    va_end(args);
}

void GameObj::_LQDEBUG(int type, const char * format, ...)
{
    if(type!=_DEBUG_TYPE)
        return;
    if(m_index!=_DEBUG_INDEX && _DEBUG_INDEX>0)
        return;
    va_list args;
    va_start(args, format);
    _log(m_index, format, args);
    va_end(args);
}

void GameObj::_LQDEBUG(int type, int index, const char * format, ...)
{
    if(type!=_DEBUG_TYPE)
        return;
    if(m_index!=index && index<_DEBUG_INDEX_MAX)
        return;
    if(index>=_DEBUG_INDEX_MAX)
        index = m_index;
    va_list args;
    va_start(args, format);
    _log(index, format, args);
    va_end(args);
}

//碰撞检测
bool GameObj::isColliding(GameObj *obj)
{
	return (this->getBoundingBox().intersectsRect(obj->getBoundingBox()));
}

//设置目标
void GameObj::setTarget(GameObj* target)
{
    //if (m_index==2 && m_type==klqLQGameObjSquad && target){
    //    CCLOG("Squad[%s] setTarget target:%d", m_name.c_str(), target->getIndex());
    //}
    //if (m_index==32 && m_type!=klqLQGameObjSquad && target){
    //    CCLOG("Unit[%d]  setTarget target:%d", m_index, target->getIndex());
    //}
    
    if(!target)
        m_oldtarget = m_target;
    if(m_taunter==m_oldtarget && target==NULL)
        m_taunter=NULL;
    m_target = target;
    if (m_target) target->setAttacker(this);
    m_targetIndex = target ? target->getIndex() : -1;
    if(!m_target && m_oldtarget)
        onLostTarget();
}

void GameObj::setTargetPoint(Point target)
{
    m_targetPoint = target;
}

//根据方向确定移动的偏移
void GameObj::setDirection(LQUnitDirectionType direction)
{
    if(direction==klqUnitDirectionTypeNone)
        return;
    
    //频繁转方向
    if(m_animationInterval<DEF_ANIMATION_INTERVAL_MAX && m_direction!=klqUnitDirectionTypeNone)
        return;
    m_animationInterval = 0;
    
    if(m_index==_DEBUG_INDEX && m_direction!=klqUnitDirectionTypeNone)
        _LQDEBUG(_DEBUG_TYPE_MOVE, "GameObj::setDirection:: %s->%s",  UnitDIR[m_direction],  UnitDIR[direction]);
	if(m_direction!=direction){
        m_direction = direction;
        m_directionOffset = getPointOffsetFromDirection(direction);
        
		changeAnimation();
	}else
        m_directionOffset = getPointOffsetFromDirection(m_direction);
    
    if(m_index==_DEBUG_INDEX && m_index>=0)
        CCLOG("2 CCDirectionOffset:%s %s", UnitDIR[direction], TileCoordString(m_directionOffset).c_str());
}

// 求到目标的距离
float GameObj::getDistance(GameObj* target)
{
    if(target)
        return target->getPosition().getDistance(_position);
    return 0;
}

// 求到目标的格子距离
float GameObj::getTileDistance(GameObj* target)
{
    if(target){
        Point pt = target->getTileCoord() - this->getTileCoord();
        return MAX(abs(pt.x),abs(pt.y));
    }
    return 0;
}

//---------------------------------------------------------
void GameObj::update(float dt, Layer* parent)
{
    m_animationInterval+=dt;
	//TODO
}

// 设定路径
void GameObj::addWayPoint(Point way)
{
    if(way == Point::ZERO)
        return;
    m_wayPoints.push_back(way);
    
    //if(m_wayPoints.size()>2)
    //    CCLOG("GameObj::addWayPoint %lu", m_wayPoints.size());
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("IDX:%d 设定路径 %s", m_index, TileCoordString(way).c_str());
}

// 设定路径
void GameObj::insertWayPoint(Point way)
{
    if(way == Point::ZERO)
        return;
    m_wayPoints.insert(m_wayPoints.begin(),way);
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("IDX:%d 设定路径 %s", m_index, TileCoordString(way).c_str());
}


void GameObj::addMustPoint(Point way)
{
    if(way == Point::ZERO)
        return;
    m_mustPoints.push_back(way);
    
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("GameObj::addMustPoints %lu %s", m_mustPoints.size(), TileCoordString(way).c_str());
    //CCLOG("IDX:%d 设定路径 %s", m_index, TileCoordString(way).c_str());
}

//存在路径点true
bool GameObj::hasWayPoint()
{
    return m_mustPoints.size()+m_wayPoints.size();
}
