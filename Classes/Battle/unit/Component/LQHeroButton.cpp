//
//  LQHeroButton.cpp
//  game
//
//  Created by lequdev1 on 14-7-21.
//
//

#include "LQHeroButton.h"

#define KLQHEROBUTTONHEADBG 52300
#define KLQHEROBUTTONHEADBG2 52301
#define KLQHEROBUTTONHEADBGHEADSP 52301
LQHeroButton::LQHeroButton():ControlButton(),m_isHeroDie(false),m_isCanFreeSkill(false),m_hpSlider(NULL),m_furySlider(NULL),m_isSelected(false),m_freeSprite(NULL),m_selectedSprite(NULL), m_headiconSp(NULL), m_hurtsSprite(NULL), hpLabel(NULL)
{

}
LQHeroButton::~LQHeroButton()
{

}
LQHeroButton* LQHeroButton::createHeroButtonWithData(LQCombatHero* hero)
{
    LQHeroButton* layer = new LQHeroButton();
    if (layer && layer->init()) {
        layer->initWithHeroButtonData(hero);
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    
    return layer;
}
bool LQHeroButton::init()
{
    bool isTrue = Node::init();
    return isTrue;
}
void LQHeroButton::onEnter()
{
    Node::onEnter();
}
void LQHeroButton::onExit()
{
    Node::onExit();
}

bool LQHeroButton::initWithHeroButtonData(LQCombatHero* hero)
{
    m_hero = hero;
    m_heroData = (LQHeroUserData* )hero->getData();
    m_squadData = hero->getSquadData();
    
    //Sprite* tempSp = Sprite::create("res/head/head-caocao.png");
    //Size addSize = Size(0, tempSp->getContentSize().height *0.5);
    
    string tempStr;
    stringf(tempStr, "qualitybg_%d.png",m_heroData->getQualityId());
    Scale9Sprite* iconbg = Scale9Sprite::createWithSpriteFrameName(tempStr);
    //Size addSize = Size(0, iconbg->getContentSize().height *0.5);
    
    Size buttonSize = Size(iconbg->getContentSize().width*1.1, iconbg->getContentSize().height*1.45);
    Size iconsize = iconbg->getContentSize()*1.1;
    
    this->initWithBackgroundSprite(Scale9Sprite::create("res/blank.png"));
    this->setPreferredSize(buttonSize);
    this->ignoreAnchorPointForPosition(false);
    this->setZoomOnTouchDown(false);
    
    iconbg->setTag(KLQHEROBUTTONHEADBG);
    iconbg->setAnchorPoint(Point(0.5,0.5));
    iconbg->setPosition(Point(buttonSize.width/2,buttonSize.height-iconsize.height/2 ));
    this->addChild(iconbg);
    
    stringf(tempStr, "q_bz_%d.png",m_heroData->getQualityId());
    Sprite* bzicon = Sprite::createWithSpriteFrameName(tempStr);
    bzicon->setPosition(0,iconbg->getContentSize().height);
    bzicon->setAnchorPoint(Point(0,1));
    iconbg->addChild(bzicon,10);
    string armicon = m_heroData->getArmsCfgdata()->getSmallIcon().c_str();
    stringf(tempStr, "res/level/%s", armicon.c_str());
    Sprite* _flagarmicon = Sprite::create(tempStr.c_str());
    _flagarmicon->setPosition(Point(bzicon->getContentSize().width/2,bzicon->getContentSize().height/2));
    bzicon->addChild(_flagarmicon);
    
    //创建裁减节点类
//    auto clipper = ClippingNode::create();//设置裁剪节点
//    clipper->setInverted(false);//设置底板可见
//    clipper->setContentSize(iconbg->getContentSize());
//    clipper->setAlphaThreshold(0.0f);//设置透明度Alpha值为0
//    clipper->setAnchorPoint(Point(0.5, 0.5));
//    clipper->setTag(KLQHEROBUTTONHEADBG2);
//    clipper->setPosition(Point(iconbg->getContentSize().width/2, iconbg->getContentSize().height/2));
//    iconbg->addChild(clipper,5);
    
    //设置裁减节点类所放的内容
    stringf(tempStr, "res/head/%s",m_heroData->getHeadID().c_str());
    //tempStr = "res/level/avatar_m_01.png";
    m_headiconSp = Sprite::create(tempStr);
    //m_headiconSp->setPosition( Point(clipper->getContentSize().width/2,clipper->getContentSize().height/2) );
    m_headiconSp->setPosition(iconbg->getContentSize().width/2,iconbg->getContentSize().height/2);
    m_headiconSp->setTag(KLQHEROBUTTONHEADBGHEADSP);
    iconbg->addChild(m_headiconSp);
    //clipper->addChild(m_headiconSp);
    
    //为设置裁减节点类设置“裁减模板”
    //创建“裁减模板”
//    Sprite* _flagmark = Sprite::createWithSpriteFrameName("qualitybg_zd.png");
//    _flagmark->setPosition( Point(_flagmark->getContentSize().width/2,_flagmark->getContentSize().height/2) );
//    clipper->setStencil(_flagmark);
    
//    Label* nameLabel = Label::createWithSystemFont(m_heroData->getName(), "Helvetica", 20);
//    nameLabel->setAnchorPoint(Point(0.5, 0));
//    nameLabel->setPosition(Point(m_headiconSp->getContentSize().width/2, 0));
//    nameLabel->setColor(Color3B::BLUE);
//    m_headiconSp->addChild(nameLabel);
    
    //
    int num = m_heroData->getStar();
    float y0 = buttonSize.height-iconbg->getContentSize().height;
    if(num>0){
        float _scale = 0.75;
        Sprite* _star = Sprite::createWithSpriteFrameName("b_star.png");
        float l0 = _star->getContentSize().width*_scale*0.7;
        float x0 = this->getContentSize().width/2-(num-1)*l0*0.5;
        //float y0 = this->getContentSize().height-iconbg->getContentSize().height-_star->getContentSize().height*0.5;
        for( int i=0; i<num; i++ ){
            _star = Sprite::createWithSpriteFrameName("b_star.png");
            _star->setPosition(x0,y0);
            _star->setAnchorPoint(Point(0.5, 0.2));
            _star->setScale(_scale);
            this->addChild(_star,100-i);
            x0 += l0;
        }
    }
    
    //血条
    y0 -= iconbg->getContentSize().height*0.00;
    m_hpSlider = ControlSlider::create( Sprite::createWithSpriteFrameName("hero_bar_bg.png"),
                                       Sprite::createWithSpriteFrameName("hero_bar_lv.png"), Sprite::create("res/blank.png"));
    m_hpSlider->setEnabled(false);
    y0 -= m_hpSlider->getContentSize().height*1.15;
    m_hpSlider->setPosition(Point(buttonSize.width/2,y0));
    this->addChild(m_hpSlider);
    m_hpSlider->setMaximumValue(m_squadData->getHpValueMax());
    m_hpSlider->setMinimumValue(0);
    m_hpSlider->setValue(this->getCurrHpValue());
    
#if defined(LQ_DEMO_FIGHT)
    hpLabel = Label::createWithSystemFont(intToString(this->getCurrHpValue()).c_str(), "Helvetica", 12);
    hpLabel->setAnchorPoint(Point(0.5, 0.5));
    hpLabel->setPosition(Point(m_hpSlider->getContentSize().width/2,m_hpSlider->getContentSize().height/2));
    hpLabel->setColor(Color3B::YELLOW);
    m_hpSlider->addChild(hpLabel);
#endif
    
    //怒气条
    y0 -= m_hpSlider->getContentSize().height*1.2;
    m_furySlider = ControlSlider::create( Sprite::createWithSpriteFrameName("hero_bar_bg.png"),
                                         Sprite::createWithSpriteFrameName("hero_bar_huang.png"), Sprite::create("res/blank.png"));
    //m_isCanFreeSkill = true;
    m_furySlider->setEnabled(false);
    m_furySlider->setPosition(Point(buttonSize.width/2,y0));
    this->addChild(m_furySlider,101);
    m_furySlider->setMaximumValue(m_heroData->getFuryMaxValue());
    m_furySlider->setMinimumValue(0);
    m_furySlider->setValue(0);
    
    m_freeSprite = Sprite::createWithSpriteFrameName("head_border_1.png");
    m_freeSprite->setPosition(iconbg->getPosition());
    m_freeSprite->setScale(0.9);
    this->addChild(m_freeSprite,101);
    m_freeSprite->setVisible(m_isCanFreeSkill);
    
    m_selectedSprite = Sprite::createWithSpriteFrameName("head_border.png");
    m_selectedSprite->setPosition(iconbg->getPosition());
    m_selectedSprite->setScale(0.9);
    this->addChild(m_selectedSprite,101);
    m_selectedSprite->setVisible(m_isSelected);
    
    //怒气
    m_hurtsSprite = Sprite::createWithSpriteFrameName("nq_1.png");
    m_hurtsSprite->setPosition(m_furySlider->getPosition());
    this->addChild(m_hurtsSprite,101);
    ActionInterval *action = this->createAnimation();
    RepeatForever* forever1 = RepeatForever::create(action);
    m_hurtsSprite->runAction(forever1);
    m_hurtsSprite->setVisible(m_isSelected);
    
    return true;
}

/*
bool LQHeroButton::initWithHeroButtonData(LQCombatHero* hero)
{
    m_hero = hero;
    m_heroData = (LQHeroUserData* )hero->getData();
    m_squadData = hero->getSquadData();
    
    Sprite* tempSp = Sprite::create("res/head/head-caocao.png");
    Size addSize = Size(0, tempSp->getContentSize().height *0.5);
    
    this->initWithBackgroundSprite(Scale9Sprite::create("res/blank.png"));
    //ControlButton* button = ControlButton::create(Scale9Sprite::create("res/blank.png"));
    this->setPreferredSize(tempSp->getContentSize()*1.3 + addSize);
//    this->setContentSize(this->getPreferredSize());
    this->ignoreAnchorPointForPosition(false);
    this->setZoomOnTouchDown(false);
    //button->setPosition(Point(this->getContentSize()*0.5));
    //this->addChild(button);
    
    Size buttonSize = tempSp->getContentSize()*1.3;
    Size iconsize = tempSp->getContentSize()*1.1;
    
    Scale9Sprite* bg = Scale9Sprite::createWithSpriteFrameName("zd_di_hui.png");
    bg->setPreferredSize(buttonSize);
    bg->setPosition(Point(buttonSize.width/2, buttonSize.height/2 + addSize.height));
    bg->setTag(KLQHEROBUTTONHEADBG);
    this->addChild(bg);
    
    string tempStr;
    stringf(tempStr, "qualitybg_%d.png",m_heroData->getQualityId());
    Scale9Sprite* iconbg = Scale9Sprite::createWithSpriteFrameName(tempStr);
    iconbg->setPreferredSize(iconsize);
    iconbg->setAnchorPoint(Point(0.5,0.5));
    iconbg->setPosition(Point(buttonSize.width/2,buttonSize.height/2 ));
    bg->addChild(iconbg);
    
    stringf(tempStr, "res/head/%s",m_heroData->getHeadID().c_str());
    Sprite* headSp = Sprite::create(tempStr);
    headSp->setPosition(iconbg->getPosition());
    headSp->setTag(KLQHEROBUTTONHEADBGHEADSP);
    bg->addChild(headSp);
    
    Label* nameLabel = Label::createWithSystemFont(m_heroData->getName(), "Helvetica", 20);
    nameLabel->setAnchorPoint(Point(0.5, 0));
    nameLabel->setPosition(Point(headSp->getContentSize().width/2, 0));
    nameLabel->setColor(Color3B::BLUE);
    headSp->addChild(nameLabel);
    

    //血条
    m_hpSlider = ControlSlider::create( Sprite::createWithSpriteFrameName("zd_heidi.png"), Sprite::createWithSpriteFrameName("zd_di_lv.png"), Sprite::create("res/blank.png"));
    m_hpSlider->setEnabled(false);
    m_hpSlider->setPosition(Point(buttonSize.width/2,addSize.height*0.7));
    this->addChild(m_hpSlider);
    m_hpSlider->setMaximumValue(this->getCurrHpValue());
    m_hpSlider->setMinimumValue(0);
    m_hpSlider->setValue(this->getCurrHpValue());
    
    
    //怒气条
    m_furySlider = ControlSlider::create( Sprite::createWithSpriteFrameName("zd_heidi.png"), Sprite::createWithSpriteFrameName("zd_di_huang.png"), Sprite::create("res/blank.png"));
    m_furySlider->setEnabled(false);
    m_furySlider->setPosition(Point(buttonSize.width/2,addSize.height*0.3));
    this->addChild(m_furySlider);
    m_furySlider->setMaximumValue(m_heroData->getFuryMaxValue());
    m_furySlider->setMinimumValue(0);
    m_furySlider->setValue(0);
    
    
    
    m_freeSprite = Sprite::createWithSpriteFrameName("head_border_1.png");
    m_freeSprite->setPosition(buttonSize.width/2, buttonSize.height/2 + addSize.height);
    this->addChild(m_freeSprite);
    m_freeSprite->setVisible(m_isCanFreeSkill);
    
    
    m_selectedSprite = Sprite::createWithSpriteFrameName("head_border.png");
    m_selectedSprite->setPosition(buttonSize.width/2, buttonSize.height/2 + addSize.height);
    
    this->addChild(m_selectedSprite);
    m_selectedSprite->setVisible(m_isSelected);
    
    return true;
}
*/
void LQHeroButton::setSelected(bool isSelected)
{
    if (!m_selectedSprite) {
        return;
    }
    if (m_selectedSprite) {
        m_selectedSprite->setVisible(isSelected);
    }
    
    if (!m_isCanFreeSkill) {
        m_selectedSprite->setVisible(false);
    }
}
void LQHeroButton::setIsCanFreeSkill(bool isCanFreeSkill)
{
    if (m_isCanFreeSkill == isCanFreeSkill) {
        return;
    }
    m_freeSprite->stopAllActions();
    m_isCanFreeSkill = isCanFreeSkill;
    m_freeSprite->setVisible(isCanFreeSkill); //DelayTime::create(0.5),
    m_freeSprite->runAction(RepeatForever::create(Sequence::create(FadeTo::create(1.0,255),FadeTo::create(0.7,100),NULL)));

}

//将获取英雄血提出，好修改取数方法12.18
int LQHeroButton::getCurrHpValue()
{
    //return m_heroData->getCurrHpValue();
    //12.18改为英雄没有血，共用军团
    return m_squadData->getCurrHpValue();
}

void LQHeroButton::updataButton()
{
    if (m_isHeroDie || !m_heroData) {//英雄死亡把血条怒气条清空 把头像变灰
        m_hpSlider->setValue(0);
        m_furySlider->setValue(0);
        m_hurtsSprite->stopAllActions();
        m_hurtsSprite->setVisible(false);
        m_headiconSp->setColor(Color3B(128, 128,128));
        return;
    }
#if defined(LQ_DEMO_FIGHT)
    hpLabel->setString(intToString(this->getCurrHpValue()).c_str());
#endif
    m_hpSlider->setValue(this->getCurrHpValue());
    m_furySlider->setValue(m_heroData->getHeroFuries());
    
    setIsCanFreeSkill(m_hero->getActive()&&m_heroData->isCanFreeActiveSkill());
    
    if (this->getCurrHpValue() <= 0 || !m_hero->getActive()) {
        m_isHeroDie = true;
        setSelected(false);
        setIsCanFreeSkill(false);
    }
    
    if(m_heroData->getHeroFuries()>=m_heroData->getFuryMaxValue()){
        m_hurtsSprite->setVisible(true);
        m_hurtsSprite->resume();
    }else{
        m_hurtsSprite->pause();
        m_hurtsSprite->setVisible(false);
    }
    
}
void LQHeroButton::setHeroDie()
{
    if (m_isHeroDie) {
        return;
    }
    m_isHeroDie = true;
}

//创建系列帧动画
ActionInterval* LQHeroButton::createAnimation()
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