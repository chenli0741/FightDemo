//
//  LQSkillShowingLayer.cpp
//  SuperWarriors
//
//  Created by lequ on 15/4/1.
//
//

#include "LQSkillShowingLayer.h"
#include <math.h>
#include "AudioManager.h"
LQSkillShowingLayer* LQSkillShowingLayer::create()
{
    LQSkillShowingLayer* layer = new LQSkillShowingLayer();
    if (layer && layer->init()) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    
    return layer;
}

bool LQSkillShowingLayer::init()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(LQSkillShowingLayer::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LQSkillShowingLayer::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LQSkillShowingLayer::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    return true;
}

void LQSkillShowingLayer::onEnter()
{
    Layer::onEnter();
}

void LQSkillShowingLayer::onExit()
{
    Layer::onExit();
}

void LQSkillShowingLayer::show(LQCombatHero* pSender,LQBaseUserData* data, int type, FiniteTimeAction* endcall)
{
    int color[2][3] = {{16,121,248},{251,11,31}};
    string back[2] = {"zd_sudubg_1.png","zd_sudubg_2.png"};
    string guang[2] = {"zd_guangdian_1.png","zd_guangdian_2.png"};
    int dir[2]= {1,-1};
    auto _skillhero = (LQCombatHero*)pSender;
    auto _effectdata = (LQEffectUserData*)data;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    int position[2] = {0,(int)visibleSize.width};
    
    LayerColor* marklayer = LayerColor::create(Color4B(color[type][0], color[type][1], color[type][2], 0), visibleSize.width, visibleSize.height);
    marklayer->setPosition(Point(0, 0));
    marklayer->setAnchorPoint(Point(0, 0));
    this->addChild(marklayer);
    
    Sprite *skillBG = Sprite::createWithSpriteFrameName(back[type].c_str());
    skillBG->setAnchorPoint(Point(1, 0.5));
    skillBG->setScale(2);
    if(type == 0)
    {
        skillBG->setRotation(180);
        skillBG->setPosition(Point(0,visibleSize.height/2));
    }
    else
    {
        skillBG->setPosition(Point(visibleSize.width,visibleSize.height/2));
    }
    marklayer->addChild(skillBG,10);
    
    auto backLine = Sprite::createWithSpriteFrameName("sbar_red.png");
    backLine->setScale(30.0f, 5);
    backLine->setAnchorPoint(Point(0.5,1));
    backLine->setPosition(Point(visibleSize.width/2,visibleSize.height/2 - 100));
    auto pClip = ClippingNode::create();
    pClip->setStencil(backLine);
    pClip->setInverted(true);
    pClip->setAlphaThreshold(0);
    marklayer->addChild(pClip,100);
    /*
    for (int i = 0;i < 50;i++)
    {
        int poX = rand()%((int)visibleSize.width + 1000);
        int poY = rand()%((int)visibleSize.height);
        auto sprite = Sprite::createWithSpriteFrameName(guang[type]);
        sprite->setPosition(Point(poX-500,poY));
                
        marklayer->addChild(sprite,9);
        auto moTo = MoveBy::create(1,Point(dir[type]*500,0));
        sprite->runAction(moTo);
    }
    */
    float pointSkill[2] = {marklayer->getContentSize().width/3 - 100,marklayer->getContentSize().width/3*2 + 100};
    for (int i = 0;i<3;i++)
    {
        string tempStr;
        stringf(tempStr, "res/cards/%s",_skillhero->getHeroData()->getCardID().c_str());
        Sprite* headSp = Sprite::create(tempStr);
        headSp->setPosition(position[type] + dir[type]*100 - dir[type]*i*80,marklayer->getContentSize().height/2);
        headSp->setOpacity(255 - i * 100);
        headSp->setScale(2.0f,2.0f);
        pClip->addChild(headSp,100-i*10);
        auto moTo = MoveTo::create(0.325,Point(pointSkill[type] - dir[type]* i * 80,marklayer->getContentSize().height/2));
        auto moTo2 = MoveTo::create(0.1625,Point(pointSkill[type] - dir[type]* i * 10,marklayer->getContentSize().height/2));
        auto callfunc = CallFunc::create(CC_CALLBACK_0(LQSkillShowingLayer::removeSpriteSelf,this,headSp));
        auto callfunc2 = CallFunc::create(CC_CALLBACK_0(LQSkillShowingLayer::showLabel,this,headSp));
        Sequence* sequence = Sequence::create(moTo,moTo2,callfunc,NULL);
        Sequence* sequence2 = Sequence::create(moTo,callfunc2,NULL);
        if (i == 0 )
        {
            nodeSkillLable = Node::create();
            //nodeSkillLable->setPosition(Point(pointSkill[type] - dir[type]* i * 80 - dir[type]*250,marklayer->getContentSize().height/2 + 60));
            nodeSkillLable->setPosition(Point(marklayer->getContentSize().width/2,marklayer->getContentSize().height/2 + 60));
            nodeSkillLable->setScale(1.6);
            marklayer->addChild(nodeSkillLable,10000);
            //武将名称
            Scale9Sprite* heroNameSp = Scale9Sprite::createWithSpriteFrameName("ty_baomingdi.png");
            heroNameSp->setPreferredSize(Size(262,54));
            heroNameSp->setAnchorPoint(Point(0.5,0.5));
            heroNameSp->setPosition( headSp->getContentSize().width/7*5 - 20,headSp->getContentSize().height/5*3 );
            /*
            if(type == 0)
            {
                heroNameSp->setPosition( headSp->getContentSize().width/7*5,headSp->getContentSize().height/5*3 );
            }
            else
            {
                heroNameSp->setPosition( headSp->getContentSize().width/7*2,headSp->getContentSize().height/5*3 );
            }
            */
            heroNameSp->setAnchorPoint(Point(0.5,0.5));
            heroNameSp->setRotation(90);
            heroNameSp->setVisible(true);
            heroNameSp->setScale(0.5,0.5);
            headSp->addChild(heroNameSp,1000);
            auto node = Node::create();
            node->setPosition(heroNameSp->getPosition());
            node->setScale(0.5,0.5);
            node->setTag(202);
            node->setVisible(true);
            headSp->addChild(node,1000);
            string hname = _skillhero->getHeroData()->getName().c_str(); //"鬼神－夏侯惇"
            Label* heroName = Label::createWithSystemFont(hname.c_str(), "Helvetica",36);
            heroName->setDimensions(40,0);
            heroName->enableShadow();
            heroName->setPosition(Point(4,0));
            node->addChild(heroName);
            
            //技能大提示文字
            string _typePic = _effectdata->getSkillInfo()->getTypePic();
            if(_typePic.length()>0){
                auto sp = Sprite::createWithSpriteFrameName("zd_mo.png");
                sp->setScale(0.5,0.5);
                sp->setPosition(0,0);
                nodeSkillLable->addChild(sp,1000);
                _typePic = StringUtils::format("res/level/%s",_typePic.c_str());
                Sprite* skillTitleSp = Sprite::create(_typePic.c_str()); //"st_cjbd.png"
                //skillTitleSp->setScale(dir[type] * 0.5,0.5);
                skillTitleSp->setScale(0.5,0.5);
                skillTitleSp->setPosition(0,0);
                nodeSkillLable->addChild(skillTitleSp,1000);
                nodeSkillLable->setVisible(false);
            }

            headSp->runAction(sequence2);
        }else
        {
            headSp->runAction(sequence);
        }
        
    }
    
    
    
    for (int i = 0; i<200; i++) {
        int poX = rand()%((int)(visibleSize.width*3));
        int poY = rand()%116;
        int sude = rand()%100;
        int idx = i%2;
        auto child = Sprite::createWithSpriteFrameName("zd_suduxian.png");
        child->setPosition(Point(poX - visibleSize.width,marklayer->getContentSize().height/2 + dir[idx]*poY));
        if (type == 0)
        {
            float s = poX/(2*visibleSize.width);
            if (s < 1)
            {
                child->setOpacity(255 * (1-s));
            }
            
        }
        else
        {
            float s = (poX - visibleSize.width)/(2*visibleSize.width);
            if (poX > 0)
            {
                child->setOpacity(255 * s);
            }
        }
        marklayer->addChild(child,12);
        auto moTo2 = MoveBy::create(1,Point(dir[type]*visibleSize.width + sude,0));
        child->runAction(moTo2);
        
    }
    auto callfunc = CallFunc::create(CC_CALLBACK_0(LQSkillShowingLayer::removeSelf,this));
    ActionInterval *delay = DelayTime::create(1);
    Sequence* sequence = Sequence::create(delay,callfunc, endcall, NULL);
    this->runAction(sequence);
}
void LQSkillShowingLayer::removeSelf()
{
    auto layer = this->getParent();
    this->stopAllActions();
    layer->removeChild(this);
}
void LQSkillShowingLayer::removeSpriteSelf(Node* node)
{
    auto layer = node->getParent();
    layer->removeChild(node);
}
void LQSkillShowingLayer::showLabel(cocos2d::Node *node)
{
    //技能武将插图音效
    AudioManager::getInstance()->playEffect(Effect_Skill_Music1);
    nodeSkillLable->setVisible(true);
}
void LQSkillShowingLayer::onTouchMoved(Touch* touch, Event  *event){}
void LQSkillShowingLayer::onTouchEnded(Touch* touch, Event  *event){}
void LQSkillShowingLayer::onTouchCancelled(Touch *touch, Event *event){}
bool LQSkillShowingLayer::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}
