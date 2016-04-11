//
// LQBattleSession.h
// SuperWarriors
// 游戏数据类
// Created by chenli on 14-6-18.
// Copyright lequworld.com
//

#ifndef __SuperWarriors__LQBattleSession__
#define __SuperWarriors__LQBattleSession__

#include "cocos2d.h"
#include "LQCore.h"
#include "LQTemplate.h"
#include "fusm/GameObj.h"
#include "fusm/AIObj.h"
#include "fusm/AIController.h"
#include "unit/LQCombatSquad.h"

USING_NS_CC;
LQ_NAMESPACE_USE;

LQ_NAMESPACE

typedef enum{
    klqBattleStatusNone = -1,      //无
    klqBattleStatusBegin = 0,      //准备
    klqBattleStatusRun = 1,        //冲锋中
    klqBattleStatusFight = 2,      //战斗中
    klqBattleStatusEnd  = 3,       //战斗结束
    klqBattleStatusOver  = 4       //结束
} LQBattleStatusEnum;

const int DEF_BATTLE_TIMEMAX = 90;

//8方向 5人  后排比率同中间哪个点 {x宽，y高}
const float FORMAT_POSTION[8][5][2] = {
    { {1,0.5}, {0.5,0.5}, {0,0.5}, {-0.5,0.5}, {-1,0.5} },  //下S 0
    { {0.5,-2}, {0.5,-1}, {0.5,0}, {0.5,1}, {0.5,2} },  //左W 1
    { {-1,-0.5}, {-0.5,-0.5}, {0,-0.5}, {0.5,-0.5}, {1,-0.5} },  //上N 2
    { {-0.5,2}, {-0.5,1}, {-0.5,0}, {-0.5,-1}, {-0.5,-2} },  //右E 3
    { {1.5,-0.5}, {1,0}, {0.5,0.5}, {0,1}, {-0.5,1.5} },  //左下SW 4
    { {-0.5,-1.5}, {0,-1}, {0.5,-0.5}, {1,0}, {1.5,0.5} },   //左上NW 5
    { {0.5,1.5}, {0,1}, {-0.5,0.5}, {-1,0}, {-1.5,-0.5} },  //右下SE 6
    { {-1.5,0.5}, {-1,0}, {-0.5,-0.5}, {0,-1}, {0.5,-1.5} }  //右上NE 7
};

//{ {2,1}, {1,1}, {0,1}, {-1,1}, {-2,1}, {1,2} },  //左下SW
//{ {-2,-1}, {-1,-1}, {0,-1}, {1,-1}, {2,-1}, {1,2} },  //左上NW
//{ {1,-2}, {1,-1}, {1,0}, {1,1}, {1,2}, {2,1} },  //右下SE
//{ {-1,2}, {-1,1}, {-1,0}, {-1,-1}, {-1,-2}, {2,1} }   //右上NE

//游戏数据类
class LQBattleSession : public Node
{
public:
    LQBattleSession();
    ~LQBattleSession();
    
    virtual bool init();
    CREATE_FUNC(LQBattleSession);
    
    static LQBattleSession* getInstance();
    
    // 增加对象
	void addGameObj(GameObj *obj);
    // 增加军团
    void addGameSquad(LQCombatSquad *obj);
    
    //game related fucntions
    void initGame(Node* layer);     //初始化游戏
    void initBattleData(string valuestr); // 处理战斗数据
    
    string m_fightRoleID;//pvp战斗时的对手数据
    int m_fightType;    //0:演示关卡 1: 普通关卡 2: 列传 3: 排行榜挑战 4: 经验关卡 5 良驹争夺 6 奇遇战斗 7:掠夺拦截 8:南蛮入侵
    bool m_isWin;
    
    void startGame();                  //开始战斗冲锋
    void startBattle();                //开始战斗
    void endBattle();				   //战斗结束
    bool victoryConditon();			   //胜负条件
    void gameOver();				   //游戏结束
    void totalGame(bool init = false); //统计数据
    void gameGiveup();                 //放弃游戏
    void clearGame();                  //清除游戏数据
    void sendResultMsg(int isWin,int recStar);      //发送消息
    //
    void setCtrlLayer(Node* layer);
    //刷帧
    void update(float dt, Layer* layer);
    CC_SYNTHESIZE_READONLY(Layer*, m_fightLayer, FightLayer);
    
    //获取范围类的对象
    GameObj* getClosestGameObj(GameObj* obj, int type);
    LQCombatSquad* getClosestCombatSquad(LQCombatSquad* obj, LQArmsTypeEnum armsType=Combat_ArmsType_None);
    GameObj* getGameObj(int idx){ if(idx<m_activeObjs.size()) return m_activeObjs.at(idx); return NULL; };
    LQCombatSquad* getCombatSquad(int idx){ if(idx<m_squadList.size()) return m_squadList.at(idx); return NULL; };
    //获取obj锁定的最近的对象
    LQCombatUnit* getLockedUnit(GameObj* obj, LQCombatSquad* squad);
    //根据index匹配军团，用在阵型索敌
    LQCombatSquad* getMatchingCombatSquad(LQCombatSquad* squad);
    //记录每个对象的格子位置，用于检测碰撞
    void recordTileCoord(const Point& tileCoord,LQCombatUnit* obj);
    //去除格子上的对象计数（死亡时）
    void deleteTileCoord(GameObj* obj);
    //获取格子上的对象计数
    int getTileCoordCount(const Point& tileCoord);
    //校验格子是否可以通过
    bool checkPassTileCoord(const Point& tileCoord);
    //校验格子是否可以站着 self = 1 自己还没有进去
    bool checkStandTileCoord(const Point& tileCoord,GameObj* obj, int m);
    // 判断格子是否被包围了 n = 1 检查范围
    bool checkSurroundTileCoord(const Point& tileCoord, int n = 1);
    // 我要锁定这个格子，去哪里战斗，你们不要抢了
    void bookTileCoord(GameObj* obj,const Point& tileCoord);
    void unbookTileCoord(GameObj* obj);
    // 检查格子被人订的情况
    bool isNotBookTileCoord(int index,const Point& tileCoord);
    bool isBookTileCoord(int index,const Point& tileCoord);
    bool getIsReplay();
    
    //参数
    CC_SYNTHESIZE_READONLY(float, m_rage_attack, RageATTACK);  //攻击怒气基数
    CC_SYNTHESIZE_READONLY(float, m_rage_defend, RageDEFEND);  //伤害怒气基数
    CC_SYNTHESIZE_READONLY(float, m_rage_atkmax, RageATKMAX);  //攻击怒气上限
    CC_SYNTHESIZE_READONLY(float, m_rage_defmax, RageDEFMAX);  //伤害怒气上限
    
    //速率 1x 2x 3x
    CC_SYNTHESIZE_READONLY(int, m_gameSpeed, GameSpeed);
    // 战斗状态 0 准备 1 战斗 2 结束
    CC_SYNTHESIZE_READONLY(LQBattleStatusEnum, m_status, Status);
    
    // get available tile coord from direction
    Point getAvailableTileCoord(LQCombatUnit* fromUnit,GameObj* targetUnit, uint overTile);
    // 获取范围内的兵（包含将 ） type 0 全部   1 同一阵列   2相反阵列
    GameObjectVector& getGameObjListInDistance(int skillID, LQCombatHero* obj,int type, Point touchPoint = Point::ZERO);
    // 获取技能的目标对象
    GameObjectVector* getGameObjListInSkill(LQEffectUserData* effectData, LQCombatHero* obj);
    // 获取目标对象范围内的多个对象
    GameObjectVector* getGameObjListInArea(int area, LQCombatSquad* obj, LQCombatSquadDirectionEnum squadDirection);
    void getGameObjListInArea(GameObjectVector& vect, int area, GameObj* obj, LQCombatSquadDirectionEnum squadDirection);
    // 获取目标对象范围内的最近或者最远的对象 obj当前对象 vtype 1 最近 2 最远
    void getGameObjListMinMaxInArea(GameObjectVector& vect, int area, GameObj* obj, bool myself, int vtype);
    // 获取随机目标列表
    void getGameObjListInRand(GameObjectVector& vect, GameObjectVector* temps, int count);
    // 根据条件获取对象数量  valType 1 死亡 2 存活
    int getCombatSquadCount(bool myself, int valType);
    int* getCombatSquadCount(LQCombatHero* obj);
    
    //获取自动战斗时的攻击对象
    //LQCombatUnit* getAutoSkillTarget(LQCombatHero* obj);
    //是否是自动战斗
    CC_SYNTHESIZE(bool, m_isAutoFight, IsAutoFight);
    CC_SYNTHESIZE(bool, m_isCanAutoFight, IsCanAutoFight);
    
    //释放被动技能
    void releasePassiveSkill();
    
    //军团数据列表
    CC_SYNTHESIZE_PASS_BY_REF(LQHeroUserDataVector, m_heroDatas, HeroDatas);
    CC_SYNTHESIZE_READONLY(LQHeroUserDataVector, m_leftheroDatas, LeftHeroDatas);
    CC_SYNTHESIZE_READONLY(LQHeroUserDataVector, m_rightheroDatas, RightHeroDatas);

    //关卡ID
    LQZoneInfoData* m_zoneData;
    CC_SYNTHESIZE_READONLY(int, m_zoneID, ZoneID);
    //扫荡
    CC_SYNTHESIZE_READONLY(bool, m_isRaids, IsRaids);
    //获得战斗星级
    CC_SYNTHESIZE_READONLY(int , m_resStar, ResStar);
    
    //战斗场景数据
    CC_SYNTHESIZE_READONLY(std::string, m_battlaMap, BattlaMap );	//战斗场景的背景地图
    CC_SYNTHESIZE_READONLY(int, m_smokeEffect, SmokeEffect );	//烟尘特效ID
    CC_SYNTHESIZE_READONLY(std::string, m_wav1, Wav1 );	//场景音乐
    CC_SYNTHESIZE_READONLY(std::string, m_wav2, Wav2 );	//环境音效
    CC_SYNTHESIZE_READONLY(int, m_battleMonster, BattleMonster );	//城池中的怪物组ID
    CC_SYNTHESIZE_READONLY(int, m_moneyAward, MoneyAward );	//通关后固定的金钱奖励
    CC_SYNTHESIZE_READONLY(int, m_soulAward, SoulAward );	//通关后固定的将魂奖励
    CC_SYNTHESIZE_READONLY(bool, m_isHasMelee, IsHasMelee );  //存在近战兵种
    
    CC_SYNTHESIZE_READONLY(std::string, m_battleValueStr, BattleValueStr );  //存战斗数据字符串
    
    //回调事件
    std::function<void(Ref*)> onStartBattleCallback;
    
    //用于处理接受到的消息数据
    void receiveMassage();
    
    //战斗资源管理
    CC_SYNTHESIZE_READONLY(vector<std::string>, m_heroResourses, HeroResourses );
    //自动战斗引导是否开启
    CC_SYNTHESIZE_READONLY(bool, m_AutoFightGuide, AutoFightGuide );
    //自动战斗引导是否开启
    CC_SYNTHESIZE_READONLY(std::string, m_CondDesc, CondDesc );
private:
    //对象的唯一序号
    int m_objectindex;
    int max_width;
    int max_heigth;
    float m_gameover;
    float m_bgmusic_dt;  //看着背景音乐
    float m_bgmusic_dt0;
    int m_victoryType; //
    std::vector<std::string> m_victoryCond;  //结束条件
    //对象放置层
	Node* m_layer;
    //
    GameObjectVector inDistanceList;
    bool m_isOldWin;    //回放时战斗输赢
    bool m_isReplay;    //回放
    
    void initData();
    
    // obj 技能触发方 myself 自己一方的 occu  兵种类型
    void getCombatSquadByCondition(GameObjectVector& vect, LQCombatHero* obj,  bool myself, LQArmsTypeEnum arms, bool isTactician = false);
    //获取对象的数值 hero 0所有 1英雄 2兵团 valType 0为生命
    void getCombatSquadByValue(GameObjectVector& vect, LQCombatHero* obj, bool myself, int hprate, int valType);
protected:
    //记录所有对象
    GameObjectVector m_activeObjs;
    //军团所有对象
    LQCombatSquadVector m_squadList;
    std::map<string,LQCombatSquad*> m_squadMap;
    
    
    //记录格子中的对象数量    格子key:数量
    ValueMap m_tileCoordCounts;
    //记录对象锁定目标格子的数据  格子key:index
    ValueMap m_tileCoordBooks;
    
    //记录的评价数据
    CC_SYNTHESIZE_READONLY(float, m_battleTimes, BattleTimes); //战斗时长
    int m_leftSquadCount;      //本方军团数
    int m_leftSquadCountMax;   //本方军团数，判断阵亡数量
    int m_rightSquadCount;     //敌方军团数
    //军团HP
    CC_SYNTHESIZE_READONLY(int, m_leftSquadHP, LeftSquadHP);        //我方
    CC_SYNTHESIZE_READONLY(int, m_rightSquadHP, RightSquadHP);      //对方
    CC_SYNTHESIZE_READONLY(int, m_leftSquadHPMax, LeftSquadHPMax);  //我方
    CC_SYNTHESIZE_READONLY(int, m_rightSquadHPMax, RightSquadHPMax);//对方
    int m_rightSquadHPMaxInit; //初始化值，上一次战斗已经死去的，一般用于boss
    int m_leftSquadHPMaxInit;  //初始化值，上一次战斗已经死去的，一般用于boss
    //军团战力
    CC_SYNTHESIZE_READONLY(int, m_leftSquadFightPower, LeftSquadFightPower);    //我方
    CC_SYNTHESIZE_READONLY(int, m_rightSquadFightPower, RightSquadFightPower);  //对方
    //军团级别
    CC_SYNTHESIZE_READONLY(int, m_leftSquadLevel, LeftSquadLevel);    //我方
    CC_SYNTHESIZE_READONLY(int, m_rightSquadLevel, RightSquadLevel);  //对方
    
};

LQ_NAMESPACE_END

#endif /* defined(__game__LQBattleManager__) */