#include "HelloWorldScene.h"
#include "LQCombatUnitEnums.h"
#include "LQFlyNumber.h"
#include "LoadFightScene.h"
#include "LineupLayer.h"
#include "LQTipInfoData.h"
 
USING_NS_CC;

//  界面第一个按钮 回复技能初始位置
//  界面第二个按钮 释放技能
//  界面第三个按钮 用设置阵型布阵
//  界面第四个按钮 直接战斗

//查找TESTDATA 有三处控制的位置
//TESTDATA1 是否显示武将模型动作测试，美术测试模型和特效使用 1为使用
int testHeroAction = 0;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    skillidadd = 0;
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    
    /*
    int size1 = 5;
    int *randval = new int[size1];
    for (int i = 0; i<size1; i++)
        randval[i] = size1*30;
    
    srand((unsigned)time(NULL));
    for (int i = 0; i<5; i++) {
        int k = randomWeightInt(randval, size1);
        CCLOG("randomInt:%d %d %d %d %d", randval[0], randval[1], randval[2], randval[3], randval[4]);
        CCLOG("randomInt = %d", k);
    }
    CC_SAFE_FREE(randval);*/
    
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("./res/tiledmap/Vs_snow.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    missileSp = NULL;
    Sprite* skillsp = Sprite::create("btn_skill.png");
    ControlButton* skillbutton = ControlButton::create(Scale9Sprite::create("btn_skill.png"));
    skillbutton->setPreferredSize(skillsp->getContentSize());
    skillbutton->setPosition(Point(visibleSize.width*0.38,visibleSize.height*0.1));
    skillbutton->setAnchorPoint(Point(0, 0.5));
    skillbutton->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::OnSkillButtonClicked), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(skillbutton,100);
    
    Sprite* lineupsp = Sprite::create("btn_lineup.png");
    ControlButton* lineupbutton = ControlButton::create(Scale9Sprite::create("btn_lineup.png"));
    lineupbutton->setPreferredSize(lineupsp->getContentSize());
    lineupbutton->setPosition(Point(visibleSize.width*0.62,visibleSize.height*0.1));
    lineupbutton->setAnchorPoint(Point(0, 0.5));
    lineupbutton->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::OnLineupButtonClicked), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(lineupbutton,100);
    
    Sprite* autosp = Sprite::create("btn_fuben.png");
    ControlButton* autobutton = ControlButton::create(Scale9Sprite::create("btn_fuben.png"));
    autobutton->setPreferredSize(autosp->getContentSize());
    autobutton->setPosition(Point(visibleSize.width*0.82,visibleSize.height*0.1));
    autobutton->setAnchorPoint(Point(0, 0.5));
    autobutton->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::OnAutoButtonClicked), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(autobutton,100);
    
    Sprite* autosp2 = Sprite::create("btn_clear.png");
    ControlButton* autobutton2 = ControlButton::create(Scale9Sprite::create("btn_clear.png"));
    autobutton2->setPreferredSize(autosp2->getContentSize());
    autobutton2->setPosition(Point(visibleSize.width*0.12,visibleSize.height*0.1));
    autobutton2->setAnchorPoint(Point(0, 0.5));
    autobutton2->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::OnAutoButton2Clicked), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(autobutton2,100);
    
    shootsp = NULL;
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("res/soldier/td-special.plist","res/soldier/td-special.pvr.ccz");
    frameCache->addSpriteFramesWithFile("res/skill/skillbuffer.plist","res/skill/skillbuffer.pvr.ccz");
    
    //TESTDATA2 测试数据  模型的ID 修改此处数据测试美术资源
    resid = 90010001;  //90041906
    int resid2 = 90010002;  //90041906
    //测试技能的ID，修改将改变技能开始的位置
    skillid0 = 40020065;  //技能回复后回到的位置
    skillid = 40020021;   //技能开始的位置
    skillidadd = 1; //每一次释放，技能ID增加的值，0就固定释放一个资源
    
    if ( LQDataCache::sharedLQDataCache()->getTipDatas()==NULL ){
        LQDataMap& tipdatas = *(LQDataMap*)LQDataCache::sharedLQDataCache()->lqAllDatasByName(LQTipInfoData::getAliasName());
        LQTipMap* tipMap = new LQTipMap();
        LQDataMap::iterator iter;
        for(iter = tipdatas.begin(); iter != tipdatas.end(); iter++)
        {
            LQTipInfoData* pcfgData = (LQTipInfoData*)iter->second;
            tipMap->insert(pair<int, string>(pcfgData->getFID(), pcfgData->getWord()));
            delete pcfgData;
        }
        //内存处理
        delete  &tipdatas;
        
        LQDataCache::sharedLQDataCache()->setTipDatas(tipMap);
    }
    
    m_herodata = new LQHeroUserData(resid, 90001001, 15);
    m_herodata->calcData();
    string soldierCode = m_herodata->getBaseName().c_str();
    string reshead;
    string resfile;
    string resfile2;
    string soldierColor = "B";
    LQ::stringf(reshead, "%s_%s", soldierColor.c_str(), soldierCode.c_str());
    this->loadHeroResource(soldierColor,soldierCode);
    
    
    m_skilldata = m_herodata->getActiveSkillData();
    m_hero = LQCombatHero::create(m_herodata, klqCombatSquadDirectionLeft);
    m_hero->setPosition(280,180);
    this->addChild(m_hero,1600-m_hero->getPositionY());
    
    m_targetdata = new LQHeroUserData(resid2, 90001001, 15);
    if (resid!=resid2){
        soldierCode = m_targetdata->getBaseName().c_str();
        this->loadHeroResource(soldierColor,soldierCode);
    }
    
    m_target = LQCombatHero::create(m_targetdata, klqCombatSquadDirectionRight);
    m_target->setPosition(680,380);
    this->addChild(m_target,1600-m_target->getPositionY());
    
//    m_targetdata = new LQHeroUserData(90010026, 90001001, 15);
//    m_target = LQCombatHero::create(m_targetdata, klqCombatSquadDirectionRight);
//    m_target->setPosition(680,620);
//    this->addChild(m_target,10);
    
    if (testHeroAction>0){
        LQHeroUserData* m_testdata;
        LQCombatHero* m_testHero;
        m_hero->setPosition(280,110);
        m_target->setPosition(680,110);
        //m_target->setVisible(false);
        
        //TESTDATA3 测试模型数据ID 修改此处数据测试美术资源
        _testHeroId = 90010031; //90010031 90010065  模型开始位置
        _testsize = 30;    //一次装载的模型数量
        _directionType = 4;
        _animPhaseEnum = 1;
        _stestsize = 0;
        for (int k=0; k<_testsize; k++) {
            int m = k/10;
            int n = k%10;
            m_testdata = new LQHeroUserData(_testHeroId+k, 90001001, 15);
            soldierCode = m_testdata->getBaseName().c_str();
            LQ::stringf(reshead, "%s_%s", soldierColor.c_str(), soldierCode.c_str());
            this->loadHeroResource(soldierColor,soldierCode);
            
            m_testHero = LQCombatHero::create(m_testdata, klqCombatSquadDirectionRight);
            m_testHero->setPosition(80+n*90,630-m*120);
            //klqUnitDirectionTypeDown  klqUnitDirectionTypeUp klqUnitDirectionTypeRight
            //klqUnitDirectionTypeLeftDown klqUnitDirectionTypeRightDown
            m_testHero->setTag(_testHeroId+k);
            this->addChild(m_testHero,10);
            m_testHero->restoreHeroHalo();
            
            ttf0 = Label::createWithSystemFont(m_testdata->getName().c_str(), "Helvetica",14);
            ttf0->setColor(Color3B::BLACK);
            ttf0->setPosition(Point(m_testHero->getPositionX(),m_testHero->getPositionY()-30));
            this->addChild(ttf0, 101);
        }
        LQArmsUserData* m_soldier;
        m_soldierIDC = 90001001; //装载的士兵模型ID
        _stestsize = 17;
        soldierColor = "R";
        for (int k=0; k<_stestsize; k++) {
            int m = k/10;
            int n = k%10;
            m_soldier = new LQArmsUserData();
            m_soldier->initWithIDC(m_soldierIDC+k);
            soldierCode = m_soldier->getBaseName().c_str();
            LQ::stringf(reshead, "%s_%s", soldierColor.c_str(), soldierCode.c_str());
            LQ::stringf(resfile, "res/soldier/avt_%s_%s.plist", soldierColor.c_str(), soldierCode.c_str());
            LQ::stringf(resfile2, "res/soldier/avt_%s_%s.pvr.ccz", soldierColor.c_str(),  soldierCode.c_str());
            frameCache->addSpriteFramesWithFile(resfile.c_str(),resfile2.c_str());
            
            LQCombatSoldier* unit = LQCombatSoldier::create(m_soldier, klqCombatSquadDirectionRight);
            unit->setPosition(80+n*90,300-m*100);
            unit->setTag(m_soldierIDC+k);
            unit->changeAnimationByType((LQUnitAnimPhaseEnum)_animPhaseEnum,klqUnitDirectionTypeDown);
            this->addChild(unit,1600-this->getPositionY());
        }
    }
    
    m_hero->restoreHeroHalo();
    m_target->restoreHeroHalo();
    
    //SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    ttf0 = Label::createWithSystemFont("Hello World", "Helvetica", 24);
    ttf0->setColor(Color3B::BLACK);
    ttf0->setPosition(Point(80,
                             origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(ttf0, 101);
    
    frameCache->addSpriteFramesWithFile("res/skill/skillbuffer.plist","res/skill/skillbuffer.pvr.ccz");
    frameCache->addSpriteFramesWithFile("res/skill/fightlayer.plist","res/skill/fightlayer.pvr.ccz");
    
    Sprite* m_hurtsSprite = Sprite::createWithSpriteFrameName("nq_1.png");
    m_hurtsSprite->setPosition(m_target->getPosition()-Point(0, 80));
    this->addChild(m_hurtsSprite);
    ActionInterval *action = this->createAnimation();
    RepeatForever* forever1 = RepeatForever::create(action);
    m_hurtsSprite->runAction(forever1);
    
    // 启动update函数
    scheduleUpdate();
    
    
//    this->updata();
    
    return true;
}

void HelloWorld::loadHeroResource(string soldierColor, string  soldierCode)
{
    string resfile;
    string resfile2;
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    LQ::stringf(resfile, "res/hero/avt_%s_%s.plist", soldierColor.c_str(), soldierCode.c_str());
    LQ::stringf(resfile2, "res/hero/avt_%s_%s.pvr.ccz", soldierColor.c_str(),  soldierCode.c_str());
    frameCache->addSpriteFramesWithFile(resfile.c_str(),resfile2.c_str());
    LQ::stringf(resfile, "res/hero/avt_%s_%s.plist", "A", soldierCode.c_str());
    LQ::stringf(resfile2, "res/hero/avt_%s_%s.pvr.ccz", "A",  soldierCode.c_str());
    frameCache->addSpriteFramesWithFile(resfile.c_str(),resfile2.c_str());
}

// 开发时候要关闭下载功能 true开启  false关闭下载功能
//static bool isFirst = false;
//void HelloWorld::updata()
//{
//    if(isFirst){
//        isFirst = false;
//        std::vector<std::string> searchPaths = FileUtils::getInstance()->getSearchPaths();
//        std::vector<std::string>::iterator iter = searchPaths.begin();
//        searchPaths.insert(iter, FileUtils::getInstance()->getWritablePath());
//        FileUtils::getInstance()->setSearchPaths(searchPaths);
//      
//        UpdateLayer *layer = UpdateLayer::create();
//        layer->setshowType(1);//设置展示类别为下载
//        layer->setLatestVersion("http://192.168.1.8/SWSKILLTEST/down/testdata/",1);
//        this->addChild(layer,1000);
//        layer->starDownload();
//    }
//}

//创建系列帧动画
ActionInterval* HelloWorld::createAnimation()
{
    std::string numstr;
    Vector<SpriteFrame*>* allFrames = new Vector<SpriteFrame*>();
    SpriteFrameCache* sharedSFCache = SpriteFrameCache::getInstance();
    string file = "nq_1.png";
    Animation *animation = AnimationCache::getInstance()->getAnimation(file);
    
    float fps = 0.1;
    if (!animation) {
        string oldframe = "";
        for(int i = 1; i <= 11 ; i++) {
            std::string fname;
            LQ::stringf( fname, "nq_%d.png", i );
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

void HelloWorld::OnAutoButton2Clicked(Ref *sender, Control::EventType controlEvent)
{
    //LQFlyNumber* pHp = LQFlyNumber::createFlyWithSpriteFrameName("crit_jt.png", 0, klq_flaytype_criticon);
    //pHp->setPosition(Point(680,480));
    //this->addChild(pHp, 2048);
    //pHp->startAnimation();
    
    // Director::getInstance()->purgeCachedData();
    resid = 90041906;
    skillid = skillid0;
    
    if (testHeroAction>0){
        _animPhaseEnum++;
        if (_animPhaseEnum>5){
            _animPhaseEnum = 1;
            _directionType++;
            if (_directionType==3) _directionType = 4;
            if (_directionType==5||_directionType==6) _directionType = 7;
            if (_directionType>7) _directionType = 0;
        }
        LQCombatHero* hero;
        for (int k=0; k<_testsize; k++) {
            hero = (LQCombatHero*)this->getChildByTag(_testHeroId+k);
            if (hero){
                hero->changeAnimationByType((LQUnitAnimPhaseEnum)_animPhaseEnum,(LQUnitDirectionType)_directionType);
            }
        }
        LQCombatSoldier* soldier;
        for (int k=0; k<_stestsize; k++) {
            soldier = (LQCombatSoldier*)this->getChildByTag(m_soldierIDC+k);
            if (soldier){
                soldier->changeAnimationByType((LQUnitAnimPhaseEnum)_animPhaseEnum,(LQUnitDirectionType)_directionType);
            }
        }
        return;
    }
    //LQBuffSprite* m_skillSprite= LQBuffSprite::createWithId(40029005);
    //m_skillSprite->setAnchorPoint(Point(0.5, 0.5));  //haloSprite->getSkillAnchPosition()
   // m_skillSprite->setPosition(Point(780,280));
    //this->addChild(m_skillSprite, 1000);
    //m_skillSprite->play2(2);
    
//    if(_anodes.size()>0){
//        Director::getInstance()->getActionManager()->resumeTargets(_anodes);
//        for (auto& child : m_sprite->getChildren())
//        {
//            if(child)
//                Director::getInstance()->getActionManager()->resumeTarget(child);
//        }
//        _anodes.clear();
//    }else{
//         _anodes = Director::getInstance()->getActionManager()->pauseAllRunningActions();
//        
//        Director::getInstance()->getActionManager()->resumeTarget(m_target->getSprite());
//    }
    
}

void HelloWorld::OnLineupButtonClicked(Ref *sender, Control::EventType controlEvent)
{
    //LQDataCache::sharedLQDataCache()->purgeSharedLQDataCache();
    //return;
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    LineupLayer* lineup = LineupLayer::create();
    lineup->setPosition(Point(0,0));
    this->addChild(lineup,100);
}

void HelloWorld::OnSkillButtonClicked(Ref *sender, Control::EventType controlEvent)
{
    m_herodata = new LQHeroUserData(resid, 90001001, 15);
    if(!m_herodata->getCfgdata())
        return;
    m_herodata->calcData();
    m_skilldata = m_herodata->getActiveSkillData();  //new LQSkillUserData(skillid++);//
    
    Point frompt = m_hero->getMissileStartPostion();
    Point topt = m_target->getMissileTargetPostion();
    LQAnimationSprite* sp = LQAnimationSprite::createWithId(40022000);
    sp->play();
    if(sp){
        missileSp = LQMissile::create(Missile_Fly_ZX,sp);
        missileSp->setPosition(frompt);
        this->getParent()->addChild(missileSp,3000);
        
        auto callback = CallFuncN::create(CC_CALLBACK_1(HelloWorld::shootEnd,this));
        missileSp->shoot(frompt, topt, callback, 40020007);
    }
    
    m_hero->changeSkillAnimation();
}

void HelloWorld::shootEnd(Node * node)
{
    skillHitEffect();
    missileSp->removeFromParent();
    missileSp  =NULL;
    LQMissileManager::sharedMissileManager()->clear();
    m_hero->changeAnimationByType(klqUnitAnimPhaseStand, klqUnitDirectionTypeNone);
}

//技能单个命中特效
void HelloWorld::skillHitEffect()
{
    CCLOG("%p CombatHero::skillHitEffect %ld", this, getCurrentMillSecond() );
    
    if(!shootsp){
        shootsp = Sprite::createWithSpriteFrameName("shoottarget.png");
        shootsp->setPosition(m_target->getPosition());
        m_target->getParent()->addChild(shootsp,1);
        shootsp->runAction( RepeatForever::create(Sequence::create(ScaleTo::create(1.0, 1.2), ScaleTo::create(1.0, 1), NULL)) );
    }
    skillid+=skillidadd;
    int _hiteffect = skillid; //m_skilldata->getSkillCfgData()->getFireEffect();
    //if (skillid>40020046) skillid = 40020043;
    //if (skillid==40020028) skillid = 40020029; else skillid = 40020028;
    if (_hiteffect<=0)
        return;
    ttf0->setString(intToString(_hiteffect).c_str());
    
    LQBuffSprite* _hitSp = LQBuffSprite::createWithId(_hiteffect);
    _hitSp->setPosition(m_target->getPosition());
    if ( _hitSp->getHangingPoint()==4)  //底部
        m_target->getParent()->addChild(_hitSp, 1);
    else m_target->getParent()->addChild(_hitSp,3001);
    _hitSp->play1();
    return;
}

void HelloWorld::update(float dt)
{
    if(missileSp)
        missileSp->customUpdate(dt);
}

void HelloWorld::OnAutoButtonClicked(Ref *sender, Control::EventType controlEvent)
{
    // create a scene. it's an autorelease object
    //auto scene = LQFightLayer::createScene();
    // run
    //auto director = Director::getInstance();
    //director->replaceScene(scene);
    
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    //SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    
    LoadFightScene* layer = LoadFightScene::create();
    //layer->initData("");
    Scene *scene = Scene::create();
    scene->addChild(layer);
    auto director = Director::getInstance();
    director->pushScene(scene);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
