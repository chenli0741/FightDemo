//
//  LQPVELoseLayer.cpp
//  game
//
//  Created by lequdev1 on 14-7-26.
//
//
#include "../Socket/ComService.h"
#include "LQPVELoseLayer.h"
#include "LQBattleSession.h"
#include "HorseBattleAwardRequest.h"
#include "LQHelp.h"

USING_NS_CC;
LQPVELoseLayer::LQPVELoseLayer()
{
}

LQPVELoseLayer::~LQPVELoseLayer()
{
}
 
LQPVELoseLayer* LQPVELoseLayer::createLayer()
{
    LQPVELoseLayer *ret = new LQPVELoseLayer();
    if (ret && ret->init()){
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool LQPVELoseLayer::init()
{
    
    std::string LTJM_PLIST12 = "files/ccbiui/ccbires/resv4/V4GRID.plist";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST12.c_str());
    std::string LTJM_PLIST = "files/ccbiui/ccbires/resv4/V4ZHANDOU.plist";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST.c_str());
    std::string UNIVERSAL_PLIST = "files/ccbiui/ccbires/resv4/V4UNIVERSAL.plist";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(UNIVERSAL_PLIST.c_str());
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(LQPVELoseLayer::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LQPVELoseLayer::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LQPVELoseLayer::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    
    Size visibleSize =  Director::getInstance()->getVisibleSize();

    // 半透明背景
    LayerColor *cbg = LayerColor::create(Color4B(0, 0, 0, 200), visibleSize.width, visibleSize.height);
    cbg->setAnchorPoint(Point(0,0));
    cbg->setPosition(0, 0);
    this->addChild(cbg);

    // 绘制UI的Layer;
    layerNode = LayerColor::create(Color4B(0, 0, 0, 0), designResolutionSize.width, designResolutionSize.height);
    layerNode->setAnchorPoint(Point(0, 0));
    layerNode->setPosition((visibleSize.width-designResolutionSize.width)/2,(visibleSize.height-designResolutionSize.height)/2);
    this->addChild(layerNode);
    
    awardlayer = Layer::create();  //获得物品的层
    awardlayer->setPosition(Point(250,70));
    awardlayer->setContentSize(Size(480,100));
    layerNode->addChild(awardlayer);

    
    
    SpriteFrame *frame =SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jixu.png");

    // 继续按钮 退出战斗那个
    ControlButton* autobutton = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame));
    autobutton->setPreferredSize(Size(78,76));
    autobutton->setPosition(Point(layerNode->getContentSize().width/5 * 4,layerNode->getContentSize().height/5 * 3));
    autobutton->setAnchorPoint(Point(1, 0));
    autobutton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::onBtnClickGoOn),Control::EventType::TOUCH_UP_INSIDE);
    layerNode->addChild(autobutton);
    
    // 头  2个斧子那图片
    Sprite *spTitle = Sprite::createWithSpriteFrameName("zd_shibai.png");
    spTitle->setAnchorPoint(Point(0.5,1));
    spTitle->setPosition(Point(layerNode->getContentSize().width/2,layerNode->getContentSize().height));
    layerNode->addChild(spTitle,2);
    
    //斧头背面光特效
    Sprite* spGuang = Sprite::createWithSpriteFrameName("ty_languang.png");
    spGuang->setAnchorPoint(Point(0.5, 0.5));
    spGuang->setPosition(Point(spTitle->getPositionX(),spTitle->getPositionY()-spTitle->getContentSize().height/2));
    RotateBy* rotate = RotateBy::create(4, 360);
    RepeatForever* forever = RepeatForever::create(rotate);
    spGuang->runAction(forever);
    layerNode->addChild(spGuang,1);
    
    Size layerNodeSize = layerNode->getContentSize();
    float button_height = layerNodeSize.height/4;
    float button_width[3] = {layerNodeSize.width/4-50,layerNodeSize.width/2,layerNodeSize.width/4 * 3+50};
    // 失败字体
    SpriteFrame* charSpriteFrm = SpriteFrameCache::getInstance()->getSpriteFrameByName("meishuzi.png");
    Sprite* charSprite = Sprite::createWithSpriteFrame(charSpriteFrm);
    charSprite->setPosition(Point(layerNodeSize.width/2,layerNodeSize.height/2));
    layerNode->addChild(charSprite);
    
    
    
    // 提升武将等级  中间
    SpriteFrame * buttonBG = SpriteFrameCache::getInstance()->getSpriteFrameByName("ty_beijingdi_1.png");
    SpriteFrame * yunBG = SpriteFrameCache::getInstance()->getSpriteFrameByName("ty_huawenxiangyun.png");

    
    
    heroStrongBtn = ControlButton::create(Scale9Sprite::createWithSpriteFrame(buttonBG));
    heroStrongBtn->setPosition(Point(layerNodeSize.width/2,layerNodeSize.height/4));
    heroStrongBtn->setPreferredSize(Size(270,250));
    heroStrongBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::OnHeroStrongButtonClicked),Control::EventType::TOUCH_UP_INSIDE);
    /*
    auto yunheroStrong1 = Sprite::createWithSpriteFrame(yunBG);
    yunheroStrong1->setPosition(Point(30,220));
    heroStrongBtn->addChild(yunheroStrong1);
    auto yunheroStrong2 = Sprite::createWithSpriteFrame(yunBG);
    yunheroStrong2->setPosition(Point(220,220));
    yunheroStrong2->setScale(-1, 1);
    heroStrongBtn->addChild(yunheroStrong2);
     */
    SpriteFrame * jieTuFrame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jietu1.png");
    auto jieTu1 = Sprite::createWithSpriteFrame(jieTuFrame1);
    jieTu1->setPosition(Point(124, 170));
    heroStrongBtn->addChild(jieTu1);
    /*
    // TTF配置
    TTFConfig adjust_ttfCfg;
    adjust_ttfCfg.fontSize = 22;
    adjust_ttfCfg.fontFilePath = FONENAME;
    Label* adjustLabel = Label::createWithSystemFont("提升武将",adjust_ttfCfg.fontFilePath, adjust_ttfCfg.fontSize);
    adjustLabel->setColor(Color3B(154,220,251));
    adjustLabel->setPosition(Point(125,220));
    adjustLabel->setContentSize(Size(heroStrongBtn->getContentSize().width, heroStrongBtn->getContentSize().height));
    heroStrongBtn->addChild(adjustLabel);
    
     */
    layerNode->addChild(heroStrongBtn);
    heroStrongBtn->setVisible(false);
    
    // TTF配置
    TTFConfig heroStrongDis_ttfCfg;
    heroStrongDis_ttfCfg.fontSize = 20;
    heroStrongDis_ttfCfg.fontFilePath = FONENAME;

    Label* heroStrongDisText = Label::createWithSystemFont("在将领界面点击强化武将，可以消耗将魂提升武将等级。",heroStrongDis_ttfCfg.fontFilePath, heroStrongDis_ttfCfg.fontSize);
    heroStrongDisText->setPosition(Point(135,50));
    heroStrongDisText->setContentSize(Size(200,100));
    heroStrongDisText->setColor(Color3B(241,226,133));
    heroStrongDisText->setDimensions(200, 100);
    heroStrongBtn->addChild(heroStrongDisText);
    
    // 更新装备  左侧
    equipmentStrongBtn = ControlButton::create(Scale9Sprite::createWithSpriteFrame(buttonBG));
    equipmentStrongBtn->setPosition(Point(layerNodeSize.width/4-50,layerNodeSize.height/4));
    equipmentStrongBtn->setPreferredSize(Size(270,250));
    equipmentStrongBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::OnEquipmentStrongButtonClicked),Control::EventType::TOUCH_UP_INSIDE);
    /*
    auto yunEquipStr1 = Sprite::createWithSpriteFrame(yunBG);
    yunEquipStr1->setPosition(Point(30,220));
    equipmentStrongBtn->addChild(yunEquipStr1);
    auto yunEquipStr2 = Sprite::createWithSpriteFrame(yunBG);
    yunEquipStr2->setPosition(Point(220,220));
    yunEquipStr2->setScale(-1, 1);
    equipmentStrongBtn->addChild(yunEquipStr2);
     */
    SpriteFrame * jieTuFrame2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jietu2.png");
    auto jieTu2 = Sprite::createWithSpriteFrame(jieTuFrame2);
    jieTu2->setPosition(Point(124, 170));
    equipmentStrongBtn->addChild(jieTu2);
    equipmentStrongBtn->setVisible(false);
    /*
    // TTF配置
    TTFConfig equipmentStrong_ttfCfg;
    equipmentStrong_ttfCfg.fontSize = 22;
    equipmentStrong_ttfCfg.fontFilePath = FONENAME;
    Label* equipmentStrongLabel = Label::createWithSystemFont("更新装备",adjust_ttfCfg.fontFilePath, adjust_ttfCfg.fontSize);
    equipmentStrongLabel->setColor(Color3B(154,220,251));
    equipmentStrongLabel->setPosition(Point(125,220));
    equipmentStrongLabel->setContentSize(Size(equipmentStrongBtn->getContentSize().width, equipmentStrongBtn->getContentSize().height));
    equipmentStrongBtn->addChild(equipmentStrongLabel);
    
    auto yunStr1 = Sprite::createWithSpriteFrame(yunBG);
    yunStr1->setPosition(Point(30,220));
    equipmentStrongBtn->addChild(yunStr1);
    auto yunStr2 = Sprite::createWithSpriteFrame(yunBG);
    yunStr2->setPosition(Point(220,220));
    yunStr2->setScale(-1, 1);
    equipmentStrongBtn->addChild(yunStr2);
     */
    layerNode->addChild(equipmentStrongBtn);
    // TTF配置
    TTFConfig equipmentStrongDis_ttfCfg;
    equipmentStrongDis_ttfCfg.fontSize = 20;
    equipmentStrongDis_ttfCfg.fontFilePath = FONENAME;
    Label* equipmentStrongDisText = Label::createWithSystemFont("通关精英副本可以获得装备，武将穿上装备后会更加强大。",equipmentStrongDis_ttfCfg.fontFilePath, equipmentStrongDis_ttfCfg.fontSize);
    equipmentStrongDisText->setPosition(Point(135,50));
    equipmentStrongDisText->setContentSize(Size(200,100));
    equipmentStrongDisText->setColor(Color3B(241,226,133));
    equipmentStrongDisText->setDimensions(200, 100);
    equipmentStrongBtn->addChild(equipmentStrongDisText);
    
    
    // 调整阵型  youce
    formationBtn = ControlButton::create(Scale9Sprite::createWithSpriteFrame(buttonBG));
    formationBtn->setPosition(Point(layerNodeSize.width/4 * 3+50,layerNodeSize.height/4));
    formationBtn->setPreferredSize(Size(270,250));
    formationBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::onBtnClickGoOn),Control::EventType::TOUCH_UP_INSIDE);

    
    /*
    auto yunformation1 = Sprite::createWithSpriteFrame(yunBG);
    yunformation1->setPosition(Point(30,220));
    formationBtn->addChild(yunformation1);
    auto yunformation2 = Sprite::createWithSpriteFrame(yunBG);
    yunformation2->setPosition(Point(220,220));
    yunformation2->setScale(-1, 1);
    formationBtn->addChild(yunformation2);
     */
    SpriteFrame * jieTuFrame3 = SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jietu3.png");
    auto jieTu3 = Sprite::createWithSpriteFrame(jieTuFrame3);
    jieTu3->setPosition(Point(124,170));
    formationBtn->addChild(jieTu3);
    
    // TTF配置
    TTFConfig formationDis_ttfCfg;
    formationDis_ttfCfg.fontSize = 20;
    formationDis_ttfCfg.fontFilePath = FONENAME;
    Label* formationDisText = Label::createWithSystemFont("用枪兵对骑兵，骑兵对弓兵，弓兵对枪兵，合理布置阵型事半功倍。",formationDis_ttfCfg.fontFilePath, formationDis_ttfCfg.fontSize);
    formationDisText->setPosition(Point(135,50));
    formationDisText->setContentSize(Size(200,100));
    formationDisText->setColor(Color3B(241,226,133));
    formationDisText->setDimensions(200, 100);
    formationBtn->addChild(formationDisText);
    /*
    // TTF配置
    TTFConfig heroStrong_ttfCfg;
    heroStrong_ttfCfg.fontSize = 22;
    heroStrong_ttfCfg.fontFilePath = FONENAME;
    Label* heroStrongLabel = Label::createWithSystemFont("调整阵型",heroStrong_ttfCfg.fontFilePath, heroStrong_ttfCfg.fontSize);
    //Label* heroStrongLabel = Label::createWithTTF(heroStrong_ttfCfg, "武将强化");
    heroStrongLabel->setColor(Color3B(154,220,251));
    heroStrongLabel->setPosition(Point(125,220));
    heroStrongLabel->setContentSize(Size(formationBtn->getContentSize().width, formationBtn->getContentSize().height));
    formationBtn->addChild(heroStrongLabel);
     */
    layerNode->addChild(formationBtn);
    formationBtn->setVisible(false);
    ///////////////////////////////////////////
    // 武将升星  youce
    upStarBtn = ControlButton::create(Scale9Sprite::createWithSpriteFrame(buttonBG));
    upStarBtn->setPosition(Point(layerNodeSize.width/2,layerNodeSize.height/2));
    upStarBtn->setPreferredSize(Size(270,250));
    upStarBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::upStarButtonClicked),Control::EventType::TOUCH_UP_INSIDE);
    
    SpriteFrame * jieTuFrame4 = SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jietu5.png");
    auto jieTu4 = Sprite::createWithSpriteFrame(jieTuFrame4);
    jieTu4->setPosition(Point(124,170));
    upStarBtn->addChild(jieTu4);
    upStarBtn->setVisible(false);
    // TTF配置
    TTFConfig upStarDis_ttfCfg;
    upStarDis_ttfCfg.fontSize = 20;
    upStarDis_ttfCfg.fontFilePath = FONENAME;
    Label* upStarDisText = Label::createWithSystemFont("通过列传、酒馆获得武将并提升星级，星级越高武将战力越强。",upStarDis_ttfCfg.fontFilePath, upStarDis_ttfCfg.fontSize);
    upStarDisText->setPosition(Point(135,50));
    upStarDisText->setContentSize(Size(200,100));
    upStarDisText->setColor(Color3B(241,226,133));
    upStarDisText->setDimensions(200, 100);
    upStarBtn->addChild(upStarDisText);
    layerNode->addChild(upStarBtn);
    ///////////////////////////////////////////
    // 武将升品  youce
    upQualityBtn = ControlButton::create(Scale9Sprite::createWithSpriteFrame(buttonBG));
    upQualityBtn->setPosition(Point(layerNodeSize.width/4 -50,layerNodeSize.height/2));
    upQualityBtn->setPreferredSize(Size(270,250));
    upQualityBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::upQualityButtonClicked),Control::EventType::TOUCH_UP_INSIDE);
    
    SpriteFrame * jieTuFrame5 = SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jietu6.png");
    auto jieTu5 = Sprite::createWithSpriteFrame(jieTuFrame5);
    jieTu5->setPosition(Point(124,170));
    upQualityBtn->addChild(jieTu5);
    
    // TTF配置
    TTFConfig upQualityDis_ttfCfg;
    upQualityDis_ttfCfg.fontSize = 20;
    upQualityDis_ttfCfg.fontFilePath = FONENAME;
    Label* upQualityDisText = Label::createWithSystemFont("武将升品后可以大幅提升战力。升品材料通过普通副本获得。",upQualityDis_ttfCfg.fontFilePath, upQualityDis_ttfCfg.fontSize);
    upQualityDisText->setPosition(Point(135,50));
    upQualityDisText->setContentSize(Size(200,100));
    upQualityDisText->setColor(Color3B(241,226,133));
    upQualityDisText->setDimensions(200, 100);
    upQualityBtn->addChild(upQualityDisText);
    layerNode->addChild(upQualityBtn);
    upQualityBtn->setVisible(false);
    
    ///////////////////////////////////////////
    // 获得好的装备  youce
    chengeEquBtn = ControlButton::create(Scale9Sprite::createWithSpriteFrame(buttonBG));
    chengeEquBtn->setPosition(Point(layerNodeSize.width/4 * 3+50,layerNodeSize.height/2));
    chengeEquBtn->setPreferredSize(Size(270,250));
    chengeEquBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::chengeEquButtonClicked),Control::EventType::TOUCH_UP_INSIDE);
    
    SpriteFrame * jieTuFrame6 = SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jietu4.png");
    auto jieTu6 = Sprite::createWithSpriteFrame(jieTuFrame6);
    jieTu6->setPosition(Point(124,170));
    chengeEquBtn->addChild(jieTu6);
    
    // TTF配置
    TTFConfig chengeEquDis_ttfCfg;
    chengeEquDis_ttfCfg.fontSize = 20;
    chengeEquDis_ttfCfg.fontFilePath = FONENAME;
    Label* chengeEquDisText = Label::createWithSystemFont("通关精英副本可以获得装备，武将穿上装备后会更加强大。",chengeEquDis_ttfCfg.fontFilePath, chengeEquDis_ttfCfg.fontSize);
    chengeEquDisText->setPosition(Point(135,50));
    chengeEquDisText->setContentSize(Size(200,100));
    chengeEquDisText->setColor(Color3B(241,226,133));
    
    chengeEquDisText->setDimensions(200, 100);
    chengeEquBtn->addChild(chengeEquDisText);
    layerNode->addChild(chengeEquBtn);
    chengeEquBtn->setVisible(false);
    
     
    //////////////////////////////////////////////////
    
    // 绘制良驹的Layer;
    layerHorseNode = LayerColor::create(Color4B(0, 0, 0, 0), designResolutionSize.width, designResolutionSize.height);
    layerHorseNode->setAnchorPoint(Point(0, 0));
    layerHorseNode->setPosition((visibleSize.width-designResolutionSize.width)/2,(visibleSize.height-designResolutionSize.height)/2);
    this->addChild(layerHorseNode);
    buttonHorse = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame));
    buttonHorse->setPreferredSize(Size(78,76));
    buttonHorse->setPosition(Point(layerNode->getContentSize().width-10,10));
    buttonHorse->setAnchorPoint(Point(1, 0));
    buttonHorse->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::onBtnClickGoOn),Control::EventType::TOUCH_UP_INSIDE);
    layerHorseNode->addChild(buttonHorse);
    
    // 头  2个斧子那图片
    Sprite *spTitle1 = Sprite::createWithSpriteFrameName("zd_shibai.png");
    spTitle1->setAnchorPoint(Point(0.5,1));
    spTitle1->setPosition(Point(layerNode->getContentSize().width/2,layerNode->getContentSize().height));
    layerHorseNode->addChild(spTitle1,2);
    // 斧头背面光特效
    Sprite* spGuang1 = Sprite::createWithSpriteFrameName("ty_languang.png");
    spGuang1->setAnchorPoint(Point(0.5, 0.5));
    spGuang1->setPosition(Point(spTitle1->getPositionX(),spTitle1->getPositionY()-spTitle->getContentSize().height/2));
    RotateBy* rotate1 = RotateBy::create(4, 360);
    RepeatForever* forever1 = RepeatForever::create(rotate1);
    spGuang1->runAction(forever1);
    layerHorseNode->addChild(spGuang1,1);
    
    auto diTiao = Sprite::createWithSpriteFrameName("zd_dikuang.png");
    diTiao->setAnchorPoint(Point(0.5,0.5));
    diTiao->setPosition(Point(521,359));
    layerHorseNode->addChild(diTiao);
    m_HorseLabel = Label::createWithSystemFont("未获得任何碎片",FONENAME, 35);
    //m_HorseLabel = Label::createWithTTF("未获得任何碎片",TTF_YOUYUAN, 35);
    m_HorseLabel->setPosition(Point(521,359));
    m_HorseLabel->setColor(ccc3(238,185,55));
    layerHorseNode->addChild(m_HorseLabel);
    
    
    auto textLabel = Label::createWithSystemFont("请主公获得额外的奖励：",FONENAME, 26);
    //auto textLabel = Label::createWithTTF("请主公获得额外的奖励：",TTF_YOUYUAN, 26);
    textLabel->setColor(ccc3(251,246,249));
    textLabel->setPosition(Point(370,290));
    textLabel->enableOutline(Color4B(0,0,0,255),2);
    layerHorseNode->addChild(textLabel);
    int Position[2] = {289,185};
    int verX = 160;
    for(int i = 0;i<4;i++)
    {
        m_HorsePai[i] = Sprite::createWithSpriteFrameName("zd_fanpai_fan.png");
        auto size = m_HorsePai[i] ->getContentSize();
        //sprite ->setPosition(Point(size.width/2,size.height/2));
        m_HorsePai[i] ->setPosition(Point(Position[0]+i*verX,Position[1]));
        layerHorseNode->addChild(m_HorsePai[i]);
        
        SpriteFrame *frame1 =SpriteFrameCache::getInstance()->getSpriteFrameByName("toumingkuang.png");
        ControlButton* button = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame1));
        button->setTag(i+1);
        button->setPreferredSize(Size(132,152));
        button->setPosition(Point(0+size.width/2,-4+size.height/2));
        button->setAnchorPoint(Point(0.5, 0.5));
        button->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVELoseLayer::OnBtnClickSelect),Control::EventType::TOUCH_UP_INSIDE);
        m_HorsePai[i]->addChild(button);
        
        m_HorseSpriteNode[i] = Sprite::createWithSpriteFrameName("ty_xingezi_1.png");
        m_HorseSpriteNode[i] ->setPosition(0+size.width/2,10+size.height/2);
        m_HorsePai[i]->addChild(m_HorseSpriteNode[i]);
        
        m_HorseNameLabel[i] = Label::createWithSystemFont("name",FONENAME, 20);
        //m_HorseNameLabel[i] = Label::createWithTTF("name", TTF_YOUYUAN, 20);
        m_HorseNameLabel[i]->setPosition(Point(0+size.width/2,-45+size.height/2));
        m_HorseNameLabel[i]->setColor(ccc3(238,185,55));
        m_HorsePai[i]->addChild(m_HorseNameLabel[i]);
        
        m_HorseNumLabel[i] = Label::createWithSystemFont("?",FONENAME, 20);
//        m_HorseNumLabel[i] = Label::createWithTTF("?", TTF_YOUYUAN, 20);
        m_HorseNumLabel[i]->setColor(ccc3(238,185,55));
        m_HorseNumLabel[i]->setAnchorPoint(Point(1,0.5));
        m_HorseNumLabel[i]->setPosition(Point(32+size.width/2,-15+size.height/2));
        m_HorsePai[i]->addChild(m_HorseNumLabel[i]);
        
        m_HorseSpritePai[i] = Sprite::createWithSpriteFrameName("zd_fanpai_zheng.png");
        m_HorseSpritePai[i] ->setPosition(Point(0+size.width/2,size.height/2));
        m_HorsePai[i]->addChild(m_HorseSpritePai[i]);
        
        
    }
    PVELoseGuid();
    return true;
}

void LQPVELoseLayer::upStarButtonClicked(Ref * sender, Control::EventType controlEvent)
{
    this->removeFromParent();
    LoadBattlefield::getInstance()->upStarGeneral();
}
void LQPVELoseLayer::upQualityButtonClicked(Ref * sender, Control::EventType controlEvent)
{
    this->removeFromParent();
    LoadBattlefield::getInstance()->upQualityGeneral();
}
void LQPVELoseLayer::chengeEquButtonClicked(Ref * sender, Control::EventType controlEvent)
{
    this->removeFromParent();
    LoadBattlefield::getInstance()->chengeEquip();
}
void LQPVELoseLayer::PVELoseGuid()
{
    int GuideType = LQBattleSession::getInstance()->m_fightType;
    isHaveGuide = false;
    if (GuideType == 1 ) //失败引导
    {
        const char* guide =HclcData::getInstance()->callLuaFunction("myTest");
        CCLOG("==guide== = %s  ",guide);
        string str = "";
        stringf(str, "%s",guide);
        isHaveGuide = (str != "1");
        CCLOG("%d",isHaveGuide);
    }
    
}

void LQPVELoseLayer::initUI(){
    layerHorseNode ->setVisible(false);
    layerNode ->setVisible(true);
    formationBtn->setVisible(true);
    Size layerNodeSize = layerNode->getContentSize();
    float button_height = layerNodeSize.height/4;
    float button_width[3] = {layerNodeSize.width/4-50,layerNodeSize.width/2,layerNodeSize.width/4 * 3+50};
    string fileStr = LoadBattlefield::getInstance()->getValue();
    std::vector<std::string> guide;
    split(fileStr, ';', guide);
    for (int i = 0; i < guide.size(); i++) {
        int idx = atoi(guide[i].c_str());
        auto button = getButton(idx);
        button->setPosition(Point(button_width[i],button_height));
        button->setVisible(true);
        if (idx == GETEQUIP_BUTTON && isHaveGuide)
        {
            int guide = UserDefault::getInstance()->getIntegerForKey("FIGHTLOSEGUIDE");
            if (guide > 0)
            {
                Size visibleSize =  Director::getInstance()->getVisibleSize();
                float verx =  (visibleSize.width - designResolutionSize.width)/2;
                float very =  (visibleSize.height - designResolutionSize.height)/2;

                auto guideLayer = BattleGuideLayer::create();
                guideLayer->setTag(_TAG_GUIDE);
                guideLayer->setDataToLayer("主公，你需要去精英副本获得一些更好的装备。", Point(button_width[i] + verx,button_height + very), Point(layerNodeSize.width/4-250 + verx,button_height + 150 + very));
                this->getParent()->addChild(guideLayer,9999999);
                UserDefault::getInstance()->setIntegerForKey("FIGHTLOSEGUIDE",1);
            }
        }
    }
    
    
}
ControlButton* LQPVELoseLayer::getButton(int idx)
{
    /*
     #define UPGEN_BUTTON      1     //武将升级
     #define UPEQUIP_BUTTON    2     //装备升级
     #define GETEQUIP_BUTTON   3     //获得装备
     #define UPQUALITY_BUTTON  4     //武将生品
     #define UPSTAR_BUTTON     5     //武将升星
     ControlButton* heroStrongBtn;
     ControlButton* equipmentStrongBtn;
     ControlButton* formationBtn;
     ControlButton* upStarBtn;
     ControlButton* upQualityBtn;
     ControlButton* chengeEquBtn;
     */
    switch (idx) {
        case UPGEN_BUTTON:
            return heroStrongBtn;
        case UPEQUIP_BUTTON:
            return equipmentStrongBtn;
        case GETEQUIP_BUTTON:
            return chengeEquBtn;
        case UPQUALITY_BUTTON:
            return upQualityBtn;
        case UPSTAR_BUTTON:
            return upStarBtn;
        default:
            return heroStrongBtn;
            break;
    }
}
void LQPVELoseLayer::initUI(string str){

    layerHorseNode ->setVisible(true);
    layerNode ->setVisible(false);
    
    if (str == "")
    {
        m_HorseLabel->setString("未获得任何碎片");
    }
    else{
        string s = "恭喜获得"+str;
        m_HorseLabel->setString(s);
    }
    buttonHorse->setVisible(false);
}
void LQPVELoseLayer::OnBtnClickSelect(Ref * sender, Control::EventType controlEvent)
{
    if (!isFanPai)
    {
        return;
    }
    auto node = (Node*)sender;
    auto tag = node->getTag();
    this->tag = tag;
    CCLOG("点击第%d个卡牌",tag);

    LQSocketBuffer* buffer = NULL;
    HorseBattleAwardRequest* request2 = new HorseBattleAwardRequest();
    buffer = request2->toMessage();
    ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
}


void LQPVELoseLayer::initAwardItem(string awardItems,int recStar)
{
    heroStrongBtn->setVisible(false);
    equipmentStrongBtn->setVisible(false);
    Point desPoint = awardlayer->getPosition();
    Size viewSize = awardlayer->getContentSize();
    string awardStr = awardItems;
    if (awardStr == "") {
        return;
    }
    int exp;
    int cash;
    int soul;
    int level;
    std::vector<std::string> Award3;
    split(awardStr, '#', Award3);
    for (int i = 0; i < Award3.size(); i++) {
        string types = Award3.at(i);
        CCLOG("types = %s",types.c_str());
        if(i==0) { // 金币 经验  将魂
            std::vector<std::string> itemData;
            split(types, ',', itemData);
            exp = atoi(itemData.at(0).c_str());
            cash = atoi(itemData.at(1).c_str());
            soul = atoi(itemData.at(2).c_str());
            level = atoi(itemData.at(3).c_str());
        } else { // 奖励单元
            float x = 0;
            float y = viewSize.height/2;
            std::vector<std::string> AwardItemStrs;
            split(types, ';', AwardItemStrs);
            for (int i = 0; i < AwardItemStrs.size(); i++) {
                string items = AwardItemStrs.at(i);
                std::vector<std::string> itemData;
                split(items, ',', itemData);
                string name = itemData.at(0).c_str();
                int quality = atoi(itemData.at(1).c_str());
                int number = atoi(itemData.at(2).c_str());
                string filename = itemData.at(3).c_str();
                int type = atoi(itemData.at(4).c_str());
                
                std::string iconname;
                if (type == 99 || type == 10) {//卡牌
                    
                    stringf(iconname, "res/head/%s",filename.c_str());
                }else{
                    stringf(iconname, "res/icons/%s",filename.c_str());
                }
                //float scale = 1,float fontSize = 30, Color3B fontcolor = Color3B::YELLOW
                Scale9Sprite* icon = createItemWithFileName(iconname, quality, number,1,22,Color3B(184,255,100));
                icon->setAnchorPoint(Point(0, 0.5));
                icon->setPosition(x, y);
                awardlayer->addChild(icon);
                if (type >= 11 && type <= 14)
                {
                    std::string iconN;
                    stringf(iconN, "ty_suipian_%d.png",quality);
                    auto suiPin = Sprite::createWithSpriteFrameName(iconN);
                    suiPin ->setPosition(Point(icon->getContentSize().width/2,icon->getContentSize().height/2));
                    icon->addChild(suiPin);
                    auto suiSprite = Sprite::createWithSpriteFrameName("ty_suipainbiao.png");
                    suiSprite ->setPosition(Point(icon->getContentSize().width/5,icon->getContentSize().height*4/5));
                    icon->addChild(suiSprite);
                }

                Label* nameLabel = Label::createWithSystemFont(name.c_str(),FONENAME, 18);
                //              Label* nameLabel = Label::createWithSystemFont(name.c_str(), TTF_YOUYUAN, 18);
                nameLabel->setAnchorPoint(Point(0.5, 0));
                nameLabel->setPosition(Point(icon->getContentSize().width/2-10, 0));
                //测试用名字
                //icon->addChild(nameLabel);
                nameLabel->setColor(Color3B(184,255,100));
                nameLabel->enableOutline(Color4B(0, 0, 0, 255),2);
                
                x += icon->getContentSize().width*1.1;
            }
            awardlayer->setContentSize(Size(x, viewSize.height));
            
        }
    }
    Node* node = awardlayer->getParent();
    awardlayer->removeFromParentAndCleanup(false);
    auto m_contentView = ScrollView::create(viewSize);
    m_contentView->setContainer(awardlayer);
    m_contentView->setContentSize(awardlayer->getContentSize());
    m_contentView->setPosition(desPoint);
    node->addChild(m_contentView);
    m_contentView->setContentOffset(m_contentView->maxContainerOffset());
    m_contentView->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
}

void LQPVELoseLayer::setDataToUI(string str)
{
    CCLOG("收到数据%s",str.c_str());
    if(str == "")
    {
        buttonHorse->setVisible(true);
        return;
    }
    int count = 0;
    std::vector<std::string> AwardItemStrs;
    split(str, ';', AwardItemStrs);
    for (int i = 0; i < AwardItemStrs.size(); i++) {
        string items = AwardItemStrs.at(i);
        std::vector<std::string> itemData;
        split(items, ',', itemData);
        if (i == 0)  //获得的奖励  放在点击位置
        {
            this->runActionForCard(this->tag-1,0, itemData);
        }
        else
        {
            if (count == this->tag-1)
            {
                count ++;
            }
            
            this->runActionForCard(count,0.6, itemData);
            count++;
        }
        //float scale = 1,float fontSize = 30, Color3B fontcolor = Color3B::YELLOW
    }
    buttonHorse->setVisible(true);
}
void LQPVELoseLayer::runActionForCard(int actionID,float time,std::vector<std::string> data)//翻牌的特效
{
    auto delay = DelayTime::create(time);
    auto mb = ScaleTo::create(0.3,0,1);
    auto mt = ScaleTo::create(0.3,1,1);
    auto action = Sequence::create(
                                   delay,
                                   mb,
                                   CallFunc::create(CC_CALLBACK_0(LQPVELoseLayer::callback1,this,actionID,data)),
                                   mt,
                                   NULL);
    auto layer = m_HorsePai[actionID];
    layer ->runAction(action);
}
void LQPVELoseLayer::callback1(int actionID, std::vector<std::string> itemData)
{
    string name = itemData.at(0).c_str();
    int quality = atoi(itemData.at(1).c_str());
    int number = atoi(itemData.at(2).c_str());
    string filename = itemData.at(3).c_str();
    int type = atoi(itemData.at(4).c_str());
    //CCLOG("数据%s",items.c_str());
    CCLOG("数据%s",name.c_str());
    CCLOG("数据%d",quality);
    CCLOG("数据%d",number);
    CCLOG("数据%s",filename.c_str());
    CCLOG("数据%d",type);
    std::string iconname;
    
    if (type == 99 || type == 10) {//卡牌
        
        stringf(iconname, "res/head/%s",filename.c_str());
    }else{
        stringf(iconname, "res/icons/%s",filename.c_str());
    }
    auto sprite = Sprite::create(iconname);
    auto node = m_HorseSpriteNode[actionID];
    auto pai = m_HorseSpritePai[actionID];
    auto nameLable = m_HorseNameLabel[actionID];
    nameLable->setString(name);
    pai ->setVisible(false);
    std::string iconN;
    stringf(iconN, "ty_xingezi_%d.png",quality);
    node->setSpriteFrame(iconN);
    node->addChild(sprite);
    auto con = node->getContentSize();
    sprite->setPosition(Point(con.width/2,con.height/2));
    m_HorseNumLabel[actionID]->setString("X"+ itemData.at(2));
    if (actionID == this->tag - 1)
    {
        isFanPai = false;
    }
}

void LQPVELoseLayer::onEnter()
{
    Layer::onEnter();
}
void LQPVELoseLayer::onExit()
{
    Layer::onExit();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

//调整阵型
void LQPVELoseLayer::OnChangeFormationButtonClicked(Ref * sender, Control::EventType controlEvent)
{
    this->removeFromParent();
    LoadBattlefield::getInstance()->changeFormation();
}

//装备强化
void LQPVELoseLayer::OnEquipmentStrongButtonClicked(Ref * sender, Control::EventType controlEvent)
{
    this->removeFromParent();
    LoadBattlefield::getInstance()->quipmentStrong();
}

//武将强化
void LQPVELoseLayer::OnHeroStrongButtonClicked(Ref * sender, Control::EventType controlEvent)
{
    this->removeFromParent();
    LoadBattlefield::getInstance()->heroStrong();
}

void LQPVELoseLayer::onBtnClickGoOn(Ref * sender, Control::EventType controlEvent)
{
    // 继续游戏
 
    if (getFightType() == 0) {
        LoadBattlefield::getInstance()->combatCreateRole();
    } else {
        LoadBattlefield::getInstance()->combatBack();
    }
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    //Director::getInstance()->getTextureCache()->removeAllTextures();
}

bool LQPVELoseLayer::onTouchBegan(Touch* touch, Event  *event){
    return true;
}
void LQPVELoseLayer::onTouchMoved(Touch* touch, Event  *event){}
void LQPVELoseLayer::onTouchEnded(Touch* touch, Event  *event){}
void LQPVELoseLayer::onTouchCancelled(Touch *touch, Event *event){}