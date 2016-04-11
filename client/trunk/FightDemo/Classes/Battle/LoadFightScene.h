//
//  LoadFightScene.h
//  SuperWarriors
//
//  Created by lequdev1 on 14-8-12.
//
//

#ifndef __SuperWarriors__LoadFightScene__
#define __SuperWarriors__LoadFightScene__

#include <iostream>
#include "cocos2d.h"
#include "LQCore.h"
#include "LQFightLayer.h"
#include "../extensions/cocos-ext.h"
USING_NS_CC_EXT;

LQ_NAMESPACE_USE;
using namespace std;
class LoadFightScene : public Layer
{
public:
    LoadFightScene();
    ~LoadFightScene();
    
    static Scene* createScene();
    void initData(int attackList[], int defenceList[],int ftype=-1);
    void initData(std::string valuestr,int ftype=-1);  //战斗类型ftype传入-1按valuestr判断
    void initCloud();
    bool init();
    void onEnter();
    void onExit();
    void initDataToBattle();
    std::string m_valueStr;
    ValueMapIntKey m_resFile;
    int m_index;
    void update(float dt);
    
    void initUI();
    
    void showLayer();
    
    Label *loadInfoLabel;
//    ParticleSystemQuad *particle;
    
    CREATE_FUNC(LoadFightScene);
protected:
    std::string valueBattle;
    bool boolBattle;
    LQFightLayer* _fightlayer;
};
#endif /* defined(__SuperWarriors__LoadFightScene__) */
