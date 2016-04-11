//
//  LQPVELoseLayer.h
//  game
//
//  Created by lequdev1 on 14-7-26.
//
//

#ifndef __game__LQPVEWinLayer__
#define __game__LQPVEWinLayer__

#include <iostream>

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
#include "../Socket/ComService.h"
#include "LQHeroUserData.h"
#include <vector>
USING_NS_CC;
USING_NS_CC_EXT;

//#include "cocosbuilder/CocosBuilder.h"
//using namespace cocosbuilder;

using namespace std;
USING_NS_CC;
#define NODE_ZORDER_DIALOG 1500          //对话层
#define NODE_TAG_YUN 2200          //  云tag
////#include "LQZoneUserData.h"
#include "LoadBattlefield.h"

//typedef struct labelParaStruct{
//    Node* _parent;
//    const std::string& _str;
//    const char _FontSize;
//    Color3B _FontColor;
//    const Point _FontPos;
//    const Point _AnchorPoint;
//}labelpara;

typedef struct spConfig{
    Point anchorP;
    Point spPos;
    std::string spName;
}SPC;

class LQPVEWinLayer:public Layer
{
public:
    LQPVEWinLayer();
    ~LQPVEWinLayer();
    
    static LQPVEWinLayer* createLayer();
    bool init();
    void initUI();
    void initUI(string str);
    void initBossUI(LQHeroUserDataVector it ,string str);
    void initAwardItem(string awardItems,int recStar);
    void setDataToUI(string str);//良驹争夺战接受翻牌消息调用来刷新界面
    void drawGetItemList(string str);
    
    void onEnter();
    void onExit();
    
    virtual bool onTouchBegan(Touch* touch, Event  *event) override;
    virtual void onTouchMoved(Touch* touch, Event  *event) override;
    virtual void onTouchEnded(Touch* touch, Event  *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
    
    void onBtnClickGoOn(Ref * sender, Control::EventType controlEvent);
    void onBtnClickDialog(Ref * sender, Control::EventType controlEvent);
    void onBtnClickDialogClose(Ref * sender, Control::EventType controlEvent);
    void OnBtnSendData(Ref * sender, Control::EventType controlEvent);
    
    void sendDataHorse();
    string getContentByQulity(int q);
    std::string getFragmentGridByQulity(int q);
    Sprite* star[3];
    LayerColor *layerNode; // 背景layerNode  所有的UI都会绘制在这上面  这个在去适配
    LayerColor* layerHorseNode; //良驹背景层
    LayerColor* layerDialog; //良驹背景层
    
    Label* m_obtainLabel[4]; // 获得的 1等级 2经验 3金币 4魂魄值
    Label* m_describeLabel[3]; // 星级的描述 1一颗星星的描述 2两块醒醒的描述 3...
    
    ControlButton* buttonHorse;// 返回键
    ControlButton* buttonOne;
    Layer* awardlayer;  //绘制领奖滑动层
    
    Label* m_HorseLabel; //获得马的碎片
    Sprite* m_HorsePai[4]; //翻牌的层
    Sprite* m_HorseSpriteNode[4];  //获得奖励的精灵父节点
    Sprite* m_HorseSpritePai[4];  //翻牌的精灵
    Label* m_HorseNameLabel[4]; //获得奖励名称
    Label* m_HorseNumLabel[4]; //获得奖励数量

    void OnBtnClickSelect(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent);
    bool isFanPai = true;
    void runActionForCard(int actionID ,float time,std::vector<std::string> data);
    void callback1(int actionID, std::vector<std::string> data);
    void shakeScreen(int i);
    void notShakeScreen(int i);
    void playFightWinEffect(int i);
    void OnClickCheckAward(Ref * sender, Control::EventType controlEvent);
private:
    int tag;
    bool isGetHorse;
    Label* _createFont(Node* _parent,const std::string& _str,const char _FontSize, Color3B _FontColor,const Point _FontPos,const Point _AnchorPoint);
    Sprite* _createSp();
    LayerColor* _createLayerColor();
    ControlButton* _createButton();
};

#endif /* defined(__game__LQPVELoseLayer__) */
