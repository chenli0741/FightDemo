//
//  LQArrow.h
//  game
//
//  Created by lequdev1 on 14-7-9.
//
//

#ifndef __SuperWarriors__LQArrow__
#define __SuperWarriors__LQArrow__

#include "LQCore.h"
#include "LQBezier.h"
#include "LQCombatSoldier.h"
USING_NS_CC;
using namespace std;

LQ_NAMESPACE

class LQArrow :public Sprite{
public:
    LQArrow();
    ~LQArrow();
    static LQArrow* create(LQCombatSoldier* beginUnit, LQCombatSoldier* endUnit,float speed,LQUnitDirectionType direction = klqUnitDirectionTypeNone,int type = 0);
    bool initArrow(LQCombatSoldier* beginUnit, LQCombatSoldier* endUnit, float speed, string frameName,LQUnitDirectionType direction, int type);
    
    CC_SYNTHESIZE(Point, m_fromPoint, FromPoint);
    CC_SYNTHESIZE(Point, m_toPoint, ToPoint);
    CC_SYNTHESIZE(int, m_totalStep, TotalStep);
    CC_SYNTHESIZE(float, m_speed, Speed);
    CC_SYNTHESIZE(string, m_fileName, FileName);
    CC_SYNTHESIZE(LQCombatSoldier*, m_fromUnit, FromUnit);
    CC_SYNTHESIZE(LQCombatSoldier*, m_toUnit, toUnit);
    CC_SYNTHESIZE_READONLY(LQUnitDirectionType, m_direction, Direction);//与兵的八个方向相适应
    
    CC_SYNTHESIZE(int, m_type, Type);//弓箭类型
    // 是否已经从屏幕中释放
    CC_SYNTHESIZE(bool, m_isDispose, IsDispose);
    
    CC_SYNTHESIZE(LQBezier*, m_bezier, Bezier);
    
    Point getControlPoint(const Point &from, const Point &to);
    
    void setDirection(LQUnitDirectionType direction);
    void customUpdate(float dt);
    
    // 回收，不走dealloc
    void dispose();
    
    void shoot();
    
protected:
    
    // target unit position
    Point getTargetPos();
    
protected:
    uint m_halfTotalStep;
    uint m_currentStep;
    bool m_isLostTarget;
    bool m_isSkillAttack;
    bool m_isShoot;
};

class LQArrowManager {
    
public:
    ~LQArrowManager();
    
    // 工厂类
    static LQArrowManager* sharedArrowManager();
    
    // 查找是否可以重用的Arrow
    LQArrow* getReuseArrowFromSpriteFrameName(const string& frameName);
    
    // 回收Arrow
    void addArrowToReuse(LQArrow* arrow);
    
    //
    void shootMissile(LQArrow* arrow);
    
    // 战斗结束，清理
    static void clear();
    //
    void update(float dt);
private:
    LQArrowManager();
    
    // 回收可重复利用的弓箭
    map<string, Vector<LQArrow*>>  m_reuseArrowDict;
    
    //
    Vector<LQArrow*> m_missileDict;
};
LQ_NAMESPACE_END
#endif /* defined(__game__LQArrow__) */
