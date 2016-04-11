//
//  LQMissile.h
//  game
//
//  Created by lequdev2 on 14-11-3.
//
//

#ifndef __SuperWarriors__LQMissile__
#define __SuperWarriors__LQMissile__

#include "LQCore.h"
#include "LQCombatUnitEnums.h"
#include "LQBezier.h"
#include "LQStringExtend.h"

USING_NS_CC;
using namespace std;

LQ_NAMESPACE

struct missileConfig{
    float baseTime;//基础时间，值越大越慢，距离为100的时候需要的时间，增长时间也是以100为单位的
    float flyTime;//飞行基础时间时的飞行高度，绝对值越大飞得越高，这个变量控制飞行的弧度
    float minTime; //动画展示的最小时间（总时间），太近的话演示时间就按这个走，避免两个人过近的时候看不出来
};

    /**
        Missile_Fly_ZX = 0,   //直线
        Missile_Fly_GJ = 1,   // 弓箭
        Missile_Fly_TS = 2,   // 投石
        Missile_Fly_BQ = 3,   // 标枪
        Missile_Fly_FS = 4,   // 法术
        Missile_Fly_1FX = 7,  // 单方向直线
        Missile_Fly_8FX = 8   // 八方向直线
     **/

//0是普通平射 1是普通中射 2是普通高射 3是投石车的抛高发射
static const missileConfig missileType[] = {
    missileConfig{0.2,-10,0.5},         //0直线
    missileConfig{0.20,-15,0.95},       //1弓箭
    missileConfig{0.5,-10,2},           //2投石
    missileConfig{0.35,-0.12,1},        //3标枪（还没调）
    missileConfig{0.35,-0.12,1},        //4法术（还没调）
    missileConfig{0.35,-0.12,1},        //5暂时没用
    missileConfig{0.35,-0.12,1},        //6暂时没用
    missileConfig{0.35,-0.12,1},        //7单方向直线，暂时不知道怎么用
    missileConfig{0.35,-0.12,1},        //8八方向直线，暂时不知道怎么用
};
//missileConfig{60,-2},



class LQMissile :public Sprite{
public:
    //int ttt;
    LQMissile();
    ~LQMissile();
    static LQMissile* create(LQMissileFlyType type, string missileRes, int frameCount);
    static LQMissile* create(LQMissileFlyType type, Sprite* missileSp);
    bool initWithSprite(string frameName, LQMissileFlyType type, Sprite* missileSp);
    bool initWithMissile(string frameName, LQMissileFlyType type, string missileRes, int frameCount);
    
    CC_SYNTHESIZE_READONLY(Point, m_fromPoint, FromPoint);
    CC_SYNTHESIZE_READONLY(Point, m_toPoint, ToPoint);
    CC_SYNTHESIZE_READONLY(string, m_fileName, FileName);
    CC_SYNTHESIZE_READONLY(int, m_hitEffect, HitEffect);
    
    void customUpdate(float dt);
    
    //virtual void retain();
    //virtual void release();
    
    // 回收，不走dealloc
    void dispose();
    
    void shoot(Point fromPoint, Point toPoint, CallFunc* callback = nullptr,int hitEffect = 0);
    
protected:
    CallFunc* m_callback;
    float m_totalTime;
    float  m_speedX;        
    float  m_speedY;
    int m_frameCount;
    LQMissileFlyType m_type;//弓箭类型
    //是否已经从屏幕中释放
    bool m_isDispose;

    Point getTargetPos();
private:
    float m_currentTime;
    bool m_isLostTarget;
    bool m_isSkillAttack;
    bool m_isShoot;
    string m_missileRes;
    
    Point getControlPoint(const Point &from, const Point &to);
    
    ActionInterval* createAnimation();
};

class LQMissileManager {
    
public:
    ~LQMissileManager();
    
    // 工厂类
    static LQMissileManager* sharedMissileManager();
    
    // 查找是否可以重用的Missile
    LQMissile* getReuseMissileFromSpriteFrameName(const string& frameName);
    
    // 回收Missile
    void addMissileToReuse(LQMissile* missile);
    
    //
    void shootMissile(LQMissile* missile);
    
    // 战斗结束，清理
    static void clear();
    
    //
    void update(float dt);
private:
    LQMissileManager();
    
    // 回收可重复利用的弓箭
    map<string, Vector<LQMissile*>>  m_reuseMissileDict;
    
    //
    Vector<LQMissile*> m_missileDict;

};
LQ_NAMESPACE_END


#endif /* defined(__game__LQMissile__) */
