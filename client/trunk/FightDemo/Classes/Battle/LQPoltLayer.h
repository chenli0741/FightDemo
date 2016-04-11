//
//  LQPoltLayer.h
//  SuperWarriors
//
//  Created by lequ on 14-9-18.
//
//

#ifndef __SuperWarriors__LQPoltLayer__
#define __SuperWarriors__LQPoltLayer__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#include "cocosbuilder/CocosBuilder.h"
using namespace cocosbuilder;
using namespace std;
//#include "LQZoneUserData.h"
//#include "LoadBattlefield.h"

struct MSG_PDATA{
    int types;
    string value;
    int cardid;
    string name;
    string headname;
    int dir;
};

class LQPoltLayer;
class LQPoltLayerDelegate
{
public:
    virtual void poltEnd(LQPoltLayer *polt) = 0;
};


class LQPoltLayer
:public Layer
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
,public NodeLoaderListener
{
public:
    static Scene* createScene();
    static LQPoltLayer* createLayer();
    void initUI();
    void setDelegate(LQPoltLayerDelegate *delegate) {this->_delegate = delegate;}
    bool getIsThereAre(int pid);
    
    void onEnter();
    void onExit();
    
    virtual bool onTouchBegan(Touch* touch, Event  *event) override;
    virtual void onTouchMoved(Touch* touch, Event  *event) override;
    virtual void onTouchEnded(Touch* touch, Event  *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
    
    // 设置对话内容
    void setPolt(std::string value);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * pNode);
    virtual void onNodeLoaded(cocos2d::Node * node, cocosbuilder::NodeLoader * nodeLoader);

private:
    bool initLayer();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LQPoltLayer, create);
    
    LQPoltLayerDelegate *_delegate;
    
    Label *nameLabel;
    Label *valueLabel;
    Layer *dirLayer;
    Layer *taskBG;
    Sprite *downSP;
    Layer *layerAni;
    Layer *touchLayer;
    Layer *bottomNode;
    Scale9Sprite *dixia;
    Scale9Sprite *dishang;
    
    
    int sessionIndex; // 对话索引
    int beforeDir;
    
    vector<MSG_PDATA> m_pdata;// 内容结构
    void setNextPolt(); // 下一句话哈
    
    std::string strKey;
    int pidKey;
    void saveThePoltId();
    
    void aniEnd();
    
    void aniCallback();
    
    // 设置后续触发动作 chenli 5.13
    CC_SYNTHESIZE(string, m_triggerEvent, TriggerEvent);
};


#endif /* defined(__SuperWarriors__LQPoltLayer__) */
