//
//  LQBezier.h
//  非CCObject扩展
//  sgmz
//
//  Created by ByronSong on 13-4-10.
//  Copyright (c) 2013年 LequWorld. All rights reserved.
//

#ifndef __SuperWarriors__LQBezier__
#define __SuperWarriors__LQBezier__

#include "LQCore.h"

USING_NS_CC;
using namespace std;
LQ_NAMESPACE
// 非CCObject扩展
class LQBezier {
    // 起点
    CC_SYNTHESIZE_PASS_BY_REF(Point, m_startPos, StartPos)
    // 控制点
    CC_SYNTHESIZE_PASS_BY_REF(Point, m_controlPos, ControlPos);
    // 终点
    CC_SYNTHESIZE_PASS_BY_REF(Point, m_targetPos, TargetPos);
    // 分割份数
    CC_SYNTHESIZE(int, m_step, Step);
    
    // 辅助变量
    CC_SYNTHESIZE(int, m_ax, AX);
    CC_SYNTHESIZE(int, m_ay, AY);
    CC_SYNTHESIZE(int, m_bx, BX);
    CC_SYNTHESIZE(int, m_by, BY);
    
    CC_SYNTHESIZE(float, m_a, A);
    CC_SYNTHESIZE(float, m_b, B);
    CC_SYNTHESIZE(float, m_c, C);
    
    CC_SYNTHESIZE(float, m_total_length, TotalLength);
    
public:
    LQBezier();
    LQBezier(Point startPos, Point controlPos, Point targetPos, float speed);
    ~LQBezier();
    
    // 根据步数计算新的坐标以及转向
    void getAnchorPoint(vector<float>& list, int nIndex);
    void getAnchorPoint(vector<float>& list, int nIndex, const Point& startPos, const Point& controlPos, const Point& targetPos, float speed);
    
    // 计算步数
    int calcStep(const Point& startPos, const Point& controlPos, const Point& targetPos, float speed);
    
private:
    // 速度函数
    float speed(float t);
    //  长度函数
    float length(float t);
    //  长度函数反函数，使用牛顿切线法求解
    float inertLen(float t, float length);
};

LQ_NAMESPACE_END

#endif /* defined(__sgmz__LQBezier__) */
