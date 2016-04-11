//
// LQCombatSoldier.h
// SuperWarriors
// 士兵
// Created by chenli on 14-6-17.
// Copyright lequworld.com
//

#ifndef __SuperWarriors__LQCombatSoldier__
#define __SuperWarriors__LQCombatSoldier__

#include "cocos2d.h"

#include "LQCombatUnit.h"

USING_NS_CC;

LQ_NAMESPACE

class LQCombatSoldier : public LQCombatUnit
{
public:
    LQCombatSoldier(LQBaseUserData* data,LQGameObjType type,LQCombatSquadDirectionEnum direction);
    ~LQCombatSoldier();
    
    virtual bool init();
    //CREATE_FUNC(LQCombatHero);
    static LQCombatSoldier* create(LQBaseUserData* data,LQCombatSquadDirectionEnum direction);
    
    // 射击
    CC_SYNTHESIZE(bool, m_isShoot, IsShoot);
public:
    //实现父类的虚方法，用于改变其身上buff 的方向
    virtual void setDirection(LQUnitDirectionType direction);
    //箭矢的位置
    Point getMissileTargetPostion();
    Point getMissileStartPostion();
    //攻击处理
    virtual void normalAttackEnd(){};   //普通攻击状态结束，即将进行下一次攻击动作
    virtual void normalAttackReady();   //攻击准备动作完成，对方减血
    //技能攻击
    virtual void skillAttack(){};
    
    //攻击目标
    //virtual void changeTargetDps(GameObj* pTarget,int dps,LQFlyNumberTypeEnum numbertype = klq_flaytype_normal);
    
    //显示刀光
    void shootFlash();
    // 弓箭攻击系列动作
    virtual void shootAttack();
    void shootEnd(Node * node);
    
    //buff 处理
    void addBufferIcon(LQBuffUserData* bufferData);     //添加BUFF图标
    void addBufferBmp(LQBuffUserData* bufferData);     //添加BUFF图标
    void removeBufferIcon(LQBuffUserData* bufferData);
    
    void addBuffer(LQBuffUserData* bufferData);       //添加BUFF
    void removeBuffer(LQBuffUserData* bufferData);    //删除BUFF
    void cleareBuffer();
    
    //buff 处理
    void addBufferAnimation(LQAnimationInfoData* animaData,int tag);     //显示BUFF Animation
    
    //void changeBuffValue(LQEffectUserData* buffdata);//buff 状态瞬时输出的变化
    //清除 valuetype 该属性的类型
    //void clearBuffData(int valueType);
    
    // 通过类型改变属性值
    //void setUserDataValueByType(int valueType, float changeValue,LQCombatUnit* pSender);
    
    //特效展示
    //void showDamage(int hp,LQFlyNumberTypeEnum numbertype = klq_flaytype_normal);     //显示伤害
    //void showCrit();       //显示暴击
    //void showMiss();       //显示闪避
    //void showParry();      //显示格挡
    void showEffect(LQShowEffectTypeEnum eftype);     //显示自身位置的特效（刀光，闪避，格挡，加速等）
    void showSmoke(int effectID, Point posi = Point(0, 0)); //显示烟尘
    
    //更改动画
    void changeAnimation() override;
    
    void update(float dt, Layer *layer);
private:
    void initData();
protected:
    string shoot_effect_res[3];
    //Sprite* playSmoke(int resID);
    //ActionInterval* createAnimation(int resID);
};
typedef vector<LQCombatSoldier*> LQCombatSoldierVector;
LQ_NAMESPACE_END
#endif /* defined(__game__LQCombatHero__) */