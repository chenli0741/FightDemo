//
// LQBattleSession.cpp
// SuperWarriors
// 战斗环境参数管理类
// Created by chenli on 14-6-18.
// Copyright lequworld.com
//

#include "LQBattleSession.h"
#include "LQCombatSquad.h"
#include "LQCombatUnit.h"
#include "LQData.h"
#include "LQFightLayer.h"
#include "AudioManager.h"
#include "mesage/ZoneFightResultRequest.h"
#include "mesage/BiogFightResultRequest.h"
#include "mesage/ExpLevelFightResultRequest.h"
#include "mesage/ChallengeRivalRequest.h"
#include "mesage/HorseBattleFightResultRequest.h"
#include "mesage/GetBubbleEventPVPAwardRequest.h"
#include "mesage/RationInterceptRequest.h"
#include "mesage/BarbarianFightResultRequest.h"
#include "mesage/CampaignFightResultRequest.h"
#include "BattleReturnLayer.h"
#include "../Socket/ComService.h"
//
//
#if !defined(LQ_DEMO_FIGHT)
#include "CCLuaEngine.h"
#include "HclcData.h"
#include "LoadBattlefield.h"
#endif

LQ_NAMESPACE

static LQBattleSession *_sharedBattleSession = nullptr;
//前端数据缓存类
LQBattleSession* LQBattleSession::getInstance()
{
    if (! _sharedBattleSession)
    {
        _sharedBattleSession = new LQBattleSession();
        _sharedBattleSession->init();
    }
    
    return _sharedBattleSession;
}


LQBattleSession::LQBattleSession():m_gameSpeed(1), m_layer(NULL),m_leftSquadFightPower(0),m_rightSquadFightPower(0),m_isAutoFight(false),m_isRaids(false),m_fightRoleID(""),onStartBattleCallback(nullptr),m_isHasMelee(false),m_zoneData(NULL),m_CondDesc("")
{
    max_width = 1024;
    max_heigth = 768;
    m_smokeEffect = 40028001;	//烟尘特效ID
    m_fightLayer = NULL;
}

LQBattleSession::~LQBattleSession()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQBattleSession::~LQBattleSession  %p", this);
    //
    releaseInVector(m_activeObjs);
    //
    releaseInVector(m_squadList);
    //
    m_heroDatas.clear();
    m_leftheroDatas.clear();
    m_rightheroDatas.clear();
}

bool LQBattleSession::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    initData();
    return true;
}

//清除游戏数据
void LQBattleSession::clearGame()
{
    //releaseInVector(m_activeObjs);
    m_activeObjs.clear();
    releaseInVector(m_squadList);
    //
    deleteInVector(m_heroDatas);
    m_heroDatas.clear();
    m_leftheroDatas.clear();
    m_rightheroDatas.clear();
}

#pragma mark - initData
void LQBattleSession::initData()
{
    // add your codes here...
    m_objectindex = 0;
    m_status = klqBattleStatusBegin;
    m_gameSpeed = 0;
    m_battleTimes= 0;
    m_bgmusic_dt = 0;
    m_bgmusic_dt0 = 0;
    m_AutoFightGuide = false;
    m_isWin = false;
}

//---------------------------------------------------------
static bool removeNotActive(GameObj* obj)
{
	if(!obj->getActive())
    {
        if(!obj->isVisible()){
            obj->removeAllChildrenWithCleanup(true);
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------
void LQBattleSession::update(float dt, Layer* layer)
{
	if(layer==NULL)
		return;
    m_fightLayer = layer;
    receiveMassage();
    
    //if (m_status < klqBattleStatusRun)
    //    return;
    if (m_status == klqBattleStatusOver)
        return;
    
    GameObjectVector::iterator list1;
    //check for no main ship, respawn
	for(list1=m_activeObjs.begin();list1!=m_activeObjs.end();++list1)
	{
        AIObj* temp = (AIObj*)*list1;
        if(temp && temp->isVisible())
		{
			temp->getController()->update(dt,layer);
		}
	}
    
	for(list1=m_activeObjs.begin();list1!=m_activeObjs.end();++list1)
	{
        //GameObj* temp = *list1;
		//update logic and positions
        if (!(*list1)->isVisible())
            continue;
        
		if((*list1)->getActive() )
		{
            (*list1)->update(dt,layer);
		}
		else continue;
		
		//check for collisions 检查碰撞
		if((*list1)->getCollisionFlags() != klqLQGameObjNone)
		{
			GameObjectVector::iterator list2;
			for(list2=m_activeObjs.begin();list2!=m_activeObjs.end();++list2)
			{
                //the first obj may have already collided with something, making it inactive
                if(!(*list1)->getActive())
                    continue;
				//don't collide with yourself
				if(list1 == list2)
					continue;
				//增加其他条件判断，是否敌人等等
				//if((*list1)->getType()==(*list2)->getType())
				//	continue;
				
				if((*list2)->getActive()							   &&
                   ((*list1)->getCollisionFlags() & (*list2)->getType()) &&
                   (*list1)->isColliding(*list2))
				{
					//(*list1)->Explode();
					(*list1)->doCollision((*list2));
				}
			}
		}
		if(list1==m_activeObjs.end()) break;
	}
    
	//get rid of inactive objects
	GameObjectVector::iterator end    = m_activeObjs.end();
	GameObjectVector::iterator newEnd = remove_if(m_activeObjs.begin(),m_activeObjs.end(), removeNotActive);
	if(newEnd != end)
        m_activeObjs.erase(newEnd,end);
   
    //军团AI
    m_isHasMelee = false;
    LQCombatSquadVector::iterator list2;
	for(list2=m_squadList.begin();list2!=m_squadList.end();++list2)
	{
        AIObj* temp = (AIObj*)*list2;
        if(temp)
		{
            if (!(*list2)->isVisible())
                continue;
            
            if((*list2)->getActive()){
                if ((*list2)->isAttackMelee())
                    m_isHasMelee = true;
                (*list2)->update(dt,layer);
                //军团AI
                temp->getController()->update(dt,layer);
            }
		}
    }
    
    //时间限制为1.5分钟，若超过时间未能结束战斗判防守方胜利
    if( victoryConditon() || m_battleTimes>DEF_BATTLE_TIMEMAX)
        endBattle();

    if (m_status == klqBattleStatusEnd)
        return;
    
    m_bgmusic_dt += dt;
    m_bgmusic_dt0 += dt;
    if(m_isHasMelee && m_bgmusic_dt>9){
        m_bgmusic_dt = 0;
        int m = randomInt(1, 10);
        //CCLOG( "m_bgmusic 1  %ld   %d", getCurrentMillSecond()/1000, m);
        AudioManager::getInstance()->playEffect(m>5 ? Effect_BG3_Music : Effect_BG4_Music);
    }
    if(m_isHasMelee && m_bgmusic_dt0>10){
        //CCLOG( "m_bgmusic 2  %ld", getCurrentMillSecond()/1000);
        m_bgmusic_dt0 = 0;
        AudioManager::getInstance()->playEffect(Effect_BG1_Music);
        
        AudioManager::getInstance()->playEffect(Effect_Swcombi_1);
    }
    
    //军团数据统计
    this->totalGame();
    m_battleTimes+=dt;
}

void LQBattleSession::receiveMassage()
{
#if !defined(LQ_DEMO_FIGHT)
    int isGameSucceed = LoadBattlefield::getInstance()->getState();
    //CCLOG("LQBattleSession::receiveMassage: %d", isGameSucceed);
    if (isGameSucceed == 1) {
        LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
        
        string fileStr = LoadBattlefield::getInstance()->getValue();
        if (controllayer) {
            controllayer->gameOver(true, m_resStar, fileStr);
        }
        LoadBattlefield::getInstance()->setState(-1);
    }else if (isGameSucceed == 0){
        LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
        string fileStr = LoadBattlefield::getInstance()->getValue();
        if (controllayer) {
            controllayer->gameOver(false, m_resStar,fileStr);
        }
        LoadBattlefield::getInstance()->setState(-1);
    }else if (isGameSucceed == 2 or isGameSucceed == 3)  //良驹争夺战  成功翻牌
    {
        LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
        controllayer->getMessageToFightHorse();
        LoadBattlefield::getInstance()->setState(-1);
    }
    else if (isGameSucceed == 4)
    {
        LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
        if (controllayer) {
            controllayer->gameOver(false, m_resStar, "");
        }
        LoadBattlefield::getInstance()->setState(-1);
    }
    else if (isGameSucceed == 5)
    {
        string fileStr = LoadBattlefield::getInstance()->getValue();
        LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
        if (controllayer) {
            controllayer->gameOver(false, m_resStar,fileStr);
        }
        LoadBattlefield::getInstance()->setState(-1);
    }
    else if (isGameSucceed == 6)  //南蛮入侵
    {
        string fileStr = LoadBattlefield::getInstance()->getValue();
        LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
        if (controllayer) {
            controllayer->gameOver(true, m_heroDatas,fileStr);
        }
        LoadBattlefield::getInstance()->setState(-1);
    }
#endif
}

//获取obj锁定的最近的对象
LQCombatUnit* LQBattleSession::getLockedUnit(GameObj* obj, LQCombatSquad* squad)
{
    if(!squad || !squad->getTarget())
        return NULL;
    LQCombatSquad* target = (LQCombatSquad*)(squad->getTarget());
    LQCombatUnit* result = NULL;
    LQCombatUnit* unit = (LQCombatUnit*)obj;
    float mindist = -1;
    
    if(!unit) return NULL;
    
    if(unit->isBannerman() || target->getCombatUnits()->size()<=0)  //旗手先锁定旗手
    {
        if (target->getBannerman() && target->getBannerman()->getActive() && target->getBannerman()->isCanLocked(unit) && target->getBannerman()!=obj->getOldTarget() )
        {
            return target->getBannerman();
        }
        //CCLOG("IDX%d Bannerman[%d] is not My food... [LOCK %d]", obj->getIndex(), target->getBannerman()->getIndex(), target->getBannerman()->getLockedCount());
    }
    
    //锁定最近的目标
    LQCombatUnit* pUnit = NULL;
    for (int i=0; i<target->getCombatUnits()->size(); i++)
    {
        pUnit = target->getCombatUnits()->at(i);
        if (pUnit && pUnit->getActive() && pUnit->isCanLocked(unit) && pUnit!=obj->getOldTarget()){
            //通过convertTile计算格子距离
            Point p1 = convertTileCoord(pUnit->getPosition());
            Point p2 = convertTileCoord(obj->getPosition());
            float dist = p1.getDistance(p2);
            if(mindist>dist || mindist<0){
                mindist = dist;
                result = pUnit;
            }
        }
    }
    
    /*
    if (target->getBannerman() && target->getBannerman()->getActive()
        && target->getBannerman()->isCanLocked() && target->getBannerman()!=obj->getOldTarget())
    {
        Point p1 = convertTileCoord(target->getHero()->getPosition());
        Point p2 = convertTileCoord(obj->getPosition());
        float dist = p1.getDistance(p2);
        if(mindist>dist || mindist<0){
            mindist = dist;
            result = target->getHero();
        }
    }
    
    //锁定最近的目标
    LQCombatUnit* pUnit = NULL;
    for (int i=0; i<target->getCombatUnits()->size(); i++)
    {
        pUnit = target->getCombatUnits()->at(i);
        if (pUnit && pUnit->getActive() && pUnit->isCanLocked() && pUnit!=obj->getOldTarget()){
            //通过convertTile计算格子距离
            Point p1 = convertTileCoord(pUnit->getPosition());
            Point p2 = convertTileCoord(obj->getPosition());
            float dist = p1.getDistance(p2);
            if(mindist>dist || mindist<0){
                mindist = dist;
                result = pUnit;
            }
        }
    }*/
    
    return result;
}

//获取obj最近的对象
GameObj* LQBattleSession::getClosestGameObj(GameObj* obj, int type)
{
    //go through the list, find the closest object of param "type"
    //to the param "obj"
    float closeDist = 100000000.0f;
    GameObj* closeObj = NULL;
    
    GameObjectVector::iterator list1;
    for(list1=m_activeObjs.begin();list1!=m_activeObjs.end();++list1)
    {
        //watch out for yourself
        if(*list1 == obj)
            continue;
        if(!(*list1)->isVisible())
            continue;
        
        //通过convertTile计算格子距离
        Point p1 = convertTileCoord((*list1)->getPosition());
        Point p2 = convertTileCoord(obj->getPosition());
        float dist = p1.getDistance(p2);
		//float dist = (*list1)->getPosition().getDistance(obj->getPosition());
		if((*list1)->getType() == type && dist< closeDist)
        {
            closeDist = dist;
            closeObj = *list1;
        }
    }
    return closeObj;
    
}

//记录每个对象的格子位置，用于检测碰撞
// tileCoord 新格子位置
void LQBattleSession::recordTileCoord(const Point& tileCoord,LQCombatUnit* obj)
{
    int count = 0;
    LQCombatSoldier* soldier = (LQCombatSoldier*)obj;
    if (!soldier) {
        return;
    }
    //老的格子位置
    Point tilept = soldier->getTileCoord();
    
    string key = TileCoordString(tilept);
    if (m_tileCoordCounts.find(key) != m_tileCoordCounts.end())
    {
        count = m_tileCoordCounts[key].asInt()-1;
        CCASSERT(count>=0, "Lequworld:Old TileCoord is not exist....");
        m_tileCoordCounts[key] = Value(count);
    }else if ( tilept!=Point(-1000, -1000) )
        CCLOGWARN("Lequworld:Old TileCoord is not exist....");
    //新的格子
    key = TileCoordString(tileCoord);
    if (m_tileCoordCounts.find(key) != m_tileCoordCounts.end())
    {
        count = m_tileCoordCounts[key].asInt()+1;
    }else count = 1;
    m_tileCoordCounts[key] = Value(count);
}

//去除格子上的对象计数（死亡时）
void LQBattleSession::deleteTileCoord(GameObj* obj)
{
    int count = 0;
    
    //老的格子位置
    Point tilept = obj->getTileCoord();
    string key = TileCoordString(tilept);
    if (m_tileCoordCounts.find(key) != m_tileCoordCounts.end())
    {
        count = m_tileCoordCounts[key].asInt()-1;
        CCASSERT(count>=0, "Lequworld:Old TileCoord is not exist....");
        m_tileCoordCounts[key] = Value(count);
    }else if ( tilept!=Point(-1000, -1000) )
        CCLOGWARN("Lequworld:Old TileCoord is not exist....");
}

//获取格子上的对象计数
int LQBattleSession::getTileCoordCount(const Point& tileCoord)
{
    string key = TileCoordString(tileCoord);
    if (m_tileCoordCounts.find(key) != m_tileCoordCounts.end()){
        return m_tileCoordCounts[key].asInt();
    }
    return 0;
}

//校验格子是否可以通过, count<3能够通过
bool LQBattleSession::checkPassTileCoord(const Point& tileCoord)
{
    return this->getTileCoordCount(tileCoord)<MAP_TILE_PASSMAX;
}

//校验格子是否可以站着, count<1+self能够进入
//      self = 1 自己已经进去站着了，判断能不能站住不动开打（默认）
//      self = 0 自己还没有进去的预判能不能站住
bool LQBattleSession::checkStandTileCoord(const Point& tileCoord,GameObj* obj, int m)
{
    string key = TileCoordString(tileCoord);
    int self = 0;
    if(obj && obj->getTileCoord()==tileCoord )
        self = 1;

    //我已经预定
    if(obj && isBookTileCoord(obj->getIndex(),tileCoord))
        return true;
    
    //CCLOG("OBJ[%d]->getTileCoordCount(tileCoord):%d", obj->getIndex(),this->getTileCoordCount(tileCoord));
    bool result = this->getTileCoordCount(tileCoord)<(MAP_TILE_STANDMAX + self + m);
    if(result){
        if( obj && isNotBookTileCoord(obj->getIndex(),tileCoord) ) //其他人已经预定
            result = false;
    }
    return result;
}

// 判断格子是否被包围了，周围都是人，？？？？都在战斗，需要改进
bool LQBattleSession::checkSurroundTileCoord(const Point& tileCoord, int n)
{
    //环绕一周
    for (int i = -n; i <=n; i++)
        for (int j = -n; j <=n; j++){
            if ( i==0 && j==0 ) continue;
            int n = this->getTileCoordCount(tileCoord+Point(i,j));
            if(n<=MAP_TILE_STANDMAX)  //有空位
                return false;
        }
    return true;
}

// 我要锁定这个格子，去哪里战斗，你们不要抢了
//    占据了别人就只能通过，不能作为站的位置，而我到了就可以站着，不用管上面有没有人，
void LQBattleSession::bookTileCoord(GameObj* obj,const Point& tileCoord)
{
    if(obj->getIndex()==3)
    {
        //CCLOG("IDX:%d bookTileCoord %s", obj->getIndex(), TileCoordString(tileCoord).c_str());
    }
    string key = TileCoordString(tileCoord);
    m_tileCoordBooks[key] = Value(obj->getIndex());
}

// 站住了就释放这个标志，因为他已经是我的了
void LQBattleSession::unbookTileCoord(GameObj* obj)
{
    if(obj->getIndex()==3)
    {
        //CCLOG("IDX:%d unbookTileCoord %s", obj->getIndex(), TileCoordString(obj->getTileCoord()).c_str());
    }
    string key = TileCoordString(obj->getTileCoord());
    ValueMap::iterator it = m_tileCoordBooks.find(key);
    if (it != m_tileCoordBooks.end()){
        m_tileCoordBooks.erase(it);
    }else{
        //CCLOG("GameObj %d unlockTileCoord is exist", obj->getIndex());
    }
}

// 检查格子是否被人订了  index我的编号
bool LQBattleSession::isNotBookTileCoord(int index,const Point& tileCoord)
{
    if(index<0) return false;
    string key = TileCoordString(tileCoord);
    if (m_tileCoordBooks.find(key) != m_tileCoordBooks.end()){
        return m_tileCoordBooks[key].asInt()!=index;
    }
    return false;
}

// 检查格子是否obj->index订了 index我的编号
bool LQBattleSession::isBookTileCoord(int index,const Point& tileCoord)
{
    if(index==3)
    {
        
    }
    if(index<0) return false;
    string key = TileCoordString(tileCoord);
    if (m_tileCoordBooks.find(key) != m_tileCoordBooks.end()){
        return m_tileCoordBooks[key].asInt()==index;
    }
    return false;
}

//释放被动技能
void LQBattleSession::releasePassiveSkill()
{
    for (int i = 0 ; i < m_squadList.size(); i++) {
        LQCombatSquad* squad = m_squadList.at(i);
        squad->getHero()->showPassiveSkill();
    }
}

//获取范围内的兵（包含将 ）type 0 全部   1 同一阵列   2相反阵列
//    1.玩家点选范围内友方
//    2.玩家点选范围内敌方
//    3.随机选择交战的友军
//    4.随机选择交战的敌军
//    5.施法者为中心友军
//    6.施法者为中心敌军
//    7.我方武将单体
//    8.敌方武将单体
//    9.自身武将所属军团
//    10.目标武将所属军团（友，可主动可被动）
//    11.目标武将所属军团（敌，可主动可被动）
GameObjectVector& LQBattleSession::getGameObjListInDistance(int skillID, LQCombatHero* obj,int type, Point touchPoint)
{
    inDistanceList.clear();
    
    //log("AOE 攻击范围类型 = %d",type);
    srand ((unsigned)time(nullptr));
    if (type == 9) {//9.自身武将所属军团
        inDistanceList.assign(obj->getSquad()->getCombatUnits()->begin(), obj->getSquad()->getCombatUnits()->end());
        inDistanceList.push_back(obj->getSquad()->getHero());
        return inDistanceList;
    }else if (type == 10) {//    10.目标武将所属军团（友，可主动可被动）
        int size = (int)randomInt(0, 10000)%m_leftSquadCount;
        int index = 0;
        for (int i = 0 ; i < m_squadList.size(); i++) {
            LQCombatSquad* squad = m_squadList.at(i);
            if (squad->getSquadDirection() == obj->getSquadDirection()) {
                if (index == size) {
                    inDistanceList.assign(squad->getCombatUnits()->begin(), squad->getCombatUnits()->end());
                    inDistanceList.push_back(squad->getHero());
                    return inDistanceList;
                }
                
                index ++;
            }
        }
        return inDistanceList;
    }else if (type == 11){//    11.目标武将所属军团（敌，可主动可被动）
        int size = (int)randomInt(0, 10000)%m_rightSquadCount;
        int index = 0;
        for (int i = 0 ; i < m_squadList.size(); i++) {
            LQCombatSquad* squad = m_squadList.at(i);
            if (squad->getSquadDirection() != obj->getSquad()->getSquadDirection()) {
                if (index == size) {
                    inDistanceList.assign(squad->getCombatUnits()->begin(), squad->getCombatUnits()->end());
                    inDistanceList.push_back(squad->getHero());
                    return inDistanceList;
                }
                
                index ++;
            }
        }
        return inDistanceList;
    }
    
    LQSkillUserData* skillData = new LQSkillUserData(skillID);
    
    float distance = skillData->getSkillDistance();
    LQCombatSoldierVector::iterator list1;
    for (int i = 0; i < m_squadList.size(); i++) {
        LQCombatSquad* squad = m_squadList.at(i);
        for (list1 = squad->getCombatUnits()->begin();list1!=squad->getCombatUnits()->end();++list1) {
            switch (type) {
                case 1://    1.玩家点选范围内友方
                {
                    if((*list1)->getPosition().getDistance(touchPoint) > distance)
                        continue;
                    if ((*list1)->getSquadDirection() == obj->getSquadDirection()) {
                        inDistanceList.push_back(*list1);
                    }
                }
                    break;
                case 2://    2.玩家点选范围内敌方
                {
                    if((*list1)->getPosition().getDistance(touchPoint) > distance)
                        continue;
                    if ((*list1)->getSquadDirection() != obj->getSquadDirection()) {
                        inDistanceList.push_back(*list1);
                    }
                }
                    break;
                case 3://    3.随机选择交战的友军
                {
                    if ((*list1)->getSquadDirection() == obj->getSquadDirection()) {
                        if (rand()%5 < 5) {
                            inDistanceList.push_back(*list1);
                        }
                    }
                }
                    break;
                case 4://    4.随机选择交战的敌军
                {
                    if ((*list1)->getSquadDirection() != obj->getSquadDirection() && rand()%5 < 5) {
                        inDistanceList.push_back(*list1);
                    }
                    
                }
                    break;
                case 5://    5.施法者为中心友军
                {
                    if((*list1)->getPosition().getDistance(obj->getPosition()) > distance)
                        continue;
                    if ((*list1)->getSquadDirection() == obj->getSquadDirection()) {
                        inDistanceList.push_back(*list1);
                    }
                }
                    break;
                case 6://    6.施法者为中心敌军
                {
                    if((*list1)->getPosition().getDistance(obj->getPosition()) > distance)
                        continue;
                    if ((*list1)->getSquadDirection() != obj->getSquadDirection()) {
                        inDistanceList.push_back(*list1);
                    }
                }
                    break;
                case 7://    7.我方武将单体
                {
                    break;
//                    if((*list1)->getPosition().getDistance(obj->getPosition()) > distance)
//                        continue;
//                    if ((*list1)->getSquadDirection() == obj->getSquadDirection() && (*list1)->getType() == klqLQGameObjHero) {
//                        inDistanceList.push_back(*list1);
//                    }
                }
                    break;
                case 8://    8.敌方武将单体
                {
                    break;
//                    if((*list1)->getPosition().getDistance(obj->getPosition()) > distance)
//                        continue;
//                    if ((*list1)->getSquadDirection() != obj->getSquadDirection() && (*list1)->getType() == klqLQGameObjHero) {
//                        inDistanceList.push_back(*list1);
//                    }
                }
                    break;
                    
                default:
                    break;
            }
        }
        
        
        //针对军团的武将单独处理
        if (squad->getHero() && squad->getHero()->getActive()) {
            
            switch (type) {
                case 1://    1.玩家点选范围内友方
                {
                    if(squad->getHero()->getPosition().getDistance(touchPoint) > distance)
                        continue;
                    if (squad->getHero()->getSquadDirection() == obj->getSquadDirection()) {
                        inDistanceList.push_back(squad->getHero());
                    }
                }
                    break;
                case 2://    2.玩家点选范围内敌方
                {
                    if(squad->getHero()->getPosition().getDistance(touchPoint) > distance)
                        continue;
                    if (squad->getHero()->getSquadDirection() != obj->getSquadDirection()) {
                        inDistanceList.push_back(squad->getHero());
                    }
                }
                    break;
                case 3://    3.随机选择交战的友军
                {
                    if (squad->getHero()->getSquadDirection() == obj->getSquadDirection()) {
                        if (rand()%5 < 5) {
                            inDistanceList.push_back(squad->getHero());
                        }
                    }
                }
                    break;
                case 4://    4.随机选择交战的敌军
                {
                    if (squad->getHero()->getSquadDirection() != obj->getSquadDirection() && rand()%10 < 5) {
                        inDistanceList.push_back(squad->getHero());
                    }
                    
                }
                    break;
                case 5://    5.施法者为中心友军
                {
                    if(squad->getHero()->getPosition().getDistance(obj->getPosition()) > distance)
                        continue;
                    if (squad->getHero()->getSquadDirection() == obj->getSquadDirection()) {
                        inDistanceList.push_back(squad->getHero());
                    }
                }
                    break;
                case 6://    6.施法者为中心敌军
                {
                    if(squad->getHero()->getPosition().getDistance(obj->getPosition()) > distance)
                        continue;
                    if (squad->getHero()->getSquadDirection() != obj->getSquadDirection()) {
                        inDistanceList.push_back(squad->getHero());
                    }
                }
                    break;
                case 7://    7.我方武将单体
                {
                    if(squad->getHero()->getPosition().getDistance(obj->getPosition()) > distance)
                        continue;
                    if (squad->getHero()->getSquadDirection() == obj->getSquadDirection()) {
                        inDistanceList.push_back(squad->getHero());
                    }
                }
                    break;
                case 8://    8.敌方武将单体
                {
                    if(squad->getHero()->getPosition().getDistance(obj->getPosition()) > distance)
                        continue;
                    if (squad->getHero()->getSquadDirection() != obj->getSquadDirection()) {
                        inDistanceList.push_back(squad->getHero());
                    }
                }
                    break;
                    
                default:
                    break;
            }
            
        }
    }
  
    
    CC_SAFE_DELETE(skillData);
    return inDistanceList;
}

// 获取技能的目标对象（New）
GameObjectVector* LQBattleSession::getGameObjListInSkill(LQEffectUserData* effectData, LQCombatHero* obj)
{
    GameObjectVector* vect = new GameObjectVector();
    GameObj* _target = (GameObj*)obj->getTarget();
    if(!_target){  //单兵还没有索敌，有这种情况
        _target = obj->getSquad()->getTarget();;
    }
    switch (effectData->getObjectType()) {
        case 1://1.目标敌将
        {
            if(_target){
                if (_target->getType()==klqLQGameObjHero )
                    vect->push_back(_target);
                else if (_target->getType()==klqLQGameObjSquad )
                    vect->push_back( ((LQCombatSquad*)_target)->getHero() );
            }
        }break;
        case 2://2.个人自身
        {
            vect->push_back(obj);
        }break;
        case 3://3.目标敌军团
        {
            if(_target){
                if (_target->getType()==klqLQGameObjSquad )
                    vect->push_back(_target);
                else if (_target->getType()==klqLQGameObjHero )
                    vect->push_back( ((LQCombatHero*)_target)->getSquad() );
            }
        }break;
        case 4://4.自身军团
        {
            vect->push_back(obj->getSquad());
        }break;
        case 5://5.敌方全体
        {
            this->getCombatSquadByCondition(*vect, obj, false, Combat_ArmsType_None);
        }break;
        case 6://6.我方全体
        {
            this->getCombatSquadByCondition(*vect, obj, true, Combat_ArmsType_None);
        }break;
        case 7://7	敌方全体弓兵
        {
            this->getCombatSquadByCondition(*vect, obj, false, Combat_ArmsType_Remote);
        }break;
        case 8://8	我方全体弓兵
        {
            this->getCombatSquadByCondition(*vect, obj, true, Combat_ArmsType_Remote);
        }break;
        case 9://9	敌方全体步兵
        {
            this->getCombatSquadByCondition(*vect, obj, false, Combat_ArmsType_Melee);
        }break;
        case 10://10	我方全体步兵
        {
            this->getCombatSquadByCondition(*vect, obj, true, Combat_ArmsType_Melee);
        }break;
        case 11://11	敌方全体骑兵
        {
            this->getCombatSquadByCondition(*vect, obj, false, Combat_ArmsType_Cavalry);
        }break;
        case 12://12	我方全体骑兵
        {
            this->getCombatSquadByCondition(*vect, obj, true, Combat_ArmsType_Cavalry);
        }break;
        case 13://13    敌方全体策士
        {
            this->getCombatSquadByCondition(*vect, obj, false, Combat_ArmsType_None, true);
        }break;
        case 14://14	我方全体策士
        {
            this->getCombatSquadByCondition(*vect, obj, true, Combat_ArmsType_None, true);
        }break;
        case 15://15	敌方全体攻城
        {
            this->getCombatSquadByCondition(*vect, obj, false, Combat_ArmsType_Machine);
        }break;
        case 16://16	我方全体攻城
        {
            this->getCombatSquadByCondition(*vect, obj, true, Combat_ArmsType_Machine);
        }break;
        case 17://17	敌方战力最强军团
        {
            this->getCombatSquadByValue(*vect, obj, false, 0, 0);
        }break;
        case 18://18	我方战力最强军团
        {
            this->getCombatSquadByValue(*vect, obj, true, 0, 0);
        }break;
        case 19://19	敌方武勇最强将领
        {
            this->getCombatSquadByValue(*vect, obj, false, 1, 12);
        }break;
        case 20://20	我方武勇最强将领
        {
            this->getCombatSquadByValue(*vect, obj, true, 1, 12);
        }break;
        case 21://21	敌方智力最强将领
        {
            this->getCombatSquadByValue(*vect, obj, false, 1, 13);
        }break;
        case 22://22	我方智力最强将领
        {
            this->getCombatSquadByValue(*vect, obj, true, 1, 13);
        }break;
        case 23://23	敌方统率最强将领
        {
            this->getCombatSquadByValue(*vect, obj, false, 1, 14);
        }break;
        case 24://24	我方统率最强将领
        {
            this->getCombatSquadByValue(*vect, obj, true, 1, 14);
        }break;
        case 25://25	敌方生命比率最高军团
        {
            this->getCombatSquadByValue(*vect, obj, false, 1, 0);
        }break;
        case 26://26	我方生命比率最高军团
        {
            this->getCombatSquadByValue(*vect, obj, true, 1, 0);
        }break;
        case 27://27	敌方生命比率最低军团
        {
            this->getCombatSquadByValue(*vect, obj, false, 1, -1);
        }break;
        case 28://28	我方生命比率最低军团
        {
            this->getCombatSquadByValue(*vect, obj, true, 1, -1);
        }break;
        case 29://29	敌方生命最高军团
        {
            this->getCombatSquadByValue(*vect, obj, false, 2, 0);
        }break;
        case 30://30	我方生命最高军团
        {
            this->getCombatSquadByValue(*vect, obj, true, 2, 0);
        }break;
        case 31://31	敌方生命最低军团
        {
            this->getCombatSquadByValue(*vect, obj, false, 2, -1);
        }break;
        case 32://32	我方生命最低军团
        {
            this->getCombatSquadByValue(*vect, obj, true, 2, -1);
        }break;
        case 33://33	施法者范围内的敌方军团
        {
            int area = effectData->getSkillInfo()->getTargetRange();
            this->getGameObjListInArea(*vect, area, obj, obj->getSquadDirection());
        }break;
        case 35://35	目标方范围内的敌方军团
        {
            int area = effectData->getSkillInfo()->getTargetRange();
            this->getGameObjListInArea(*vect, area, _target, obj->getSquadDirection());
        }break;
        case 34://34	施法者范围内的我方军团
        {
            int area = effectData->getSkillInfo()->getTargetRange();
            LQCombatSquad* squad = NULL;
            if (_target){
                if (_target->getType() == klqLQGameObjSquad) {
                    squad = (LQCombatSquad*)_target;
                }else{
                    squad = ((LQCombatSoldier*)_target)->getSquad();
                }
                this->getGameObjListInArea(*vect, area, obj, squad->getSquadDirection());
                vect->push_back(obj->getSquad());
            }
        }break;
        case 36://36	目标方范围内的我方军团
        {
            int area = effectData->getSkillInfo()->getTargetRange();
            LQCombatSquad* squad = NULL;
            if (_target){
                if (_target->getType() == klqLQGameObjSquad) {
                    squad = (LQCombatSquad*)_target;
                }else{
                    squad = ((LQCombatSoldier*)_target)->getSquad();
                }
                this->getGameObjListInArea(*vect, area, _target, squad->getSquadDirection());
            }
        }break;
        case 40:
        case 41:
        case 42:
        case 43:
        case 46:
        case 47:
        case 48:
        case 49://40-43	战场范围内的随机敌方军团 46-49	战场范围内的随机我方军团
        {
            bool isour = effectData->getObjectType()>45; //我方
            int count = effectData->getObjectType() - (isour?46:40) + 1;
            GameObjectVector* temp = new GameObjectVector();
            this->getCombatSquadByCondition(*vect, obj, isour, Combat_ArmsType_None);
            //随机
            this->getGameObjListInRand(*vect, temp, count);
            CC_SAFE_DELETE(temp);
        }break;
        case 50:
        case 51:
        case 52:
        case 53:
        case 56:
        case 57:
        case 58:
        case 59://50-55	目标方范围内随机敌方军团 56-59	目标方范围的随机我方军团
        {
            bool isour = effectData->getObjectType()>55; //我方
            int count = effectData->getObjectType() - (isour?56:50) + 1;
            int area = effectData->getSkillInfo()->getTargetRange();
            LQCombatSquad* squad = NULL;
            if (_target){
                if (_target->getType() == klqLQGameObjSquad) {
                    squad = (LQCombatSquad*)_target;
                }else{
                    squad = ((LQCombatSoldier*)_target)->getSquad();
                }
                GameObjectVector* tempObjs = new GameObjectVector();
                this->getGameObjListInArea(*tempObjs, area, _target, isour?squad->getSquadDirection():obj->getSquadDirection());
                if(!isour) tempObjs->push_back(squad);
                //随机
                this->getGameObjListInRand(*vect, tempObjs, count);
                CC_SAFE_DELETE(tempObjs);
            }
        }break;
        case 61://61	离我最近的敌方军团
        {
            int area = effectData->getSkillInfo()->getTargetRange();
            this->getGameObjListMinMaxInArea(*vect, area, obj, false, 1);
        }break;
        case 62://62	离我最近的我方军团
        {
            int area = effectData->getSkillInfo()->getTargetRange();
            this->getGameObjListMinMaxInArea(*vect, area, obj, true, 1);
        }break;
        case 63://63	离我最远的敌方军团
        {
            int area = effectData->getSkillInfo()->getTargetRange();
            this->getGameObjListMinMaxInArea(*vect, area, obj, false, 2);
        }break;
        case 64://64	离我最远的我方军团
        {
            int area = effectData->getSkillInfo()->getTargetRange();
            this->getGameObjListMinMaxInArea(*vect, area, obj, true, 2);
        }break;
        case 65://65.我方全体 除去自身
        {
            this->getCombatSquadByCondition(*vect, obj, true, Combat_ArmsType_None);
            for(GameObjectVector::iterator it=vect->begin();it!=vect->end();)
            {
                if(*it == obj->getSquad())
                    it=vect->erase(it);    //删除元素，返回值指向已删除元素的下一个位置
                else ++it;    //指向下一个位置
            }
        }break;
        default:
            break;
    }
    return vect;
}

//随机目标列表
void LQBattleSession::getGameObjListInRand(GameObjectVector& vect, GameObjectVector* temps, int count)
{
    int size1 = (int)temps->size();
    int *randval = new int[size1];
    for (int i = 0; i<size1; i++)
        randval[i] = size1*30;
    
    srand((unsigned)time(NULL));
    for (int i = 0; i<count; i++) {
        int k = randomWeightInt(randval, size1, 0);
        //CCLOG("randomInt:%d", k);
        vect.push_back(temps->at(k));
    }
    CC_SAFE_FREE(randval);
}

//根据条件查找目标列表
void LQBattleSession::getCombatSquadByCondition(GameObjectVector& vect, LQCombatHero* obj, bool myself, LQArmsTypeEnum arms, bool isTactician)
{
    for (int i = 0; i < m_squadList.size(); i++) {
        LQCombatSquad* squad = m_squadList.at(i);
        if(arms!=Combat_ArmsType_None && arms!=squad->getArmsType()) //某种兵种
            continue;
        if(isTactician && squad->getOccupation()!=Combat_Occupation_Tactician)  //要求是策士
            continue;
        if(!squad->getActive())  //已经死了
            continue;
        bool isself = squad->getSquadDirection() == obj->getSquadDirection();
        if( (myself && isself) || (!myself && !isself) ){
            vect.push_back(squad);
        }
    }
}

//根据条件获取对象数量 valType 1 死亡 2 存活
int LQBattleSession::getCombatSquadCount(bool myself, int valType)
{
    LQCombatSquad* _squad = NULL;
    int _value = 0;
    for (int i = 0; i < m_squadList.size(); i++) {
        _squad = m_squadList.at(i);
        if(_squad->getActive())  //活着
            _value += valType==2 ? 1 : 0;
        else
            _value += valType==1 ? 1 : 0;
    }
    return _value;
}

int* LQBattleSession::getCombatSquadCount(LQCombatHero* obj)
{
    LQCombatSquad* squad = NULL;
    int* _value = new int[4];
    _value[0] = 0; _value[1] = 0; _value[2] = 0; _value[3] = 0;
    for (int i = 0; i < m_squadList.size(); i++) {
        squad = m_squadList.at(i);
        bool isself = squad->getSquadDirection() == obj->getSquadDirection();
        if(squad->getActive()){  //活着
            if (isself) _value[3]++;  else _value[2]++;
        }else{
            if (isself) _value[1]++;  else _value[0]++;
        }
    }
    return _value;
}

//获取对象列表的根据数值条件
// hprate 1 生命比率 2 生命 3 战力 4 死亡 5 存活
void LQBattleSession::getCombatSquadByValue(GameObjectVector& vect, LQCombatHero* obj, bool myself, int hprate, int valType)
{
    LQCombatSquad* _squad = NULL;
    int _value = 0;
    for (int i = 0; i < m_squadList.size(); i++) {
        LQCombatSquad* squad = m_squadList.at(i);
        if(!squad->getActive())  //已经死了
            continue;
        bool isself = squad->getSquadDirection() == obj->getSquadDirection();
        if( (myself && isself) || (!myself && !isself) ){
            //if (isself)
            {
                int v1 = 0;
                if(hprate==1){
                    //现在改成生命比率了
                    v1 = squad->getSquadData()->getCurrHpRate()*100;
                    if (v1<=0)
                        continue;
                }
                else if( hprate==2){
                    if (valType<=0) //生命
                        v1 = squad->getSquadData()->getCurrHpValue();
                    else
                        v1 = squad->getSquadData()->getCurrStdAtr(valType);
                }
                else v1 = squad->getSquadData()->getPower();  //战力
                bool isHigh = v1>_value;
                if (valType<0) //最小生命
                    isHigh = !isHigh;
                if(isHigh || _squad==NULL){
                    _value = v1;
                    _squad = squad;
                }
            }
        }
    }
    if(_squad)
        vect.push_back(_squad);
}

// 获取目标对象范围内的最近或者最远的对象 obj当前对象 vtype 1 最近 2 最远
void LQBattleSession::getGameObjListMinMaxInArea(GameObjectVector& vect, int area, GameObj* obj, bool myself, int vtype)
{
    LQCombatSquad* objsquad = NULL;
    if (obj){
        if (obj->getType() == klqLQGameObjSquad) {
            objsquad = (LQCombatSquad*)obj;
        }else{
            objsquad = ((LQCombatSoldier*)obj)->getSquad();
        }
    }
    
    if (!objsquad) return;
    LQCombatSquad* _squad = NULL;
    Point cpt = objsquad->getPosition();
    float _d = 0;  //记录临时距离
    for (int i = 0; i < m_squadList.size(); i++) {
        LQCombatSquad* squad = m_squadList.at(i);
        if(objsquad==squad)  //不溅射目标自己 4.10
            continue;
        if(!squad->getActive())  //已经死了
            continue;
        if (squad->getSquadDirection() != objsquad->getSquadDirection() && myself)  //同方
            continue;
        else if (squad->getSquadDirection() == objsquad->getSquadDirection() && !myself)  //敌方
            continue;
        Point tpt = squad->getPosition();
        float d = getDistance(cpt,tpt);
        if (area<=0 || d<=area*MAP_WIDTH){  //距离符合
            if (_d<=0) {
                _d = d; _squad = squad;
            }
            if ( vtype==1 && _d>d ){  //1 最近 2 最远
                _d = d; _squad = squad;
            }else if ( vtype==2 && _d<d  ){
                _d = d; _squad = squad;
            }
        }
    }
    
    if (_squad)
        vect.push_back(_squad);
}

// 获取目标对象范围内的多个对象 obj当前对象
void LQBattleSession::getGameObjListInArea(GameObjectVector& vect, int area, GameObj* obj, LQCombatSquadDirectionEnum squadDirection)
{
    LQCombatSquad* squad = NULL;
    if (obj){
        if (obj->getType() == klqLQGameObjSquad) {
            squad = (LQCombatSquad*)obj;
        }else{
            squad = ((LQCombatSoldier*)obj)->getSquad();
        }
    }
    //vect.push_back(squad);
    //LQCombatSquadDirectionEnum squadDirection = squad->getSquadDirection();
    GameObjectVector* splashObjects = LQBattleSession::getGameObjListInArea(area, squad, squadDirection);
    for (GameObj* targetobj : *splashObjects ) {
        vect.push_back(targetobj);
    }
    CC_SAFE_DELETE(splashObjects);
}

//area<0 无限范围
GameObjectVector* LQBattleSession::getGameObjListInArea(int area, LQCombatSquad* obj, LQCombatSquadDirectionEnum squadDirection)
{
    GameObjectVector* vect = new GameObjectVector();
    if (!obj)
        return vect;
    Point cpt = obj->getPosition();
    if(_DEBUG_TYPE == _DEBUG_TYPE_VALUE || _DEBUG_TYPE == _DEBUG_TYPE_SKILL )
        CCLOG("Area form [%s]:%s area=%0.3f", obj->getName().c_str(), PointString(cpt).c_str(), area*MAP_WIDTH);
    for (int i = 0; i < m_squadList.size(); i++) {
        LQCombatSquad* squad = m_squadList.at(i);
        if(obj==squad)  //不溅射目标自己 4.10
            continue;
        if(!squad->getActive())  //已经死了
            continue;
        if (squad->getSquadDirection() == squadDirection)  //同方
            continue;
        Point tpt = squad->getPosition();
        float d = getDistance(cpt,tpt);
        if (area<0 || d<=area*MAP_WIDTH)
            vect->push_back(squad);
        if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL)
            CCLOG("Area -- to [%s]:%s  d:%.3f is %d", squad->getName().c_str(), PointString(tpt).c_str(), d , d<=area*MAP_WIDTH);
    }
    
    return vect;
}

/*
//获取自动战斗时的攻击对象
LQCombatUnit* LQBattleSession::getAutoSkillTarget(LQCombatHero* obj)
{
    LQCombatUnit* target = NULL;
    
    int count_1 = 0;
    LQCombatSquad* moreSq_1 = NULL;
    
    int count_2 = 0;
    LQCombatSquad* moreSq_2 = NULL;
    for (int i = 0; i < m_squadList.size(); i++) {
        LQCombatSquad* squad = m_squadList.at(i);
        if (squad->getSquadDirection() == obj->getSquadDirection()) {
            if (count_1 < squad->getCombatUnits()->size()) {
                count_1 = squad->getCombatUnits()->size();
                moreSq_1 = squad;
            }
            if (count_1 == 0) {
                moreSq_1 = squad;
            }
        }else{
            if (count_2 < squad->getCombatUnits()->size()) {
                count_2 = squad->getCombatUnits()->size();
                moreSq_2 = squad;
            }
            if (count_2 == 0) {
                moreSq_2 = squad;
            }
        }
    }
    LQHeroUserData* heroData = (LQHeroUserData*)obj->getData();
    
    if (heroData->getActiveSkillID() <= 0) {
        return target;
    }
    
   
    LQSkillUserData* skillData = new LQSkillUserData(heroData->getActiveSkillID());
    
    switch (skillData->getAOEType()) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 9:
        case 10:
            if (moreSq_1) {
                target = moreSq_1->getBannerman();
            }
            break;
        case 2:
        case 4:
        case 6:
        case 8:
        case 11:
            if (moreSq_2) {
                target = moreSq_2->getBannerman();
            }
            break;
            
        default:
            break;
    }
    return target;
}*/

// 添加一个管控对象
void LQBattleSession::addGameObj(GameObj *obj)
{
	if(!obj->getParent())
        m_layer->addChild(obj,max_heigth-obj->getPositionY());
    obj->setIndex(m_objectindex++);
	m_activeObjs.push_back(obj);
}

// 获取obj最近的匹配军团对象
LQCombatSquad* LQBattleSession::getClosestCombatSquad(LQCombatSquad* obj, LQArmsTypeEnum armsType)
{
    //go through the list, find the closest object of param "type"
    //to the param "obj"
    float closeDist = 100000000.0f;
    LQCombatSquad* closeObj = NULL;
    
    Point objp1 = obj->getTileCoord();
    LQCombatSquadVector::iterator list1;
    for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
    {
        if(!(*list1)->isVisible())
            continue;
        //watch out for yourself
        if(*list1 == obj || !(*list1)->getActive() || (*list1)->getSquadDirection()==obj->getSquadDirection())
            continue;
        
        if ( armsType!=Combat_ArmsType_None && (*list1)->getArmsType()!=armsType )  //当需要过滤索敌喜好时
            continue;
        
        //通过convertTile计算格子距离
        Point p1 = (*list1)->getTileCoord();
        float dist = p1.getDistance(objp1);
		//float dist = (*list1)->getPosition().getDistance(obj->getPosition());
		if((*list1)->getType() == klqLQGameObjSquad && dist< closeDist)
        {
            closeDist = dist;
            closeObj = *list1;
            
            //if(obj->getIndex()==2){
            //    CCLOG("SINDX%d: %d %f", obj->getIndex(), (*list1)->getIndex(), dist);
            //}
        }
    }
    //if(obj->getIndex()==2){
    //    CCLOG("SINDX%d END: %d %f", obj->getIndex(), closeObj->getIndex(), closeDist);
    //}

    return closeObj;
    
}

//根据index匹配军团,先匹配相同序号
LQCombatSquad* LQBattleSession::getMatchingCombatSquad(LQCombatSquad* squad)
{
    //CCLOG("getMatchingCombatSquad %d, %d", squad->getSquadDirection(), squad->getSquadY());
    int sqType = squad->getSquadDirection()==klqCombatSquadDirectionLeft ? 1 : 0;
    string str;
    int y_ = LQCombatSquad::SQUAD_MAXROW - squad->getSquadY() + 1;  //镜像y
    stringf(str, "%d_%d_%d", sqType, squad->getSquadX(), y_ );
    
    //先找同样位置的
    if ( m_squadMap.find(str)!= m_squadMap.end()){
        return m_squadMap[str];
    }
    int squadidxs[5][5] = {
        {1,2,3,4,5},
        {4,2,1,3,5},
        {5,4,3,2,1}
    };
    
    for(int i=MAX_SQUAD_COLS; i>0; i--)
        for(int j=1; j<=MAX_SQUAD_COLS; j++)
        {
            int k = squadidxs[squad->getSquadY()-1][j-1];
            stringf(str, "%d_%d_%d", sqType, i, k);
            if ( m_squadMap.find(str)!= m_squadMap.end()){
                return m_squadMap[str];
            }
        }
    
    return NULL;
}

void LQBattleSession::addGameSquad(LQCombatSquad *obj)
{
    //CCLOG("addGameSquad:%p", obj);
	m_squadList.push_back(obj);
    m_squadMap[obj->getKey()] = obj;
}


//---------------------------------------------------------
void LQBattleSession::initGame(Node* layer)
{
    m_layer = layer;    
}

//处理界面传来的数据
//等级 resID 位置 兵种 兵数 (兵力 武力 智力 统帅)
void LQBattleSession::initBattleData(string valuestr)
{
    m_battleValueStr = valuestr;
    m_objectindex = 0;
    m_status = klqBattleStatusBegin;
    m_gameSpeed = 0;
    m_battleTimes= 0;
    m_isAutoFight = false;
    m_victoryType = 0; //结束条件
    m_isOldWin = false;
    m_isReplay = false;
    
    //军团数据列表
    m_heroDatas.clear();
    m_leftheroDatas.clear();
    m_rightheroDatas.clear();
    
    m_objectindex = 0;
    m_gameSpeed = 0;
	m_activeObjs.clear();
    m_status = klqBattleStatusBegin;
    m_tileCoordCounts.clear();
    m_tileCoordBooks.clear();
    m_squadMap.clear();
    m_squadList.clear();
    
    //记录数据
    m_battleTimes= 0;           //战斗时长
    m_leftSquadCount = 0;       //本方军团数
    m_leftSquadCountMax = -1;
    m_rightSquadCount = 0;      //敌方军团数
    m_leftSquadHP = 0;          //本方军团HP
    m_rightSquadHP = 0;         //敌方军团HP
    m_leftSquadHPMax = 0;
    m_rightSquadHPMax = 0;
    m_rightSquadHPMaxInit = 0;
    m_leftSquadHPMaxInit = 0;
    m_leftSquadLevel = 0;       //我方军团级别
    m_rightSquadLevel = 0;      //对方军团级别
    m_leftSquadFightPower = 0;  //我方军团战力
    m_rightSquadFightPower = 0; //对方军团战力
    m_CondDesc = "";
    
    m_rage_attack = getFormulaParamFactor(18,"m");
    m_rage_defend = getFormulaParamFactor(18,"n");
    m_rage_atkmax = getFormulaParamFactor(18,"j");
    m_rage_defmax = getFormulaParamFactor(18,"k");
    
    m_zoneID = 0;
    m_battlaMap = "Vs_city.jpg";	//战斗场景的背景地图 Vs_city.jpg
    m_smokeEffect = 40028001;	//烟尘特效ID
    m_wav1 = "";	//场景音乐
    m_wav2 = "";	//环境音效
    m_battleMonster = -1;	//城池中的怪物组ID
    m_moneyAward = 0;	//通关后固定的金钱奖励
    m_soulAward = 0;	//通关后固定的将魂奖励
    m_fightType = 1;    //1: 普通关卡 2: 列传 3: 排行榜挑战 4: 经验关卡 5 良驹争夺 6 奇遇战斗 7:掠夺拦截 8:南蛮入侵
    m_AutoFightGuide = false;
    
    m_isRaids = false;
    //m_isPvp = false;
    //加载资源目录清理
    m_heroResourses.clear();
    CCLOG("initBattleData:\n%s\n", valuestr.c_str());
    LQHeroUserData* hero;
    std::vector<std::string> battleNs = split(valuestr.c_str(), '#');
    int k=0;

    while (k<battleNs.size())
    {
        string battleS = battleNs.at(k);
        std::vector<std::string> paramIds = split(battleS.c_str(), ';');
        if(k==0){
            m_zoneID = atoi(paramIds[0].c_str());
            //关卡id后面的类型 1: 普通关卡 2: 列传 3: 排行榜挑战 4: 经验关卡
            
            m_fightType = atoi(paramIds[1].c_str());
            if (m_fightType == 99) {
                m_AutoFightGuide = true;
                m_fightType = 1;
            }
            if ( m_fightType == 3 || m_fightType == 0) {
                m_zoneID = 22010102;
                if (m_fightType == 0) m_zoneID = 0;
                m_battlaMap =  "Vs_city.jpg";
                m_smokeEffect = 40028001;
                m_fightRoleID = paramIds[2];
            }
            if (m_zoneID>0){
                int pkey = (m_zoneID/1000)%10+1;
                m_zoneData = (LQZoneInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName("ZoneInfo",pkey,m_zoneID);
            }
            if (paramIds.size()>4){
                m_battlaMap = paramIds[3].c_str(); //战斗场景的背景地图
                m_battleMonster = atoi(paramIds[4].c_str()); //城池中的怪物组ID
                m_smokeEffect = atoi(paramIds[5].c_str());   //烟尘特效ID
                m_moneyAward = atoi(paramIds[6].c_str());	//通关后固定的金钱奖励
                m_soulAward = atoi(paramIds[7].c_str()); 	//通关后固定的将魂奖励
                m_isCanAutoFight = atoi(paramIds[8].c_str())>0;	//通关自动战斗
                if (paramIds.size()>9){
                    string victoryConds = paramIds[9].c_str(); //"1,2,0" "3,2000" "4,2000" "5,50" "10,2,2000"
                    m_victoryCond = split(victoryConds.c_str(), ',');
                    //1.特定NPC被击溃\r\n2.任意我军部队被击溃\r\n3.我军总生命降低到Y%或更少 万分\r\n4.任意我军部队生命降低到Y%或更少\r\n5.开战X秒后未能达成胜利条件（相当于调整单场战斗的时间上限）\r\n\r\n6.在1分30秒后，只要我军仍有部队存活，则战斗胜利。（而非通常的失败）\r\n\r\n10.将敌方特定NPC的血量降低到X%或更少
                    //"1,2,0" [类型,NPC索引,哪方] "3,2000" [类型,Y万分] "4,2000" [类型,Y万分] "5,50" [类型,秒] "10,2,2000" [类型,NPC索引,X万分]
                    if (m_victoryCond.size()>0)
                        m_victoryType = atoi(m_victoryCond[0].c_str());
                    if (paramIds.size()>10){
                        LQTipMap *tipMap = LQDataCache::sharedLQDataCache()->getTipDatas();
                        LQTipMap::iterator it = tipMap->find(atoi(paramIds[10].c_str()));
                        if(it != tipMap->end()) {
                            m_CondDesc = tipMap->at(atoi(paramIds[10].c_str()));
                        }else{
                            m_CondDesc = "";
                        }
                    }
                }
                
                m_wav1 = "";	//场景音乐
                m_wav2 = "";	//环境音效
            }
            if (m_fightType == 0)
                m_battlaMap = "Vs_city.jpg";            k++;
            continue;
        }else if(k==1){
            for (int j=0; j<paramIds.size(); j++) {
                string unitVal = paramIds.at(j);
                std::vector<std::string> sqVals = split(unitVal.c_str(), ',');
                if(j==0){ //我方
                    m_leftSquadLevel = atoi(sqVals[0].c_str());       //军团级别
                    m_leftSquadFightPower = atoi(sqVals[1].c_str());  //军团战力
                }else if(j==1){ //对方
                    m_rightSquadLevel = atoi(sqVals[0].c_str());      //军团级别
                    m_rightSquadFightPower = atoi(sqVals[1].c_str()); //军团战力
                }
            }
            k++;
            continue;
        }else if(k==4){  //附加属性
            CCLOG("附加属性....... %s", battleS.c_str());
            LQHeroUserData* mhero = NULL;
            for (int j=0; j<paramIds.size(); j++) {
                string unitVal = paramIds.at(j);
                std::vector<std::string> unitVals = split(unitVal.c_str(), ',');
                if(unitVals.size()<3)
                {
                    CCLOG("Battle 附加 Data [%s] is Error;", unitVal.c_str());
                    continue;
                }
                int posTp = atoi(unitVals[0].c_str());   //敌我 0 我 1 敌
                int posId = atoi(unitVals[1].c_str());   //位置
                LQHeroUserDataVector::iterator _hero;
                for(_hero=m_heroDatas.begin();_hero!=m_heroDatas.end();_hero++)
                {
                    if((*_hero)->getSquadDirection()==posTp && (*_hero)->getFormationIndex()==posId){
                        mhero = *_hero;
                        break;
                    }  
                }
                if(!mhero){
                    CCLOG("ERROR [%d] %d 没有数据;", posTp, posId);
                    continue;
                }
                string propValuestr = unitVals[2].c_str();
                CCLOG("propValuestr 2:%s",propValuestr.c_str());
                mhero->splitValue(propValuestr, '$' , ':');
                mhero->calcData();
                
                if(mhero->getCurrStdAtr(ATTR_SQUEHP)<=0){
                    for(_hero=m_heroDatas.begin();_hero!=m_heroDatas.end();_hero++)
                        if((*_hero)==mhero){
                            m_heroDatas.erase(_hero);
                            break;
                        }
                    if(posTp==0) {
                        for(_hero=m_leftheroDatas.begin();_hero!=m_leftheroDatas.end();_hero++)
                            if((*_hero)==mhero){
                                m_leftheroDatas.erase(_hero);
                                break;
                            }
                    }else{
                        for(_hero=m_rightheroDatas.begin();_hero!=m_rightheroDatas.end();_hero++)
                            if((*_hero)==mhero){
                                m_rightheroDatas.erase(_hero);
                                break;
                            }
                    }
                    continue;
                }

            }
            k++;
            continue;
        }else if(k==5){ //回放数据
            m_isReplay = true;
            CCLOG("回放数据....... %s", battleS.c_str());
            for (int j=0; j<paramIds.size(); j++) {
                string unitVal = paramIds.at(j);
                std::vector<std::string> unitVals = split(unitVal.c_str(), ',');
                if (j==0){  //第一段是公共信息
                    m_isOldWin = atoi(unitVals[0].c_str())>0;  //输赢
                }
            }
            k++;
            continue;
        }
        //战斗参与的武将数据 2 本方 3 敌方
        bool isself = (k==2);
        if(!isself)
            CCLOG("敌方武将数据.......");
        else
            CCLOG("本方武将数据.......");
        for (int j=0; j<paramIds.size(); j++) {
            string unitVal = paramIds.at(j);
            std::vector<std::string> unitVals = split(unitVal.c_str(), ',');
            if(unitVals.size()<=5)
            {
                CCLOG("Battle Data [%s] is Error;", unitVal.c_str());
                continue;
            }
            //军团的基本数据
            int resLv = atoi(unitVals[0].c_str());   //英雄等级
            int pkey =  atoi(unitVals[1].c_str());   //分割文件，决定怪物资源数据位置
            int resId = atoi(unitVals[2].c_str());   //英雄资源ID
            bool ishero = atoi(unitVals[3].c_str())>0;   //英雄资源ID
            int posId = atoi(unitVals[4].c_str());   //位置
            int soldId = atoi(unitVals[5].c_str());  //兵ID
            int soldCount = atoi(unitVals[6].c_str());//兵数量
            int power = atoi(unitVals[7].c_str());//队伍战力
            int star  = atoi(unitVals[8].c_str());//将星级数量
            int quality  = atoi(unitVals[9].c_str());//将品质
            if(soldCount<0) soldCount = 15;
            
            // 排行榜和良驹先从玩家表里取值 回头加个判断条件来区分是人是怪
            if(ishero){  //isself || m_fightType == 3 || m_fightType == 5
                hero = new LQHeroUserData(resId,soldId,soldCount);
                if (quality>0)
                    hero->setQuality(quality);
            }else //对手是怪物
                hero = new LQMonsterUserData(pkey,resId,soldId,soldCount);
            hero->setSquadDirection(isself?klqCombatSquadDirectionLeft:klqCombatSquadDirectionRight);
            if (power>0) hero->setPower(power);
            if (star>0) hero->setStar(star);
            
            //PVE没敌人数据，PVP有
            if(unitVals.size()>10){
                string propValuestr = unitVals[10].c_str();
                CCLOG("propValuestr 1:%s",propValuestr.c_str());
                hero->splitValue(propValuestr, '$' , ':');
            }
            
            //hero->setCurrStdAtr(103, 50000);
            
            hero->setFormationIndex(posId);
            hero->setLevel(resLv);
            hero->calcData();
            
            //CCLOG("ATTR_SQUEHP:%.3f",hero->getCurrStdAtr(ATTR_SQUEHP));
            if(hero->getCurrStdAtr(ATTR_SQUEHP)<=0){
                if(isself) m_leftSquadHPMaxInit += hero->getSoldHpValue();
                else m_rightSquadHPMaxInit += hero->getSoldHpValue();
                continue;
            }
            
            //数据
            m_heroDatas.push_back(hero);
            if (isself) m_leftheroDatas.push_back(hero);
            else m_rightheroDatas.push_back(hero);
            
            string resfile;
            LQ::stringf(resfile, "res/hero/avt_%s_%s.plist", "A", hero->getBaseName().c_str());
            LQ::stringf(resfile, "res/hero/avt_%s_%s.plist", "B", hero->getBaseName().c_str());
            m_heroResourses.push_back(resfile.c_str());
        }
        k++;
    }
    if (m_zoneData && m_zoneData->getActStoryID().length()>0){
        //
        CCLOG("剧情演员 %s", m_zoneData->getActStoryID().c_str());
        std::vector<std::string> attrvals = split(m_zoneData->getActStoryID().c_str(), ';');
        int k=0;
        while (k<attrvals.size())
        {
            string attrstr = attrvals.at(k);
            k++;
            if(attrstr.length()<=0) continue;
            std::vector<std::string> attrvalue = split(attrstr.c_str(), ',');
            if(attrvalue.size()>=3){
                int trigHeroDr = atoi(attrvalue[0].c_str());
                int trigHeroPkey = atoi(attrvalue[1].c_str());
                int trigHeroId = atoi(attrvalue[2].c_str());
                
                // 排行榜和良驹先从玩家表里取值 回头加个判断条件来区分是人是怪
                if(trigHeroDr==1){
                    hero = new LQHeroUserData(trigHeroId,-1, 12);
                }else //对手是怪物
                    hero = new LQMonsterUserData(trigHeroPkey, trigHeroId, -1, 12);
                hero->setSquadDirection(trigHeroDr==1?klqCombatSquadDirectionLeft:klqCombatSquadDirectionRight);
                
                hero->setFormationIndex(15+k);  //位置
                hero->calcData();
                //数据
                m_heroDatas.push_back(hero);
                if (trigHeroDr==1) m_leftheroDatas.push_back(hero);
                else m_rightheroDatas.push_back(hero);
                
                string resfile;
                LQ::stringf(resfile, "res/hero/avt_%s_%s.plist", "A", hero->getBaseName().c_str());
                LQ::stringf(resfile, "res/hero/avt_%s_%s.plist", "B", hero->getBaseName().c_str());
                m_heroResourses.push_back(resfile.c_str());
            }
        }
    }
}

//开始冲锋
void LQBattleSession::startGame()
{
    //设置阵型初始数据
    LQCombatSquadVector::iterator list1;
    Point td;
    int d1 = 1;
    for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
    {
        td = (*list1)->getHero()->getTileCoord();
        bool isleft = (*list1)->getSquadDirection() == klqCombatSquadDirectionLeft;
        d1 = isleft ? 1 : -1;
        int _index = (*list1)->getIndex();
        bool is47 = _index==1 || _index==6 || _index==11;
        bool is69 = _index==5 || _index==10 || _index==15;
        //近战 Combat_RestrainType_Melee   平移一步向前
        //远程 Combat_RestrainType_Remote  正常直行
        //骑兵 Combat_RestrainType_Cavalry 斜向几步向前
        Point w1 = Point::ZERO;
        Point w2 = Point::ZERO;
        float r1 = 5.0; float r2 = 3.0;
        if (_index==11 || _index==15){
            r1 += 2;  r2 += 2;
        }
        //int y_ =  is79?0:2;  //第3排多一个
        
        if(_DEBUG_TYPE == _DEBUG_TYPE_VALUE || _DEBUG_TYPE == _DEBUG_TYPE_SKILL )
            CCLOG("军团[%d] HERO %d = [%d] %s", (*list1)->getIndex(), _index, (*list1)->getHero()->getIndex(), (*list1)->getHero()->getName().c_str());
            
        if((isleft && is47) || (!isleft && is69)){
            if((*list1)->getArmsType()==Combat_ArmsType_Cavalry)        //骑兵
            {
                w1 = Point(d1*r2*LQCombatSquad::SQUAD_MAXCOL, 0);
                w2 = Point(d1*r1*LQCombatSquad::SQUAD_MAXCOL, -LQCombatSquad::SQUAD_MAXCOL);
            }else if((*list1)->getArmsType()==Combat_ArmsType_Melee)    //步兵
            {
                w1 = Point(d1*r2*LQCombatSquad::SQUAD_MAXCOL, 0);
                //w2 = Point(d1*LQCombatSquad::SQUAD_MAXCOL, LQCombatSquad::SQUAD_MAXCOL);
            }
        }else if((isleft && is69) || (!isleft && is47)){
            if((*list1)->getArmsType()==Combat_ArmsType_Cavalry)        //骑兵
            {
                w1 = Point(d1*r2*LQCombatSquad::SQUAD_MAXCOL, 0);
                w2 = Point(d1*r1*LQCombatSquad::SQUAD_MAXCOL, LQCombatSquad::SQUAD_MAXCOL);
            }else if((*list1)->getArmsType()==Combat_ArmsType_Melee)    //步兵
            {
                w1 = Point(d1*r2*LQCombatSquad::SQUAD_MAXCOL, 0);
                //w2 = Point(d1*6, -LQCombatSquad::SQUAD_MAXCOL-1);
            }
        }
        if(w1!=Point::ZERO || w2!=Point::ZERO){
            if(w2!=Point::ZERO)
                (*list1)->getHero()->addWayPoint(td+Point((int)w2.x,(int)w2.y));
            if(w1!=Point::ZERO)
                (*list1)->getHero()->addWayPoint(td+Point((int)w1.x,(int)w1.y));
        }else{  //没有绝对路径，向前走
            w1 = Point(isleft?15:-15, 0);
            (*list1)->getHero()->addWayPoint(td+w1);
        }
        
        //if((*list1)->getHero()->getMustPoints().size()>=2)
        //CCLOG("startGame way:%s %d %s,%s", (*list1)->getName().c_str(), (*list1)->getHero()->getIndex(), TileCoordString((*list1)->getHero()->getMustPoints().at(0)).c_str(), TileCoordString((*list1)->getHero()->getMustPoints().at(1)).c_str() );
        //if((*list1)->getHero()->getMustPoints().size()>=1)
        //    CCLOG("startGame way:%s %d %s", (*list1)->getName().c_str(), (*list1)->getHero()->getIndex(), TileCoordString((*list1)->getHero()->getMustPoints().at(0)).c_str());
        
        //第一排打开视野，可以索敌
        //if((*list1)->getSquadX()==1)
        //    (*list1)->setIsOpenView(true);
        //进入阵型冲锋状态
        (*list1)->updateRunCommand();
    }
    //开始战斗冲锋了
    m_status = klqBattleStatusRun;
}

//
void LQBattleSession::setCtrlLayer(Node* layer)
{
    GameObjectVector::iterator obj1;
    for(obj1=m_activeObjs.begin();obj1!=m_activeObjs.end();++obj1)
    {
        LQCombatUnit* unit = (LQCombatUnit*)(*obj1);
        unit->setControlLayer(layer);
    }
}

//开始战斗
void LQBattleSession::startBattle()
{
    if (m_status != klqBattleStatusRun)
        return;
    m_gameover = 0;
    m_status = klqBattleStatusFight;
    
    //开始战斗，调用界面回调
    if (this->onStartBattleCallback)
        this->onStartBattleCallback(this);
    
    //打开所有军团视野
    //LQCombatSquadVector::iterator list1;
    //for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
    //    (*list1)->setIsOpenView(true);
}

//统计数据
void LQBattleSession::totalGame(bool init){
    
    m_leftSquadCount = 0;
    m_leftSquadHP = 0;
    m_rightSquadCount = 0;
    m_rightSquadHP = 0;
    if (init){
        m_leftSquadHPMax = m_leftSquadHPMaxInit;
        m_rightSquadHPMax = m_rightSquadHPMaxInit;
    }
    LQCombatSquadVector::iterator list1;
    for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
    {
        //计算总的血量
        if(!(*list1)->getActive() || !(*list1)->isVisible())
            continue;
        if((*list1)->getSquadDirection() == klqCombatSquadDirectionLeft){
            m_leftSquadCount++;
            m_leftSquadHP+= (*list1)->getSquadHpValue();
            if (init)
                m_leftSquadHPMax += (*list1)->getSquadHpValueMax();
        }else{
            m_rightSquadCount++;
            m_rightSquadHP+= (*list1)->getSquadHpValue();
            if (init)
                m_rightSquadHPMax += (*list1)->getSquadHpValueMax();
        }
    }
    if(m_leftSquadCountMax<0) m_leftSquadCountMax = m_leftSquadCount;
    m_leftSquadHP = m_leftSquadHP/100;
    m_rightSquadHP = m_rightSquadHP/100;
    if (init){
        m_leftSquadHPMax = m_leftSquadHPMax/100;
        m_rightSquadHPMax = m_rightSquadHPMax/100;
    }
    
    if(m_leftSquadHP<0 || m_rightSquadHP<0)
        CCLOG("totalGame: %d %d", m_leftSquadHP, m_rightSquadHP);
    //胜负条件，
    if( victoryConditon() ){
        endBattle();
    }
}

//胜负条件
//0.战斗时间内存活方为胜利方 (默认)
//1.特定NPC被击溃
//2.任意我军部队被击溃
//3.我军总生命降低到Y%或更少
//4.任意我军部队生命降低到Y%或更少
//5.开战X秒后未能达成胜利条件（相当于调整单场战斗的时间上限）
//6.在1分30秒后，只要我军仍有部队存活，则战斗胜利。（而非通常的失败）
//10.将敌方特定NPC的血量降低到X%或更少
bool LQBattleSession::victoryConditon()
{
    m_isWin = false;
    bool isEnd = false;
    bool isError = false;
    if(m_leftSquadCount<=0 || m_rightSquadCount<=0 || m_battleTimes>=DEF_BATTLE_TIMEMAX){
        isEnd = true;
        m_isWin = m_leftSquadCount>0 && m_battleTimes<DEF_BATTLE_TIMEMAX;
        if (m_victoryType==6 && m_battleTimes>=DEF_BATTLE_TIMEMAX){
            m_isWin = true;
        }
        return isEnd;
    }
    std::vector<std::string> paramVs;
    switch (m_victoryType) {
        case 1:{ //1.特定NPC被击溃
            if(m_victoryCond.size()<3){ isError = true; break; }
            int npcIdx = atoi(m_victoryCond[1].c_str());  //NPC 索引
            int npcDirc = atoi(m_victoryCond[2].c_str());  //NPC 阵营 klqCombatSquadDirectionLeft
            bool npcWin = false;
            if (m_victoryCond.size()>3) //NPC条件决定输还是赢 1是赢 0和没有是输
                npcWin = atoi(m_victoryCond[3].c_str())>0;
            LQCombatSquadVector::iterator list1;
            for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
            {
                if((*list1)->getActive())
                    continue;
                if (((*list1)->getIndex()==npcIdx || (*list1)->getHeroData()->getResIDC()==npcIdx)
                    && (*list1)->getSquadDirection()==npcDirc){
                    isEnd = true;
                    m_isWin = npcWin;
                    break;
                }
            }
        }
            break;
        case 2:{ //2.任意我军部队被击溃
            LQCombatSquadVector::iterator list1;
            for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
            {
                if((*list1)->getActive())
                    continue;
                if ((*list1)->getSquadDirection()==klqCombatSquadDirectionLeft){
                    isEnd = true;
                    m_isWin = false;
                    break;
                }
            }
        }
            break;
        case 3:{ //3.我军总生命降低到Y%或更少
            if(m_victoryCond.size()<2){ isError = true; break; }
            float YRate = atoi(m_victoryCond[1].c_str())/100.0f;
            float rate = (m_leftSquadHP*1.0f/m_leftSquadHPMax)*100;
            if (rate<=YRate){
                isEnd = true;
                m_isWin = false;
            }
        }
            break;
        case 4:{ //4.任意我军部队生命降低到Y%或更少
            if(m_victoryCond.size()<2){ isError = true; break; }
            float YRate = atoi(m_victoryCond[1].c_str())/100.0f;
            LQCombatSquadVector::iterator list1;
            for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
            {
                if((*list1)->getActive() && (*list1)->getSquadDirection()==klqCombatSquadDirectionLeft
                   && (*list1)->getHPRate()<YRate)
                {
                    isEnd = true;
                    m_isWin = false;
                    break;
                }
            }
        }
            break;
        case 5:{ //5.开战X秒后未能达成胜利条件（相当于调整单场战斗的时间上限）
            if(m_victoryCond.size()<2){ isError = true; break; }
            int Xtime= atoi(m_victoryCond[1].c_str());
            if (m_battleTimes>Xtime){
                isEnd = true;
                m_isWin = false;
            }
        }
            break;
        case 6:{ //6.在1分30秒后，只要我军仍有部队存活，则战斗胜利。（而非通常的失败）
            if(m_victoryCond.size()<2){ isError = true; break; }
            int Xtime= atoi(m_victoryCond[1].c_str());  //DEF_BATTLE_TIMEMAX
            if (m_battleTimes>=Xtime && m_leftSquadCount>0){
                isEnd = true;
                m_isWin = true;
            }
        }
            break;
        case 10:{//10.将敌方特定NPC的血量降低到X%或更少）
            if(m_victoryCond.size()<2){ isError = true; break; }
            int npcIdx= atoi(m_victoryCond[1].c_str());  //NPC 索引
            float XRate = atoi(m_victoryCond[2].c_str())/100.0f;
            LQCombatSquadVector::iterator list1;
            for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
            {
                if((*list1)->getActive())
                    continue;
                if (((*list1)->getIndex()==npcIdx || (*list1)->getHeroData()->getResIDC()==npcIdx) && (*list1)->getSquadDirection()==klqCombatSquadDirectionRight
                    && (*list1)->getHPRate()<XRate){
                    isEnd = true;
                    m_isWin = true;
                    break;
                }
            }
        }
            break;
        default:
            isEnd = m_leftSquadCount<=0 || m_rightSquadCount<=0;
            m_isWin = m_leftSquadCount>0;
            break;
    }
    if(isError){
        isEnd = m_leftSquadCount<=0 || m_rightSquadCount<=0;
        m_isWin = m_leftSquadCount>0;
    }
    return isEnd;
}

//战斗结束
void LQBattleSession::endBattle()
{
    if(!m_fightLayer)
        return;
    //return;
    LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
    controllayer->setIsGameOver(true);
    m_gameover += 1;
    if(m_gameover > 30/*160*/ && m_status != klqBattleStatusOver){
        this->gameOver();
    }
    
    if(m_status==klqBattleStatusEnd || m_status==klqBattleStatusOver)
        return;
    m_status = klqBattleStatusEnd;
    LQCombatSquadVector::iterator list1;
    for(list1=m_squadList.begin();list1!=m_squadList.end();++list1)
    {
        if(!(*list1)->getActive())
            continue;
        (*list1)->updateWaitCommand();
    }
}

void LQBattleSession::gameOver()
{
    m_status = klqBattleStatusOver;
    
    CCLOG( "战斗结算" );
    LQHeroUserDataVector::iterator it;
    for(it=m_heroDatas.begin();it!=m_heroDatas.end();++it)
    {
        LQHeroUserData* hdata = (*it);
        
        CCLOG( "英雄[%d %s] %d", hdata->getResIDC(), hdata->getName().c_str(), hdata->getSquadDirection() );
        CCLOG( "    剩余血量 HP:%d", hdata->getSquadHpValue() );
        CCLOG( "    攻击的伤害 DpsTotal:%d", hdata->getDpsTotal() );
        CCLOG( "    杀死敌人数量 KillTotal:%d", hdata->getKillTotal() );
    }
    
    int aa = 0;
    GameObjectVector::iterator obj1;
    for(obj1=m_activeObjs.begin();obj1!=m_activeObjs.end();++obj1)
    {
        LQCombatUnit* unit = (LQCombatUnit*)(*obj1);
        if(unit){
            unit->getSprite()->stopAllActions();
            if(unit->getActive()){
                aa++;
                if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("LQCombatUnit %p: %s", unit, TileCoordString(unit->getTileCoord()).c_str());
            }
        }
    }
    
    int cc = 0;
    for (auto dataMapIter = m_tileCoordCounts.cbegin(); dataMapIter != m_tileCoordCounts.cend(); ++dataMapIter)
    {
        //CCLOG("tileCoordCounts %s = %d", dataMapIter->first.c_str(), dataMapIter->second.asInt());
        cc += dataMapIter->second.asInt();
        if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE && dataMapIter->second.asInt()!=0)
            CCLOG("tileCoordCounts %s = %d", dataMapIter->first.c_str(), dataMapIter->second.asInt());
    }
    
    if(cc!=aa)
        CCLOG("ToTal ERROR (Units:Tiles): %d != %d", aa, cc);
    else
        CCLOG("ToTal (Units:Tiles): %d == %d", aa, cc);
        
    bool isWin = m_isWin;

    if (m_isReplay)    //回放
    {
        if(m_isOldWin!=isWin){
            CCLOG("战斗结果不同于回放结果.....");
            isWin = m_isOldWin;//回放时战斗输赢
        }
    }
    
    //星级评定
    short recStar = 0;
    if(isWin) recStar+=1;                //战斗胜利
    if(m_battleTimes<60) recStar+=2;     //小于60
    if(m_leftSquadCountMax==m_leftSquadCount && m_leftSquadCount>0) recStar+=4; //未阵亡
    m_resStar = recStar;
    
    CCLOG("");
    CCLOG("=======================");
    CCLOG("战斗结果: 胜负:%d 评星:%d", isWin, m_resStar);
    CCLOG("=======================");
    CCLOG("");
    
    if (isWin) {
        switch (m_fightType) {
            case 1:{
                LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
                controllayer->showPolt();

            }break;
            default:{
                sendResultMsg(true,m_resStar);
            }
        }
    }
    else{
        sendResultMsg(false,m_resStar);
    }
}

//战斗胜利结束发消息处
void LQBattleSession::sendResultMsg(int isWin,int recStar)
{
    
#if defined(LQ_DEMO_FIGHT)
    isWin = false;
    m_fightType = 88;
#endif
    CCLOG("isWin  %d",isWin);
    if (isWin) {
        //1: 普通关卡 2: 列传 3: 竞技场挑战 4: 经验关卡 5 良驹争夺 6 奇遇战斗 7:掠夺拦截 8:南蛮入侵 9自动战斗（普通副本引导）10征战
        LQSocketBuffer* buffer = NULL;
        switch (m_fightType) {
            case 10:{ //征战
                LQHeroUserDataVector::iterator it;
                string selfResult;
                
                for(it=m_leftheroDatas.begin();it!=m_leftheroDatas.end();++it)
                {
                    LQHeroUserData* hdata = (*it);
                    
                    CCLOG( "我方英雄[%d %s] %d", hdata->getResIDC(), hdata->getName().c_str(), hdata->getSquadDirection() );
                    CCLOG( "    剩余血量 HP:%d", hdata->getSquadHpValue() );
                    CCLOG( "    剩余怒气 Furies:%d", hdata->getHeroFuries() );
                    
                    string str;
                    string s1;
                    string s2;
                    string s3;
                    stringf(s1, "%d,",hdata->getFormationIndex());
                    stringf(s2, "%d,",hdata->getSquadCurrHurtValue());
                    stringf(s3, "%d",hdata->getHeroFuries());
                    if (it == m_leftheroDatas.end() -1)
                        str = "";
                    else
                        str = ";";
                    selfResult = selfResult + s1 + s2 + s3 + str;
                    CCLOG("=============str%s",selfResult.c_str());
                }
                CCLOG("selfResult  = %s",selfResult.c_str());
                string vivalResult;
                for(it=m_rightheroDatas.begin();it!=m_rightheroDatas.end();++it)
                {
                    LQHeroUserData* hdata = (*it);
                    CCLOG( "我方英雄[%d %s] %d", hdata->getResIDC(), hdata->getName().c_str(), hdata->getSquadDirection() );
                    CCLOG( "    剩余血量 HP:%d", hdata->getSquadHpValue() );
                    CCLOG( "    剩余怒气 Furies:%d", hdata->getHeroFuries() );
                    string str;
                    string s1;
                    string s2;
                    string s3;
                    stringf(s1, "%d,",hdata->getFormationIndex());
                    stringf(s2, "%d,",hdata->getSquadCurrHurtValue());
                    stringf(s3, "%d",hdata->getHeroFuries());
                    if (it == m_leftheroDatas.end() -1)
                        str = "";
                    else
                        str = ";";
                    vivalResult = vivalResult + s1 + s2 + s3 + str;
                    CCLOG("=============str%s",vivalResult.c_str());
                }
                CCLOG("vivalResult  = %s",vivalResult.c_str());
                CampaignFightResultRequest* request10 = new CampaignFightResultRequest(	isWin,recStar,selfResult,vivalResult);
                buffer = request10->toMessage();
            }break;
            case 7:{ //拦截战斗
#if !defined(LQ_DEMO_FIGHT)
                std::string strCityResId = HclcData::getInstance()->getLuaVarString("ThiefCityResId");
                //CCLOG("strCityResId %s",strCityResId.c_str());
                int cityResId = atoi(strCityResId.c_str());
                std::string strRoleId =  HclcData::getInstance()->getLuaVarString("ThiefRoleId");
                //CCLOG("strRoleId %s",strRoleId.c_str());
                std::string strFightRoleId = HclcData::getInstance()->getLuaVarString("ThiefFightRoleId");
                //CCLOG("strFightRoleId %s",strFightRoleId.c_str());
                RationInterceptRequest* request7 = new RationInterceptRequest(cityResId,strRoleId,strFightRoleId,isWin);
                buffer = request7->toMessage();
#endif
            }break;
            case 8:{ //世界boss  南蛮入侵
#if !defined(LQ_DEMO_FIGHT)
                LQHeroUserDataVector::iterator it;
                string str = "";
                for(it=m_heroDatas.begin();it!=m_heroDatas.end();++it)
                {
                    LQHeroUserData* hdata = (*it);
                    CCLOG( "英雄[%d %s] %d", hdata->getResIDC(), hdata->getName().c_str(),  hdata->getSquadDirection() );
                    CCLOG( "    伤血量 HP:%d", hdata->getSquadHurtValue());
                    CCLOG( "    剩余血量 HP:%d", hdata->getSquadHpValue() );
                    CCLOG( "    攻击的伤害 DpsTotal:%d", hdata->getDpsTotal() );
                    CCLOG( "    杀死敌人数量 KillTotal:%d", hdata->getKillTotal());
                    if ( hdata->getSquadDirection() == 1)
                    {
                        string s;
                        string s1;
                        string s2;
                        stringf(s, "%d,",hdata->getResIDC());
                        stringf(s1, "%d,",hdata->getFormationIndex());
                        stringf(s2, "%d",hdata->getSquadCurrHurtValue());
                        str = str + s1 + s2 + ";";
                        CCLOG("=============str%s",str.c_str());
                    }
                }
                
                BarbarianFightResultRequest* request8 = new BarbarianFightResultRequest(str);
                buffer = request8->toMessage();
#endif
            }break;
            case 6:{ //奇遇战斗
#if !defined(LQ_DEMO_FIGHT)
                std::string EventId = HclcData::getInstance()->getLuaVarString("robMountID");
                int id = atoi(EventId.c_str());
                GetBubbleEventPVPAwardRequest* request6 = new GetBubbleEventPVPAwardRequest(id,isWin);
                buffer = request6->toMessage();
#endif
            }break;
            case 5:{ //5:良驹争夺
#if !defined(LQ_DEMO_FIGHT)
                std::string sid = HclcData::getInstance()->getLuaVarString("robFragmentsMountID");
                std::string MonId = HclcData::getInstance()->getLuaVarString("robMountID");
                int id = atoi(sid.c_str());
                HorseBattleFightResultRequest* request5 = new HorseBattleFightResultRequest(id,MonId,isWin);
                buffer = request5->toMessage();
#endif
            }break;
            case 4:{ //4: 经验关
                ExpLevelFightResultRequest* request4 = new ExpLevelFightResultRequest(m_zoneID,isWin);
                buffer = request4->toMessage();
            }break;
            case 3:{  //3: 排行榜挑战
                if (m_isReplay) //回放按钮
                {
                    auto oLayer = BattleReturnLayer::create();
                    LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
                    controllayer->getParent()->addChild(oLayer,15000);
                    return;
                }
                else
                {
                    std::string str1 = m_battleValueStr + "##1";
                    ChallengeRivalRequest* request1 = new ChallengeRivalRequest(m_fightRoleID,isWin,str1);
                    buffer = request1->toMessage();
                }
            }break;
            case 2:{ //2: 列传
                BiogFightResultRequest* request2 = new BiogFightResultRequest(m_zoneID,isWin);
                buffer = request2->toMessage();
            }break;
            default:{ //1: 普通关卡
                
                ZoneFightResultRequest* request2 = new ZoneFightResultRequest( m_zoneID, m_isRaids, isWin, recStar);
                buffer = request2->toMessage();
            }break;
        }
        if(buffer){
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
            LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
            if (controllayer) {
                controllayer->showNetPrompt(true);
            }
        }
    }else{
        LQSocketBuffer* buffer = NULL;
        if(m_fightType == 1) {
            ZoneFightResultRequest* request2 = new ZoneFightResultRequest( m_zoneID, m_isRaids, isWin, recStar);
            buffer = request2->toMessage();
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
            LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
            if (controllayer) {
                controllayer->showNetPrompt(true);
            }
        }
        else if (m_fightType == 2)
        {
            BiogFightResultRequest* request2 = new BiogFightResultRequest(m_zoneID,isWin);
            buffer = request2->toMessage();
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
        }
        else if (m_fightType == 3) {
            if (m_isReplay)  //回放按钮
            {
                auto oLayer = BattleReturnLayer::create();
                LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
                controllayer->getParent()->addChild(oLayer,15000);
                return;
            }
            else
            {
                std::string str1 = m_battleValueStr + "##0";
                LQSocketBuffer* buffer = NULL;
                ChallengeRivalRequest* request1 = new ChallengeRivalRequest(m_fightRoleID,isWin,str1);
                buffer = request1->toMessage();
                ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
                LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
                if (controllayer) {
                    controllayer->showNetPrompt(true);
                }
            }
        }
        else if (m_fightType == 4)
        {
            ExpLevelFightResultRequest* request4 = new ExpLevelFightResultRequest(m_zoneID,false);
            buffer = request4->toMessage();
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
        }
        else if(m_fightType == 5) {
#if !defined(LQ_DEMO_FIGHT)
            std::string sid = HclcData::getInstance()->getLuaVarString("robFragmentsMountID");
            std::string MonId = HclcData::getInstance()->getLuaVarString("robMountID");
            int id = atoi(sid.c_str());
            HorseBattleFightResultRequest* request5 = new HorseBattleFightResultRequest(id,MonId,isWin);
            buffer = request5->toMessage();
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
            LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
            if (controllayer) {
                controllayer->showNetPrompt(true);
            }

#endif
        }else if (m_fightType == 6)
        {
#if !defined(LQ_DEMO_FIGHT)
            std::string EventId = HclcData::getInstance()->getLuaVarString("robMountID");
            int id = atoi(EventId.c_str());
            GetBubbleEventPVPAwardRequest* request6 = new GetBubbleEventPVPAwardRequest(id,isWin);
            buffer = request6->toMessage();
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
            LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
            if (controllayer) {
                controllayer->showNetPrompt(true);
            }

#endif
        }
        else if (m_fightType == 7)
        {
#if !defined(LQ_DEMO_FIGHT)
            std::string strCityResId = HclcData::getInstance()->getLuaVarString("ThiefCityResId");
            //CCLOG("strCityResId %s",strCityResId.c_str());
            int cityResId = atoi(strCityResId.c_str());
            std::string strRoleId =  HclcData::getInstance()->getLuaVarString("ThiefRoleId");
            //CCLOG("strRoleId %s",strRoleId.c_str());
            std::string strFightRoleId = HclcData::getInstance()->getLuaVarString("ThiefFightRoleId");
            //CCLOG("strFightRoleId %s",strFightRoleId.c_str());
            RationInterceptRequest* request7 = new RationInterceptRequest(cityResId,strRoleId,strFightRoleId,isWin);
            buffer = request7->toMessage();
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
#endif
        }
        else if (m_fightType == 8)
        {
 //世界boss  南蛮入侵
#if !defined(LQ_DEMO_FIGHT)
            LQHeroUserDataVector::iterator it;
            string str = "";
            for(it=m_heroDatas.begin();it!=m_heroDatas.end();++it)
            {
                LQHeroUserData* hdata = (*it);
                CCLOG( "英雄[%d %s] %d", hdata->getResIDC(), hdata->getName().c_str(),  hdata->getSquadDirection() );
                CCLOG( "    伤血量 HP:%d", hdata->getSquadHurtValue());
                CCLOG( "    本场伤血量 HP:%d", hdata->getSquadCurrHurtValue());
                CCLOG( "    剩余血量 HP:%d", hdata->getSquadHpValue() );
                CCLOG( "    攻击的伤害 DpsTotal:%d", hdata->getDpsTotal() );
                CCLOG( "    杀死敌人数量 KillTotal:%d", hdata->getKillTotal());
                if ( hdata->getSquadDirection() == 1)
                {
                    string s;
                    string s1;
                    string s2;
                    stringf(s, "%d,",hdata->getResIDC());
                    stringf(s1, "%d,",hdata->getFormationIndex());
                    stringf(s2, "%d",hdata->getSquadCurrHurtValue());
                    str = str + s1 + s2 + ";";
                    CCLOG("=============str%s",str.c_str());
                }
            }
            BarbarianFightResultRequest* request8 = new BarbarianFightResultRequest(str);
            buffer = request8->toMessage();
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
            LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
            if (controllayer) {
                controllayer->showNetPrompt(true);
            }
#endif
        }
        else if(m_fightType == 10){
            LQHeroUserDataVector::iterator it;
            string selfResult;
            
            for(it=m_leftheroDatas.begin();it!=m_leftheroDatas.end();++it)
            {
                LQHeroUserData* hdata = (*it);
                
                CCLOG( "我方英雄[%d %s] %d", hdata->getResIDC(), hdata->getName().c_str(), hdata->getSquadDirection() );
                CCLOG( "    剩余血量 HP:%d", hdata->getSquadHpValue() );
                CCLOG( "    剩余怒气 Furies:%d", hdata->getHeroFuries() );
                
                string str;
                string s1;
                string s2;
                string s3;
                stringf(s1, "%d,",hdata->getFormationIndex());
                stringf(s2, "%d,",hdata->getSquadCurrHurtValue());
                stringf(s3, "%d",hdata->getHeroFuries());
                if (it == m_leftheroDatas.end() -1)
                    str = "";
                else
                    str = ";";
                selfResult = selfResult + s1 + s2 + s3 + str;
                CCLOG("=============str%s",selfResult.c_str());
            }
            CCLOG("selfResult  = %s",selfResult.c_str());
            string vivalResult;
            for(it=m_rightheroDatas.begin();it!=m_rightheroDatas.end();++it)
            {
                LQHeroUserData* hdata = (*it);
                CCLOG( "我方英雄[%d %s] %d", hdata->getResIDC(), hdata->getName().c_str(), hdata->getSquadDirection() );
                CCLOG( "    剩余血量 HP:%d", hdata->getSquadHpValue() );
                CCLOG( "    剩余怒气 Furies:%d", hdata->getHeroFuries() );
                
                string str;
                string s1;
                string s2;
                string s3;
                stringf(s1, "%d,",hdata->getFormationIndex());
                stringf(s2, "%d,",hdata->getSquadCurrHurtValue());
                stringf(s3, "%d",hdata->getHeroFuries());
                if (it == m_leftheroDatas.end() -1)
                    str = "";
                else
                    str = ";";
                vivalResult = vivalResult + s1 + s2 + s3 + str;
                CCLOG("=============str%s",vivalResult.c_str());
            }
            CCLOG("vivalResult  = %s",vivalResult.c_str());
            CampaignFightResultRequest* request10 = new CampaignFightResultRequest(	false,0,selfResult,vivalResult);
            buffer = request10->toMessage();
            ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
            
        }
        else {
            LQFightLayer* controllayer = (LQFightLayer*)m_fightLayer;
            if (controllayer) {
                controllayer->gameOver(isWin,m_resStar, "");
            }
        }
    }
}
//放弃游戏
void LQBattleSession::gameGiveup()
{
    m_status = klqBattleStatusOver;
    //bool isWin = false;
    //short recStar = 0;
    //ZoneFightResultRequest* request = new ZoneFightResultRequest( m_zoneID, m_isRaids, isWin, recStar);
    //LQSocketBuffer* buffer = request->toMessage();
    //ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
    
}
bool LQBattleSession::getIsReplay()
{
    return m_isReplay;
}
//
Point LQBattleSession::getAvailableTileCoord( LQCombatUnit* fromUnit,GameObj* targetUnit, uint overTile) {
    
    Point disPoint = targetUnit->getCurrentTileCoord() - fromUnit->getCurrentTileCoord();
    Point profileDir  = Point::ZERO;
    
    int index = calcDirectionFromPoint(disPoint);
    for (int i = 0; i < DIRECTION_SIZE; i++) {
        
        if (index >= DIRECTION_SIZE  )
            index = 0;
        
        //Point tempPoint = getTileCoordFromDirection(fromUnit->getCurrentTileCoord(),(LQUnitDirectionType)UnitDirection[index],overTile);
        Point tempPoint = fromUnit->getCurrentTileCoord() + getTileCoordOffsetFromDirection((LQUnitDirectionType)UnitDirection[index]);
        
        if (checkPassTileCoord(tempPoint)) {
            profileDir = tempPoint;
            break;
        }
        index ++ ;
    }
    
    return profileDir;
}

LQ_NAMESPACE_END
