//
// LQCombatUnit.cpp
// SuperWarriors
//
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#include "LQCombatUnit.h"
#include "LQUnitAIControl.h"
#include "LQBattleSession.h"

#include "arrow/LQArrow.h"
#include "Component/LQFlyNumber.h"
//#include "Component/LQSkillSprite.h"
#include "AudioManager.h"

#define BuffSkillSpriteTagNumber 40010000



LQ_NAMESPACE

//===================  LQCombatUnitShadow  ==========================
LQCombatUnitShadow::LQCombatUnitShadow(LQCombatUnit* combatUnit) : m_direction(klqUnitDirectionTypeNone), m_unitType(Combat_People_None), m_combatUnit(combatUnit), m_shadowSprite(NULL)
{
}

LQCombatUnitShadow::~LQCombatUnitShadow() {
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQCombatUnitShadow::~LQCombatUnitShadow %p", this);
}

void LQCombatUnitShadow::init(LQCombatUnitType unitType, LQUnitDirectionType direction) {
    m_unitType = unitType;
    m_direction= direction;
    
    createSprite();
}

void LQCombatUnitShadow::changeDirection(LQUnitDirectionType direction) {
    if (m_unitType != Combat_People_Cavalry && m_unitType != Combat_People_CavalryArc) {
        // 非骑兵
        return;
    }
    // 同方向
    if (m_direction == direction) {
        return;
    }
    
    m_combatUnit->getSprite()->removeChildByTag(SHADOW_TAG);
    createSprite();
}

void LQCombatUnitShadow::setScale(float scale)
{
    m_shadowSprite->setScale(scale);
}

Sprite* LQCombatUnitShadow::createSprite() {
    Sprite* sprite = m_combatUnit->getSprite();
    
    // 添加阴影
    
    string file;
    switch (m_unitType) {
        case Combat_People_Cavalry:
        case Combat_People_CavalryArc:
            // 骑兵特殊
            switch (m_direction) {
                case klqUnitDirectionTypeDown:
                case klqUnitDirectionTypeUp:
                    file = "Shadow_Cavalry_S.png";
                    break;
                case klqUnitDirectionTypeLeft:
                case klqUnitDirectionTypeRight:
                    file = "Shadow_Cavalry_E.png";
                    break;
                case klqUnitDirectionTypeLeftDown:
                case klqUnitDirectionTypeRightUp:
                    file = "Shadow_Cavalry_NE.png";
                    break;
                case klqUnitDirectionTypeLeftUp:
                case klqUnitDirectionTypeRightDown:
                    file = "Shadow_Cavalry_SE.png";
                    break;
                default:
                    break;
            }
            break;
            
        default:
            file = "Shadow.png";
            break;
    }
    Sprite* shadowSprite = Sprite::createWithSpriteFrameName(file.c_str());
    Point positionFactor = sprite->getAnchorPoint();
    shadowSprite->setPosition(Point(sprite->getContentSize().width * positionFactor.x, sprite->getContentSize().height * positionFactor.y));
    shadowSprite->setTag(SHADOW_TAG);
    
    sprite->addChild(shadowSprite,-1);
    m_shadowSprite = shadowSprite;
    return shadowSprite;
}

//===================  LQCombatUnitForward  ==========================
LQCombatUnitForward::LQCombatUnitForward(LQCombatUnit* combatUnit) : m_direction(klqUnitDirectionTypeNone), m_armsType(Combat_ArmsType_None), m_combatUnit(combatUnit), m_forwardSprite(NULL), m_forwardScale(1)
{
    
}

LQCombatUnitForward::~LQCombatUnitForward() {
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQCombatUnitForward::~LQCombatUnitForward %p", this);
}

void LQCombatUnitForward::init(LQArmsTypeEnum armsType, LQUnitDirectionType direction, float scale) {
    m_armsType = armsType;
    m_direction= direction;
    m_forwardScale = scale;
    createSprite();
}

void LQCombatUnitForward::changeDirection(LQUnitDirectionType direction) {
    if (m_armsType != Combat_ArmsType_Cavalry) {
        // 非骑兵
        return;
    }
    // 同方向
    if (m_direction == direction) {
        return;
    }
    m_direction= direction;
    m_combatUnit->getSprite()->removeChildByTag(FORWARD_TAG);
    createSprite();
}

void LQCombatUnitForward::setScale(float scale)
{
    if(m_forwardSprite){
        m_forwardScale = scale;
        m_forwardSprite->setScale(scale);
        this->play();
    }
}

void LQCombatUnitForward::setVisible(bool bVisible)
{
    if(!m_forwardSprite)
        return;
    if(bVisible){
        this->play();
    }else{
        m_forwardSprite->stopAllActions();
    }
    m_forwardSprite->setVisible(bVisible);
}

void LQCombatUnitForward::play()
{
    if(!m_forwardSprite)
        return;
    float dt = 0.5;
    ActionInterval *actScale = CCSequence::create(ScaleTo::create(dt, 1.1*m_forwardScale), ScaleTo::create(dt, 1.0*m_forwardScale), NULL);
    ActionInterval *actFade = CCSequence::create(FadeTo::create(dt, 180), FadeTo::create(dt, 255), NULL);
    ActionInterval *action1 = Spawn::create(actScale, actFade, NULL);
    m_forwardSprite->runAction(RepeatForever::create(action1));
}

Sprite* LQCombatUnitForward::createSprite()
{
    Sprite* sprite = m_combatUnit->getSprite();
    
    // 添加阴影
    string file;
    switch (m_armsType) {
        case Combat_ArmsType_Cavalry:
            // 骑兵特殊
            switch (m_direction) {
                case klqUnitDirectionTypeDown:
                case klqUnitDirectionTypeUp:
                    file = "forward/QB_N.png";
                    break;
                case klqUnitDirectionTypeLeft:
                case klqUnitDirectionTypeRight:
                    file = "forward/QB_W.png";
                    break;
                case klqUnitDirectionTypeLeftUp:
                case klqUnitDirectionTypeRightDown:
                case klqUnitDirectionTypeLeftDown:
                case klqUnitDirectionTypeRightUp:
                    file = "forward/QB_NW.png";
                    break;
                default:
                    break;
            }
            break;
        case Combat_ArmsType_Melee:
            // 步兵特殊
            switch (m_direction) {
                case klqUnitDirectionTypeDown:
                case klqUnitDirectionTypeUp:
                    file = "forward/BB_N.png";
                    break;
                case klqUnitDirectionTypeLeft:
                case klqUnitDirectionTypeRight:
                    file = "forward/BB_W.png";
                    break;
                case klqUnitDirectionTypeLeftUp:
                case klqUnitDirectionTypeRightDown:
                case klqUnitDirectionTypeLeftDown:
                case klqUnitDirectionTypeRightUp:
                    file = "forward/BB_NW.png";
                    break;
                default:
                    break;
            }
            break;

        default:
            CCLOG("ERROR: 非骑兵,步兵没有冲锋效果！");
            return NULL;
            break;
    }
    Sprite* forwardSprite = Sprite::createWithSpriteFrameName(file.c_str());
    Point posAnchor = sprite->getAnchorPoint();
    forwardSprite->setPosition(Point(sprite->getContentSize().width * posAnchor.x, sprite->getContentSize().height * posAnchor.y));
    forwardSprite->setTag(FORWARD_TAG);
    // 骑兵特殊
    switch (m_direction) {
        case klqUnitDirectionTypeDown:
            forwardSprite->setFlippedY(true);
            break;
        case klqUnitDirectionTypeRight:
            forwardSprite->setFlippedX(true);
            break;
        case klqUnitDirectionTypeRightUp:
            forwardSprite->setFlippedX(true);
            break;
        case klqUnitDirectionTypeLeftDown:
            forwardSprite->setFlippedY(true);
            break;
        case klqUnitDirectionTypeRightDown:
            forwardSprite->setFlippedX(true);
            forwardSprite->setFlippedY(true);
            break;
            break;
        default:
            break;
    }
    
    sprite->addChild(forwardSprite,-1);
    m_forwardSprite = forwardSprite;
    m_forwardSprite->setScale(m_forwardScale);
    this->play();
    return forwardSprite;
}

//===================  LQCombatUnit  ==========================
LQCombatUnit* LQCombatUnit::create(LQBaseUserData* data,LQGameObjType type,LQCombatSquadDirectionEnum direction)
{
    LQCombatUnit *pRet = new LQCombatUnit(data,type,direction);
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

LQCombatUnit::LQCombatUnit(LQBaseUserData* data,LQGameObjType type, LQCombatSquadDirectionEnum direction):AIObj(NULL), m_squadDirection(direction), m_sprite(NULL), m_squad(NULL), m_shadow(NULL), m_forward(NULL), m_armdata(NULL), m_patrolSpeed(0), m_squadState(FUSM_SQSTATE_WAIT), m_state(FUSM_STATE_WAIT), m_hero(NULL), _currentAnimation(NULL), m_lockedcount(0),m_lockedremotecount(0), m_cdTime(600), m_isStand(true), m_isBattle(false),m_coprseColorIndex(0), m_occupation(Combat_Occupation_None),m_armsType(Combat_ArmsType_None),m_arraySyncSpeed(1),m_arrayPoint(Point::ZERO),m_armflags(NULL),m_controlLayer(NULL), m_stopMove(false), m_flashID(""), m_buffflashID(""), m_isSkilling(false)
{
    _session = LQBattleSession::getInstance();
    m_data = data;
    m_armdata = (LQArmsUserData*)data;
    m_speed = m_armdata->getSpeed();
    m_patrolSpeed = m_armdata->getPatrolSpeed();
    m_bookTileCoord = Point::ZERO;
    m_oldphaseType = klqUnitAnimPhaseNone;
    //m_buff_body_tag = -1;
    //m_buff_foot_tag = -1;
    m_buff_ico = -1;
    m_isDeadhasSkill = false;
    m_type = type;
    string baseName = m_armdata->getBaseName();
    string soldierColor = "B";
    if(m_type != klqLQGameObjHero){
        soldierColor = m_squadDirection==klqCombatSquadDirectionLeft ? "B" : "R";
    }
    LQ::stringf(m_spriteBaseName, "%s_%s", soldierColor.c_str(), baseName.c_str());
    
    m_data->getCfgdata()->getValue("View", &m_view);        //作战单位视野（单位：格子）
    m_data->getCfgdata()->getValue("AttackType", &m_attackType);
    //优先攻击列表，格式：\r\n优先兵种类型1(int),优先兵种类型2(int),优先兵种类型3(int),\r\n留空表示采取距离优先原则。
    m_data->getCfgdata()->getValue("AttackSpeed", &m_attackSpeed);     //以毫秒为单位，兵种从发动攻击指令到攻击动作完成所需要的时间
    m_data->getCfgdata()->getValue("AttackGap", &m_attackGap);         //攻击方式攻击间隔时间，万分之一秒
    m_data->getCfgdata()->getValue("AttackDistMin", &m_attackDistMin); //攻击方式的有效范围
    m_data->getCfgdata()->getValue("AttackDistMax", &m_attackDistMax); //攻击方式的有效范围
    if(m_attackDistMax<=0 )
        m_attackDistMax = 1;
    //if(m_attackDistMax > 8 ) m_attackDistMax += 5;              //直接加5来扩充弓兵距离 TODO
    m_data->getCfgdata()->getValue("AIType", &m_aIType);        //决定兵种的AI类型
    m_currentAttackSpeed = m_attackSpeed/RATE_PERCENT;
    m_currentAttackGap = m_attackGap/RATE_PERCENT;
    
    //图片的重心坐标，格式为x;y
    string _centralCoordinate;
    m_armdata->getActionInfo()->getValue("CentralCoordinate", &_centralCoordinate);
    m_centralCoordinate = PointFromString(_centralCoordinate);  //Point(0.52,0.36);
    m_armdata->getActionInfo()->getValue("ImgWidth", &m_imgwidth);    //图片的宽度
    m_armdata->getActionInfo()->getValue("ImgHeight", &m_imgheight);  //图片的高度
    m_armdata->getActionInfo()->getValue("StandBy", &m_standBy);      //待命动作序列
    m_armdata->getActionInfo()->getValue("Move", &m_move);            //移动动作序列
    m_armdata->getActionInfo()->getValue("Death", &m_death);          //死亡动作序列
    m_armdata->getActionInfo()->getValue("MissileType", &m_missileType); //箭矢类型：\r\n1、弓箭\r\n2、投石\r\n3、标枪\r\n纯近战兵种，填-1
    m_armdata->getActionInfo()->getValue("MissileRes", &m_missileRes); //箭矢应用的资源文件，与模型资源绑定
    m_missileFrameCount = 0;  //箭矢资源数量
    std::vector<std::string> strs = split(m_missileRes.c_str(), ',');
    if(strs.size()>1){
        m_missileRes = strs[0].c_str();
        m_missileFrameCount = atoi(strs[1].c_str());
    }
    
    m_armdata->getActionInfo()->getValue("HitEffect", &m_hitEffect);   //命中爆点特效
    m_missileResPostion = Point(1,1);
    string _missileResPostion;
    m_armdata->getActionInfo()->getValue("MissileResPostion", &_missileResPostion);
    if(_missileResPostion.length()>0)
        m_missileResPostion = PointFromString(_missileResPostion);
    m_controller = new LQUnitAIControl(this);
    m_controller->init();
    m_state = FUSM_STATE_WAIT;
    
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQCombatUnit::LQCombatUnit %p index=%d %s", this, m_index, m_spriteBaseName.c_str());
}

LQCombatUnit::~LQCombatUnit()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQCombatUnit::~LQCombatUnit %p", this);
    
    if (m_sprite) {
        m_sprite->removeFromParentAndCleanup(true);
    }
    
    CC_SAFE_RELEASE(m_sprite);
    CC_SAFE_DELETE(m_shadow);
    CC_SAFE_DELETE(m_forward);
    
    CC_SAFE_DELETE(m_controller);
}

bool LQCombatUnit::init()
{
    if ( !AIObj::init() )
    {
        return false;
    }
    initData();
    return true;
}

#pragma mark - initData
void LQCombatUnit::initData()
{
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    m_haloSprite = NULL;
    
    // 根据双方阵营来初始化动画方向
    LQUnitDirectionType direction = getDefaultDirection();
    this->setDirection(direction);
    m_phaseType = klqUnitAnimPhaseStand;
    
    m_unitType = (LQCombatUnitType)m_armdata->getUnitType();
    
    // add your codes here...
    string frameName = this->getSpriteFrameName(getSpriteBaseName(), klqUnitAnimPhaseStand, direction, 0);
    auto sprite = Sprite::createWithSpriteFrameName(frameName.c_str());
    sprite->setPosition(Point(this->getContentSize().width/2,this->getContentSize().height/2));
    setSprite(sprite);
    this->addChild(sprite,0);
    //铆钉位置
    m_sprite->setAnchorPoint(m_centralCoordinate);
    this->setContentSize(sprite->getContentSize());
    // 挂点计算
    m_TAnchorPoint = Point(m_imgwidth*0.5,m_imgheight*1.1);	//头上的挂点
    m_LAnchorPoint = Point(m_imgwidth*0.5,m_imgheight*0.5);	//左上的挂点
    //m_RAnchorPoint = ;	//右上的挂点
    //m_DAnchorPoint = ;	//脚上的挂点
    
    // 添加阴影
    m_shadow = new LQCombatUnitShadow(this);
    m_shadow->init(m_unitType, direction);
    if(m_type==klqLQGameObjHero){
        m_shadow->setScale(2.0);
        
    }
    // 添加冲锋
    m_forward = new LQCombatUnitForward(this);
    
    Animation *animation = Animation::create();
    //const vector<int>& animFrameList = getArmyStat()->getMoveFrameList();
    for(int i = 0; i < 4; ++i) {
        frameName = this->getSpriteFrameName(getSpriteBaseName(), klqUnitAnimPhaseStand, direction, i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str());
        if(frame)
            animation->addSpriteFrame(frame);
    }
    // delayPerUnit means the delay to add to each frame (exactly the same as the old delay)
    animation->setDelayPerUnit(AnimationFPS*2.0);
    Action* action = RepeatForever::create(Animate::create(animation));
    action->setTag(SPRITE_SPEDD_ACTION_TAG);
    m_sprite->runAction(action);
    
    if(direction==klqUnitDirectionTypeRight ||
       direction==klqUnitDirectionTypeRightUp ||
       direction==klqUnitDirectionTypeRightDown)
        m_sprite->setFlippedX(true);
    
    //LQArmsUserData* udata = new LQArmsUserData();
    //udata->initWithIDC(90001001);
    
    //testHeadLabel();
}

//
bool LQCombatUnit::isDied()
{
    if (this->getHPValue()<=0)
        this->setActive(false);
        //_updateDieCommand();
    
    return this->getHPValue()<=0;
}

//数据获取
int LQCombatUnit::getHPValue()
{
    return m_armdata->getCurrHpValue();
}

//设置所属的军团
void LQCombatUnit::setSquad(LQCombatSquad* squad)
{
    if(m_squad == squad)
        return;
    m_squad = squad;
    ((LQUnitAIControl*)m_controller)->setSquad(squad);
}

//设置所属的军团状态
void LQCombatUnit::setSquadState(LQSquadStateEnum squadState)
{
    if(m_squadState == squadState)
        return;
    m_squadState = squadState;
    if(m_squadState==FUSM_SQSTATE_APPROACH){
        //this->_updateFollowCommand();
    }else if(m_squadState==FUSM_SQSTATE_RUN){
        //this->_updateFollowCommand();
    }
}

LQUnitDirectionType LQCombatUnit::getDefaultDirection()
{
    if (m_squadDirection==klqCombatSquadDirectionRight)
        return klqUnitDirectionTypeLeftDown;
    return klqUnitDirectionTypeRightUp;
}

//当前速度
float LQCombatUnit::getCurrSpeedValue()
{
    return m_armdata->getCurrSpeedValue();
}

//根据格子坐标设置位置设置, 校验吗
void LQCombatUnit::setTileCoord(float tx,float ty)
{
    Point tilept = Point(tx, ty);
    if (m_tileCoord.equals(tilept))
        return;
    
    m_moveMileage++;
    _session->recordTileCoord(tilept,this);
    m_tileCoord = tilept;
    //if(m_index==16)
    //    CCLOG("setTileCoord: %s %s", PointString(convertPoint(m_tileCoord)).c_str(), TileCoordString(m_tileCoord).c_str() );
}

void LQCombatUnit::setTileCoordandPoint(float tx,float ty)
{
    this->setTileCoord(tx,ty);
    this->setPosition(convertPoint(tx,ty));
}

void LQCombatUnit::setUnitXY(float x,float y)
{
    unit_x = x;
    unit_y = y;
    stringf(m_name, "%s%.0f.%.0f", m_spriteBaseName.c_str(), unit_x, unit_y);
    //CCLOG("setUnitXY %s", m_name.c_str());
}

//静止移动
void LQCombatUnit::stopMove(bool value)
{
    if(value){
        m_oldphaseType = m_phaseType;
        m_phaseType = klqUnitAnimPhaseStand;
        this->changeAnimation();
        m_stopMove = value;
    }else{
        m_phaseType = m_oldphaseType;
        m_stopMove = value;
        this->changeAnimation();
    }
}

//禁锢
void LQCombatUnit::durance(bool value)
{
    m_stopMove = value;
    if(value)
        m_sprite->stopAllActions();
    else
        this->changeAnimation();
}


// 我要站在这里，看看能不能
bool LQCombatUnit::isCanStand()
{
    //现在近战不管，远程处理TODO
    if(m_attackType==Combat_AttackType_Melee)
        return true;
    if(m_type==klqLQGameObjHero)   //临时处理，英雄的位置重了
        return true;
    return _session->checkStandTileCoord(m_tileCoord,this, m_attackType==Combat_AttackType_Melee?1:0);
}

// 嗨，我要站住  isattack:(true 攻击站住，false 等待站住）
bool LQCombatUnit::doStand(bool isattack)
{
    if(!isCanStand())  //格子不满足条件
    {
        //开始攻击目标，要站着
        if(isattack && m_target && m_target->getActive())
        {
            // 要去近战，目标却被包围了，换目标吧
            if (m_attackType==Combat_AttackType_Melee && _session->checkSurroundTileCoord(m_target->getTileCoord()))
            {
                //CCLOG("要去近战，目标却被包围了，换目标吧  %p %d -> %p", this, m_index, this->getTarget());
                this->setTarget(NULL);
            };
            moveNextStandPostion();
        }
        //等待停留此处不让，随便换个地方试一试
        else{
            moveNextStandPostion();
        }
        return false;
    }
    
    //this->setIsStand(true);
    return true;
}

// 更改位置坐标，其中校验是否可以进入
void LQCombatUnit::movebyPostion(const Point& move)
{
    if (move.equals(Point(0, 0)))  //位置没有变化
        return;
    if (move.getDistance(Point(0, 0))>200)
        CCLOG("Distance is ERROR %s", PointString(move).c_str());
    Point position = getPosition()+move;
    Point tilept = convertTileCoord( position );
    
    if ( !m_tileCoord.equals(tilept)){
        //TODO 位置控制
        /*if( !_session->checkPassTileCoord(tilept)
           && m_bookTileCoord==Point::ZERO && !this->hasWayPoint() && m_squadState!=FUSM_SQSTATE_RUN )
        {  //不能进入
            moveNextPassPostion(m_tileCoord);
            return;
        }*/
        
        setTileCoord(tilept.x,tilept.y);

        //if(m_type == klqLQGameObjHero || m_index==96) //m_index==0 || m_index==16
        //CCLOG("%d movePostion %s %s", m_index, TileCoordString(m_tileCoord).c_str(), TileCoordString(position).c_str() );
    }
    
    //旗手移动，旗帜也跟着移动
    if (this->isBannerman()){
        //Point _pp = _position-position;
        //m_armflags->setPosition(m_armflags->getPosition()-_pp);
        
        //Point pt = FlagPosition::getInstance()->drawFlag(_position, m_squadDirection ==klqCombatSquadDirectionLeft, m_armflags, m_squadState==FUSM_SQSTATE_ATTACK);
        
        int _x = m_squadDirection ==klqCombatSquadDirectionLeft?-1:1;
        Point pt = Point(this->getPositionX()+m_imgwidth*_x,this->getPositionY()+m_imgheight*1.1+m_armflags->getContentSize().height/2);
        //新加的检查重叠的逻辑
        Point pt2 = FlagPosition::getInstance()->drawFlag(pt, m_squadDirection ==klqCombatSquadDirectionLeft, m_armflags, m_squadState==FUSM_SQSTATE_ATTACK && m_isStand);  //
        //if (m_index == 128 || m_index == 112)
        //    CCLOG("%d drawFlag [%s]:%s %s", m_index, PointString(this->getPosition()).c_str(), PointString(pt).c_str(), PointString(pt2).c_str());
        if( pt2.x!=-2000 && pt2.y!=-2000 )
            m_armflags->setPosition(pt2);
    }
    this->setPosition(position);
    this->setLocalZOrder(1600-position.y);
    
    if(_DEBUG_TYPE==_DEBUG_TYPE_TIME && m_index==0)
        CCLOG( "LQCombatUnit::movebyPostion %s  at %ld", PointString(position).c_str(), getCurrentMillSecond() );
    
    if(!m_isBattle){        //第一次进入战场，置标志
        if(m_tileCoord.x>0 && m_tileCoord.x<MAP_COL && m_tileCoord.y>0 && m_tileCoord.y<MAP_ROW)
            m_isBattle = true;
    }
    
    if(m_haloSprite){
        m_haloSprite->setPosition(position);
        m_haloSprite->setLocalZOrder(1600-position.y-1);
    }
    
    //if(m_index == 138)
    //CCLOG("IDX%d move %d %s: %s %s",  m_index, m_phaseType, _currentHeadFrameName.c_str(), PointString(_position).c_str(), TileCoordString(m_tileCoord).c_str() );
}

// 移动到下一个可以通过的位置，扩展成寻多步
//    现在点位置 tileCoord
void LQCombatUnit::moveNextPassPostion(Point tileCoord)
{
    //if(m_index==56)
        //CCLOG("moveNextPassPostion...%s %s", m_name.c_str(), TileCoordString(tileCoord).c_str() );
    int index = 0;
    Point move;
    if(m_target)
    {
        Point td = m_target->getCurrentTileCoord()-tileCoord; //这里是原向的方向
        index = calcDirectionFromTileCoord(td);
    }
    for (int i = 0; i < DIRECTION_SIZE; i++) {
        if (index >= DIRECTION_SIZE  )
            index = 0;
        index++;
        move = getTileCoordOffsetFromDirection(UnitDirection[index]);
        if (move.equals(Point::ZERO)) continue;
        if( _session->checkPassTileCoord(tileCoord+move) )  //可以进入
        {
            this->addWayPoint(tileCoord+move);
            //CCLOG("IDX:%d moveNextPassPostion... %s", m_index, TileCoordString(tileCoord+move).c_str());
            if (m_wayPoints.size()>=2)      //控制最大次数
                break;
            moveNextPassPostion(tileCoord+move); //再来一步
            return;
        }
    }
}

// 移动到下一个空位置
void LQCombatUnit::moveNextStandPostion()
{
    //if(m_index==19)
        //CCLOG("moveNextStandPostion... %s %s", m_name.c_str(), TileCoordString(m_tileCoord).c_str());
    int index = 0;
    Point move;
    if(m_target &&  m_attackType==Combat_AttackType_Melee)
    {
        Point targetTileCoord = m_target->getCurrentTileCoord();  //这里是反向的方向
        index = calcDirectionFromTileCoord(m_tileCoord - targetTileCoord);
        for (int i = 0; i < DIRECTION_SIZE; i++) {
            if (index >= DIRECTION_SIZE  )
                index = 0;
            index++;
            move = getTileCoordOffsetFromDirection(UnitDirection[index]);
            if (move.equals(Point::ZERO)) continue;
            if( _session->checkStandTileCoord(targetTileCoord+move,this,m_attackType==Combat_AttackType_Melee?1:0) )  //敌人的哪个位置我可以进入
            {
                this->addWayPoint(targetTileCoord+move);
                m_bookTileCoord = targetTileCoord+move;
                _session->bookTileCoord(this,targetTileCoord+move);
                //CCLOG("IDX:%d 订了位置%s要去: 目标位置%s", m_index, TileCoordString(targetTileCoord+move).c_str(), TileCoordString(targetTileCoord).c_str());
                break;
            }
            
        }
    }
}

void LQCombatUnit::onLostTarget()
{
    if(m_state==FUSM_STATE_ATTACK)
    {
        //this->setIsStand(true);
        //this->setState( FUSM_STATE_WAIT ); //目标已经死亡了
        //m_phaseType = klqUnitAnimPhaseStand;
        //changeAnimation();
    }
}

//---------------------------------------------------------
bool LQCombatUnit::isColliding(GameObj *obj)
{
	return (this->getBoundingBox().intersectsRect(obj->getBoundingBox()));
}
//转交旗帜
void LQCombatUnit::setArmFlags(LQFlagsSprite* fsp)
{
    m_armflags = fsp;
    
    if(m_armflags){
        //Point pt = FlagPosition::getInstance()->drawFlag(_position, m_squadDirection ==klqCombatSquadDirectionLeft, m_armflags, m_squadState==FUSM_SQSTATE_ATTACK);
        int _x = m_squadDirection ==klqCombatSquadDirectionLeft?-1:1;
        Point pt = Point(this->getPositionX()+_x*m_imgwidth,this->getPositionY()+m_imgheight*1.1+m_armflags->getContentSize().height/2);
        if( pt.x!=-2000 && pt.y!=-2000 )
            m_armflags->setPosition(pt);
    }
}

//测试显示
void LQCombatUnit::testHeadLabel()
{
    //if(m_type==klqLQGameObjSoldier)
    return;
    Label* ttf0 = (Label*)m_sprite->getChildByTag(10000);
    string str;
    stringf(str, "%s %d->%d", "", m_index, m_targetIndex);
    if(!ttf0){
        ttf0 = Label::createWithSystemFont(str.c_str(), "Helvetica", 12);
        ttf0->setColor(Color3B::BLACK);
        float yy = (m_type==klqLQGameObjSoldier) ? 0.5 :0.7;
        ttf0->setPosition(Point(m_sprite->getContentSize().width/2,m_sprite->getContentSize().height*yy));
        ttf0->setTag(10000);
        m_sprite->addChild(ttf0, 101);
    }else{
        if(m_state==FUSM_STATE_ATTACK)
            ttf0->setColor(Color3B::BLUE);
        else if(m_state==FUSM_STATE_APPROACH)
            ttf0->setColor(Color3B::BLUE);
        //ttf0->setColor(Color3B::RED);
        ttf0->setString(str.c_str());
    }
}

#pragma mark -- 状态变更
void LQCombatUnit::setState(LQUnitStateEnum state)
{
    if(m_state==state)
        return;
    if(m_index==45){
        //CCLOG("");
    }
        
    _LQDEBUG("setState %d->%d", m_state, state);
    this->onExitState(state);
    m_state = state;
    this->onEnterState();
    
    LQUnitStateEnum curstate = FUSM_STATE_NONE;
    if( m_controller ){
        curstate =  (LQUnitStateEnum)((FSMMachine*)m_controller->getMachine())->getCurrentState()->getType();
        _LQDEBUG("state Machine:%d state:%d", curstate, m_state);
        if(curstate!=m_state )
            CCLOG("%p setState Unit[%d] state Error Machine:%d  state:%d", this, m_index, curstate, m_state);
    }
}

void LQCombatUnit::onEnterState()
{
    //烟尘  --只有当是阵列行军过程中才需要产生烟尘
    LQCombatSoldier* soldier = (LQCombatSoldier*)this;
    if (!soldier) {
        return;
    }
    if (soldier->getState() == FUSM_STATE_FOLLOW ||
        (soldier->getType() == klqLQGameObjHero && soldier->getState() == FUSM_STATE_APPROACH))
    {
        //LQZoneUserData* zoneData = new LQZoneUserData();
        //zoneData->initWithIDC(LQBattleSession::getInstance()->getZoneID());
        if (soldier->getActive()&&LQBattleSession::getInstance()->getSmokeEffect() > 0) {
            soldier->showSmoke(LQBattleSession::getInstance()->getSmokeEffect(), soldier->getPosition());
        }
        //CC_SAFE_DELETE(zoneData);
    }
}

void LQCombatUnit::onExitState(LQUnitStateEnum state)
{
    LQCombatSoldier* soldier = (LQCombatSoldier*)this;
    if (!soldier) {
        return;
    }
    
    if (state != FUSM_STATE_FOLLOW || (soldier->getType() == klqLQGameObjHero && soldier->getState() != FUSM_STATE_APPROACH)) {
        LQBuffSprite* smokeSp = (LQBuffSprite*)soldier->getChildByTag(SmokeSpriteTagNumber);
        if (smokeSp) {
            smokeSp->setVisible(false);
        }
    }

}

//设置站立状态
void LQCombatUnit::setIsStand(bool value)
{
    if (value==m_isStand)
        return;
    m_isStand = value;
    if (m_squad && isBannerman() && m_squad->getIsRallied()){  //不在集结过程中
        m_squad->setIsStand(value);
    }
    if(_DEBUG_TYPE==_DEBUG_TYPE_TIME && m_index==0)
        CCLOG( "LQCombatUnit::setIsStand %d  at %ld", m_isStand, getCurrentMillSecond() );
}

//等待
void LQCombatUnit::_updateWaitCommand()
{
    if( m_state==FUSM_STATE_WAIT)
        return;
    
    //if(!this->doStand(false))  //站住了才能攻击
    //    return;
    m_phaseType = klqUnitAnimPhaseStand;
    this->setIsStand(true);
    _LQDEBUG("Unit[%s]::updateWaitCommand....",m_name.c_str());
    this->setState(FUSM_STATE_WAIT); //等待
    changeAnimation();
}

//攻击
void LQCombatUnit::_updateAttackCommand()
{
    if( m_state==FUSM_STATE_ATTACK || !m_target || !m_target->getActive())
        return;
    /*
    if (this->hasWayPoint())
    {
        Point wpt = m_wayPoints.back();
        //CCLOG("IDX:%d 还没有到近战目的地%s，继续前进 %s %s", m_index,  TileCoordString(wpt).c_str(),TileCoordString(m_tileCoord).c_str() , TileCoordString(_position).c_str());
        //return;
    }*/
    
    //想停留在此处攻击不让，按顺序去逐一试对象身边的空位，如果空位被其他好事者占了怎么办？
    if(!isCanStand()){
        //CCLOG("IDX:%d 近战不能站住%s 目标%d在：%s", m_index, TileCoordString(m_tileCoord).c_str(), m_target->getIndex(), TileCoordString(m_target->getTileCoord()).c_str() );
        
        // 要去近战，目标却被包围了，换目标吧
        if (m_target && m_attackType==Combat_AttackType_Melee
            && _session->checkSurroundTileCoord(m_target->getTileCoord()) )
        {
            //CCLOG("要去近战，目标却被包围了，换目标吧  %p %d -> %p", this, m_index, this->getTarget());
            this->setTarget(NULL);
            testHeadLabel();
            return;
        };
        
        moveNextStandPostion();
        
        return;
    }
    
    //冲锋特效,清除
    if(m_type == klqLQGameObjHero){
        m_squad->showForwardEffect(Combat_ShowEffect_Attack);
    }
    
    m_moveMileage = 0;  //每次攻击的开始将冲击移动判断距离清零
    m_phaseType = klqUnitAnimPhaseAttack;
    if (m_isSkilling)
        m_phaseType = klqUnitAnimPhaseSkillAttack;
    //去掉了曾经登记的位置
    _session->unbookTileCoord(this);
    m_bookTileCoord = Point::ZERO;
    _LQDEBUG(_DEBUG_TYPE_MOVE, "Unit::updateAttackCommand.... --> %d 位置：%s", m_targetIndex, TileCoordString(m_tileCoord).c_str());
    this->setIsStand(true);
    this->setState( FUSM_STATE_ATTACK ); //攻击
    changeDirection(m_target);
    changeAnimation();
    
}

//索敌状态，移动到目的地
void LQCombatUnit::_updateMoveCommand(Point dest)
{
    if(dest!=Point::ZERO)
        this->setTargetPoint(dest);
    
    if( m_state==FUSM_STATE_APPROACH)
        return;
    
    this->setIsStand(false);
    m_phaseType = klqUnitAnimPhaseMove;
    _LQDEBUG(_DEBUG_TYPE_MOVE, "Unit[%s]::_updateMoveCommand....",m_name.c_str());
    this->setState( FUSM_STATE_APPROACH ); //索敌，移动到目的地
    changeAnimation();
}

//索敌，追踪敌人
void LQCombatUnit::_updateApprochCommand(GameObj* lockedunit)
{
    if(lockedunit!=this->getTarget())
        this->setTarget(lockedunit);
    if(lockedunit)
        this->setIsStand(false);
    if( m_state==FUSM_STATE_APPROACH || !lockedunit || !lockedunit->getActive())
        return;
    
    this->setIsStand(false);
    m_phaseType = klqUnitAnimPhaseMove;
    _LQDEBUG("Unit[%s]::updateApprochCommand....", m_name.c_str());
    this->setState( FUSM_STATE_APPROACH ); //索敌，追踪敌人
    changeAnimation();
}

//逃跑或者后撤
void LQCombatUnit::_updateEvadeCommand()
{
    if( m_state==FUSM_STATE_EVADE )
        return;
    this->setIsStand(false);
    m_phaseType = klqUnitAnimPhaseMove;
    //CCLOG("Unit[%s]::updateEvadeCommand....",m_name.c_str());
    this->setState( FUSM_STATE_EVADE ); //逃跑或者后撤
    changeAnimation();
}

//跟随
void LQCombatUnit::_updateFollowCommand()
{
    if( m_state==FUSM_STATE_COUNT || m_index<0 ) return;  //TEST
    
    this->setIsStand(false);
    if( this->getHero()==NULL) //武将索敌追踪
    {
        this->setState( FUSM_STATE_APPROACH );
        m_phaseType = klqUnitAnimPhaseMove;
        changeAnimation();
        return;
    }
    
    this->setState( FUSM_STATE_FOLLOW ); //跟随武将
    if(m_index==_DEBUG_INDEX)
        CCLOG("2 _updateFollowCommand: setDirection");
    this->setDirection(this->getBannerman()->getDirection());
    m_phaseType = this->getBannerman()->getPhaseType();
    if (m_phaseType==klqUnitAnimPhaseSkillAttack && m_type!=klqLQGameObjHero)
        m_phaseType = klqUnitAnimPhaseAttack;
    changeAnimation();
}

//死亡
void LQCombatUnit::_updateDieCommand()
{
    if( m_state==FUSM_STATE_DIE )
        return;
    
    m_stopMove = false;
    m_phaseType = klqUnitAnimPhaseFallDown;
    _LQDEBUG("Unit[%s]::updateDieCommand....", m_name.c_str());
    this->setState( FUSM_STATE_DIE ); //死亡
    changeAnimation();
    if(m_target){
        m_target->unlocked(this);
        m_target = NULL;
    }

    this->setActive(false);
    _session->deleteTileCoord(this);
    m_squad->dieUnit(this);
    changeAnimTo_CorpseToGrave();
}

//军团旗手
LQCombatUnit* LQCombatUnit::getBannerman(){
    return m_squad->getBannerman();
}

#define REPEAT_CORPSE 60
void LQCombatUnit::changeAnimTo_CorpseToGrave()
{
    // 移除阴影
    m_sprite->removeChildByTag(SHADOW_TAG);
    // 移除冲锋
    if( m_sprite->getChildByTag(FORWARD_TAG))
        m_sprite->removeChildByTag(FORWARD_TAG);
    // 移除BUFF
    //if(m_buff_body_tag>0)
    //   this->removeBufferAnimation( m_buff_body_tag );
    //if(m_buff_foot_tag>0)
    //    this->removeBufferAnimation( m_buff_foot_tag );
    for (int m_buff_tag : m_buff_tags)
    {
        if (m_buff_tag>0)
            this->removeBufferAnimation( m_buff_tag );
    }
    
    if(m_buff_ico>0)
        this->removeChildByTag(m_buff_ico+10000);
    // 移除光圈
    if(m_haloSprite){
        m_haloSprite->removeFromParent();
        m_haloSprite = NULL;
    }
    // 发到地表最底面
    this->setLocalZOrder(10);
    
    // 延时
    DelayTime *waiting = DelayTime::create(0.1f);
    // 尸体特效
    auto effectFunc = CallFunc::create( std::bind( &LQCombatUnit::corpseEffect, this));
    // Action Sequence
    FiniteTimeAction *timeAction = CCSequence::createWithTwoActions(effectFunc, waiting);
    // 最后Action
    auto effectEndFunc = CallFunc::create( std::bind( &LQCombatUnit::afterCorpseShake, this));
    // Action组合
    Action *shakeAction = Sequence::createWithTwoActions(Repeat::create(timeAction, REPEAT_CORPSE), effectEndFunc);
    
    m_sprite->runAction(shakeAction);
}

const Color3B corpse_Effect_colors[2] = {Color3B(255, 60, 60), Color3B(65, 100, 255)};
void LQCombatUnit::corpseEffect()
{
    float baseR = 0.08;
    float bounce = 25;
    if(m_coprseColorIndex < 5 ){
        float bounceY  =bounce*3-bounce*m_coprseColorIndex;
        if(this->getDirectionOffset().y >0){
            bounceY = 0.3*(bounce*m_coprseColorIndex-bounce*3);
        }
        m_sprite->setPositionX(m_sprite->getPositionX()-(13-m_coprseColorIndex)*baseR*this->getDirectionOffset().x*(10-m_coprseColorIndex));
        m_sprite->setPositionY(m_sprite->getPositionY()-(13-m_coprseColorIndex)*baseR*this->getDirectionOffset().y*(10-m_coprseColorIndex)+bounceY) ;
        //m_sprite->setPositionY(m_sprite->getPositionY()-(13-m_coprseColorIndex)*baseR*this->getDirectionOffset().y*(10-m_coprseColorIndex));
    }else if (m_coprseColorIndex < 11){
        m_sprite->setPositionX(m_sprite->getPositionX()-(13-m_coprseColorIndex)*baseR*this->getDirectionOffset().x*(10-m_coprseColorIndex));
        m_sprite->setPositionY(m_sprite->getPositionY()-(13-m_coprseColorIndex)*baseR*this->getDirectionOffset().y*(10-m_coprseColorIndex));

    }

    // 死亡特效
    m_sprite->setOpacity(255 * (REPEAT_CORPSE - m_coprseColorIndex) / REPEAT_CORPSE);
    m_coprseColorIndex++;
}

// 抖动结束
void LQCombatUnit::afterCorpseShake()
{
    setVisible(false);
}

//删除BUFF Animation
void LQCombatUnit::removeBufferAnimation(int animaTag)
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
        CCLOG("%p removeBufferAnimation m_buff_tag:%d", this, animaTag);
    Node* node = this->getChildByTag(animaTag);
    if(node){
        node->stopAllActions();
        node->removeFromParent();
    }
    
    for (vector<int>::iterator iter = m_buff_tags.begin(); iter != m_buff_tags.end(); iter++)
    {
        if ((*iter)==animaTag){
            m_buff_tags.erase(iter);
            break;
        }
    }
    //if (m_buff_body_tag==animaTag) m_buff_body_tag = -1;
    //else if (m_buff_foot_tag==animaTag) m_buff_foot_tag = -1;
}

//改变方向去面对目标
void LQCombatUnit::changeDirection(GameObj* target)
{
    if(!target) return;
    changeDirection(m_target->getCurrentTileCoord());
}

//改变方向去面对目标
void LQCombatUnit::changeDirection(Point targetTileCoord)
{
    Point tileCoord;
    if( m_state==FUSM_STATE_EVADE && !this->hasWayPoint())  //背道而驰
        tileCoord = this->getCurrentTileCoord() - targetTileCoord;
    else
        tileCoord = targetTileCoord - this->getCurrentTileCoord();
    
    LQUnitDirectionType direction = calcDirectionFromTileCoord(tileCoord);
    
    //if(m_target && m_target->getType()==klqLQGameObjHero && direction!=m_direction){
    if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE && (m_index==0 || m_index==48) && m_direction!=direction && direction!=klqUnitDirectionTypeNone){
        CCLOG("IDX%d: from: %s to %s", m_index, TileCoordString(this->getCurrentTileCoord()).c_str(), TileCoordString(targetTileCoord).c_str() );
        if(direction!=klqUnitDirectionTypeNone)
            CCLOG("GameObj::update %s %s %s", m_name.c_str(), TileCoordString(tileCoord).c_str(), UnitDIR[direction]);
        _LQDEBUG(_DEBUG_TYPE_MOVE, "LQCombatUnit::changeDirection:: %s->%s",  UnitDIR[m_direction],  UnitDIR[direction]);
    }
    if(direction==klqUnitDirectionTypeNone)
        return;
    if(m_index==_DEBUG_INDEX)
        CCLOG("2 LQCombatUnit: changeDirection");
    this->setDirection( direction );
}

//改变方向去面对目标
void LQCombatUnit::changeDirectionWithPoint(Point targetpt)
{
    Point pt;
    if( m_state==FUSM_STATE_EVADE && !this->hasWayPoint())  //背道而驰
        pt = this->getPosition() - targetpt;
    else
        pt = targetpt - this->getPosition();
    
    LQUnitDirectionType direction = calcDirectionFromPoint(pt);
    if((m_index == 0 || m_index == 48) && m_direction!=direction)
        _LQDEBUG(_DEBUG_TYPE_MOVE, "changeDirectionWithPoint direction %s->%s  %s", UnitDIR[m_direction],  UnitDIR[direction], PointString(pt).c_str());
    this->setDirection( direction );
    
    if(pt==Point::ZERO)
        m_directionOffset = Point::ZERO;
    else{
        m_directionOffset = abs(pt.x)>abs(pt.y) ? Point(1, abs(pt.y/pt.x)) : Point(abs(pt.x/pt.y), 1);
        m_directionOffset = Point(m_directionOffset.x*(pt.x>0?1:-1),m_directionOffset.y*(pt.y>0?1:-1));
        _LQDEBUG(_DEBUG_TYPE_MOVE, " changeDirectionWithPoint %s %s ...", PointString(pt).c_str(), PointString(m_directionOffset).c_str());
    }
    _LQDEBUG(_DEBUG_TYPE_MOVE, "1 CCDirectionOffset:%s %s", UnitDIR[direction], TileCoordString(m_directionOffset).c_str());
}

//this被obj当目标锁定
void LQCombatUnit::locked(GameObj* obj)
{
    LQCombatUnit* unit = (LQCombatUnit*)obj;
    if(unit->getAttackType()==Combat_AttackType_Remote)  //远程锁定
    {
        m_lockedremotecount++;
        obj->setIsCanAttack(true);
    }else{
        m_lockedcount++;
        //近战攻击位置, 先看一下当前方向是否可以，不行再找其他的
        int idx = getUnitDirectionIndex(m_direction);  //当前位置,起始位置
        int adddir = 1;
        int _dir = idx;
        if (unit->getType()==klqLQGameObjHero)
            adddir = 2;
        while(true) {
            int d = UnitDirection[_dir];
            if (m_lockedObjs[d]==NULL){
                //CCLOG(" %d %d m_lockedObjs:%d", unit->getIndex(), m_data->getResIDC(), d);
                m_lockedObjs[d] = obj;
                obj->setIsCanAttack(true);
                break;
            }
            
            _dir += adddir;
            if(_dir<0)
                _dir+=8;
            else if(_dir>7)
                _dir-=8;
            if (unit->getType()==klqLQGameObjHero && idx==_dir && adddir>0){
                _dir += 1;
                if(_dir<0) _dir=7;
                else if(_dir>7) _dir=0;
                idx = _dir;
                adddir = -2;
                continue;
            }
            if(idx==_dir)
                break;
        }
    }
}
//this被obj当释放锁定
void LQCombatUnit::unlocked(GameObj* obj)
{
    LQCombatUnit* unit = (LQCombatUnit*)obj;
    obj->setIsCanAttack(false);
    if(unit->getAttackType()==Combat_AttackType_Remote)  //远程锁定
    {
        m_lockedremotecount--;
    }else {
        m_lockedcount--;
        for (int i=0; i<8; i++) {
            if (m_lockedObjs[i]==obj){
                m_lockedObjs[i] = NULL;
                break;
            }
        }
    }
    CCASSERT(m_lockedcount>=0, "locked object must >0");
}

//是否能锁定
bool LQCombatUnit::isCanLocked(GameObj* obj)
{
    LQCombatUnit* unit = (LQCombatUnit*)obj;
    if(unit->getAttackType()==Combat_AttackType_Remote)  //远程锁定
        return true;
    return m_lockedcount<=8;
}

//获取obj攻击目标的位置
Point LQCombatUnit::getTargetAttackTileCoord(GameObj* obj)
{
    Point result = m_tileCoord;
    LQCombatUnit* unit = (LQCombatUnit*)obj;
    if(unit->getAttackType()==Combat_AttackType_Remote)  //远程锁定
        return result;
    if(obj){
        for (int _dir=0; _dir<8; _dir++) {
            if (m_lockedObjs[_dir]==obj){
                Point offset = Point::ZERO;
                if(m_type==klqLQGameObjHero)
                    offset = getHeroTileCoordOffsetFromDirection((LQUnitDirectionType)_dir);
                else
                    offset = getTileCoordOffsetFromDirection((LQUnitDirectionType)_dir);
                result = result + offset;
                break;
            }
        }
    }
    return result;
}

void LQCombatUnit::setTarget(GameObj* target)
{
    if(target!=m_target){
        if(m_target){
            m_target->unlocked(this);
        }
        GameObj::setTarget(target);
        if(m_target){
            m_target->locked(this);
        }
        m_wayPoints.clear();
    }
    
    m_squad->drawmArmflags();
}

//---------------------------------------------------------
void LQCombatUnit::update(float dt, Layer *layer)
{
    GameObj::update(dt, layer);
    
    if(m_index==0){
        //CCLOG("update1... %d S:%d SQ:%d", m_index, m_state, m_squadState);
    }else if(m_index==80){
        //CCLOG("update2... %d S:%d SQ:%d", m_index, m_state, m_squadState);
    }
    
    //站住了就不能动了
    if ( !this->getActive() || this->getIsStand() || (m_state==FUSM_STATE_FOLLOW && m_squad->getIsStand()) || m_stopMove)  // || m_state==FUSM_STATE_COUNT
        return;
    
    //调试用的代码
    LQUnitStateEnum curstate = FUSM_STATE_NONE;
    if( m_controller ){
        curstate =  (LQUnitStateEnum)((FSMMachine*)m_controller->getMachine())->getCurrentState()->getType();
        if(curstate!=m_state && _DEBUG_TYPE==_DEBUG_TYPE_MOVE)
            CCLOG("update Unit %p state Error Machine:%d  state:%d", this, curstate, m_state);
    }
    //END
    
    float speed = this->m_armdata->getCurrSpeedValue();  //自身的速度
    Point offset = Point::ZERO;
    
    //索敌和冲锋时军团速度一致  
    if(m_squadState==FUSM_SQSTATE_RUN || m_squadState==FUSM_SQSTATE_APPROACH)
        speed = m_squad->getRunSpeed();
    
    //移动越界处理
    if(m_isBattle && (m_tileCoord.x<=0 || m_tileCoord.x>=MAP_COL || m_tileCoord.y<=0 || m_tileCoord.y>=MAP_ROW) && !this->hasWayPoint() )
    {
        Point tileCoord = Point::ZERO;
        if(m_tileCoord.x<=0 )
            tileCoord = Point(m_tileCoord.x+2, m_tileCoord.y+(m_tileCoord.y>MAP_ROW/2 ? -4 : 4) );
        else if(m_tileCoord.x>=MAP_COL)
            tileCoord = Point(m_tileCoord.x-2, m_tileCoord.y+(m_tileCoord.y>MAP_ROW/2 ? -4 : 4) );
        else if(m_tileCoord.y<=0 )
            tileCoord = Point(m_tileCoord.x+(m_tileCoord.x>MAP_COL/2 ? -4 : 4), m_tileCoord.y+2 );
        else if(m_tileCoord.y>=MAP_ROW)
            tileCoord = Point(m_tileCoord.x+(m_tileCoord.x>MAP_COL/2 ? -4 : 4), m_tileCoord.y-2 );
        //CCLOG("IDX:%d 移动越界处理 %s %s", m_index, TileCoordString(m_tileCoord).c_str(), TileCoordString(tileCoord).c_str());
        addWayPoint(tileCoord);
    }
    
    //阵型跟随
    Point _targetpt = Point::ZERO;
    Point _targettd = Point::ZERO;
    bool isTarget = false;
    Point lastPoint = CCPoint::ZERO;
    int targetType = 0;
    if(m_arrayPoint!=Point::ZERO)  //存在集合点
    {
        _targetpt = m_arrayPoint;
        speed = speed*m_arraySyncSpeed;
        targetType = 1;
        //_LQDEBUG(_DEBUG_TYPE_MOVE, _DEBUG_INDEX_MAX, "存在集合点..... %s", TileCoordString(_targetpt).c_str());
    }else if(m_targetPoint!=Point::ZERO)  //存在目标点
    {
        _targetpt = m_targetPoint;
        targetType = 2;
    }else if (this->getMustPoints().size()>0){
        _targettd = this->getMustPoints().back();
        //if(m_index==32)
        _targetpt = convertPoint(_targettd);
        targetType = 3;
    }else if (this->getWayPoints().size()>0){
        _targettd = this->getWayPoints().back();
        _targetpt = convertPoint(_targettd);
        targetType = 4;
    }else if (this->getTarget()){
        //_targettd = this->getTarget()->getTileCoord();
        _targettd = this->getTarget()->getTargetAttackTileCoord(this);
        isTarget = true;
        //CCLOG("TargetAttackTileCoord %p <- %p %d: %s", this->getTarget(), this, this->getIndex(), PointString(_targettd).c_str());
    }else{
        //没有目标，现在应该是不移动
        if(m_state!=FUSM_STATE_FOLLOW){
            //_LQDEBUG(_DEBUG_TYPE_MOVE, _DEBUG_INDEX_MAX, "NO TARGET,不知道去干什么.");
            return;
        }
    }
    
    //进入格子目标后，将格子转换成点处理
    if(m_tileCoord==_targettd){
        _targetpt = convertPoint(_targettd);
    }
    
    //speed = 1;
    //if (_DEBUG_TYPE==_DEBUG_TYPE_TIME && m_index==0)
    //    CCLOG( "LQCombatUnit::update speed:%0.3f %0.3f state:%d squadState:%d %ld", speed, dt, m_state, m_squadState, getCurrentMillSecond() );
    
    //速度
    speed = speed*dt/0.01;
    
    //根据目标点和格子改变方向
    if(_targetpt != Point::ZERO){
        //面向目标,除了靠近目标的调整
        if( _targetpt.getDistance(this->getPosition())>MAP_HEIGTH/8 ){
            if(isTarget)
                changeDirectionWithPoint(this->getTarget()->getPosition());
            else
                changeDirectionWithPoint(_targetpt); //面向目标
        }else{ //最后一步不转方向
            lastPoint = _targetpt-this->getPosition();  //最后一步
            //_LQDEBUG(_DEBUG_TYPE_MOVE, _DEBUG_INDEX_MAX, "最后一步%s", PointString(m_directionOffset).c_str());
        }
    }else if(_targettd != Point::ZERO){
        changeDirection(_targettd);
    }
    
    offset = this->getDirectionOffset();
    //跟随状态的单元归为后按照旗手的方式行进
    if (m_state==FUSM_STATE_FOLLOW && m_arrayPoint==Point::ZERO && m_squad->getBannerman()){
        if(!m_squad->getIsRallied()){
            //_LQDEBUG(_DEBUG_TYPE_MOVE, _DEBUG_INDEX_MAX, "军团没有集结完毕,等一等.....");
            return;
        }
        if(m_index==_DEBUG_INDEX)
            CCLOG("2 LQCombatUnit: update 跟随状态的单元归为后按照旗手的方式行进");
        this->setDirection( m_squad->getBannerman()->getDirection() );
        offset = m_squad->getBannerman()->getDirectionOffset();
    }
    
    _LQDEBUG(_DEBUG_TYPE_MOVE , " unit::update 1: S:%d DR:%d %d %s", m_state, m_direction, targetType, PointString(m_directionOffset).c_str());
    
    //CCLOG("Direction %d Offset:%s", m_direction, PointString(offset).c_str());
    if(speed!=0){
        Point move = Point(speed*offset.x,speed*offset.y);
        if(lastPoint!=Point::ZERO)  //最后一步
            move = lastPoint;
        this->movebyPostion(move);
        //_LQDEBUG(_DEBUG_TYPE_MOVE, " unit::update 2: %s", PointString(this->getPosition()).c_str());
        if(m_arrayPoint!=Point::ZERO)
        {
            //到达集结地点
            if(lastPoint!=Point::ZERO || m_arrayPoint==_position){
                lastPoint = Point::ZERO;
                m_arrayPoint = Point::ZERO;
                m_arraySyncSpeed = 1;
                m_squad->doArrived(this);
                //this->setState(FUSM_STATE_COUNT);
            }
        }else if (m_targetPoint!=Point::ZERO) {
            //到达目的地
            if(lastPoint!=Point::ZERO || m_targetPoint==_position){
                m_targetPoint = Point::ZERO;
                m_squad->doArrivedDestination();
            }
        }else if (m_mustPoints.size()>0){
            if(m_tileCoord==_targettd || (_targetpt!=Point::ZERO && lastPoint!=Point::ZERO))
            {
                m_mustPoints.pop_back();
            }
        }else if (m_wayPoints.size()>0){
            if(m_tileCoord==_targettd  || (_targetpt!=Point::ZERO && lastPoint!=Point::ZERO))
            {
                m_wayPoints.pop_back();
            }
        }else if(isTarget){
            //到达集结地点
            if(lastPoint!=Point::ZERO || (lastPoint==Point::ZERO && m_tileCoord==_targettd) ){
                this->setIsCanAttack(true);
            }
        }
        //路径走完后开视野，--过中线就开战
        if( isBannerman() && !m_squad->getIsOpenView() ){
            if( m_wayPoints.size()+m_mustPoints.size()==0 ) //|| m_tileCoord.x==MAP_COL/2
            {
                _LQDEBUG(_DEBUG_TYPE_MOVE,"SIDX[%s]:打开视野 %lu %lu x=%.2f", m_squad->getName().c_str(),m_wayPoints.size(),m_mustPoints.size(),m_tileCoord.x);
                m_squad->setIsOpenView(true);
            }
        }
    }
    
    /*
    if(m_state==FUSM_STATE_ATTACK && m_adjustPosition==Point::ZERO)
        return;
    
    if(m_type==klqLQGameObjHero ){
       if (m_squad->getIndex()==4)
           CCLOG("IDX m_squad->getIndex()");
    }
    
    //单元不能离开英雄
    bool isNearHero = false;
    if (m_squad->getTileCoord().getDistance(m_tileCoord)>6 && m_adjustPosition==Point::ZERO)
    {
        //if(m_index==125)
        //CCLOG("IDX%d超出英雄范围 state:%d", m_index, m_state);
        //CCLOG("IDX%d:%d超出英雄范围 %f ; %s", m_index, m_squad->getIndex(), m_squad->getTileCoord().getDistance(m_tileCoord),
        //      TileCoordString(m_squad->getTileCoord()).c_str());
        isNearHero = true;
        //试一试,被英雄拉住就换目标，因为反正无法追逐了
        if(m_state==FUSM_STATE_APPROACH){
            this->setTarget(NULL);
        }
        if(m_state==FUSM_STATE_WAIT){
            m_phaseType = klqUnitAnimPhaseMove;
            this->setState(FUSM_STATE_APPROACH); //靠近英雄
            changeAnimation();
        }
    }
    
    if(m_state==FUSM_STATE_WAIT && !isNearHero)
        return;
    
    float speed = m_speed;  //速度
    
    //没有列阵好，需要列阵
    if (!m_squad->getIsRallied())
    {
        speed = m_arraySyncSpeed*speed;
    }
    
    if(m_adjustPosition!=Point::ZERO && speed!=0){ //需要微调位置哟
        speed = speed*dt/0.01;
        float mx = MIN( abs(m_adjustPosition.x), speed) * (m_adjustPosition.x>0?1:-1);
        float my = MIN( abs(m_adjustPosition.y), speed*0.5) * (m_adjustPosition.y>0?1:-1);
        Point adpt = Point(mx,my);
        this->movePostion(adpt);
        m_adjustPosition = m_adjustPosition-adpt;
        if(m_adjustPosition==CCPoint::ZERO)
            changeAnimation();
        return;
    }
    
    //移动越界处理
    if(m_isBattle && (m_tileCoord.x<=0 || m_tileCoord.x>=MAP_COL || m_tileCoord.y<=0 || m_tileCoord.y>=MAP_ROW) && !this->hasWayPoint() )
    {
        Point tileCoord = Point::ZERO;
        if(m_tileCoord.x<=0 )
            tileCoord = Point(m_tileCoord.x+2, m_tileCoord.y+(m_tileCoord.y>MAP_ROW/2 ? -4 : 4) );
        else if(m_tileCoord.x>=MAP_COL)
            tileCoord = Point(m_tileCoord.x-2, m_tileCoord.y+(m_tileCoord.y>MAP_ROW/2 ? -4 : 4) );
        else if(m_tileCoord.y<=0 )
            tileCoord = Point((m_tileCoord.x+m_tileCoord.x>MAP_COL/2 ? -4 : 4), m_tileCoord.x+2 );
        else if(m_tileCoord.y>=MAP_ROW)
            tileCoord = Point((m_tileCoord.x+m_tileCoord.x>MAP_COL/2 ? -4 : 4), m_tileCoord.x-2 );
        //CCLOG("IDX:%d 移动越界处理 %s", m_index, TileCoordString(tileCoord).c_str());
        addWayPoint(tileCoord);
    }
    
    if(m_isBattle&& (m_tileCoord.x<=-5 || m_tileCoord.x>=MAP_COL+5 || m_tileCoord.y<=-5 || m_tileCoord.y>=MAP_ROW+5) )
    {
        CCLOG("movePostion 越界");
    }
    
    //有移动路径,就根据路径去追踪
    //if (m_index==117 && m_squadDirection==klqCombatSquadDirectionRight && m_mustPoints.size()<=0)
    //    CCLOG("m_direction 1:%d", m_direction);
	Point targetWay = Point(0, 0);
    if(m_arrayPoint!=Point::ZERO)
    {
        targetWay = m_arrayPoint;
        changeDirection(targetWay);
    }else if (m_mustPoints.size()>0){
        targetWay = m_mustPoints.back();
        changeDirection(targetWay);
	}else if (m_wayPoints.size()>0){
        targetWay = m_wayPoints.back();
        changeDirection(targetWay);
	}else if(isNearHero){
        changeDirection(m_squad->getTileCoord());
        Point move = getTileCoordOffsetFromDirection(m_direction);
        this->addWayPoint(m_tileCoord+move);
    }else if (m_target){
        //有目标了,就根据目标去追踪
        changeDirection(m_target);
	}
    //if (m_index==117 && m_squadDirection==klqCombatSquadDirectionRight && m_mustPoints.size()<=0)
    //    CCLOG("m_direction 2:%d", m_direction);
    
    if(speed!=0){
        Point offset = this->getDirectionOffset();
        speed = speed*dt/0.01;
        //CCLOG("IDX%d: speed[%f]*dt/0.01 = %f %f", m_index, m_speed, dt, speed);
        this->movePostion( Point(speed*offset.x,speed*offset.y*0.5));
        if(m_arrayPoint!=Point::ZERO)
        {
            if(this->getPosition()==targetWay){
                m_arrayPoint = Point::ZERO;
                this->setState( FUSM_STATE_WAIT );
                //CCLOG("IDX:%d 到目的地 %s", m_index, TileCoordString(m_tileCoord).c_str());
            }
        }else if (m_mustPoints.size()>0){
            if(this->getTileCoord()==targetWay){
                m_mustPoints.pop_back();
                //CCLOG("IDX:%d 到目的地 %s", m_index, TileCoordString(m_tileCoord).c_str());
            }
        }else if (m_wayPoints.size()>0){
            if(this->getTileCoord()==targetWay){
                m_wayPoints.pop_back();
                //CCLOG("IDX:%d 到目的地 %s", m_index, TileCoordString(m_tileCoord).c_str());
            }
        }
    }
    */
}

//
void LQCombatUnit::changeForwardDirection(LQUnitDirectionType direction)
{
    if(m_forward)
        m_forward->changeDirection(direction);
}

//更改指定动作changeAnimation
void LQCombatUnit::changeAnimationByType(LQUnitAnimPhaseEnum phaseType,LQUnitDirectionType direction)
{
    if (direction!=klqUnitDirectionTypeNone)
        m_direction = direction;
    m_phaseType = phaseType;
    changeAnimation();
}

//实现父类的虚方法，用于改变其身上buff 的方向
void LQCombatUnit::setDirection(LQUnitDirectionType direction)
{
    if(m_direction==direction && m_direction!=klqUnitDirectionTypeNone)
        return;
    
    LQUnitDirectionType old_direction = m_direction;
    GameObj::setDirection(direction);
    if(m_direction!=direction)  //转向失败
        return;
    
    //if(m_armflags){
        //if(m_direction==klqUnitDirectionTypeLeft || m_direction==klqUnitDirectionTypeLeft || m_direction==klqUnitDirectionTypeLeftDown || m_direction==klqUnitDirectionTypeUp || m_direction==klqUnitDirectionTypeDown)
        //    m_armflags->setFlippedX(false);
        //else m_armflags->setFlippedX(true);
    //}
    
    //旗手转向会造成部队转向
    if(isBannerman()){
        if(old_direction!=direction && old_direction!=klqUnitDirectionTypeNone)
        {
            //冲锋转向
            if(this->getSquadState()==FUSM_SQSTATE_APPROACH || this->getSquadState()==FUSM_SQSTATE_RUN){
                m_forward->changeDirection(m_direction);
                LQCombatSoldier* pUnit;
                for (int i=0; i<this->getSquad()->getCombatUnits()->size(); i++)
                {
                    pUnit = this->getSquad()->getCombatUnits()->at(i);
                    pUnit->changeForwardDirection(m_direction);
                }
            }
            
            this->getSquad()->setDirection(direction);
            if(this->getSquadState()==FUSM_SQSTATE_APPROACH  && m_target){
                if(_DEBUG_TYPE==_DEBUG_TYPE_MOVE) CCLOG("SIDX[%s]: setDirection %d->%d", this->getSquad()->getName().c_str(), old_direction, direction);
                this->getSquad()->trunRound(m_target->getPosition());
            }
        }
    }
}



string LQCombatUnit::createAnimationPhaseName(const string &baseName, int modifierNum)
{
    LQUnitAnimPhaseEnum animPhase = (LQUnitAnimPhaseEnum)modifierNum;
    string replaceChar;
    switch (animPhase) {
        case klqUnitAnimPhaseFallDown:
            replaceChar = "_Dead";
            break;
        case klqUnitAnimPhaseMove:
            replaceChar = "_Move";
            break;
        case klqUnitAnimPhaseStand:
            replaceChar = "_Idle";
            break;
        case klqUnitAnimPhaseAttack:
             replaceChar = "_Attack";
            break;
        case klqUnitAnimPhaseSkillAttack:
            replaceChar = "_Skill";
            break;
        default:
            break;
    }
    return baseName + replaceChar;
}

string LQCombatUnit::createAnimationDirectionName(const string &baseName, int modifierNum) {
    string str;
    const char* _dir = DIR[modifierNum];
    //if(m_phaseType==klqUnitAnimPhaseSkillAttack && m_spriteBaseName=="B_DW")  //典韦只有N
    //    _dir = DIR[2];
    stringf(str, "%s_%s", baseName.c_str(), _dir);
    return str;
}

string LQCombatUnit::getSpriteFrameName(const string &unitKey, LQUnitAnimPhaseEnum phaseType,  LQUnitDirectionType directType, uint index)
{
    _currentHeadFrameName = createAnimationDirectionName(createAnimationPhaseName(unitKey, phaseType), directType);
    string spriteFrameName;
    stringf(spriteFrameName, "%s_%03d.png", _currentHeadFrameName.c_str(), index);
    return spriteFrameName;
}

//void LQCombatUnit::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
//{
//    Node::draw(renderer, transform, transformUpdated);
//    
//    _customDebugDrawCommand.init(_globalZOrder);
//    _customDebugDrawCommand.func = CC_CALLBACK_0(LQCombatUnit::drawLineData, this,transform,transformUpdated);
//    renderer->addCommand(&_customDebugDrawCommand);
//}

Point LQCombatUnit::getLinePosition(Point pt)
{
    Point _p = _position - pt;
    Point _rpt = Point::ZERO;
    //float _x = 0;
    ///if(_p.x>0) _x = 0.1; else if(_p.x<0)  _x = -0.1;
    //float _y = 0;
    //if(_p.y>0) _y = 0; else if(_p.y<0)  _y = 0.85;
    //this->getContentSize().width
    _rpt = _position;// + Point(_x*m_imgwidth*getScale(), _y*m_imgheight*getScale());
    return _rpt;
}

void LQCombatUnit::drawLineData(const kmMat4& transform, bool transformUpdated)
{
    /*
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    
    if( m_armflags ){
        if(m_squadDirection==klqCombatSquadDirectionLeft)
            DrawPrimitives::setDrawColor4B(100, 255, 255, 100);
        else
            DrawPrimitives::setDrawColor4B(255, 255, 100, 100);
        glLineWidth(2);
        Point pt1 = m_armflags->getLinePosition(this->getPosition()) - this->getLinePosition(m_armflags->getPosition());
        DrawPrimitives::drawLine( Point::ZERO, pt1 );
    }    
    
    kmGLPopMatrix();*/
}

LQ_NAMESPACE_END
