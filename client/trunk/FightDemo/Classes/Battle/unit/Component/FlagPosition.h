//
//  LQFlagPosition.h
//  LostEmpire
//  军团旗帜
//  Created by chenli on 14-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#ifndef __LQFlagPosition_H__
#define __LQFlagPosition_H__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
#include "LQStringExtend.h"
#include "LQTemplate.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

LQ_NAMESPACE

static int flagTimesLeft = 80;
static int flagTimesRight = 0;
static const Point drawFlagPositionsLeft[] = {
    Point(0,flagTimesRight),
    Point(-0.1*flagTimesLeft,flagTimesRight),
    Point(0.1*flagTimesLeft,flagTimesRight),
    Point(-0.3*flagTimesLeft,flagTimesRight),
    Point(0.3*flagTimesLeft,flagTimesRight),
    Point(-0.6*flagTimesLeft,flagTimesRight),
    Point(0.6*flagTimesLeft,flagTimesRight),
    Point(-0.9*flagTimesLeft,flagTimesRight),
    Point(0.9*flagTimesLeft,flagTimesRight),
    Point(-1.0*flagTimesLeft,flagTimesRight),
    Point(1.0*flagTimesLeft,flagTimesRight)
 };

 static const Point drawFlagPositionsRight[] = {
     Point(0,flagTimesRight),
     Point(-0.1*flagTimesLeft,flagTimesRight),
     Point(0.1*flagTimesLeft,flagTimesRight),
     Point(-0.3*flagTimesLeft,flagTimesRight),
     Point(0.3*flagTimesLeft,flagTimesRight),
     Point(-0.6*flagTimesLeft,flagTimesRight),
     Point(0.6*flagTimesLeft,flagTimesRight),
     Point(-0.9*flagTimesLeft,flagTimesRight),
     Point(0.9*flagTimesLeft,flagTimesRight),
     Point(-1.0*flagTimesLeft,flagTimesRight),
     Point(1.0*flagTimesLeft,flagTimesRight)
 };
 
/*static const Point drawFlagPositionsLeft[] = {
    Point(-108,64),
    Point(-0.940*flagTimesLeft,0.342*flagTimesLeft),
    Point(-1*flagTimesLeft,0*flagTimesLeft),
    Point(-0.940*flagTimesLeft,-0.342*flagTimesLeft),
    Point(-0.766*flagTimesLeft,-0.643*flagTimesLeft),
    Point(-0.5*flagTimesLeft,-0.866*flagTimesLeft),
    Point(-0.174*flagTimesLeft,-0.985*flagTimesLeft),
    Point(0.174*flagTimesLeft,-0.985*flagTimesLeft),
    Point(0.5*flagTimesLeft,-0.866*flagTimesLeft),
    Point(0.766*flagTimesLeft,-0.643*flagTimesLeft),
};
static const Point drawFlagPositionsRight[] = {
    //Point(108,-25), 踩脚下的位置
    //Point(84,116),顶头上效果不错但挡脸
    Point(84,96),
    Point(-0.766*flagTimesRight,0.643*flagTimesRight),
    Point(-0.5*flagTimesRight,0.866*flagTimesRight),
    Point(-0.174*flagTimesRight,0.985*flagTimesRight),
    Point(0.174*flagTimesRight,0.985*flagTimesRight),
    Point(0.5*flagTimesRight,0.866*flagTimesRight),
    Point(0.766*flagTimesRight,0.643*flagTimesRight),
    Point(0.940*flagTimesRight,0.342*flagTimesRight),
    Point(1*flagTimesRight,0*flagTimesRight),
    Point(0.940*flagTimesRight,-0.342*flagTimesRight),
};*/
/**
 static const Point drawFlagPositionsLeft[] = {
    Point(-108,64),
    Point(-120,208),
    Point(-154,184),
    Point(-184,154),
    Point(-208,120),
    Point(-225,82),
    Point(-236,42),
    Point(-240,0),
    Point(-236,-42),
    Point(-225,-82),
    };
static const Point drawFlagPositionsRight[] = {
    Point(108,-32),
    Point(150,260),
    Point(193,230),
    Point(230,193),
    Point(260,150),
    Point(282,103),
    Point(295,52),
    Point(300,0),
    Point(295,-52),
    Point(282,-103),
};
 **/

class FlagPosition
{
public:
	FlagPosition();
	~FlagPosition();
    
    static FlagPosition* getInstance();
    static void clear();
    
    //初始化所有武将和旗帜所在点的位置
    bool initWithDatas();
    void clearPoints();
    //根据传入的坐标绘制旗帜，第二个参数为真则为蓝方（左面），否则是红方（右面）
    Point drawFlag(Point pt,bool isLeft, Node* node);
	Point drawFlag(Point rolePoint,bool isLeft, Node* node, bool isfight);
    //添加一个新的位置
    void pushNode(Node* node);
    //移除一个的位置
    void removeNode(Node* node);
private:
	float m_flagWidth;//旗帜的宽,判断位置冲突时根据这个值进行计算
	float m_flagHeight;//旗帜的高,判断位置冲突时根据这个值进行计算
	float m_radius;//旋转半径，计算旗帜放置地点时使用
	cocos2d::Vector<Node*> m_objects;//存储所有武将和旗帜所在点的位置
    
    //根据传入值来计算旗帜要绘制的坐标
	Point calcPosition(Point point, int num, bool isLeft);
    //检查当前点是否和已有武将或旗帜位置冲突
	bool checkPosition(Point point, Node* node);
};
LQ_NAMESPACE_END
#endif
