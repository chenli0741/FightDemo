//
// LQCombatHero.h
// SuperWarriors
// 武将或者英雄
// Created by chenli on 14-6-17.
// Copyright lequworld.com
//

#ifndef __SuperWarriors__LQCombatHero__
#define __SuperWarriors__LQCombatHero__

#include "cocos2d.h"

#include "LQCombatSoldier.h"
//#include "Component/LQSkillSprite.h"

USING_NS_CC;

LQ_NAMESPACE

static int Hero_Halo_Tag_Id = 20100;

class LQCombatHero : public LQCombatSoldier
{
public:
    LQCombatHero(LQBaseUserData* data,LQCombatSquadDirectionEnum direction);
    ~LQCombatHero();
    
    virtual bool init();
    static LQCombatHero* create(LQBaseUserData* data,LQCombatSquadDirectionEnum direction);
    
    LQSquadUserData* getSquadData();
    
    //
    void changeAnimTo_CorpseToGrave();
    
    //释放被动技能
    void showPassiveSkill();
    //武将的一些动作效果
    void showHeroHalo(int resId);   //显示武将光环
    void removeHeroHalo();          //移除武将光环
    void restoreHeroHalo();         //恢复武将光环
    void visibleHeroHalo(bool val);
    void changeSkillAnimation();    //更改技能动作changeAnimation
    void changeAnimationByType(LQUnitAnimPhaseEnum phaseType,LQUnitDirectionType direction);
    
    //技能攻击处理
    bool isCanFreeActiveSkill();//是否可以释放主动技能
    void showSkill(LQSkillUserData* skilldata);   //技能释放的人口
    void skillAttack();                                  //技能攻击,在动作帧中调用
    void skillAttackReady(LQEffectUserData* effectData); //技能的施放动作攻击帧已经播放结束，开始收尾动作
    void skillAttackEnd(LQEffectUserData* effectData);    //只有当被动技能，和主动技能才会调用这个方法，需要结尾处理，是否添加buff
    //技能单个命中特效
    void skillHitEffect(LQEffectUserData* effectData,int m);
    void skillHitReady(LQEffectUserData* effectData, GameObjectVector* targetObjects);     //技能的攻击命中特效前段结束，开始伤害
    void skillHitEnd(LQSkillUserData* effectData);       //技能的攻击命中特效结束，也是整个技能的结束
    void skillHitEndCallBack(Ref* pSender);
    
    //将怒气增量减少
    void reduceHeroFuries(int var);
    //变更怒气 最大不得超过最大怒气上限
    void changeHeroFuries(int value);
    // 弓箭攻击系列动作
    void shootAttack();
    //普攻
    void normalAttackEnd();//普通攻击结束
    void normalAttackReady(){};// 普通攻击动作完成，准备进行收尾动作
    
    void timeAttack();  //用于武将的时间输出伤害
    
    //静止技能
    void stopSkill(bool value);
    
    //实现父类的虚方法，用于改变其身上buff 的方向
    virtual void setDirection(LQUnitDirectionType direction);
    //设置站立状态
    virtual void setIsStand(bool value);
    
    // update
    void update(float dt, Layer *layer);
    void updateSkill(float dt, Layer *layer);//更新技能专用方法
    
    
    //获取对象的战斗公式计算器
    LQAttackDataInterface* getCalculator();
    //攻击目标
    void changeTargetDps(GameObj* pTarget,int dps,LQFlyNumberTypeEnum numbertype = klq_flaytype_normal,LQEffectInfoData* effectdata = NULL,int times = 0, float outtime = 0);
    
    //Buff 覆盖方法
    virtual void bufferCallback(Ref* pSender);     //buff触发数据动作
    virtual void bufferEndCallback(Ref* pSender);  //buff结束动作
    
    //回调事件
    std::function<void(Node*,LQBaseUserData*,CallFunc*)> onSkillStartCallback;
    std::function<void(Node*,LQBaseUserData*)> onHitStartCallback;
    std::function<void(Node*,LQBaseUserData*)> onSkillEndCallback;
    std::function<void(Node*,LQBaseUserData*)> onHeroDiedCallback;
    
    //
    CC_SYNTHESIZE(ControlButton*, m_heroButton, HeroButton );
    //
    CC_SYNTHESIZE_READONLY(int, m_radius, Radius );
    CC_SYNTHESIZE_READONLY(LQHeroUserData*, m_herodata, HeroData);//武将数据
    CC_SYNTHESIZE(string, m_critPic, CritPic );
    CC_SYNTHESIZE(string, m_critFont, CritFont);
    //英雄剧情条件
    CC_SYNTHESIZE(int, m_storyIDFuries, StoryIDFuries);  //怒气满足StoryID
    CC_SYNTHESIZE(int, m_storyHPRate, StoryHPRate);    //HP满足
    CC_SYNTHESIZE(int, m_storyIDHP, StoryIDHP);        //HP满足StoryID
    CC_SYNTHESIZE(bool, m_furiesPilot, FuriesPilot);    //怒气导航
    CC_SYNTHESIZE(int, m_storyIDIsDead, StoryIDIsDead);    //死亡满足StoryID
    
    //军团溃败了
    void squadIsDied();
    //
    Rect getHeroBoundingBox();
    //
    bool isShowSkill(){ return m_skillVector.size()>0; };
private:
    float m_attackTime; //攻击时间
    float m_unitTime;   //单位攻击时间
    int _atkCount;   //攻击次数
    float _squadfuries; //击杀怒气
    int _heroHaloResId;
    float _isDeadhasSkillTime;
    
    int m_currentEffect;
    bool m_currentEffectEnd;
    
    LQSkillUserDataVector m_skillVector;
    LQSkillUserDataVector m_passiveSkillVector;
    void initData();
protected:

};

LQ_NAMESPACE_END
#endif /* defined(__game__LQCombatHero__) */