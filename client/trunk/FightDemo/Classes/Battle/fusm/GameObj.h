//
// GameObj.h
// SuperWarriors
// 控制对象
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#ifndef __Lequworld_SuperWarriors_GAMEOBJ_H_
#define __Lequworld_SuperWarriors_GAMEOBJ_H_

#include "cocos2d.h"
#include "FSMEnum.h"
#include "FState.h"
#include "LQAttackDataInterface.h"
#include "../unit/Component/LQFlyNumber.h"
#include "LQEffectInfoData.h"

USING_NS_CC;

LQ_NAMESPACE_USE;

using namespace std;

// 对象的序号起始数 (1-9)*DEF_COMBAT_IDXTIMES 或者 DEF_COMBAT_INDEX+(1-9)*DEF_COMBAT_IDXTIMES
const int DEF_COMBAT_INDEX = 1000;
const int DEF_COMBAT_IDXTIMES = 100;
const float DEF_ANIMATION_INTERVAL_MAX = 0.09;
typedef enum{
    klqLQGameObjNone,
    klqLQGameObjSoldier,
    klqLQGameObjHero,
    klqLQGameObjSquad,
    klqLQGameObjBULLET
}LQGameObjType;

const int _DEBUG_TYPE = 7;
const int _DEBUG_INDEX = -1;
const int _DEBUG_INDEX_MAX = 9999999;

#define _DEBUG_TYPE_MOVE  1   //移动调试
#define _DEBUG_TYPE_FIGHT 2   //战斗调试
#define _DEBUG_TYPE_VALUE 3   //伤血调试
#define _DEBUG_TYPE_SKILL 4   //技能调试
#define _DEBUG_TYPE_SKILL2 5  //技能调试
#define _DEBUG_TYPE_SKILL3 6  //技能调试
#define _DEBUG_TYPE_BUFF  7   //技能BUFF调试
#define _DEBUG_TYPE_TIME  65  //时间调试
#define _DEBUG_TYPE_SAFE  66  //释放调试
#define _DEBUG_TYPE_LOAD  67  //装载调试

class LQBuffUserData;

//移动的对象基类
class GameObj: public Node
{
public:
    //constructors/functions
    GameObj();
    GameObj(LQUnitDirectionType _direction);
    ~GameObj();
    virtual bool init();
    virtual void update(float dt, Layer* parent);
    
    //检查是否碰撞
    virtual bool isColliding(GameObj *obj);
    
    //碰撞后处理
    virtual void doCollision(GameObj *obj) {m_active = false;}
    
    //更改动画
    virtual void changeAnimation(){};
    
    // 获取我们要的位置(军团坐标位置重写了)
    virtual Point getLQPosition(){ return _position; };
    
    //Index
    CC_SYNTHESIZE(int, m_index, Index)
    //Name
    CC_SYNTHESIZE_READONLY(string, m_name, Name)
    // 对象唯一ID
    CC_SYNTHESIZE(int, m_objectUID, ObjectUID)
    //对象类型
    CC_SYNTHESIZE_READONLY(LQGameObjType, m_type, Type)
    // tileCoord
    //CC_SYNTHESIZE_READONLY(Point, m_tileCoord, TileCoord)
    virtual Point getTileCoord(){ return m_tileCoord; }
    
    //对象方向
    CC_SYNTHESIZE_READONLY(LQUnitDirectionType, m_direction, Direction)
    virtual void setDirection(LQUnitDirectionType direction);
    //获取格子坐标
    Point getCurrentTileCoord(){ return convertTileCoord(_position); };
    //对象方向偏移位置
    CC_SYNTHESIZE_READONLY(Point, m_directionOffset, DirectionOffset)
    
    //是否活动
    //CC_SYNTHESIZE_READONLY(bool, m_active, Active)
    bool getActive(){
        return m_active ;//&& isVisible()
    }
    void setActive(bool val){
        //        if (m_index==16 && val==false){
        //            CCLOG("setActive");
        //        }
        m_active = val;
    };
    bool getIsDie(){ return m_active; }
    
    virtual bool isDied(){ return !m_active;};
    // 攻击对象数据
    CC_SYNTHESIZE(GameObj*, m_attacker, Attacker)
    //是否打开视野
    CC_SYNTHESIZE(bool, m_isOpenView, IsOpenView)
    //碰撞标志
    CC_SYNTHESIZE_READONLY(int, m_collisionFlags, CollisionFlags)
    // 索敌目标index
    CC_SYNTHESIZE_READONLY(int, m_targetIndex, TargetIndex);
    // 目标对象数据
    CC_SYNTHESIZE_READONLY(GameObj*, m_target, Target)
    virtual void setTarget(GameObj* target);
    virtual void onLostTarget(){};  //变更目标，
    //嘲讽者
    CC_SYNTHESIZE_READONLY(GameObj*, m_taunter, Taunter );
    // 位置目标点（可以用来设置集中点，功能同WayPoints，MustPoints）
    CC_SYNTHESIZE_READONLY(Point, m_targetPoint, TargetPoint)
    virtual void setTargetPoint(Point target);
    //上一次的目标，切换目标用
    CC_SYNTHESIZE_READONLY(GameObj*, m_oldtarget, OldTarget)
    // 行走的路程
    CC_SYNTHESIZE(int, m_moveMileage, MoveMileage)
    // 求到目标的距离
    float getDistance(GameObj* target);
    // 求到目标的格子距离
    float getTileDistance(GameObj* target);
    //获取攻击目标的位置
    virtual Point getTargetAttackTileCoord(GameObj* obj){ return m_tileCoord; };
    //获取命中的中心位置
    virtual Point getHitPosition(){ return _position; };
    //获取箭矢的位置
    virtual Point getMissileTargetPostion(){ return _position; };
    virtual Point getMissileStartPostion(){ return _position; };
    
    //获取对象的战斗公式计算器
    virtual LQAttackDataInterface* getCalculator(){ return NULL; };
    //攻击目标
    virtual void changeTargetDps(GameObj* pTarget,int dps,LQFlyNumberTypeEnum numbertype = klq_flaytype_normal,LQEffectInfoData* effectdata = NULL,int times = 0, float outtime = 0){};
    //变更怒气 最大不得超过最大怒气上限
    virtual void changeHeroFuries(int value){};
    
    //BUFF
    virtual void addBuffer(LQBuffUserData* bufferData){};       //添加BUFF
    virtual void removeBuffer(LQBuffUserData* bufferData){};    //删除BUFF
    virtual void cleareBuffer(){};                              //移除所有BUFF
    virtual void bufferCallback(Ref* pSender){};                //buff触发数据动作
    virtual void bufferEndCallback(Ref* pSender){};             //buff结束动作

    //被目标锁定
    virtual void locked(GameObj* obj){};
    virtual void unlocked(GameObj* obj){};
    CC_SYNTHESIZE(bool, m_isCanAttack, IsCanAttack);
    
    //是否有可攻击的敌人
    inline bool isHasTarget() {
        return this->getTarget() && this->getTarget()->getActive() && this->getTarget()->isVisible();
    }
    
    //是否站着
    virtual bool isStand() { return false; };
    
    //路径  格子集合
    CC_SYNTHESIZE_READONLY(vector<Point>, m_wayPoints, WayPoints)
    void addWayPoint(Point way);        //way 格子
    void insertWayPoint(Point way);
    CC_SYNTHESIZE_READONLY(vector<Point>, m_mustPoints, MustPoints)
    void addMustPoint(Point way);      //must 格子
    
    //存在路径点true
    bool hasWayPoint();
    
    //
    void clearEffect(){ m_effectpt = 0; };
protected:
    int m_effectpt;
    bool m_active;
    Point m_tileCoord;
    // 动作切换间隔
    float m_animationInterval;
    // 锁定对象
    GameObj *m_lockedObjs[8];
    
    void _LQDEBUG(const char * format, ...);  //只考虑index条件
    void _LQDEBUG(int type, const char * format, ...); //考虑type条件，index是默认
    void _LQDEBUG(int type, int index, const char * format, ...);
};

//根据方向获取偏移值,是坐标点的偏移
static Point getPointOffsetFromDirection(LQUnitDirectionType direction)
{
    Point m_directionOffset = Point::ZERO;
    switch(direction){
        case  klqUnitDirectionTypeDown: m_directionOffset = Point(0,-1); break;
        case  klqUnitDirectionTypeLeft: m_directionOffset = Point(-1,0); break;
        case  klqUnitDirectionTypeUp: m_directionOffset = Point(0,1); break;
        case  klqUnitDirectionTypeRight: m_directionOffset = Point(1,0); break;
        case  klqUnitDirectionTypeLeftDown: m_directionOffset = Point(-1,-1); break;
        case  klqUnitDirectionTypeLeftUp: m_directionOffset = Point(-1,1); break;
        case  klqUnitDirectionTypeRightDown: m_directionOffset = Point(1,-1); break;
        case  klqUnitDirectionTypeRightUp: m_directionOffset = Point(1,1); break;
        default: m_directionOffset = Point::ZERO; break;
    }
    
    return Point(m_directionOffset.x,m_directionOffset.y*0.5);
}

//根据方向获取偏移值,是格子的偏移
static Point getTileCoordOffsetFromDirection(LQUnitDirectionType direction)
{
    Point m_directionOffset = Point::ZERO;
    switch(direction){
        case  klqUnitDirectionTypeDown: m_directionOffset = Point(-1,-1); break;
        case  klqUnitDirectionTypeLeft: m_directionOffset = Point(-1,1); break;
        case  klqUnitDirectionTypeUp: m_directionOffset = Point(1,1); break;
        case  klqUnitDirectionTypeRight: m_directionOffset = Point(1,-1); break;
        case  klqUnitDirectionTypeLeftDown: m_directionOffset = Point(-1,0); break;
        case  klqUnitDirectionTypeLeftUp: m_directionOffset = Point(0,1); break;
        case  klqUnitDirectionTypeRightDown: m_directionOffset = Point(0,-1); break;
        case  klqUnitDirectionTypeRightUp: m_directionOffset = Point(1,0); break;
        default: m_directionOffset = Point::ZERO; break;
    }
    
    return m_directionOffset;
}

//根据方向获取武将的偏移值,是格子的偏移
static Point getHeroTileCoordOffsetFromDirection(LQUnitDirectionType direction)
{
    Point m_directionOffset = Point::ZERO;
    switch(direction){
        case  klqUnitDirectionTypeDown: m_directionOffset = Point(-1,-1); break;
        case  klqUnitDirectionTypeLeft: m_directionOffset = Point(-1,1); break;
        case  klqUnitDirectionTypeUp: m_directionOffset = Point(1,1); break;
        case  klqUnitDirectionTypeRight: m_directionOffset = Point(1,-1); break;
        case  klqUnitDirectionTypeLeftDown: m_directionOffset = Point(-2,0); break;
        case  klqUnitDirectionTypeLeftUp: m_directionOffset = Point(0,2); break;
        case  klqUnitDirectionTypeRightDown: m_directionOffset = Point(0,-2); break;
        case  klqUnitDirectionTypeRightUp: m_directionOffset = Point(2,0); break;
        default: m_directionOffset = Point::ZERO; break;
    }
    return m_directionOffset;
}

//根据目标的格子差求方向
static LQUnitDirectionType calcDirectionFromTileCoord(Point tileCoord)
{
    //CCLOG("B= %f,%f", tileCoord.x, tileCoord.y);
    int dx = fabsf(tileCoord.x);
    int dy = fabsf(tileCoord.y);
    if (dx == dy) {
        // 垂直或者水平方向
        if (tileCoord.x < 0 && tileCoord.y < 0) {
            return klqUnitDirectionTypeDown;
        }else if (tileCoord.x < 0 && tileCoord.y > 0) {
            return klqUnitDirectionTypeLeft;
        }else if (tileCoord.x > 0 && tileCoord.y > 0) {
            return klqUnitDirectionTypeUp;
        }else if (tileCoord.x > 0 && tileCoord.y < 0) {
            return klqUnitDirectionTypeRight;
        }
    }else {
        // 其他方向
        if (dx > dy) {
            // 以X轴为方向准则
            if (tileCoord.x > 0) {
                return klqUnitDirectionTypeRightUp;
            }else {
                return klqUnitDirectionTypeLeftDown;
            }
        }else {
            // 以Y轴为方向准则
            if (tileCoord.y > 0) {
                return klqUnitDirectionTypeLeftUp;
            }else {
                return klqUnitDirectionTypeRightDown;
            }
        }
    }
    return klqUnitDirectionTypeNone;
}

//根据目标的位置差求方向
static LQUnitDirectionType calcDirectionFromPoint(Point point)
{
    float ag = point.getAngle();
    float constag = 0.3927;
    if(ag>-constag && ag<=constag)
        return klqUnitDirectionTypeRight;
    else if(ag>constag && ag<=constag*3)
        return klqUnitDirectionTypeRightUp;
    else if(ag>constag*3 && ag<=constag*5)
        return klqUnitDirectionTypeUp;
    else if(ag>constag*5 && ag<=constag*7)
        return klqUnitDirectionTypeLeftUp;
    else if((ag>constag*7 && ag<=constag*8) || (ag>-constag*8 && ag<=-constag*7))
        return klqUnitDirectionTypeLeft;
    else if(ag>-constag*7 && ag<=-constag*5)
        return klqUnitDirectionTypeLeftDown;
    else if(ag>-constag*5 && ag<=-constag*3)
        return klqUnitDirectionTypeDown;
    else if(ag>-constag*3 && ag<=-constag)
        return klqUnitDirectionTypeRightDown;
    return klqUnitDirectionTypeNone;
}

//游戏的对象集合
typedef std::vector<GameObj*> GameObjectVector;

#endif