//
// LQCombatSoldier.cpp
// SuperWarriors
//
// Created by chenli on 14-6-17.
// Copyright lequworld.com
//

#include "LQCombatSoldier.h"
#include "LQUnitAIControl.h"

#include "LQBattleSession.h"

#include "arrow/LQArrow.h"
#include "arrow/LQMissile.h"
#include "Component/LQFlyNumber.h"
//#include "Component/LQSkillSprite.h"
#include "AudioManager.h"
//#include "SmokeManager.h"


#define BuffSkillSpriteTagNumber 40010000
//float fps = 0.09


LQ_NAMESPACE

LQCombatSoldier* LQCombatSoldier::create(LQBaseUserData* data,LQCombatSquadDirectionEnum direction)
{
    LQCombatSoldier *pRet = new LQCombatSoldier(data,klqLQGameObjSoldier,direction);
    pRet->setUnitType(Combat_People_Cavalry);
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

LQCombatSoldier::LQCombatSoldier(LQBaseUserData* data,LQGameObjType type,LQCombatSquadDirectionEnum direction) :
    LQCombatUnit(data,type,direction),m_isShoot(false)
{
    m_armdata = (LQArmsUserData*)data;
    m_speed = m_armdata->getSpeed();
    m_patrolSpeed = m_armdata->getPatrolSpeed();
    m_flashID = m_armdata->getFlashID();
    m_buffflashID = "";
    
    shoot_effect_res[0] = Effect_BowShoot_01;
    shoot_effect_res[1] = Effect_BowShoot_02;
    shoot_effect_res[2] = Effect_BowShoot_03;
}

LQCombatSoldier::~LQCombatSoldier()
{
    
}


bool LQCombatSoldier::init()
{
    if ( !LQCombatUnit::init() )
    {
        return false;
    }
    initData();
    return true;
}

#pragma mark - initData
void LQCombatSoldier::initData()
{
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // add your codes here...
}

#pragma mark - methods
void LQCombatSoldier::normalAttackReady()
{
    if (!getTarget()) {
        return;
    }
    LQGameObjType type = getTarget()->getType();
    
    if (type != klqLQGameObjSoldier && type != klqLQGameObjHero) {
        return;
    }
    
    this->shootFlash();
}

//显示刀光
void LQCombatSoldier::shootFlash()
{
    if(m_flashID.length()<=0 && m_buffflashID.length()<=0)
        return;
    string flashid = m_flashID;
    if (m_buffflashID.length()>0)
        flashid = m_buffflashID;
    Sprite* sp = Sprite::createWithSpriteFrameName(flashid);
    if(!sp)
        return;
    sp->setPosition(this->getPosition());
    sp->setRotation(Def_Buff_Rotate[m_direction]);
    this->getParent()->addChild(sp, 1000);
    Point _offset = this->getDirectionOffset()*50;
    float duration = 0.5f;
    FiniteTimeAction* act1 = Sequence::create( DelayTime::create(duration*0.7), FadeOut::create(duration*0.3), NULL );  //末端，渐渐消失
    sp->runAction(Sequence::create( Spawn::create(MoveBy::create(duration, _offset), ScaleTo::create(duration, 1.2), act1, NULL) ,RemoveSelf::create(), NULL));
}

//
Point LQCombatSoldier::getMissileStartPostion()
{
    return this->getPosition()+Point(0,m_imgheight*m_missileResPostion.y);
}
Point LQCombatSoldier::getMissileTargetPostion()
{
    return this->getPosition()+Point(0,m_imgheight/2);
}

//远程射击动作
void LQCombatSoldier::shootAttack()
{
    //return;  //TODO
    if((m_unitType == Combat_People_Archers || m_unitType == Combat_People_CavalryArc) && m_target);
    {
        //log("攻击射箭");
        LQCombatSoldier* target = (LQCombatSoldier*)m_target;
        if (target && target->getActive()) {
            Point frompt = this->getMissileStartPostion();
            Point topt = target->getMissileTargetPostion();
            LQMissile* arrow = LQMissile::create(this->getMissileType(), this->getMissileRes().c_str(), this->getMissileFrameCount() );
            arrow->setPosition(frompt);
            if (!arrow->getParent()) {
                //log("发射弓箭");
                this->getParent()->addChild(arrow,this->getZOrder()-1);
            }
            
            auto callback = CallFuncN::create(CC_CALLBACK_1(LQCombatSoldier::shootEnd,this));
            arrow->shoot(frompt, topt, callback, target->getHitEffect());
        }
        
        //弓箭手攻击
        int m = randomInt(0, 2);
        AudioManager::getInstance()->playEffect(shoot_effect_res[m].c_str());
        return;
    }
}

void LQCombatSoldier::shootEnd(Node * node)
{
    //log("射中了哦");
    LQMissile* missile = (LQMissile *)node;
    if (missile) {
        if (missile->getHitEffect()>0){
            LQBuffSprite* _hitSp = LQBuffSprite::createWithId(missile->getHitEffect());
            _hitSp->setPosition(missile->getToPoint());
            this->getParent()->addChild(_hitSp,4001);
            _hitSp->play1();
        }
        /*LQArmsUserData* userdata_this = (LQArmsUserData*)this->getData();
        LQArmsUserData* userdata_target = (LQArmsUserData*)target->getData();
        int dps =  userdata_this->getCurrentDPS(userdata_target);
        this->attack(target,-dps);*/
    }
}

//攻击准备
//void LQCombatSoldier::attackReady()
//{
//    LQCombatSoldier* target = (LQCombatSoldier *)m_target;
//    if (target) {
//        LQArmsUserData* userdata_this = (LQArmsUserData*)this->getData();
//        LQArmsUserData* userdata_target = (LQArmsUserData*)target->getData();
//        
//        int dps = userdata_this->getCurrentDPS(userdata_target);
//        
//        if (userdata_this->isCrit(userdata_target)) {  //暴击
//            this->attack(target,dps*(-1),klq_flaytype_crit);
//        }else if(userdata_this->isParry(userdata_target)){//格挡
//            this->attack(target,dps*(-1),klq_flaytype_parry);
//        }else{
//            this->attack(target,dps*(-1));
//        }
//    }
//}

//攻击目标,目标的血量变化
//void LQCombatSoldier::changeTargetDps(GameObj* pTarget,int dps,LQFlyNumberTypeEnum numbertype)
//{
//    if(!pTarget->getActive())
//        return;
//    
//    //当攻击对象的类型不是兵或者将，则就不处理DPS 的改变
//    if (pTarget->getType() != klqLQGameObjSoldier && pTarget->getType() != klqLQGameObjHero) {
//        return;
//    }
//    
//    LQCombatSoldier* target = (LQCombatSoldier*)pTarget;
//    
//    //显示伤害
//    if (numbertype != klq_flaytype_normal) {
//        target->showDamage(dps,numbertype);
//        if (target->getUnitType() == Combat_People_Cavalry) {
//            ////AudioManager::getInstance()->playEffect(Effect_Whinny_1);
//        }
//        if (this->getUnitType() == Combat_People_Infantry) {
//            ////AudioManager::getInstance()->playEffect(Effect_HurtManSword_1);
//        }
//    }
//    
//    
//    if(numbertype != klq_flaytype_fklsg){
//        LQArmsUserData* targetdata = (LQArmsUserData*)target->getData();
//        targetdata->changeHpValue(dps);
//        //if (targetdata->getCurrHpValue()<=0)
//        //    ((LQArmsUserData*)m_data)->changeKillTotal(1);  //象兵处理
//        //((LQArmsUserData*)m_data)->changeDpsTotal(dps);
//        //18 m 攻击2  18 n 挨打1
//        //((LQArmsUserData*)m_data)->changeHeroFuries(1*LQBattleSession::getInstance()->getRAGE_ATTACK());
//        //targetdata->changeHeroFuries(1*LQBattleSession::getInstance()->getRAGE_DEFEND());
//    }
//}
//
////显示伤害
//void LQCombatSoldier::showDamage(int hp,LQFlyNumberTypeEnum numbertype)
//{
//    if(hp==0)
//        return;
//    
//    if(numbertype == klq_flaytype_normal)
//        return;
//    
//    Point headPosition = Point(getPositionX() , getPositionY());
//    int _hp = hp/100.00f;
//    LQFlyNumber* pHp = LQFlyNumber::createFlyWithFile("", _hp, numbertype, true);
//    pHp->setPosition(headPosition);
//    this->getParent()->addChild(pHp,this->getZOrder() + 100);
//    pHp->startAnimation();
//}
//
////显示暴击
//void LQCombatSoldier::showCrit()
//{
//    LQFlyNumberTypeEnum numbertype = klq_flaytype_crit; //klq_flaytype_normal
//    Point headPosition = Point(getPositionX() , getPositionY());
//    
//    LQFlyNumber* pHp = LQFlyNumber::createFlyWithFile("icon3010.png", 0, numbertype, true);
//    pHp->setScale(0.5);
//    pHp->setPosition(headPosition);
//    this->getParent()->addChild(pHp,this->getZOrder() + 1);
//    
//    pHp->startAnimation();
//}
//
////显示闪避
//void LQCombatSoldier::showMiss()
//{
//    
//}
////显示格挡
//void LQCombatSoldier::showParry()
//{
//    
//}

//显示自身位置的特效（刀光，闪避，格挡，加速等）
void LQCombatSoldier::showEffect(LQShowEffectTypeEnum eftype)
{
    // 冲锋管理器
    //if (this->getType()==klqLQGameObjHero && isshow)
    //    CCLOG("showEffect");
    LQArmsTypeEnum _armsType = m_armsType;
    
    if(eftype!=Combat_ShowEffect_None && eftype!=Combat_ShowEffect_Attack){
        if (_armsType==Combat_ArmsType_None && m_hero->getArmsType()==Combat_ArmsType_Melee)
            _armsType = m_hero->getArmsType();
        
        if(_armsType!=Combat_ArmsType_Cavalry && _armsType!=Combat_ArmsType_Melee)
            return;
        if (this->getType()==klqLQGameObjHero){
            if (_armsType==Combat_ArmsType_Cavalry)  //骑兵冲锋气流声 4.9
                AudioManager::getInstance()->playEffect(Effect_QBRun_Music);
        }
        //骑兵显示冲锋效果
        if (this->getUnitX()<1 || this->getType()==klqLQGameObjHero)
        {
            float scale = 1;
            if (this->getType()==klqLQGameObjHero)
                scale = 1.2;
            else
                scale = 1.0;  //0.5
            LQUnitDirectionType _direction = m_direction;
            if (m_squad->getHero())
                _direction = m_squad->getHero()->getDirection();
            if (m_forward->getForwardSprite())
                m_forward->changeDirection(_direction);
            else m_forward->init(_armsType, _direction, scale);
            m_forward->setVisible(true);
        }
    }else if((eftype==Combat_ShowEffect_None || eftype==Combat_ShowEffect_Attack) && m_forward)
        m_forward->setVisible(false);
}

void LQCombatSoldier::showSmoke(int effectID, Point posi)
{
    if (effectID <= 0 || !this->getActive()) {
        return;
    }
    LQBuffSprite* smokeSprite = (LQBuffSprite*)this->getChildByTag(SmokeSpriteTagNumber);
    if (smokeSprite ) {
        smokeSprite->setVisible(true);
        return;
    }
    
    LQAnimationInfoData* m_animationData = (LQAnimationInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAnimationInfoData::getAliasName(), effectID);
    smokeSprite = LQBuffSprite::createWithData(m_animationData, 1,KkqBuffShowType_Smoke);
    
    if (this->getType() == klqLQGameObjHero) {
        smokeSprite->setScale(1.2);
    }else{
        smokeSprite->setScale(0.8);
    }

    smokeSprite->setAnchorPoint(Point(1, 0.5));//skillSprite->getSkillAnchPosition()
    smokeSprite->setPosition(Point(m_imgwidth*0,  m_imgheight * 0.1));
    
    smokeSprite->setTag(SmokeSpriteTagNumber);
    if (smokeSprite->getParent()) {
        smokeSprite->retain();
        smokeSprite->removeFromParentAndCleanup(false);
        this->addChild(smokeSprite, - 1);
        smokeSprite->release();
    }else{
        this->addChild(smokeSprite, - 1);
    }
    smokeSprite->play();
    smokeSprite->setDirction(this->getDirection());
}

//添加头顶BUFF图标
void LQCombatSoldier::addBufferIcon(LQBuffUserData* bufferData)
{
    if(bufferData->getBufferIcon().length()<=0)
        return;
    if(m_buff_ico>0) this->removeChildByTag(m_buff_ico+10000);
    Sprite* sp = Sprite::createWithSpriteFrameName(bufferData->getBufferIcon().c_str());
    sp->setPosition(Point(m_imgwidth*0.5, m_imgheight*0.9));
    sp->setAnchorPoint(Point(0.5,0));
    sp->setTag(bufferData->getTagId()+10000);
    m_buff_ico = bufferData->getTagId();
    this->addChild( sp,100 );
}

//添加身上的BUFF图标，带动作
void LQCombatSoldier::addBufferBmp(LQBuffUserData* bufferData)
{
    if(bufferData->getBufferIcon().length()<=0)
        return;
    if(m_buff_ico>0) this->removeChildByTag(m_buff_ico+10000);
    Sprite* sp = Sprite::createWithSpriteFrameName(bufferData->getBufferIcon().c_str());
    sp->setPosition(Point(m_imgwidth*0.5, m_imgheight*0.5));
    sp->setAnchorPoint(Point(0.5,0.5));
    sp->setTag(TAG_BUFFER_ID);
    if(bufferData->getCfgdata()->getAnimationDef()>1){
        sp->setScale(2.2);
        auto seq1 = Sequence::create(ScaleTo::create(0.5, 2.4), ScaleTo::create(0.5, 2.4), NULL);
        auto seq2 = Sequence::create(RotateBy::create(1.0, 360), NULL);
        auto seq3 = Spawn::create(seq1, seq2, NULL);
        sp->runAction(RepeatForever::create(seq3));
    }
    m_buff_ico = TAG_BUFFER_ID;
    this->addChild( sp,100 );
}

//移除BUFF图标
void LQCombatSoldier::removeBufferIcon(LQBuffUserData* bufferData)
{
    m_buff_ico = -1;
    this->removeChildByTag(bufferData->getTagId()+10000);
}

//显示BUFF Animation
void LQCombatSoldier::addBufferAnimation(LQAnimationInfoData* animaData,int tag)
{
    /* 5.31 改为支持5个
    bool isfoot = animaData->getHangingPoint()==4;
    
    if ((!isfoot && m_buff_body_tag == tag) || (isfoot && m_buff_foot_tag == tag))
        return;
    
    if(!isfoot && m_buff_body_tag>0)
        this->removeBufferAnimation( m_buff_body_tag );
    else if(isfoot && m_buff_foot_tag>0)
        this->removeBufferAnimation( m_buff_foot_tag );*/
    
    for (int m_buff_tag : m_buff_tags)  //已经存在就直接返回
        if (m_buff_tag== tag)
            return;
    //如果满了就要清除第一个
    if (m_buff_tags.size()==DEF_BUFF_TAGCOUNT){
        this->removeBufferAnimation( m_buff_tags.at(0) );
    }
    
    LQBuffSprite* sp = LQBuffSprite::createWithData(animaData);
    sp->setTag(tag);
    
    //if(isfoot) m_buff_foot_tag = tag;
    //else m_buff_body_tag = tag;
    m_buff_tags.push_back(tag);
    
    if(_DEBUG_TYPE==_DEBUG_TYPE_SKILL){
        CCLOG("%p addBufferAnimation m_buff_tag:%d ResID:%d", this, tag, animaData->getResID());
    }
    Point _effectPosition = PointFromString(animaData->getPosition());
    sp->setAnchorPoint(_effectPosition);
    //上 1 中 2 下 3 4 地
    Point desPoint = Point::ZERO;
    if (animaData->getHangingPoint()==1)
        desPoint = Point(0, m_imgheight);
    else if (animaData->getHangingPoint()==2)
        desPoint = Point(0, m_imgheight*0.5 );
    sp->setPosition(desPoint);
    this->addChild(sp,100);
    if (animaData->getHangingPoint()==4) //脚底下踩着
    {
        sp->setPosition(Point(0,-m_imgwidth*0.45));
        sp->setLocalZOrder(-100);
    }
    sp->play();
}


//添加BUFF 40003002
void LQCombatSoldier::addBuffer(LQBuffUserData* bufferData)
{
    //添加bufferData
    if(m_type == klqLQGameObjHero){
        //
        bufferData->setTarget(this);
        
        ((LQAttackCalculator*)this->getCalculator())->addBufferData(bufferData);
        //英雄飘Buff
        if (bufferData->getAnimationType()!=klqAnimationDisplayTypeNone)
            m_armflags->showBufferName(bufferData->getCfgdata()->getEffectText());
        if( bufferData->getAnimationType()==klqAnimationDisplayTypeIcon)  //图标
        {
            //英雄自己顶图标
            this->addBufferIcon(bufferData);
        }else if( bufferData->getAnimationType()==klqAnimationDisplayTypeBmp)  //图片
        {
            //英雄自己顶图标
            this->addBufferBmp(bufferData);
        }
    }
    //添加Buff特效
    if( bufferData->getAnimationType()==klqAnimationDisplayTypeAnimation )  //动画
    {
       this->addBufferAnimation(bufferData->getAnimationCfgData(), bufferData->getTagId());
    }
}

//删除BUFF
void LQCombatSoldier::removeBuffer(LQBuffUserData* bufferData)
{
    if (!bufferData)
        return;
    
    //添加Buff特效
    if( bufferData->getAnimationType()==klqAnimationDisplayTypeAnimation)  //动画
        this->removeBufferAnimation(bufferData->getTagId());
    else if( bufferData->getAnimationType()==klqAnimationDisplayTypeIcon)  //图标
        this->removeBufferIcon(bufferData);
    
    if(m_type == klqLQGameObjHero){
        //删除bufferData
        ((LQAttackCalculator*)this->getCalculator())->removeBufferData(bufferData);
        
        if( bufferData->getAnimationType()==klqAnimationDisplayTypeIcon)  //图标
            this->removeBufferIcon(bufferData);
        else if( bufferData->getAnimationType()==klqAnimationDisplayTypeBmp)  //图片
            this->removeChildByTag(TAG_BUFFER_ID);
    }
}

//清除所有BUFF
void LQCombatSoldier::cleareBuffer()
{
    if(m_type == klqLQGameObjHero){
        ((LQAttackCalculator*)this->getCalculator())->cleareBuffer();
    }
}

#pragma mark -
void LQCombatSoldier::setDirection(LQUnitDirectionType direction)
{
    if(m_direction==direction && m_direction!=klqUnitDirectionTypeNone){
        m_directionOffset = getPointOffsetFromDirection(direction);
        return;
    }
    
    LQCombatUnit::setDirection(direction);
    
    LQBuffSprite* smokeSp = (LQBuffSprite*)this->getChildByTag(SmokeSpriteTagNumber);
    if (smokeSp)
        smokeSp->setDirction(direction);
    
    LQBuffSprite* forwardSp = (LQBuffSprite*)this->getChildByTag(ForwardSpriteTagNumber);
    if (forwardSp)
        forwardSp->setDirction(direction);
}

void LQCombatSoldier::update(float dt, Layer *layer)
{
    LQCombatUnit::update(dt, layer);
    
    if (m_phaseType == klqUnitAnimPhaseAttack ) {  //&& m_type != klqLQGameObjHero
        getSquad()->setIsAttacking(true);
    }
    
    if (m_armdata) {
        /*
        LQEffectUserDataVector* buffVector = m_armdata->getBuffVector();
        
        for (int i = 0; i < buffVector->size(); i++) {
            LQEffectUserData* buffData = (LQEffectUserData*)buffVector->at(i);
            buffData->setBuffDelayTime(buffData->getBuffDelayTime() + dt);
            buffData->setBuffIntervelDelayTime(buffData->getBuffIntervelDelayTime() + dt);
            
            //用于移除buff
            if (buffData->getBuffDelayTime() >= buffData->getBuffTime()) {
                removeBuffer(buffData);
            }
            
            //用于触发buff瞬时伤害
            if (buffData->getBuffIntervel() > 0) {//表示buff有瞬时输出
                if (buffData->getBuffIntervelDelayTime() >= buffData->getBuffIntervel()) {
                    buffData->setBuffIntervelDelayTime(0);
                    changeBuffValue(buffData);
                }
            }
        }*/
    }

}

void LQCombatSoldier::changeAnimation()
{
    if(!getSprite() || m_stopMove)  //|| !m_active
        return;
    
    if(m_index==0 && m_phaseType==klqUnitAnimPhaseAttack){
        //CCLOG("is dead changeAnimation %p: m_phaseType: %d %d", this , m_phaseType, m_isSkilling);
    }
    //if(m_isSkilling && m_phaseType!=klqUnitAnimPhaseSkillAttack)
    //    CCLOG("is dead changeAnimation ERROR..... %p %d ", this, m_phaseType);
    
    m_isSkilling = m_phaseType==klqUnitAnimPhaseSkillAttack;
    
    if (m_isSkilling && m_type!=klqLQGameObjHero)
        CCLOG("is dead changeAnimation ERROR..... %p %d ", this, m_phaseType);
    
    testHeadLabel();
	
    float _delayPerUnit = AnimationFPS;  //
    string file = createAnimationDirectionName(createAnimationPhaseName(m_spriteBaseName.c_str(), m_phaseType), m_direction);
    _LQDEBUG("changeAnimation:%s", file.c_str());
    
    Animation *animation = AnimationCache::getInstance()->getAnimation(file);
    if(animation && animation==_currentAnimation){
        getSprite()->setFlippedX((m_direction==klqUnitDirectionTypeRight || m_direction==klqUnitDirectionTypeRightDown|| m_direction==klqUnitDirectionTypeRightUp));
        return;
    }
    
	int n = m_armdata->getCurrAnimationPhaseCount(m_phaseType);
    getSprite()->stopAllActions();
	getSprite()->setFlippedX(false);
    if(!animation){
		animation = Animation::create();
		string frameName;
        int maxPhase = m_armdata->getCurrAnimationCount(m_phaseType);
		for(int i = 0; i < maxPhase; ++i) {
			frameName = this->getSpriteFrameName(m_spriteBaseName.c_str(), m_phaseType, m_direction, i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str());
			if(frame)
				animation->addSpriteFrame(frame);
			else
				CCLOG("frame:%s is NULL", frameName.c_str());
		}
		AnimationCache::getInstance()->addAnimation(animation, file);
		//CCLOG("Animation:%s", file.c_str());
	}
    // delayPerUnit means the delay to add to each frame (exactly the same as the old delay)
    
    if (m_phaseType == klqUnitAnimPhaseAttack)
        _delayPerUnit = m_currentAttackSpeed;
    else if (m_phaseType == klqUnitAnimPhaseStand )
        _delayPerUnit *= 2.0;
    
    animation->setDelayPerUnit(_delayPerUnit);
    _currentAnimation = animation;
    
    Action* action = NULL;
    if ( m_phaseType == klqUnitAnimPhaseAttack) {//攻击动作处理
        //LQArmsUserData* data = (LQArmsUserData*)this->getData();
        int readyIndex = m_armdata->getMelATKR();
        
        switch (m_type) {
            case klqLQGameObjSoldier :
            {
                ActionInterval* action1;
                switch (m_unitType) {
                    case Combat_People_Infantry:// 剑兵
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            ////AudioManager::getInstance()->playEffect(Effect_Swcombi_1);
                            this->normalAttackReady();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)), NULL), NULL);
                        action = RepeatForever::create( Sequence::create(action1, DelayTime::create(m_currentAttackGap), NULL) );
                    }
                    break;
                    case Combat_People_Spearman:// 枪兵
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            ////AudioManager::getInstance()->playEffect(Effect_HurtSpear_1);
                            this->normalAttackReady();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)), NULL), NULL);
                        action = RepeatForever::create( Sequence::create(action1, DelayTime::create(m_currentAttackGap), NULL) );
                    }
                    break;
                    case Combat_People_Cavalry:// 骑兵
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            ////AudioManager::getInstance()->playEffect(Effect_Swcombi_1);
                            this->normalAttackReady();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)), NULL), NULL);
                        action = RepeatForever::create( Sequence::create(action1, DelayTime::create(m_currentAttackGap), NULL) );
                    }
                    break;
                    case Combat_People_Halberdier:// 戟兵
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            ////AudioManager::getInstance()->playEffect(Effect_HurtSpear_1);
                            this->normalAttackReady();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)), NULL), NULL);
                        action = RepeatForever::create( Sequence::create(action1, DelayTime::create(m_currentAttackGap), NULL) );
                    }
                    break;
                    case Combat_People_Archers:// 弩兵
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            this->shootAttack();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)), NULL), NULL);
                        action = RepeatForever::create( Sequence::create(action1, DelayTime::create(m_currentAttackGap), NULL) );
                    }
                    break;
                    case Combat_People_Elephants:// 象兵
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            ////AudioManager::getInstance()->playEffect(Effect_Swcombi_1);
                            this->normalAttackReady();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)), NULL), NULL);
                        action = RepeatForever::create( Sequence::create(action1, DelayTime::create(m_currentAttackGap), NULL) );
                    }
                    break;
                    case Combat_People_Rattan:// 藤甲
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            ////AudioManager::getInstance()->playEffect(Effect_Swcombi_1);
                            this->normalAttackReady();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)), NULL), NULL);
                        action = RepeatForever::create( Sequence::create(action1, DelayTime::create(m_currentAttackGap), NULL) );
                    }
                    break;
                    case Combat_People_Women:// 女兵
                    {
                        action = RepeatForever::create(Animate::create(animation));

                    }
                    break;
                    default:
                    {
                        action = RepeatForever::create(Animate::create(animation));
                    }
                    break;
                        
                }
                break;
            }
            case klqLQGameObjHero:
            {
                ActionInterval* action1;
                switch (m_unitType) {
                    case Combat_People_Archers:// 弩兵
                    case Combat_People_CavalryArc:// 弩兵
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            ////AudioManager::getInstance()->playEffect(Effect_Swcombi_1);
                            this->normalAttackReady();
                            this->shootAttack();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)),CallFunc::create([this](){
                            this->normalAttackEnd();
                        }), NULL), NULL);
                        action = RepeatForever::create( action1 );
                    }
                        break;
                    default:
                    {
                        action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
                            ////AudioManager::getInstance()->playEffect(Effect_Swcombi_1);
                            this->normalAttackReady();
                        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)),CallFunc::create([this](){
                            this->normalAttackEnd();
                        }), NULL), NULL);
                        action = RepeatForever::create( action1 );
                     }
                        break;
                }
            }
            break;
            default:
            {
                action = RepeatForever::create(Animate::create(animation));
            }
            break;
        }
        
    }
    else if((m_phaseType == klqUnitAnimPhaseSkillAttack && m_type == klqLQGameObjHero))
    {
        int readyIndex = m_armdata->getStATKR();
        ActionInterval* action1 = Spawn::create(Animate::create(animation),Sequence::create(DelayTime::create(m_currentAttackSpeed * readyIndex),CallFunc::create([this](){
            ////AudioManager::getInstance()->playEffect(Effect_Swcombi_1);
            this->skillAttack();
        }),DelayTime::create(m_currentAttackSpeed *( n - readyIndex)), NULL), NULL);
        action = RepeatForever::create( Sequence::create(action1, DelayTime::create(m_currentAttackGap), NULL) );
    }
    else if(m_phaseType == klqUnitAnimPhaseFallDown)
    {
        action = RepeatForever::create(Animate::create(animation));
    }
    else
    {
        action = RepeatForever::create(Animate::create(animation));
    }
    
    action->setTag(SPRITE_SPEDD_ACTION_TAG);
    getSprite()->runAction(action);
    if(this->getType()==klqLQGameObjHero)
    {
        _LQDEBUG("changeAnimation 2: %s", file.c_str());
    }
    if(m_direction==klqUnitDirectionTypeRight || m_direction==klqUnitDirectionTypeRightDown|| m_direction==klqUnitDirectionTypeRightUp)
		getSprite()->setFlippedX(true);
}

LQ_NAMESPACE_END
