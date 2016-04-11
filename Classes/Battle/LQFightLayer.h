//
// LQFightLayer.h
// SuperWarriors
//
// Created by chenli on 14-6-16.
// Copyright lequworld.com
//

#ifndef __SuperWarriors__LQFightLayer__
#define __SuperWarriors__LQFightLayer__

#include "cocos2d.h"

#include "LQConfig.h"
#include "LQCore.h"
#include "LQCombatSquad.h"
#include "../extensions/cocos-ext.h"
USING_NS_CC_EXT;

LQ_NAMESPACE_USE;
#include "LQHeroButton.h"
#include "MaskLayer.h"
#include "LQPoltLayer.h"
//#include "LQSkillShowLayer.h"
#include "LQSkillEffectListener.h"
#include "LQGutInfoData.h"
#include "BattleGuideLayer.h"
using namespace std;

LQ_NAMESPACE

#define NODE_TAG_CTRLLAYER  10000        //控制层
#define NODE_TAG_FIGHTMAP   10001        //地图层
#define NODE_TAG_SHAKE      10002        //SHAKE层
#define NODE_TAG_SKILLMAKE  100011       //技能描述蒙层
#define NODE_TAG_SKILLBAR   100012       //技能说明条
#define NODE_TAG_SKILLDESC  100051       //技能描述
#define NODE_TAG_SKILLGUIDE 100060       //技能释放引导层

#define NODE_TAG_GUIDE     100099       //自动战斗引导层
#define NODE_TAG_PVEWIN    1008610       //战斗胜利界面
#define NODE_TAG_PVELOST   1008611       //战斗失败界面

#define NODE_ZORDER_MAX    9999999       //最大层
#define NODE_ZORDER_GUT    1999999       //剧情层
#define NODE_ZORDER_MASK   20000         //蒙层
#define NODE_ZORDER_DIALOG 1500          //对话层
#define NODE_ZORDER_CTRLLAYER 100        //控制层
#define NODE_ZORDER_CLOUD  5001          //云层

class LQFightLayer : public Layer,public LQPoltLayerDelegate
{
public:
    LQFightLayer();
    ~LQFightLayer();
    virtual bool init();
    void initData(string valuestr);
    CREATE_FUNC(LQFightLayer);
    
    //CC_SYNTHESIZE_READONLY(LQZoneUserData*, m_currZoneData, CurrZoneData);//地图数据
    void getMessageToFightHorse(); //通知战斗胜利失败界面收到消息
    //多点响应
    void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event);
    //void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event);

    Size mapSize;       //场景的尺寸
    Size visibleSize;   //屏幕的尺寸
    Point origin;
    float mapMinWidth,mapMinHeight;  //可移动控制的值
    bool m_isStared;
    Layer* m_BossHpLayer; //boss血条节点
    Sprite* spriteHp[3];
    Layer* m_controlLayer;
    void initLayer();
    void initControls();
    void refushButtonPostion();
    
    LQCombatSquadVector squads;
    CC_SYNTHESIZE(LQCombatSquadVector*, m_mySquadVector, MySquadVector);//我方将领
    CC_SYNTHESIZE(LQCombatSquadVector*, m_otherSquadVector, OtherSquadVector);//敌方将领
    
    bool isTouchInside(Node* pChild,    Touch* pTouch);
    vector<LQHeroButton*>* m_skillButtonVector;
    vector<ControlButton*>* m_normalButtonVector;
    
    LQHeroButton* m_currentSkillButton;
    ControlSlider* m_myHpSlider;  //我方所有军团的总血量
    ControlSlider* m_otherHpSlider[3];// 敌方的所有军团的总血量
    Label* m_timeSp;                //时长
    MaskLayer* m_maskLayer;
    Layer* m_shakeLayer;
    LayerColor* m_pauseLayer;
    
    void itemTouch(Ref * sender, Control::EventType controlEvent);
    void OnAutoButtonClicked(ControlButton *sender);
    void OnPauseButtonClicked(ControlButton *sender);
    void OnSpeedButttonClicked(ControlButton *sender);
    void OnIconButtonClicked(Ref * sender, Control::EventType controlEvent);
    
    void iconButtonResponse(Touch* pTouch);//点击界面来释放技能
    void showSkillMaskLayer(Point touchPoint, float scale = 1);
    void hadeSkillMaskLayer();
    void shakeScene();          //振屏
    
    //场景放缩控制
    static const float LAYER_SCALEMAX;
    static const float LAYER_SCALEMIN;
    
    // update per frame
    void update(float dt);
    
    //测试格子显示
    void testSpritexr(SpriteBatchNode* parent, int x,int y);
    
    //放缩比率 
    float nowScale, lastScale;
    float m_beginPinchLength;  //手势开始距离长度
    bool isMultipleTouch;  //多点触摸
    int moveTouchID;       //控制移动的单点touchID
    //手势操作放缩移动
    void doHandlePinchFrom(float scale);
    void doSceneScale(float nowScale,float time);//执行缩放操作,传入非0有风险
    bool isSceneScaleing;
    void scaleToCallback(Ref* pSender);
    void scaleDrawmArmflags();
    
    //英雄死了回调
    void heroDiedCallback(Node* pSender,LQBaseUserData* data);
    
    //Skill CallBack方法
    void skillStartCallback(Node* pSender,LQBaseUserData* data,CallFunc* callback);
    void skillDescCallback(Node* pSender);
    void hitStartCallback(Node* pSender,LQBaseUserData* data);     //Skill触发数据动作
    void skillEndCallback(Node* pSender,LQBaseUserData* data);     //Skill结束动作
    //void skillStreamEndCallback(Ref* pSender);                   //Skill Stream结束动作
    //技能Event响应
    void onEffectBegan(LQEventSkillEffect* event);
    void onEffectEnded(LQEventSkillEffect* event);
    
    //拖动开始位置
    Point m_oldPoint;
    //移动背景
    void doHandleMoveFrom(Point pt);
    void onEnter();
    void onExit();
    
    //调用剧情
    bool playPoltLayer(int storyID,int tag,bool isneedPass = true);
    // 剧情结束后开始战斗
    void plotEndStarGame();
    // 剧情结束后显示胜利界面
    void plotEndStarShowWin();
    void showWinLayer();
    void showPolt();
    string loadPlotPlayText(int idc);
    std::string m_gameWinValue; // 胜利界面内容
    // 战斗前奏
    void beginStartGame();
    //战斗开始剧情结束, 回调正式开始
    void poltEnd(LQPoltLayer *polt);
    //为了跳过对话
    void poltTalkEnd(int polttype, string triggerEvent);
    //技能剧情引导回调
    void plotSkillGuide(Ref* pSender);
    bool plotSkillTouchBegan(Touch *touch, Event *event);
    
    //开始战斗
    void gameBegin();
    void pauseGame();
    void resumeGame();
    bool isPausedGame();
    void doStartBattleCallback(Ref* pSender);  //startBattle回调

    //战斗结束
    void gameOver(bool win,int recStar, string awardItem);
    void gameOver(bool win,LQHeroUserDataVector it,string str);
    CC_SYNTHESIZE(bool, m_isGameOver, IsGameOver);//游戏结束
    
    //发送战斗结果
    void showNetPrompt(bool isopen);
private:
    int _fightType;
    
    int _startStoryID;
    float _storyIDTime;  //剧情触发时间 现在只支持一个
    float _furiesTime;   //怒气恢复周期
    float _furiesTimeParam; //怒气增加间隔
    float _furiesAddRadio;  //怒气增加系数
    int _timeStoryID;
    string _triggerEvent;
    
    //void skillShowDescription(LQCombatHero* hero,int type,float speed=1);//展示释放技能的动画,参数是施法英雄以及展示ID,根据展示ID不同，特效也不同,时间也预设好了，需要调时间的话传入第三个参数速率，数值越大速度越快
    void skillRelease();   //英雄释放技能
    
    ActionInterval* createAnimation(string hname, int num);
    
    LQCombatHero* _skillhero;
    LQEffectUserData* _effectdata;
    CallFunc* _skillcallback;
    LQCombatHero* _plothero;  //剧情引导的英雄
    LQCombatHero* _devilhero; //貂蝉魔王特殊
    
    //LQSkillShowLayer* m_skillShowLayer;//用来展示技能释放的层
    float m_time_showing_skill;
    bool m_now_showing_skill;//现在是否正在释放技能
    bool m_now_showing_plot; //现在是否正在播放剧情
    
    int m_recStar;
    bool m_win;
    Sprite* m_sprite;
    bool _isPausedLayer;  //暂停界面
    Scene* m_scene;
    unsigned int m_meleeEffect;
    
    Vector<Node*> _anodes;
};

LQ_NAMESPACE_END
#endif /* defined(__game__LQFightLayer__) */