//
//  FlagPosition.cpp
//  LostEmpire
//
//  Created by chenli on 13-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "FlagPosition.h"

using namespace cocos2d;
LQ_NAMESPACE

FlagPosition::FlagPosition()
{
    m_flagWidth = 42;  //旗帜的宽,判断位置冲突时根据这个值进行计算
	m_flagHeight= 153;  //旗帜的高,判断位置冲突时根据这个值进行计算
	m_radius= 1;      //旋转半径，计算旗帜放置地点时使用
}


FlagPosition::~FlagPosition()
{
    clearPoints();
}

static FlagPosition* m_flagnstance = NULL;
FlagPosition* FlagPosition::getInstance()
{
    if (!m_flagnstance) {
        m_flagnstance = new FlagPosition();
    }
    
    return m_flagnstance;
}

void FlagPosition::clear()
{
    CC_SAFE_DELETE(m_flagnstance);
}

bool FlagPosition::initWithDatas()
{	/**
     *
     *		未实现
     **/
	//将当前已绘制的所有武将坐标和旗帜坐标赋值给本类的私有变量positions，然后return true;
    
	return false;
}

void FlagPosition::clearPoints()
{
    //deleteInVector(m_objects);
    m_objects.clear();
}

Point FlagPosition::drawFlag(Point pt, bool isLeft, Node* node)
{
    return pt;
}

//调用initPositions初始化完成后调用本方法，传入武将坐标和所处阵营即可返回旗帜位置
Point FlagPosition::drawFlag(Point rolePoint, bool isLeft, Node* node, bool isfight){
    if(!isfight){//如果没战斗，不让旗子跳了，被踩住也不管
        return calcPosition(rolePoint, 0, isLeft);
    }
//    int b0 = isfight ? 1 : 0;
    int b0 =1;
    for (int num = b0; num < 10;num++){//把20个坐标依次循环判断
			Point tempPoint = calcPosition(rolePoint, num, isLeft);//得到相应的测试点
			if (checkPosition(tempPoint,node))//如果这个点可以画，则直接返回该点坐标
			{
				return tempPoint;
			}
	}
	//这儿应该返回一个找点失败的标志，比如当点坐标是（-2000，-2000）时判定失败，不画图
    return calcPosition(rolePoint, 0, isLeft); //Point(-2000,-2000);
}

//根据传入值计算旗帜坐标
Point FlagPosition::calcPosition(Point point, int num,bool isLeft){
	if(isLeft){
        m_radius = -1;
        return Point(point.x + m_radius*drawFlagPositionsLeft[num].x, point.y + drawFlagPositionsLeft[num].y);
	}else{
        m_radius = 1;
        return Point(point.x + m_radius*drawFlagPositionsRight[num].x, point.y + drawFlagPositionsRight[num].y);
    }
}

//检查传入坐标是否符合规范
bool FlagPosition::checkPosition(Point point, Node* node){
	//遍历positions as nowPosition{
    for(const auto &node1 : m_objects)
	{
        if(node1==node)
            continue;
        Point position = node1->getPosition();
		if (abs(point.x - position.x)<1.9 * m_flagWidth  && abs(point.y - position.y)<1.2 * m_flagHeight){
			return false;
		}
	}
	return true;
}

//添加一个新的位置
void FlagPosition::pushNode(Node* node)
{
    m_objects.pushBack(node);
}

//移除一个的位置
void FlagPosition::removeNode(Node* node)
{
    if (node == nullptr)
	{
		return;
	}
	m_objects.eraseObject(node);
}

LQ_NAMESPACE_END