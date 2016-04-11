//
// LQCombatSquad.cpp
// SuperWarriors
// 战斗队伍
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#include "LQCombatSquad.h"
#include "../LQBattleSession.h"
#include "LQSquadAIControl.h"
#include "LQSkillEffectListener.h"
#include "LQData.h"
#include "AudioManager.h"

LQ_NAMESPACE

const int LQCombatSquad::SQUAD_MAXCOL = 5;      //队列每行的最多人数 现在最多是3*6
const int LQCombatSquad::SQUAD_NUMCOL = 5;
const int LQCombatSquad::SQUAD_MAXROW = 3;      //队列最多行数 现在最多是3*6

const char *LQCombatSquad::EVENT_FORWARD_EFFECT = "event_forward_effect";

//LQCombatSquad* LQCombatSquad::create(string code,string scolor,int count,LQCombatSquadDirectionEnum direction)
LQCombatSquad* LQCombatSquad::create(LQHeroUserData* hero,LQCombatSquadDirectionEnum sqDirection)
{
    LQCombatSquad *pRet = new LQCombatSquad(hero,sqDirection);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

//code 兵种代码 scolor 阵营 count 兵数量
//LQCombatSquad::LQCombatSquad(string code,string scolor,int count,LQCombatSquadDirectionEnum direction):
LQCombatSquad::LQCombatSquad(LQHeroUserData* hero,LQCombatSquadDirectionEnum sqDirection = klqCombatSquadDirectionLeft):AIObj(NULL), m_combatUnits(NULL), m_parent(NULL), m_squadDirection(sqDirection), m_hero(NULL), m_herodata(NULL), m_squad_x(0), m_squad_y(0), m_planTarget(NULL), m_armflags(NULL), m_bannerman(NULL), m_occupation(Combat_Occupation_Normal),m_armsType(Combat_ArmsType_None),m_israllied(true),m_attackFrequency(0),m_chargeSpeed(1),m_isStand(true), m_isAttacking(false), m_firstLock(true),m_totalHurtHp(0),m_UnitHp(0),m_forwardEffect(Combat_ShowEffect_None),m_oldforwardEffect(Combat_ShowEffect_None),m_addSpeed(1), m_showEffectDuration(-1),m_squaVisible(true), m_isAttackMelee(false)
//,m_soldierHpValue(0),
{
    m_isOpenView = false;
    m_type = klqLQGameObjSquad;
    m_targetIndex = 0;
    m_target = NULL;
    m_state = FUSM_SQSTATE_WAIT;
    m_controller = new LQSquadAIControl(this);
    m_controller->init();
    
    m_herodata = hero;
    m_combatUnits = new LQCombatSoldierVector();
    m_squadData = new LQSquadUserData();
    m_squadData->initData(m_herodata);
    m_squadData->setParent(this);
    
    m_attackSquads = new vector<LQCombatSquad*>();
    m_attackSquadTime = 0;  //攻击锁定计时，2秒仍在攻击，被攻击者会切换目标过来，和m_attackSquads组合使用
    //m_attackSquadMelee = false;
    
    _atkCount = 0;
    m_moveeffect = 0;
    _hdbufferData = NULL;
    
    
    archerDM = getFormulaParamFactor(14,"ArcherDM");  //
    archerHurt = getFormulaParamFactor(14,"ArcherHurt");  //
    
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQCombatSquad::LQCombatSquad %p", this);
}

LQCombatSquad::~LQCombatSquad()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQCombatSquad::~LQCombatSquad %p", this);
    m_hero = NULL;
    m_combatUnits->clear();
    
    deleteInVector(m_nextActionList);
    
    //if(m_combatUnits) deleteInVector(*m_combatUnits);
    CC_SAFE_DELETE(m_combatUnits);
    CC_SAFE_DELETE(m_squadData);
    CC_SAFE_DELETE(m_attackSquads);
    CC_SAFE_DELETE(m_controller);
}


bool LQCombatSquad::init()
{
    if ( !AIObj::init() )
    {
        return false;
    }
    initData();
    return true;
}

#pragma mark - initData
void LQCombatSquad::initData()
{
    // add your codes here... resfile2,
    string resfile, reshead,resflag;
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    
    string soldierCode = "";  //兵种Code
    string soldierColor = "B";  //兵种Code
    
    soldierCode = m_herodata->getBaseName().c_str();
    LQ::stringf(reshead, "%s_%s", soldierColor.c_str(), soldierCode.c_str());
    LQ::stringf(resfile, "res/hero/avt_%s_%s.plist", soldierColor.c_str(), soldierCode.c_str());
    //LQ::stringf(resfile2, "res/hero/avt_%s_%s.pvr.ccz", soldierColor.c_str(),  soldierCode.c_str());
    frameCache->addSpriteFramesWithFile(resfile.c_str());  //,resfile2.c_str()
    LQ::stringf(resfile, "res/hero/avt_%s_%s.plist", "A", soldierCode.c_str());
    frameCache->addSpriteFramesWithFile(resfile.c_str());
    m_hero = LQCombatHero::create(m_herodata, m_squadDirection);
    m_hero->setSquad(this);
    m_bannerman = m_hero;
    m_direction = m_hero->getDirection();
    m_radius = m_hero->getRadius();
    m_view = m_hero->getView();
    m_atkview = m_herodata->getAtkView();
    m_armsType = m_herodata->getArmsType();
    m_occupation = m_herodata->getOccupation();
    m_attackFrequency = m_herodata->getAttackAnimTime();
    
    soldierColor = m_squadDirection==klqCombatSquadDirectionLeft ? "B" : "R";
    
    //建一个头像旗帜
    m_armflags = LQFlagsSprite::createWithData(m_squadData);
    //m_armflags->setScale(1.3);
    FlagPosition::getInstance()->pushNode(m_armflags);
    
    if(m_herodata->getSoldiers()->size()<=0)
        return;
    soldierCode = m_herodata->getSoldiers()->at(0)->getBaseName().c_str(); // code;
    //LQ::stringf(reshead, "%s_%s", soldierColor.c_str(), soldierCode.c_str());
    LQ::stringf(resfile, "res/soldier/avt_%s_%s.plist", soldierColor.c_str(), soldierCode.c_str());
    //LQ::stringf(resfile2, "res/soldier/avt_%s_%s.pvr.ccz", soldierColor.c_str(),  soldierCode.c_str());
    frameCache->addSpriteFramesWithFile(resfile.c_str());  //,resfile2.c_str()
    if(_DEBUG_TYPE==_DEBUG_TYPE_SAFE) CCLOG("addSpriteFramesWithFile: %s", resfile.c_str());
    // add your codes here...
    int row,col;
    LQArmsUserData* soldier;
    
    //m_HpValue = m_hero->getHPValue();
    //m_soldierHpValue = 0;
    for(int i=0; i<m_herodata->getSoldiers()->size(); i++)
    {
        soldier = m_herodata->getSoldiers()->at(i);
        row = i/SQUAD_NUMCOL;
        col = i%SQUAD_NUMCOL;
        //Point p = convertPoint(row,col);
        LQCombatSoldier* unit = LQCombatSoldier::create(soldier, m_squadDirection);
        unit->setUnitXY(row,col);
        unit->setIndex(i);
        unit->setSquad(this);
        unit->setOccupation(m_occupation);
        
        if(i==0)
            m_atkview = MAX(m_atkview, soldier->getAtkView());
        
        //m_HpValue+=unit->getHPValue();
        //m_soldierHpValue += unit->getHPValue();
        m_combatUnits->push_back(unit);
    }
    
    //需要用到的固定数据
    if (m_atkview<=5) m_atkview += 2.9;  //近战兵种扩大3个格子范围2.9
    m_UnitHp = m_herodata->getDefaultSoldierHP();   //单兵的血量，即可以造成单兵的死亡
    //m_HpValue = m_squadData->getHpValue();
    m_HpValueMax = m_squadData->getHpValueMax();    //记录最大血
    m_herodata->setSquadData(m_squadData);
    
    _squadfuries = getFormulaParamFactor(21, "a");     //杀死军团怒气
}

//获取当前攻击距离，弓兵会被技能触发成狂暴变成近战兵种
float LQCombatSquad::getCurrAtkView()
{
    float dist = this->getSquadData()->getCurrStdAtr(ATTR_ATKDIST);
    //if (dist>0.01 || this->getHero()->getIndex()==16){
    //    CCLOG("getCurrAtkView.. 弓兵会被技能触发成狂暴变成近战兵种.. %.3f", dist);
    //}
    return dist>0 ? dist : m_atkview;
}

//设置军团的目标
void LQCombatSquad::setTarget(GameObj* target)
{
    //被迫换对手
    if ((m_state != FUSM_SQSTATE_ATTACK && m_target && m_target->getActive() && m_target!=target)
        || (m_state == FUSM_SQSTATE_ATTACK && !target))
    {
        if(m_hero) m_hero->setTarget(NULL);
        LQCombatUnit* pUnit = NULL;
        for (int i=0; i<m_combatUnits->size(); i++)
        {
            pUnit = m_combatUnits->at(i);
            pUnit->setTarget(NULL);
        }
        if(m_state == FUSM_SQSTATE_ATTACK)
            m_state = FUSM_SQSTATE_APPROACH;
    }
    
    GameObj::setTarget(target);
    this->drawmArmflags();
        
    //巡逻->追踪
    if(target){
         if (target->getActive())
             m_state = FUSM_SQSTATE_APPROACH;
    }
    
    m_firstLock = false;
}

//是否转换目标，true可以转换  :::（远程）军团当被攻击时间超过0.7时切换目标
bool LQCombatSquad::getAttackSquadMelee(){
    if(!m_target)
        return true;
    if (this->getArmsType()!=Combat_ArmsType_Remote)
        return false;
    if (m_taunter)
        return false;  //有嘲讽对象，不切换目标
    LQCombatSquad* tsquad = this->getTargetSquad();
    if (this->getArmsType()==Combat_ArmsType_Remote && tsquad->getArmsType()==Combat_ArmsType_Remote)  //弓对弓转换
        return true;
    if (m_target->getTarget()==this)  //互为攻击目标
        return false;
    return true;
}

//目标对象军团
LQCombatSquad* LQCombatSquad::getTargetSquad(GameObj* target)
{
    if (target==NULL)
        target = m_target;
    LQCombatSquad* squad = NULL;
    if (target){
        if (target->getType() == klqLQGameObjSquad) {
            squad = (LQCombatSquad*)target;
        }else{
            squad = ((LQCombatSoldier*)target)->getSquad();
        }
    }
    return squad;
}

//放置到场景上,初始化
// parent 地图，方阵放置的场景
// index  方阵在阵型的序号
void LQCombatSquad::putLayer(Node* parent, short index)
{
    m_parent = parent;
    this->setIndex(index);
    
//    if (index>15)
//    {
//        m_squad_x = (index-3)/MAX_SQUAD_COLS + 1;  //列
//        m_squad_y = (index-3)%MAX_SQUAD_COLS + 1;  //行
//    }
    
    this->setObjectUID((m_squadDirection==klqCombatSquadDirectionRight?DEF_COMBAT_INDEX:0)+index*DEF_COMBAT_IDXTIMES);
    
    int squadLen = SQUAD_MAXROW+2;  //行间隔 (兵行+将+空行)
    int beginx = m_squadDirection==klqCombatSquadDirectionRight ? MAP_COL-3-1 : 0+3;    //x列开始格子，-》自左
    int beginy = MAP_ROW-1-(SQUAD_MAXCOL/2+1);  //Y开始格子，自上向下 1+6+(6+6+6)+6+1
    if(m_squadDirection==klqCombatSquadDirectionLeft)
        beginy = MAP_ROW-1-(SQUAD_MAXCOL/2+1);
    else
        beginy = 1+(SQUAD_MAXCOL/2+1);
    
    LQBattleSession::getInstance()->addGameSquad(this);
    
    int n = m_squadDirection == klqCombatSquadDirectionRight ? (m_squad_x-1) : -(m_squad_x-1);  //我方:左->右   敌方:右->左
    int m = m_squadDirection == klqCombatSquadDirectionLeft ? m_squad_y-1 : -(m_squad_y-1);
    
    //根据阵型，调整一些兵种的出发位置
    //bool isLeft = this->getIndex()==4 || this->getIndex()==7;
    //bool isRight = this->getIndex()==6 || this->getIndex()==9;
    //if((isLeft && m_squadDirection==klqCombatSquadDirectionLeft) || (isRight && m_squadDirection==klqCombatSquadDirectionRight)){
    //    beginy += (SQUAD_MAXCOL+1);
    //}else if((isRight && m_squadDirection==klqCombatSquadDirectionLeft) || (isLeft && m_squadDirection==klqCombatSquadDirectionRight)){
    //    beginy -= (SQUAD_MAXCOL+1);
    //}
    
    Point startpt = Point(beginx+squadLen*n,beginy-(SQUAD_MAXCOL+1)*m);
    m_hero->setTileCoordandPoint(startpt.x,startpt.y);  //+(SQUAD_MAXCOL-1)/2.0f
    if(m_hero){
        parent->addChild(m_hero,parent->getContentSize().height-m_hero->getPositionY());
        m_hero->restoreHeroHalo();
        m_hero->setCritPic(m_squadData->getCritPic().c_str());
        LQBattleSession::getInstance()->addGameObj(m_hero);
        m_herodata->setIndex(m_hero->getIndex());
    }
    
    m_parent->addChild(m_armflags, 2048);
    m_bannerman->setArmFlags(m_armflags);  //初始的旗手
    
    //添加位置信息
    FlagPosition::getInstance()->pushNode(m_bannerman);
    
    m_direction = m_hero->getDirection();
    this->drawSquadFormat(m_direction);
    
    this->drawmArmflags();
    this->drawmArmflagsStatus();
    
    if (this->getIndex()>15){
        this->drawVisible(false);
    }
    
    if(_DEBUG_TYPE>0 || _DEBUG_TYPE==_DEBUG_TYPE_MOVE || _DEBUG_TYPE==_DEBUG_TYPE_TIME || _DEBUG_TYPE==_DEBUG_TYPE_SKILL){
        CCLOG("squad:%d %s ===hero %d id:%d %s", m_index, m_name.c_str() ,m_hero->getIndex(), m_herodata->getResIDC(),  m_herodata->getName().c_str());
        CCLOG("%d startpt n=%d, m=%d  n1=%f,m1=%f x=%f,y=%f", m_squadDirection, n,m,startpt.x,startpt.y,m_hero->getPositionX(),m_hero->getPositionY());
        CCLOG("patrolSpeed=%.3f, m_speed=%.3f  isVisible=%d", m_patrolSpeed, m_speed, this->isVisible());
    }
}

void LQCombatSquad::drawSquadPoint(Point postion)
{
    Point oldpt = m_hero->getPosition();
    m_hero->setTileCoordandPoint(postion.x,postion.y);
    Point pt = m_hero->getPosition()-oldpt;
    LQCombatUnit* pUnit = NULL;
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        Point pt1 = pUnit->getPosition() + pt;
        pUnit->setPosition(pt1);
        Point tilept = convertTileCoord( pt1 );
        pUnit->setTileCoord(tilept.x, tilept.y);
    }
}

//根据方向排阵
void LQCombatSquad::drawSquadFormat(LQUnitDirectionType direction)
{
    if( m_hero==NULL )
        return;
    LQCombatUnit* pUnit = NULL;
    Point startpt = m_hero->getPosition();
    m_speed = m_hero->getSpeed();
    m_patrolSpeed = m_hero->getPatrolSpeed();
    int row,col;
    Point mid = Point(FORMAT_POSTION[direction][2][0], FORMAT_POSTION[direction][2][1]);
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        row = pUnit->getUnitX();
        col = pUnit->getUnitY();
        float x =  FORMAT_POSTION[direction][col][0];
        float y =  FORMAT_POSTION[direction][col][1];
        
        Point pt = startpt+Point((x+mid.x*row)*MAP_WIDTH, (y+mid.y*row)*MAP_HEIGTH);
        pUnit->setPosition(pt);
        Point tilept = convertTileCoord( pt );
        pUnit->setTileCoord(tilept.x, tilept.y);
        pUnit->setHero(m_hero);
        if(pUnit->getSpeed()<m_speed)
            m_speed = pUnit->getSpeed();
        m_parent->addChild(pUnit,m_parent->getContentSize().height-pUnit->getPositionY());
        LQBattleSession::getInstance()->addGameObj(pUnit);
    }
    //
    if(m_patrolSpeed>0){
        m_speed *= 1.02+m_patrolSpeed/100;
    }else m_speed *= 1.02;
    //军团的速度是，最慢单元的速度slowSpeed   slowSpeed*(1.02+m_patrolSpeed/100)
    m_addSpeed = 1;
}

//显示隐藏方阵
void LQCombatSquad::drawVisible(bool val)
{
    m_squaVisible = val;
    m_armflags->setVisible(val);
    this->setVisible(val);
    m_hero->visibleHeroHalo(val);
    m_hero->setVisible(val);
    LQCombatUnit* pUnit = NULL;
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        pUnit->setVisible(val);
        if(val){
            if(val && pUnit->getSquadState()==FUSM_SQSTATE_RUN)
                pUnit->setSquadState(FUSM_SQSTATE_WAIT);
            pUnit->changeAnimation();
        }
    }
    
    if(val && m_hero->getSquadState()==FUSM_SQSTATE_RUN)
        m_hero->setSquadState(FUSM_SQSTATE_WAIT);
}

// 获取开场冲锋速度 squadSpeed*squadAddSpeed
float LQCombatSquad::getRunSpeed(){
    float rSpeed = 0;
    if( m_squad_x==klqCombatSquadRowBack && m_state==FUSM_SQSTATE_RUN && m_bannerman->getMoveMileage()<SQUAD_BACKROW_RUNS)  //后排冲锋时处理
    {
        rSpeed = this->getSlowSpeed();
    }else
        rSpeed = this->getSpeed()*this->getAddSpeed();
    
    if (m_bannerman->getArmdata()->getSpeedBuffAdd()!=0 ){ //被加速和减速了4.9
        rSpeed = m_bannerman->getCurrSpeedValue();
    }
    //if(m_index==3 || m_index==8) CCLOG("%d getRunSpeed=%0.3f", m_index, rSpeed);
    return rSpeed;
};

//变换阵型.战斗和其他
void LQCombatSquad::trunFormation(Point targetpt)
{
    m_isRalliedCount = 0;
    Point startpt = m_bannerman->getPosition();
    Point pt = targetpt-startpt;
    //当距离大于一个范围，集结应该在某一段就完成了，取一小段为目标点
    float d = targetpt.getDistance(startpt);
    float a = pt.getAngle();
    Point _targetpt = targetpt;  //集结的目标位置
    LQUnitDirectionType direction = calcDirectionFromPoint(pt);
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("SINDX%s: 变换阵型 trunFormation: %s -> %s", m_name.c_str(), PointString(startpt).c_str(),  PointString(targetpt).c_str());
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG(" targetpt 2: x=%f y=%f a=%f d=%d", pt.x,  pt.y, a, direction);
    float max_len = MAP_WIDTH*2;  //集结完成的最小范围
    if (d>max_len){
        _targetpt = startpt + Point(max_len*cos(a),max_len*sin(a));
    }
    m_bannerman->setArrayPoint(_targetpt);
    //m_bannerman->setPosition(_targetpt);
    //m_bannerman->setDirection(direction);
    
    int value = (arc4random() % 6) + 1;
    value = 1;
    
    LQCombatSoldier* pUnit = NULL;
    int row,col;
    float mind0 = _targetpt.getDistance(m_bannerman->getPosition());
    float maxd0 = mind0;
    Point mid = Point(FORMAT_POSTION[direction][2][0], FORMAT_POSTION[direction][2][1]);
    int k = 0;
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        if(pUnit==m_bannerman)
            continue;
        row = k/SQUAD_MAXCOL;
        col = k%SQUAD_MAXCOL;
        pUnit->setUnitXY(row, col);
        float x =  FORMAT_POSTION[direction][col][0];
        float y =  FORMAT_POSTION[direction][col][1];
        Point newpt = _targetpt+Point((x+mid.x*row)*MAP_WIDTH, (y+mid.y*row)*MAP_HEIGTH);
        //pUnit->setPosition(newpt);
        //pUnit->setDirection(direction);
        
        
        if (value == 1) {
            x = FORMAT_POSTION[direction][col][0];
            y = FORMAT_POSTION[direction][col][1];
            int spacing0 = 3 * (i % 4);
            int spacing1 = 3 * (i % 4);
            newpt = _targetpt+Point((x+mid.x*(row))*(MAP_WIDTH+spacing0+10),
                                    (y+mid.y*row)*(MAP_HEIGTH+spacing1+10));
        }else if (value == 2) {
            // 可做骑士跑步用  哪个尖尖的队形  <<·
            int wh = 15; // 间距
            int centerindex = 3; //3列 //int(m_combatUnits->size() / 4);
            int mi = i % 7; // 每列有7个
            int mpi = i / 7;
            if(m_combatUnits->size()-1 == i){mi = 3;} // 最后一个让他在中间
            int ofx = mi < centerindex ? -mi*wh + wh*centerindex : mi*wh - wh*centerindex;
            float rx = ofx + 50 + (50 * mpi); //偏移
            float ry = (centerindex - mi) * wh;
            
            Point pos = Point(rx,ry);
            switch (direction) {
                case klqUnitDirectionTypeNone:{
                }break;
                case klqUnitDirectionTypeDown:{
                    pos = Point(ry,rx);
                }break;
                case klqUnitDirectionTypeLeft:{
                    pos = Point(rx,ry);
                }break;
                case klqUnitDirectionTypeUp:{
                    pos = Point(ry,-rx);
                }break;
                case klqUnitDirectionTypeRight:{
                    pos = Point(-rx,ry);
                }break;
                case klqUnitDirectionTypeLeftDown:{
                    pos = Point(rx,ry);
                }break;
                case klqUnitDirectionTypeLeftUp:{
                    pos = Point(rx,ry);
                }break;
                case klqUnitDirectionTypeRightDown:{
                    pos = Point(-rx,ry);
                }break;
                case klqUnitDirectionTypeRightUp:{
                    pos = Point(-rx,ry);
                }break;
                default:{
                }break;
            }
            newpt = _targetpt+pos;
        } else if(value == 3) {
            // 前 半圆阵型
            const float FORMAT_RADIUS[8] = {9,17,24,1,12,21,5,28};
            float radius = 80; // 到圆心的距离
            float radian = 0.2 * (i + FORMAT_RADIUS[direction]);// 根据方向给一个不同的角度
            float rx = radius * sin(radian);
            float ry = radius * cos(radian);
            newpt = _targetpt+Point(rx,ry);
        } else if(value == 4) {
            // 后 半圆阵型
            const float FORMAT_RADIUS[8] = {24,17,9,1,20,11,28,7};
            float radius = 80; // 到圆心的距离
            float radian = -0.2 * (i + FORMAT_RADIUS[direction]);// 根据方向给一个不同的角度
            float rx = radius * sin(radian);
            float ry = radius * cos(radian);
            newpt = _targetpt+Point(rx,ry);
        } else if(value == 5) {
            float radius = 80; // 到圆心的距离
            float radian = 0.42 * i; // 圆上间距
            float rx = radius * sin(radian);
            float ry = radius * cos(radian);
            newpt = _targetpt+Point(rx,ry);
        }else if(value == 6) {
            //  一个大于号的队形 目测没用了
            //            int centerindex = int(m_combatUnits->size() / 2);
            //            int ofx = i < centerindex ? -i*20 + 20*centerindex: i*20 - 20*centerindex;
            //            float rx = ofx;//(centerindex - ofx) * 20;
            //            float ry = (centerindex - i) * 20;
            //            newpt = _targetpt+Point(rx,ry);
            x =  FORMAT_POSTION[direction][col][0];
            y =  FORMAT_POSTION[direction][col][1];
            newpt = _targetpt+Point((x+mid.x*row)*MAP_WIDTH, (y+mid.y*row)*MAP_HEIGTH);
            
        }
        
        pUnit->setArrayPoint(newpt);
        
        float _d0 = abs(newpt.getDistance(pUnit->getPosition()));
        if(_d0>mind0)
            mind0 = _d0;
        else maxd0 = _d0;
        k++;
    }
    //阵型同步系数
    if (mind0==0 && maxd0==0)  { mind0 = 1; maxd0 = 1;};
    float r = maxd0>mind0 ? maxd0/mind0 : mind0/maxd0;    //d/(max_len*6);
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("trunFormation %f / %f, %f", mind0, maxd0, r);
    float _accelerated = 1;  //r<1?1:(r>2.0?2.0:r); //整体加速了2.0倍
    float _d0 = m_bannerman->getArrayPoint().getDistance(m_bannerman->getPosition());
    m_bannerman->setArraySyncSpeed((_d0/mind0)*_accelerated);
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        if(pUnit==m_bannerman)
            continue;
        _d0 = pUnit->getArrayPoint().getDistance(pUnit->getPosition());
        pUnit->setArraySyncSpeed((_d0/mind0)*_accelerated);
    }
    //集结开始
    m_isStand = false;
    m_israllied = false;
}

//旋转阵型.转弯和集结
void LQCombatSquad::trunRound(Point targetpt)
{
    m_isRalliedCount = 0;
    Point startpt = m_bannerman->getPosition();
    Point pt = targetpt-startpt;
    //当距离大于一个范围，集结应该在某一段就完成了，取一小段为目标点
    float d = targetpt.getDistance(startpt);
    float a = pt.getAngle();
    Point _targetpt = targetpt;  //集结的目标位置
    LQUnitDirectionType direction = calcDirectionFromPoint(pt);
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("SINDX%s: 集结 trunRound: %s -> %s", m_name.c_str(), PointString(startpt).c_str(),  PointString(targetpt).c_str());
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG(" targetpt 2: x=%f y=%f a=%f d=%d", pt.x,  pt.y, a, direction);
    float max_len = MAP_WIDTH*2;  //集结完成的最小范围
    if (d>max_len){
        _targetpt = startpt + Point(max_len*cos(a),max_len*sin(a));
    }
    m_bannerman->setArrayPoint(_targetpt);
    //m_bannerman->setPosition(_targetpt);
    //m_bannerman->setDirection(direction);
    
    LQCombatSoldier* pUnit = NULL;
    int row,col;
    float mind0 = _targetpt.getDistance(m_bannerman->getPosition());
    //float maxd0 = mind0;
    Point mid = Point(FORMAT_POSTION[direction][2][0], FORMAT_POSTION[direction][2][1]);
    int k = 0;
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        if(pUnit==m_bannerman)
            continue;
        row = k/SQUAD_MAXCOL;
        col = k%SQUAD_MAXCOL;
        pUnit->setUnitXY(row, col);
        float x =  FORMAT_POSTION[direction][col][0];
        float y =  FORMAT_POSTION[direction][col][1];
        Point newpt = _targetpt+Point((x+mid.x*row)*MAP_WIDTH, (y+mid.y*row)*MAP_HEIGTH);
        //pUnit->setPosition(newpt);
        //pUnit->setDirection(direction);
        pUnit->setArrayPoint(newpt);
        
        //float _d0 = abs(newpt.getDistance(pUnit->getPosition()));
        //if(_d0>mind0)
        //    mind0 = _d0;
        //else maxd0 = _d0;
        k++;
    }
    //阵型同步系数
    //float r = maxd0>mind0 ? maxd0/mind0 : mind0/maxd0;    //d/(max_len*6);
    //CCLOG("trunRound %f / %f, %f", mind0, maxd0, r);
    float _accelerated = 1;  //r<1?1:(r>2.0?2.0:r); //整体加速了2.0倍
    float _d0 = m_bannerman->getArrayPoint().getDistance(m_bannerman->getPosition());
    m_bannerman->setArraySyncSpeed(1);  //(_d0/mind0)*_accelerated
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        if(pUnit==m_bannerman)
            continue;
        _d0 = pUnit->getArrayPoint().getDistance(pUnit->getPosition());
        pUnit->setArraySyncSpeed((_d0/mind0)*_accelerated);
    }
    //集结开始
    m_isStand = false;
    m_israllied = false;
}

//到达集结点。集结计数，现在不需要记录谁到没到
void LQCombatSquad::doArrived(LQCombatUnit* unit)
{
    m_isRalliedCount++;
    //中途死了怎么办？TODO
    int max = m_combatUnits->size()+(m_bannerman->getType()==klqLQGameObjHero?1:0);
    m_israllied = m_isRalliedCount>=max;
    
    //_LQDEBUG(_DEBUG_TYPE_MOVE, _DEBUG_INDEX_MAX, " %d(%d) 到了集结点 %s", unit->getIndex(), m_isRalliedCount, PointString(unit->getPosition()).c_str());
    
    if(m_israllied)
        this->onRallydCompleted();
}

//队伍集结完毕调用
void LQCombatSquad::onRallydCompleted()
{
    //和旗手的方向保持一致
    LQCombatSoldier* pUnit = NULL;
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        if (pUnit!=this->getBannerman())
            pUnit->setDirection( this->getBannerman()->getDirection() );
        //CCLOG("%d: %d %d", i, pUnit->getDirection(), this->getBannerman()->getDirection());
    }
    
    //集结完成，同步旗手的状态
    this->setIsStand(m_bannerman->getIsStand());
    _LQDEBUG(_DEBUG_TYPE_MOVE, _DEBUG_INDEX_MAX, " 集结点完毕.....");
    //Director::getInstance()->pause();
}

//到达目的地
void LQCombatSquad::doArrivedDestination()
{
    updateWaitCommand();
    _LQDEBUG(_DEBUG_TYPE_MOVE, _DEBUG_INDEX_MAX, " 到达目的地.....");
    if(!m_israllied){
        CCLOG("还没有集结完毕");
    }
}

//设置目标点
void LQCombatSquad::setTargetPoint(Point target)
{
    GameObj::setTargetPoint(target);
    
    //传递给旗手
    m_bannerman->setTargetPoint(target);
}

//绘制旗帜的内容
void LQCombatSquad::drawmArmflags(float scale)
{
    if(!m_armflags)
        return;
    
    m_armflags->updateValue();
    
    if(scale>0)
        m_armflags->setScale(scale);
}

//变更旗帜的状态
void LQCombatSquad::drawmArmflagsStatus()
{
    //return;
    if(!this->getActive())
        return;
    if (!m_armflags) {
        return;
    }
    
    m_armflags->updateStatus(m_state);
    
    /*
    Sprite* sp = (Sprite*)m_armflags->getChildByTag(2000);
    if(sp)
        sp->removeFromParent();
    string stname;
    if(m_state==FUSM_SQSTATE_RUN)
        stname = "squadrun.png";
    else if(m_state==FUSM_SQSTATE_APPROACH)
        stname = "squadapproach.png";
    else if(m_state==FUSM_SQSTATE_ATTACK)
        stname = "squadattack.png";
    //else if(m_state==FUSM_SQSTATE_PATROL)
    //    stname = "squadidea.png";
    else if(m_state==FUSM_SQSTATE_WAIT)
        stname = "squadidea.png";
    else stname = "squadwait.png";
    sp = Sprite::createWithSpriteFrameName(stname.c_str());
    sp->setAnchorPoint(Point(0.5, 0));
    sp->setPosition(m_armflags->getContentSize().width/2,m_armflags->getContentSize().height*0.2);
    sp->setTag(2000);
    m_armflags->addChild(sp);*/
}

// 11,12,13
// 21,22,23
// 31,32,33
void LQCombatSquad::setIndex(int value)
{
    m_index = value+1;
    m_squad_x = value/MAX_SQUAD_COLS + 1;  //列
    m_squad_y = value%MAX_SQUAD_COLS + 1;  //行
    
    stringf(m_name, "%s%d", m_squadDirection==klqCombatSquadDirectionLeft?"B":"R", m_index);
}

// type_x_y
string LQCombatSquad::getKey()
{
    string str;
    stringf(str, "%d_%d_%d", m_squadDirection, m_squad_x, m_squad_y);
    //if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("getKey %s",str.c_str());
    return str;
}

//获取军团坐标位置
Point LQCombatSquad::getLQPosition()
{
    if (m_bannerman && m_bannerman->getActive())
        return m_bannerman->getPosition();
    return Point::ZERO;
}

//获取中心位置
Point LQCombatSquad::getHitPosition()
{
    Point pt = Point::ZERO;
    if (m_bannerman && m_bannerman->getActive())
        pt = m_bannerman->getPosition();
    
    Point minpt = Point::ZERO;
    Point maxpt = Point::ZERO;
    for (int i = 0; i < m_combatUnits->size(); i++) {
        LQCombatSoldier* soldier = m_combatUnits->at(i);
        if (!soldier || !soldier->getActive()) {
            continue;
        }
        if(soldier->getPositionX()>maxpt.x || maxpt.x==0 )
            maxpt.x = soldier->getPositionX();
        else if(soldier->getPositionX()<minpt.x)
            minpt.x = soldier->getPositionX();
        if(minpt.x==0) minpt.x = soldier->getPositionX();
        if(soldier->getPositionY()>maxpt.y || maxpt.y==0 )
            maxpt.y = soldier->getPositionY();
        else if(soldier->getPositionY()<minpt.x)
            minpt.y = soldier->getPositionY();
        if(minpt.y==0) minpt.y = soldier->getPositionY();
    }
    if (minpt!=Point::ZERO && maxpt!=Point::ZERO){
        pt = minpt + (maxpt-minpt)*0.5;
    }
    _LQDEBUG(_DEBUG_TYPE_MOVE, "getSquadCenter %s %s", PointString(pt).c_str(), PointString(getLQPosition()).c_str());
    return pt;
}

//获取军团TileCoord坐标位置
Point LQCombatSquad::getTileCoord()
{
    if (m_bannerman && m_bannerman->getActive())
        return m_bannerman->getTileCoord();
    return Point::ZERO;
}

//添加新的伤害
void LQCombatSquad::modifyHurtHp(int hurt, bool iscrit, string critPic, string critFont, bool isskill, Point pt, int times, float outtime)
{
    if (hurt==0)
        return;
    
    if (m_armsType==Combat_ArmsType_Remote && hurt>0)  //弓兵伤害系数 弓兵承受1.2的伤害
    {
        hurt = hurt*archerHurt;
        if (_DEBUG_TYPE_VALUE==_DEBUG_TYPE || _DEBUG_TYPE_SKILL==_DEBUG_TYPE)
            CCLOG("  弓兵承受的伤害系数%f后 = %d  ", archerHurt, hurt );
    }
    
    //3.2增加护盾逻辑，存在护盾的先减掉护盾血条，再才是当前血条
    m_totalHurtHp += m_squadData->changeSquadHp(hurt);
    if (_hdbufferData && m_squadData->getCurrShValue()<=0){   //将护盾打没有了
        this->removeBuffer(_hdbufferData);
        _hdbufferData = NULL;
    }
    //CCLOG("LQCombatSquad[%s] modifyHurtHp = %d  getSoldierHpValue = %d", m_name.c_str(), m_totalHurtHp, m_squadData->getSoldierHpValue());
    
    m_armflags->updateValue();
    
    float _x = randomInt(1, 20)*0.05;
    float _y = randomInt(1, 10, false)*0.2;
    int _hurt = -hurt/100.00f;
    Point mpt = Point::ZERO;
    string fontname;
    if(iscrit || isskill){
        if (outtime<0) outtime = 0;
        if(iscrit){
            LQFlyNumber* pCrit = LQFlyNumber::createFlyWithSpriteFrameName(critPic.c_str(), 0, klq_flaytype_criticon);
            if (pt==Point::ZERO) pt = this->getPosition();
            pCrit->setPosition( pt+Point(pCrit->getContentSize().width*_x,pCrit->getContentSize().height*_y) );
            m_bannerman->getParent()->addChild(pCrit, 5048);
            pCrit->startAnimation(outtime);
            _LQDEBUG(_DEBUG_TYPE_VALUE, "军团对目标暴击飘字%s %d", critPic.c_str(), _hurt);
        }
        _x = randomInt(1, 10)*0.05;
        _y = randomInt(1, 10, false)*0.2;
        //mpt = Point(times*randomInt(5, 10)*(randomInt(1, 10)>5?1:-1), times*(15+randomInt(1, 10)));
        LQFlyNumber* pHp = LQFlyNumber::createFlyWithFile("", _hurt, iscrit? klq_flaytype_crit : klq_flaytype_skill, true);
        if(times>0){
            mpt = Point(pHp->getContentSize().height*3.2*times*(_x>0.2?1:-1),pHp->getContentSize().height*1.8*times);
        }
        if (critFont.length()<=0) critFont = "normal_skill_cure";
        stringf(fontname, "res/fonts/%s.fnt", critFont.c_str());
        //pHp->setPassiveFontNumber("normal_skill_cure"); 加血固定
        pHp->setNagetiveFontNumber(fontname.c_str());
        pHp->setPosition( this->getPosition()+Point(pHp->getContentSize().width*_x,pHp->getContentSize().height*_y)+mpt );
        if (times>0) pHp->setVisible(false);
        m_bannerman->getParent()->addChild(pHp, 5049-times*2);
        pHp->setTimes(0);
        pHp->startAnimation(outtime+times*0.4);
    }else{
#if defined(LQ_DEMO_FIGHT)
#if SHOW_NORMAL_HURT
        LQFlyNumber* pHp = LQFlyNumber::createFlyWithFile("", _hurt, klq_flaytype_parry, true);
        fontname = "res/fonts/normal_des.fnt";
        pHp->setPassiveFontNumber(fontname.c_str());
        pHp->setNagetiveFontNumber(fontname.c_str());
        _x = randomInt(1, 20)*0.05;
        _y = randomInt(1, 20, false)*0.2;
        pHp->setPosition( this->getPosition()+Point(pHp->getContentSize().width*_x,pHp->getContentSize().height*_y) );
        m_bannerman->getParent()->addChild(pHp, 5049);
        pHp->startAnimation();
#endif 
#endif 
    }
    
    //New 测试
    if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE && m_squadData->getCurrHpValue()<=10)
        CCLOG("HeroHP: %d %d", m_squadData->getCurrHpValue(), m_hero->getHeroData()->getCurrHpValue());
}

void LQCombatSquad::killUnitHP()
{
    m_totalHurtHp -= m_UnitHp;
    //CCLOG("LQCombatSquad[%s] m_totalHurtHp = %d", m_name.c_str(), m_totalHurtHp);
}

//技能单个命中特效
void LQCombatSquad::skillHitEffect(LQEffectUserData* effectData,int m)
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3)
        CCLOG("8.2) %p LQCombatSquad::skillHitEffect %ld", this, getCurrentMillSecond() );
    int _hiteffect = effectData->getHitEffect();
    if (_hiteffect>0){
        LQAnimationInfoData* _animationData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), _hiteffect);
        if (_animationData->getActionType()==4 && this->getSquadDirection()==klqCombatSquadDirectionLeft)   //直线命中
            _animationData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), _hiteffect+1);
        LQBuffSprite* _hitSp = LQBuffSprite::createWithData(_animationData);  //LQBuffSprite::createWithId(_hiteffect);
        if (_animationData->getActionType()==2 && m_hero)   //单体命中
            _hitSp->setPosition(m_hero->getHitPosition());
        else if (_animationData->getActionType()==4)   //直线命中
        {
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Point origin = Director::getInstance()->getVisibleOrigin();
            Point pt = Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
            pt = m_bannerman->getParent()->convertToNodeSpace(pt);
            //CCLOG("%d skillHitEffect:::", m);
            if (m>0){
                int b0 = 10; int  k = 4;
                int x0 = (m-1)/2; int y0 = (m-1)%2;
                float dd = (this->getSquadDirection()==klqCombatSquadDirectionLeft? 1 : -1);
                Point p1 = convertPoint(b0,b0);
                Point p2 = (y0==1 ? Point(b0+k*dd+x0*k*dd,b0+k+x0*k) : Point(b0+k*dd+x0*k*dd,b0-k-x0*k));
                Point p3 = convertPoint(p2);
                //CCLOG("直线命中 %d ::: %s = %s-%s", m, PointString(p2).c_str(), PointString(p1).c_str(), PointString(p3).c_str());
                pt += (p3-p1);
            }
            _hitSp->setAnchorPoint(Point(0.5,0.55));
            _hitSp->setPosition(pt);
        }else    //集团命中
            _hitSp->setPosition(this->getHitPosition());
        
        if ( _hitSp->getHangingPoint()==4)  //底部
            m_bannerman->getParent()->addChild(_hitSp, 1);
        else m_bannerman->getParent()->addChild(_hitSp,3001);
        
        if (_animationData->getActionType()==4)
            _hitSp->playLine(this->getSquadDirection()==klqCombatSquadDirectionLeft? 1 : 0);
        else{
            if(m_effectpt>0){
                int x0 = randomInt(1, 5)-2; int y0 = randomInt(1, 5)-2;
                _hitSp->setPosition(this->getHitPosition()+Point(x0*10,y0*10));
                _hitSp->play1(m*3);
            }
            else
                _hitSp->play1(_hitSp->getFrameDelay());
            m_effectpt = m;
        }
        //控制外面的伤害飘字时间
        effectData->setOutputTime(_hitSp->getSkillAllOutputTime());
    }
}

//用于军团对目标（兵团/将）的伤害
void LQCombatSquad::timeAttack()
{
    if (!getTarget() || getActiveSoldierCountByAttackType(klqLQGameObjNone) <= 0) {
        return;
    }
    LQCombatSquad* squad = (LQCombatSquad*)getTarget();
    
    //弓对步第一次必出乱射
    if (this->getArmsType()==Combat_ArmsType_Remote && squad->getArmsType()==Combat_ArmsType_Melee && _atkCount<=0)
        m_squadData->setIsMustCrit(); 
    
    if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL)
       CCLOG("***军团对目标（兵团/将）的伤害\nLQCombatSquad[%d]::timeAttack [%d] %ld", m_index, squad->getIndex(), getCurrentMillSecond());
    //兵团对兵团的伤害
    if(squad->getIndex()==11){
        CCLOG("ERROR 1......hurt1 hurt2");
    }
    int hurt1 = getCurrentAps(squad,klqLQGameObjSoldier);
    if(hurt1!=0)
        this->changeTargetDps(squad, hurt1, m_squadData->getIsCrit()?klq_flaytype_crit:klq_flaytype_normal);
    //兵团对武将的伤害
    int hurt2 = 0;
    if(squad->getHero()){
        //hurt2 = getCurrentAps(squad, klqLQGameObjHero);
        //12.18修改去掉武将伤害
        hurt2 = 0;
        if(hurt2!=0 )
            this->changeTargetDps(squad->getHero(), hurt2, m_squadData->getIsCrit()?klq_flaytype_crit:klq_flaytype_normal);
    }
    
    m_oldforwardEffect = Combat_ShowEffect_None;
    //if((hurt1>0 && hurt2>0) || (hurt1==0 && hurt2==0))
    //{
        //CCLOG("ERROR......hurt1 hurt2");
    //}
    _atkCount++;
    _LQDEBUG(_DEBUG_TYPE_VALUE, "timeAttack [%d次] 兵团 %s 攻击 %s \n    兵团对兵团%d 兵团对武将%d", _atkCount, this->getName().c_str(), squad->getName().c_str(), hurt1, hurt2);
    _LQDEBUG(_DEBUG_TYPE_VALUE, "    Squad::getSquadHpValue: %d getSoldiersCount:%d", squad->getSquadData()->getSquadHpValue(), squad->getSoldiersCount() );
    
    if (_DEBUG_TYPE==_DEBUG_TYPE_TIME)
        CCLOG( "timeAttack [%d次] %ld \n 兵团 %s 攻击 %s 兵团对兵团%d", _atkCount, getCurrentMillSecond(), this->getName().c_str(), squad->getName().c_str(), hurt1 );
}

//获取当前的攻击力  兵--兵 ； 兵--将
int LQCombatSquad::getCurrentAps(LQCombatSquad* target,LQGameObjType type)
{
    int hurt = 0;
    int count = 0;
    //LQCombatSquad* squad = (LQCombatSquad*)getTarget();
    if (type == klqLQGameObjSoldier) {
        //兵团---兵团
        count = getActiveSoldierCountByAttackType(klqLQGameObjSoldier);
        if (count<=0) return 0;
        hurt = m_squadData->getCurrentDPS(target->getSquadData());
        _LQDEBUG(_DEBUG_TYPE_VALUE, "LQCombatSquad::getCurrentAps 兵团---兵团 攻击个数 = %d; 攻击hurt = %d", count, hurt);
    }else if (type == klqLQGameObjHero){
        count = getActiveSoldierCountByAttackType(klqLQGameObjHero);
        /*
        if (squad->getHeroData()->getSoldierUnitType() == Combat_People_Archers) {//被弓兵索敌-- 不限数量
            maxNum = count;
        }else{//被除弓兵外的其他兵种索敌-- 限数量最大值为8
            maxNum = minInt(count, 8);
        }*/
        if (count<=0) return 0;
        hurt = m_squadData->getCurrentDPS(target->getHeroData());
        _LQDEBUG(_DEBUG_TYPE_VALUE, "LQCombatSquad::getCurrentAps 兵团---武将 攻击个数 = %d; 攻击hurt = %d", count, hurt);
    }
    return hurt;
}

//获取攻击对象为武将或者兵团的士兵的个数
//klqLQGameObjNone,     所有活跃的士兵
//klqLQGameObjSoldier,  攻击目标位士兵的士兵的个数
//klqLQGameObjHero,     攻击目标为武将却或活跃的士兵的个数
int LQCombatSquad::getActiveSoldierCountByAttackType(LQGameObjType type)
{
    if (!getActive()) {
        return 0;
    }
    int count = 0;
    
    for (int i = 0; i < m_combatUnits->size(); i++) {
        LQCombatSoldier* soldier = m_combatUnits->at(i);
        if (!soldier || !soldier->getActive()) {
            continue;
        }
        if (soldier->getStopMove())
            continue;
        if (type == klqLQGameObjNone) {
            count++;
            continue;
        }
        GameObj* target = soldier->getTarget();
        if (target && target->getActive() &&target->getType() == type) {
            count++;
        }
    }
    
    return count;
}

//获取对象的战斗公式计算器
LQAttackDataInterface* LQCombatSquad::getCalculator()
{
    return (LQAttackCalculator*)this->getSquadData();
}

//攻击目标,目标的血量变化、、以及攻击者与被攻击者的怒气的处理
void LQCombatSquad::changeTargetDps(GameObj* pTarget,int dps,LQFlyNumberTypeEnum numbertype,LQEffectInfoData* effectdata, int times, float outtime)
{
    if (dps == 0) {
        return;
    }
    
    int dps0 = dps;
    if( !pTarget || (!this->getActive() && numbertype!=klq_flaytype_buff) || (pTarget && !pTarget->getActive()) )
        return;
    
    int buffSubType = 0;
    if (effectdata)
        buffSubType = effectdata->getBuffSubType();
    
    if (pTarget==this && dps>0 && buffSubType==56){
        //56、姚果加的特殊DOT，不伤自己
        dps = 0;
        _LQDEBUG(_DEBUG_TYPE_VALUE, "Squad[%d]::56不伤自己 自杀性攻击.....");
    }
    //_LQDEBUG(_DEBUG_TYPE_VALUE, "Squad[%d]::changeTargetDps.....");
    
    LQGameObjType targetType = pTarget->getType();//用于区分伤害类型  军团、武将
    LQHeroUserData* senderHeroData = this->getHeroData();
    int hp1 = 0; int hp2 = 0;
    //被攻击者
    if(targetType == klqLQGameObjHero)//武将
    {
        LQCombatHero* target = (LQCombatHero*)pTarget;
        target->getHeroData()->changeHpValue(dps*(-1));
        senderHeroData->changeHeroDpsTotal(-dps);
        if (target->getHeroData()->getCurrHpValue() <= 0) {
            senderHeroData->changeKillTotal(1);
        }
        
        //错误产生了
        if(target->getSquad()->getSquadData()->getCurrHpValue()<=0)
        {
            target->getHeroData()->setHpValue(0);
        }
        
        target->getSquad()->drawmArmflags();
        hp1 = target->getHPValue();
        //CCLOG("军团[]HP squadData->getHpValue：%d", target->getSquadData()->getHpValue());
    }else if((targetType == klqLQGameObjSquad || targetType == klqLQGameObjSoldier) && dps!=0){
        LQCombatSquad* squad = this->getTargetSquad(pTarget);
        
        //影响治疗% ATTR_AFFECT_TREAT = 80;
        bool istreat = false;  //已经治疗变化了
        if (squad->getSquadData()->getCurrStdAtr(ATTR_AFFECT_TREAT)!=0 && dps<0){
            istreat = true;
            dps = dps*(1+squad->getSquadData()->getCurrStdAtr(ATTR_AFFECT_TREAT)*0.01);
            if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL  || _DEBUG_TYPE==_DEBUG_TYPE_VALUE)
                CCLOG("Squad[%d]::影响治疗%f.....%d", m_index, squad->getSquadData()->getCurrStdAtr(ATTR_AFFECT_TREAT), dps);
        }
        
        //增加新的伤害 m_totalHurtHp+dps
        bool iscrit = numbertype == klq_flaytype_crit;
        bool isskill = numbertype == klq_flaytype_skill || numbertype == klq_flaytype_buff;
        if(dps>squad->getSquadData()->getCurrHpValue()+squad->getSquadData()->getCurrShValue()){
            //溃败,击溃提示
            LQFlyNumber* pExit = LQFlyNumber::createFlyWithSpriteFrameName("crit_jk.png", 0, klq_flaytype_criticon);
            pExit->setPosition( squad->getHitPosition()+Point(pExit->getContentSize().width,pExit->getContentSize().height) );
            m_bannerman->getParent()->addChild(pExit, 9048);
            pExit->startAnimation(outtime);
            //溃败音效
            //CCLOG("Effect FSEKILL  Music:%s", Effect_FSEKILL_Music );
            AudioManager::getInstance()->playEffect(Effect_FSEKILL_Music);
        }
        string critPic = m_squadData->getCritPic();
        string critFont = m_squadData->getCritFont();
        if (m_forwardEffect!=Combat_ShowEffect_None && m_forwardEffect!=Combat_ShowEffect_Attack){
            critPic = m_squadData->getSpPic();
            critFont = m_squadData->getSpFont();
            if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL)
                CCLOG("squad::changeTargetDps is ForwardEffect %d; %s", m_forwardEffect,  critPic.c_str());
        }
        
        if (m_armsType==Combat_ArmsType_Remote && dps>0 && !istreat)  //弓兵伤害系数 我攻击者是弓兵 伤害*1.1
        {
            dps *= archerDM; //
            if (_DEBUG_TYPE_VALUE==_DEBUG_TYPE || _DEBUG_TYPE_SKILL==_DEBUG_TYPE)
                CCLOG("  弓兵造成的伤害系数%f后 = %d  ", archerDM, dps );
        }
        
        squad->modifyHurtHp(dps, iscrit, critPic, critFont,  isskill, this->getPosition(), times, outtime);
        senderHeroData->changeHeroDpsTotal(-dps);
        
        //暴击音效
        if(iscrit){
            int r1 = randomInt(0, 100);
            int r2 = randomInt(0, 100, false);
            int r3 = randomInt(0, 100, false);
            string hitmusic = "";
            switch (m_armsType) {
                case Combat_ArmsType_Melee: //步兵
                    AudioManager::getInstance()->playEffect(Effect_CritQJ_Music);
                    if (r1<20) hitmusic = Effect_Death_Music3;
                    break;
                case Combat_ArmsType_Remote: //远程
                    AudioManager::getInstance()->playEffect(Effect_CritLS_Music);
                    if (r1<20) hitmusic = Effect_Death_Music1;
                    if (r2>90) hitmusic = Effect_Death_Music2;
                    break;
                case Combat_ArmsType_Cavalry: //骑兵
                    AudioManager::getInstance()->playEffect(Effect_CritTJ_Music);
                    break;
                case Combat_ArmsType_Machine: //机械
                    break;
                default:
                    break;
            }
            if (r3<5) hitmusic = Effect_Death_Music4;
            else if (r3<10) hitmusic = Effect_Death_Music5;
            if(hitmusic!=""){
                //CCLOG("playEffect hitmusic:%s", hitmusic.c_str());
                AudioManager::getInstance()->playEffect(hitmusic);
            }
        }
        
        if((_DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL) && squad->getSquadData()->getCurrHpValue()<=0){
            CCLOG("%s 的兵都死了。。。。", squad->getHeroData()->getName().c_str());
        }
        while ( squad->isCanKillUnit() ) {
            //死亡一个unit 进行处理
            LQCombatSoldier* dieunit = squad->getDieUnit();
            if(!dieunit)
                break;
            squad->killUnitHP();  //从伤害取出一个UnitHP干掉一个 m_totalHurtHp-UnitHP
            if (dieunit && dieunit->getActive()) {
                //用于统计杀敌
                if (senderHeroData && dieunit->getArmdata()->getCurrHpValue() > 0) {
                    senderHeroData->changeKillTotal(1);
                    _LQDEBUG(_DEBUG_TYPE_VALUE, " HERO[%s] Die = %d",senderHeroData->getName().c_str(),senderHeroData->getKillTotal());
                    
                    //杀死小兵
                    //senderHero->getHeroData()->changeHeroFuries(_soldierfuries);
                    //_LQDEBUG(_DEBUG_TYPE_VALUE, "军团攻击小兵 死亡 +怒气%f", _soldierfuries);
                }
                dieunit->getArmdata()->setHpValue(0);
                if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
                    CCLOG("Squad[%d] 兵团的 %d[%p] 将要完蛋了 ", m_index, dieunit->getIndex(), dieunit);
            }
        }
        
        //4.22姚果加特殊 代码62，失去生命；代码66，间隔一定时间失去生命
        if (buffSubType!=62 && buffSubType!=66)
        {
            //击杀掉军团一次性给很多怒气
            if(squad->getCalculator()->getCurrHpValue()<=0){
                if(this->getHeroData())
                    this->getHeroData()->changeHeroFuries(_squadfuries);
                
                //军团的血没有了，英雄就死了
                if(squad->getHero())
                {
                    squad->getHero()->squadIsDied();
                    if(_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_VALUE )
                        CCLOG(" 军团的血没有了，英雄就死了 Hero %s", squad->getHero()->getName().c_str());
                }
                _LQDEBUG(_DEBUG_TYPE_VALUE, "军团击杀军团 死亡 +怒气%f  dps:%d", _squadfuries, dps);
            }
            
            //给被害者涨怒气
            float furyTimes = 0;
            if (this->getSquadDirection()!=squad->getSquadDirection()) //5.27增加规则，目标为同方的不加怒气，针对董卓
            {
                if(squad->getHeroData()){
                    furyTimes = squad->getSquadData()->getFuriesValue(dps, squad->getSquadData(), squad->getSquadData()->getRageBeAttack())+LQBattleSession::getInstance()->getRageDEFEND();
                    furyTimes = MIN(furyTimes,LQBattleSession::getInstance()->getRageDEFMAX());
                    furyTimes = MAX(furyTimes,squad->getHeroData()->getRageMin());
                    squad->getHeroData()->changeHeroFuries(furyTimes);
                    if(_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_VALUE )
                        CCLOG( "军团攻击军团 给被害者%p涨怒气 %d +怒气%.3f = %d ", squad->getHeroData(), squad->getHeroData()->getResIDC(), furyTimes, squad->getHeroData()->getHeroFuries());
                }
                //给自己涨怒气
                if(this->getHeroData() && numbertype<2){
                    furyTimes = this->getSquadData()->getFuriesValue(dps, squad->getSquadData(), this->getSquadData()->getRageAttack())+LQBattleSession::getInstance()->getRageATTACK();
                    furyTimes = MIN(furyTimes,LQBattleSession::getInstance()->getRageATKMAX());
                    furyTimes = MAX(furyTimes,this->getHeroData()->getRageMin());
                    this->getHeroData()->changeHeroFuries(furyTimes);
                    if(_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_VALUE )
                        CCLOG( "军团攻击军团 给自己%p涨怒气 %d +怒气%.3f = %d ", this->getHeroData(), this->getHeroData()->getResIDC(), furyTimes, this->getHeroData()->getHeroFuries());
                }
            }
            
            hp2 = squad->getCalculator()->getCurrHpValue();
            if (hp2<=0){
                _LQDEBUG(_DEBUG_TYPE_VALUE, "军团击杀军团 死亡 +怒气%f  dps:%d", _squadfuries, dps);
            }
        }
        _LQDEBUG(_DEBUG_TYPE_VALUE, "军团[%d]HP squadData->getHpValue：%d", squad->getIndex() , squad->getSquadData()->getCurrHpValue());
        squad->drawmArmflags();
    }
    
    //溅射处理 溅射提示4.10
    int splasharea = 0;
    if(effectdata)
        splasharea = effectdata->getSplashArea();
    if(splasharea>0){
        GameObjectVector* splashObjects = LQBattleSession::getInstance()->getGameObjListInArea(splasharea, this->getTargetSquad(pTarget), m_squadDirection);
        if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
            CCLOG(">>>>溅射处理 范围splasharea = %d 溅射对象数量 count = %lu", splasharea, splashObjects->size());
        for (GameObj* targetobj : *splashObjects ) {
            _LQDEBUG(_DEBUG_TYPE_VALUE, "Splash attack targetobj:%s",((LQCombatSquad*)targetobj)->getName().c_str());
            int mainAttr = effectdata->getDominantAttribute();
            m_squadData->setIsMustNotCrit();
            dps0 = LQEffectUserData::calculatorValue(effectdata, this, targetobj, m_herodata, m_squadData, mainAttr);
            if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
                CCLOG("  溅射伤害 targetobj:%s value = %d", ((LQCombatSquad*)targetobj)->getName().c_str(), dps0);
            this->changeTargetDps(targetobj, dps0, numbertype);
        }
        if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
            CCLOG(">>>>溅射处理结束");
        CC_SAFE_DELETE(splashObjects);
    }
    
    //怒气处理
    //攻击者怒气处理
    //this->getHeroData()->changeHeroFuries(this->getHeroData()->getRageAttack());

    if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
        CCLOG("****** 军团%s攻击后 敌人%s Hero:%d Squad:%d", this->getName().c_str(), pTarget->getName().c_str(), hp1, hp2);
}

//BUFF
//添加BUFF
void LQCombatSquad::addBuffer(LQBuffUserData* bufferData)
{
    bufferData->setTarget(this);
    
    //if (m_hero->getIndex()==80)
    //    CCLOG("addBuffer.....");
    
    //添加bufferData
    ((LQAttackCalculator*)this->getCalculator())->addBufferData(bufferData);
    //添加头像图标
    m_armflags->showBufferName(bufferData->getCfgdata()->getEffectText());
    m_armflags->addBufferIcon(bufferData);
    //添加Buff特效
    if( bufferData->getAnimationType()==klqAnimationDisplayTypeAnimation)  //动画
    {
        for (int i = 0; i < m_combatUnits->size(); i++) {
            LQCombatSoldier* tempUnit = m_combatUnits->at(i);
            if (!tempUnit->getActive() || tempUnit->getArmdata()->getCurrHpValue()<=0 ) {
                continue;
            }
            tempUnit->addBufferAnimation(bufferData->getAnimationCfgData(), bufferData->getTagId());
        }
    }else if( bufferData->getAnimationType()==klqAnimationDisplayTypeIcon)  //图标
    {
        if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
            CCLOG("Squad:: %d addBuffer %s %s",bufferData->getResIDC(), bufferData->getCfgdata()->getEffectText().c_str(), bufferData->getBufferIcon().c_str());
        for (int i = 0; i < m_combatUnits->size(); i++) {
            LQCombatSoldier* tempUnit = m_combatUnits->at(i);
            if (!tempUnit->getActive() || tempUnit->getArmdata()->getCurrHpValue()<=0 ) {
                continue;
            }
            tempUnit->addBufferIcon(bufferData);
        }
        if(m_hero) m_hero->addBufferIcon(bufferData);
    }else if( bufferData->getAnimationType()==klqAnimationDisplayTypeBmp)  //图片
    {
        if(m_hero)
            m_hero->addBufferBmp(bufferData);
    }else if( bufferData->getAnimationType()==klqAnimationDisplayTypeEffect)  //特效
    {
        if(m_hero)
            m_hero->addBufferAnimation(bufferData->getAnimationCfgData(), bufferData->getTagId());
    }
}

void LQCombatSquad::addBufferBmp(LQBuffUserData* bufferData)
{
    if(bufferData->getBufferIcon().length()<=0)
        return;
    Sprite* sp = Sprite::createWithSpriteFrameName(bufferData->getBufferIcon().c_str());
    sp->setPosition( this->getLQPosition() );
    sp->setAnchorPoint(Point(0.5,0));
    sp->setTag(TAG_BUFFER_ID);
    this->addChild( sp,100 );
}

//删除BUFF
void LQCombatSquad::removeBuffer(LQBuffUserData* bufferData)
{
    //if (m_hero->getIndex()==80)
    //    CCLOG("removeBuffer.....");
    
    m_armflags->removeBufferIcon(bufferData);
    //添加Buff特效
    if( bufferData->getAnimationType()==klqAnimationDisplayTypeAnimation)  //动画
    {
        for (int i = 0; i < m_combatUnits->size(); i++) {
            LQCombatSoldier* tempUnit = m_combatUnits->at(i);
            if (!tempUnit->getActive() || tempUnit->getArmdata()->getCurrHpValue()<=0 ) {
                continue;
            }
            tempUnit->removeBufferAnimation(bufferData->getTagId());
        }
        
    }else if( bufferData->getAnimationType()==klqAnimationDisplayTypeIcon)  //图标
    {
        if(m_hero)
            m_hero->removeBufferIcon(bufferData);
        for (int i = 0; i < m_combatUnits->size(); i++) {
            LQCombatSoldier* tempUnit = m_combatUnits->at(i);
            if (!tempUnit->getActive() || tempUnit->getArmdata()->getCurrHpValue()<=0 ) {
                continue;
            }
            tempUnit->removeBufferIcon(bufferData);
        }
    }else if( bufferData->getAnimationType()==klqAnimationDisplayTypeBmp)  //图片
    {
        if(m_hero)
            m_hero->removeChildByTag(TAG_BUFFER_ID);
    }else if( bufferData->getAnimationType()==klqAnimationDisplayTypeEffect)  //特效
    {
        if(m_hero)
            m_hero->removeBufferAnimation(bufferData->getTagId());
    }
    //删除bufferData
    ((LQAttackCalculator*)this->getCalculator())->removeBufferData(bufferData);
}

//清除所有BUFF
void LQCombatSquad::cleareBuffer()
{
    ((LQAttackCalculator*)this->getCalculator())->cleareBuffer();
}

//buff触发数据动作
void LQCombatSquad::bufferCallback(Ref* pSender)
{
    LQBuffUserData* bufferData = (LQBuffUserData*)pSender;
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_BUFF)
        CCLOG("Buff LQCombatSquad::bufferCallback..... %p", bufferData);
    
    //BUF放缩
    if (bufferData->getScalefactor()>0){
        //CCLOG("%d BUF放缩: Scale:%.3f Scalefactor:%.3f", m_bannerman->getIndex(), m_bannerman->getScale(), bufferData->getScalefactor());
        m_bannerman->setScale(bufferData->getScalefactor());
    }
    
    switch ( bufferData->getCfgdata()->getBuffSubType() ) {
        case 1:  //1、属性加值
        case 2:  //2、属性减值
        case 62: //62、特殊属性减值
        {
            if(bufferData->getOutputType()==ATTR_RAGE){
                bufferData->getTarget()->changeHeroFuries(bufferData->getBuffValue());
            }else
                this->getCalculator()->addCurrStdAtr(bufferData->getOutputType(), bufferData->getBuffValue());
        }
            break;
        case 3:  //3、加速
        case 4:  //4、减速
        {
            float value = bufferData->getBuffValue();
            if (value>1)
                CCLOG("Buff SpeedBuffAdd %.3f is >1.............!@#$$%%%%", value);
            ///CCASSERT(abs(value)<10, "error: Add SpeedBuff is big!!!");
            for (int i = 0; i < m_combatUnits->size(); i++) {
                LQCombatSoldier* tempUnit = m_combatUnits->at(i);
                tempUnit->getArmdata()->setSpeedBuffAdd(value);
            }
            this->getHeroData()->setSpeedBuffAdd(value);
            if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
                CCLOG("Buff SpeedBuffAdd = %.3f", value);
        }
            break;
        case 5:  //5、间隔一定时间加值
        case 6:  //6、间隔一定时间减值
        case 66: //66、特殊间隔一定时间减值
        {
            if(bufferData->getOutputType()==ATTR_RAGE){
                bufferData->getTarget()->changeHeroFuries(bufferData->getBuffValue());
            }else if(bufferData->getOutputType()==ATTR_HP){
                LQCombatHero* heroUnit = (LQCombatHero*)bufferData->getHero();
                heroUnit->changeTargetDps(bufferData->getTarget(), bufferData->getDotValue(), klq_flaytype_buff,bufferData->getCfgdata());
            }else
                CCLOG("error: Buff OutputType must is 5!!!");
        }
            break;
        case 15: //15、间隔一定时间加变动值
        case 16: //16、特殊间隔一定时间减变动值
        {
            if(bufferData->getOutputType()==ATTR_HP){
                LQCombatHero* heroUnit = (LQCombatHero*)bufferData->getHero();
                int mainAttr = bufferData->getDominantAttribute();
                int dps0 = LQEffectUserData::calculatorValue(bufferData->getCfgdata(), this, bufferData->getTarget(), m_herodata, m_squadData, mainAttr);
                bufferData->setDotValue(dps0);  //换掉DOT，将会在后面累计输出
                heroUnit->changeTargetDps(bufferData->getTarget(), bufferData->getDotValue(), klq_flaytype_buff, bufferData->getCfgdata());
            }else
                CCLOG("error: Buff OutputType must is 15!!!");
        }
            break;
        case 56: //56、姚果加的特殊DOT，不伤自己
        {
            if(bufferData->getOutputType()==ATTR_HP){
                LQCombatHero* heroUnit = (LQCombatHero*)bufferData->getHero();
                //bufferData->setDotValue(0);  //换掉DOT，将会在后面累计输出
                heroUnit->changeTargetDps(bufferData->getTarget(), bufferData->getDotValue(), klq_flaytype_buff, bufferData->getCfgdata());
            }else
                CCLOG("error: Buff OutputType must is 56!!!");
            break;
        }
        case 7:  //7、禁止移动
        {
            for (int i = 0; i < m_combatUnits->size(); i++) {
                LQCombatSoldier* tempUnit = m_combatUnits->at(i);
                tempUnit->stopMove(true);
            }
            this->getHero()->stopMove(true);
            this->showForwardEffect(Combat_ShowEffect_None);
        }
            break;
        case 8:  //8、禁止暴击
        {
            this->getSquadData()->stopCrit(true);
        }
            break;
        case 9:  //9、禁止技能
        {
            this->getHero()->stopSkill(true);
        }
            break;
        case 10:  //10、无视防御
        {
            
        }
            break;
        case 11:  //11、无视攻击
        {
            
        }
        case 12:  //12、禁锢
        {
            for (int i = 0; i < m_combatUnits->size(); i++) {
                LQCombatSoldier* tempUnit = m_combatUnits->at(i);
                tempUnit->durance(true);
            }
            this->getHero()->durance(true);
        }
            break;
        case 13:  //13、护盾
        {
            //this->getSquadData()->setCurrStdAtr(ATTR_SHIELD,1110);
            _hdbufferData = bufferData;
        }
            break;
        case 14:  //14、白刃
        {
            this->getCalculator()->addCurrStdAtr(bufferData->getOutputType(),bufferData->getBuffValue());
            this->getCalculator()->setCurrStdAtr(ATTR_ATKDIST,2);
            this->setTarget(NULL);
        }
            break;
        case 17:  //17、嘲讽
        {
            this->taunt(bufferData->getHero());
        }
            break;
    }
    //变化的刀光特效
    if (bufferData->getFlashID().length()>0){
        for (int i = 0; i < m_combatUnits->size(); i++) {
            LQCombatSoldier* tempUnit = m_combatUnits->at(i);
            tempUnit->setBuffFlashID(bufferData->getFlashID());
        }
        m_bannerman->setBuffFlashID(bufferData->getFlashID());
    }
}

//buff结束动作
void LQCombatSquad::bufferEndCallback(Ref* pSender)
{
    LQBuffUserData* bufferData = (LQBuffUserData*)pSender;
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_VALUE || _DEBUG_TYPE==_DEBUG_TYPE_BUFF)
        CCLOG("Buff LQCombatSquad::bufferEndCallback..... %d", bufferData->getResIDC());
    switch ( bufferData->getCfgdata()->getBuffSubType() ) {
        case 1:  //1、属性加值
        case 2:  //2、属性减值
        case 62: //62、特殊属性减值
        {
            this->getCalculator()->addCurrStdAtr(bufferData->getOutputType(), -bufferData->getBuffValue());
        }
            break;
        case 3:  //3、加速
        case 4:  //4、减速
        {
            for (int i = 0; i < m_combatUnits->size(); i++) {
                LQCombatSoldier* tempUnit = m_combatUnits->at(i);
                tempUnit->getArmdata()->setSpeedBuffAdd(0);
            }
            this->getHeroData()->setSpeedBuffAdd(0);
            if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
                CCLOG("Buff SpeedBuffAdd delete");
        }
            break;
        case 5:  //5、间隔一定时间加值
        case 6:  //6、间隔一定时间减值
        case 66: //66、特殊间隔一定时间减值
        case 15: //15、间隔一定时间加变动值
        case 16: //16、间隔一定时间减变动值
        case 56: //56、姚果加的特殊DOT，不伤自己
        {
            //暂定为不是伤血的DOT，结束时复位
            if(bufferData->getOutputType()!=ATTR_HP && bufferData->getOutputType()!=ATTR_RAGE)
                this->getCalculator()->addCurrStdAtr(bufferData->getOutputType(), -bufferData->getBuffValue());
        }
            break;
        case 7:  //7、禁止移动
        {
            for (int i = 0; i < m_combatUnits->size(); i++) {
                LQCombatSoldier* tempUnit = m_combatUnits->at(i);
                tempUnit->stopMove(false);
            }
            this->getHero()->stopMove(false);
        }
            break;
        case 8:  //8、禁止暴击
        {
            this->getSquadData()->stopCrit(false);
        }
            break;
        case 9:  //9、禁止技能
        {
            this->getHero()->stopSkill(false);
        }
            break;
        case 10:  //10、无视防御
        {
            
        }
            break;
        case 11:  //11、无视攻击
        {
            
        }
            break;
        case 12:  //12、禁锢
        {
            for (int i = 0; i < m_combatUnits->size(); i++) {
                LQCombatSoldier* tempUnit = m_combatUnits->at(i);
                tempUnit->durance(false);
            }
            this->getHero()->durance(false);
        }
            break;
        case 13:  //13、护盾
        {
            this->getCalculator()->setCurrStdAtr(ATTR_SHIELD,0);
            this->getCalculator()->setCurrStdAtr(ATTR_SHIELD+ATTR_ADDVALUE,0);
            _hdbufferData = NULL;
            this->getBannerman()->getArmFlags()->updateValue();
        }
            break;
        case 14:  //14、白刃
        {
            this->getCalculator()->addCurrStdAtr(bufferData->getOutputType(), -bufferData->getBuffValue());
            this->getCalculator()->setCurrStdAtr(ATTR_ATKDIST,0);
            this->setTarget(NULL);
        }
            break;
        case 17:  //17、嘲讽
        {
            this->taunt(NULL);
        }
            break;
    }
    //清除变化的刀光特效
    if (bufferData->getFlashID().length()>0){
        for (int i = 0; i < m_combatUnits->size(); i++) {
            LQCombatSoldier* tempUnit = m_combatUnits->at(i);
            tempUnit->setBuffFlashID("");
        }
        m_bannerman->setBuffFlashID("");
    }
    //清除BUF放缩
    if (bufferData->getScalefactor()>0){
        bool isClearScale = true;
        for (int i  = 0; i <((LQAttackCalculator*)this->getCalculator())->getBuffers()->size(); i++) {
            LQBuffUserData* tempdata = ((LQAttackCalculator*)this->getCalculator())->getBuffers()->at(i);
            if (bufferData!=tempdata && tempdata->getScalefactor()!=1){
                isClearScale = false;
                break;
            }
        }
        if (isClearScale) {
           //CCLOG("%d 清除BUF放缩: Scalefactor:%d", m_index, 1);
           m_bannerman->setScale(1);
        }
    }
    
    this->removeBuffer(bufferData);
}

//获取假死的unit  判定条件： 该军团内被索敌个数最多的unit
LQCombatSoldier* LQCombatSquad::getDieUnit()
{
    LQCombatSoldier* unit = NULL;
    int count = 0;
    for (int i = 0; i < m_combatUnits->size(); i++) {
        LQCombatSoldier* tempUnit = m_combatUnits->at(i);
        if (!tempUnit->getActive() || tempUnit->getArmdata()->getCurrHpValue()<=0 ) {
            continue;
        }
        if ((unit==NULL || tempUnit->getLockedCount() > count) && tempUnit != getHero()) {
            unit = tempUnit;
            count = tempUnit->getLockedCount();
        }
    }
    
    return unit;

}

void LQCombatSquad::update(float dt, Layer *layer)
{
    AIObj::update(dt, layer);
 
    if(m_showEffectDuration>=0){
        m_showEffectDuration += dt;
        if (m_showEffectDuration>SQUAD_ASSAULT_RUNS)
            m_showEffectDuration = -1;  //枪击效果持续时间
    }
    
    //冲锋特效
    if((m_state==FUSM_SQSTATE_RUN || m_state==FUSM_SQSTATE_APPROACH)
       && (m_armsType==Combat_ArmsType_Melee || m_armsType==Combat_ArmsType_Cavalry)
       && m_bannerman){
        int m_currSpeed = m_bannerman->getCurrSpeedValue()*1000;
        if (m_currSpeed<SQUAD_MIN_RUNSPEED)  //太慢了就取消冲击波
            this->showForwardEffect(Combat_ShowEffect_None);
        else{
            if (m_armsType==Combat_ArmsType_Melee){
                if (m_state==FUSM_SQSTATE_RUN)  //4.9号修改，YAO需求
                    this->showForwardEffect(Combat_ShowEffect_Assault);
                //else
                //    CCLOG("YAO INFO:::游戏进行中步兵不应再冲锋了.......");
            }
            else if (m_armsType==Combat_ArmsType_Cavalry){
                //1.骑兵在以1000或更高的移动速度行进cavcharge距离后，会进入冲锋状态，部队前方出现突击波。
                //2.骑兵移动速度等于或低于1000时，无法进入冲锋状态。
                //3.注意cavcharge是一段固定距离，因此骑兵1000速度/1500速度，行进到cavcharge距离所需的时间是不同的。也就是高速移动的骑兵（通常由加速技能造成）会更快地进入突击状态。
                int Runs = 0;  //根据速度减少步数，简化处理
                if (m_currSpeed>=1500) Runs = 2;
                else if (m_currSpeed>=1360) Runs = 1;
                if (m_currSpeed>=1000 && m_bannerman->getMoveMileage()>SQUAD_FORWARD_RUNS-Runs){
                    //CCLOG("showForwardEffect Forward Speed:%d; %d > 4-%d", m_currSpeed, m_bannerman->getMoveMileage(), Runs);
                    this->showForwardEffect(Combat_ShowEffect_Forward);
                }
            }
        }
    }
    
    //怕有人调用getPosition()
    if(m_bannerman)
        _position = m_bannerman->getPosition();
    
    //（远程）军团当被攻击时间超过0.7时切换目标
    if (this->getTarget() && this->getAttackSquadMelee()){
        vector<LQCombatSquad*>::iterator it;
        LQCombatSquad* _atSquad;
        for(it=m_attackSquads->begin ();it!=m_attackSquads->end();it++)
        {
            _atSquad = (*it);
            if(_atSquad && _atSquad->getActive() && _atSquad->getAttackSquadTime()>0.55f){
                if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE)
                    CCLOG(" %s 切换目标 %s->%s", this->getName().c_str(), m_target->getName().c_str(), _atSquad->getName().c_str());
                this->setTarget(_atSquad);
                if (this->getHero())
                    this->getHero()->setTarget(_atSquad->getHero());
            }
        }
    }
    
    //用于处理军团攻击频率
    if ( m_isAttacking ) {  //getState() == FUSM_SQSTATE_ATTACK setSpeedBuffAdd
        m_attackSquadTime += dt;
        m_attackCdTime += dt;
    }else{
        m_attackCdTime = 0;
        m_isAttacking = false;
    }
    
    //冲锋触发第一次攻击
    if (m_attackCdTime >= m_attackFrequency && m_state!=FUSM_SQSTATE_WAIT) {
        if(_DEBUG_TYPE==_DEBUG_TYPE_VALUE)
            CCLOG("冲锋触发第一次攻击 %.2f %.2f", m_attackCdTime, m_attackFrequency);
        m_attackCdTime = 0;
        timeAttack();
    }
    
    //军团Buff刷新
    this->getCalculator()->update(dt);
    if (m_herodata)
        m_herodata->update(dt);
    
    //刷新军团的血值
    //if(!this->getActive())
    //    return;
    //m_armflagsBar->setPercentage( m_squadData->getHpValue()*100/m_HpValueMax );
}

//单元死了
void LQCombatSquad::dieUnit(LQCombatUnit* unit)
{
    if(!unit) return;
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL2 )//&& (unit->getIndex()==144 || unit->getIndex()==112)
        CCLOG("Squad::dieUnit: %d %d", unit->getIndex(), ((LQCombatHero*)unit)->isShowSkill());
    unit->setActive(false);
    //交换旗帜
    if(unit->getArmFlags()!=NULL)
    {
        if(m_combatUnits->size()>0){
            LQCombatSoldierVector::iterator it;
            for(it=m_combatUnits->begin ();it!=m_combatUnits->end();it++)
            {
                if (*it!=unit){
                    m_bannerman = (*it);
                    m_bannerman->setArmFlags(unit->getArmFlags());
                    unit->setArmFlags(NULL);
                    break;
                }
            }
        }
    }
    //死了，清除数据 TODO
    if(unit==m_hero){
        m_hero = NULL;
        
        /*if (m_combatUnits->size()>0){
            CCLOG("%s m_combatUnits is Error size %lu", unit->getName().c_str(), m_combatUnits->size());
            LQCombatSoldierVector::iterator it;
            for(it=m_combatUnits->begin ();it!=m_combatUnits->end();it++)
            {
                CCLOG("%s m_combatUnit %d", (*it)->getName().c_str(), (*it)->getHPValue());
                if((*it)->getHPValue()>0)
                    CCLOG("%s m_combatUnit Error", (*it)->getName().c_str());
            }
        }*/
    }else{
        LQCombatSoldierVector::iterator it;
        for(it=m_combatUnits->begin ();it!=m_combatUnits->end();it++)
        {
            if (*it==unit){
                m_combatUnits->erase(it);
                break;
            }
        }
    }
    if(m_squadData->getCurrHpValue()<=0){
        if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
            CCLOG("Squad[%d] 兵团完蛋了 %p", m_index, unit);
    }
    
    m_active = m_combatUnits->size()>0 || (m_hero && m_hero->getActive());
    //旗帜倒掉了,军团完蛋了
    if(!m_active && m_armflags){
        if (m_target && m_target->getActive() && m_squadData->getAttackType()==Combat_AttackType_Remote)
        {
            LQCombatSquad* squad = this->getTargetSquad();
            squad->getAttackSquads()->clear();
            //squad->setAttackSquadMelee(false,squad);
        }
        FlagPosition::getInstance()->removeNode(m_bannerman);
        m_bannerman->setArmFlags(NULL);
        FlagPosition::getInstance()->removeNode(m_armflags);
        m_armflags->runAction(Sequence::create(DelayTime::create(0.3f),
                                               FadeOut::create(0.4f),
                                               RemoveSelf::create(),NULL));
        m_armflags = NULL;
    }
}

// make next
void LQCombatSquad::makeNextAction()
{
    if(m_state==FUSM_SQSTATE_RUN)
        if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("Squad[%d]::FUSM_SQSTATE_RUN...", m_index);
    if(m_hero)
        m_hero->setSquadState(m_state);
    LQCombatSoldier* pUnit = NULL;
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        if (pUnit && pUnit->getActive())
            pUnit->setSquadState(m_state);
    }
    
    this->drawmArmflagsStatus();
}

//开始阵型移动
void LQCombatSquad::updateRunCommand() {
    if( m_state==FUSM_SQSTATE_RUN )
        return;
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("Squad[%s]::updateStartCommand....", m_name.c_str());
    m_state = FUSM_SQSTATE_RUN; //开始阵型移动
    makeNextAction();
    
    //阵型移动为1
    this->setChargeSpeed(1.0);
    //阵型移动音效
    this->doCombatMoveEffect();
}

//阵型移动音效
void LQCombatSquad::doCombatMoveEffect() {
    if(m_moveeffect)
        return;
    if(m_armsType==Combat_ArmsType_Cavalry)
        m_moveeffect = AudioManager::getInstance()->playEffect(Effect_MoveQB_Music1);
    else if(m_armsType==Combat_ArmsType_Melee || m_armsType==Combat_ArmsType_Remote)
        m_moveeffect = AudioManager::getInstance()->playEffect(Effect_MoveBB_Music2);
    else if(m_armsType==Combat_ArmsType_Machine)
        return;
}

//被嘲讽 使得嘲讽者target为我的目标
void LQCombatSquad::taunt(GameObj* taunter)
{
    if (taunter){
        LQCombatSquad* _taunter = getTargetSquad(taunter);
        if(taunter==m_taunter || _taunter==m_target)
            return;
        m_taunter = _taunter;
        //CCLOG("Squad[%s]::taunt[%d] %p", m_name.c_str(), taunter->getIndex(), taunter);
        GameObj::setTarget(m_taunter);
        this->drawmArmflags();
        if(m_hero) m_hero->setTarget(NULL);
        LQCombatUnit* pUnit = NULL;
        for (int i=0; i<m_combatUnits->size(); i++)
        {
            pUnit = m_combatUnits->at(i);
            pUnit->setTarget(NULL);
        }
        m_state = FUSM_SQSTATE_APPROACH;
    }else{ //取消嘲讽，去掉嘲讽者，但不做后续操作，自然转换目标
        m_taunter = NULL;
    }
}

//等待状态
void LQCombatSquad::updateWaitCommand() {
    if( m_state==FUSM_SQSTATE_WAIT )
        return;
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("Squad[%s]::updateWaitCommand....", m_name.c_str());
    m_state = FUSM_SQSTATE_WAIT; //等待
    m_attackCdTime = 0;
    m_isAttacking = false;
    makeNextAction();
}

//索敌状态
void LQCombatSquad::updateApprochCommand(LQCombatSquad* lockedSquad)
{
    if( m_state==FUSM_SQSTATE_APPROACH || lockedSquad==NULL || !lockedSquad->getActive())
        return;
    
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("Squad[%s]::updateApprochCommand.... %s", m_name.c_str(), lockedSquad->getName().c_str());
    this->setTarget(lockedSquad);
    
    m_state = FUSM_SQSTATE_APPROACH; //索敌，追踪敌人
    m_attackCdTime = 0;
    m_isAttacking = false;
    
    //索敌成功，集结队伍准备冲向敌人
    if(lockedSquad)
        updateArrayCommand(lockedSquad->getLQPosition());
    
    //索敌成功，加速了
    //this->setSquadSyncSpeed(1.2);
    
    //
    makeNextAction();
    
    //阵型移动音效
    this->doCombatMoveEffect();
}

// 追踪敌人
void LQCombatSquad::_updateMoveCommand()
{
    //CCLOG("Squad[%s]::updateMoveCommand....", m_name.c_str());
    
    //索敌成功，集结队伍准备冲向敌人
    //updateArrayCommand(dest);
    
    if( m_state==FUSM_SQSTATE_APPROACH)
        return;
    m_state = FUSM_SQSTATE_APPROACH; //索敌，追踪敌人
    m_attackCdTime = 0;
    m_isAttacking = false;
    
    makeNextAction();
    
    //阵型移动音效
    this->doCombatMoveEffect();
}

//去往目的地坐标
void LQCombatSquad::updateGotoCommand(Point dest)
{
    CCLOG("Squad[%s]::updateGotoCommand....", m_name.c_str());
    
    //设置目的地坐标
    this->setTargetPoint(dest);
    
    //索敌成功，集结队伍准备冲向敌人
    updateArrayCommand(dest);
    
    //索敌成功，加速了
    //this->setSquadSyncSpeed(1.2);
    
    if( m_state==FUSM_SQSTATE_RUN)
        return;
    m_state = FUSM_SQSTATE_RUN; //索敌，追踪敌人
    
    makeNextAction();
    
    //阵型移动音效
    this->doCombatMoveEffect();
}

//攻击状态
void LQCombatSquad::updateAttackCommand(LQCombatSquad* lockedSquad)
{
    //索敌
    if(lockedSquad){
        this->setTarget(lockedSquad);
    }

    if( m_state==FUSM_SQSTATE_ATTACK )
        return;
    
    m_attackSquadTime = 0;  //攻击开始计时
    
    //if(this->getHeroData()->getResIDC()==90010017)
    //    CCLOG("刘备开战啦.....");
    //攻击的军团还没有处于攻击状态，切换目标还击,存在问题：弓兵先攻击，所以要判断当前锁定的目标和攻击自己的哪个近
    LQCombatSquad* target= (LQCombatSquad*)this->getTarget();
    if(target){
        
        m_isAttackMelee = target && (this->getArmsType()!=2 || target->getArmsType()!=2);
                                     
        if (target->getState()!=FUSM_SQSTATE_ATTACK){
            if(target->getTarget()){  //切换攻击目标
                float dist1 = target->getDistance(this);
                float dist2 = target->getDistance(target->getTarget());
                if(dist1<dist2)
                    target->setTarget(this);
            }//else
            //target->setTarget(this);
        }else{
            if(target->getTarget()){  //切换攻击目标
                //对手记住攻击者，远程攻击者被非目标近战攻击时 target->getSquadData()->getAttackType()==Combat_AttackType_Remote &&
                if(this->getSquadData()->getAttackType()==Combat_AttackType_Melee && target->getTarget()!=this){
                    target->getAttackSquads()->push_back(this);
                }
            }
        }
    }
    
    //klqBattleStatusRun时起作用
    LQBattleSession::getInstance()->startBattle();
    
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("Squad[%s]::updateAatackCommand....", m_name.c_str());
    m_state = FUSM_SQSTATE_ATTACK;    //攻击
    
    //冲锋特效,清除
    if(m_moveeffect){
        AudioManager::getInstance()->stopEffect(m_moveeffect);
        m_moveeffect = 0;
    }
    
    //散开阵型
    //if (this->getTileDistance(target)>MIN_SQUAD_ATK){
        //this->trunFormation(target->getPosition());
    //}
    
    makeNextAction();
}

//集结 整理队型状态
void LQCombatSquad::updateArrayCommand(Point targetpt)
{
    //
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("Squad[%s]::updateArrayCommand....", m_name.c_str());
    this->trunRound(targetpt);
}

//显示自身位置的特效（刀光，闪避，格挡，加速，突击，迎击，乱射等）
void LQCombatSquad::showForwardEffect(LQShowEffectTypeEnum eftype)
{
    if(m_forwardEffect==eftype)
        return;
    
    //不到一定的速度不能出冲击波
    if(eftype==Combat_ShowEffect_Forward){
        //if(m_squad_x==klqCombatSquadRowBack)
        //    CCLOG("getRunSpeed:%.3f",this->getRunSpeed());
        if(this->getRunSpeed()*1000<SQUAD_MIN_RUNSPEED) //太慢了就取消冲击波
            return;
    }
    
    LQCombatSoldier* pUnit = NULL;
    for (int i=0; i<m_combatUnits->size(); i++)
    {
        pUnit = m_combatUnits->at(i);
        if (pUnit && pUnit->getActive())
            pUnit->showEffect(eftype);
    }
    if (m_hero && m_hero->getActive()){
        m_hero->showEffect(eftype);
        m_hero->setMoveMileage(0);
    }

    //突击 冲锋效果相遇后触发一次攻击
    if(m_forwardEffect!=Combat_ShowEffect_None && eftype==Combat_ShowEffect_Attack){
        EventCustom event(EVENT_FORWARD_EFFECT);
        event.setUserData(this);
        _eventDispatcher->dispatchEvent(&event);
        
        //突击 CCLOG("m_armsType: %d",m_armsType);
        //骑兵的一次冲击，将触发一次战斗暴击，（骑兵冲锋状态撞到枪头，则冲锋状态取消、不会触发突击，且骑兵自身受到一次“迎击”伤害。）
        if (m_target && m_target->getActive()){
            LQCombatSquad* squad = this->getTargetSquad();
            if (m_forwardEffect==Combat_ShowEffect_Forward){ //骑对什么都突，枪尖除外
                //骑兵出突击(暴击)和冲锋(特殊动作，但现在显示暴击)
                if(squad->getForwardEffect()!=Combat_ShowEffect_Assault)
                    m_squadData->setIsMustForward();
            }else if (m_forwardEffect==Combat_ShowEffect_Assault){
                //枪兵出枪击(暴击)和迎击(特殊动作)
                if((squad->getForwardEffect()==Combat_ShowEffect_Forward || squad->getOldForwardEffect()==Combat_ShowEffect_Forward)
                   && squad->getShowEffectDuration()<0)
                {
                    m_squadData->setIsMustForward();
                    squad->setForwardEffect(Combat_ShowEffect_None);
                }else
                    this->setForwardEffect(Combat_ShowEffect_None);
            }
            
            if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE || _DEBUG_TYPE==_DEBUG_TYPE_SKILL)
                CCLOG("ForwardEffect:%d, %d ; squad->getShowEffectDuration():%0.3f", this->getForwardEffect(), squad->getForwardEffect(), squad->getShowEffectDuration());
        }
        if (m_forwardEffect==Combat_ShowEffect_Assault)
            m_showEffectDuration = 0;  //第一次枪击开始
        this->timeAttack();
        eftype = Combat_ShowEffect_None;
    }
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE)
        CCLOG("set m_armsType: %d m_forwardEffect = %d", m_armsType, m_forwardEffect);
    m_oldforwardEffect = m_forwardEffect;
    m_forwardEffect = eftype;
    m_addSpeed = m_forwardEffect ? 1.5 : 1;  //突击加速
}

//设置军团速度变化系数 value 1.0 正常 2.0 索敌加速 0.0 攻击
void LQCombatSquad::setChargeSpeed(float value)
{
    m_chargeSpeed = value;
}

//变更怒气 最大不得超过最大怒气上限
void LQCombatSquad::changeHeroFuries(int value)
{
    m_hero->changeHeroFuries(value);
}

LQ_NAMESPACE_END
