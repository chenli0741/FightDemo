//
// LQFightLayer.cpp
// game
//
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#include "LQFightLayer.h"

#include "LQCombatUnit.h"
#include "LQBattleSession.h"
#include "LQData.h"
#include "LQMonsterUserData.h"
#include "LQHeroButton.h"
#include "AudioManager.h"
#include "MaskLayer.h"
#include "LQSkillShowingLayer.h"

#if !defined(LQ_DEMO_FIGHT)
#include "HclcData.h"
#include "LQPVELoseLayer.h"
#include "LQPVEWinLayer.h"
#include "WPlatform.h"
#else
#include "HelloWorldScene.h"
#endif

#include "ResourceManage.h"
#include "LQMissile.h"
//#include "SmokeManager.h"
#define KHpValueLabelTagNumber  100

#define KQuickButtonTagNumber   101     //快进
#define KAutoButtonTagNumber    102     //自动战斗
#define KContinueButtonTagNumber 103    //继续战斗
#define KReBackButtonTagNumber 104      //撤退
#define KPauseButtonTagNumber 105       //暂停
#define KBeginButtonTagNumber 106       //开始战斗


#define KButtonHightLightSpriteTagNumber 111
#define KSkillButtonFuryFullStateSpriteTagNumber 120


LQ_NAMESPACE
using namespace std;
USING_NS_CC;

LQFightLayer::LQFightLayer():m_sprite(NULL), nowScale(1), lastScale(1), isMultipleTouch(false), m_isStared(false), mapSize(1024,768),m_controlLayer(NULL),m_currentSkillButton(NULL),m_myHpSlider(NULL),m_maskLayer(NULL),m_now_showing_skill(false),m_time_showing_skill(0),m_now_showing_plot(false),m_isGameOver(false),m_pauseLayer(NULL),m_normalButtonVector(NULL),m_shakeLayer(NULL),_isPausedLayer(false),m_win(false),_skillhero(NULL),_plothero(NULL),_devilhero(NULL),isSceneScaleing(false),m_meleeEffect(0),_storyIDTime(0), _timeStoryID(0) //,m_currHeroButton(NULL)
{
    m_mySquadVector = new LQCombatSquadVector();
    m_otherSquadVector = new LQCombatSquadVector();
    m_skillButtonVector = new vector<LQHeroButton*>();
    m_normalButtonVector = new vector<ControlButton*>();
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQFightLayer::LQFightLayer  %p", this);
}

LQFightLayer::~LQFightLayer()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQFightLayer::~LQFightLayer  %p", this);
    
    squads.clear();
    CC_SAFE_DELETE(m_mySquadVector);
    CC_SAFE_DELETE(m_otherSquadVector);
    CC_SAFE_DELETE(m_skillButtonVector);
    CC_SAFE_DELETE(m_normalButtonVector);
    //CC_SAFE_DELETE(m_currZoneData);
    
    FlagPosition::getInstance()->clearPoints();
    LQBattleSession::getInstance()->clearGame();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    
    //Director::getInstance()->purgeCachedData();
}


const float LQFightLayer::LAYER_SCALEMAX = 1.3f;
const float LQFightLayer::LAYER_SCALEMIN = 0.75f;

bool LQFightLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}

#pragma mark - initData
void LQFightLayer::initData(string valuestr)
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
        CCLOG("LQFightLayer::initData Begin %f", getCurrentMillSecond());
    
    _startStoryID = 0;
    
    m_scene = (Scene*)this->getParent();
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioManager::getInstance()->start();
    
    //0:演示关卡 1: 普通关卡 2: 列传 3: 排行榜挑战 4: 经验关卡 :9引导自动战斗
    _fightType = LQBattleSession::getInstance()->m_fightType;
    
    _furiesTime = 0; _furiesTimeParam = 0; _furiesAddRadio = 0;
    if (_fightType==8){
        _furiesTimeParam = getFormulaParamFactor(40,"a");
        _furiesAddRadio = getFormulaParamFactor(40,"b");
    }

    string mapFile;
    stringf(mapFile, "res/tiledmap/%s",LQBattleSession::getInstance()->getBattlaMap().c_str());
    auto sprite = Sprite::create(mapFile);
    sprite->setTag(NODE_TAG_FIGHTMAP);
    mapSize = sprite->getContentSize();
    Sprite* ground = Sprite::create("res/blank.png");
    ground->setContentSize(Size(2048,1024));
    sprite->setPosition(Point(ground->getContentSize().width/2, ground->getContentSize().height/2));
    ground->addChild(sprite);
    m_sprite = ground;
    
    mapMinWidth = (mapSize.width-visibleSize.width)/2;
    mapMinHeight = (mapSize.height-visibleSize.height)/2;
    
    // position the sprite on the center of the screen
    m_sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->setColor(Color3B::BLACK);
    //m_shakeLayer = LayerColor::create(Color4B(0, 0, 0, 255), visibleSize.width, visibleSize.height);
    m_shakeLayer = Layer::create();
    m_shakeLayer->addChild(m_sprite);
    m_shakeLayer->setTag(NODE_TAG_SHAKE);
    this->addChild(m_shakeLayer,-1);
    
    //初始化
    LQBattleSession::getInstance()->initGame(m_sprite);
    LQHeroUserData* hero;
    float slowSpeed = -1;
    for(int i=0;i<LQBattleSession::getInstance()->getLeftHeroDatas().size();i++){
        hero = LQBattleSession::getInstance()->getLeftHeroDatas().at(i);
        LQCombatSquad* unit1 = LQCombatSquad::create(hero,hero->getSquadDirection());
        unit1->putLayer(m_sprite, hero->getFormationIndex()-1);
        squads.push_back(unit1);
        unit1->retain();
        if(unit1->getSquadX()==klqCombatSquadRowBack){
            if(slowSpeed<0) slowSpeed = unit1->getSpeed();
            slowSpeed = MIN(slowSpeed,unit1->getSpeed());
        }
        m_mySquadVector->push_back(unit1);
        unit1->getHero()->onSkillStartCallback = CC_CALLBACK_3(LQFightLayer::skillStartCallback,this);
        unit1->getHero()->onHitStartCallback = CC_CALLBACK_2(LQFightLayer::hitStartCallback,this);
        unit1->getHero()->onSkillEndCallback = CC_CALLBACK_2(LQFightLayer::skillEndCallback,this);
        unit1->getHero()->onHeroDiedCallback = CC_CALLBACK_2(LQFightLayer::heroDiedCallback,this);
        if (_fightType==0){
            _startStoryID = 40399001;
            if(hero->getResIDC()==90041502)
                unit1->getHero()->setStoryIDFuries(40399002);
            else if(hero->getResIDC()==90041501)
                unit1->getHero()->setStoryIDFuries(40399003);
            else if(hero->getResIDC()==90041503)
                unit1->getHero()->setStoryIDFuries(40399004);
        }
    }
    if (slowSpeed>0.65) slowSpeed = 0.65;  //最后排速度
    for (int i = 0; i < m_mySquadVector->size(); i++) {
        LQCombatSquad* squad = m_mySquadVector->at(i);
        if(squad->getSquadX()==klqCombatSquadRowBack)
            squad->setSlowSpeed(slowSpeed);
    }
    slowSpeed = -1;
    for(int i=0;i<LQBattleSession::getInstance()->getRightHeroDatas().size();i++){
        hero = LQBattleSession::getInstance()->getRightHeroDatas().at(i);
        LQCombatSquad* unit2 = LQCombatSquad::create(hero,hero->getSquadDirection());
        unit2->putLayer(m_sprite, hero->getFormationIndex()-1);
        squads.push_back(unit2);
        unit2->retain();
        if(unit2->getSquadX()==klqCombatSquadRowBack){
            if(slowSpeed<0) slowSpeed = unit2->getSpeed();
            slowSpeed = MIN(slowSpeed,unit2->getSpeed());
        }
        m_otherSquadVector->push_back(unit2);
        unit2->getHero()->onSkillStartCallback = CC_CALLBACK_3(LQFightLayer::skillStartCallback,this);
        unit2->getHero()->onHitStartCallback = CC_CALLBACK_2(LQFightLayer::hitStartCallback,this);
        unit2->getHero()->onSkillEndCallback = CC_CALLBACK_2(LQFightLayer::skillEndCallback,this);
        unit2->getHero()->onHeroDiedCallback = CC_CALLBACK_2(LQFightLayer::heroDiedCallback,this);
        if (_fightType==0){
            if(hero->getResIDC()==90041508){
                //unit2->getHero()->setStoryHPRate(30);
                //unit2->getHero()->setStoryIDHP(40399004);
            }else if(hero->getResIDC()==90041509){
                _devilhero = (LQCombatHero*)unit2->getHero();
                unit2->getHero()->setStoryIDFuries(40399005);
            }
        }
    }
    if (slowSpeed>0.65) slowSpeed = 0.65; //最后排速度
    for (int i = 0; i < m_otherSquadVector->size(); i++) {
        LQCombatSquad* squad = m_otherSquadVector->at(i);
        if(squad->getSquadX()==klqCombatSquadRowBack)
            squad->setSlowSpeed(slowSpeed);
    }
    
    #if !defined(LQ_DEMO_FIGHT)
        WPlatform::getInstance()->setTouchStyle();
    #endif

    // Register Touch Event
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(LQFightLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(LQFightLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(LQFightLayer::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
#if defined(LQ_DEMO_FIGHT)
    //测试格子显示
    //SpriteBatchNode* batchNode = SpriteBatchNode::create("res/xxx.png");
    for(int i=0;i<MAP_COL;i++)
        for(int j=0;j<MAP_ROW;j++){
            //testSpritexr(batchNode, i,j);
        }
#endif
    
    //统计数据
    LQBattleSession::getInstance()->totalGame(true);
    //TODO
    this->setScale(LQFightLayer::LAYER_SCALEMIN);
    mapMinWidth = (mapSize.width*LQFightLayer::LAYER_SCALEMIN-visibleSize.width)/2;
    mapMinHeight = (mapSize.height*LQFightLayer::LAYER_SCALEMIN-visibleSize.height)/2;
    
    //开启监听事件
//    auto listener1 = LQSkillEffectListener::create();
//    listener1->onEffectBeganEvent = CC_CALLBACK_2(LQFightLayer::onEffectBegan, this);
//    listener1->onEffectEndedEvent = CC_CALLBACK_2(LQFightLayer::onEffectEnded, this);
//    _eventDispatcher->addEventListenerWithFixedPriority(listener1, 1);
    
    //事件监听
    auto listener1 = LQSkillEffectListener::create( LQEventSkillEffect::SKILL_EFFECT_EVENT_TYPE, CC_CALLBACK_1(LQFightLayer::onEffectEnded, this) );
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    auto listener2 = EventListenerCustom::create(LQCombatSquad::EVENT_FORWARD_EFFECT, [this](EventCustom* event){
        this->shakeScene();
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
    
    if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
        CCLOG("LQFightLayer::initData End %.3f", getCurrentMillSecond());
}

void LQFightLayer::onEnter()
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
        CCLOG("LQFightLayer::onEnter (战斗场景进入) %.3f", getCurrentMillSecond());
    
    Layer::onEnter();
    
    initLayer();
    
    bool isplay = UserDefault::getInstance()->getIntegerForKey("UD_MUSICBG_KEY");
    if(0==isplay)
    {
        AudioManager::getInstance()->addAudio();
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        SimpleAudioEngine::getInstance()->playBackgroundMusic(FightBackMusic,true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8);//设置音乐声音
    }
}
void LQFightLayer::onExit()
{
    Layer::onExit();
    
    //this->unscheduleAllSelectors();
    bool isplay = UserDefault::getInstance()->getIntegerForKey("UD_MUSICBG_KEY");
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    if(0==isplay)
    {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(MainBackMusic,true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8);//设置音乐声音
    }
    
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    frameCache->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    AnimationCache::destroyInstance();
    ResourceManage::clear();
    LQMissileManager::clear();
    //SmokeManager::clear();
    
    //debug
    if( _DEBUG_TYPE==_DEBUG_TYPE_SAFE )
        CCLOG("TextureCache()->getCachedTextureInfo....");
    AudioManager::getInstance()->unloadAudio();
    //Director::getInstance()->purgeCachedData();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();

}

void LQFightLayer::initLayer()
{
    m_controlLayer = Layer::create();
    m_controlLayer->setContentSize(visibleSize);
    m_controlLayer->setTag(NODE_TAG_CTRLLAYER);
    
    Node* _ctrllayer = m_controlLayer;
#if defined(LQ_DEMO_FIGHT)
    _ctrllayer = this->getParent();  //demo有BUG
#endif
    LQBattleSession::getInstance()->setCtrlLayer(_ctrllayer);
    
    this->getParent()->addChild(m_controlLayer, NODE_ZORDER_CTRLLAYER);
    
    Sprite* headIcon = Sprite::create("res/head/head-caocao.png");
    unsigned long count = 0; //m_mySquadVector->size()
    for (int i = 0; i < m_mySquadVector->size(); i++) {
        if (m_mySquadVector->at(i)->isVisible())
            count++;
    }
    
    
    float x = visibleSize.width/2 - headIcon->getContentSize().width*0.1;
    float len = 0;
    float bottomcd = 1.25;
    Size buttonsize;
    for (int k = m_mySquadVector->size()-1; k >= 0; k--) {
        LQCombatHero* hero = m_mySquadVector->at(k)->getHero();
        
        LQHeroButton* heroButton = LQHeroButton::createHeroButtonWithData(hero);
        hero->setHeroButton(heroButton);
        heroButton->setAnchorPoint(Point(1, 0.5));
        heroButton->setScale(1.25);
        heroButton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQFightLayer::OnIconButtonClicked),Control::EventType::TOUCH_UP_INSIDE);
        m_controlLayer->addChild(heroButton,1);
        if(_fightType==3)
            heroButton->setEnabled(false);
        
        if (k==count-1){
            buttonsize = heroButton->getContentSize();
            len = buttonsize.width*bottomcd*count+buttonsize.width*0.1;
            x += len/2;
            x -= buttonsize.width*0.05;
        }
        heroButton->setPosition(Point(x, buttonsize.height*0.5*bottomcd));
        
        m_skillButtonVector->push_back(heroButton);
        if (!m_mySquadVector->at(k)->isVisible())
            heroButton->setVisible(false);
        else
            x -= heroButton->getContentSize().width * bottomcd;
    }
 
    Scale9Sprite* bottom9Sp = Scale9Sprite::createWithSpriteFrameName("zddt_6.png");
    bottom9Sp->setPreferredSize(Size(len, buttonsize.height * bottomcd));
    bottom9Sp->setAnchorPoint(Point(0.5, 0));
    bottom9Sp->setPosition(Point(visibleSize.width/2, 0));
    bottom9Sp->setVisible(false);
    m_controlLayer->addChild(bottom9Sp,-1);
     
    //自动战斗
    float disX = visibleSize.width*(1-0.05);
    float disY = visibleSize.height*0.1;
    Sprite* autosp = Sprite::createWithSpriteFrameName("btn_zidongzhandou.png");
    ControlButton* autobutton = ControlButton::create(Scale9Sprite::createWithSpriteFrameName("btn_zidongzhandou.png"));
    autobutton->setPreferredSize(autosp->getContentSize());
    autobutton->setPosition(Point(disX,disY));
    autobutton->setTag(KAutoButtonTagNumber);
    autobutton->setAnchorPoint(Point(1, 0.5));
    autobutton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQFightLayer::itemTouch),Control::EventType::TOUCH_UP_INSIDE);
    m_controlLayer->addChild(autobutton);
    m_normalButtonVector->push_back(autobutton);
    
    Sprite* autoHSp = Sprite::createWithSpriteFrameName("btn_zidongzhandou_anxia.png");
    autoHSp->setTag(KButtonHightLightSpriteTagNumber);
    autoHSp->setPosition(Point(autobutton->getContentSize()*0.5));
    autobutton->addChild(autoHSp);
    autoHSp->setVisible(LQBattleSession::getInstance()->getIsAutoFight());
    
    bool isCanAuto = LQBattleSession::getInstance()->getIsCanAutoFight();
    autobutton->setEnabled(isCanAuto);
    if (!isCanAuto)
        autobutton->setVisible(false);
    
    //怒气击鼓
    if (_fightType==8){
        Sprite* hurtsSprite = Sprite::createWithSpriteFrameName("jigu_1.png");
        hurtsSprite->setPosition(Point(visibleSize.width*0.1,disY));
        m_controlLayer->addChild(hurtsSprite);
        ActionInterval *action = this->createAnimation("jigu_%d.png",8);
        RepeatForever* forever1 = RepeatForever::create(action);
        hurtsSprite->runAction(forever1);
    }
    
    //快进按钮
    /*disX += autosp->getContentSize().width *1.2;
    Sprite* quicksp = Sprite::createWithSpriteFrameName("zd_di_jiazu.png");
    ControlButton* quickButton = ControlButton::create(Scale9Sprite::createWithSpriteFrameName("zd_di_jiazu.png"));
    quickButton->setPreferredSize(quicksp->getContentSize());
    quickButton->setPosition(Point(disX,disY));
    quickButton->setTag(KQuickButtonTagNumber);
    quickButton->setAnchorPoint(Point(0, 0.5));
    quickButton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQFightLayer::itemTouch),Control::EventType::TOUCH_UP_INSIDE);
    m_normalButtonVector->push_back(quickButton);
    
    m_controlLayer->addChild(quickButton);
    
    Sprite* pauseHSp = Sprite::createWithSpriteFrameName("tiaoguo_anxia.png");
    pauseHSp->setTag(KButtonHightLightSpriteTagNumber);
    pauseHSp->setPosition(Point(quickButton->getContentSize()*0.5));
    quickButton->addChild(pauseHSp);
    pauseHSp->setVisible(!isPausedGame());*/
    
    
    //暂停按钮
    //disX += autosp->getContentSize().width *1.2;
    disX = visibleSize.width*0.02;
    disY = visibleSize.height*0.95;
    Sprite* pausesp = Sprite::createWithSpriteFrameName("zhanting.png");
    ControlButton* pauseButton = ControlButton::create(Scale9Sprite::createWithSpriteFrameName("zhanting.png"));
    pauseButton->setPreferredSize(pausesp->getContentSize());
    pauseButton->setPosition(Point(disX,disY));
    pauseButton->setTag(KPauseButtonTagNumber);
    pauseButton->setAnchorPoint(Point(0, 0.5));
    pauseButton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQFightLayer::itemTouch),Control::EventType::TOUCH_UP_INSIDE);
    m_normalButtonVector->push_back(pauseButton);
    m_controlLayer->addChild(pauseButton);
    
    //胜利条件
    string sltj = LQBattleSession::getInstance()->getCondDesc();// "保证NPC不死";
    Sprite* sltjSp = Sprite::create("res/level/zx_sltj.png");
    sltjSp->setPosition(Point(visibleSize.width*0.95, disY));
    sltjSp->setAnchorPoint(Point(0.5, 0.5));
    sltjSp->setVisible(sltj.length()>0);
    m_controlLayer->addChild(sltjSp,10);
    Label* sltjLabel = Label::createWithSystemFont(sltj.c_str(), "Helvetica", 24);
    sltjLabel->setAnchorPoint(Point(1.0, 0.5));
    sltjLabel->setPosition(Point(visibleSize.width*0.95-sltjSp->getContentSize().width*0.55,disY));
    sltjLabel->setColor(Color3B::WHITE);
    sltjLabel->enableShadow();
    sltjLabel->setVisible(sltj.length()>0);
    m_controlLayer->addChild(sltjLabel,10);
    
    //计时器
    disX = pauseButton->getContentSize().width*1.3;
    Scale9Sprite* timepbg = Scale9Sprite::createWithSpriteFrameName("zd_shijiandi.png");
    timepbg->setPreferredSize(Size(pauseButton->getContentSize().width*1.8, pauseButton->getContentSize().height*0.9));
    timepbg->setAnchorPoint(Point(0, 0.5));
    timepbg->setPosition(Point(disX, disY));
    m_controlLayer->addChild(timepbg);
    //计数器
    m_timeSp = Label::createWithSystemFont(intToTimeString(DEF_BATTLE_TIMEMAX), "Helvetica", 30);
    m_timeSp->setColor(Color3B::GREEN);
    m_timeSp->setAnchorPoint(Point(0.5, 0.5));
    m_timeSp->setPosition(Point(timepbg->getContentSize().width*0.5, timepbg->getContentSize().height/2));
    timepbg->addChild(m_timeSp);
    
    m_pauseLayer = LayerColor::create(Color4B(0, 0, 0, 125), visibleSize.width, visibleSize.height);
    this->getParent()->addChild(m_pauseLayer,NODE_ZORDER_MAX);
    m_pauseLayer->setVisible(false);
    
    float x1 = visibleSize.width/3;
    float y1 = visibleSize.height/2;
    
    //退出按钮-- 撤退
    Sprite* tempSp = Sprite::createWithSpriteFrameName("btn_exit.png");
    ControlButton* rebackButton = ControlButton::create(Scale9Sprite::createWithSpriteFrameName("btn_exit.png"));
    rebackButton->setPreferredSize(tempSp->getContentSize());
    rebackButton->setPosition(Point(x1, y1));
    m_pauseLayer->addChild(rebackButton);
    rebackButton->setTag(KReBackButtonTagNumber);
    rebackButton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQFightLayer::itemTouch),Control::EventType::TOUCH_UP_INSIDE);
    m_normalButtonVector->push_back(rebackButton);
    
    //继续游戏
    x1 += visibleSize.width/3;
    tempSp = Sprite::createWithSpriteFrameName("btn_continue.png");
    ControlButton* continueButton = ControlButton::create(Scale9Sprite::createWithSpriteFrameName("btn_continue.png"));
    continueButton->setPosition(Point(x1, y1));
    continueButton->setPreferredSize(tempSp->getContentSize());
    m_pauseLayer->addChild(continueButton);
    continueButton->setTag(KContinueButtonTagNumber);
    continueButton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQFightLayer::itemTouch),Control::EventType::TOUCH_UP_INSIDE);
    m_normalButtonVector->push_back(continueButton);
    int maxHp = ceil(LQBattleSession::getInstance()->getRightSquadHPMax()/3);
    m_BossHpLayer = Layer::create();
    m_BossHpLayer->setPosition(Point(visibleSize.width/2,visibleSize.height*0.90 + 20) );
    m_controlLayer->addChild(m_BossHpLayer);
    auto hpDi = Sprite::createWithSpriteFrameName("nq_jindutiaodi.png");
    m_BossHpLayer->addChild(hpDi);
    
    //增加BOSS血量  4.6chenli
    m_otherHpSlider[0] = ControlSlider::create( Sprite::createWithSpriteFrameName("nq_jindutiaolan_3.png"),
                                            Sprite::createWithSpriteFrameName("nq_jindutiaolan_1.png"), Sprite::create("res/blank.png"));
    m_otherHpSlider[0]->setEnabled(false);
    m_otherHpSlider[0]->setVisible(false);
    m_BossHpLayer->addChild(m_otherHpSlider[0]);
    m_otherHpSlider[0]->setMaximumValue(maxHp);
    m_otherHpSlider[0]->setMinimumValue(0);
    m_otherHpSlider[0]->setValue(maxHp);
    
    //boss血量分层  8.12wangbing
    m_otherHpSlider[1] = ControlSlider::create( Sprite::createWithSpriteFrameName("nq_jindutiaolan_1.png"),
                                               Sprite::createWithSpriteFrameName("nq_jindutiaolan_2.png"), Sprite::create("res/blank.png"));
    m_otherHpSlider[1]->setEnabled(false);
    m_otherHpSlider[1]->setVisible(false);
    m_BossHpLayer->addChild(m_otherHpSlider[1]);
    m_otherHpSlider[1]->setMaximumValue(maxHp);
    m_otherHpSlider[1]->setMinimumValue(0);
    m_otherHpSlider[1]->setValue(maxHp);
    
    m_otherHpSlider[2] = ControlSlider::create( Sprite::createWithSpriteFrameName("nq_jindutiaolan_2.png"),
                                               Sprite::createWithSpriteFrameName("nq_jindutiaolan_1.png"), Sprite::create("res/blank.png"));
    m_otherHpSlider[2]->setEnabled(false);
    m_otherHpSlider[2]->setVisible(false);
    m_BossHpLayer->addChild(m_otherHpSlider[2]);
    m_otherHpSlider[2]->setMaximumValue(maxHp);
    m_otherHpSlider[2]->setMinimumValue(0);
    m_otherHpSlider[2]->setValue(maxHp);
    
    m_otherHpSlider[0]->setPosition(Point(57,15));
    m_otherHpSlider[1]->setPosition(Point(57,15));
    m_otherHpSlider[2]->setPosition(Point(57,15));
    /*
    auto _flagmark = Sprite::create("res/level/rHeadStencil.png");
    _flagmark->setScale(0.95);
    _flagmark->setPosition(Point(_flagmark->getContentSize().width/2,_flagmark->getContentSize().height/2));
    m_BossHpLayer->addChild(_flagmark);
     */
    /*
    //创建裁减节点类
    auto clipper = ClippingNode::create();//设置裁剪节点
    clipper->setInverted(false);//设置底板可见
    clipper->setAlphaThreshold(0);//设置透明度Alpha值为0
    clipper->setAnchorPoint(Point(0.5, 0.5));
    clipper->setPosition(Point(-101,0));
    m_BossHpLayer->addChild(clipper,5);
    clipper->setStencil(_flagmark);
    */
    auto _flaghead = Sprite::create("res/head/head-menghuo.png");
    _flaghead->setPosition(Point(-101,0));
    //clipper:addChild(_flaghead);
    m_BossHpLayer->addChild(_flaghead, 5);
    auto _fream = Sprite::create("res/level/rHeadStencil_waiquan.png"); // 头像边框，可以读表改变边框，但是大小资源大小必须大小一样
    _fream->setScale(0.85);
    _fream->setPosition(Point(-101,0));
    m_BossHpLayer->addChild(_fream,6);
    auto point = Point(-30,-20);
    for(int i = 0;i<3;i++)
    {
        spriteHp[i] = Sprite::createWithSpriteFrameName("gz_heart.png");
        auto size = spriteHp[i]->getContentSize();
        spriteHp[i]->setPosition(Point(point.x + i * size.width,point.y));
        m_BossHpLayer->addChild(spriteHp[i],7);
    }
    
    bool isreplay = LQBattleSession::getInstance()->getIsReplay();
    //展示关卡
    if (_fightType==0){
        autobutton->setVisible(false);
    }else if(_fightType==3){
        if(!isreplay)
            rebackButton->setEnabled(false);
#if !defined(LQ_DEMO_FIGHT)
        autobutton->setEnabled(false);
#endif
        LQBattleSession::getInstance()->setIsAutoFight(true);
        autoHSp->setVisible(LQBattleSession::getInstance()->getIsAutoFight());
    }
    
    else if (_fightType == 8){  //boss  显示血条
        m_otherHpSlider[0]->setVisible(true);
        m_otherHpSlider[1]->setVisible(true);
        m_otherHpSlider[2]->setVisible(true);
    }
    m_BossHpLayer->setVisible(_fightType == 8);
    initControls();
}

void LQFightLayer::refushButtonPostion()
{
    Sprite* headIcon = Sprite::create("res/head/head-caocao.png");
    unsigned long count = 0; //m_mySquadVector->size()
    for (int i = 0; i < m_mySquadVector->size(); i++) {
        if (m_mySquadVector->at(i)->isVisible())
            count++;
    }
    float x = visibleSize.width/2 - headIcon->getContentSize().width*0.1;
    float len = 0;
    float bottomcd = 1.25;
    Size buttonsize;
    for (int k = 0; k < m_skillButtonVector->size(); k++) {
        LQHeroButton* heroButton = m_skillButtonVector->at(k);
        
        if (k==0){
            buttonsize = heroButton->getContentSize();
            len = buttonsize.width*bottomcd*count+buttonsize.width*0.1;
            x += len/2;
            x -= buttonsize.width*0.05;
        }
        heroButton->setPosition(Point(x, buttonsize.height*0.5*bottomcd));
        
        if (!heroButton->getHero()->getSquad()->isVisible())
            heroButton->setVisible(false);
        else{
            heroButton->setVisible(true);
            x -= heroButton->getContentSize().width * bottomcd;
        }
    }
    
}

void LQFightLayer::initControls()
{
    m_isStared = true;
    
    gameBegin();
    
    //-----剧情----------------------------------------------------------------
    int zoneID = LQBattleSession::getInstance()->getZoneID();
    if( _fightType > 1 ) {
        // 不是普通关卡不播放剧情
        this->plotEndStarGame();
        return;
    }
    
    LQZoneInfoData* m_zoneData = NULL;
    if (zoneID>0){
        int pkey = (zoneID/1000)%10+1;
        m_zoneData = (LQZoneInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName("ZoneInfo", pkey, zoneID);
    }
    
    if(m_zoneData == NULL && _fightType>0) {
        // 没有得到场景对象;  直接战斗  没剧情
        this->plotEndStarGame();
    }else{
        // 得到场景了
        if(_startStoryID<=0){
            if (m_zoneData){
                _startStoryID = m_zoneData->getStartStoryID();
                std::string triggerstr = m_zoneData->getTriggerStoryID();
                if (triggerstr.length()>0){  //处理触发剧情初始化数据  （类型,剧情ID,值）
                    std::vector<std::string> attrvals = split(triggerstr.c_str(), ';');
                    int k=0;
                    while (k<attrvals.size())
                    {
                        string attrstr = attrvals.at(k);
                        std::vector<std::string> attrvalue = split(attrstr.c_str(), ',');
                        if(attrvalue.size()>2){
                            int trigType = atoi(attrvalue[0].c_str());  //
                            int trigPolt = atoi(attrvalue[1].c_str());
                            switch (trigType) { //1 怒气满  2 血值  3 时间 4 死亡
                                case 1:{  //1,PoltId,1,HeroId
                                    int trigHeroDr = atoi(attrvalue[2].c_str());
                                    int trigHero = atoi(attrvalue[3].c_str());
                                    LQCombatSquadVector* squadVector;
                                    LQCombatHero* hero;
                                    squadVector = trigHeroDr==1 ? m_mySquadVector : m_otherSquadVector;
                                    for (int i = 0; i < squadVector->size(); i++) {
                                        hero = squadVector->at(i)->getHero();
                                        if (hero->getData()->getResIDC()==trigHero)
                                            hero->setStoryIDFuries(trigPolt);
                                    }
                                }
                                    break;
                                case 2:{  //1,PoltId,1,HeroId,HPRate
                                    int trigHeroDr = atoi(attrvalue[2].c_str());
                                    int trigHero = atoi(attrvalue[3].c_str());
                                    int HPRate = atoi(attrvalue[4].c_str());
                                    LQCombatSquadVector* squadVector;
                                    LQCombatHero* hero;
                                    squadVector = trigHeroDr==1 ? m_mySquadVector : m_otherSquadVector;
                                    for (int i = 0; i < squadVector->size(); i++) {
                                        hero = squadVector->at(i)->getHero();
                                        if (hero->getData()->getResIDC()==trigHero){
                                            hero->setStoryHPRate(HPRate);
                                            hero->setStoryIDHP(trigPolt);
                                        }
                                    }
                                }
                                    break;
                                case 3: { //3,PoltId,time(毫秒)
                                    int trigTime = atoi(attrvalue[2].c_str());
                                    _storyIDTime = trigTime*0.001;
                                    _timeStoryID = trigPolt;
                                }
                                    break;
                                case 4: { //4,PoltId,1,HeroId
                                    int trigHeroDr = atoi(attrvalue[2].c_str());
                                    int trigHero = atoi(attrvalue[3].c_str());
                                    LQCombatSquadVector* squadVector;
                                    LQCombatHero* hero;
                                    squadVector = trigHeroDr==1 ? m_mySquadVector : m_otherSquadVector;
                                    for (int i = 0; i < squadVector->size(); i++) {
                                        hero = squadVector->at(i)->getHero();
                                        if (hero->getData()->getResIDC()==trigHero){
                                            hero->setStoryIDIsDead(trigPolt);
                                        }
                                    }
                                }
                                    break;
                                default:
                                    break;
                            }
                        }
                        k++;
                    }
                }
            }
#if defined(LQ_DEMO_FIGHT)
            _startStoryID = 0;
#endif
        }
        
        if(_startStoryID == 0) { // 没有剧情
            this->plotEndStarGame();
        }else{ // 有剧情
            CCLOG("1111有剧情 ");
            LQPoltLayer*layer = LQPoltLayer::createLayer();
            if(layer->getIsThereAre(_startStoryID) || _fightType==0){ // 剧情没有播放过  开始播放
                //LQOpeningInfoData* m_gutData = (LQOpeningInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQOpeningInfoData::getAliasName(),_startStoryID);
                layer->setTag(1);
                layer->setDelegate(this);
                layer->setPolt(this->loadPlotPlayText(_startStoryID));
                layer->setPosition(layer->getPositionX(),layer->getPositionY()-(visibleSize.height-683)/2);
                this->getParent()->addChild(layer,NODE_ZORDER_GUT);
            }else{ // 剧情被播放过了 so...
                this->plotEndStarGame();
            }
        }
    }
    //------------------------------------------------------------------------

}

//战斗开始剧情结束
void LQFightLayer::poltEnd(LQPoltLayer *polt)
{
    int types = polt->getTag();
    CCLOG("polt->getTag: %d", types);
    m_now_showing_plot = false;
    if(types == 1) { // 战斗前的剧情
        plotEndStarGame();
    }else if(types == 2) { // 战斗胜利后的
        plotEndStarShowWin();
    }else if(types == 3) { // 战斗中
        //我方剧情忽略时，下面将直接被调了
        poltTalkEnd(types, polt->getTriggerEvent());
    }else if(types == 4){
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        SimpleAudioEngine::getInstance()->stopAllEffects();
#if !defined(LQ_DEMO_FIGHT)
        // 第一场战斗失败后直接就退出
        LoadBattlefield::getInstance()->combatCreateRole();
#else
        FiniteTimeAction* endcall = CallFunc::create([this](){
            auto scene = HelloWorld::createScene();
            auto director = Director::getInstance();
            director->replaceScene(scene);
        });
        this->runAction(Sequence::create(DelayTime::create(2), endcall, NULL));
#endif
    }
}

//为了跳过对话，将此提出为方法
void LQFightLayer::poltTalkEnd(int polttype, string triggerEvent)
{
    if (_plothero){  //引导怒气释放
        LQHeroButton* heroBtn = (LQHeroButton*)_plothero->getHeroButton();
        if (!heroBtn){
            this->plotSkillGuide(heroBtn);
            return;
        }
        //1 蒙层
        LayerColor* marklayer = LayerColor::create(Color4B(0, 0, 0, 0), mapSize.width, mapSize.height);
        marklayer->ignoreAnchorPointForPosition( false );
        marklayer->setTag(NODE_TAG_SKILLGUIDE);
        marklayer->setPosition(Point(mapSize.width/2,mapSize.height/2));
        auto _listener0 = EventListenerTouchOneByOne::create();
        _listener0->onTouchBegan = CC_CALLBACK_2(LQFightLayer::plotSkillTouchBegan, this);
        _listener0->setSwallowTouches(true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener0,marklayer);
        this->getParent()->addChild(marklayer,NODE_ZORDER_MAX);
        
        Point point1 = marklayer->convertToNodeSpace(heroBtn->getPosition())-Point(heroBtn->getContentSize().width*0.5,0);
        Sprite* sp1 = Sprite::createWithSpriteFrameName("yd_quanquan.png");
        point1 -= Point(sp1->getContentSize().width*0.1,0);
        sp1->setAnchorPoint(Point(0.5, 0.5));
        sp1->setPosition(point1+Point(0, sp1->getContentSize().height*0.15));
        marklayer->addChild(sp1,1);
        Sprite* sp2 = Sprite::createWithSpriteFrameName("yd_shouzhi.png");
        sp2->setAnchorPoint(Point(0.3, 1.0));
        sp2->setPosition(point1); //+Point(sp2->getContentSize().width*0.5, -sp2->getContentSize().height*0.4)
        RepeatForever* act1 = RepeatForever::create(Sequence::create(ScaleTo::create(0.2, 0.9),
                                                                     DelayTime::create(0.1),
                                                                     ScaleTo::create(0.28, 1),
                                                                     DelayTime::create(0.1),
                                                                     ScaleTo::create(0.32, 0.9),
                                                                     DelayTime::create(0.1),
                                                                     NULL));
        sp1->runAction(act1);
        RepeatForever* act2 = RepeatForever::create(Sequence::create(ScaleTo::create(0.2, 0.8),
                                                                     DelayTime::create(0.1),
                                                                     ScaleTo::create(0.28, 0.6),
                                                                     DelayTime::create(0.1),
                                                                     ScaleTo::create(0.32, 0.8),
                                                                     DelayTime::create(0.1),
                                                                     NULL));
        sp2->runAction(act2);
        marklayer->addChild(sp2,2);
        //妹子
        Point point2 = marklayer->convertToNodeSpace(heroBtn->getPosition());
        point2 = Point(10,point2.y+heroBtn->getContentSize().height*0.62);
        Sprite* sp0 = Sprite::createWithSpriteFrameName("xinshouzhiyin.png");
        sp0->setPosition(point2);
        sp0->setAnchorPoint(Point(0.0, 0.0));
        marklayer->addChild(sp0,2);
        Label* talklab = Label::createWithSystemFont("点击施放技能", "Helvetica", 32);
        talklab->enableShadow();
        talklab->setAnchorPoint(Point(0.0,0.5));
        talklab->setPosition(Point(sp0->getContentSize().width*0.5, sp0->getContentSize().height*0.22));
        sp0->addChild(talklab);
        
        //FiniteTimeAction* btncall = CallFuncN::create(CC_CALLBACK_1(LQFightLayer::plotSkillGuide,this));
        //plotSkillGuide();
    }else{
        if (triggerEvent.length()>0){
            CCLOG("有事件处理....");
            std::vector<std::string> attrvalue = split(triggerEvent.c_str(), ',');
            int trigHeroDr = atoi(attrvalue[0].c_str());
            int trigHero = atoi(attrvalue[1].c_str());
            Point trigPost = PointFromString(attrvalue[2]+","+attrvalue[3]);
            LQCombatSquadVector* squadVector;
            LQCombatSquad* squad;
            squadVector = trigHeroDr==1 ? m_mySquadVector : m_otherSquadVector;
            for (int i = 0; i < squadVector->size(); i++) {
                squad = squadVector->at(i);
                if (squad->getHero()->getData()->getResIDC()==trigHero){
                    //隐藏的出场，来个动画吧
                    squad->drawSquadPoint(trigPost);
                    squad->drawVisible(true);
                    squad->setIsOpenView(true);
                    if(trigHeroDr==1)
                        this->refushButtonPostion();
                }
            }
            
        }
        resumeGame();
    }
}

bool LQFightLayer::plotSkillTouchBegan(Touch *touch, Event *event)
{
    if (!_plothero || !_plothero->getHeroButton())
        return false;
    LQHeroButton* heroBtn = (LQHeroButton*)_plothero->getHeroButton();
    Point pt = heroBtn->convertTouchToNodeSpace(touch);
    //CCLOG("pt.x=%.1f pt.y=%.1f\n", pt.x, pt.y);
    
    int nw = heroBtn->getContentSize().width;
    int nh = heroBtn->getContentSize().height;
    Rect rc = Rect(0, 0, nw, nh);
    if(rc.containsPoint(pt))
    {
        //获得点击的OpenGL的世界坐标值
        //CCPoint touchPoint = touch->getLocation();
        //CCLOG("ccTouchBegan x=%.1f y=%.1f\n", touchPoint.x, touchPoint.y);
        this->plotSkillGuide(heroBtn);
        return true;
    }
    return true;
}

//技能剧情引导回调
void LQFightLayer::plotSkillGuide(Ref* pSender)
{
//    _plothero->getParent()->removeChildByTag(NODE_ZORDER_MASK);
//    _plothero->getParent()->removeChildByTag(NODE_ZORDER_MASK+1);
//    _plothero->getParent()->removeChildByTag(NODE_ZORDER_MASK+2);
    this->getParent()->removeChildByTag(NODE_TAG_SKILLGUIDE);
    
    if (_plothero->getHeroData()->getResIDC()==90041503 && _devilhero){
        _devilhero->getHeroData()->setHeroFuries(_devilhero->getHeroData()->getFuryMaxValue()-1100);
    }
    _plothero->reduceHeroFuries(30);
    resumeGame();
    _plothero->showSkill(_plothero->getHeroData()->getActiveSkillData());
}

void LQFightLayer::plotEndStarGame()
{
    // 启动update函数
    scheduleUpdate();
    
    this->runAction(Sequence::create(DelayTime::create(1.0),CallFunc::create([this](){
        beginStartGame();
        //AudioManager::getInstance()->playEffect(Effect_ArmyCharge_1);
    }), NULL));
}

string LQFightLayer::loadPlotPlayText(int idc)
{
    string playText = "";
    _triggerEvent = "";
    if(_fightType==0){
        LQOpeningInfoData* m_openData = (LQOpeningInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQOpeningInfoData::getAliasName(),idc);
        playText = m_openData->getPlayText();
    }else{
        LQGutInfoData* m_gutData = (LQGutInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQGutInfoData::getAliasName(),idc);
        playText = m_gutData->getPlayText();
        _triggerEvent = m_gutData->getTriggerEvent();
    }
    return playText;
}
// 新手结束后发消息
void LQFightLayer::plotEndStarShowWin()
{
    int recStar = LQBattleSession::getInstance()->getResStar();
    LQBattleSession::getInstance()->sendResultMsg(true,recStar);
}
//显示战斗胜利界面
void LQFightLayer::showWinLayer()
{
#if !defined(LQ_DEMO_FIGHT)
    int m_fightType = LQBattleSession::getInstance()->m_fightType;
    LQPVEWinLayer*layer = LQPVEWinLayer::createLayer();
    this->getParent()->removeChildByTag(NODE_TAG_PVEWIN);
    layer->setTag(NODE_TAG_PVEWIN);
    if (m_fightType == 5) {
        layer->initUI(m_gameWinValue);
    }
    else if (m_fightType == 8) //南蛮入侵
    {
        layer->initUI();
    }
    else
    {
        layer->initUI();
        layer->initAwardItem(m_gameWinValue,m_recStar);
    }
    this->getParent()->addChild(layer,NODE_ZORDER_DIALOG);
#endif
}

void LQFightLayer::beginStartGame()
{
    //触发所有的被动技能
    LQBattleSession::getInstance()->releasePassiveSkill();
    
    //开始冲锋
    LQBattleSession::getInstance()->startGame();
    bool isCanAuto = LQBattleSession::getInstance()->getIsCanAutoFight();
    bool isAutoGuide = LQBattleSession::getInstance()->getAutoFightGuide();
    if (isAutoGuide && isCanAuto && _fightType == 1) //自动战斗引导
    {
        UserDefault::getInstance()->setBoolForKey("FIGHTAUTO",false);
        pauseGame();
        auto guideLayer = BattleGuideLayer::create();
        guideLayer->setTag(NODE_TAG_GUIDE);
        this->getParent()->addChild(guideLayer,NODE_TAG_GUIDE);
    }
}

//
void LQFightLayer::itemTouch(Ref * sender, Control::EventType controlEvent){
    if(m_isGameOver)
    {
        return;
    }
    ControlButton* button = (ControlButton*)sender;
    if (!button) {
        return;
    }
    int tag = button->getTag();
    switch (tag) {
        case KQuickButtonTagNumber://快进
        {
            OnSpeedButttonClicked(button);
        }
            break;
        case KAutoButtonTagNumber://自动战斗
        {
#if !defined(LQ_DEMO_FIGHT)
            if (_fightType==3) break;
#endif
            if (m_pauseLayer->isVisible()) {
                break;
            }
            bool isAutoGuide = LQBattleSession::getInstance()->getAutoFightGuide();
            if (isAutoGuide && _fightType == 1)  //清除自动引导层
            {
                this->getParent()->removeChildByTag(NODE_TAG_GUIDE);
                resumeGame();
            }
            OnAutoButtonClicked(button);
        }
            break;
        case KReBackButtonTagNumber://撤退
        {
#if !defined(LQ_DEMO_FIGHT)
            bool isreplay = LQBattleSession::getInstance()->getIsReplay();
            if ((_fightType==3 || _fightType==0) && !isreplay) break;
            if (m_pauseLayer->isVisible()) {
                LoadBattlefield::getInstance()->combatBack();
            }
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            SimpleAudioEngine::getInstance()->stopAllEffects();
#else
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            SimpleAudioEngine::getInstance()->stopAllEffects();
            auto scene = HelloWorld::createScene();
            auto director = Director::getInstance();
            director->replaceScene(scene);
#endif
        }
            break;
        case KPauseButtonTagNumber://暂停
        {
            if (_fightType==0) break;
            if (m_pauseLayer->isVisible()) {
                break;
            }
            pauseGame();
            
            if (m_pauseLayer) {
                m_pauseLayer->setVisible(true);
            }
        }
            break;
        case KContinueButtonTagNumber://继续战斗
        {
            if (m_pauseLayer) {
                m_pauseLayer->setVisible(false);
            }
            resumeGame();
        }
            break;
        default:
            break;
    }
}

bool LQFightLayer::isPausedGame()
{
    return Director::getInstance()->isPaused() || _isPausedLayer;
}

void LQFightLayer::pauseGame()
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL2)
        CCLOG("pauseGame....");
    _isPausedLayer = true;
    
    _anodes = Director::getInstance()->getActionManager()->pauseAllRunningActions();
}

void LQFightLayer::resumeGame()
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL2)
        CCLOG("resumeGame....");
    //Director::getInstance()->resume();
    Director::getInstance()->getActionManager()->resumeTargets(_anodes);
    for (auto& child : m_sprite->getChildren())
    {
        if(child)
            Director::getInstance()->getActionManager()->resumeTarget(child);
    }
    _isPausedLayer = false;
}

void LQFightLayer::OnIconButtonClicked(Ref * sender, Control::EventType controlEvent)
{
    if(m_isGameOver)
    {
        return;
    }
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL2)
        CCLOG("OnIconButtonClicked 1.... showingskill:%d", m_now_showing_skill);
    
    if(m_now_showing_skill || m_now_showing_plot)
        return;
    
    LQHeroButton* button = (LQHeroButton*)sender;
    if (!button->getIsCanFreeSkill()) {
        return;
    }
    
    //手动操作 释放主动技能
    //LQHeroUserData* heroData = (LQHeroUserData*)button->getHeroData();
    LQCombatHero* hero = button->getHero();
    if (hero && hero->isCanFreeActiveSkill()) {
        m_now_showing_skill = true;
        m_time_showing_skill = 0;
        if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL2)
            CCLOG("OnIconButtonClicked 2.... showingskill:%d", m_now_showing_skill);
        hero->showSkill(hero->getHeroData()->getActiveSkillData());
        //skillShowDescription(hero, 1);
    }
}

//自动
void LQFightLayer::OnAutoButtonClicked(ControlButton *sender)
{
    //log("~~~~~~~~~~OnAutoButtonClicked");
    bool isAuto = LQBattleSession::getInstance()->getIsAutoFight();
    LQBattleSession::getInstance()->setIsAutoFight(!isAuto);
    Sprite* hightSp = (Sprite*)sender->getChildByTag(KButtonHightLightSpriteTagNumber);
    if (hightSp) {
        hightSp->setVisible(LQBattleSession::getInstance()->getIsAutoFight());
    }
}

//暂停
void LQFightLayer::OnPauseButtonClicked(ControlButton *sender)
{
    //log("~~~~~~~~~~OnPauseButtonClicked");
    Sprite* hightSp = (Sprite*)sender->getChildByTag(KButtonHightLightSpriteTagNumber);
    if (hightSp) {
        hightSp->setVisible(!isPausedGame());
    }
}

//加速
void LQFightLayer::OnSpeedButttonClicked(ControlButton *sender)
{
    //ControlButton* button = (ControlButton*)sender;
    if (m_pauseLayer->isVisible()) {
        return;
    }
}

//点击界面，释放技能，现在已经没有使用了。
void LQFightLayer::iconButtonResponse(Touch* pTouch)
{
    /*
    if (m_isGameOver) {
        return;
    }
    if (LQBattleSession::getInstance()->getIsAutoFight()) {
        return;
    }
    if (m_pauseLayer->isVisible()) {
        return;
    }
    //Point touchpoint = m_sprite->convertToNodeSpace(pTouch->getLocation());
    LQHeroUserData* heroData = (LQHeroUserData*)m_currentSkillButton->getHeroData();
    if (heroData && heroData->isCanFreeActiveSkill()) {
        LQCombatHero* hero = m_currentSkillButton->getHero();
        
        hero->showSkill(heroData->getActiveSkillData(),klqSkillShowType_ActiveSkill);  //touchpoint
        showSkillMaskLayer(pTouch->getLocation());
    }
    m_currentSkillButton->setSelected(false);
    m_currentSkillButton = NULL;
    pauseGame();*/
}

//显示技能遮罩层
void LQFightLayer::showSkillMaskLayer(Point touchPoint, float scale)
{
    if (!m_maskLayer) {
        m_maskLayer = MaskLayer::createMaskLayer();
        this->getParent()->addChild(m_maskLayer,NODE_ZORDER_GUT-1);
    }
    m_maskLayer->stopAllActions();
    m_maskLayer->setVisible(true);
    m_maskLayer->setStencilSprite("res/stencil_round.png");
    m_maskLayer->setRectPoint(touchPoint);
    Sequence* action = Sequence::create(DelayTime::create(1),CallFunc::create([this](){
        this->hadeSkillMaskLayer();
    }), NULL);
    m_maskLayer->runAction(action);
}

//游戏开始，打开云朵
void LQFightLayer::gameBegin()
{
    float scale = 3.0;
    float delaytime = 0.5;
    Sprite* c1 = Sprite::create("res/level/cloud_battle.png"); //左下
    c1->setAnchorPoint(Point(1,1));
    c1->setPosition(Point(visibleSize.width*0.7,visibleSize.height*0.7));
    c1->setScale(scale);
#if defined(LQ_DEMO_FIGHT)
    this->addChild(c1,NODE_ZORDER_CLOUD);
#else
    this->getParent()->addChild(c1,NODE_ZORDER_CLOUD);
#endif
    c1->runAction(Sequence::create(MoveTo::create(delaytime,Point(visibleSize.width*0,visibleSize.height*0)),RemoveSelf::create(), NULL));
    
    Sprite* c2 =  Sprite::create("res/level/cloud_battle.png"); //--右下
    c2->setAnchorPoint(Point(0,1));
    c2->setPosition(Point(visibleSize.width*0.3,visibleSize.height*0.7));
    c2->setScale(scale);
#if defined(LQ_DEMO_FIGHT)
    this->addChild(c2,NODE_ZORDER_CLOUD);
#else
    this->getParent()->addChild(c2,NODE_ZORDER_CLOUD);
#endif
    c2->runAction(Sequence::create(MoveTo::create(delaytime,Point(visibleSize.width*1,visibleSize.height*0)),RemoveSelf::create(), NULL));
    
    Sprite* c3 = Sprite::create("res/level/cloud_battle.png");// --左上
    c3->setAnchorPoint(Point(1,0));
    c3->setPosition(Point(visibleSize.width*0.7,visibleSize.height*0.3));
    c3->setScale(scale);
#if defined(LQ_DEMO_FIGHT)
    this->addChild(c3,NODE_ZORDER_CLOUD);
#else
    this->getParent()->addChild(c3,NODE_ZORDER_CLOUD);
#endif
    c3->runAction( Sequence::create(MoveTo::create(delaytime,Point(visibleSize.width*0,visibleSize.height*1)),RemoveSelf::create(), NULL));
    
    Sprite* c4 = Sprite::create("res/level/cloud_battle.png"); //--右上
    c4->setAnchorPoint(Point(0,0));
    c4->setPosition(Point(visibleSize.width*0.3,visibleSize.height*0.3));
    c4->setScale(scale);
#if defined(LQ_DEMO_FIGHT)
    this->addChild(c4,NODE_ZORDER_CLOUD);
#else
    this->getParent()->addChild(c4,NODE_ZORDER_CLOUD);
#endif
    c4->runAction( Sequence::create(MoveTo::create(delaytime,Point(visibleSize.width*1,visibleSize.height*1)),RemoveSelf::create(), NULL));
    
    //注册事件
    LQBattleSession::getInstance()->onStartBattleCallback =  CC_CALLBACK_1(LQFightLayer::doStartBattleCallback,this);
}
//南蛮入侵战斗结束
void LQFightLayer::gameOver(bool win,LQHeroUserDataVector it,string str)
{
    if( _DEBUG_TYPE==_DEBUG_TYPE_SAFE )
    
    m_isGameOver = true;
    m_recStar = 0;
    m_gameWinValue = str;
    this->m_win = win;  //记录胜利失败
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
#if !defined(LQ_DEMO_FIGHT)
    this->showNetPrompt(false);
    LQPVEWinLayer*layer = LQPVEWinLayer::createLayer();
    this->getParent()->removeChildByTag(NODE_TAG_PVEWIN);
    layer->setTag(NODE_TAG_PVEWIN);
    layer->initBossUI(it,m_gameWinValue);
    this->getParent()->addChild(layer,NODE_ZORDER_DIALOG);
#endif

}
//战斗结束
void LQFightLayer::gameOver(bool win,int recStar, string awardItem)
{
    if( _DEBUG_TYPE==_DEBUG_TYPE_SAFE )
        CCLOG("gameOver win %d recStar:%d awardItem:%s", win, recStar, awardItem.c_str());
    m_isGameOver = true;
    m_recStar = recStar;
    this->m_win = win;  //记录胜利失败
    
    this->showNetPrompt(false);

    //
    if(_fightType == 0) {
        //this->playPoltLayer(40399006,4,false);
        
        LQPoltLayer*layer = LQPoltLayer::createLayer();
        //if(layer->getIsThereAre(40399006))
        { // 剧情没有播放过  开始播放
            //LQOpeningInfoData* m_gutData = (LQOpeningInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQOpeningInfoData::getAliasName(),40399006);
            layer->setPolt(loadPlotPlayText(40399006));
            layer->setTag(4);
            layer->setDelegate(this);
            layer->setPosition(layer->getPositionX(),layer->getPositionY()-(visibleSize.height-683)/2);
            this->getParent()->addChild(layer,NODE_ZORDER_GUT);
            _plothero = NULL;
            return;
        }
        return;
    }else{
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        SimpleAudioEngine::getInstance()->stopAllEffects();
    }
    
#if !defined(LQ_DEMO_FIGHT)
    if (win) {
        //战斗胜利音效
        AudioManager::getInstance()->playEffect(Effect_Win_Music1);
        
        m_gameWinValue = awardItem;
        // --------------------------------------------------
        showWinLayer();
        
    }else{
        int m_fightType = LQBattleSession::getInstance()->m_fightType;
        //战斗失败音效
        AudioManager::getInstance()->playEffect(Effect_Lost_Music1);

        // 战斗后失败后弹出失败界面
        LQPVELoseLayer*layer = LQPVELoseLayer::createLayer();
        this->getParent()->removeChildByTag(NODE_TAG_PVELOST);
        layer->setTag(NODE_TAG_PVELOST);
        layer->setFightType(m_fightType);
        this->getParent()->addChild(layer,NODE_ZORDER_DIALOG);
        if (m_fightType == 5)
        {
            if(awardItem == "")
            {
                layer->initUI("");
            }
            else
            {
                layer->initUI();
            }
            
        
        }
        else if (m_fightType == 6)  //奇遇  失败带奖励
        {
            layer->initUI();
            layer->initAwardItem(awardItem,recStar);
        }
        else
            layer->initUI();
        
    }
    
#else
    FiniteTimeAction* endcall = CallFunc::create([this](){
        auto scene = HelloWorld::createScene();
        auto director = Director::getInstance();
        director->replaceScene(scene);
    });
    this->runAction(Sequence::create(DelayTime::create(2), endcall, NULL));
#endif
    //释放
    //SimpleAudioEngine::getInstance()->end();

    
}
//显示胜利后的剧情
void LQFightLayer::showPolt()
{
    CCLOG("showPolt");
    int zoneID = LQBattleSession::getInstance()->getZoneID();
    int m_fightType = LQBattleSession::getInstance()->m_fightType;
    if(m_fightType != 1 ) {
        // 不是普通关卡不播放剧情
        this->plotEndStarShowWin();
        return;
    }
    int pkey = (zoneID/1000)%10+1;
    LQZoneInfoData* m_zoneData = (LQZoneInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName("ZoneInfo",pkey,zoneID);
    if(m_zoneData == NULL) {
        // 没有得到场景对象;  直接战斗  没剧情
        plotEndStarShowWin();
    }else{
        // 得到场景了
        int endStoryID = m_zoneData->getEndStoryID();
        if(endStoryID == 0) { // 没有剧情
            plotEndStarShowWin();
        }else{ // 有剧情
            LQPoltLayer*layer = LQPoltLayer::createLayer();
            if(layer->getIsThereAre(endStoryID)){ // 剧情没有播放过  开始播放
                //LQOpeningInfoData* m_gutData = (LQOpeningInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQOpeningInfoData::getAliasName(),endStoryID);
                layer->setPolt(this->loadPlotPlayText(endStoryID));
                layer->setTag(2);
                layer->setDelegate(this);
                layer->setPosition(layer->getPositionX(),layer->getPositionY()-(visibleSize.height-683)/2);
                this->getParent()->addChild(layer,NODE_ZORDER_GUT);
            }else{ // 剧情被播放过了 so...
                plotEndStarShowWin();
            }
        }
    }
}
//游戏 startBattle回调
void LQFightLayer::doStartBattleCallback(Ref* pSender)
{
    //开战前，拉近视图
    //doSceneScale(1.0f,1.0f);
    
    //开始战斗放战场背景01
    if (LQBattleSession::getInstance()->getIsHasMelee())
        m_meleeEffect = AudioManager::getInstance()->playEffect(Effect_BG1_Music, true);
}

void LQFightLayer::getMessageToFightHorse()
{
#if !defined(LQ_DEMO_FIGHT)
    string fileStr = LoadBattlefield::getInstance()->getValue();
    if( _DEBUG_TYPE==_DEBUG_TYPE_VALUE )
        CCLOG("收到数据%s",fileStr.c_str());
    if (this->m_win)
    {
        auto child = (LQPVEWinLayer*)(this->getParent()->getChildByTag(NODE_TAG_PVEWIN));
        if (child)
        {
            child->setDataToUI(fileStr);
        }
    }else
    {
        auto child = (LQPVELoseLayer*)(this->getParent()->getChildByTag(NODE_TAG_PVELOST));
        child->setDataToUI(fileStr);
    }
#endif
}

void LQFightLayer::hadeSkillMaskLayer()
{
    if (m_maskLayer){
        m_maskLayer->setVisible(false);
    }
}

//振屏
void LQFightLayer::shakeScene()
{
    if (m_shakeLayer) {
        m_shakeLayer->stopAllActions();
        m_shakeLayer->setPosition(Point(0, 0));
        
        float _xx = 12;
        ActionInterval* move1 = MoveBy::create(0.02,Point(-_xx,_xx));
        ActionInterval* move2 = MoveBy::create(0.02,Point(-_xx,-_xx));
        ActionInterval* move3 = MoveBy::create(0.03,Point(_xx,_xx));
        ActionInterval* move4 = MoveBy::create(0.02,Point(-_xx,-_xx));
        ActionInterval* move5 = MoveBy::create(0.02,Point(_xx,-_xx));
        ActionInterval* move6 = MoveBy::create(0.03,Point(-_xx,_xx));
        ActionInterval* moves = Sequence::create(move1, move1->reverse(),
                                                 move2, move2->reverse(),
                                                 move3, move3->reverse(),
                                                 move4, move4->reverse(),
                                                 move5, move5->reverse(),
                                                 move6, move6->reverse(), NULL);
        m_shakeLayer->runAction(moves);
    }
   
}

bool LQFightLayer::isTouchInside(Node* pChild, Touch* pTouch)
{
    Point touchLocation = pTouch->getLocation();
    
    Point local = pChild->convertToNodeSpace(touchLocation);
    Point _obPosition = pChild->getPosition();
    Size _obContentSize = pChild->getContentSize();
    Point _obAnchorPoint = pChild->getAnchorPoint();
    Rect r = Rect( _obPosition.x - _obContentSize.width * _obAnchorPoint.x,
                  _obPosition.y - _obContentSize.height * _obAnchorPoint.y,
                  _obContentSize.width, _obContentSize.height);
    //CCRect r = pChild->rect();
    r.origin = Point::ZERO;
    
    if (r.containsPoint(local))
        return true;
    
    return false;
}


//测试格子显示
void LQFightLayer::testSpritexr(SpriteBatchNode* parent, int x,int y) {
    //Sprite* shadow = Sprite::createWithSpriteFrameName("Shadow.png");
    //Sprite* shadow = Sprite::create("xxx.png");
    Sprite* shadow = Sprite::createWithTexture(parent->getTexture());
    shadow->setPosition(convertPoint(x,y));
    shadow->setOpacity(100);
    parent->addChild(shadow,1000);
    string str;
    stringf(str, "%d,%d", x, y);
    auto ttf0 = LabelTTF::create(str.c_str(), "Helvetica", 11);
    ttf0->setColor(Color3B::BLUE);
    ttf0->setPosition(convertPoint(x,y));
    //ttf0->setPosition(Point(shadow->getContentSize().width/2, shadow->getContentSize().height/2));
    m_sprite->addChild(ttf0, 100);

}

void LQFightLayer::update(float dt)
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_TIME)
        CCLOG( "LQFightLayer::update %ld", getCurrentMillSecond() );
    
    if (m_pauseLayer->isVisible())
        return;
    
    //暂停不影响技能施放者的动作继续
    if(m_now_showing_skill){
        m_time_showing_skill += dt;
        if(m_time_showing_skill>7.0){ //只能修正可能带来的技能释放
            m_now_showing_skill = false;
            m_time_showing_skill = 0;
            _skillhero = NULL;
            _effectdata = NULL;
            _skillcallback = NULL;
            resumeGame();
        }
    }
    if(_skillhero)
        _skillhero->updateSkill(dt, this);
        
    if( this->isPausedGame() )
        return;
    
    LQMissileManager::sharedMissileManager()->update(dt);
    LQBattleSession::getInstance()->update(dt, this);
    AudioManager::getInstance()->updataEffectCount(dt);
    
    if(!m_isGameOver && _fightType==8 && _furiesTimeParam>0 && _furiesTime>_furiesTimeParam){  //南蛮打鼓怒气
        _furiesTime = 0;
        for (int i = 0; i < m_mySquadVector->size(); i++) {
            if(m_mySquadVector->at(i)->getHero()){
                LQCombatHero* hero = m_mySquadVector->at(i)->getHero();
                hero->getHeroData()->addHeroFuriesRadio(_furiesAddRadio);
            }
        }
    }else _furiesTime += dt;
    
    //到了某个剧情时间
    if (_storyIDTime>0 && _timeStoryID>0){
        if (LQBattleSession::getInstance()->getBattleTimes()>_storyIDTime){
            _storyIDTime = 0;
            this->playPoltLayer(_timeStoryID,3);
            /*
            LQPoltLayer*layer = LQPoltLayer::createLayer();
            //if(layer->getIsThereAre(_timeStoryID))
            { // 剧情没有播放过  开始播放
                pauseGame();
                m_now_showing_plot = true;
                layer->setTag(3);
                layer->setDelegate(this);
                layer->setPolt(this->loadPlotPlayText(_timeStoryID));
                layer->setPosition(layer->getPositionX(),layer->getPositionY()-(visibleSize.height-683)/2);
                this->getParent()->addChild(layer,NODE_ZORDER_GUT);
            }*/
        }
    }
    
    if (m_meleeEffect>0 && !LQBattleSession::getInstance()->getIsHasMelee()){
        AudioManager::getInstance()->pauseEffect(m_meleeEffect);
        //m_meleeEffect = 0;
    }
    /*
    m_myHpSlider->setValue(LQBattleSession::getInstance()->getLeftSquadHP());
    Label* hpvalue = (Label *)m_myHpSlider->getChildByTag(KHpValueLabelTagNumber);
    if (hpvalue) {
        string hpStr;
        stringf(hpStr, "%d/%d",LQBattleSession::getInstance()->getLeftSquadHP(),(int)m_myHpSlider->getMaximumValue());
        hpvalue->setString(hpStr);
    }*/
    //8.12  boss血量分层
    int maxHp = ceil(LQBattleSession::getInstance()->getRightSquadHPMax()/3);
    for (int i = 0;i<3;i++)
    {
        if (LQBattleSession::getInstance()->getRightSquadHP() > maxHp * (i + 1))
        {
            if (m_otherHpSlider[i]){
                m_otherHpSlider[i]->setValue(maxHp);
            }
            spriteHp[i]->setVisible(true);
            m_otherHpSlider[i]->setVisible(true);
        }
        else if (LQBattleSession::getInstance()->getRightSquadHP() > maxHp * i)
        {
            int value = LQBattleSession::getInstance()->getRightSquadHP() - maxHp * i;
            m_otherHpSlider[i]->setValue(value);
         
          
            spriteHp[i]->setVisible(true);

            m_otherHpSlider[i]->setVisible(true);
        }
        else
        {
            m_otherHpSlider[i]->setValue(0);
            m_otherHpSlider[i]->setVisible(false);
            spriteHp[i]->setVisible(false);
        }
    }
    /*
    //4.6号增加BOSS血量
    if (m_otherHpSlider[0]){
        m_otherHpSlider[0]->setValue(LQBattleSession::getInstance()->getRightSquadHP());
        
    
    }
    */
    
    m_timeSp->setString(intToTimeString(DEF_BATTLE_TIMEMAX-LQBattleSession::getInstance()->getBattleTimes()));
    
    for (int i = 0; i < m_skillButtonVector->size(); i++) {
        m_skillButtonVector->at(i)->updataButton();
    }
    
    //自动战斗时己方自动释放技能，否则手动释放；敌方不管是否自动战斗，都自动释放技能
    skillRelease();
}

#pragma Touches
//多点响应
void LQFightLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
//    if(Director::getInstance()->isPaused())
//        Director::getInstance()->resume();
    
    m_beginPinchLength = 0;
    
    if (m_isGameOver) {
        m_currentSkillButton = NULL;
        return;
    }
#if defined(LQ_DEMO_FIGHT)
    if (touches.size() == 1) {
        //log("touchPoint ===(%f,%f)",touches.at(0)->getLocation().x,touches.at(0)->getLocation().y);
        //普通的功能按钮
        for (int i = 0; i < m_normalButtonVector->size(); i++) {
            ControlButton* button = m_normalButtonVector->at(i);
            if (button && isTouchInside(button, touches.at(0))) {
                itemTouch(button, Control::EventType::TOUCH_UP_INSIDE);
                return;
            }
        }
        for (int i = 0; i < m_skillButtonVector->size(); i++) {
            ControlButton* button = m_skillButtonVector->at(i);
            if (button && isTouchInside(button, touches.at(0))) {
                OnIconButtonClicked(button, Control::EventType::TOUCH_UP_INSIDE);
                return;
            }
        }
    }
#endif
    
    if (m_pauseLayer->isVisible()) {
        return;
    }
   
    moveTouchID = -1;
    if(touches.size()>=2){
        isMultipleTouch = true;
        lastScale = this->getScale();
        Point location = (Point)touches.at(0)->getLocation();
        m_beginPinchLength = location.getDistance(touches.at(1)->getLocation());
    }
}

void LQFightLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    if(_skillhero)
        return;
    
    if( _DEBUG_TYPE==_DEBUG_TYPE_MOVE )
        CCLOG("onTouchesMoved: %lu", touches.size());
    
    if (m_isGameOver || isSceneScaleing) {
        return;
    }
    float pinchLength = 0;
    for( auto &item: touches)
    {
        auto touch = item;
        auto location = touch->getLocation();
        for( auto &item2: touches)
        {
            auto touch2 = item2;
            auto location2 = touch2->getLocation();
            float len;
            if(touch->getID()!=touch2->getID()){
                len = location.getDistance(location2);
                pinchLength = MAX(pinchLength,len);
            }
        }
        //CCLOG("a2: %f,%f", location.x, location.y);
    }
    if( _DEBUG_TYPE==_DEBUG_TYPE_MOVE )
        CCLOG("onTouchesMoved: %f,%f", pinchLength, m_beginPinchLength );
    if(m_beginPinchLength==0){
        m_beginPinchLength = pinchLength;
        lastScale = this->getScale();
    }else{
        if(pinchLength>0 && m_beginPinchLength>0){
            float scale = pinchLength/m_beginPinchLength;
            doHandlePinchFrom(scale);
        }
    }
    
    //移动屏幕
    if(touches.size()==1){
        auto touch = touches.at(0);
        if(moveTouchID != touch->getID()){
            moveTouchID = touch->getID();
            m_oldPoint = touch->getStartLocation();
        }
        Point pt = touch->getLocation()-m_oldPoint;
        if( _DEBUG_TYPE==_DEBUG_TYPE_MOVE )
        CCLOG("Loc: %f,%f -- %f,%f = %f,%f", touch->getStartLocation().x, touch->getStartLocation().y, touch->getLocation().x, touch->getLocation().y, pt.x, pt.y);
        m_oldPoint = touch->getLocation();
        doHandleMoveFrom(pt);
    }
}

void LQFightLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    if (m_isGameOver) {
        return;
    }
    //只有当前有技能按钮才需要处理
    if (m_currentSkillButton) {
       iconButtonResponse(touches.at(0));
       return;
    }
    m_beginPinchLength = 0;
    isMultipleTouch = false;
}

//操作移动
void LQFightLayer::doHandleMoveFrom(Point pt)
{
    if(isMultipleTouch)
        return;
    Point pos = getPosition() + pt;
    if( _DEBUG_TYPE==_DEBUG_TYPE_MOVE ) CCLOG("pos1: %f,%f", pos.x, pos.y);
    pos.x = MAX(pos.x, -mapMinWidth);
    pos.x = MIN(pos.x, mapMinWidth);
    pos.y = MAX(pos.y, -mapMinHeight);
    pos.y = MIN(pos.y, mapMinHeight);
    if( _DEBUG_TYPE==_DEBUG_TYPE_MOVE ) CCLOG("pos2: %f,%f", pos.x, pos.y);
    setPosition(pos);
}

//手势操作放缩移动
void LQFightLayer::doHandlePinchFrom(float scale)
{
    //lastScale就代表我们进行一次新的pinch手势之前的图层的sacle
    //recognizer.scale是pinch手势的scale值
    //nowScale = (lastScale - 1) + recognizer.scale;    //4
    nowScale = lastScale*scale;//(lastScale - 1) + scale;
    nowScale = MIN(nowScale,LQFightLayer::LAYER_SCALEMAX);//设置缩放上限   //5
    nowScale = MAX(nowScale,LQFightLayer::LAYER_SCALEMIN);//设置缩放下限   //6
    doSceneScale(nowScale,0.0f);
}

//执行缩放操作，传入非0有风险
void LQFightLayer::doSceneScale(float nowScale,float time){
    if( _DEBUG_TYPE==_DEBUG_TYPE_MOVE ) CCLOG("nowScale: %f", nowScale);
    //-1.得到移动允许的范围
    
    //-2.添加缩小时的处理
    if(time == 0){//如果传入0，那么即时释放
        this->setScale(nowScale);//7
        this->scaleDrawmArmflags();
    }else{//如果传入时间不是0，那么渐进缩放,传入非0有风险，如果正在缩放过程中被暂停，那么有极高概率会崩
        isSceneScaleing  = true;
        //FiniteTimeAction* endcall = CallFunc::create([this](){
        //    isSceneScaleing = false;
        //});
        FiniteTimeAction* endcall = CallFuncN::create(CC_CALLBACK_1(LQFightLayer::scaleToCallback,this));
        this->runAction(Sequence::create(ScaleTo::create(time, nowScale), endcall, NULL));
    }
    /*auto ttf0 = (Label*)this->getChildByTag(1020);
     if(ttf0){
     string str;
     stringf(str, "Scale:%02f", nowScale);
     //ttf0->setString(str.c_str());
     }*/
    
    mapMinWidth = (mapSize.width*nowScale-visibleSize.width)/2;
    mapMinHeight = (mapSize.height*nowScale-visibleSize.height)/2;
    
    Point pos = getPosition();
    pos.x = MAX(pos.x, -mapMinWidth);
    pos.x = MIN(pos.x, mapMinWidth);
    pos.y = MAX(pos.y, -mapMinHeight);
    pos.y = MIN(pos.y, mapMinHeight);
    setPosition(pos);
    
}

//scaleTo触发数据动作
void LQFightLayer::scaleToCallback(Ref* pSender)
{
    isSceneScaleing = false;
    //this->scaleDrawmArmflags();
}

void LQFightLayer::scaleDrawmArmflags()
{
    LQCombatSquad* squad;
    for (int i = 0; i < squads.size(); i++) {
        squad = squads.at(i);
        if(squad && squad->getActive())
            squad->drawmArmflags((1/nowScale)*LQFightLayer::LAYER_SCALEMIN);
    }
}

//通过刷帧，将满足条件的英雄技能放出来，每一个帧只能触发一个技能触发，同时会暂停其他动作
void LQFightLayer::skillRelease()
{
    if(m_now_showing_skill)
        return;
    
    //当特定的武将怒气满时将触发引导或者剧情 TODO
    LQCombatHero* hero;
    LQCombatSquad* squad;
    for (int i = 0; i < squads.size(); i++) {
        squad = squads.at(i);
        if(squad->getHero()){
            hero = squad->getHero();
            int storyID = -1;
            if (hero->getStoryIDFuries()>0 && hero->isCanFreeActiveSkill())
            {
                //怒气满足 getHeroStoryIDFuries
                //pauseGame();
                storyID = hero->getStoryIDFuries();
                hero->setStoryIDFuries(0);
                hero->setFuriesPilot(true);
                _plothero = hero;
            }else if ( hero->getStoryIDHP()>0 && squad->getHPRate()<=hero->getStoryHPRate() ){
                //HP满足  getHeroStoryIDHP
                CCLOG(" %d %d", squad->getHPRate(), hero->getStoryHPRate());
                //pauseGame();
                storyID = hero->getStoryIDHP();
                hero->setStoryIDHP(0);
            }
            
            //1.11时的更改，注调下面3行就可以出剧情
            //if (storyID>0 && hero->getSquadDirection()==klqCombatSquadDirectionLeft){
            //    poltTalkEnd(3);
            //    storyID = 0;
            //}
            
            if (storyID>0){
                if (this->playPoltLayer(storyID,3,_fightType!=0))
                    return;
//                m_now_showing_plot = true;
//                LQPoltLayer*layer = LQPoltLayer::createLayer();
//                //if(layer->getIsThereAre(storyID))
//                { // 剧情没有播放过  开始播放
//                    //LQOpeningInfoData* m_gutData = (LQOpeningInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQOpeningInfoData::getAliasName(),storyID);
//                    layer->setPolt(this->loadPlotPlayText(storyID));
//                    layer->setTag(3);
//                    layer->setDelegate(this);
//                    layer->setPosition(layer->getPositionX(),layer->getPositionY()-(visibleSize.height-683)/2);
//                    this->getParent()->addChild(layer,NODE_ZORDER_GUT);
//                    return;
//                }
            }
        }
    }
    
    bool isAuto = LQBattleSession::getInstance()->getIsAutoFight();
    if(isAuto){//己方英雄放技能
        for (int i = 0; i < m_mySquadVector->size(); i++) {
            if(m_mySquadVector->at(i)->getHero()){
                hero = m_mySquadVector->at(i)->getHero();
                if (hero->getStopMove())
                    continue;
                //释放被动技能
                hero->showPassiveSkill();
                //释放主动技能前提：怒气值足够；是否自动战斗
                if (hero->isCanFreeActiveSkill())
                {
                    //CCLOG(" Hero::释放主动技能this = %p ,faxFuries = %d, heroFuries = %d",this, m_herodata->getFuryMaxValue(), m_herodata->getHeroFuries());
                    m_now_showing_skill = true;
                    m_time_showing_skill = 0;
                    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL2)
                        CCLOG("skillRelease 己方英雄放技能.... showingskill:%d", m_now_showing_skill);
                    hero->showSkill(hero->getHeroData()->getActiveSkillData());
                    //skillShowDescription(hero, 1);
                    
                    return;  //一帧只让一个英雄放技能
                }
            }
        }
    }
    //敌方英雄放技能
    if(true){
        for (int i = 0; i < m_otherSquadVector->size(); i++) {
            if(m_otherSquadVector->at(i)->getHero()){
                hero = m_otherSquadVector->at(i)->getHero();
                if (hero->getStopMove())
                    continue;
                //释放被动技能
                hero->showPassiveSkill();
                //释放主动技能前提：怒气值足够；是否自动战斗
                if (hero->isCanFreeActiveSkill())
                {
                    //CCLOG(" Hero::释放主动技能this = %p ,faxFuries = %d, heroFuries = %d",this, m_herodata->getFuryMaxValue(), m_herodata->getHeroFuries());
                    m_now_showing_skill = true;
                    m_time_showing_skill = 0;
                    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL2)
                        CCLOG("skillRelease 敌方英雄放技能....IDX:%d showingskill:%d", hero->getIndex(), m_now_showing_skill);
                    hero->showSkill(hero->getHeroData()->getActiveSkillData());
                    //skillShowDescription(hero, 1);
                    return;  //一帧只让一个英雄放技能
                }
            }
        }
    }
    //
}

bool LQFightLayer::playPoltLayer(int storyID,int tag,bool isneedPass)
{
    if (storyID<=0)
        return false;
    LQPoltLayer*layer = LQPoltLayer::createLayer();
    if(layer->getIsThereAre(storyID) || !isneedPass)
    { // 剧情没有播放过  开始播放
        m_now_showing_plot = true;
        //LQOpeningInfoData* m_gutData = (LQOpeningInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQOpeningInfoData::getAliasName(),storyID);
        pauseGame();
        layer->setPolt(this->loadPlotPlayText(storyID));
        layer->setTriggerEvent(_triggerEvent); //随便处理一下吧
        layer->setTag(3);
        layer->setDelegate(this);
        layer->setPosition(layer->getPositionX(),layer->getPositionY()-(visibleSize.height-683)/2);
        this->getParent()->addChild(layer,NODE_ZORDER_GUT);
        return true;
    }
    return false;
}

//Hero CallBack方法
//英雄死了回调
void LQFightLayer::heroDiedCallback(Node* pSender,LQBaseUserData* data)
{
    LQCombatHero* hero = (LQCombatHero*)pSender;
    if (hero && hero->getStoryIDIsDead()>0){
        this->playPoltLayer(hero->getStoryIDIsDead(),3);
    }
}

//Skill CallBack方法
//Skill触发数据动作
void LQFightLayer::skillStartCallback(Node* pSender,LQBaseUserData* data,CallFunc* callback)
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("4) %p skillStartCallback...", pSender);
    
    _skillhero = (LQCombatHero*)pSender;
    _effectdata = (LQEffectUserData*)data;
    _skillcallback = callback;
    if(_skillcallback) _skillcallback->retain();
    
    //停住界面
    pauseGame();
    
    //    0我方   1敌方
    LQSkillShowingLayer *layer = LQSkillShowingLayer::create();
    FiniteTimeAction* endcall = CallFunc::create([this](){
        //this->getParent()->removeChildByTag(NODE_TAG_SKILLDESC);
        this->skillDescCallback(_skillhero);
    });
    layer->show((LQCombatHero*)pSender,data, _skillhero->getSquadDirection()==klqCombatSquadDirectionLeft?0:1, endcall );
    this->getParent()->addChild(layer,NODE_ZORDER_DIALOG);
    
    
    //技能说明条
    LayerColor* sbarlayer = LayerColor::create(Color4B(0, 0, 0, 160), visibleSize.width, visibleSize.height*0.24);
    sbarlayer->ignoreAnchorPointForPosition( false );
    sbarlayer->setTag(NODE_TAG_SKILLBAR);
    sbarlayer->setContentSize(Size(visibleSize.width, visibleSize.height*0.24));
    //sbarlayer->setPosition(Point(visibleSize.width/2+origin.x,mapSize.height*0.08+origin.y));
    sbarlayer->setVisible(false);
    float y0 = sbarlayer->getContentSize().height;
    float h0 = y0*0.4;
    sbarlayer->setPosition(Point(visibleSize.width/2, visibleSize.height*0.12));
    sbarlayer->setVisible(false);
    this->getParent()->addChild(sbarlayer,NODE_ZORDER_DIALOG);
    Sprite* skillLine1 = Sprite::createWithSpriteFrameName("skillhero_line.png");
    float s0 = visibleSize.width/skillLine1->getContentSize().width;
    float w0 = skillLine1->getContentSize().width*s0;
    skillLine1->setScaleX(s0);
    skillLine1->setAnchorPoint(Point(0.5, 1));
    skillLine1->setPosition(Point(visibleSize.width/2, y0));
    sbarlayer->addChild(skillLine1);
    y0 -= skillLine1->getContentSize().height;
    Color4B c4 = _skillhero->getSquadDirection()==klqCombatSquadDirectionLeft ? Color4B(0,0,255,105) : Color4B(255,0,0, 105);
    LayerColor* skillBar = LayerColor::create(c4, w0, h0 );
    skillBar->ignoreAnchorPointForPosition( false );
    skillBar->setAnchorPoint(Point(0.5, 1));
    skillBar->setPosition(Point(visibleSize.width/2, y0));
    sbarlayer->addChild(skillBar);
    y0 -= skillBar->getContentSize().height;
    Sprite* skillLine2 = Sprite::createWithSpriteFrameName("skillhero_line.png");
    skillLine2->setScaleX(s0);
    skillLine2->setAnchorPoint(Point(0.5, 1));
    skillLine2->setPosition(Point(visibleSize.width/2, y0));
    sbarlayer->addChild(skillLine2);
    
    
    //技能名称和描述 data->getName()
    string skillname = _effectdata->getSkillInfo()->getName();
#if defined(LQ_DEMO_FIGHT)
    #if SHOW_NORMAL_HURT
    //stringf(skillname, "%s SIDX=%d IDX:%d ", skillname.c_str(), _skillhero->getSquad()->getIndex(), _skillhero->getIndex());
    #endif
#endif
    Label* skillNameLabel = Label::createWithSystemFont(skillname.c_str(), "Helvetica", 42);
    skillNameLabel->setColor(Color3B::WHITE);
    skillNameLabel->enableShadow();
    //virtual void enableShadow(const Color4B& shadowColor = Color4B::BLACK,const Size &offset = Size(2,-2), int blurRadius = 0);
    //virtual void enableOutline(const Color4B& outlineColor,int outlineSize = -1);
    //virtual void enableGlow(const Color4B& glowColor);
    skillNameLabel->setAnchorPoint(Point(0.51,0.5));
    skillNameLabel->setPosition(Point(skillBar->getContentSize().width/2, skillBar->getContentSize().height/2));
    skillBar->addChild(skillNameLabel);
    string desc = _effectdata->getSkillInfo()->getDescription(); //"对敌方全体弓兵造成伤害并将敌人晕眩2秒。武勇增加对敌伤害。";
    //Label* skillNameDesc = Label::createWithSystemFont(desc, "Helvetica", 24);
    
    Size csize  = Size(skillBar->getContentSize().width*0.9, skillBar->getContentSize().height*2);
    Label* skillNameDesc = Label::createWithSystemFont(desc, "Helvetica", 24, csize, TextHAlignment::CENTER, TextVAlignment::CENTER);
    skillNameDesc->enableShadow();
    skillNameDesc->setAnchorPoint(Point(0.5,0.5));
    skillNameDesc->setPosition(Point(skillBar->getContentSize().width/2, -skillBar->getContentSize().height*0.5));
    skillBar->addChild(skillNameDesc);
    
    if(true){
        //this->skillDescCallback(_skillhero);
        return;
    }
}

//技能描述结束后
void LQFightLayer::skillDescCallback(Node* pSender)
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("5) %p LQFightLayer::skillDescCallback ...", pSender);
    Node* sbarlayer = this->getParent()->getChildByTag(NODE_TAG_SKILLBAR);
    if (sbarlayer)
        sbarlayer->setVisible(true);
    
    //武将释放技能光效特效
    float _scale0 = 1.4;
    LQBuffSprite* m_skillLight= LQBuffSprite::createWithId(40029005,_scale0);
    m_skillLight->setAnchorPoint(Point(0.5, 0.5));  //haloSprite->getSkillAnchPosition()
    m_skillLight->setPosition(_skillhero->getPosition()+_skillhero->getLAnchorPoint());
    _skillhero->getParent()->addChild(m_skillLight, _skillhero->getLocalZOrder()+1);
    Sequence* sequ1 = Sequence::create(CallFunc::create([this](){
        if(_skillhero){
            //改为特效释放后去除，现在不显示蒙层
            //this->getParent()->removeChildByTag(NODE_TAG_SKILLBAR);
            if (_skillhero->getParent()->getChildByTag(NODE_TAG_SKILLMAKE))
                _skillhero->getParent()->getChildByTag(NODE_TAG_SKILLMAKE)->setOpacity(0);
            
            _skillhero->setScale(1.0);
            _skillhero->setLocalZOrder(1600-_skillhero->getPositionY());
            Director::getInstance()->getActionManager()->resumeTarget(_skillhero->getSprite());
        }
    }), _skillcallback, NULL);
    if(_skillcallback) _skillcallback->release();
    //技能武将光环音效
    AudioManager::getInstance()->playEffect(Effect_Skill_Music2);
    
    m_skillLight->playWithTimes(2,sequ1);
}

//Skill触发数据动作
void LQFightLayer::hitStartCallback(Node* pSender,LQBaseUserData* data)
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("10) %p LQFightLayer::hitStartCallback ...", pSender);
    LQEffectUserData* _edata = (LQEffectUserData*)data;
    //LQCombatHero* _hero = (LQCombatHero*)pSender;
    //振动屏幕
    if(_edata && _edata->getAnimationCfgData()){
        //int breakType = _edata->getAnimationCfgData()->getBreakType();
        //if (breakType==1)
        //    shakeScene();
    }
}

//Skill结束动作
void LQFightLayer::skillEndCallback(Node* pSender,LQBaseUserData* data)
{
    if (_skillhero && _skillhero->getParent()->getChildByTag(NODE_TAG_SKILLMAKE))
        _skillhero->getParent()->removeChildByTag(NODE_TAG_SKILLMAKE);
    if (this->getParent()->getChildByTag(NODE_TAG_SKILLBAR))
        this->getParent()->removeChildByTag(NODE_TAG_SKILLBAR);
    
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL2 || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("13) %p LQFightLayer::skillEndCallback.... showingskill:%d -> 0",  pSender, m_now_showing_skill);
    _skillhero = NULL;
    _effectdata = NULL;
    _skillcallback = NULL;
    m_now_showing_skill = false;
    m_time_showing_skill = 0;
    //SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);//设置音乐声音
    resumeGame();
    //战斗放战场背景01
    if (LQBattleSession::getInstance()->getIsHasMelee() && m_meleeEffect>0){
        AudioManager::getInstance()->resumeEffect(m_meleeEffect);
        //m_meleeEffect = AudioManager::getInstance()->playEffect(Effect_BG1_Music, true);
    }
}

//技能Event响应
void LQFightLayer::onEffectBegan(LQEventSkillEffect* event)
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3)
        CCLOG("??) Skill LQFightLayer::onEffectBegan event: %p", event);
}

void LQFightLayer::onEffectEnded(LQEventSkillEffect* event)
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3 )
        CCLOG("11) Skill LQFightLayer::onEffectEnded event: %p", event);
}

// 发送战斗结果后提示网络等待
// isopen->true开启提示  发送消息了
// isopen->false关闭提示  收到消息了
void LQFightLayer::showNetPrompt(bool isopen)
{
#if !defined(LQ_DEMO_FIGHT)
    if(isopen) {
        FiniteTimeAction* endcall = CallFunc::create([this](){
            Sprite *sp2 = (Sprite *)  this->getParent()->getChildByTag(103532);
            if (sp2) {
                sp2->stopAllActions();
                sp2->removeFromParent();
            }
            ComService::getInstance()->Close();
        });
        FiniteTimeAction* endcallshow = CallFunc::create([this](){
            Sprite *sp2 = (Sprite *)  this->getParent()->getChildByTag(103532);
            if (sp2) {
                sp2->setVisible(true);
            }
        });

        Sprite* sp2 = Sprite::create("res/level/loading.png");
        sp2->setAnchorPoint(Point(0.5, 0.5));
        sp2->setPosition(Point(visibleSize.width*0.5,visibleSize.height*0.4));
        sp2->setTag(103532);
        sp2->setVisible(false);
        DelayTime *dtime = DelayTime::create(2);
        RotateBy* rotate = RotateBy::create(4, 360);
        Repeat *repeat = Repeat::create(rotate, 4);
        Sequence *seq = Sequence::create(dtime,endcallshow,repeat, endcall, NULL);
        sp2->runAction(seq);
        this->getParent()->addChild(sp2,NODE_ZORDER_MAX);
    } else {
        Sprite *sp2 = (Sprite *)  this->getParent()->getChildByTag(103532);
        if (sp2) {
            sp2->stopAllActions();
            sp2->removeFromParent();
        }
    }
#endif
}

//创建系列帧动画 "nq_%d.png"
ActionInterval* LQFightLayer::createAnimation(string hname, int num)
{
    std::string numstr;
    Vector<SpriteFrame*>* allFrames = new Vector<SpriteFrame*>();
    SpriteFrameCache* sharedSFCache = SpriteFrameCache::getInstance();
    string file = "";
    LQ::stringf( file, hname.c_str(), 1 );
    Animation *animation = AnimationCache::getInstance()->getAnimation(file);
    
    float fps = 0.1;
    if (!animation) {
        string oldframe = "";
        for(int i = 1; i <= num ; i++) {
            std::string fname;
            LQ::stringf( fname, hname.c_str(), i );
            SpriteFrame* frame = sharedSFCache->getSpriteFrameByName(fname.c_str());
            if(!frame){
                frame = sharedSFCache->getSpriteFrameByName(oldframe.c_str());
            }else{
                oldframe = fname;
            }
            allFrames->pushBack(frame);
        }
        
        animation=Animation::createWithSpriteFrames(*allFrames,fps);
        
        AnimationCache::getInstance()->addAnimation(animation, file);
    }
    ActionInterval* animate = Animate::create(animation);
    //有具体播放者控制次数
    CC_SAFE_DELETE(allFrames);
    return animate;
}


LQ_NAMESPACE_END
