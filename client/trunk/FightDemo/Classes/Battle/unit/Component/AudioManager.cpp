//
//  AudioManager.cpp
//  game
//
//  Created by lequdev1 on 14-7-22.
//
//

#include "AudioManager.h"
#include "LQCore.h"

string getFullFilePath(string filename)
{
   return FileUtils::getInstance()->fullPathForFilename(filename);
}

static AudioManager* m_audioInstance = NULL;
AudioManager::AudioManager():m_delayTime(0)
{
}

AudioManager::~AudioManager()
{
    unloadAudio();
}

AudioManager* AudioManager::getInstance()
{
    if (!m_audioInstance) {
        m_audioInstance = new AudioManager();
    }
    
    return m_audioInstance;
}

void AudioManager::clear()
{
    CC_SAFE_DELETE(m_audioInstance);
}

void AudioManager::addAudio()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return;
#endif
    
    //SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_GUANYU_Music).c_str());
    //SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_DIAOCHAN_Music).c_str());
    //return;
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_ArmyCharge_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_ArrowStab_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_BowShoot_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Fire_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Die_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_HorseDie_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_HurtPike_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_HurtSpear_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_DieSpear_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Swcombi_1).c_str());
//    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_DieSwdth_1).c_str());
/*
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Whinny_1).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_HurtManSword_1).c_str());
    
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_BG1_Music).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_BG3_Music).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_BG4_Music).c_str());
    
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_BowShoot_01).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_BowShoot_02).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_BowShoot_03).c_str());
    
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(FightBackMusic).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(MainBackMusic).c_str());
    
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_MoveQB_Music1).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_MoveBB_Music2).c_str());
    
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Skill_Music1).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Skill_Music2).c_str());
  
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_CritTJ_Music).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_CritQJ_Music).c_str());    
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_CritLS_Music).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_FSEKILL_Music).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_QBRun_Music).c_str());
    
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Death_Music1).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Death_Music2).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Death_Music3).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Death_Music4).c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(getFullFilePath(Effect_Death_Music5).c_str());
 */
}

void AudioManager::unloadAudio()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return;
#endif
    /*
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_Whinny_1).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_HurtManSword_1).c_str());
    
    
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_BG1_Music).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_BG3_Music).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_BG4_Music).c_str());
    
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_BowShoot_01).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_BowShoot_02).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_BowShoot_03).c_str());
    
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(FightBackMusic).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(MainBackMusic).c_str());
    
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_MoveQB_Music1).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_MoveBB_Music2).c_str());
    
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_Skill_Music1).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_Skill_Music2).c_str());
    
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_CritTJ_Music).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_CritQJ_Music).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_CritLS_Music).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_FSEKILL_Music).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_QBRun_Music).c_str());
    
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_Death_Music1).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_Death_Music2).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_Death_Music3).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_Death_Music4).c_str());
    SimpleAudioEngine::getInstance()->unloadEffect(getFullFilePath(Effect_Death_Music5).c_str());
     */
}

void AudioManager::start()
{
    m_soundCount.clear();
}

unsigned int AudioManager::playEffect(string filename, int time)
{
    return playEffect(filename, false, time);
}

unsigned int AudioManager::playEffect(string filename, bool loop, int time)
{
    bool isplay = UserDefault::getInstance()->getIntegerForKey("UD_SOUND_KEY");
    if(1 == isplay)
        return 0;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    timeval t2 = LQ::getCurrentTime();
    timeval t1 = m_soundCount[filename];
    int t0 = (t2.tv_sec - t1.tv_sec)*1000 + (t2.tv_usec - t1.tv_usec)*0.001;
    if(t1.tv_sec>0 && t0<time){
       //if (filename=="res/audio/cs_tj.mp3")
       CCLOG( "   MISS %s\t%d\t%ld - %ld", filename.c_str(), t0, t1.tv_sec, m_soundCount[filename].tv_sec);
       return 0;
    }
    //if (filename=="res/audio/cs_tj.mp3")
    CCLOG( "AudioManager::playEffect %s\t%ld", filename.c_str(), t2.tv_sec);
    m_soundCount[filename] = t2;
#endif

    //CCLOG("music filename = %s",filename.c_str());
    if (m_soundMap.find(filename) != m_soundMap.end())
    {
        int count = m_soundMap[filename].asInt();
        if (count > 5) {
            return 0;
        }else
        {
            m_soundMap[filename] = Value( m_soundMap[filename].asInt() + 1);
        }
    }else{
        m_soundMap[filename] = 1;
    }
    
    unsigned int nSoundId = SimpleAudioEngine::getInstance()->playEffect(getFullFilePath(filename).c_str(), loop);
    //CCLOG("play music filename = %s nSoundId = %d",filename.c_str(), nSoundId);
    return nSoundId;
}

void AudioManager::stopEffect(unsigned int nSoundId)
{
    //CCLOG("stop music nSoundId = %d",nSoundId);
    SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
}

void AudioManager::pauseEffect(unsigned int nSoundId)
{
    //CCLOG("stop music nSoundId = %d",nSoundId);
    SimpleAudioEngine::getInstance()->pauseEffect(nSoundId);
}

void AudioManager::resumeEffect(unsigned int nSoundId)
{
    //CCLOG("stop music nSoundId = %d",nSoundId);
    SimpleAudioEngine::getInstance()->resumeEffect(nSoundId);
}

void AudioManager::updataEffectCount(float dt)
{
    m_delayTime += dt;
    if (m_delayTime < 0.3) {
        return;
    }
    m_delayTime = 0;
    
    ValueMap::iterator iter = m_soundMap.begin();
    for (iter = m_soundMap.begin(); iter != m_soundMap.end(); iter++) {
        int count = ((*iter).second).asInt();
        if (count > 0) {
            count--;
            (*iter).second = Value(count);
        }
    }
}