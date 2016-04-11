//
//  ResourceManage.h
//  SuperWarriors
//
//  Created by lequdev1 on 14-7-29.
//
//

#ifndef __SuperWarriors__ResourceManage__
#define __SuperWarriors__ResourceManage__

#include <iostream>
#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
#include "LQStringExtend.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace CocosDenshion;

class ResourceManage
{
    ResourceManage();
public:
    ~ResourceManage();
    static ResourceManage* getInstance();
    static void clear();
    
    ValueMap m_resMap;
    void addResFile(string filename, string pvrFileName);
    void removeResFile(string filename);
};
#endif /* defined(__SuperWarriors__ResourceManage__) */
