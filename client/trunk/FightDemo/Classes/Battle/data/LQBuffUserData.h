//
//  LQBuffUserData.h
//  SuperWarriors
//  BUFF效果单元数据
//  Created by lequdev1 on 14-7-15.
//
//

#ifndef __SuperWarriors__LQBuffUserData__
#define __SuperWarriors__LQBuffUserData__

#include "LQAnimationInfoData.h"
#include "LQEffectInfoData.h"
#include "LQBaseUserData.h"
#include "LQDataCache.h"
#include "GameObj.h"
#include "LQEffectUserData.h"

static int Buff_Tag_Id = 10000;

class LQBuffUserData:public LQBaseUserData
{
private:
    bool m_isdot;  //是否DOT
    //技能特效
    LQAnimationInfoData* m_animationCfgData;
    string m_buffericon;
    string m_flagsicon;
    string m_flashicon;
    float m_scalefactor;
    float m_dottime;        //Dot时间
    bool m_isDoEffect;      //处理效果标志，一次性BUF处理完置为true
public:
    LQBuffUserData(GameObj* hero, LQEffectUserData* effect, float bufftime, float buffvalue);
    ~LQBuffUserData();
    // 初始化数据,子类重写
    const char* getAliasName();
    LQEffectInfoData*  getCfgdata();
    // 返回特效配置数据
    LQAnimationInfoData* getAnimationCfgData();
    // 返回Buff图标
    string getBufferIcon();
    // 返回Buff旗帜上的图标
    string getFlagsIcon();
    // 返回FlashID图标
    string getFlashID();
    // 返回Scalefactor
    float getScalefactor();
    
    // 初始化数据,子类重写
    void putData();
    
    CC_SYNTHESIZE_READONLY(int, m_buffType, BuffType );	//1、属性加值 2、属性减值 3、加速 4、减速 5、间隔一定时间加值 6、间隔一定时间减值 7、禁止移动 8、禁止暴击 9、禁止技能 10、无视防御 11、无视攻击 12、禁锢(冰冻等) 13、护盾 15、间隔一定时间加变值 16、间隔一定时间减变值
    
    //是否活动
	CC_SYNTHESIZE(bool, m_active, Active)
    CC_SYNTHESIZE(int, m_tagId, TagId );            //效果实施的ID

    
    CC_SYNTHESIZE(GameObj *, m_hero, Hero );        //释放技能的武将数据
    CC_SYNTHESIZE(GameObj *, m_target, Target );    //目标技能的武将数据
    CC_SYNTHESIZE(int, m_objectType, ObjectType );  //效果实施的对象类型
    
    CC_SYNTHESIZE_READONLY(int, m_dominantAttribute, DominantAttribute );	//技能由何种属性支配，属性分类见分类代码说明，技能的输出判定与对应属性差有关\r\n（12武勇、13智力、14统率）
    CC_SYNTHESIZE_READONLY(KLQAnimationDisplayType, m_animationType, AnimationType );	//特效显示类型
    CC_SYNTHESIZE_READONLY(int, m_animationID, AnimationID );	//特效ID
    CC_SYNTHESIZE_READONLY(Point, m_effectPosition, EffectPosition );	//特效位置
    CC_SYNTHESIZE_READONLY(int, m_buffFlag, BuffFlag );	//
    CC_SYNTHESIZE_READONLY(int, m_priority, Priority );	//决定状态之间的顶替
    
    CC_SYNTHESIZE_READONLY(float, m_bufftime, BuffTime );   //特效BuffTime
    CC_SYNTHESIZE_READONLY(int, m_outputType, OutputType );	//输出数值作用于何种属性，见分类代码说明
    CC_SYNTHESIZE(float, m_dotvalue, DotValue );	//特效DotValue
    
    CC_SYNTHESIZE_READONLY(float, m_buffvalue, BuffValue );	//特效BuffValue
    
    CC_SYNTHESIZE_READONLY(int, m_splashArea, SplashArea );	//溅射范围
    
    CC_SYNTHESIZE(float, m_buffDelayTime, BuffDelayTime);               //buff从开始挂起的时间 用于限制buff结束
    CC_SYNTHESIZE(float, m_buffIntervelDelayTime, BuffIntervelDelayTime);//buff从开始挂起的时间  用于触发瞬时伤害
    void isBreak(); //打断BUFF
    
    // update
    void update(float dt);
    
    //回调事件
    std::function<void(Ref*)> onBufferCallback;
    std::function<void(Ref*)> onBufferEndTime;
    
    void doBufferCallback();
    void doBufferEndTime();
};

typedef vector<LQBuffUserData *> LQBuffUserDataVector;
#endif
