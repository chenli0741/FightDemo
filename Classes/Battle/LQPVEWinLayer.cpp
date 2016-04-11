//
//  LQPVELoseLayer.cpp
//  game
//
//  Created by lequdev1 on 14-7-26.
//
//

#include "LQPVEWinLayer.h"
#include "LQBattleSession.h"
#include "HorseBattleAwardRequest.h"
#include "LQHelp.h"
#include "AudioManager.h"
#include "mesage/HorseBattleFightResultRequest.h"
#include "LQZoneInfoData.h"
//using namespace cocosbuilder;
USING_NS_CC;
LQPVEWinLayer::LQPVEWinLayer()
{

}

LQPVEWinLayer::~LQPVEWinLayer()
{

}
 
LQPVEWinLayer* LQPVEWinLayer::createLayer()
{
    LQPVEWinLayer *ret = new LQPVEWinLayer();
    if (ret && ret->init()){
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool LQPVEWinLayer::init()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(LQPVEWinLayer::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LQPVEWinLayer::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LQPVEWinLayer::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    
    Size visibleSize =  Director::getInstance()->getVisibleSize();
    
    // 半透明背景
    LayerColor *cbg = LayerColor::create(Color4B(0, 0, 0, 200), visibleSize.width*2, visibleSize.height*2);
    cbg->setAnchorPoint(Point(0,0));
    cbg->setPosition(0, 0);
    this->addChild(cbg);
        

    // 绘制UI的Layer;
    layerNode = LayerColor::create(Color4B(0, 0, 0, 0), designResolutionSize.width, designResolutionSize.height);
    layerNode->setAnchorPoint(Point(0, 0));
    layerNode->setPosition((visibleSize.width-designResolutionSize.width)/2,(visibleSize.height-designResolutionSize.height)/2);
    this->addChild(layerNode);
    
    
    SPC uiSpConfig[] = {
        {Point(0.5,1),
         Point(layerNode->getContentSize().width/2,layerNode->getContentSize().height-50),
         "zd_shengli.png"},
        {},
    };
    
    
    
    std::string LTJM_PLIST = "files/ccbiui/ccbires/resv4/V4ZHANDOU.plist";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST.c_str());
    std::string LTJM_PLIST1 = "files/ccbiui/ccbires/resv4/V4UNIVERSAL.plist";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST1.c_str());
    std::string LTJM_PLIST2 = "files/ccbiui/ccbires/resv4/V4GRID.plist";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST2.c_str());
    awardlayer = Layer::create();  //获得物品的层
    awardlayer->setPosition(Point(250,70));
    awardlayer->setContentSize(Size(480,100));
    layerNode->addChild(awardlayer);
    
    
    
    
    
    return true;
}
void LQPVEWinLayer::initUI(){ //非良驹争夺战


    //普通战斗胜利界面绘制
    // 继续按钮 退出战斗那个
    SpriteFrame *frame =SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jixu.png");
    auto autobutton = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame));
    autobutton->setPreferredSize(Size(78,76));
    autobutton->setPosition(Point(layerNode->getContentSize().width-100,20));
    autobutton->setAnchorPoint(Point(1, 0));
    autobutton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVEWinLayer::onBtnClickGoOn),Control::EventType::TOUCH_UP_INSIDE);
    layerNode->addChild(autobutton);
    
    // 头  2个斧子那图片
    Sprite *spTitle = Sprite::createWithSpriteFrameName("zd_shengli.png");
    spTitle->setAnchorPoint(Point(0.5,1));
    spTitle->setPosition(Point(layerNode->getContentSize().width/2,layerNode->getContentSize().height-50));
    layerNode->addChild(spTitle,2);
    
    //斧头背面光特效
    Sprite* spGuang = Sprite::createWithSpriteFrameName("ty_fangguang.png");
    spGuang->setAnchorPoint(Point(0.5, 0.5));
    spGuang->setPosition(Point(spTitle->getPositionX(),spTitle->getPositionY()-spTitle->getContentSize().height/2));
    RotateBy* rotate = RotateBy::create(8, 360);
    RepeatForever* forever = RepeatForever::create(rotate);
    spGuang->runAction(forever);
    layerNode->addChild(spGuang,1);
    
    
    // 上面显示等级 金币 啥的底条
    Scale9Sprite *dTiao = Scale9Sprite::createWithSpriteFrameName("zd_dikuang.png");
    dTiao->setAnchorPoint(Point(0,0));
    dTiao->setPreferredSize(Size(600,52));
    dTiao->setPosition(Point(211,360));
    //dTiao->setPosition(Point(layerNode->getContentSize().width/2,layerNode->getContentSize().height-200));
    
    layerNode->addChild(dTiao,3);
    //lv 
    auto Lvlabel = _createFont(nullptr,"LV  ",26,ccc3(238,185,55),Point(280,385),Point(0,0.5));
    layerNode->addChild(Lvlabel,4);
    Sprite *ExeTitle = Sprite::createWithSpriteFrameName("EXP.png");
    ExeTitle->setAnchorPoint(Point(0,0.5));
    ExeTitle->setPosition(Point(420,385));
    layerNode->addChild(ExeTitle,4);
    
    
    Sprite *YuTitle = Sprite::createWithSpriteFrameName("a_ty_jinbi.png");
    YuTitle->setAnchorPoint(Point(0,0.5));
    YuTitle->setPosition(Point(560,385));
    layerNode->addChild(YuTitle,4);
    
    Sprite *JhTitle = Sprite::createWithSpriteFrameName("a_ty_jianghun.png");
    JhTitle->setAnchorPoint(Point(0,0.5));
    JhTitle->setPosition(Point(700,385));
    layerNode->addChild(JhTitle,4);
    
    double wid[4];
    wid[0] = Lvlabel->getContentSize().width;
    wid[1] = ExeTitle->getContentSize().width;
    wid[2] = YuTitle->getContentSize().width;
    wid[3] = JhTitle->getContentSize().width;
    for (int i = 0; i<4; i++){
        // 显示上面那等级 金币 啥的地矿
        m_obtainLabel[i] = _createFont(layerNode,"0",30,Color3B::WHITE,Point(145*i+280+wid[i], 385),Point(0,0.5));
        layerNode->addChild(m_obtainLabel[i] ,4);
    }
    
    for (int i = 0; i<3; i++){        // 显示星级的底条
        Scale9Sprite *dSkuang = Scale9Sprite::createWithSpriteFrameName("zd_dikuang2.png");
        dSkuang->setAnchorPoint(Point(0.5,0.5));
        dSkuang->setPosition(layerNode->getContentSize().width/2, layerNode->getContentSize().height-360 - i*60);
        dSkuang->setContentSize(Size(300,50));
        layerNode->addChild(dSkuang,4);
        
        
        m_describeLabel[i] = _createFont(dSkuang, "星星获得等级的描述", 25, Color3B::WHITE,Point(dSkuang->getContentSize().width/2, dSkuang->getContentSize().height/2), Point(0.5,0.5));
        dSkuang->addChild(m_describeLabel[i],4);
        
    }
    
    // 最下面的那条线
    Sprite *line = Sprite::createWithSpriteFrameName("ty_xiantiao.png");
    line->setAnchorPoint(Point(0.5,0.5));
    line->setPosition(Point(layerNode->getContentSize().width/2,50));
    layerNode->addChild(line);
    
    
    
    
    
    ///////////////////////////////////////////////////////

//    layerHorseNode ->setVisible(false);
//    layerNode ->setVisible(true);
}
void LQPVEWinLayer::initBossUI(LQHeroUserDataVector gameData,string str)
{
    
    string des[2] ={"伤害：","获得："};
    int x[2] = {150,200};
    Size visibleSize =  Director::getInstance()->getVisibleSize();
    // 继续按钮 退出战斗那个
    SpriteFrame *frame =SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jixu.png");
    auto autobutton = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame));
    autobutton->setPreferredSize(Size(78,76));
    autobutton->setPosition(Point(layerNode->getContentSize().width-100,20));
    autobutton->setAnchorPoint(Point(1, 0));
    autobutton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVEWinLayer::onBtnClickGoOn),Control::EventType::TOUCH_UP_INSIDE);
    layerNode->addChild(autobutton);
    
    // 头  2个斧子那图片
    Sprite *spTitle = Sprite::createWithSpriteFrameName("nm_jieshu.png");
    spTitle->setAnchorPoint(Point(0.5,1));
    spTitle->setPosition(Point(layerNode->getContentSize().width/2,layerNode->getContentSize().height-50));
    layerNode->addChild(spTitle,2);
    
    //斧头背面光特效
    Sprite* spGuang = Sprite::createWithSpriteFrameName("ty_fangguang.png");
    spGuang->setAnchorPoint(Point(0.5, 0.5));
    spGuang->setPosition(Point(spTitle->getPositionX(),spTitle->getPositionY()-spTitle->getContentSize().height/2));
    RotateBy* rotate = RotateBy::create(8, 360);
    RepeatForever* forever = RepeatForever::create(rotate);
    spGuang->runAction(forever);
    layerNode->addChild(spGuang,1);

    layerDialog = LayerColor::create(Color4B(0, 0, 0, 200), visibleSize.width, visibleSize.width);
    layerDialog->setAnchorPoint(Point(0,0));
    layerDialog->setPosition(0, 0);
    layerNode->addChild(layerDialog,10);
    layerDialog->setVisible(false);
    LayerColor *cbg = LayerColor::create(Color4B(0, 0, 0, 0), 2*visibleSize.width, 2*visibleSize.height);
    cbg->setAnchorPoint(Point(0.5,0.5));
    cbg->setPosition(-visibleSize.width/2,-visibleSize.height/2);
    layerDialog->addChild(cbg,1);

    Scale9Sprite *dTiao = Scale9Sprite::createWithSpriteFrameName("ty_zuodi.png");
    dTiao->setAnchorPoint(Point(0,0));
    dTiao->setPreferredSize(Size(584,450));
    dTiao->setPosition(Point(227,91));
    layerDialog->addChild(dTiao,3);
    Scale9Sprite *dTiao2 = Scale9Sprite::createWithSpriteFrameName("ty_zuodi.png");
    dTiao2->setAnchorPoint(Point(0,0));
    dTiao2->setPreferredSize(Size(560,340));
    dTiao2->setPosition(Point(239,106));
    layerDialog->addChild(dTiao2,3);
    for (int i = 0;i < 3; i++)
    {
        for (int j = 0;j<4;j++)
        {
            Sprite* sprite = Sprite::createWithSpriteFrameName("mosha.png");
            sprite->setAnchorPoint(Point(0.5, 0.5));
            sprite->setPosition(Point(361 + (156 * i),360 - (j * 54)));
            layerDialog->addChild(sprite,4);
        }
    }
    SpriteFrame *frame4 =SpriteFrameCache::getInstance()->getSpriteFrameByName("ty_guanbicha.png");
    auto Diabutton = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame4));
    Diabutton->setPreferredSize(Size(56,56));
    Diabutton->setPosition(Point(777,510));
    Diabutton->setAnchorPoint(Point(0.5, 0.5));
    Diabutton->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVEWinLayer::onBtnClickDialogClose),Control::EventType::TOUCH_UP_INSIDE);
    layerDialog->addChild(Diabutton,5);
    int dialogX[2] = {390,654};
    int dialogY[2] = {496,467};
    int color[2][3] = {{75,140,222},{233,83,84}};
    string titleText[2] = {"我方","敌方"};
    for (int i = 0;i<2;i++)
    {
        auto label = _createFont(layerDialog, titleText[i], 26, ccc3(color[i][0],color[i][1],color[i][2]), Point(dialogX[i],dialogY[0]), Point(0.5,0.5));
        layerDialog->addChild(label,4);
        auto label2 = _createFont(layerDialog, "输出伤害", 26, ccc3(color[i][0],color[i][1],color[i][2]), Point(dialogX[i],dialogY[1]), Point(0.5,0.5));
        layerDialog->addChild(label2,4);
    }
    
    /////////////////////////////////////////////////////
    // 底条
    Scale9Sprite *diTiao = Scale9Sprite::createWithSpriteFrameName("zd_dikuang.png");
    diTiao->setAnchorPoint(Point(0,0));
    diTiao->setPreferredSize(Size(600,52));
    diTiao->setPosition(Point(211,360));
//    diTiao->setPosition(Point(layerNode->getContentSize().width/2,layerNode->getContentSize().height-200));
    layerNode->addChild(diTiao,3);
    auto relabel = _createFont(nullptr, "关卡进度", 26, ccc3(238,185,55), Point(270,384), Point(0,0.5));
    layerNode->addChild(relabel,4);
    
    
    SpriteFrame *frame2 =SpriteFrameCache::getInstance()->getSpriteFrameByName("ty_xiaoanniu_1.png");
    auto button = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame2));
    button->setPreferredSize(Size(80,48));
    button->setPosition(Point(792,384));
    button->setAnchorPoint(Point(0.5, 0.5));
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVEWinLayer::onBtnClickDialog),Control::EventType::TOUCH_UP_INSIDE);
    layerNode->addChild(button,4);
    
    auto Bulabel = _createFont(layerNode, "详情", 20, ccc3(242,199,36), Point(792,384), Point(0.5,0.5));
    layerNode->addChild(Bulabel,4);

    
    std::vector<std::string> Award;
    split(str, '$', Award);
    auto Vlabel = _createFont(layerNode, "+"+Award.at(0)+"%", 26, ccc3(255,255,255), Point(378,384), Point(0,0.5));
    layerNode->addChild(Vlabel,4);
    int press = atoi(Award.at(0).c_str());
    Sprite *pline = Sprite::createWithSpriteFrameName("nm_jindutiao.png");
    //int lineWidth = pline->boundingBox().size.width;
    Scale9Sprite *dikuang = Scale9Sprite::createWithSpriteFrameName("ty_dingjidi.png");
    dikuang->setAnchorPoint(Point(0,0.5));
    dikuang->setPosition(Point(480,381));
    dikuang->setPreferredSize(Size(270,26));
    layerNode->addChild(dikuang,4);
    auto progress = ProgressTimer::create(pline);
    progress->setBarChangeRate(Point(1, 0));    //设置进度条的长度和高度开始变化的大小
    progress->setMidpoint(Point(0, 0));
    progress->setType(ProgressTimer::Type::BAR);    //设置进度条为水平
    progress->setPercentage(press);    //设置初始百分比的值
    progress->setAnchorPoint(Point(0,0.5));
//    progress->setPercentage(100);    //设置初始百分比的值
    progress->setPosition(Point(482,384));    //放置进度条位置
    layerNode->addChild(progress,4);    //加入Layer中
//    CCLOG( "战斗结算" );

    LQHeroUserDataVector::iterator it;
    int Cardhirt = 0;
    int MaxHurt = 0;
    for(it=gameData.begin();it!=gameData.end();++it)
    {
        LQHeroUserData* hdata = (*it);
        
//        CCLOG( "英雄[%d %s] %d", hdata->getResIDC(), hdata->getName().c_str(), hdata->getSquadDirection() );
//        CCLOG( "    剩余血量 HP:%d", hdata->getSquadHpValue() );
//        CCLOG( "    攻击的伤害 DpsTotal:%d", hdata->getDpsTotal() );
//        CCLOG( "    杀死敌人数量 KillTotal:%d", hdata->getKillTotal());
//        CCLOG("武将的icon%s",hdata->getHeadID().c_str());
        if (hdata->getSquadDirection() == 0)
        {
            Cardhirt = Cardhirt + hdata->getDpsTotal();
        }
        if (MaxHurt < hdata->getDpsTotal())
        {
            MaxHurt = hdata->getDpsTotal();
        }
    }
    int idx[2] = {0,0};
    string xue[2] = {"nm_wfshanghai.png","nm_dfshanghai.png"};
    int PXueX[2] = {346,680};
    int PIconX[2] = {290,740};
    int HurtX[2] = {386,590};
    int scale[2] ={1,-1};
    for(it=gameData.begin();it!=gameData.end();++it)
    {
        LQHeroUserData* hdata = (*it);
        
        int SquadDirection = hdata->getSquadDirection();
        int quality = hdata->getQuality();
        SpriteFrame *frameQ =SpriteFrameCache::getInstance()->getSpriteFrameByName(getContentByQulity(quality));
        auto kuang = Sprite::createWithSpriteFrame(frameQ);
        kuang ->setPosition(Point(PIconX[SquadDirection],400 - (idx[SquadDirection] * 62)));
        kuang->setScale(0.62f);
        layerDialog->addChild(kuang,4);
        auto icon = Sprite::create("res/head/"+hdata->getHeadID());
        auto size = kuang->getContentSize();
        icon->setPosition(Point(size.width/2,size.height/2));
        kuang->addChild(icon);
        Scale9Sprite *diTiao = Scale9Sprite::createWithSpriteFrameName("flag_fangguangdi.png");
        diTiao->setAnchorPoint(Point(0,0));
        diTiao->setPreferredSize(Size(152,9));
        diTiao->setPosition(Point(PXueX[SquadDirection] - scale[SquadDirection]*2,382 - (idx[SquadDirection] * 62)));
        diTiao->setScaleX(scale[hdata->getSquadDirection()]);
        layerDialog->addChild(diTiao,4);

        Sprite *pline = Sprite::createWithSpriteFrameName(xue[SquadDirection]);
        auto progress = ProgressTimer::create(pline);
        progress->setBarChangeRate(Point(1, 0));    //设置进度条的长度和高度开始变化的大小
        progress->setMidpoint(Point(0, 0));
        progress->setType(ProgressTimer::Type::BAR);    //设置进度条为水平
        progress->setPercentage(((float)(hdata->getDpsTotal())/MaxHurt) * 100);    //设置初始百分比的值
        progress->setAnchorPoint(Point(0,0.5));
        progress->setPosition(Point(PXueX[SquadDirection],386 -(idx[SquadDirection] * 62) )); //放置进度条位置
        progress->setScaleX(scale[hdata->getSquadDirection()]);
        layerDialog->addChild(progress,4);    //加入Layer中
        string hurt;
        stringf(hurt,"%d",(hdata->getDpsTotal()/100));
        
        auto HurtLabel = _createFont(layerDialog, hurt, 18, ccc3(251,228,192), Point(HurtX[SquadDirection],402 -(idx[SquadDirection] * 62)), Point(0,0.5));
        layerDialog->addChild(HurtLabel,4);

        idx[SquadDirection]++;
    }
    string value[2] = {"",""};
    stringf(value[0], "%d",(Cardhirt/100));
    for (int i = 0; i<1; i++){        // 显示星级的底条
        Scale9Sprite *dSkuang = Scale9Sprite::createWithSpriteFrameName("zd_dikuang2.png");
        dSkuang->setAnchorPoint(Point(0.5,0.5));
        dSkuang->setPosition(layerNode->getContentSize().width/2, layerNode->getContentSize().height-380 - i*70);
        dSkuang->setContentSize(Size(300,50));
        layerNode->addChild(dSkuang,4);
        
        auto label = _createFont(dSkuang, des[i], 25, ccc3(242,199,36),Point(dSkuang->getContentSize().width/4, dSkuang->getContentSize().height/2), Point(0.5,0.5));
        dSkuang->addChild(label,4);
        
        auto Vlabel = _createFont(dSkuang, value[i], 25, ccc3(242,199,36), Point(x[i], dSkuang->getContentSize().height/2), Point(0.5,0.5));
        dSkuang->addChild(Vlabel,4);
    }
    
    for (int i=0; i<4; i++){
        m_obtainLabel[i] = NULL;
    }
    
    if (Award.size() > 1){
        //CCLOG("==============%s",Award.at(1).c_str());
        drawGetItemList(Award.at(1));
    }
    


}
string LQPVEWinLayer::getContentByQulity(int q)
{
    if (q == 1)
    {
        return "ty_xingezi_1.png";
    }
    else if (q >= 2 && q<= 3)
    {
        return "ty_xingezi_2.png";
    }
    else if (q >= 4 && q <= 6)
    {
        return "ty_xingezi_3.png";
    }
    else if (q >= 7 && q <= 9)
    {
        return "ty_xingezi_4.png";
    }
    else if (q == 10)
    {
        return "ty_xingezi_5.png";
    }
}

void LQPVEWinLayer::initUI(string str){
    Size visibleSize =  Director::getInstance()->getVisibleSize();
    SpriteFrame *frame =SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_jixu.png");
    // 绘制良驹的Layer;
    layerHorseNode = LayerColor::create(Color4B(0, 0, 0, 0), designResolutionSize.width, designResolutionSize.height);
    layerHorseNode->setAnchorPoint(Point(0, 0));
    layerHorseNode->setPosition((visibleSize.width-designResolutionSize.width)/2,(visibleSize.height-designResolutionSize.height)/2);
    this->addChild(layerHorseNode);
    buttonHorse = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame));
    buttonHorse->setPreferredSize(Size(78,76));
    buttonHorse->setPosition(Point(layerNode->getContentSize().width-10,10));
    buttonHorse->setAnchorPoint(Point(1, 0));
    buttonHorse->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVEWinLayer::onBtnClickGoOn),Control::EventType::TOUCH_UP_INSIDE);
    layerHorseNode->addChild(buttonHorse);
    SpriteFrame *frameSao =SpriteFrameCache::getInstance()->getSpriteFrameByName("zd_saodang.png");
    buttonOne = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frameSao));
    //buttonOne->setPreferredSize(Size(78,75));
    buttonOne->setPreferredSize(Size(78,76));
    buttonOne->setPosition(Point(layerNode->getContentSize().width-10,150));
    buttonOne->setAnchorPoint(Point(1, 0));
    buttonOne->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVEWinLayer::OnBtnSendData),Control::EventType::TOUCH_UP_INSIDE);
    buttonOne->setVisible(false);
    layerHorseNode->addChild(buttonOne);
    
    // 头  2个斧子那图片
    Sprite *spTitle1 = Sprite::createWithSpriteFrameName("zd_shengli.png");
    spTitle1->setAnchorPoint(Point(0.5,1));
    spTitle1->setPosition(Point(layerNode->getContentSize().width/2,layerNode->getContentSize().height));
    layerHorseNode->addChild(spTitle1,2);
    
    //斧头背面光特效
    Sprite* spGuang1 = Sprite::createWithSpriteFrameName("ty_languang.png");
    spGuang1->setAnchorPoint(Point(0.5, 0.5));
    spGuang1->setPosition(Point(spTitle1->getPositionX(),spTitle1->getPositionY() - spTitle1->getContentSize().height/2));
    RotateBy* rotate1 = RotateBy::create(4, 360);
    RepeatForever* forever1 = RepeatForever::create(rotate1);
    spGuang1->runAction(forever1);
    layerHorseNode->addChild(spGuang1,1);
    
    auto diTiao = Sprite::createWithSpriteFrameName("zd_dikuang.png");
    diTiao->setAnchorPoint(Point(0.5,0.5));
    //diTiao->setPreferredSize(Size(152,9));
    diTiao->setPosition(Point(521,359));
    layerHorseNode->addChild(diTiao,2);
    
    m_HorseLabel = _createFont(layerHorseNode, "未获得任何碎片", 35, ccc3(238,185,55), Point(521,359), Point(0.5,0.5));
    layerHorseNode->addChild(m_HorseLabel,2);
    
    auto textLabel = _createFont(layerHorseNode, "请主公获得额外的奖励：", 26, ccc3(251,246,249), Point(370,290-30), Point(0.5,0.5));
    textLabel->enableOutline(Color4B(0,0,0,255),2);
    layerHorseNode->addChild(textLabel,2);
    int Position[2] = {289,185};
    int verX = 160;
    for(int i = 0;i<4;i++)
    {
        m_HorsePai[i] = Sprite::createWithSpriteFrameName("zd_fanpai_fan.png");
        auto size = m_HorsePai[i] ->getContentSize();
        //sprite ->setPosition(Point(size.width/2,size.height/2));
        m_HorsePai[i] ->setPosition(Point(Position[0]+i*verX,Position[1]-30));
        layerHorseNode->addChild(m_HorsePai[i]);
        
        SpriteFrame *frame1 =SpriteFrameCache::getInstance()->getSpriteFrameByName("toumingkuang.png");
        ControlButton* button = ControlButton::create(Scale9Sprite::createWithSpriteFrame(frame1));
        button->setTag(i+1);
        button->setPreferredSize(Size(132,152));
        button->setPosition(Point(0+size.width/2,-4+size.height/2));
        button->setAnchorPoint(Point(0.5, 0.5));
        button->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVEWinLayer::OnBtnClickSelect),Control::EventType::TOUCH_UP_INSIDE);
        m_HorsePai[i]->addChild(button);
        
        m_HorseSpriteNode[i] = Sprite::createWithSpriteFrameName("ty_xingezi_1.png");
        m_HorseSpriteNode[i] ->setPosition(0+size.width/2,10+size.height/2);
        m_HorsePai[i]->addChild(m_HorseSpriteNode[i]);
        
        m_HorseNameLabel[i] = _createFont(m_HorsePai[i], "name", 20, ccc3(238,185,55), Point(0+size.width/2,-45+size.height/2), Point(0.5,0.5));
        m_HorsePai[i]->addChild(m_HorseNameLabel[i]);
        
        m_HorseNumLabel[i] = _createFont(m_HorsePai[i], "?", 20, ccc3(238,185,55), Point(32+size.width/2,-15+size.height/2), Point(1,0.5));
        m_HorsePai[i]->addChild(m_HorseNumLabel[i]);
        
        m_HorseSpritePai[i] = Sprite::createWithSpriteFrameName("zd_fanpai_zheng.png");
        m_HorseSpritePai[i] ->setPosition(Point(0+size.width/2,size.height/2));
        m_HorsePai[i]->addChild(m_HorseSpritePai[i]);
        
    }


//    layerHorseNode ->setVisible(true);
//    layerNode ->setVisible(false);

    if (str == "")
    {
        m_HorseLabel->setString("未获得任何碎片");
//        buttonOne->setVisible(true);
        isGetHorse = true;
    }
    else{
        // 获得抢马碎片
        std::string fragmentAwards = HclcData::getInstance()->getLuaVarString("fightHorseFragment");
        int fragmentType;
        std::string fragmentIcon;
        int fragmentQuality;
        // 拆分字符串
        std::vector<std::string> awards;
        split(fragmentAwards, ',', awards);
        for (int i = 0; i < awards.size(); i++){
            string str = awards.at(i);
            if (i == 0) {
                fragmentType = atoi(str.c_str());
            }else if (i == 1) {
                fragmentIcon = str;
            }else if (i == 2) {
                fragmentQuality = atoi(str.c_str());
            }
            CCLOG("-----------fragmentAwards-----------%s",str.c_str());
        }
        std::string iconname;
        if (fragmentType == 99 || fragmentType == 10) {//卡牌
            stringf(iconname, "res/head/%s",fragmentIcon.c_str());
        }else{
            stringf(iconname, "res/icons/%s",fragmentIcon.c_str());
        }

        Point gridPoint = m_HorseLabel->getPosition();
        std::string gridName;
        std::string suipianName;
        stringf(gridName,"ty_xingezi_%d.png",fragmentQuality);
        stringf(suipianName, "ty_suipian_%d.png",fragmentQuality);
        
        SpriteFrame * gridFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(gridName.c_str());
        SpriteFrame * suipianFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(suipianName.c_str());
        
        auto fragmentNode = Node::create();
        auto fragmentBg = Sprite::createWithSpriteFrame(gridFrame);
        auto fragmentSp = Sprite::create(iconname.c_str());
        auto suipian = Sprite::createWithSpriteFrame(suipianFrame);
        
        fragmentBg->setPosition(Point::ZERO);
        fragmentSp->setPosition(Point::ZERO);
        suipian->setPosition(Point::ZERO);
        
        fragmentNode->addChild(fragmentBg,1);
        fragmentNode->addChild(fragmentSp,2);
        fragmentNode->addChild(suipian,3);
        
        Size fraConSize = fragmentSp->getContentSize();
        fragmentNode->setPosition(Point(gridPoint.x, gridPoint.y-33));
        layerHorseNode->addChild(fragmentNode);
        
        m_HorseLabel->setPosition(Point(m_HorseLabel->getPositionX(),m_HorseLabel->getPositionY()+40));
        diTiao->setPosition(m_HorseLabel->getPosition());
        
        CCLOG("---------------height = %d",(int)fraConSize.height);
        string s = "恭喜获得"+str;
        m_HorseLabel->setString(s);
//        buttonOne->setVisible(false);
        isGetHorse = false;
    }
    buttonHorse->setVisible(false);
    
}

std::vector<std::string> gAwardArr;
void LQPVEWinLayer::drawGetItemList(string awardItem)
{
    Size viewSize = awardlayer->getContentSize();
    Point desPoint = awardlayer->getPosition();
    string awardStr = awardItem;
    if (awardStr == "") {
        return;
    }
    //放4个数值的数组
    int AwardNumArr[4];
    std::vector<std::string> Award3;
    split(awardStr, '#', Award3);
    
    for (int i = 0; i < Award3.size(); i++) {
        string types = Award3.at(i);
        CCLOG("types = %s",types.c_str());
        if(i==0) { // 金币 经验  将魂
            std::vector<std::string> itemData;
            split(types, ',', itemData);
            for (int j=0; j<itemData.size(); j++) {
//                AwardNumArr[j] = atoi(itemData.at(j).c_str());
//                string str = "";
//                stringf(str, "%d",AwardNumArr[j]);
                string str = itemData.at(j).c_str();
                
                if(m_obtainLabel[j]) {
                    m_obtainLabel[j]->setString(str);
                }
            }
        } else { // 奖励单元
            float x = 0;
            float y = viewSize.height/2;
            std::vector<std::string> AwardItemStrs;
            split(types, ';', AwardItemStrs);
            gAwardArr = AwardItemStrs;
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
                if (quality <= 0 )
                {
                    quality = 1;
                }
                Scale9Sprite* icon = createItemWithFileName(iconname, quality, number,1,22,Color3B(184,255,100));
                icon->setAnchorPoint(Point(0, 0.5));
                icon->setPosition(x, y);
                awardlayer->addChild(icon);
                if (type >= 10 && type <= 14)
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
                //-----详情---增加按钮----2015.7.28 --by wangqi
                auto awardBtn = ControlButton::create(Scale9Sprite::create("res/level/toumingkuang.png"));
                awardBtn->setPreferredSize(Size(78,76));
                awardBtn->setPosition(Point(x, y));
                awardBtn->setAnchorPoint(Point(0, 0.5));
                awardBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LQPVEWinLayer::OnClickCheckAward),Control::EventType::TOUCH_UP_INSIDE);
                awardBtn->setTag(i);
                awardlayer->addChild(awardBtn);
                //---------------------------------------------
                Label* nameLabel = _createFont(awardlayer, name.c_str(), 18, Color3B(184,255,100), Point(icon->getContentSize().width/2-10, 0), Point(0.5, 0));
                nameLabel->enableOutline(Color4B(0, 0, 0, 255),2);
                awardlayer->addChild(nameLabel);
                nameLabel->setVisible(false);
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


void LQPVEWinLayer::OnClickCheckAward(Ref * sender, Control::EventType controlEvent)
{
    auto node = (Node*)sender;
    int tag = node->getTag();
    CCLOG("tag = %d ",tag);
    std::vector<std::string> awardItem; //每个奖励单元的信息
    split(gAwardArr.at(tag), ',', awardItem);
    for (auto it:awardItem) {
        CCLOG("我的奖励 = %s", it.c_str());
    }
    HclcData::getInstance()->callLuaFunctionWithPara("myTest2",awardItem.at(4).c_str(),awardItem.at(5).c_str(),nullptr);
}
void LQPVEWinLayer::initAwardItem(string awardItems,int recStar)
{
    m_describeLabel[0]->setString("战斗取得胜利");
    m_describeLabel[1]->setString("60秒内通关");
    m_describeLabel[2]->setString("无阵亡武将");
    int types = LQBattleSession::getInstance()->m_fightType;
    int zoneID = LQBattleSession::getInstance()->getZoneID();
    LQZoneInfoData* groupData = (LQZoneInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQZoneInfoData::getAliasName(), zoneID);
    if (types != 1 || ( groupData && groupData->getTimeLimited() != 1)) {
        int starNum = recStar;
        int j3 = int(starNum/4);
        int j2 = int(starNum%4/2);
        int j1 = int(starNum%2);
        CCLOG("异或结果%d,%d,%d",j1,j2,j3);
        int array[3] = {j1,j2,j3};
        int Px= 390,Py = 323;
        int verY = 60;
        int co = 0;  //记录时间
        int VERX = 40;
        int CERY = 40;
        int VER[3] = {CERY,0,-1*CERY};
        int DELAYTIME = 0.0625;
        for (int i = 0; i<3; i++) {
            string s1 = (array[i] == 1)? "ty_xingxing.png":"ty_xingxingkong.png";
            star[i] = Sprite::createWithSpriteFrameName(s1);
            layerNode->addChild(star[i]);
            if (array[i] == 1)
            {
                star[i]->setLocalZOrder(33);
                star[i]->setVisible(false);
                star[i]->setPosition(Point(Point(Px - VERX,Py - i*verY + VER[i])));
                star[i]->setScale(4.5);
                ActionInterval *delay = DelayTime::create(0.3 * co);
                co++;
                auto callfunc = CallFunc::create(CC_CALLBACK_0(LQPVEWinLayer::notShakeScreen,this,i));
                auto callfunc2 = CallFunc::create(CC_CALLBACK_0(LQPVEWinLayer::shakeScreen,this,i));
                auto callfunc3 = CallFunc::create(CC_CALLBACK_0(LQPVEWinLayer::playFightWinEffect,this,i));
                auto moAction = MoveTo::create(0.0625, Point(Px,Py - i*verY));
                
                auto sAction = ScaleTo::create(0.0625,1);
                //            auto an = EaseIn::create(sAction, 6);
                ActionInterval *delay2 = DelayTime::create(0.0625);
                auto aAction = Spawn::create(sAction,moAction, NULL);
                //Sequence* sequence = Sequence::create(delay,callfunc,aAction,callfunc2, NULL);
                Sequence* sequence = Sequence::create(delay,callfunc,delay2,aAction,callfunc3,NULL);
                star[i]->runAction(sequence);
            }
            else
            {
                star[i]->setVisible(false);
                star[i]->setPosition(Point(Px,Py - i*verY));
                ActionInterval *delay = DelayTime::create(0.3 * co);
                auto callfunc = CallFunc::create(CC_CALLBACK_0(LQPVEWinLayer::notShakeScreen,this,i));
                Sequence* sequence = Sequence::create(delay,callfunc,NULL);
                star[i]->runAction(sequence);
            }
            
        }

    }
    drawGetItemList(awardItems);

}
//战斗胜利星星音效
void LQPVEWinLayer::playFightWinEffect(int i)
{
    AudioManager::getInstance()->playEffect(Effect_FightWinMusic1,0);

    ParticleSystemQuad *particle = ParticleSystemQuad::create("res/particle/winpl.plist");
    if(particle){
        particle->setPosition(Point(star[i]->getPositionX(),star[i]->getPositionY()));
        layerNode->addChild(particle);
    }
}

void LQPVEWinLayer::shakeScreen(int i)
{
    FiniteTimeAction* moveback = MoveBy::create(0.05,Point(0,-30));
    FiniteTimeAction* moveto2 = MoveBy::create(0.05,Point(0,30));
    Sequence* sequence = Sequence::create(moveback,moveto2,NULL);
    layerNode->runAction(sequence);
}
void LQPVEWinLayer::notShakeScreen(int i)
{
    star[i]->setVisible(true);
}


void LQPVEWinLayer::onEnter()
{
    Layer::onEnter();
}
void LQPVEWinLayer::onExit()
{
    Layer::onExit();
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

void LQPVEWinLayer::onBtnClickGoOn(Ref * sender, Control::EventType controlEvent)
{
    // 继续游戏
    LoadBattlefield::getInstance()->combatBack();
    
    //Director::getInstance()->getTextureCache()->removeUnusedTextures();
    //Director::getInstance()->getTextureCache()->removeAllTextures();
}

void LQPVEWinLayer::onBtnClickDialog(Ref * sender, Control::EventType controlEvent)
{
    layerDialog->setVisible(true);
}
void LQPVEWinLayer::onBtnClickDialogClose(Ref * sender, Control::EventType controlEvent)
{
    layerDialog->setVisible(false);
}


bool LQPVEWinLayer::onTouchBegan(Touch* touch, Event  *event){
    return true;
}
void LQPVEWinLayer::onTouchMoved(Touch* touch, Event  *event){}
void LQPVEWinLayer::onTouchEnded(Touch* touch, Event  *event){}
void LQPVEWinLayer::onTouchCancelled(Touch *touch, Event *event){}

void LQPVEWinLayer::OnBtnClickSelect(Ref * sender, Control::EventType controlEvent)
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
    isFanPai = false;
}
void LQPVEWinLayer::OnBtnSendData(Ref * sender, Control::EventType controlEvent)
{
    float scale = 3.0;
    float delaytime = 1;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* c1 = Sprite::create("res/level/cloud_battle.png"); //左下
    c1->setAnchorPoint(Point(1,1));
    c1->setPosition(Point(visibleSize.width*0,visibleSize.height*0));
    c1->setScale(scale);
    this->getParent()->addChild(c1,NODE_ZORDER_DIALOG + 10);
    c1->runAction(Sequence::create(MoveTo::create(delaytime,Point(visibleSize.width*0.7,visibleSize.height*0.7)),MoveTo::create(delaytime,Point(visibleSize.width*0,visibleSize.height*0)),RemoveSelf::create(), NULL));
    
    Sprite* c2 =  Sprite::create("res/level/cloud_battle.png"); //--右下
    c2->setAnchorPoint(Point(0,1));
    c2->setPosition(Point(visibleSize.width*1,visibleSize.height*0));
    c2->setScale(scale);
    this->getParent()->addChild(c2,NODE_ZORDER_DIALOG + 10);
    c2->runAction(Sequence::create(MoveTo::create(delaytime,Point(visibleSize.width*0.3,visibleSize.height*0.7)),MoveTo::create(delaytime,Point(visibleSize.width*1,visibleSize.height*0)),RemoveSelf::create(), NULL));
    
    Sprite* c3 = Sprite::create("res/level/cloud_battle.png");// --左上
    c3->setAnchorPoint(Point(1,0));
    c3->setPosition(Point(visibleSize.width* 0,visibleSize.height* 1));
    c3->setScale(scale);
    this->getParent()->addChild(c3,NODE_ZORDER_DIALOG + 10);
    c3->runAction( Sequence::create(MoveTo::create(delaytime,Point(visibleSize.width*0.7,visibleSize.height*0.3)),MoveTo::create(delaytime,Point(visibleSize.width*0,visibleSize.height*1)),RemoveSelf::create(), NULL));
    
    Sprite* c4 = Sprite::create("res/level/cloud_battle.png"); //--右上
    c4->setAnchorPoint(Point(0,0));
    c4->setPosition(Point(visibleSize.width*1,visibleSize.height*1));
    c4->setScale(scale);
    this->getParent()->addChild(c4,NODE_ZORDER_DIALOG + 10);
    c4->runAction( Sequence::create(MoveTo::create(delaytime,Point(visibleSize.width*0.3,visibleSize.height*0.3)),MoveTo::create(delaytime,Point(visibleSize.width*1,visibleSize.height*1)),CallFunc::create(CC_CALLBACK_0(LQPVEWinLayer::sendDataHorse,this)),RemoveSelf::create(), NULL));
    this->setVisible(false);
}
void LQPVEWinLayer::sendDataHorse()
{
    std::string sid = HclcData::getInstance()->getLuaVarString("robFragmentsMountID");
    std::string MonId = HclcData::getInstance()->getLuaVarString("robMountID");
    int id = atoi(sid.c_str());
    HorseBattleFightResultRequest* request5 = new HorseBattleFightResultRequest(id,MonId,true);
    LQSocketBuffer* buffer = request5->toMessage();
    if(buffer)
    ComService::getInstance()->SendData(buffer->bodyData(), buffer->size());
    this->getParent()->removeChild(this);

}

void LQPVEWinLayer::setDataToUI(string str)
{
    CCLOG("LQPVEWinLayer::setDataToUI -------收到数据%s",str.c_str());
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
    std::string Horsecount = HclcData::getInstance()->getLuaVarString("ThiefHorseCount");
    std::string horseTimes = HclcData::getInstance()->getLuaVarString("ThiefHorseTimes");
    CCLOG("Horsecount%s ==== horseTimes%s",Horsecount.c_str(),horseTimes.c_str());
    buttonOne->setVisible(isGetHorse && atoi(Horsecount.c_str()) == 0 && atoi(horseTimes.c_str()) > 0);
    buttonHorse->setVisible(true);
}
void LQPVEWinLayer::runActionForCard(int actionID,float time,std::vector<std::string> data)//翻牌的特效
{
    auto delay = DelayTime::create(time);
    auto mb = ScaleTo::create(0.3,0,1);
    auto mt = ScaleTo::create(0.3,1,1);
    auto action = Sequence::create(
               delay,
               mb,
               CallFunc::create(CC_CALLBACK_0(LQPVEWinLayer::callback1,this,actionID,data)),
                mt,
                                   NULL);
    auto layer = m_HorsePai[actionID];
    layer ->runAction(action);
}

void LQPVEWinLayer::callback1(int actionID, std::vector<std::string> itemData)
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
        auto dui = Sprite::createWithSpriteFrameName("ty_tubiao.png");
        dui->setScale(2, 2);
        dui->setPosition(Point(80,-30));
        sprite->addChild(dui);
        isFanPai = false;
    }
}

Label* LQPVEWinLayer::_createFont(Node* _parent,const std::string& _str,const char _FontSize,Color3B _FontColor,const Point _FontPos,const Point _AnchorPoint){
    auto label = Label::createWithSystemFont(_str,FONENAME, _FontSize);
    label->setAnchorPoint(_AnchorPoint);
    label->setColor(_FontColor);
    label->setPosition(_FontPos);
    return label;
}

Sprite* LQPVEWinLayer::_createSp(){
    return nullptr;
}
LayerColor* LQPVEWinLayer::_createLayerColor(){
    
}
ControlButton* LQPVEWinLayer::_createButton(){
    
}