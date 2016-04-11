//
//  LQBezier.cpp
//  非CCObject扩展
//  sgmz
//
//  Created by ByronSong on 13-4-10.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#include "LQBezier.h"
LQ_NAMESPACE

// 非CCObject扩展
LQBezier::LQBezier() : m_startPos(Point::ZERO), m_controlPos(Point::ZERO), m_targetPos(Point::ZERO), m_step(0), m_ax(0), m_ay(0), m_bx(0), m_by(0), m_a(0), m_b(0), m_c(0), m_total_length(0)
{
    
}

LQBezier::LQBezier(Point startPos, Point controlPos, Point targetPos, float speed) : m_startPos(startPos), m_controlPos(controlPos), m_targetPos(targetPos), m_step(0), m_ax(0), m_ay(0), m_bx(0), m_by(0), m_a(0), m_b(0), m_c(0), m_total_length(0)
{
    
}

LQBezier::~LQBezier()
{
}

// 根据步数，获得坐标及旋转角度
void LQBezier::getAnchorPoint(vector<float>& list, int nIndex)
{
    list.clear();
    if (nIndex >= 0 && nIndex <= m_step)
    {
        float t = (float)nIndex/m_step;
        //  如果按照线行增长，此时对应的曲线长度
        float l = t*m_total_length;
        //  根据L函数的反函数，求得l对应的t值
        t = inertLen(t, l);
        
        //  根据贝塞尔曲线函数，求得取得此时的x,y坐标
        float xx = (1 - t) * (1 - t) * m_startPos.x + 2 * (1 - t) * t * m_controlPos.x + t * t * m_targetPos.x;
        float yy = (1 - t) * (1 - t) * m_startPos.y + 2 * (1 - t) * t * m_controlPos.y + t * t * m_targetPos.y;
        
        //  获取切线
        Point Q0 = Point((1 - t) * m_startPos.x + t * m_controlPos.x, (1 - t) * m_startPos.y + t * m_controlPos.y);
        Point Q1 = Point((1 - t) * m_controlPos.x + t * m_targetPos.x, (1 - t) * m_controlPos.y + t * m_targetPos.y);
        
        //  计算角度
        float dx = Q1.x - Q0.x;
        float dy = Q1.y - Q0.y;
        float radians = atan2f(dy, dx);
        // // 设置旋转了多少度
        // OpenGLES 的旋转角度和cocos2d sprite的旋转角度相反，所以要乘以-1.
        float degrees = CC_RADIANS_TO_DEGREES(radians * -1); //radians * 180 / M_PI;
        //        NSLog(@"degrees:%0.2f", degrees);
        
        list.push_back(xx);
        list.push_back(yy);
        list.push_back(degrees);
    }
}

// 根据步数，获得坐标及旋转角度
void LQBezier::getAnchorPoint(vector<float> &list, int nIndex, const Point &startPos, const Point &controlPos, const Point &targetPos, float speed){
    calcStep(startPos, controlPos, targetPos, speed);
    getAnchorPoint(list, nIndex);
}

//  计算步数
int LQBezier::calcStep(const Point &startPos, const Point &controlPos, const Point &targetPos, float speed)
{
    setStartPos(startPos);
    setControlPos(controlPos);
    setTargetPos(targetPos);
    //step = 30;
    
    m_ax = m_startPos.x - 2 * m_controlPos.x + m_targetPos.x;
    m_ay = m_startPos.y - 2 * m_controlPos.y + m_targetPos.y;
    m_bx = 2 * m_controlPos.x - 2 * m_startPos.x;
    m_by = 2 * m_controlPos.y - 2 * m_startPos.y;
    
    m_a = 4*(m_ax * m_ax + m_ay * m_ay);
    m_b = 4*(m_ax * m_bx + m_ay * m_by);
    m_c = m_bx * m_bx + m_by * m_by;
    
    //  计算长度
    m_total_length = length(1);
    
    //  计算步数
    m_step = floorf(m_total_length / speed);
    int len = m_total_length;
    int s = speed;
    if ((len % s) > (speed / 2)) m_step++;
    
    return m_step;
}

//  速度函数
float LQBezier::speed(float t)
{
    return sqrtf(m_a * t * t + m_b * t + m_c);
}

// 长度函数
float LQBezier::length(float t)
{
    float temcontrolPos = sqrtf(m_c + t * (m_b + m_a * t));
    float temtargetPos = (2 * m_a * t * temcontrolPos + m_b *(temcontrolPos - sqrtf(m_c)));
    float temp3 = logf(m_b + 2 * sqrt(m_a) * sqrtf(m_c));
    float temp4 = logf(m_b + 2 * m_a * t + 2 * sqrtf(m_a) * temcontrolPos);
    float temp5 = 2 * sqrtf(m_a) * temtargetPos;
    float temp6 = (m_b * m_b - 4 * m_a * m_c) * (temp3 - temp4);
    
    return (temp5 + temp6) / (8 * powf(m_a, 1.5));
}
//  长度函数反函数，使用牛顿切线法求解
float LQBezier::inertLen(float t, float l)
{
    float t1 = t;
    float t2;
    do
    {
        t2 = t1 - (length(t1) - l)/ speed(t1);
        if (isnan(t2)) {
            break;
        }
        if (fabsf(t1-t2) < 0.000001) break;
        t1 = t2;
    }while(true);
    return t2;
}
LQ_NAMESPACE_END