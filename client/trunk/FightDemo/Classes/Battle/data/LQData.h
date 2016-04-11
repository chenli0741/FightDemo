//
//  LQData.h
//  SuperWarriors
//
//  Created by chenli on 14-6-19.
//
//

#ifndef SuperWarriors_LQData_h
#define SuperWarriors_LQData_h

#include "LQArmsUserData.h"
#include "LQHeroUserData.h"
#include "LQSkillUserData.h"
#include "LQEffectUserData.h"
#include "LQBuffUserData.h"
#include "LQMonsterUserData.h"
#include "LQSquadUserData.h"
#include "LQAttackCalculator.h"

#include "LQActionInfoData.h"
#include "LQArmsInfoData.h"
#include "LQCardInfoData.h"
#include "LQMonsterInfoData.h"
#include "LQFormulaParamData.h"
#include "LQEffectInfoData.h"
#include "LQSkillInfoData.h"
#include "LQMonsterGroupInfoData.h"
#include "LQZoneInfoData.h"
#include "LQOpeningInfoData.h"
#include "LQAnimationInfoData.h"
#include "LQAdsInfoData.h"

/**
 * 根据公式类别和参数代号来获得相生相克系数
 * @param formulaType 公式类别
 * @param param 参数代号
 */
static std::map<std::string, float> m_formulaparamfactors;
static float getFormulaParamFactor(int formulaType, string parameter)
{
    string skey;
    stringf(skey, "%d-%s", formulaType, parameter.c_str());
    std::map<std::string, float>::iterator it = m_formulaparamfactors.find(skey);
	if(it==m_formulaparamfactors.end())
    {
        LQDataMap& cfgdatas = *(LQDataMap*)LQDataCache::sharedLQDataCache()->lqAllDatasByName(LQFormulaParamData::getAliasName());
        LQDataMap::iterator iter;
        for(iter = cfgdatas.begin(); iter != cfgdatas.end(); iter++)
        {
            LQFormulaParamData* pcfgData = (LQFormulaParamData*)iter->second;
            if(pcfgData->getBelongTo()==formulaType && pcfgData->getParameter().compare(parameter.c_str())==0){
                m_formulaparamfactors[skey] = pcfgData->getFactor() / RATE_PERCENT;
                break;
            }
        }
        //内存处理
        delete  &cfgdatas;
        return m_formulaparamfactors[skey];
    }else
        return m_formulaparamfactors[skey];
    return 1.0f;
}
#endif
