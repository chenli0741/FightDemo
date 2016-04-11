//
// LQCombatHero.cpp
// SuperWarriors
//
// Created by chenli on 14-6-17.
// Copyright lequworld.com
//

#include "LQCombatHero.h"
#include "LQUnitAIControl.h"
#include "AudioManager.h"

#include "LQBattleSession.h"
#include "SimpleAudioEngine.h"
#include "arrow/LQMissile.h"

#define TestDistanceSpriteTagNumber 100//攻击范围测试tag
using namespace CocosDenshion;
LQ_NAMESPACE

LQCombatHero* LQCombatHero::create(LQBaseUserData* data,LQCombatSquadDirectionEnum direction)
{
    LQCombatHero *pRet = new LQCombatHero(data,direction);
    pRet->setUnitType(Combat_People_None);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

LQCombatHero::LQCombatHero(LQBaseUserData* data,LQCombatSquadDirectionEnum direction) :
LQCombatSoldier(data,klqLQGameObjHero,direction),m_attackTime(0),m_unitTime(0), onHitStartCallback(nullptr), onSkillEndCallback(nullptr), onSkillStartCallback(nullptr), onHeroDiedCallback(nullptr), m_heroButton(NULL)
{
    m_storyIDFuries = 0;  //怒气满足StoryID
    m_storyHPRate = 0;    //HP满足
    m_storyIDHP = 0;      //HP满足StoryID
    m_furiesPilot = false;
    m_storyIDIsDead = 0;
    
    _isDeadhasSkillTime = -1;
    m_currentEffect = 0;
    m_critPic = "";
    m_critFont = "";
    _heroHaloResId = -1;
    m_haloSprite = NULL;
    m_herodata = (LQHeroUserData*)data;
    m_herodata->setParent(this);
    m_speed = m_herodata->getSpeed();
    m_patrolSpeed = m_herodata->getPatrolSpeed();
    m_armsType = m_herodata->getArmsType();
    m_occupation = m_herodata->getOccupation();
    m_critPic = m_herodata->getArmsCfgdata()->getCritPic();
    m_critFont = m_herodata->getArmsCfgdata()->getCritFont();
    
    if(m_herodata->getSkillData1())
        m_passiveSkillVector.push_back(m_herodata->getSkillData1());
    if(m_herodata->getSkillData2())
        m_passiveSkillVector.push_back(m_herodata->getSkillData2());
    
    m_data->getCfgdata()->getValue("Radius", &m_radius);        //军团最大半径（单位：格子）
    
    if(!m_controller){
        m_controller = new LQUnitAIControl(this);
        m_controller->init();
    }
    
    
    stringf(m_name, "%s.%s", direction==klqCombatSquadDirectionLeft? "B":"R", m_armdata->getBaseName().c_str() );
    
    m_unitTime = AnimationFPS * m_armdata->getCurrAnimationPhaseCount(klqUnitAnimPhaseAttack);
    
    _atkCount = 0;
    if(m_attackDistMax>=14) m_attackDistMax = 12;  //武将TODO
    
    _squadfuries = getFormulaParamFactor(21, "a");     //杀死军团
    
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQCombatHero::LQCombatHero %p", this);
}

LQCombatHero::~LQCombatHero()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQCombatHero::~LQCombatHero %p", this);
}


bool LQCombatHero::init()
{
    if ( !LQCombatUnit::init() )
    {
        return false;
    }
    initData();
    return true;
}

#pragma mark - initData
void LQCombatHero::initData()
{
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // add your codes here...
    m_cdTime = 0;
}

LQSquadUserData* LQCombatHero::getSquadData()
{
    return m_squad ? m_squad->getSquadData() : NULL;
}

//---------------------------------------------------------
void LQCombatHero::update(float dt, Layer *layer)
{
    LQCombatSoldier::update(dt, layer);
    
    if(m_isDeadhasSkill)
        _isDeadhasSkillTime += dt;
    if(_isDeadhasSkillTime>3){
        CCLOG("m_isDeadhasSkill is ERROR....");
        m_skillVector.clear();
        squadIsDied();
        this->m_squad->dieUnit(this);
    }
    
    if (this->getPhaseType() == klqUnitAnimPhaseAttack ) {
        m_attackTime += dt;
        if (m_attackTime >= m_unitTime) {
            timeAttack();
            m_attackTime -= m_unitTime;
        }
    }
    else
    {
        m_attackTime = 0;
    }
    
    updateSkill(dt, layer);
}


void LQCombatHero::updateSkill(float dt, Layer *layer)//更新技能专用方法
{
    for (int i = 0;  i < m_skillVector.size(); i++) {
        LQSkillUserData* skillData = m_skillVector.at(i);
        skillData->update(dt);
        //CCLOG("技能 skillID = %d, lastTime = %f, currTime = %f",skillSprite->getSkillID(),skillSprite->getSkillLastTime(),skillSprite->getTime());
        if (skillData->getIsSkillEnd()) {
            //CCLOG("技能施放动作结束");
            skillData->setIsSkillEnd(false);
            //skillAttackEnd(skillData);
            break;
        }
        if (skillData->getIsHitReady()) {
            //CCLOG("技能的攻击命中特效前段结束，开始伤害");
            skillData->setIsHitReady(false);
            //skillHitReady(skillData);
            break;
        }
        if (skillData->getIsHitEnd()) {
            //CCLOG("技能的攻击命中特效结束，也是整个技能的结束");
            skillData->setIsHitEnd(false);
            //skillHitEnd(skillData);
            break;
        }
    }
}

//实现父类的虚方法，用于改变其身上buff 的方向
void LQCombatHero::setDirection(LQUnitDirectionType direction)
{
    LQCombatSoldier::setDirection(direction);
}

Rect LQCombatHero::getHeroBoundingBox()
{
    Rect rect = this->getBoundingBox();
    rect.size = Size(m_imgwidth, m_imgheight);
    return rect;
}

//显示武将光环
void LQCombatHero::showHeroHalo(int resId)
{
    if (resId<=0)
        return;
    if( Hero_Halo_Tag_Id==resId )
        return;
    
    if (m_haloSprite)
        m_haloSprite->removeFromParent();
    m_haloSprite= LQBuffSprite::createWithId(resId);
    m_haloSprite->setTag(Hero_Halo_Tag_Id);
    m_haloSprite->setAnchorPoint(Point(0.5, 0.35));  //haloSprite->getSkillAnchPosition()
    m_haloSprite->setPosition(this->getPosition());
    this->getParent()->addChild(m_haloSprite, this->getLocalZOrder()-1);
    m_haloSprite->play();
    _heroHaloResId = resId;
    m_haloSprite->setVisible(this->isVisible());
}

//恢复武将光环
void LQCombatHero::restoreHeroHalo()
{
    this->showHeroHalo(m_squadDirection==klqCombatSquadDirectionLeft?40029004:40029003);
}

//移除武将光环
void LQCombatHero::removeHeroHalo()
{
    if (!m_haloSprite)
        return;
    m_haloSprite->removeFromParent();
    m_haloSprite = NULL;
}

void LQCombatHero::visibleHeroHalo(bool val)
{
    if (!m_haloSprite)
        return;
    m_haloSprite->setVisible(val);
}

//更改技能动作changeAnimation
void LQCombatHero::changeSkillAnimation()
{
    changeAnimationByType(klqUnitAnimPhaseSkillAttack, klqUnitDirectionTypeNone);
}

//更改指定动作changeAnimation
void LQCombatHero::changeAnimationByType(LQUnitAnimPhaseEnum phaseType,LQUnitDirectionType direction)
{
    if (direction!=klqUnitDirectionTypeNone)
        m_direction = direction;
    m_phaseType = phaseType;
    changeAnimation();
}

//禁止技能
void LQCombatHero::stopSkill(bool value)
{
    m_herodata->setStopSkill(value);
}

//设置站立状态
void LQCombatHero::setIsStand(bool value)
{
    if (value==m_isStand)
        return;
    LQCombatUnit::setIsStand(value);
}

//攻击目标,目标的血量变化、、以及攻击者与被攻击者的怒气的处理
void LQCombatHero::changeTargetDps(GameObj* pTarget,int dps,LQFlyNumberTypeEnum numbertype,LQEffectInfoData* effectdata,int times, float outtime)
{
    LQGameObjType targetType = pTarget->getType();
    if (targetType==klqLQGameObjHero)
        pTarget = ((LQCombatHero*)pTarget)->getSquad();
    this->getSquad()->changeTargetDps(pTarget, dps, numbertype, effectdata, times, outtime);
    
/*
    //武将的伤血处理去掉了，现在只有军团的伤血处理，武将只处理状态和技能释放
    if (dps == 0) {
        return;
    }
 
    int hp1 = 0; int hp2 = 0;
    if( !this->getActive() || (pTarget && !pTarget->getActive()) )
        return;
    //_LQDEBUG(_DEBUG_TYPE_VALUE, "Squad[%d]::changeTargetDps.....");
    LQGameObjType targetType = pTarget->getType();//用于区分伤害类型  军团、武将
    //被攻击者
    if(targetType == klqLQGameObjHero)//武将
    {
        LQCombatHero* target = (LQCombatHero*)pTarget;
        target->getHeroData()->changeHpValue(dps*(-1));
        if (target->getHeroData()->getCurrHpValue() <= 0) {
            this->getHeroData()->changeKillTotal(1);
            //武将杀死武将
            ////this->getHeroData()->changeHeroFuries(_herofuries);
            ////_LQDEBUG(_DEBUG_TYPE_VALUE, "武将攻击武将 死亡 +怒气%f", _herofuries);
        }else{
            //给自己涨怒气
            ////float furyTimes = 0;
            ////if(numbertype<2){
                ////furyTimes = this->getHeroData()->getFuriesValue(dps, target->getHeroData(), target->getHeroData()->getRageAttack());
                ////this->getHeroData()->changeHeroFuries(furyTimes);
                ////_LQDEBUG(_DEBUG_TYPE_VALUE, "武将攻击武将 给自己涨怒气 +怒气%f", furyTimes);
            ////}
            //给被害者涨怒气
            ////furyTimes = target->getHeroData()->getFuriesValue(dps, this->getHeroData(), this->getHeroData()->getRageBeAttack());
            ////target->getHeroData()->changeHeroFuries(furyTimes);
            ////_LQDEBUG(_DEBUG_TYPE_VALUE, "武将攻击武将 给被害者涨怒气 +怒气%f", furyTimes);
        }
        
        target->getSquad()->drawmArmflags();
        hp1 = target->getHPValue();
        //CCLOG("军团[]HP squadData->getHpValue：%d", target->getSquadData()->getHpValue());
    }else if(targetType == klqLQGameObjSquad || targetType == klqLQGameObjSoldier){
        LQCombatSquad* squad = NULL;
        if (targetType == klqLQGameObjSquad) {
            squad = (LQCombatSquad*)pTarget;
        }else{
            LQCombatSoldier* soldier = (LQCombatSoldier*)pTarget;
            squad = soldier->getSquad();
        }
        
        //增加新的伤害 m_totalHurtHp+dps
        bool iscrit = numbertype == klq_flaytype_crit;
        bool isskill = numbertype == klq_flaytype_skill || numbertype == klq_flaytype_buff;
        squad->modifyHurtHp(dps, iscrit, m_critPic.c_str(), m_critFont.c_str(), isskill);
        
        if((_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_VALUE) && squad->getSquadData()->getCurrHpValue()<=0)
            CCLOG("%s 的兵都死了。。。。", squad->getHeroData()->getName().c_str());
        while ( squad->isCanKillUnit() ) {
            //死亡一个unit 进行处理
            LQCombatSoldier* dieunit = squad->getDieUnit();
            squad->killUnitHP();  //从伤害取出一个UnitHP干掉一个 m_totalHurtHp-UnitHP
            if (dieunit && dieunit->getActive()) {
                //用于统计杀敌
                if (dieunit->getArmdata()->getCurrHpValue() > 0) {
                    this->getHeroData()->changeKillTotal(1);
                    _LQDEBUG(_DEBUG_TYPE_VALUE, " HERO[%s] Die = %d",this->getHeroData()->getName().c_str(),this->getHeroData()->getKillTotal());
                    
                    //杀死小兵
                    //现在杀死小兵不给怒气了，杀掉兵团才给
                    //this->getHeroData()->changeHeroFuries(_soldierfuries);
                    //_LQDEBUG(_DEBUG_TYPE_VALUE, " 攻击小兵死亡 +怒气%f",_soldierfuries);
                }
                dieunit->getArmdata()->setHpValue(0);
                if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
                    CCLOG("Squad[%d] 兵团将要完蛋了 %p", squad->getIndex(), dieunit);
            }
        }
        
        //佳琪版本
        //给被害者涨怒气
        float furyTimes = 0;
        if(squad->getHeroData()){
            furyTimes = squad->getSquadData()->getFuriesValue(dps, squad->getSquadData(), squad->getSquadData()->getRageBeAttack());
            squad->getHeroData()->changeHeroFuries(furyTimes);
            if(_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_VALUE )
                CCLOG( "武将技能攻击军团 给被害者%p涨怒气 %d +怒气%.3f = %d ", squad->getHeroData(), squad->getHeroData()->getResIDC(), furyTimes, squad->getHeroData()->getHeroFuries());
        }
        
        //击杀掉军团一次性给很多怒气
        if(!squad->getActive()){
            this->getHeroData()->changeHeroFuries(_squadfuries);
            if(_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_VALUE )
                CCLOG("武将击杀军团 死亡 +怒气%f  dps:%d", _squadfuries, dps);
        }
        //else{
            //给自己涨怒气
            ////float furyTimes = 0;
            ////if(numbertype<2){
            ////    furyTimes = this->getHeroData()->getFuriesValue(dps, squad->getSquadData(), squad->getSquadData()->getRageAttack());
            ////    this->getHeroData()->changeHeroFuries(furyTimes);
            ////    _LQDEBUG(_DEBUG_TYPE_VALUE, "武将攻击军团 给自己涨怒气 +怒气%f",furyTimes);
            ////}
            //给被害者涨怒气,当对方存在军团时
            ////if(squad->getHeroData()){
            ////    furyTimes = squad->getHeroData()->getFuriesValue(dps, this->getHeroData(), this->getHeroData()->getRageBeAttack());
            ////    squad->getHeroData()->changeHeroFuries(furyTimes);
            ////    _LQDEBUG(_DEBUG_TYPE_VALUE, "武将攻击军团 给被害涨怒气 +怒气%f",furyTimes);
            ////}
        //}
        _LQDEBUG(_DEBUG_TYPE_VALUE, "军团[%d]HP squadData->getHpValue：%d", squad->getIndex() , squad->getSquadData()->getCurrHpValue());
        if(squad->getSquadData()->getCurrHpValue()==0)
            _LQDEBUG(_DEBUG_TYPE_VALUE, "军团[%d]HP squadData->getHpValue：%d %d", squad->getIndex() , squad->getSquadData()->getCurrHpValue(), squad->getSquadData()->getSquadHpValue());
        squad->drawmArmflags();
        hp2 = squad->getSquadData()->getCurrHpValue();
    }
    
    //怒气处理
    //攻击者怒气处理
    //this->getHeroData()->changeHeroFuries(this->getHeroData()->getRageAttack());
    
    //被攻击着怒气处理
    //if (targetType == klqLQGameObjHero) {
    //    LQCombatHero* hero = (LQCombatHero*)pTarget;
    //    hero->getHeroData()->changeHeroFuries(hero->getHeroData()->getRageBeAttack());
    //}
    if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
        CCLOG("****** 武将%d %s攻击后 敌人%s Hero:%d Squad:%d", this->getIndex(), this->getName().c_str(), pTarget->getName().c_str(), hp1, hp2);
*/
}

// 武将的时间输出伤害
void LQCombatHero::timeAttack()
{
    //12.18修改去掉武将伤害
    return;
    /*
    if (!getTarget() || !getTarget()->getActive()) {
        return;
    }
    LQGameObjType type = getTarget()->getType();
    
    if (type != klqLQGameObjSoldier && type != klqLQGameObjHero) {
        return;
    }
    int hurt = 0;
    _LQDEBUG(_DEBUG_TYPE_VALUE, "***武将的时间输出伤害\nLQCombatHero[%d]::timeAttack", m_index);
    LQCombatSoldier* soldier = (LQCombatSoldier*)getTarget();
    hurt = m_herodata->getCurrentDPS(soldier->getArmdata());
    if (type == klqLQGameObjHero) {//攻击对象为英雄
        this->changeTargetDps (soldier, hurt, m_herodata->getIsCrit()?klq_flaytype_crit:klq_flaytype_normal);
    }else if(type == klqLQGameObjSoldier){//攻击对象为兵、、----军团
        this->changeTargetDps (soldier->getSquad(), hurt, m_herodata->getIsCrit()?klq_flaytype_crit:klq_flaytype_normal );
    }
    
    _atkCount++;
    _LQDEBUG(_DEBUG_TYPE_VALUE, "timeAttack [%d次] 武将 %s 攻击 %s \n    武将输出伤害 %d  攻击对象为%s", _atkCount, this->getSquad()->getName().c_str(), soldier->getSquad()->getName().c_str(), hurt, type == klqLQGameObjHero?"英雄":"兵团");
    _LQDEBUG(_DEBUG_TYPE_VALUE, "    Hero::getSquadHpValue: %d", soldier->getSquad()->getSquadData()->getSquadHpValue());
     */
}

void LQCombatHero::shootAttack()
{
    LQCombatSoldier::shootAttack();
}

//普通攻击结束
void LQCombatHero::normalAttackEnd()
{
    
}

//获取对象的战斗公式计算器
LQAttackDataInterface* LQCombatHero::getCalculator()
{
    return (LQAttackCalculator*)this->getData();
}

//军团溃败了
void LQCombatHero::squadIsDied()
{
    //如果还有技能等待释放，让武将晚死一会子，这里有些怪，5.25先去掉
    if(m_skillVector.size()>0){
        m_isDeadhasSkill = true;
        _isDeadhasSkillTime = 0;
        return;
    }
    this->getHeroData()->setHpValue(0);
    if (this->onHeroDiedCallback)
        this->onHeroDiedCallback(this,m_herodata);
}

//
void LQCombatHero::changeAnimTo_CorpseToGrave()
{
    if(m_skillVector.size()>0){
        if (this->onSkillEndCallback)
            this->onSkillEndCallback(this,m_skillVector.at(0));
    }
    
    LQCombatUnit::changeAnimTo_CorpseToGrave();
    //死亡释放音效AtkSound
    if (m_herodata->getDthMusic() != "") {
        //CCLOG("Died Music:%s", m_herodata->getDthMusic().c_str());
        AudioManager::getInstance()->playEffect(m_herodata->getDthMusic().c_str());
    }
    //CCLOG("%p changeAnimTo_CorpseToGrave",this);
}

//将怒气增量减少,释放后怒气增量减少
void LQCombatHero::reduceHeroFuries(int var)
{
    m_squad->getSquadData()->setRageBeAttack(var);
    m_squad->getSquadData()->setRageAttack(var);
}
//变更怒气 最大不得超过最大怒气上限
void LQCombatHero::changeHeroFuries(int value)
{
    m_herodata->changeHeroFuries(value);
}

//释放被动技能
void LQCombatHero::showPassiveSkill()
{
    if (m_herodata==NULL)
        return;
    
    LQSkillUserDataVector::iterator iter;
    for (iter = m_passiveSkillVector.begin(); iter != m_passiveSkillVector.end(); ) {
        LQSkillUserData* _passiveSkill = (*iter);
        showSkill(_passiveSkill);
        if(_passiveSkill->getIsDoPassive())
            m_passiveSkillVector.erase(iter); //删除元素，返回值指向已删除元素的下一个位置
        else ++iter; //指向下一个位置
    }
    
    //if(m_herodata->getSkillData1())
    //    showSkill(m_herodata->getSkillData1());
    //if(m_herodata->getSkillData2())
    //    showSkill(m_herodata->getSkillData2());
}

//是否可以释放主动技能
bool LQCombatHero::isCanFreeActiveSkill()
{
    GameObj* target = this->getSquad()->getTarget();
    if (target && target->getActive()==false)
        return false;
    if (target==NULL && !m_herodata->isFirstSkill() )
        return false;
    
    bool doskill = m_herodata->isCanFreeActiveSkill();
    if(doskill && target){
        LQCombatSquad* squad = NULL;
        if (target->getType() == klqLQGameObjSquad) {
            squad = (LQCombatSquad*)target;
        }else{
            LQCombatSoldier* soldier = (LQCombatSoldier*)target;
            squad = soldier->getSquad();
        }
        if (squad && squad->getSquadData()->getCurrHpValue()<=0)
            return false;
        if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL && squad)
            CCLOG("%d %s自动释放技能", m_herodata->getResIDC(), squad->getName().c_str() );
    }
    return doskill;
}

//技能释放
void LQCombatHero::showSkill(LQSkillUserData* skilldata)
{
    if (skilldata == NULL)
        return;
    //技能释放类型，7.1新加的
    KLQSkillType skillShowType = skilldata->getSkillType();
    
    if(_DEBUG_TYPE == _DEBUG_TYPE_VALUE || _DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3 )
        CCLOG( "1) %p %s CombatHero::showSkill %p %d %ld", this, skillShowType==klqSkillShowType_ActiveSkill?"主动":"被动", this->getHeroData(), skilldata->getCfgdata()->getFID(), getCurrentMillSecond() );
    
    m_isSkilling = skillShowType == klqSkillShowType_ActiveSkill;
    
    //if(m_index==48 && _DEBUG_TYPE==_DEBUG_TYPE_SKILL2)
    //    CCLOG("Hero::showSkill %p %s %d", this, this->getName().c_str(), m_isSkilling);
    
    //主动被动技能
    if (skillShowType != klqSkillShowType_Passive && skillShowType != klqSkillShowType_ActiveSkill)
        return;
    
    skilldata->restore();
    
    //主动技能消耗怒气
    if (skillShowType == klqSkillShowType_ActiveSkill) {
        m_herodata->setHeroFuries(0);
        //CCLOG("%p herodata->setHeroFuries = 0",m_herodata);
        
        m_phaseType = klqUnitAnimPhaseSkillAttack;
        changeAnimation();
        
        //技能进入计时循环
        m_skillVector.push_back(skilldata);
    }else if (skillShowType == klqSkillShowType_Passive){
        bool doskill = m_herodata->isCanFreePassiveSkill(skilldata);
        if (doskill){
            for(LQEffectUserData* effectdata : *skilldata->getEffectDatas())
            {
                GameObjectVector* targetObjects = LQBattleSession::getInstance()->getGameObjListInSkill(effectdata, this);
                if(targetObjects->size()>0)
                    this->skillHitReady(effectdata,targetObjects);
                CC_SAFE_DELETE(targetObjects);
            }
            skilldata->setIsDoPassive(true);
        }else{
            //不能执行的放到技能执行队列中去
            CCLOG("%p 不能执行的放到技能执行队列中去 skilldata:%d",m_herodata, skilldata->getResIDC());
        }
    }
    return;
}

//技能释放
void LQCombatHero::skillAttack()
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("2) %p CombatHero::skillAttack currentEffect:%d %ld", this, m_currentEffect, getCurrentMillSecond() );
    
    //技能处理，总是处理第一个技能，一个一个处理
    if(m_skillVector.size()<=0)
        return;
    LQSkillUserData* _skillData = m_skillVector.at(0);
    if (_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
        CCLOG("   getSkillData _skillData:%d ", _skillData->getResIDC());
    //当前效果已经完成了，就从队列中删除
    if (m_currentEffect>=_skillData->getEffectDatas()->size()){
        //5.31 如果立刻进入技能结束，会因为动画没有播放完成而显得技能叠加
        //skillHitEnd(_skillData);
        if (m_currentEffectEnd) return;
        int _dalay = _skillData->getSkillCfgData()->getSkillTime();
        if(_dalay<=0) _dalay = 2000;
        FiniteTimeAction* endcall = CallFuncN::create(CC_CALLBACK_1(LQCombatHero::skillHitEndCallBack,this));
        this->runAction(Sequence::create(DelayTime::create(_dalay*0.001), endcall, NULL));
        m_currentEffectEnd = true;
        return;
    }
    
    m_currentEffectEnd = false;
    //5.22 如果想一次处理播放所有效果单元，要加上下面1行
    if (_skillData->getMutiEffectType()>0){
         if (m_currentEffect>0) return;
    };
        
    LQEffectUserData* effectData = _skillData->getEffectDatas()->at(m_currentEffect);
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("   getEffectData [%d] effectData:%d ", m_currentEffect, effectData->getResIDC());
    //技能开始动作
    if (m_currentEffect==0){
        m_armflags->restartEffectIndex();
        //技能释放音效AtkSound
        if (m_herodata->getAtkMusic() != "") {
            if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
                CCLOG("3) skillAttack playEffect:%s", m_herodata->getAtkMusic().c_str());
            AudioManager::getInstance()->playEffect(m_herodata->getAtkMusic().c_str());//m_herodata->getAtkMusic().c_str()
        }
        
        //技能释放突出武将特效
        if(m_herodata->getStuntEffect()>0){
            this->showHeroHalo(m_herodata->getStuntEffect());
        }
        
        if (this->onSkillStartCallback){
            auto callback = CallFunc::create(std::bind(&LQCombatHero::skillAttackReady,this,effectData));
            //callback->retain();
            this->onSkillStartCallback(this,effectData,callback);
        }
    }else skillAttackReady(effectData);
}

//技能攻击施放动画前段完成，弹道开始飞出去，或者命中了
void LQCombatHero::skillAttackReady(LQEffectUserData* effectData)
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("6) %p CombatHero::skillAttackReady %ld", this, getCurrentMillSecond() );
    //LQSkillUserData* skillUserData = skillData;//new LQSkillUserData(skillid);

    m_currentEffect++;
    //技能飞行特效,怎么飞行，对象是谁？TODO
    if (effectData->getMissileEffect()>0 && this->getTarget() && m_currentEffect==1)
    {
        Point frompt = this->getMissileStartPostion();
        Point topt = this->getTarget()->getMissileTargetPostion();
        LQAnimationSprite* sp = LQAnimationSprite::createWithId(effectData->getMissileEffect());
        if(sp){
            LQMissile* missileSp = LQMissile::create(Missile_Fly_ZX,sp);
            missileSp->setPosition(frompt);
            this->getParent()->addChild(missileSp,3000);

            auto callback = CallFunc::create(std::bind(&LQCombatHero::skillAttackEnd,this,effectData));
            missileSp->shoot(frompt, topt, callback, effectData->getHitEffect());
            return;
        }
    }
        
    this->skillAttackEnd(effectData);
}

//只有当被动技能，和主动技能才会调用这个方法，需要结尾处理，是否产生buff
void LQCombatHero::skillAttackEnd(LQEffectUserData* effectData)
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("7) %p CombatHero::skillAttackEnd %ld", this, getCurrentMillSecond() );
    
    effectData->setOutputTime(0);
    //
    GameObjectVector* targetObjects = LQBattleSession::getInstance()->getGameObjListInSkill(effectData, this);
    for (GameObj* targetobj : *targetObjects )
        targetobj->clearEffect();
    int m = 0;
    for (GameObj* targetobj : *targetObjects ) {
        if(targetobj->getType()==klqLQGameObjHero)
            ((LQCombatHero*)targetobj)->skillHitEffect(effectData,m);
        else
            ((LQCombatSquad*)targetobj)->skillHitEffect(effectData,m);
        m++;
    }
    //攻击伤害数据
    if(targetObjects->size()>0)
        this->skillHitReady(effectData,targetObjects);  //里面释放了targetObjects
   
    CC_SAFE_DELETE(targetObjects);
    
    if (this->onHitStartCallback)
        this->onHitStartCallback(this,effectData);
    
    //5.22 如果想一次处理播放所有效果单元，下面4行即可处理
    LQSkillUserData* _skillData = m_skillVector.at(0);
    if (_skillData->getMutiEffectType()>0){
        if (m_currentEffect<_skillData->getEffectDatas()->size()){
            LQEffectUserData* effectData = _skillData->getEffectDatas()->at(m_currentEffect);
            skillAttackReady(effectData);
        }
    }
    
    
    //判断技能结束
//    LQSkillUserData* _skillData = NULL;
//    bool isEnd = false;
//    if(m_skillVector.size()>0){
//        _skillData = m_skillVector.at(0);
//        //当前效果已经完成了，就从队列中删除
//        if (m_currentEffect>=_skillData->getEffectDatas()->size())
//            isEnd = true;
//    }else isEnd = true;
//    if(isEnd)
//        this->skillAttackEnd(_skillData);
    
    
    //这里是否要处理施放者的动作
    //  TODO
    /*
    //动画处理，从第一个效果取对象
    GameObjectVector* targetObjects = NULL;
    LQEffectUserData* effectdata = NULL;
    if (skillData->getEffectDatas()->size()>0){
        effectdata = skillData->getEffectDatas()->at(0);
        targetObjects = LQBattleSession::getInstance()->getGameObjListInSkill(effectdata, this);
    }else{
        targetObjects = new GameObjectVector();
        if(this->getTarget())
            targetObjects->push_back(this->getTarget());
    }
    for (GameObj* targetobj : *targetObjects ) {
        if(targetobj->getType()==klqLQGameObjHero)
            ((LQCombatHero*)targetobj)->skillHitEffect(NULL);
        else
            ((LQCombatSquad*)targetobj)->skillHitEffect(NULL);
    }
    CC_SAFE_DELETE(targetObjects);
     */
}

//技能单个命中特效
void LQCombatHero::skillHitEffect(LQEffectUserData* effectData,int m)
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("8.1) %p CombatHero::skillHitEffect %ld", this, getCurrentMillSecond() );
    
    int _hiteffect = effectData->getHitEffect();
    if (_hiteffect>0){
        LQBuffSprite* _hitSp = LQBuffSprite::createWithId(_hiteffect);
        _hitSp->setPosition(this->getHitPosition());
        if ( _hitSp->getHangingPoint()==4)  //底部
            this->getParent()->addChild(_hitSp, this->getLocalZOrder()-1);
        else this->getParent()->addChild(_hitSp,3001);
        _hitSp->play1();
    }
    
    return;
}

//技能的攻击命中特效前段结束，开始伤害
void LQCombatHero::skillHitReady(LQEffectUserData* effectdata, GameObjectVector* targetObjects)
{
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL || _DEBUG_TYPE==_DEBUG_TYPE_SKILL3)
       CCLOG("9) %p CombatHero::skillHitReady %d  ---%.f", this, effectdata->getResIDC(), getCurrentMillSecond() );
    
    int* c = LQBattleSession::getInstance()->getCombatSquadCount(this);
    this->getHeroData()->setCurrLiveAtr(c[0],c[1],c[2],c[3]);
    delete[]c;
    
    //for( LQEffectUserData* effectdata : *skillData->getEffectDatas())
    {
        int mainAttr = effectdata->getCfgdata()->getDominantAttribute();
        switch ( effectdata->getEffectType() ){
            case 1:  //1、瞬间加值或减值
            {
                int sign = effectdata->getCfgdata()->getBuffSubType()==1 ? 1 : -1;  //1、属性加值 2、属性减值
                int times = effectdata->getCfgdata()->getAtkTimes();
                float outtime = effectdata->getOutputTime();
                if (times<=0) times = 0;
                else
                    if (times>5) times = 5; else times = times-1;
                switch ( effectdata->getFormulaType() ) {
                    case 1:  //1、普通伤害
                    {
                        //times = 5;
                        effectdata->calculatorValue(this, targetObjects, m_herodata, m_squad->getSquadData(), mainAttr, times, outtime);
                    }
                        break;
                    case 3:
                    case 4:  //3、属性变动计算（取目标属性） 4、属性变动计算（取施法者属性）
                    {
                        effectdata->calculatorPropValue(this,targetObjects, m_herodata, mainAttr, sign, times, outtime);
                    }
                        break;
                    case 2:
                        CCLOGERROR("Buff瞬间加值或减值 ERROR is not 2号公式.");
                        break;
                    default:
                        break;
                }
            }
                break;
            case 2:  //2、Buff状态
            {
                //float _bufftime = effectdata->getCfgdata()->getBuffTime()/RATE_PERCENT;  //持续时间
                //float _buffvalue = effectdata->getCfgdata()->getBuffValue()/RATE_PERCENT;  //BUFF值
                //int _outputType = effectdata->getCfgdata()->getOutputType();  //输出类型
                int _buffSubType = effectdata->getCfgdata()->getBuffSubType();
                switch ( _buffSubType ) {
                    case 1:  //1、属性加值
                    {
                        if(effectdata->getFormulaType()==3 || effectdata->getFormulaType()==4){
                            effectdata->calculatorPropValue(this,targetObjects, m_herodata, mainAttr, 1);
                        }else{
                            effectdata->calculatorValue(this, targetObjects, m_herodata, m_squad->getSquadData(), mainAttr);
                            //CCLOG("Buff对属性加值 ERROR is not 3号公式.");
                        }                    }
                        break;
                    case 2:  //2、属性减值
                    case 62: //62、特殊属性减值
                    {
                        if(effectdata->getFormulaType()==1){ //DeBuff对属性的改变
                            effectdata->calculatorValue(this, targetObjects, m_herodata, m_squad->getSquadData(), mainAttr);
                        }else{
                            effectdata->calculatorPropValue(this,targetObjects, m_herodata, mainAttr, -1);
                            //CCLOG("Buff对属性加值 ERROR is not 1号公式.");
                        }
                    }
                        break;
                    case 5:  //5、间隔一定时间加值
                    case 15:  //15、间隔一定时间加值
                    {
                        effectdata->calculatorPropValue(this,targetObjects, m_herodata, mainAttr, 1);
                        break;
                    }
                    case 6:  //6、间隔一定时间减值
                    case 16:  //16、间隔一定时间减值
                    case 66:  //66、特殊间隔一定时间减值
                    case 56:  //56、姚果加的特殊DOT，不伤自己
                    {
                        //姚果加分支4.13
                        if(effectdata->getFormulaType()==3 || effectdata->getFormulaType()==4){
                            effectdata->calculatorPropValue(this,targetObjects, m_herodata, mainAttr, -1);
                        }else{
                            effectdata->calculatorValue(this, targetObjects, m_herodata, m_squad->getSquadData(), mainAttr);
                            //CCLOG("Buff对属性加值 ERROR is not 3号公式.");
                        }
                    }
                        break;
                    case 3:  //3、加速
                    case 4:  //4、减速
                    {
                        LQAttackCalculator* targetdata;
                        int sign = effectdata->getCfgdata()->getBuffSubType()==3 ? 1 : -1;  //1、属性加值 2、属性减值
                        float _buffvalue = effectdata->getCfgdata()->getBuffValue()/RATE_PERCENT*sign*0.01;  //BUFF值
                        for (GameObj* targetobj : *targetObjects ) {
                            targetdata = (LQAttackCalculator*)targetobj->getCalculator();
                            float _bufftime = effectdata->calculatorBuffTime(m_herodata, (effectdata->getCfgdata()->getTFType()==5) ?targetdata:NULL , mainAttr); //NewCL 4.6
                            
                            LQBuffUserData* newbuf = new LQBuffUserData(this, effectdata, _bufftime, _buffvalue);
                            targetobj->addBuffer( newbuf );
                        }
                    }
                        break;
                    case 7:  //7、禁止移动
                    case 8:  //8、禁止暴击
                    case 9:  //9、禁止技能
                    case 10:  //10、无视防御
                    case 11:  //11、无视攻击
                    case 12:  //12、禁锢
                    {
                        
                        LQAttackCalculator* targetdata;
                        for (GameObj* targetobj : *targetObjects ) {
                            targetdata = (LQAttackCalculator*)targetobj->getCalculator();
                            float _bufftime = effectdata->calculatorBuffTime(m_herodata, (effectdata->getCfgdata()->getTFType()==5) ?targetdata:NULL , mainAttr); //NewCL 4.6
                            float _buffrate = effectdata->calculatorPropValue(m_herodata, targetdata, mainAttr, 1);
                            int r = randomInt(1, 100, false);
                            if(_DEBUG_TYPE>=_DEBUG_TYPE_VALUE && _DEBUG_TYPE<=_DEBUG_TYPE_SKILL )
                                CCLOG("%s Add BuffUserData targetobj [%s] is (%s) %d _bufftime:%.3f", this->getName().c_str(),  targetobj->getName().c_str(), r < _buffrate ? "True":"False" , r, _bufftime);
                            if ( r < _buffrate) {
                                LQBuffUserData* newbuf = new LQBuffUserData(this, effectdata, _bufftime, 0);
                                targetobj->addBuffer( newbuf );
                            }
                        }
                    }
                        break;
                    case 13:  //13、护盾
                    {
                        effectdata->calculatorPropValue(this,targetObjects, m_herodata, mainAttr, 1);
                        for (GameObj* targetobj : *targetObjects ) {
                            LQCombatSquad* squad;
                            if (targetobj->getType() == klqLQGameObjSquad) {
                                squad = (LQCombatSquad*)targetobj;
                            }else{
                                LQCombatSoldier* soldier = (LQCombatSoldier*)targetobj;
                                squad = soldier->getSquad();
                            }
                            if(squad)
                                squad->getBannerman()->getArmFlags()->updateValue();
                        }
                    }
                        break;
                    case 14:  //14、白刃
                    {
                        effectdata->calculatorPropValue(this,targetObjects, m_herodata, mainAttr, 1);
                    }
                        break;
                    case 17:  //17、嘲讽
                    {
                        LQAttackCalculator* targetdata;
                        for (GameObj* targetobj : *targetObjects ) {
                            targetdata = (LQAttackCalculator*)targetobj->getCalculator();
                            float _bufftime = effectdata->calculatorBuffTime(m_herodata, (effectdata->getCfgdata()->getTFType()==5) ?targetdata:NULL , mainAttr);
                            LQBuffUserData* newbuf = new LQBuffUserData(this, effectdata, _bufftime, 0);
                            targetobj->addBuffer( newbuf );
                            //squad->taunt(this);
                        }
                    }
                        break;
                    default:
                        break;
                }
            }
                break;
            case 3:  //3、清空DeBuff和Dot
            {
                for (GameObj* targetobj : *targetObjects ) {
                    targetobj->cleareBuffer();
                }
            }
                break;
            case 4:  //4、策反
            {
            }
                break;
            case 5:  //5、援兵？
            {
            }
                break;
            default:
                break;
        }
        
    }
}

//技能的攻击命中特效结束，也是整个技能的结束
void LQCombatHero::skillHitEndCallBack(Ref* pSender)
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("12).a %p CombatHero::skillHitEndCallBack %ld", this, getCurrentMillSecond() );
    LQSkillUserData* _skillData = NULL;
    if (m_skillVector.size()<=0)
        return;
        
    _skillData = m_skillVector.at(0);
    skillHitEnd(_skillData);
    
    LQSkillUserDataVector::iterator iter;
    for (iter = m_skillVector.begin(); iter != m_skillVector.end(); iter++) {
        if (_skillData == (*iter)) {
            m_skillVector.erase(iter);
            m_currentEffect = 0;
            if(m_isDeadhasSkill) {
                squadIsDied();
                this->m_squad->dieUnit(this);
            }
            if(m_skillVector.size()>0){
                _skillData = m_skillVector.at(0);
            }else return;
            break;
        }
    }
    //CCLOG("技能数据从队列中删除错误！");
}

void LQCombatHero::skillHitEnd(LQSkillUserData* skillData)
{
    if (_DEBUG_TYPE == _DEBUG_TYPE_SKILL || _DEBUG_TYPE == _DEBUG_TYPE_SKILL3)
        CCLOG("12) %p CombatHero::skillHitEnd %ld", this, getCurrentMillSecond() );
    
    //动作恢复
    if (m_target && m_target->getActive()) {
        m_phaseType = klqUnitAnimPhaseAttack;
        this->setState( FUSM_STATE_ATTACK ); //攻击
    }else{
        m_phaseType = klqUnitAnimPhaseMove;
        this->setState( FUSM_STATE_APPROACH ); //等待
    }
    m_isSkilling = false;
    changeAnimation();
    
    //删除技能释放突出武将特效
    if(m_herodata->getStuntEffect()>0){
        this->restoreHeroHalo();
    }
    
    if (this->onSkillEndCallback)
        this->onSkillEndCallback(this,skillData);
    
}


//buff触发数据动作
void LQCombatHero::bufferCallback(Ref* pSender)
{
    //LQBuffUserData* bufferData = (LQBuffUserData*)pSender;
    /*
    if (_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
        CCLOG("Buff LQCombatHero::bufferCallback..... %p %ld", bufferData, getCurrentMillSecond());
    switch ( bufferData->getCfgdata()->getBuffSubType() ) {
        case 1:  //1、属性加值
        case 2:  //2、属性减值
        {
            this->getCalculator()->addCurrStdAtr(bufferData->getOutputType(), bufferData->getBuffValue());
        }
            break;
        case 3:  //3、加速
        case 4:  //4、减速
        {
            float value = bufferData->getBuffValue();
            CCASSERT(abs(value)<10, "error: Add SpeedBuff is big!!!");
            this->getHeroData()->setSpeedBuffAdd(value);
        }
            break;
        case 5:  //5、间隔一定时间加值
        case 6:  //6、间隔一定时间减值
        {
            if(bufferData->getOutputType()==ATTR_HP){
                LQCombatHero* heroUnit = (LQCombatHero*)bufferData->getHero();
                heroUnit->changeTargetDps (bufferData->getTarget(), bufferData->getBuffValue(), klq_flaytype_buff);
            }else
                CCLOG("error: Buff OutputType must is 15!!!");
        }
            break;
        case 7:  //7、禁止移动
        {
            this->stopMove(true);
        }
            break;
        case 8:  //8、禁止暴击
        {
            this->getHeroData()->stopCrit(true);
        }
            break;
        case 9:  //9、禁止技能
        {
            this->stopSkill(true);
        }
            break;
        case 10:  //10、无视防御
        {
            
        }
            break;
        case 11:  //11、无视攻击
        {
            
        }
            break;
        case 12:  //12、禁锢
        {
            this->durance(true);
        }
            break;
    }
     */
}

//buff结束动作
void LQCombatHero::bufferEndCallback(Ref* pSender)
{
    //if (_DEBUG_TYPE==_DEBUG_TYPE_SKILL)
    //    CCLOG("Buff LQCombatHero::bufferEndCallback..... %ld", getCurrentMillSecond());
    /*
    LQBuffUserData* bufferData = (LQBuffUserData*)pSender;
    switch ( bufferData->getCfgdata()->getBuffSubType() ) {
        case 1:  //1、属性加值
        case 2:  //2、属性减值
        {
            this->getCalculator()->addCurrStdAtr(bufferData->getOutputType(), -bufferData->getBuffValue());
        }
            break;
        case 3:  //3、加速
        case 4:  //4、减速
        {
            this->getHeroData()->setSpeedBuffAdd(0);
        }
            break;
        case 5:  //5、间隔一定时间加值
        case 6:  //6、间隔一定时间减值
        {
            //没有
        }
            break;
        case 7:  //7、禁止移动
        {
            this->stopMove(false);
        }
            break;
        case 8:  //8、禁止暴击
        {
            this->getHeroData()->stopCrit(false);
        }
            break;
        case 9:  //9、禁止技能
        {
            this->stopSkill(false);
        }
            break;
        case 10:  //10、无视防御
        {
            
        }
            break;
        case 11:  //11、无视攻击
        {
            
        }
            break;
        case 12:  //12、禁锢
        {
            this->durance(false);
        }
            break;
        case 13:  //13、护盾
        {
            this->getSquadData()->setCurrStdAtr(ATTR_SHIELD,0);
        }
            break;
    }
    this->removeBuffer(bufferData);
     */
}

LQ_NAMESPACE_END
