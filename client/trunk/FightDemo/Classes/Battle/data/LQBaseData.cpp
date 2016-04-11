//
//  LQBaseData.m
//  CZSQ
//
//  Created by chenli on 12-11-22.
//  Copyright (c) 2012年 LequGame. All rights reserved.
//

#include "LQBaseData.h"
#include "LQStringExtend.h"

using namespace cocos2d;


IMPLEMENT_CLASS(LQBaseData)

LQBaseData::LQBaseData():_tipMaps(NULL)
{
    //////////////////////////////
}

LQBaseData::~LQBaseData()
{
    //CC_SAFE_RELEASE_NULL(m_dict);
    //CC_SAFE_RELEASE_NULL(m_allKeys);
    CC_SAFE_RELEASE_NULL(m_values);
}

// 初始化数据
bool LQBaseData::initWithDict(cocos2d::__Dictionary *dict,cocos2d::__Dictionary *metadict, LQTipMap*tipMaps)
{
    _tipMaps = tipMaps;
    //NSLog(@"initWithDict %@", dict);
    m_dict = dict;
    m_metadict = metadict;
    //m_allKeys = dict->allKeys();
    m_values = __Dictionary::create();
    m_values->retain();
    
    DictElement* pElement;
    CCDICT_FOREACH(m_dict, pElement)
    {
        std::string key = pElement->getStrKey();
        
        Ref* value = NULL;
        value = pElement->getObject();
        if (key=="FID" || key=="id")
            m_fID = ((__String*)value)->intValue();
        
        this->setValue(value,key);
    }
    
    return true;
}

// 初始化数据
bool LQBaseData::initWithJson(const rapidjson::Value& dict,cocos2d::__Dictionary *metadict, LQTipMap*tipMaps)
{
    _tipMaps = tipMaps;
    //m_dictJson = dict;
    m_metadict = metadict;
    m_values = CCDictionary::create();
    m_values->retain();
    
    for (rapidjson::Value::ConstMemberIterator itr = dict.MemberonBegin(); itr != dict.MemberonEnd(); ++itr)
    {
        std::string key = itr->name.GetString();
        
        //value = pElement->getObject();
        const rapidjson::Value &data = itr->value;
        Ref* value = getJsonValue(data);
        if (key=="FID" || key=="id")
            m_fID = data.GetInt();
        if(value==NULL)
        {
            CCLOG("ERROR:::  JSON %s is Not Value.", key.c_str());
            continue;
        }
        this->setValue(value,key);
    }
    
    return true;
}

void LQBaseData::getValue(const std::string& key,void* p)
{
    //CCLOG("m_values = %p, %d, %d", m_values, m_values->count(),m_values->retainCount());
#if LQ_DATA_FORMAT>0
    const __String* vstr = m_values->valueForKey(key);  //m_dict
    
    __Dictionary* field = (__Dictionary*)m_metadict->objectForKey(key);
    switch (field->valueForKey("DataType")->intValue()) {
        case 12:
        case 13:
            *(int*)p = vstr->intValue();
            break;
        case 14:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            *(long*)p = (long)vstr->doubleValue();
            break;
        case 15:
            *(bool*)p = (bool)vstr->boolValue();
            break;
        default:  //11
            BOOL islang = field->valueForKey("IsLang")->boolValue();
            if(islang){
            }
            *(string*)p = vstr->getCString();
            break;
    }
#else
    auto vstr = m_values->objectForKey(key);
    __Dictionary* field = (__Dictionary*)m_metadict->objectForKey(key);
    __Integer* _t = (__Integer*)field->objectForKey("DataType");
    __Integer *islang = (__Integer*)field->objectForKey("IsLang");
    
    switch (_t->getValue()) {
        case 12:
        case 13:{
            int x = ((__Integer*)vstr)->getValue();
            if(islang->getValue()>0){
                //CCLOG("%p Field %s is IsLang %s", _tipMaps, key.c_str(), (*_tipMaps)[x].c_str());
                *(string*)p = (*_tipMaps)[x].c_str();
            }else
                *(int*)p = x;
        }
            break;
        case 14:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            *(long*)p = ((__Integer*)vstr)->getValue();
            break;
        case 15:
            *(bool*)p = ((__Bool*)vstr)->getValue();
            break;
        default:  //11
            *(string*)p = ((__String*)vstr)->getCString();
            break;
    }
#endif
    return;
}

void LQBaseData::setValue(Ref* value,const std::string& key)
{
    //CCLOG("%s = %p %s",key.c_str(), value, typeid(*value).name());
    m_values->setObject(value,key);
}

//
//cocos2d::__Array * LQBaseData::getAllKeys()
//{
//    return m_allKeys;
//}
//
cocos2d::__Dictionary* LQBaseData::getDict()
{
    return this->m_dict;
}

const char * LQBaseData::getAliasName()
{
    return "LQBaseData";
}

//
int LQBaseData::getFID()
{
    return m_fID;
}

std::string LQBaseData::getIdKey()
{
    std::string str;
    LQ::stringf(str, "%d", m_fID);
    return str;
}

