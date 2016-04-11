#include "LineupLayer.h"
#include "LoadFightScene.h"
#if defined(LQ_DEMO_FIGHT)
#include "LQTFormatinfoData.h"
#endif

USING_NS_CC;


static LQTestSession *_sharedTestSession = nullptr;
//前端数据缓存类
LQTestSession* LQTestSession::getInstance()
{
    if (! _sharedTestSession)
    {
        _sharedTestSession = new LQTestSession();
        _sharedTestSession->init();
    }
    
    return _sharedTestSession;
}

LQTestSession::LQTestSession()
{
    for(int i = 0;i <15; i++){
        attackHeroList[i]  = -1;
        defenceHeroList[i] = -1;
    }
    
#if defined(LQ_DEMO_FIGHT)
    //读数据
    LQDataMap& fmtdatas = *(LQDataMap*)LQDataCache::sharedLQDataCache()->lqAllDatasByName(LQTFormatinfoData::getAliasName());
    LQDataMap::iterator iter;
    int value = 0;
    for(iter = fmtdatas.begin();iter!=fmtdatas.end();++iter)
    {
        LQTFormatinfoData* pfmtData = (LQTFormatinfoData*)iter->second;
        CCLOG("ID:%d %s used:%d", pfmtData->getFID(), pfmtData->getName().c_str(), pfmtData->getUsed());
        if( pfmtData->getUsed()>0){  //我方
            for (int i=0; i<15; i++) {
                std::string p = StringUtils::format("IDX%d",i+1);
                pfmtData->getValue(p.c_str(), &value);
                if(value<=0) value = -1;
                if (pfmtData->getIdx()==1)
                    attackHeroList[i] = value;
                else if (pfmtData->getIdx()==2)
                    defenceHeroList[i] = value;
            }
        }
    }
    //内存处理
    delete &fmtdatas;
#endif
}

LQTestSession::~LQTestSession()
{
    
}

//======================LineupLayer==========================

LineupLayer::~LineupLayer()
{
    movableSprites.clear();
    boxSprites.clear();
}

// on "init" you need to initialize your instance
bool LineupLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 150)) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //Point origin = Director::getInstance()->getVisibleOrigin();
    
    //m_colorLayer = LayerColor::create(Color4B(KLQMaskLayerColor, KLQMaskLayerColor, KLQMaskLayerColor, LayerColorOpacity), size.width, size.height);
    //addChild(m_colorLayer,1);
    
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("res/soldier/td-special.plist","res/soldier/td-special.pvr.ccz");
    frameCache->addSpriteFramesWithFile("res/skill/fightlayer.plist","res/skill/fightlayer.pvr.ccz");
    
    Sprite* backsp = Sprite::create("ty_fanhui.png");
    ControlButton* backbutton = ControlButton::create(Scale9Sprite::create("ty_fanhui.png"));
    backbutton->setPreferredSize(backsp->getContentSize());
    backbutton->setPosition(Point(visibleSize.width*0.01,visibleSize.height*0.95));
    backbutton->setAnchorPoint(Point(0, 0.5));
    backbutton->addTargetWithActionForControlEvents(this, cccontrol_selector(LineupLayer::onBackButtonClicked), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(backbutton,100);
    
    Sprite* autosp = Sprite::create("kaizhan.png");
    ControlButton* autobutton = ControlButton::create(Scale9Sprite::create("kaizhan.png"));
    autobutton->setPreferredSize(autosp->getContentSize());
    autobutton->setPosition(Point(visibleSize.width*0.82,visibleSize.height*0.1));
    autobutton->setAnchorPoint(Point(0, 0.5));
    autobutton->addTargetWithActionForControlEvents(this, cccontrol_selector(LineupLayer::onButtonClicked), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(autobutton,100);
    
    
    
    Sprite* fk;
    LQCombatHero* herosp;
    Label* ttf0;
    LQHeroUserData* m_heroData;
    string reshead;
    string resfile;
    string resfile2;
    string soldierColor = "B";
    string soldierCode;
    string tempStr;
    int resId;
    for (int i=0 ; i<15; i++) {
        int x0 = 2-i/5;
        int y0 = 4-i%5;
        fk = Sprite::create("bz_fangkuai1.png");
        fk->setTag(100+i);
        Point pt = convertPoint(x0*3, y0*3)+Point(-visibleSize.width*0.56,visibleSize.height*0.15);
        fk->setPosition(pt);
        boxSprites.push_back(fk);
        this->addChild(fk,1);
        resId = LQTestSession::getInstance()->getAttackHeroList(i);
        CCLOG("攻方 [%d]:  %d", i, resId);
        if (resId>0){
            m_heroData = new LQHeroUserData(resId, -1, 15);
            soldierCode = m_heroData->getBaseName().c_str();
            this->loadHeroResource(soldierColor,soldierCode);
            
            herosp = LQCombatHero::create(m_heroData, klqCombatSquadDirectionLeft);
            //herosp = Sprite::create("btn_skill.png");
            herosp->setPosition(pt);
            herosp->setTag(1000+i);
            this->addChild(herosp,1);
            string str;
            stringf(str,"%s [%d]",m_heroData->getName().c_str(),i+1);
            ttf0 = Label::createWithSystemFont(str.c_str(), "Helvetica",16);
            ttf0->setColor(Color3B::WHITE);
            ttf0->setPosition(Point(herosp->getImgWidth()*0.5, herosp->getImgHeight()*0.1));
            ttf0->setAnchorPoint(Point(0.5,0));
            ttf0->setTag(1000);
            herosp->addChild(ttf0, 101);
            
            //兵种的背景
            string fname1 = "flag_lan.png";
            Sprite* _flagiconbg = Sprite::createWithSpriteFrameName(fname1.c_str());
            _flagiconbg->setTag(20002);
            _flagiconbg->setAnchorPoint(Point(0, 1));
            _flagiconbg->setPosition(Point(herosp->getImgWidth()*0, herosp->getImgHeight()*0.1));
            herosp->addChild(_flagiconbg,0);
            string armicon = m_heroData->getArmsCfgdata()->getSmallIcon().c_str();
            stringf(tempStr, "res/level/%s", armicon.c_str());
            Sprite* _flagarmicon = Sprite::create(tempStr.c_str());
            _flagarmicon->setPosition(Point(_flagiconbg->getContentSize().width/2,_flagiconbg->getContentSize().height/2));
            _flagiconbg->addChild(_flagarmicon);
            
            herosp->setLocalZOrder(1200-herosp->getPositionY());
            movableSprites.pushBack(herosp);
        }
    }
    for (int i=0 ; i<15; i++) {
        int x0 = i/5;
        int y0 = i%5;
        fk = Sprite::create("bz_fangkuai3.png");
        fk->setTag(200+i);
        Point pt = convertPoint(x0*3, y0*3)+Point(-visibleSize.width*0.25,visibleSize.height*0.35);
        fk->setPosition(pt);
        this->addChild(fk,1);
        boxSprites.push_back(fk);
        resId = LQTestSession::getInstance()->getDefenceHeroList(i);
        CCLOG("防方 [%d]:  %d", i, resId);
        if (resId>0){
            m_heroData = new LQHeroUserData(resId, -1, 15);
            soldierCode = m_heroData->getBaseName().c_str();
            LQ::stringf(reshead, "%s_%s", soldierColor.c_str(), soldierCode.c_str());
            this->loadHeroResource(soldierColor,soldierCode);
            
            herosp = LQCombatHero::create(m_heroData, klqCombatSquadDirectionRight);
            //herosp = Sprite::create("btn_skill.png");
            herosp->setTag(2000+i);
            herosp->setPosition(pt);
            this->addChild(herosp,1);
            string str;
            stringf(str,"%s [%d]",m_heroData->getName().c_str(),i+1);
            ttf0 = Label::createWithSystemFont(str.c_str(), "Helvetica",16);
            ttf0->setColor(Color3B::WHITE);
            ttf0->setPosition(Point(herosp->getImgWidth()*0.5, herosp->getImgHeight()*0.1));
            ttf0->setAnchorPoint(Point(0.5,0));
            ttf0->setTag(1000);
            herosp->addChild(ttf0, 101);
            
            //兵种的背景
            string fname1 = "flag_hong.png";
            Sprite* _flagiconbg = Sprite::createWithSpriteFrameName(fname1.c_str());
            _flagiconbg->setTag(20002);
            _flagiconbg->setAnchorPoint(Point(0, 1));
            _flagiconbg->setPosition(Point(herosp->getImgWidth()*0, herosp->getImgHeight()*0.1));
            herosp->addChild(_flagiconbg,0);
            string armicon = m_heroData->getArmsCfgdata()->getSmallIcon().c_str();
            stringf(tempStr, "res/level/%s", armicon.c_str());
            Sprite* _flagarmicon = Sprite::create(tempStr.c_str());
            _flagarmicon->setPosition(Point(_flagiconbg->getContentSize().width/2,_flagiconbg->getContentSize().height/2));
            _flagiconbg->addChild(_flagarmicon);
            
            herosp->setLocalZOrder(1200-herosp->getPositionY());
            movableSprites.pushBack(herosp);
        }
    }
    
    // 启动update函数
    //scheduleUpdate();

    //拖拽英雄
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LineupLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LineupLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LineupLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void LineupLayer::loadHeroResource(string soldierColor, string  soldierCode)
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

//创建系列帧动画
ActionInterval* LineupLayer::createAnimation()
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

void LineupLayer::onBackButtonClicked(Ref *sender, Control::EventType controlEvent)
{
    this->removeFromParent();
}

void LineupLayer::onButtonClicked(Ref *sender, Control::EventType controlEvent)
{
    LQTestSession::getInstance()->clear();
    
    //int m = 0;
    int idx = 0;
    for (LQCombatHero *sprite : movableSprites)
    {
        int tag = sprite->getTag();
        if (tag>=2000) {
            //m = 2;
            idx = tag-2000;
            LQTestSession::getInstance()->setDefenceHeroList(idx,sprite->getHeroData()->getResIDC());
            //defenceHeroList1[idx] = sprite->getHeroData()->getResIDC();
        }
        else if (tag>=1000) {
            //m = 1;
            idx = tag-1000;
            LQTestSession::getInstance()->setAttackHeroList(idx,sprite->getHeroData()->getResIDC());
            //attackHeroList1[idx] = sprite->getHeroData()->getResIDC();
        }
    }
    LoadFightScene* layer = LoadFightScene::create();
    int attackHeroList1[15] = {-1};
    int defenceHeroList1[15] = {-1};
    for (int i=0; i<15; i++) {
        attackHeroList1[i] = LQTestSession::getInstance()->getAttackHeroList(i);
        defenceHeroList1[i] = LQTestSession::getInstance()->getDefenceHeroList(i);
    }
    layer->initData(attackHeroList1,defenceHeroList1);
    Scene *scene = Scene::create();
    scene->addChild(layer);
    auto director = Director::getInstance();
    director->pushScene(scene);
}

bool LineupLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    this->selectSpriteForTouch(touchLocation);
    return true;
}

void LineupLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    
    Point oldTouchLocation = touch->getPreviousLocation();
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);
    
    Point translation = touchLocation - oldTouchLocation;
    this->panForTranslation(translation);

}

void LineupLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    Point touchLocation = this->convertTouchToNodeSpace(touch);
    bool ismove = false;
    for (Sprite *sprite : boxSprites)
    {
        if (selSprite && sprite->getBoundingBox().containsPoint(touchLocation) )
        {
            //sprite->setColor(Color3B::RED);
            ismove = true;
            selSprite->setPosition(sprite->getPosition());
            selSprite->setLocalZOrder(1200-selSprite->getPositionY());
            int old_tag = selSprite->getTag();
            int old_idx = 0;
            int old_m = 0;
            if (old_tag>=2000) { old_m = 2; old_idx = old_tag-2000;}
            else if (old_tag>=1000) { old_m = 1; old_idx = old_tag-1000; }
            
            int tag = sprite->getTag();
            int m = 0; int idx = 0;
            if (tag>=200) { m=2; idx = tag-200;}
            else if (tag>=100) {m = 1; idx = tag-100; }
            
            if (old_m != m){
                selSprite->setPosition(selSpritePostion);
                selSprite = NULL;
                selSpritePostion = Point::ZERO;
                return;
            }
            
            LQCombatHero* hero1 = (LQCombatHero*)this->getChildByTag(m*1000+idx);
            if (m>0){
                selSprite->setIndex(idx);
                selSprite->setTag(m*1000+idx);
                selSprite->setPosition(sprite->getPosition());
                Label* ttf0 = (Label*)selSprite->getChildByTag(1000);
                if (ttf0) {
                    string str;
                    stringf(str,"%s [%d]",selSprite->getHeroData()->getName().c_str(),selSprite->getIndex()+1);
                    ttf0->setString(str.c_str());
                }
            }else{
                selSprite->setPosition(selSpritePostion);
                selSprite = NULL;
                selSpritePostion = Point::ZERO;
                return;
            }
            
            if(hero1){
                hero1->setIndex(old_idx);
                hero1->setPosition(selSpritePostion);
                hero1->setTag(m*1000+old_idx);
                hero1->setLocalZOrder(1200-hero1->getPositionY());
                
                Label* ttf0 = (Label*)hero1->getChildByTag(1000);
                if (ttf0) {
                    string str;
                    stringf(str,"%s [%d]",hero1->getHeroData()->getName().c_str(),hero1->getIndex()+1);
                    ttf0->setString(str.c_str());
                }
            }
        }
    }
    if (!ismove && selSprite)
        selSprite->setPosition(selSpritePostion);
    selSprite = NULL;
    selSpritePostion = Point::ZERO;
}

void LineupLayer::selectSpriteForTouch(Point touchLocation)
{
    LQCombatHero * newSprite = NULL;
    
    for (LQCombatHero *sprite : movableSprites)
    {
        if ( sprite->getHeroBoundingBox().containsPoint(touchLocation) )
        {
            newSprite = sprite;
            break;
        }
    }
    if (newSprite != selSprite && NULL != newSprite)
    {
        selSprite = newSprite;
        selSpritePostion = selSprite->getPosition();
    }
    if (newSprite == NULL){
        selSprite = NULL;
        selSpritePostion = Point::ZERO;
    }
}

Point LineupLayer::boundLayerPos(Point newPos)
{
    Size winSize = Director::getInstance()->getWinSize();
    Point retval = newPos;
    
    retval.x = MIN(retval.x, 0);
    retval.x = MAX(retval.x, -this->getContentSize().width+winSize.width);
    
    retval.y = this->getPosition().y;
    return retval;
}

void LineupLayer::panForTranslation(Point translation)
{
    if (selSprite)
    {
        Point newPos = selSprite->getPosition() + translation;
        selSprite->setPosition(newPos);
        selSprite->setLocalZOrder(1200-selSprite->getPositionY());
    }
    else
    {
        Point newPos = this->getPosition() + translation;
        this->setPosition( this->boundLayerPos(newPos) );
    }
}