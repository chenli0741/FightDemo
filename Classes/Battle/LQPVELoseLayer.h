//
//  LQPVELoseLayer.h
//  game
//
//  Created by lequdev1 on 14-7-26.
//
//

#ifndef __game__LQPVELoseLayer__
#define __game__LQPVELoseLayer__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
#include "LoadBattlefield.h"
#include "BattleGuideLayer.h"

#define UPGEN_BUTTON      1     //武将升级
#define UPEQUIP_BUTTON    2     //装备升级
#define GETEQUIP_BUTTON   3     //获得装备
#define UPQUALITY_BUTTON  4     //武将生品
#define UPSTAR_BUTTON     5     //武将升星
#define _TAG_GUIDE    100100 //引导层


class LQPVELoseLayer
:public Layer
{
public:
    LQPVELoseLayer();
    ~LQPVELoseLayer();
    
    static LQPVELoseLayer* createLayer();
    bool init();
    void initUI();
    void initUI(string str);
    void initAwardItem(string awardItems,int recStar);
    void setDataToUI(string str);
    void PVELoseGuid();
    void onEnter();
    void onExit();
    
    virtual bool onTouchBegan(Touch* touch, Event  *event) override;
    virtual void onTouchMoved(Touch* touch, Event  *event) override;
    virtual void onTouchEnded(Touch* touch, Event  *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
    
    void OnChangeFormationButtonClicked(Ref * sender, Control::EventType controlEvent);
    void OnEquipmentStrongButtonClicked(Ref * sender, Control::EventType controlEvent);
    void OnHeroStrongButtonClicked(Ref * sender, Control::EventType controlEvent);
    void upStarButtonClicked(Ref * sender, Control::EventType controlEvent);
    void upQualityButtonClicked(Ref * sender, Control::EventType controlEvent);
    void chengeEquButtonClicked(Ref * sender, Control::EventType controlEvent);
    void onBtnClickGoOn(Ref * sender, Control::EventType controlEvent);

    // 战斗类型
    CC_SYNTHESIZE(int, m_fightType, FightType);
    
    LayerColor *layerNode; // 背景layerNode  所有的UI都会绘制在这上面  这个在去适配
    LayerColor* layerHorseNode; //良驹背景层
    ControlButton* buttonHorse;// 返回键
    //BattleGuideLayer* guideLayer;//失败引导界面
    Layer* awardlayer;  //绘制领奖滑动层
    
    ControlButton* heroStrongBtn;
    ControlButton* equipmentStrongBtn;
    ControlButton* formationBtn;
    ControlButton* upStarBtn;
    ControlButton* upQualityBtn;
    ControlButton* chengeEquBtn;
    
    Label* m_HorseLabel; //获得马的碎片
    Sprite* m_HorsePai[4]; //翻牌的层
    Sprite* m_HorseSpriteNode[4];  //获得奖励的精灵父节点
    Sprite* m_HorseSpritePai[4];  //翻牌的精灵
    Label* m_HorseNameLabel[4]; //获得奖励名称
    Label* m_HorseNumLabel[4]; //获得奖励数量
    
    bool isHaveGuide;
    
    void OnBtnClickSelect(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent);
    bool isFanPai = true;
    void runActionForCard(int actionID ,float time,std::vector<std::string> data);
    void callback1(int actionID, std::vector<std::string> data);
    void shakeScreen(int i);
    void notShakeScreen(int i);
    ControlButton* getButton(int idx);
    
private:
    int tag;


};

#endif /* defined(__game__LQPVELoseLayer__) */
