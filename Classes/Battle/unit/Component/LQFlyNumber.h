//
//  LQFlyNumber.h
//  CZSQ
//
//  Created by chenli on 12-11-30.
//  Copyright 2012年 LequGame. All rights reserved.
//
#ifndef __LQFlyNumber_H__
#define __LQFlyNumber_H__

#include "cocos2d.h"
#include "LQStringExtend.h"

using namespace std;
USING_NS_CC;
//定义飘字类型
typedef enum{
    klq_flaytype_normal=0,   //正常  普通攻击，不显示数字，只改变属性值
    klq_flaytype_crit =1,   //暴击
    klq_flaytype_parry =2,  //格挡
    klq_flaytype_fklsg=3,   //假象不减少任何数值
    klq_flaytype_skill=4,   //技能
    klq_flaytype_buff=5,    //BUFF
    klq_flaytype_other=6,    //???
    klq_flaytype_criticon =7, //暴击
    klq_flaytype_bufftxt=8
} LQFlyNumberTypeEnum;


class LQFlyNumber : public Sprite
{
private:
    float duration;
    float _start;
    ActionInterval* createFiniteTimeAction1();
public:
    LQFlyNumber();
    ~LQFlyNumber();
    
    static LQFlyNumber* createFlyWithFile(string iconname, int num = 0, LQFlyNumberTypeEnum numbertype= klq_flaytype_normal, bool isSign = true, string critstr = "", const Point& numAnchor = Point::ZERO);
    static LQFlyNumber* createFlyWithSpriteFrameName(string iconname, int num = 0, LQFlyNumberTypeEnum numbertype= klq_flaytype_normal, bool isSign = true, string critstr = "", const Point& numAnchor = Point::ZERO);
    //BUFF飘字，图形化
    static LQFlyNumber* createFlyWithBuffTxt(string bufftxt, LQFlyNumberTypeEnum numbertype=klq_flaytype_bufftxt);
    
    bool initFlyBuffTxt(string bufftxt, LQFlyNumberTypeEnum numbertype);
    bool initFlyNumber(int number, LQFlyNumberTypeEnum numbertype,bool isSign, string critstr = "");
    //飘字类型
    CC_SYNTHESIZE(LQFlyNumberTypeEnum, m_numberType, NumberType);
    //是否显示‘+’ ‘-’
    CC_SYNTHESIZE(Label*, m_label, Label);
    CC_SYNTHESIZE(bool, m_isSign, IsSign);
    CC_SYNTHESIZE(int, m_value, Value);
    CC_SYNTHESIZE(string, m_passiveFontName, PassiveFontNumber);  //加血，正数
    CC_SYNTHESIZE(string, m_nagetiveFontName, NagetiveFontNumber); //伤血，负数
    
    CC_SYNTHESIZE(int, m_times, Times);
    
    void startAnimation(float start=0);
    void removeObject();
};

#endif