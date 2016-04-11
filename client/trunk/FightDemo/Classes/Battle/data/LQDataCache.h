//
//  LQBaseDataCache.h
//  SuperWarriors
//
//获取plist的数据文件的内容
//Example:
//  LQCityBuildingData *bdata = [LQCityBuildingData dataWithNone];
//  [[LQDataCache sharedLQDataCache] addDatasWithFile:@"CityBuilding"];
//  NSDictionary* bdata = [[LQDataCache sharedLQDataCache] lqDatasByName:@"CityBuilding"];
//  LQCityBuildingData *citydata = (LQCityBuildingData*)[[LQDataCache sharedLQDataCache] lqDataByName:@"CityBuilding" id:@"1010001" Class:[LQCityBuildingData class]];
//  LQCityBuildingData *citydata = (LQCityBuildingData*)[[LQDataCache sharedLQDataCache] lqDataByName:@"CityBuilding1" id:@"1010001"];
//
//  NSDictionary *records = [[LQDataCache sharedLQDataCache] lqAllDatasByName:@"CityBuilding"];
//
//  Created by chenli on 12-11-23.
//  Copyright (c) 2012年 LequGame. All rights reserved.
//

#ifndef __LQDATACACHE_H__
#define __LQDATACACHE_H__

#include "cocos2d.h"

//#include "LQUtils.h"
#include "LQBaseData.h"
#include "../core/LQStringExtend.h"
#include "../core/LQCore.h"
#include "../core/LQTemplate.h"

#include "json/rapidjson.h"
#include "json/document.h"

using namespace cocos2d;
using namespace LQ;


class JsonValue : public Ref
{
protected:
    rapidjson::Value _value;
public:
    JsonValue(rapidjson::Value& value);
    ~JsonValue();
    
    const rapidjson::Value& getJsonValue(){ return _value; };
    static JsonValue* create(rapidjson::Value& value);
};

typedef std::map<std::string, LQBaseData*> LQDataMap;

class LQDataCache : public Ref
{
protected:
    LQDataMap *lqDatadict_;      //Data数据
    __Dictionary *lqData_;          //原始数据
    __Dictionary *lqDataMetadata_;  //元数据
	std::set<std::string>* loadedFilenames_; //加载数据文件索引
    std::string fileext;
    
    CC_SYNTHESIZE(LQTipMap*, m_tipDatas, TipDatas); //多语言数据
private:
    const char* getKey(const char * file);
    
    std::string getClassKey(const char *  file);
    
    std::string getRecordKey(int id);
    
    std::string getFile(const char * file);
    
    const char* getFileID(const char * file,int pkey);
    
    const char* getRecordID(const char * file,int id);
    const char* getRecordID(const char * file,const char* id);
    
    Value parseValueFromJsonValue(const rapidjson::Value& value);
    __Dictionary* parseDictFromJsonValue(const rapidjson::Value& value,map<string, string> fields);
public:
    LQDataCache();
    virtual ~LQDataCache();
    virtual bool init(void);
    
    /** Retruns ths shared instance of the LQ Data cache */
    static LQDataCache* sharedLQDataCache(void);
    
    /** Purges the cache. It releases all the LQ Datas and the retained instance.
     */
    void purgeSharedLQDataCache(void);
    
    /** Adds multiple LQ Datas from a plist file.
     */
    __Dictionary* addDatasWithFile(const char * file);
    
    /**清除所有数据缓存
     */
    void removeAllDatass();
    
    /**清除某一个数据缓存
     */
    void removeDatassFromFile(const char * file);
    
    /**根据表名获取数据集合
     */
    __Dictionary* lqDatasByName(const char * file);
    //const rapidjson::Value& lqJsonDatasByName(const char * file);
    
    /**根据表名获取元数据集合
     */
    __Dictionary* lqMetadatasByName(const char * file);
    
    /**根据表名，ID获取单个数据
     参数：cs 为返回的数据类，空将根据plist的值返回
     key 分段存放的key
     */
    LQBaseData* lqDataByName(const char * file, int id);
    LQBaseData* lqDataByName(const char * file, int pkey,int id);
    
    LQBaseData* lqDataByName(const char * file, int pkey, int id, LQBaseData* obj);
    /**根据表名，ID获取单个数据
     参数：cs 为返回的数据类，空将根据plist的值返回
     */
    LQDataMap* lqAllDatasByName(const char * file);
    
    LQDataMap* lqAllDatasByName(const char * file, const char* clsname);
    //LQDataMap* lqAllJsonDatasByName(const char * file, const char* clsname);

};

#endif