
#include "LQSkillEffectListener.h"

LQ_NAMESPACE

//======================SkillEffectEvent=======================
const std::string LQEventSkillEffect::SKILL_EFFECT_EVENT_TYPE = "__skill_effect_event___";

LQEventSkillEffect::LQEventSkillEffect(LQCombatUnit* skillhero, LQCombatUnit* target)
: EventCustom(SKILL_EFFECT_EVENT_TYPE)
{
    _skillhero = skillhero;
    _target = target;
}

LQEventSkillEffect::~LQEventSkillEffect()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQEventSkillEffect::~LQEventSkillEffect %p", this);
}

//======================SkillEffectListener=======================

LQSkillEffectListener::LQSkillEffectListener()
: _onCustomEvent(nullptr)
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQSkillEffectListener::LQSkillEffectListener %p", this);
}

LQSkillEffectListener::~LQSkillEffectListener()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LQSkillEffectListener::~LQSkillEffectListener %p", this);    
}

LQSkillEffectListener* LQSkillEffectListener::create(const std::string& eventName, const std::function<void(LQEventSkillEffect*)>& callback)
{
    LQSkillEffectListener* ret = new LQSkillEffectListener();
    if (ret && ret->init(eventName, callback))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LQSkillEffectListener::init(const ListenerID& listenerId, const std::function<void(LQEventSkillEffect*)>& callback)
{
    bool ret = false;
    
    _onCustomEvent = callback;
    
    auto listener = [this](Event* event){
        if (_onCustomEvent != nullptr)
        {
            _onCustomEvent(static_cast<LQEventSkillEffect*>(event));
        }
    };
    
    if (EventListener::init(EventListener::Type::CUSTOM, listenerId, listener))
    {
        ret = true;
    }
    return ret;
}

LQSkillEffectListener* LQSkillEffectListener::clone()
{
    LQSkillEffectListener* ret = new LQSkillEffectListener();
    if (ret && ret->init(_listenerID, _onCustomEvent))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LQSkillEffectListener::checkAvailable()
{
    bool ret = false;
    if (EventListener::checkAvailable() && _onCustomEvent != nullptr)
    {
        ret = true;
    }
    return ret;
}


LQ_NAMESPACE_END