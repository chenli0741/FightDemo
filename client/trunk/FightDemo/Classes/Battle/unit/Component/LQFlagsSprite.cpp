//
//  LQFlagsSprite.cpp
//  LostEmpire
//
//  Created by chenli on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "LQFlagsSprite.h"

using namespace cocos2d;
LQ_NAMESPACE

LQFlagsSprite::LQFlagsSprite():sp_mark(NULL), sp_markdie(NULL), sp_barlight(NULL), sp_clclight(NULL), m_hpValue(0), m_hdValue(0), m_hpMax(0), m_hpSlider(NULL), m_hdSlider(NULL),hpTtfName(NULL),_effectIndex(-1),_zordertimes(-1)
{
}

LQFlagsSprite::~LQFlagsSprite()
{
    //log("~~~LQFlagsSprite  type = %d",m_skillShowType);
}

LQFlagsSprite* LQFlagsSprite::createWithData(LQSquadUserData* data)
{
    LQFlagsSprite* sp = new LQFlagsSprite();
    if (sp && sp->initWithFile("res/blank.png")) {
        sp->initWithData2(data);
        sp->autorelease();
    }else{
        CC_SAFE_DELETE(sp);
    }
    
    return sp;
}

//新的旗帜18
bool LQFlagsSprite::initWithData2(LQSquadUserData* data)
{
    m_hpMax = data->getHpValueMax();
    m_hpValue = data->getSquadHpValue();
    m_hdValue = data->getCurrShValue();
    m_squaddata = data;
    m_herodata = data->getHeroData();
    string fname1 = "flag_lan.png";
    if(m_herodata->getSquadDirection()==klqCombatSquadDirectionRight){
        fname1 = "flag_hong.png";
    }
    
    this->setColor( Color3B(100,0,0) );
    //头像框
    Sprite* _flagbg = Sprite::createWithSpriteFrameName("flag_ditu.png");
    _flagbg->setTag(20000);
    _flagbg->setAnchorPoint(Point(0, 0.5));
    //血条 zd_xuetiaolv.png
    Sprite* backgroundSprite = Sprite::createWithSpriteFrameName("flag_fangguangdi.png");
    pogressSprite = Sprite::createWithSpriteFrameName("flag_xuetiaolv.png");
    Sprite* thumbFile = Sprite::create("res/blank.png");
    m_hpSlider = ControlSlider::create( backgroundSprite, pogressSprite, thumbFile);
    m_hpSlider->setRotation(-90);
    m_hpSlider->setMaximumValue(m_hpMax);
    m_hpSlider->setMinimumValue(0);
    m_hpSlider->setValue(m_hpValue);
    //护盾血条 zd_xuetiaolv.png
    Sprite* hdbackgroundSprite = Sprite::createWithSpriteFrameName("flag_fangguangdi.png");
    pogressHdSprite = Sprite::createWithSpriteFrameName("flag_xuetiaoy.png");
    Sprite* thumb2File = Sprite::create("res/blank.png");
    m_hdSlider = ControlSlider::create( hdbackgroundSprite, pogressHdSprite, thumb2File);
    m_hdSlider->setRotation(-90);
    m_hdSlider->setMaximumValue(m_hdValue<=0? 1 : m_hdValue);
    m_hdSlider->setMinimumValue(0);
    m_hdSlider->setValue(m_hdValue);
    m_hdSlider->setVisible(m_hdValue>0);
    
    //兵种的背景
    Sprite* _flagiconbg = Sprite::createWithSpriteFrameName(fname1.c_str());
    _flagiconbg->setTag(20002);
    _flagiconbg->setAnchorPoint(Point(0.5, 1));
    
    Size csize  = Size(_flagbg->getContentSize().width+m_hpSlider->getContentSize().height, _flagbg->getContentSize().height);
    this->setContentSize(csize);
    _flagbg->setPosition(Point(0, this->getContentSize().height/2));
    this->addChild(_flagbg,0);
    
    //英雄名称
    csize  = Size(_flagbg->getContentSize().width, _flagbg->getContentSize().height*0.8);
    int fsize = m_herodata->getName().length();
    fsize = fsize>6 ? 20: 22;
    hpTtfName = Label::createWithSystemFont(m_herodata->getName().c_str(), "Helvetica", fsize, csize, TextHAlignment::CENTER, TextVAlignment::TOP);  //16
    //hpTtfName->enableShadow();
    hpTtfName->setColor(Color3B::WHITE);
    hpTtfName->setAnchorPoint(Point(0.5, 1.0));
    hpTtfName->setTag(20005);
    hpTtfName->setPosition(Point(_flagbg->getContentSize().width/2, _flagbg->getContentSize().height*0.78));
    _flagbg->addChild(hpTtfName,0);
    
    //发光的血条背景
    sp_clclight = Sprite::createWithSpriteFrameName("flag_faguang.png");
    sp_clclight->setAnchorPoint(Point(0.5, 1));
    sp_clclight->setPosition(Point(this->getContentSize().width-m_hpSlider->getContentSize().height/2, this->getContentSize().height-_flagiconbg->getContentSize().height*0.2));
    sp_clclight->setVisible(false);
    this->addChild(sp_clclight,1);
    m_hpSlider->setPosition(Point(sp_clclight->getPositionX(), sp_clclight->getPositionY()-sp_clclight->getContentSize().height/2));
    this->addChild(m_hpSlider,10);
    m_hdSlider->setPosition(Point(sp_clclight->getPositionX()+m_hpSlider->getContentSize().height, sp_clclight->getPositionY()-sp_clclight->getContentSize().height/2));
    this->addChild(m_hdSlider,10);
    //兵种的背景位置
    _flagiconbg->setPosition(Point(_flagbg->getContentSize().width/2, _flagbg->getContentSize().height));
    _flagbg->addChild(_flagiconbg,0);
    // 兵种图标
    string resfile;
    string armicon = m_herodata->getArmsCfgdata()->getSmallIcon().c_str();
    LQ::stringf(resfile, "res/level/%s", armicon.c_str());
    Sprite* _flagarmicon = Sprite::create(resfile.c_str());
    //_flagarmicon->setScale(0.7);
    _flagarmicon->setTag(20004);
    _flagarmicon->setPosition(Point(_flagiconbg->getContentSize().width/2, _flagiconbg->getContentSize().height*0.5));
    _flagiconbg->addChild(_flagarmicon);
    
    _buffPos = Point(0, _flagbg->getContentSize().height );
    
    //
    //sp_barlight = Sprite::createWithSpriteFrameName("flag_xuetiaohong.png");
    //sp_barlight->setPosition(100,100);
    //this->addChild(sp_barlight,-11);
    //sp_barlight->setVisible(false);
    //sp_barlight->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.8), FadeIn::create(0.5), NULL)));
    
    return true;
}

/*
bool LQFlagsSprite::initWithData11(LQSquadUserData* data)
{
    m_hpValue = data->getSquadHpValue();
    m_squaddata = data;
    m_herodata = data->getHeroData();
    string fname1 = "flag_border.png";
    string fname2 = "flag_B_bar.png";
    string fname3 = "flag_B_bing.png";
    string fname4 = "flag_B_barlight.png";
    string fname5 = "flag_B_guang.png";
    if(m_herodata->getSquadDirection()==klqCombatSquadDirectionRight){
        fname2 = "flag_R_bar.png";
        fname3 = "flag_R_bing.png";
        fname4 = "flag_R_barlight.png";
        fname5 = "flag_R_guang.png";
    }
    //头像框
    Sprite* _flagbg = Sprite::createWithSpriteFrameName("flag_mark.png");
    _flagbg->setTag(20000);
    _flagbg->setAnchorPoint(Point(0.5, 0.5));
    Sprite* _flagborder= Sprite::createWithSpriteFrameName(fname1.c_str());
    _flagborder->setTag(20001);
    _flagborder->setAnchorPoint(Point(0.5, 0.5));
    //数值条
    Sprite* _flagbar = Sprite::createWithSpriteFrameName(fname2.c_str());
    _flagbar->setAnchorPoint(Point(0.0, 0.5));
    _flagbar->setOpacity(200);
    _flagbar->setTag(20002);
    //兵种条
    Sprite* _flagbing = Sprite::createWithSpriteFrameName(fname3.c_str());
    _flagbing->setAnchorPoint(Point(0.0, 0.5));
    _flagbing->setTag(20003);
    this->setTextureRect(Rect(0,0,_flagborder->getContentSize().width*0.9+_flagbar->getContentSize().width,_flagborder->getContentSize().height));
    
    string resfile;
    string armicon = m_herodata->getArmsCfgdata()->getSmallIcon().c_str();
    LQ::stringf(resfile, "res/level/%s", armicon.c_str());
    Sprite* _flagarmicon = Sprite::create(resfile.c_str());
    _flagarmicon->setAnchorPoint(Point(0.5, 0.5));
    _flagarmicon->setTag(20004);
    hpTtfName = Label::createWithSystemFont(intToString(m_hpValue/100).c_str(), "Helvetica", 26);  //16
    hpTtfName->setAnchorPoint(Point(0.0, 0.5));
    hpTtfName->setTag(20005);
    _flagborder->setPosition(Point(_flagborder->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(_flagborder,10);
    _borderPos = _flagborder->getPosition();
    _flagbg->setPosition(Point(_flagborder->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(_flagbg,-1);
    _flagbar->setPosition(Point(_flagborder->getContentSize().width*0.85, this->getContentSize().height/2));
    _barPos = _flagbar->getPosition();
    _buffPos = Point(_flagborder->getContentSize().width, _barPos.y-_flagbar->getContentSize().height/2);
    this->addChild(_flagbar,1);
    hpTtfName->setPosition(_flagbar->getContentSize().width*0.35,_flagbar->getContentSize().height/2);
    hpTtfName->setColor(Color3B::WHITE);
    _flagbar->addChild(hpTtfName,100);
    _flagbing->setPosition(Point(_flagborder->getContentSize().width*0.89, this->getContentSize().height*0.51));
    this->addChild(_flagbing,2);
    _flagarmicon->setPosition(Point(_flagbing->getContentSize().width*0.55, _flagbing->getContentSize().height/2));
    _flagbing->addChild(_flagarmicon);
    
    
    //创建“裁减模板”
    Sprite* _flagmark = Sprite::createWithSpriteFrameName("flag_mark.png");
    _flagmark->setPosition( Point(_flagmark->getContentSize().width/2,_flagmark->getContentSize().height/2) );
    //创建裁减节点类
    auto clipper = ClippingNode::create();//设置裁剪节点
    clipper->setInverted(false);//设置底板可见
    clipper->setAlphaThreshold(0.0f);//设置透明度Alpha值为0
    clipper->setAnchorPoint(Point(0.5, 0.5));
    clipper->setPosition(Point(_flagborder->getContentSize().height/2-_flagmark->getContentSize().width/2, _flagborder->getContentSize().height/2-_flagmark->getContentSize().height/2));
    this->addChild(clipper,5);
    
    //为设置裁减节点类设置“裁减模板”
    clipper->setStencil(_flagmark);
    
    //设置裁减节点类所放的内容
    stringf(m_heroRes, "res/head/%s", data->getHeroData()->getHeadID().c_str());
    Sprite* _flaghead = Sprite::create(m_heroRes.c_str());
    _flaghead->setPosition( Point(_flaghead->getContentSize().width/2,_flaghead->getContentSize().height/2) );
    clipper->addChild(_flaghead);
    
    //40020001
    //圆圈的发光
    sp_clclight = Sprite::createWithSpriteFrameName(fname5.c_str());
    sp_clclight->setAnchorPoint(Point(0.5, 0.5));
    sp_clclight->setPosition( _borderPos );
    this->addChild(sp_clclight,3);
    sp_clclight->setVisible(false);
    sp_clclight->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.8), FadeIn::create(0.5), NULL)));
    
    //数据条的发光
    sp_barlight = Sprite::createWithSpriteFrameName(fname4.c_str());
    sp_barlight->setAnchorPoint(Point(0.5, 0.5));
    sp_barlight->setPosition(Point(_flagbar->getContentSize().width*0.51, _flagbar->getContentSize().height/2));
    _flagbar->addChild(sp_barlight,10);
    sp_barlight->setVisible(false);
    sp_barlight->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.8), FadeIn::create(0.5), NULL)));
    
    return true;
}*/

//更新数值
void LQFlagsSprite::updateValue()
{
    if(m_squaddata && m_hdSlider && m_hdValue != m_squaddata->getCurrShValue()){
        if (m_hdValue<=0){
            m_hdSlider->setMaximumValue(m_squaddata->getCurrShValue());
        }
        m_hdValue = m_squaddata->getCurrShValue();
        m_hdSlider->setValue(m_hdValue);
        m_hdSlider->setVisible(m_hdValue>0);
    }
    if(m_squaddata && m_hpValue != m_squaddata->getSquadHpValue()){
        m_hpValue = m_squaddata->getSquadHpValue();
        
        //CCLOG("%s Flags::updateValue %d %d", m_squaddata->getHeroData()->getName().c_str(), m_hpValue, m_squaddata->getHpMaxValue());
        if(m_hpSlider)
            m_hpSlider->setValue(m_hpValue);
        
        //if(hpTtfName)
        //    hpTtfName->setString(intToString(m_hpValue).c_str());
        
        //if(m_hpValue<=0  || m_squaddata->getHeroData()->getCurrHpValue()<=0 )  //死了
        //    addMark(1);
        //else if( ((m_hpValue*1.0f)/m_squaddata->getHpValueMax()) < 0.15 )  //血量少于15%
        //    addMark(0);
        
        if( ((m_hpValue*1.0f)/m_squaddata->getHpValueMax()) < 0.27 )  //血量少于27%
        {
            if(!sp_clclight->isVisible()){
                sp_clclight->setVisible(true);
                ActionInterval* act = RepeatForever::create(Sequence::create(FadeOut::create(0.8), FadeIn::create(0.5), NULL));
                sp_clclight->runAction(act);
            }
        }
        
        //怒气满了，血条框闪烁
        //if(m_squaddata->getHeroData()->getHeroFuriesRadio()){
        //    sp_barlight->setVisible(true);
        //}else sp_barlight->setVisible(false);
    }
    if (this->getZOrder()>3000){
        _zordertimes++;
        if(_zordertimes>60){
            this->setLocalZOrder(2048);
            _zordertimes = -1;
        }
    }
}

//添加蒙城 type 0 亏血 1 死亡
void LQFlagsSprite::addMark(int type)
{
    /*
    string fname1 = "flag_markred.png";
    if(type==1){  //死亡
        fname1 = "flag_mark.png";
        if(sp_markdie) return;
        if(sp_mark){
            sp_mark->stopAllActions();
            sp_mark->removeFromParentAndCleanup(true);
            sp_mark = NULL;
        }
    }else{
        if(sp_mark) return;
        sp_markdie = NULL;
    }
    
    //圆圈的蒙层
    Sprite* _mark = Sprite::createWithSpriteFrameName(fname1.c_str());
    _mark->setAnchorPoint(Point(0.5, 0.5));
    _mark->setPosition( _borderPos );
    _mark->setOpacity(120);
    this->addChild(_mark,11);
    
    
    if(type==1){
       sp_markdie = _mark;
       if(sp_clclight) sp_clclight->setVisible(false);
       if(sp_barlight) sp_barlight->setVisible(false);
    }else{
        _mark->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.7,120), DelayTime::create(0.2), FadeTo::create(0.5,50),  NULL)));
        sp_mark = _mark;
    }*/
}

//更新状态
void LQFlagsSprite::updateStatus(LQSquadStateEnum status)
{
    //if(!sp_clclight)
        return;
    //战斗状态，头像圆圈闪烁
    //if(status==FUSM_SQSTATE_ATTACK)
    //    sp_clclight->setVisible(true);
    //else
    //    sp_clclight->setVisible(false);
    
    //showBufferName(-1);
    this->removeChildByTag(99999);
    string stname;
    if(status==FUSM_SQSTATE_RUN)
        stname = "squadrun.png";
    else if(status==FUSM_SQSTATE_APPROACH)
        stname = "squadapproach.png";
    else if(status==FUSM_SQSTATE_ATTACK)
        stname = "squadattack.png";
    else if(status==FUSM_SQSTATE_WAIT)
        stname = "squadidea.png";
    else stname = "squadwait.png";
    Sprite* _mark = Sprite::createWithSpriteFrameName(stname.c_str());
    _mark->setTag(99999);
    _mark->setPosition( Point(this->getContentSize().width,this->getContentSize().height*0.95) );
    this->addChild(_mark,11);
}

Point LQFlagsSprite::getLinePosition(Point pt)
{
    Point _p = _position - pt;
    Point _rpt = Point::ZERO;
    float _x = 0;
    if(_p.x>0) _x = 0.95; else if(_p.x<0)  _x = -1;
    float _y1 = 0;
    if(_x<=0) _y1 = -this->getContentSize().height*0.3*getScale();
    float _y = 0;
    if(_p.y>0) _y = 1; else if(_p.y<0)  _y = -1;
    _rpt = _position - Point(_x*this->getContentSize().width*0.5*getScale(), _y*(this->getContentSize().height*0.51*getScale()+_y1));
    return _rpt;
}

void LQFlagsSprite::restartEffectIndex()  //复位
{
    _effectIndex = -1;
}

//显示BUFF技能名称
void LQFlagsSprite::showBufferName(string effectText)
{
    //effectText = "暴击上升";
    if(effectText.length()<=0)
        return;
    
    _effectIndex++;
    LQFlyNumber* pbuff = LQFlyNumber::createFlyWithBuffTxt(effectText.c_str(), klq_flaytype_bufftxt);
    Point pt = Point(this->getContentSize().width*0.0, this->getContentSize().height/2-_effectIndex*24);
    pbuff->setPosition( pt ); //+this->getPosition()
    //this->getParent()->addChild(pbuff,3000);
    this->addChild(pbuff,3000);
    pbuff->startAnimation(_effectIndex*0.5);
    this->setLocalZOrder(5000);
    if (_DEBUG_TYPE == _DEBUG_TYPE_VALUE || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG( " Idex:%d 显示BUFF技能名称%s ", _effectIndex, effectText.c_str());
    
    /* 5/18/15改成图形方式
    string _fontName = "res/fonts/skillname.fnt";
    Label* _label = Label::createWithBMFont(_fontName, effectText.c_str());  //"智力降低"
    //Sprite* _label = Sprite::createWithSpriteFrameName("n40021001.png");
    _label->setAnchorPoint(Point(1.0, 0.5));
    _label->setPosition( Point(this->getContentSize().width*0.0, this->getContentSize().height/2) );
    this->addChild(_label);
    _label->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.3,Point(0,_label->getContentSize().height*0.95)),
                                                             DelayTime::create(1.0), FadeOut::create(0.3), RemoveSelf::create(), NULL)));
     */
}

//添加BUFF图标
void LQFlagsSprite::addBufferIcon(LQBuffUserData* bufferData)
{
    if(!bufferData || bufferData->getFlagsIcon().length()<=0)
        return;
    Sprite* sp = Sprite::createWithSpriteFrameName(bufferData->getFlagsIcon().c_str());
    sp->setPosition(Point(_buffPos.x,_buffPos.y));
    sp->setAnchorPoint(Point(1,1));
    sp->setTag(bufferData->getTagId());
    this->addChild( sp,100 );
    _statussp.push_back(sp);
    this->refrusheBufferIcon();
}

//移除BUFF图标
void LQFlagsSprite::removeBufferIcon(LQBuffUserData* bufferData)
{
    Node* node = this->getChildByTag(bufferData->getTagId());
    NodeVector::iterator iter = remove(_statussp.begin(), _statussp.end(), node);
    if (iter != _statussp.end()) {
        _statussp.erase(iter);
    };
    if (node)
        node->removeFromParent();
    this->refrusheBufferIcon();
}

//重排BUFF图标
void LQFlagsSprite::refrusheBufferIcon()
{
    int _y = _buffPos.y;
    for (auto& child : _statussp)
    {
        if(child){
            child->setPosition(Point(_buffPos.x,_y));
            _y -= child->getContentSize().height*1.02;
        }
    }
}

LQ_NAMESPACE_END