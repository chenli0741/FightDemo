//
// LQCombatSquad.h
// SuperWarriors
// 战斗队伍类
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#ifndef __SuperWarriors__LQCombatSquad__
#define __SuperWarriors__LQCombatSquad__

#include "cocos2d.h"

#include "LQStringExtend.h"
#include "LQTemplate.h"
//#include "LQCombatUnit.h"

#include "LQCombatSoldier.h"
#include "LQCombatUnitEnums.h"
#include "LQCombatHero.h"
#include "../data/LQData.h"
#include "../fusm/FuSMState.h"
#include "./Component/LQFlagsSprite.h"
#include "./Component/FlagPosition.h"
#include "./Component/LQBuffSprite.h"

USING_NS_CC;
using namespace std;

LQ_NAMESPACE

// 是否显示普通伤害
#define SHOW_NORMAL_HURT    1


// 等待
const int MARCH_ACTION_WAITING = -1;
// 靠近目标
const int MARCH_ACTION_FOLLOW = 100;
// 阵型的每行的方阵个数
const int MAX_SQUAD_COLS = 5;
// 阵型攻击最小格子
const int MIN_SQUAD_ATK = 4;
// 最后一排冲锋同速度的距离
const int SQUAD_BACKROW_RUNS = 10;
// 冲锋效果持续时间
const int SQUAD_FORWARD_RUNS = 5;
// 枪击效果持续时间
const int SQUAD_ASSAULT_RUNS = 1.8;
// 最小冲锋速度
const int SQUAD_MIN_RUNSPEED = 700;

//战斗军团动作
class LQCombatSquadAction {
    // 动作索引
    CC_SYNTHESIZE(int, m_actionIndex, ActionIndex);
    // 格子数量
    CC_SYNTHESIZE(int, m_tileSize, TileSize);
public:
    LQCombatSquadAction(int actionIndex, int tileSize):
            m_actionIndex(actionIndex), m_tileSize(tileSize) {};
};

//战斗军团
class LQCombatSquad : public AIObj
{
public:
    static const char* EVENT_FORWARD_EFFECT;
    
    //code 兵种代码 scolor 阵营 count 兵数量
    //LQCombatSquad(string code,string scolor,int count,LQCombatSquadDirectionEnum direction);
    LQCombatSquad(LQHeroUserData* hero,LQCombatSquadDirectionEnum sqDirection);
    ~LQCombatSquad();
    
    virtual bool init();
    //CREATE_FUNC(LQCombatSquad);
    //static LQCombatSquad* create(string code,string scolor,int count,LQCombatSquadDirectionEnum direction = klqCombatSquadDirectionLeft);
    static LQCombatSquad* create(LQHeroUserData* hero,LQCombatSquadDirectionEnum sqDirection = klqCombatSquadDirectionLeft);
    
    //队列最多人数 现在最多是3*6
    static const int SQUAD_MAXCOL;      //队列每行的最多人数
    static const int SQUAD_NUMCOL;
    static const int SQUAD_MAXROW;      //队列最多行数
    
    float archerDM;     //弓兵造成伤害系数修正
    float archerHurt;   //弓兵受到伤害系数修正
    
    //军团的数据
    CC_SYNTHESIZE_READONLY(LQSquadUserData*, m_squadData, SquadData);
    //军团的英雄(只有一个)
    CC_SYNTHESIZE_READONLY(LQHeroUserData*, m_herodata, HeroData);
    CC_SYNTHESIZE_READONLY(LQCombatHero*, m_hero, Hero);
    //兵的队列数据
    CC_SYNTHESIZE(LQCombatSoldierVector*, m_combatUnits, CombatUnits) //队列数据
    int getSoldiersCount(){ return m_combatUnits->size(); };
    
    //所有士兵的血量
    CC_SYNTHESIZE_READONLY(int, m_totalHurtHp,TotalHurtHp); //当前的伤害总量
    void modifyHurtHp(int hurt, bool iscrit, string critPic, string critFont, bool isskill, Point pt=Point::ZERO, int times=1, float outtime = 0);               //变更，添加新的伤害，足够杀一个处理
    bool isCanKillUnit(){  return m_totalHurtHp>=m_UnitHp; }; //伤害足够杀一个单元
    void killUnitHP();      //杀一个单元
    int getSquadHpValue(){ return m_squadData ? m_squadData->getSquadHpValue() : 0; };  //获取当前的伤害总量
    int getSquadHpValueMax(){ return m_squadData ? m_squadData->getHpValueMax() : 0; };
        
    //获取假死的unit
    LQCombatSoldier* getDieUnit();
    
    //获取当前的攻击力  通过类型区分
    int getCurrentAps(LQCombatSquad* target,LQGameObjType type);
    
    //获取对象的战斗公式计算器
    LQAttackDataInterface* getCalculator();
    
    //攻击频率 时间；
    CC_SYNTHESIZE_READONLY(float, m_attackFrequency, AttackFrequencye);
    
    //显示自身位置的特效（刀光，闪避，格挡，加速等）
    void showForwardEffect(LQShowEffectTypeEnum eftype);
    
    //技能单个命中特效
    void skillHitEffect(LQEffectUserData* effectData,int m);
    
    //用于军团对目标（兵团/将）的伤害
    void timeAttack();
    //攻击目标,目标的血量变化、、以及攻击者与被攻击者的怒气的处理 outtime控制飘字时间
    void changeTargetDps(GameObj* pTarget,int dps,LQFlyNumberTypeEnum numbertype = klq_flaytype_normal,LQEffectInfoData* effectdata=NULL,int times = 0, float outtime=0);
    //获取攻击对象为武将或者兵团的士兵的个数
    int getActiveSoldierCountByAttackType(LQGameObjType type);
    
    //变更怒气 最大不得超过最大怒气上限
    void changeHeroFuries(int value);
    
    //军团Index 队列索引 1-9
    void setIndex(int value);
    string getKey();
    //方阵在阵型中的位置 X(1-3) Y(1-6)
    CC_SYNTHESIZE_READONLY(int, m_squad_x, SquadX)
    CC_SYNTHESIZE_READONLY(int, m_squad_y, SquadY)
    //军团队列方向(对型方向，站的方向)
    CC_SYNTHESIZE(LQCombatSquadDirectionEnum, m_squadDirection, SquadDirection) //队列方向
    // 索敌策略方阵index: 军团序号
    void setTarget(GameObj* target);
    // 预定的敌人，由阵型索敌决定，当视野打开后直接转成正式目标
    CC_SYNTHESIZE_READONLY(LQCombatSoldier*, m_planTarget, PlanTarget)
    // 军团状态
    CC_SYNTHESIZE(LQSquadStateEnum, m_state, State);
    // 移动速度（毫秒/格子)   公式：军团的速度是，最慢单元的速度slowSpeed   slowSpeed*(1.02+m_patrolSpeed/100)
    CC_SYNTHESIZE_READONLY(float, m_speed, Speed );
    // 巡逻移动速度（毫秒/格子)
    CC_SYNTHESIZE_READONLY(float, m_patrolSpeed, PatrolSpeed );
    // 突击加速增加速度(比率) 公式： m_squad->getSpeed()*m_squad->getAddSpeed()
    CC_SYNTHESIZE_READONLY(float, m_addSpeed, AddSpeed );
    // 获取行最慢速度
    CC_SYNTHESIZE(float, m_slowSpeed, SlowSpeed );
    // 获取开场冲锋速度 squadSpeed*squadAddSpeed
    float getRunSpeed();
    // 军团控制范围 最大半径（单位：格子）
    CC_SYNTHESIZE_READONLY(int, m_radius, Radius );
    // 军团视野
    CC_SYNTHESIZE_READONLY(int, m_view, View );
    // 军团攻击视野
    CC_SYNTHESIZE_READONLY(float, m_atkview, AtkView );
    float getCurrAtkView(); //获取当前攻击距离，弓兵会被技能触发成狂暴变成近战兵种
    //军团兵种\r\n1、步兵\r\n2、远程\r\n3、骑兵\r\n4、机械
    CC_SYNTHESIZE_READONLY(LQArmsTypeEnum, m_armsType, ArmsType );
    //武将职业\r\n1、普通\r\n2、策士
    CC_SYNTHESIZE_READONLY(LQOccupationEnum, m_occupation, Occupation );
    // 军团是否集结完毕
    CC_SYNTHESIZE(bool, m_israllied, IsRallied );
    void doArrived(LQCombatUnit* unit);  //集结计数，现在不需要记录谁到没到
    // 军团静止，站住
    CC_SYNTHESIZE(bool, m_isStand, IsStand );
    // 军团开始攻击  （新的结算需求）
    CC_SYNTHESIZE(bool, m_isAttacking, IsAttacking );
    // 军团第一次索敌  骑兵AI控制
    CC_SYNTHESIZE_READONLY(bool, m_firstLock, FirstLock );
    // 军团被新的攻击者锁定,用于切换攻击目标
    CC_SYNTHESIZE_READONLY(vector<LQCombatSquad*>*, m_attackSquads, AttackSquads );
    CC_SYNTHESIZE_READONLY(float, m_attackSquadTime, AttackSquadTime );
    bool getAttackSquadMelee(); //是否可以转换目标，true可以转换
    // 军团的冲锋效果
    CC_SYNTHESIZE(LQShowEffectTypeEnum, m_forwardEffect, ForwardEffect ) ;
    CC_SYNTHESIZE_READONLY(LQShowEffectTypeEnum, m_oldforwardEffect, OldForwardEffect ) ;
    float getShowEffectDuration(){ return m_showEffectDuration; };  //有些状态的消失有时间，例如枪击 >0仍然在持续范围里
    
    //获取目标的军团对象
    LQCombatSquad* getTargetSquad(GameObj* target=NULL);
    //设置目的地
    void setTargetPoint(Point target);
    //放置到场景上
    void putLayer(Node* parent, short index);
    void drawSquadFormat(LQUnitDirectionType direction);
    void drawSquadPoint(Point postion);
    //显示隐藏方阵
    CC_SYNTHESIZE(bool, m_squaVisible, SquaVisible ) ; //阵型显示状态
    void drawVisible(bool val);
    //朝目标targetpt走，旋转阵型 列阵，先检查是否需要列阵，设置列阵数据
    void trunRound(Point targetpt);
    //变换阵型
    void trunFormation(Point targetpt);
    //绘制旗帜的内容
    void drawmArmflags(float scale = 0);
    //变更旗帜的状态
    void drawmArmflagsStatus();
    //某个单元死了
    void dieUnit(LQCombatUnit* unit);
    //到达目的地
    void doArrivedDestination();
    
    //update
    virtual void update(float dt, Layer* layer);
    
    // 获取军团位置，位置由英雄位置决定
    Point getLQPosition();  //调用getPosition会不对
    virtual Point getTileCoord();
    
    // state update
    void updateWaitCommand();       //等待
    void updateRunCommand();      //开始冲锋
    void updateAttackCommand(LQCombatSquad* lockedSquad=NULL);  //攻击 lockedSquad存在值是索敌并直接攻击
    void updateApprochCommand(LQCombatSquad* lockedSquad); //索敌，追踪敌人
    void _updateMoveCommand();      //
    void updateGotoCommand(Point dest);      //去往目的地坐标
    void updateArrayCommand(Point targetpt); //集结 整理队型状态
    
    //被嘲讽
    void taunt(GameObj* taunter);
    
    // 是否还在行军
    inline bool isOnMarch() {
        return m_state != FUSM_SQSTATE_WAIT;
    }
    
    // 是否可以改变AI  攻击中，或者攻击单元over了
    inline bool isOnChangeAI() {
        return m_state != FUSM_SQSTATE_ATTACK || (m_target && !m_target->getActive());
    }
    
    // 是否处于绕路状态
    inline bool isOnMustWays() {
        return this->getHero() && this->getHero()->getMustPoints().size()>0;
    }
    
    // 是否处于绕路状态
    inline bool isOnWays() {
        return this->getHero() && (this->getHero()->getMustPoints().size()>0 || this->getHero()->getWayPoints().size()>0);
    }
    
    // 是否近战攻击
    inline bool isAttackMelee() {
        return m_isAttackMelee;
    }
    
    //获取HP比率
    int getHPRate()
    {
        int a = (m_squadData->getSquadHpValue()*1.0/m_squadData->getHpValueMax())*100;
        return a;
    }
    
    //军团旗手
    LQCombatUnit* getBannerman(){ return m_bannerman; };
   
    //攻击次数
    int getAtkCount(){ return _atkCount; };
    
    //获取中心位置
    Point getHitPosition();
protected:
    // make next
    void makeNextAction();
    
    //队伍集结完毕调用
    void onRallydCompleted();
    
private:
    int _atkCount;   //攻击次数
    float _squadfuries;  //击杀怒气
    float m_attackCdTime;//攻击累计时间
    unsigned int m_moveeffect;
    float m_showEffectDuration;
    LQBuffUserData* _hdbufferData; //护盾BUFF
    
    //int m_HpValue;
    int m_HpValueMax;  //记录最大血
    int m_UnitHp;      //单兵的血量，即可以造成单兵的死亡
    
    Node* m_parent;
    
    bool m_isAttackMelee;   //是否开始近战
    
    LQFlagsSprite* m_armflags;      //军团旗帜
    LQCombatUnit* m_bannerman;      //军团旗手
    int m_isRalliedCount;           //需要集结的人数
    int m_chargeSpeed;              //
    // 后续动作序列
    std::vector<LQCombatSquadAction*> m_nextActionList;
    
    void initData();
    
    //设置军团速度变化系数 value 1.0 正常 2.0 索敌加速 0.0 攻击  废弃
    void setChargeSpeed(float value);
    
    //BUFF
    void addBuffer(LQBuffUserData* bufferData);     //添加BUFF
    void removeBuffer(LQBuffUserData* bufferData);  //删除BUFF
    void cleareBuffer();
    void addBufferBmp(LQBuffUserData* bufferData);
    //覆盖方法
    virtual void bufferCallback(Ref* pSender);     //buff触发数据动作
    virtual void bufferEndCallback(Ref* pSender);  //buff结束动作
    
    //阵型移动音效
    void doCombatMoveEffect();
};


typedef vector<LQCombatSquad*> LQCombatSquadVector;

LQ_NAMESPACE_END

#endif /* defined(__game__LQCombatSquad__) */