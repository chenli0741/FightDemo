//
//  LQFlyNumber.m
//  CZSQ
//
//  Created by chenli on 12-11-30.
//  Copyright 2012年 LequGame. All rights reserved.
//

#include "LQFlyNumber.h"

#define HDSCALE 2

LQFlyNumber::LQFlyNumber():m_label(NULL),m_isSign(true), _start(0),m_times(0)
{
//    m_passiveFontName = "res/fonts/normal_add.fnt";
//    m_nagetiveFontName = "res/fonts/normal_des.fnt";
    m_passiveFontName = "res/fonts/normal_skill_cure.fnt";
    m_nagetiveFontName = "res/fonts/normal_skill_hurt.fnt";
}
LQFlyNumber::~LQFlyNumber()
{

}

LQFlyNumber* LQFlyNumber::createFlyWithFile(string iconname, int num, LQFlyNumberTypeEnum numbertype, bool isSign, string critstr, const Point& numAnchor)
{
    LQFlyNumber* numLabel = new LQFlyNumber();
    
    if (numLabel )
    {
        if(iconname.size() > 0) {
            numLabel->initWithFile(iconname);
        }else{
            numLabel->initWithFile("res/blank.png");
        }
        
        numLabel->initFlyNumber(num,numbertype, isSign, critstr);
        numLabel->autorelease();
    }else{
        CC_SAFE_DELETE(numLabel);
    }
    
    return numLabel;
}

LQFlyNumber* LQFlyNumber::createFlyWithSpriteFrameName(string iconname, int num, LQFlyNumberTypeEnum numbertype, bool isSign, string critstr, const Point& numAnchor)
{
    if( strcmp(iconname.c_str(),"sp_counter.png")==0 )
        CCLOG("sp_counter.png");
    LQFlyNumber* numLabel = new LQFlyNumber();
    
    
    if (numLabel )
    {
        if(iconname.size() > 0) {
            numLabel->initWithSpriteFrameName(iconname);
        }else{
            numLabel->initWithFile("res/blank.png");
        }
        
        numLabel->initFlyNumber(num,numbertype, isSign, critstr);
        numLabel->autorelease();
    }else{
        CC_SAFE_DELETE(numLabel);
    }
    
    return numLabel;
}

//BUFF飘字，图形化
LQFlyNumber* LQFlyNumber::createFlyWithBuffTxt(string bufftxt, LQFlyNumberTypeEnum numbertype)
{
    LQFlyNumber* numLabel = new LQFlyNumber();
    
    if (numLabel )
    {
        numLabel->initWithFile("res/blank.png");
        numLabel->initFlyBuffTxt(bufftxt,numbertype);
        numLabel->autorelease();
    }else{
        CC_SAFE_DELETE(numLabel);
    }
    
    return numLabel;
}

bool LQFlyNumber::initFlyBuffTxt(string bufftxt, LQFlyNumberTypeEnum numbertype)
{
    //bufftxt = "baoji;shangsheng";
    m_numberType = numbertype;
    int _w = 0;
    int _h = 0;
    int _s = 1;
    std::vector<std::string> icons = LQ::split(bufftxt.c_str(), ';');
    if (icons.size()>0){
        string icon = icons.at(0)+".png";
        SpriteFrame *frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(icon.c_str());
        Sprite* sp1 = Sprite::createWithSpriteFrame(frame1);
        if(frame1!=nullptr){
            sp1->setAnchorPoint(Point(0,0));
            sp1->setPosition( Point(0,0) );
            this->addChild(sp1);
            _w = sp1->getContentSize().width*_s;
            _h = sp1->getContentSize().height*_s;
            sp1->setScale(_s);
            if (icons.size()>1 && icons.at(1).length()>0){
                icon = icons.at(1)+".png";
                SpriteFrame *frame2 = SpriteFrameCache::getInstance()->getSpriteFrameByName(icon.c_str());
                if(frame2!=nullptr){
                    Sprite* sp2 = Sprite::createWithSpriteFrame(frame2);
                    _w += sp2->getContentSize().width*1.1*_s;
                    sp2->setAnchorPoint(Point(1,0));
                    sp2->setPosition( Point(_w,0) );
                    this->addChild(sp2);
                    if (sp2->getContentSize().height*_s>_h)
                        _h = sp2->getContentSize().height*_s;
                    sp2->setScale(_s);
                }else{
                    CCLOG("技能BUFF描述资源不存在：%s", icon.c_str());
                }
            }
        }else{
            CCLOG("技能BUFF描述资源不存在：%s", icon.c_str());
            string _fontName = "res/fonts/skillname.fnt";
            Label* _label = Label::createWithBMFont(_fontName, bufftxt.c_str());
            _label->setPosition( Point(this->getContentSize().width/2,0) );
            this->addChild(_label);
        }
    }
    
    this->setContentSize(Size(_w,_h));
    return true;
}

bool LQFlyNumber::initFlyNumber(int number, LQFlyNumberTypeEnum numbertype,bool isSign, string critstr)
{
    m_value = number;
    _start = 0;
    std::string str;
    const char* sign = "";
    if(isSign){
        sign = (m_value>0) ? "+" : "-";
    }
    LQ::stringf(str, "%s%d", sign,  abs(m_value));
    
    if(numbertype == klq_flaytype_crit ){
        str = critstr+str;//"暴击"
    }
    
    m_numberType = numbertype;
    m_isSign = isSign;
    
    if(numbertype == klq_flaytype_criticon)
        return true;
    m_label = Label::createWithBMFont(m_passiveFontName, str);
    m_label->setAnchorPoint(Point(0.5, 1));
    //m_label->setScale(0.35);
    m_label->setPosition( Point(this->getContentSize().width/2,0) );
    this->addChild(m_label);
    
    if (m_value == 0) {
        m_label->setVisible(false);
    }
    
    return true;
}

void LQFlyNumber::startAnimation(float start)
{
    //调用changeScene函数
    duration = 0.2;
    _start = start;
    switch (m_numberType) {
        case klq_flaytype_crit:{
            //double scaletime = 0.4; normal_skill_cure
            //m_nagetiveFontName = "res/fonts/normal_skill_hurt.fnt"; //临时处理，因为一些字体没有
            if (m_isSign) {
                if(m_value>0) m_label->setBMFontFilePath(m_passiveFontName);
                else m_label->setBMFontFilePath(m_nagetiveFontName);
            }
            /*
            //m_label->setBMFontFilePath("res/fonts/normal_crit.fnt");
            //            m_label->setColor(Color3B(CCRANDOM_0_1()*75+180,0,0));
            FiniteTimeAction* randommove1 = MoveBy::create(0,Point(75*CCRANDOM_0_1()*getScale(), CCRANDOM_0_1()*50*getScale()));
            FiniteTimeAction* scaleby1 = ScaleTo::create(scaletime*duration*1.0,scaletime*getScale()*2.5f*(0.6+0.5*CCRANDOM_0_1()));
            FiniteTimeAction* scaleby2 = ScaleTo::create(scaletime*duration*1.0,scaletime*getScale()*1.5f*(0.8+0.8*CCRANDOM_0_1()));
            //            FiniteTimeAction* scaleby3 = ScaleTo::create(duration*1.0,getScale()*2.0f);
            //            FiniteTimeAction* scaleby4 = ScaleTo::create(duration*1.0,getScale()*1.0f);
            FiniteTimeAction* delay1 = DelayTime::create(duration*3.5);
            FiniteTimeAction* delay2 = DelayTime::create(duration*1.0);
            //            FiniteTimeAction* moveby1 = MoveBy::create(duration*3.5,Point(0, 100*getScale()));
            //            FiniteTimeAction* fadeout1 = CCSpawn::create(moveby1,FadeOut::create(duration*2.0),NULL);
            //            FiniteTimeAction* moveby1 = MoveBy::create(duration*1.0,Point(0, getScale()*75));
            FiniteTimeAction* scaleto1 = ScaleTo::create(duration*1.5,0);
            FiniteTimeAction* scaleby3 = ScaleTo::create(duration*0.5,getScale()*1.1);
            FiniteTimeAction* fadeout1 = CCSpawn::create(scaleto1,FadeOut::create(duration*0.5),NULL);
            Sequence* spAct1 = Sequence::create( randommove1,scaleby1, scaleby2, delay2,scaleby3,CCSpawn::create(delay1, fadeout1, NULL), RemoveSelf::create(), NULL);//
            this->runAction( spAct1 );*/
            
            ActionInterval* sequAct0 = createFiniteTimeAction1();
            this->runAction( sequAct0 );
            
            break;
        }
        case klq_flaytype_criticon:{ //暴击  放大字体，红色
            Sprite* m_light = Sprite::createWithSpriteFrameName("crit_light.png");
            m_light->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
            m_light->setScale(0.2);
            this->addChild(m_light,100);
            float s1 = 1.0f;
            FiniteTimeAction* scaleby1 = ScaleTo::create(duration*1.0,getScale()*s1*2.5f);
            FiniteTimeAction* scaleby2 = ScaleTo::create(duration*1.0,getScale()*s1*1.5f);
            FiniteTimeAction* delay1 = DelayTime::create(duration*2.0);
            //FiniteTimeAction* delay2 = DelayTime::create(duration*3.5);
            FiniteTimeAction* scaleby3 = ScaleTo::create(duration*0.5,getScale()*s1*1.0);
            FiniteTimeAction* fadeout1 = CCSpawn::create(scaleby3,FadeOut::create(duration*0.5),NULL);
            Sequence* spAct1 = Sequence::create( scaleby1, scaleby2, delay1, fadeout1, RemoveSelf::create(), NULL);//
            this->runAction( spAct1 );

            FiniteTimeAction* delay5 = DelayTime::create(duration*0.1);
            FiniteTimeAction* scaleby5 = ScaleTo::create(duration*1.0,getScale()*s1*1.3f);
            FiniteTimeAction* fadein5  = FadeIn::create(duration*1.0f);
            FiniteTimeAction* fadeout5 = FadeOut::create(duration*0.5);
            Sequence* spAct2 = Sequence::create( delay5, CCSpawn::create(scaleby5, fadein5, NULL), fadeout5, RemoveSelf::create(), NULL);
            m_light->runAction( spAct2 );
            
            break;
        }
//        case klq_flaytype_fklsg:{ //技能伤害  假象不减少任何数值
//            if (m_isSign) {
//                m_passiveFontName = m_nagetiveFontName = "res/fonts/Crit_des.fnt";
//                if(m_value>0) m_label->setBMFontFilePath(m_passiveFontName);
//                else m_label->setBMFontFilePath(m_nagetiveFontName);
//            }
//            FiniteTimeAction* scaleby1 = ScaleTo::create(duration*1.7,1.5f*getScale());
//            FiniteTimeAction* scaleby2 = ScaleTo::create(duration*0.4,1.0f*getScale());
//            FiniteTimeAction* fadeout1 = FadeOut::create(duration*0.5);
//            Sequence* spAct1 = Sequence::create( scaleby1, scaleby2, DelayTime::create(duration*0.5), fadeout1, RemoveSelf::create(), NULL);//
//            this->runAction( spAct1 );
//            break;
//        }
        case klq_flaytype_parry:{ //格挡  缩小字体，黄色
            duration = 0.3;
            float s1 = 0.5f;
            if (m_isSign) {
                if(m_value>0) m_label->setBMFontFilePath(m_passiveFontName);
                else m_label->setBMFontFilePath(m_nagetiveFontName);
            }
            this->setScale(s1);
            FiniteTimeAction* moveby1 = MoveBy::create(duration*4,Point(10*HDSCALE, 50*HDSCALE));
            FiniteTimeAction* delay1 = DelayTime::create(duration*1.5*getScale());
            FiniteTimeAction* fadeout1 = CCFadeOut::create(duration*1.5*getScale());
            Sequence* sequAct1 = Sequence::create(delay1, fadeout1, RemoveSelf::create(), NULL);
            Spawn* spAct1 = Spawn::create(sequAct1, moveby1, NULL);//
            this->runAction( spAct1 );
            break;
        }
        case klq_flaytype_bufftxt:{ //BUFF的飘文字
            this->setVisible(false);
            float _s1 = 0.0f;
            if(_start>0) _s1 = _start;
            ActionInterval* sequAct0 = Sequence::create(DelayTime::create(_s1), Show::create(), MoveBy::create(0.3,Point(0,this->getContentSize().height*0.95)), DelayTime::create(1.0), FadeOut::create(0.3), RemoveSelf::create(), NULL);
            
            this->runAction( sequAct0 );
            break;
        }
        default:{ //正常.技能现在显示
            //m_passiveFontName =
            m_nagetiveFontName = "res/fonts/normal_skill_hurt.fnt";  //临时处理，因为一些字体没有
            if (m_isSign && m_label) {
                if(m_value>0) m_label->setBMFontFilePath(m_passiveFontName);
                else m_label->setBMFontFilePath(m_nagetiveFontName);
            }
            
            ActionInterval* sequAct0 = createFiniteTimeAction1();
            this->runAction( sequAct0 );
            
            break;
        }
    }
}

ActionInterval* LQFlyNumber::createFiniteTimeAction1()
{
    float tsc = 1;//多次攻击的调整系数
    if(m_times>0)
        tsc = 0.7;
    float s1 = 0.5f;
    FiniteTimeAction* moveby1 = MoveBy::create(duration*5,Point(0, 20*getScale()*HDSCALE));
    FiniteTimeAction* scale1 = ScaleTo::create(duration*0.1*tsc,s1*1.5);
    FiniteTimeAction* delay1 = DelayTime::create(duration*1.2*tsc);
    FiniteTimeAction* scale2 = ScaleTo::create(duration*1.0*tsc,s1*0.9);
    //FiniteTimeAction* fadeout1 = FadeOut::create(duration*1.5);
    Sequence* sequAct1 = Sequence::create(scale1, delay1, scale2, NULL);
    Sequence* sequAct2 = Sequence::create(moveby1, RemoveSelf::create(),NULL);
    Spawn* spAct1 = Spawn::create(sequAct1, EaseSineIn::create(sequAct2),  NULL);//
    
    ActionInterval* sequAct0;
    if(_start>0){
        this->setVisible(false);
        FiniteTimeAction* start1 = DelayTime::create(_start*0.85);
        FiniteTimeAction* show1 = Show::create();
        sequAct0 = Sequence::create(start1, show1, spAct1, NULL);
    }else{
        this->setVisible(true);
        sequAct0 = spAct1;
    }
    
    return sequAct0;
}

void LQFlyNumber::removeObject()
{
    stopAllActions();
    removeAllChildrenWithCleanup(true);
}
