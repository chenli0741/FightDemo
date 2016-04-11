//
//  LQBaseUserData.h
//  CZSQ
//
//  Created by chenli on 12-11-24.
//  Copyright (c) 2012年 LequGame. All rights reserved.
//

#ifndef __LQBASEUSERDATA_H__
#define __LQBASEUSERDATA_H__

#include "cocos2d.h"

#include "LQRtti.h"
#include "LQBaseData.h"
#include "LQDataCache.h"
//#include "LQServerResponse.h"
//#include "LQUnitResponse.h"

using namespace cocos2d;
using namespace LQ;

typedef std::vector<string>   RunningSpriteDataVector;

//货币类型
typedef enum{
    klq_moneytype_cash = 1,       //铜钱
    klq_moneytype_provisions = 2, //粮草
    klq_moneytype_gold = 3,       //金币 
    klq_moneytype_achievement = 4 //战功
} LQMoneyTypeEnum;

#define STR_TITLE_MAX 5

#define LQ_SYNTHESIZE_NOVAR(varType, varName, funName)\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define LQ_SYNTHESIZE_PERCENTAGE(curName, maxName, funName)\
public: int get##funName(void) const { return maxName>0 ? curName*100/maxName : 0; }\

#define LQ_SAFE_DELETE_VECTOR(varType, p)  LQUserDataVector::iterator it = (p).begin(); \
for(;it != (p).end();++it) { if(*it != NULL) { delete (varType)*it; *it = NULL; } } (p).clear();\

#define LQ_SAFE_DELETE_MAP(varType, p) LQUserDataMap::iterator iter = (p).begin(); \
for(;iter != (p).end();++iter) { if(iter->second!= NULL){ delete (varType)iter->second; iter->second = NULL; } }\


class LQPopertyValue:public Ref
{
private:
    std::map<std::string,int> m_values;
public:
    LQPopertyValue();
    ~LQPopertyValue();
    bool init(){ return true;};
    
    void addValue(string name,int value);
    int getValue(string name);
    
    CREATE_FUNC(LQPopertyValue);
};

class LQBaseUserData : public LQObject, public Ref
{
private:
    vector<LQPropertyInfo> m_keyarray;  //需要JSON的字段
protected:
    //
    LQBaseData* m_cfgdata;
    //
    int m_pkey;    //分段数据
    
    // 初始化数据
    virtual bool init();
public:
    const char* aliasName;
    
    DECLARE_TYPEINFO(LQBaseUserData);
    
    LQBaseUserData();
    ~LQBaseUserData();
    
    virtual const char* getAliasName();
    
    //重载“<”操作符，自定义排序规则
    bool operator < (const LQBaseUserData &a) const
    {
        //按resIDC从大到小排列
        return a.m_resIDC<m_resIDC;
    }
    static bool sortComp( LQBaseUserData *v1, LQBaseUserData *v2)    {
        return v1->m_resIDC < v2->m_resIDC;//升序排列
    }
        
    bool initWithIDC(int resIDC);
    bool initWithData(LQBaseData* data);
    
    // 初始化数据,子类重写
    virtual void putData();
    //virtual void putDataWithMessage(LQServerResponse* resp){};
    //virtual void putDataWithUnit(LQUnitResponse* resp){};
    
    void _LQDEBUG(const char * format, ...);
    
    // 更改配置数据
    void changeCfgData(int resIDC);
    // 配置数据
    void setCfgData(LQBaseData* data);
    //返回配置数据类，子类覆盖
    virtual LQBaseData* getCfgdata();
 
    //转成JSONString
    ///virtual std::string JSONString();
    //解析JSONString数据
    //virtual void objectWithString(const std::string& str);
    
    //create() 
    CREATEDATA_FUNC(LQBaseUserData);
    
    //属性定义
    LQ_PROPERTY(int, m_tag, Tag );   //Tag
    LQ_PROPERTY(std::string, m_guid, GUID );   //编码 GUID
    LQ_PROPERTY(int, m_resIDC, ResIDC );       //配置资源编码 resIDC
};

typedef std::map<std::string, LQBaseUserData*> LQUserDataMap;
typedef vector<LQBaseUserData*> LQUserDataVector;

#endif


