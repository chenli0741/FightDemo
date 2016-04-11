#ifndef __SuperWarriors__LQEventSkillEffect__
#define __SuperWarriors__LQEventSkillEffect__

#include "CCEvent.h"
#include "CCEventCustom.h"
#include "LQCombatUnit.h"

USING_NS_CC;
using namespace std;

LQ_NAMESPACE

class LQEventSkillEffect : public EventCustom
{
public:
    static const std::string SKILL_EFFECT_EVENT_TYPE;
    
    LQEventSkillEffect(LQCombatUnit* skillhero, LQCombatUnit* target);
    ~LQEventSkillEffect();
    
    LQCombatUnit* getHero(){ return _skillhero; }
    LQCombatUnit* getTarget(){ return _target; }
private:
    LQCombatUnit* _skillhero;
    LQCombatUnit* _target;
};

class LQSkillEffectListener : public EventListener
{
public:
    /** Creates an event listener with type and callback.
     *  @param eventType The type of the event.
     *  @param callback The callback function when the specified event was emitted.
     */
    static LQSkillEffectListener* create(const std::string& eventName, const std::function<void(LQEventSkillEffect*)>& callback);
    
    /// Overrides
    virtual bool checkAvailable() override;
    virtual LQSkillEffectListener* clone() override;
    
CC_CONSTRUCTOR_ACCESS:
    /** Constructor */
    LQSkillEffectListener();
    ~LQSkillEffectListener();
    
    /** Initializes event with type and callback function */
    bool init(const ListenerID& listenerId, const std::function<void(LQEventSkillEffect*)>& callback);
    
protected:
    std::function<void(LQEventSkillEffect*)> _onCustomEvent;
    
    friend class EventDispatcher;
};

LQ_NAMESPACE_END

#endif /* defined(__SuperWarriors__LQEventSkillEffect__) */
