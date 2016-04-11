//
//  ResourceManage.cpp
//  SuperWarriors
//
//  Created by lequdev1 on 14-7-29.
//
//

#include "ResourceManage.h"
static ResourceManage* _resourceInstance = NULL;
ResourceManage::ResourceManage()
{

}
ResourceManage::~ResourceManage()
{

}
ResourceManage* ResourceManage::getInstance()
{
    if (!_resourceInstance) {
        _resourceInstance = new ResourceManage();
    }
    
    return _resourceInstance;
}
void ResourceManage::clear()
{
    CC_SAFE_DELETE(_resourceInstance);
    _resourceInstance = NULL;
}
void ResourceManage::addResFile(string filename, string pvrFileName)
{
    if (m_resMap.find(filename) != m_resMap.end())
    {
        int count = m_resMap[filename].asInt();
        if (count <= 0) {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename, pvrFileName);
            count = 0;
        }
        m_resMap[filename] = Value( count + 1);
    }else{
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename, pvrFileName);
        m_resMap[filename] = 1;
    }
    
    //log("ResourceManage::++++++ add spriteFrameFile (%s),retainCount = %d",filename.c_str(),m_resMap[filename].asInt());
}
void ResourceManage::removeResFile(string filename)
{
    if (m_resMap.find(filename) == m_resMap.end())
    {
        return;
    }
    m_resMap[filename] = Value( m_resMap[filename].asInt() - 1);
    if (m_resMap[filename].asInt() <= 0) {
        m_resMap[filename] = Value(0);
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(filename);
    
        //log("ResourceManage::----- remove spriteFrameFile (%s),retainCount = %d",filename.c_str(),m_resMap[filename].asInt());
    }
}