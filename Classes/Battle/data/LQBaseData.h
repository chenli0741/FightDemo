//
//  LQBaseData.h
//  SuperWarriors
//
//  Created by chenli on 12-11-22.
//  Copyright (c) 2012年 LequGame. All rights reserved.
//
#ifndef __LQBASEDATA_H__
#define __LQBASEDATA_H__

#include "cocos2d.h"
#include "../core/DynBase.h"
#include "json/rapidjson.h"
#include "json/document.h"

typedef std::map<int, std::string> LQTipMap;

class LQBaseData :public UObject
{
private:
    //cocos2d::__Array *m_allKeys;    //原始数据键
    cocos2d::__Dictionary *m_dict;  //原始数据
    cocos2d::__Dictionary *m_metadict;
    
    cocos2d::__Dictionary *m_values;
    
    LQTipMap* _tipMaps;
    int m_fID;
protected:
    //被用户获取，仅商品使用
    DECLARE_CLASS(LQBaseData)

    void getValue(const std::string& key,void* p);
    void setValue(cocos2d::Ref* value,const std::string& key);
public:
    LQBaseData();
    virtual ~LQBaseData();
    
    bool initWithDict(cocos2d::__Dictionary *dict,cocos2d::__Dictionary *metadict, LQTipMap *tipMaps);
    bool initWithJson(const rapidjson::Value& dict,cocos2d::__Dictionary *metadict, LQTipMap *tipMaps);
    
    //获取键数组
    //cocos2d::__Array *getAllKeys();
    //获取原始数组
    cocos2d::__Dictionary *getDict();
    
    //
    int getFID();
    //返回类名串，子类覆盖Getter
    static const char * getAliasName();
    //返回类键值
    std::string getIdKey();
    
    static bool sortComp( LQBaseData *v1, LQBaseData *v2)    {
        return v1->m_fID > v2->m_fID;//升序排列
    }
    
};

//
static cocos2d::Ref* getJsonValue(const rapidjson::Value &data)
{
    // parse by type
    switch (data.GetType()) {
        case rapidjson::Type::kFalseType:
            return cocos2d::__Bool::create(false);
            break;
        case rapidjson::Type::kTrueType:
            return cocos2d::__Bool::create(true);
            break;
        case rapidjson::Type::kStringType:
            return cocos2d::__String::create(data.GetString());
            break;
        case rapidjson::Type::kNumberType:
            if(data.IsDouble()) {
                return cocos2d::__Double::create(data.GetDouble());
            } else if(data.IsUint()) {
                int temp = data.GetUint();
                return cocos2d::__Integer::create(temp);
            } else if(data.IsInt()) {
                return cocos2d::__Integer::create(data.GetInt());
            }
            break;
        default:
            break;
    }
    return NULL;
}

#endif
