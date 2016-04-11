//
//  LQFlagsSprite.h
//  LostEmpire
//  军团旗帜
//  Created by chenli on 14-7-3.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#ifndef __LQFlagsSprite_H__
#define __LQFlagsSprite_H__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
#include "LQStringExtend.h"
#include "LQData.h"
#include "LQAction.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
#include "LQCombatUnitEnums.h"

LQ_NAMESPACE
class LQFlagsSprite : public Sprite
{
private:
    int _zordertimes;
    NodeVector _statussp;
    Point _borderPos;
    Point _barPos;
    Point _buffPos;
    int m_hpMax;
    int m_hpValue;
    int m_hdValue;
    LQHeroUserData* m_herodata;
    Sprite* createSprite();
    
    string m_heroRes;         //武将头像资源
    Label* hpTtfName;         //HP
    Sprite* sp_clclight;      //头像光圈
    Sprite* sp_barlight;      //血条光圈
    Sprite* sp_mark;          //头像蒙层
    Sprite* sp_markdie;       //头像蒙层
    Sprite* pogressSprite;
    Sprite* pogressHdSprite;
    int _effectIndex;
    
    ControlSlider* m_hpSlider;
    ControlSlider* m_hdSlider;
    
    //重拍BUFF图标
    void refrusheBufferIcon();
public:
    LQFlagsSprite();
    ~LQFlagsSprite();
    
    static LQFlagsSprite* createWithData(LQSquadUserData* data);
    //bool initWithData11(LQSquadUserData* data);
    bool initWithData2(LQSquadUserData* data);
    
    //更新数值
    void updateValue();
    
    //更新状态
    void updateStatus(LQSquadStateEnum status);
    
    //添加蒙城 type 0 亏血 1 死亡
    void addMark(int type);
    
    //显示BUFF技能名称
    void showBufferName(string effectText);
    void restartEffectIndex();  //复位
    //添加BUFF图标
    void addBufferIcon(LQBuffUserData* bufferData);
    //移除BUFF图标
    void removeBufferIcon(LQBuffUserData* bufferData);
    
    CC_SYNTHESIZE_READONLY(LQSquadUserData*, m_squaddata, SquadData );	//
    
    Point getLinePosition(Point pt);

};

LQ_NAMESPACE_END

#endif