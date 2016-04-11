//
//  LQPayInfoUserData.h
//  LostEmpire
//
//  Created by lequdev1 on 14-1-24.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//
#ifndef __LQMonsterUserData_H__
#define __LQMonsterUserData_H__

#include "cocos2d.h"
#include "LQBaseUserData.h"
#include "LQMonsterInfoData.h"
#include "LQArmsInfoData.h"
#include "LQHeroUserData.h"


class LQMonsterUserData : public LQHeroUserData
{
private:
    //const char* aliasName;
    //LQArmsInfoData* m_armsCfgdata;
    
protected:
    virtual bool init();
public:
    LQMonsterUserData(int pkey, int resIDC, int soldierIDC, int count);
    ~LQMonsterUserData();
    
    //返回配置数据类，子类覆盖
    LQArmsInfoData* getArmsCfgdata();
    
    // 初始化数据,子类重写
    const char* getAliasName();
    void putData();
    
    //设置初始值，我的英雄将由相同传入
    virtual void defaultData();
private:
    
    LQMonsterInfoData* getMonsterCfgdata();
};

typedef vector<LQMonsterUserData*> LQMonsterUserDataVector;
#endif