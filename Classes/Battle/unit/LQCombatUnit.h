//
// LQCombatUnit.h
// SuperWarriors
// 战斗单元类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#ifndef __SuperWarriors__LQCombatUnit__
#define __SuperWarriors__LQCombatUnit__

#include "cocos2d.h"
#include "AIObj.h"
#include "LQStringExtend.h"
#include "LQCombatUnitEnums.h"
#include "LQCombatUnitDefine.h"
//#include "LQHitAnimation.h"
//#include "LQMissAnimation.h"
#include "../data/LQData.h"
#include "../fusm/AIController.h"
#include "../fusm/FSMState.h"
#include "LQFlyNumber.h"
#include "LQFlagsSprite.h"
#include "LQBuffSprite.h"

USING_NS_CC;
using namespace std;

LQ_NAMESPACE


#define SmokeSpriteTagNumber   999      //烟尘SkillSprite
#define ForwardSpriteTagNumber 998    //冲锋SkillSprite
#define DEF_BUFF_TAGCOUNT  8

//static const char* DIR[] = {"S", "W", "N", "E", "SW", "NW", "SE", "NE"};
static const char* DIR[] = {"S", "W", "N", "W", "SW", "NW", "SW", "NW"};

class LQCombatUnit;
class LQCombatSquad;
class LQBattleSession;


//战斗单元阴影
class LQCombatUnitShadow {
    // 方向
    CC_SYNTHESIZE_READONLY(LQUnitDirectionType, m_direction, Direction);
    // unit type
    CC_SYNTHESIZE_READONLY(LQCombatUnitType, m_unitType, UnitType);
    
public:
    LQCombatUnitShadow(LQCombatUnit* combatUnit);
    ~LQCombatUnitShadow();
    
    void init(LQCombatUnitType unitType, LQUnitDirectionType direction = klqUnitDirectionTypeRightUp);
    
    void changeDirection(LQUnitDirectionType direction);
    void setScale(float scale);
private:
    Sprite* createSprite();
    Sprite* m_shadowSprite;
    
    LQCombatUnit* m_combatUnit;
};

//战斗单元冲击波
class LQCombatUnitForward {
    // 方向
    CC_SYNTHESIZE_READONLY(LQUnitDirectionType, m_direction, Direction);
    // unit type
    CC_SYNTHESIZE_READONLY(LQArmsTypeEnum, m_armsType, ArmsType);
    // unit type
    CC_SYNTHESIZE_READONLY(Sprite*, m_forwardSprite, ForwardSprite);
public:
    LQCombatUnitForward(LQCombatUnit* combatUnit);
    ~LQCombatUnitForward();
    
    void init(LQArmsTypeEnum armsType, LQUnitDirectionType direction = klqUnitDirectionTypeRightUp, float scale=1);
    
    void changeDirection(LQUnitDirectionType direction);
    void setScale(float scale);
    void setVisible(bool bVisible);
private:
    
    float m_forwardScale;
    Sprite* createSprite();
    void play();
    
    LQCombatUnit* m_combatUnit;
};

//战斗单元
class LQCombatUnit : public AIObj
{
public:
    LQCombatUnit(LQBaseUserData* data, LQGameObjType type, LQCombatSquadDirectionEnum direction);
    ~LQCombatUnit();
    
    virtual bool init();
    //CREATE_FUNC(LQCombatUnit);
    static LQCombatUnit* create(LQBaseUserData* data, LQGameObjType type, LQCombatSquadDirectionEnum direction);
    
    virtual bool isColliding(GameObj *obj);
	virtual void doCollision(GameObj *obj) {m_active = false;}
    virtual bool isDied();
    
    // 获取每帧的文件名
    string getSpriteFrameName(const string &unitKey, LQUnitAnimPhaseEnum phaseType, LQUnitDirectionType directType, uint index);
    // 获取动作名称
    string createAnimationPhaseName(const string &baseName, int modifierNum);
    // 获取方向动作名称
    string createAnimationDirectionName(const string &baseName, int modifierNum);

    //兵种数据
    CC_SYNTHESIZE(LQBaseUserData*, m_data, Data)
    //兵种数据
    CC_SYNTHESIZE(LQArmsUserData*, m_armdata, Armdata);
    // 提示的控件层
    CC_SYNTHESIZE(Node*, m_controlLayer, ControlLayer );
    // 精灵
    CC_SYNTHESIZE_RETAIN(Sprite*, m_sprite, Sprite);
    // tileCoord 设置格子并不设置位置
    void setTileCoord(float tx,float ty);
    void setTileCoordandPoint(float tx,float ty);
    
    //在方阵中的位置
    CC_SYNTHESIZE_READONLY(float, unit_x, UnitX)
    CC_SYNTHESIZE_READONLY(float, unit_y, UnitY)
    void setUnitXY(float x,float y);
    // sprite base name
    CC_SYNTHESIZE_READONLY(string, m_spriteBaseName, SpriteBaseName);
    // 光影资源ID，用于普通攻击的刀光
    CC_SYNTHESIZE_READONLY(std::string, m_flashID, FlashID );
    // BUFF 光影资源ID，用于普通攻击的刀光
    CC_SYNTHESIZE(std::string, m_buffflashID, BuffFlashID );
    
    // 移动速度（毫秒/格子)
    CC_SYNTHESIZE(float, m_speed, Speed );
    //当前速度
    float getCurrSpeedValue();
    // 巡逻移动速度（毫秒/格子)，集团巡逻速度
    CC_SYNTHESIZE(float, m_patrolSpeed, PatrolSpeed );
    // 集团集结移动速度同步系数
    CC_SYNTHESIZE(float, m_arraySyncSpeed, ArraySyncSpeed );
    // 集团集结移动目标
    CC_SYNTHESIZE_READONLY(Point, m_arrayPoint, ArrayPoint );
    void setArrayPoint(Point value){ m_arrayPoint = value ;};
    
    // 方向 GameObj
    //CC_SYNTHESIZE(LQUnitDirectionType, m_direction, Direction)
    //队列方向(对型方向，站的方向)
    CC_SYNTHESIZE(LQCombatSquadDirectionEnum, m_squadDirection, SquadDirection) //队列方向
    //兵种类型： 1、剑兵 2、枪兵 3、骑兵 4、戟兵 5、弩兵 6、象兵 7、藤甲 8、女兵
    CC_SYNTHESIZE(LQCombatUnitType , m_unitType, UnitType)
    // 动作类型
    CC_SYNTHESIZE_READONLY(LQUnitAnimPhaseEnum, m_phaseType, PhaseType)
    
    // 单元状态
    CC_SYNTHESIZE_READONLY(LQUnitStateEnum, m_state, State);
    // 军团状态
    CC_SYNTHESIZE_READONLY(LQSquadStateEnum, m_squadState, SquadState);
    void setSquadState(LQSquadStateEnum squadState);
    // 军团 LQCombatSquad
    CC_SYNTHESIZE_READONLY(LQCombatSquad*, m_squad, Squad);
    void setSquad(LQCombatSquad* squad);
    // 英雄 LQCombatUnit
    CC_SYNTHESIZE(LQCombatUnit*, m_hero, Hero);
    // 旗帜，拿旗帜的人，开始是英雄，英雄死后转交给其他人
    CC_SYNTHESIZE_READONLY(LQFlagsSprite*, m_armflags, ArmFlags);
    void setArmFlags(LQFlagsSprite* fsp);
    bool isBannerman(){ return m_armflags; };  //是否旗手
    // 静止，站住
    CC_SYNTHESIZE_READONLY(bool, m_isStand, IsStand );
    //是否站着
    virtual bool isStand() { return m_isStand; };
    
    // 配置的数据
    CC_SYNTHESIZE_READONLY(int, m_view, View );	//作战单位视野（单位：格子）
    CC_SYNTHESIZE_READONLY(LQAttackType, m_attackType, AttackType );	//决定采取的攻击方式，复合攻击方式读取两个攻击力数值\r\n1：近战\r\n2：远程
    
    //军团兵种\r\n1、步兵\r\n2、远程\r\n3、骑兵\r\n4、机械
    CC_SYNTHESIZE_READONLY(LQArmsTypeEnum, m_armsType, ArmsType );
    //武将职业\r\n1、普通\r\n2、策士
    CC_SYNTHESIZE(LQOccupationEnum, m_occupation, Occupation );
    
    CC_SYNTHESIZE_READONLY(int, m_attackSpeed, AttackSpeed );	//以毫秒为单位，兵种从发动攻击指令到攻击动作完成所需要的时间
    CC_SYNTHESIZE_READONLY(int, m_attackGap, AttackGap);	//攻击方式1攻击间隔时间，万分之一秒该项为－1
    CC_SYNTHESIZE_READONLY(int, m_attackDistMin, AttackDistMin );	//决定攻击方式1的有效范围
    CC_SYNTHESIZE_READONLY(int, m_attackDistMax, AttackDistMax );	//决定攻击方式1的有效范围
    //复合兵种时，当前的攻击速度
    CC_SYNTHESIZE(float, m_currentAttackSpeed, CurrentAttackSpeed );
    CC_SYNTHESIZE(float, m_currentAttackGap, CurrentAttackGap);
    CC_SYNTHESIZE_READONLY(int, m_aIType, AIType );	//决定兵种的AI类型

    CC_SYNTHESIZE_READONLY(std::string, m_priorAttacks, PriorAttacks );	//优先攻击列表，格式：\r\n优先兵种类型1(int),优先兵种类型2(int),优先兵种类型3(int),\r\n留空表示采取距离优先原则。
    CC_SYNTHESIZE_READONLY(LQMissileFlyType, m_missileType, MissileType );	//箭矢类型：\r\n1、弓箭\r\n2、投石\r\n3、标枪\r\n纯近战兵种，填-1
    CC_SYNTHESIZE_READONLY(std::string, m_missileRes, MissileRes );	//箭矢应用的资源文件，与模型资源绑定
    CC_SYNTHESIZE_READONLY(int, m_missileFrameCount, MissileFrameCount );	//箭矢资源数量
    CC_SYNTHESIZE_READONLY(Point, m_missileResPostion, MissileResPostion );	//特效位置
    CC_SYNTHESIZE_READONLY(int, m_hitEffect, HitEffect );	//命中爆点特效

    CC_SYNTHESIZE_READONLY(Point, m_centralCoordinate, CentralCoordinate );	//图片的重心坐标，格式为x;y
    CC_SYNTHESIZE_READONLY(int, m_imgwidth, ImgWidth );	//图片的宽度
    CC_SYNTHESIZE_READONLY(int, m_imgheight, ImgHeight );	//图片的高度
    // 挂点计算
    CC_SYNTHESIZE_READONLY(Point, m_TAnchorPoint, TAnchorPoint );	//头上的挂点
    CC_SYNTHESIZE_READONLY(Point, m_LAnchorPoint, LAnchorPoint );	//左上的挂点
    CC_SYNTHESIZE_READONLY(Point, m_RAnchorPoint, RAnchorPoint );	//右上的挂点
    CC_SYNTHESIZE_READONLY(Point, m_DAnchorPoint, DAnchorPoint );	//脚上的挂点
    
    CC_SYNTHESIZE_READONLY(int, m_standBy, StandBy );	//待命动作序列
    CC_SYNTHESIZE_READONLY(int, m_move, Move );     //移动动作序列
    CC_SYNTHESIZE_READONLY(int, m_death, Death );	//死亡动作序列
    
    CC_SYNTHESIZE_READONLY(bool, m_stopMove, StopMove );    //静止移动标志
    //实现父类的虚方法，
    virtual void setDirection(LQUnitDirectionType direction);

    
    // update
    void update(float dt, Layer *layer);
    
    void _updateWaitCommand();       //等待
    void _updateAttackCommand();     //攻击
    void _updateApprochCommand(GameObj* lockedunit);    //索敌，追踪敌人
    void _updateMoveCommand(Point dest=Point::ZERO); //索敌状态，移动到目的地
    void _updateEvadeCommand();      //逃跑或者后撤
    void _updateDieCommand();        //死亡
    void _updateFollowCommand();     //跟随

    //改变方向去面对目标
    void changeDirection(GameObj* target);
    void changeDirection(Point targetTileCoord);
    void changeDirectionWithPoint(Point targetpt);
    //被目标锁定
    void locked(GameObj* obj);
    void unlocked(GameObj* obj);
    bool isCanLocked(GameObj* obj);  //是否能锁定
    int getLockedCount(){ return m_lockedcount+m_lockedremotecount; };
    void setTarget(GameObj* target);
    //标题连线位置
    Point getLinePosition(Point pt);
    //获取攻击目标的位置
    virtual Point getTargetAttackTileCoord(GameObj* obj);
    // 我要站在这里，看看能不能
    bool isCanStand();
    bool doStand(bool isattack);
    // 目标消失了
    void onLostTarget();
    // 死亡消失
    virtual void changeAnimTo_CorpseToGrave();
    void corpseEffect();
    virtual void afterCorpseShake();
     //删除BUFF Animation
    void removeBufferAnimation(int animaTag);
    //静止移动
    void stopMove(bool value);
    //禁锢
    void durance(bool value);
    
    // 更改动画
    virtual void changeAnimation(){};
    //更改指定动作changeAnimation
    void changeAnimationByType(LQUnitAnimPhaseEnum phaseType,LQUnitDirectionType direction);
    
    //数据获取
    int getHPValue();
    
    //是否还在集结 true集结 false集结中
    inline bool isArrayed() {
        return m_arrayPoint==Point::ZERO;
    }
    
    //是否还在技能释放中
    inline bool isSkilling() {
        return m_isSkilling;
    }
    
    //军团旗手
    inline LQCombatUnit* getBannerman();
protected:
    bool m_isDeadhasSkill;        //死了但本轮存在技能释放
    bool m_isSkilling;            //技能释放中，只能更改技能动作，负责skillrelease没有了调用基础
    LQUnitAnimPhaseEnum m_oldphaseType;
    vector<int> m_buff_tags;  //记录buff对象tag，只能存放5个
    //int m_buff_body_tag;
    //int m_buff_foot_tag;
    int m_buff_ico;      //头上顶的图标
    string _currentHeadFrameName;          //当前的FrameName
    Animation *_currentAnimation;  //当前的animation;
    int m_lockedcount;      //被锁定数量
    int m_lockedremotecount; //被远程锁定数量
    Point m_bookTileCoord;  //订的位置
    bool m_isBattle;        //进入战斗地图了
    uint m_coprseColorIndex;//死亡消失
    int m_cdTime;
    
    //初始化数据
    void initData();
    //状态设置
    void setState(LQUnitStateEnum state);
    virtual void onExitState(LQUnitStateEnum state);
    virtual void onEnterState();
    //设置站立状态
    virtual void setIsStand(bool value);
    //攻击方向求
    LQUnitDirectionType getDefaultDirection();
    // 阴影管理器
    LQCombatUnitShadow* m_shadow;
    // 冲锋管理器
    LQCombatUnitForward* m_forward;
    void changeForwardDirection(LQUnitDirectionType direction);
    
    // 更改位置坐标，其中校验是否可以进入
    void movebyPostion(const Point& move);
    // 移动到下一个空位置
    void moveNextPassPostion(Point tileCoord);
    void moveNextStandPostion();
    
    //
    //virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
private:
    
    CustomCommand   _customDebugDrawCommand;
    void drawLineData(const kmMat4& transform, bool transformUpdated);
protected:
    LQBuffSprite* m_haloSprite;   //武将下面的光圈
    LQBattleSession* _session;
    
    //测试显示
    void testHeadLabel();
};

//typedef vector<LQCombatUnit*> LQCombatUnitVector;

LQ_NAMESPACE_END

#endif /* defined(__game__LQCombatUnit__) */