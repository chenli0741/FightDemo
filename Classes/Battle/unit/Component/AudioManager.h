//
//  AudioManager.h
//  game
//
//  Created by lequdev1 on 14-7-22.
//
//

#ifndef __game__AudioManager__
#define __game__AudioManager__

#include <iostream>
#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
#include "LQStringExtend.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace CocosDenshion;

//#define Effect_ArmyCharge_1     "res/audio/armycharge2.mp3"  //喊杀声
//#define Effect_ArrowStab_1      "res/audio/arrowstab_01.mp3"  //箭只命中
//#define Effect_BowShoot_1       "res/audio/arrowstab_01.mp3"  //弩兵攻击
//#define Effect_Fire_1           "res/audio/fireloop1.mp3"   //火焰音效
//#define Effect_Die_1            "res/audio/hit_04.mp3"      //死亡
//#define Effect_HorseDie_1       "res/audio/horsedie_01.mp3"  //骑兵死亡
//#define Effect_HurtPike_1       "res/audio/pikebasic_01.mp3"  //戟兵 枪兵攻击
//#define Effect_HurtSpear_1      "res/audio/spearbasic_01.mp3"  //剑兵，刀盾兵攻击

//#define Effect_DieSpear_1       "res/audio/speardth_01.mp3"  //收到突刺攻击（骑兵、戟兵、枪兵）后死亡
//#define Effect_Swcombi_1        "res/audio/swcombi1.mp3"  //刀剑碰撞，用于战斗中
//#define Effect_DieSwdth_1       "res/audio/swdth_07.mp3"  //被刀剑砍&死亡音效

#define Effect_Swcombi_1        "res/audio/melefight1.mp3"  //刀剑碰撞，用于战斗中

//#define Effect_Whinny_1         "res/audio/whinny_m_01.mp3"  //马的音效，可用于骑兵
//#define Effect_HurtManSword_1   "res/audio/B_Man_sword_Atk2.mp3"  //可用于打击音效
//
//#define Effect_BowShoot_N       "res/audio/fse_ar%d.mp3"    //弩兵攻击1-3

#define Effect_BG1_Music        "res/audio/bse_01.mp3"  //背景1
#define Effect_BG3_Music        "res/audio/bse_03.mp3"  //背景3
#define Effect_BG4_Music        "res/audio/bse_04.mp3"  //背景4

#define Effect_BowShoot_01       "res/audio/fse_ar1.mp3"
#define Effect_BowShoot_02       "res/audio/fse_ar2.mp3"
#define Effect_BowShoot_03       "res/audio/fse_ar3.mp3"

#define Effect_MoveQB_Music1     "res/audio/xj_cav.mp3"  //骑兵行军
#define Effect_MoveBB_Music2     "res/audio/xj_inf.mp3"  //步兵行军

#define Effect_Skill_Music1     "res/audio/skill_01.mp3"  //技能释放
#define Effect_Skill_Music2     "res/audio/skill_02.mp3"  //技能释放

#define Effect_Win_Music1       "res/audio/win.mp3"
#define Effect_Lost_Music1      "res/audio/lose.mp3"

#define Effect_CritTJ_Music     "res/audio/cs_tj.mp3"  //突击
#define Effect_CritQJ_Music     "res/audio/cs_qj.mp3"  //枪击
#define Effect_CritLS_Music     "res/audio/cs_ls.mp3"  //乱射

#define Effect_FSEKILL_Music    "res/audio/fse_kill.mp3" //溃败

#define Effect_Death_Music1    "res/audio/hit_ar1.mp3" //死亡1
#define Effect_Death_Music2    "res/audio/hit_ar_death.mp3" //死亡1
#define Effect_Death_Music3    "res/audio/hit_mele_death.mp3" //死亡1
#define Effect_Death_Music4    "res/audio/death_1.mp3" //死亡1
#define Effect_Death_Music5    "res/audio/death_2.mp3" //死亡1

#define Effect_QBRun_Music     "res/audio/spse_cavcharge.mp3" //骑兵冲锋


#define Effect_GUANYU_Music     "res/audio/voice_guanyu.mp3"
#define Effect_DIAOCHAN_Music   "res/audio/voice_diaochan.mp3"

#define FightBackMusic          "res/audio/fightmusic.mp3"
#define MainBackMusic           "res/audio/mainmusic.mp3"

#define Effect_FightWinMusic1   "res/sound/se_star.mp3" //战斗胜利星星音效

class AudioManager
{
private:
    AudioManager();
public:
    ~AudioManager();
    static AudioManager* getInstance();
    static void clear();
    
    void start();
    
    void addAudio();
    void unloadAudio();
    void preloadEffect(string filepath);
    unsigned int playEffect(string filename, bool loop, int time = 150);
    unsigned int playEffect(string filename, int time = 150);
    void stopEffect(unsigned int nSoundId);
    void pauseEffect(unsigned int nSoundId);
    void resumeEffect(unsigned int nSoundId);
    void updataEffectCount(float dt);
    float m_delayTime;
    ValueMap m_soundMap;
    
private:
    map<string, timeval> m_soundCount;
};
#endif /* defined(__game__AudioManager__) */
