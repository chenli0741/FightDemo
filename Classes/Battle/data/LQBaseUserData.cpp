//
//  LQBaseUserData.m
//  CZSQ
//
//  Created by chenli on 12-11-24.
//  Copyright (c) 2012年 LequGame. All rights reserved.
//

#include "LQBaseUserData.h"

//#include <json.h>

using namespace cocos2d;

//------------------- LQPopertyValue -------------------------

LQPopertyValue::LQPopertyValue()
{
    
}

LQPopertyValue::~LQPopertyValue()
{
    
}

void LQPopertyValue::addValue(string name,int value)
{
    std::map<std::string,int>::iterator it = m_values.find(name);
    if(it==m_values.end())
    {
        m_values.insert(pair<string, int>(name, value));
    }else{
        m_values.erase(it);
        m_values.insert(pair<string, int>(name, value));
    }
}

int LQPopertyValue::getValue(string name)
{
    std::map<std::string,int>::iterator it = m_values.find(name);
    if(it!=m_values.end())
        return (int)it->second;
    return 0;
}

//------------------- LQBaseUserData -------------------------

IMPLEMENT_TYPEINFO(LQBaseUserData, LQObject)
/*
LQBaseUserData* LQBaseUserData::CreateObject()                   
{ 
    LQBaseUserData* obj = new LQBaseUserData();
    obj->ms_classinfo = LQBaseUserData::CreateTypeInfo();
    return obj;
}

LQTypeInfo* LQBaseUserData::GetTypeInfoClass()
{
    LQTypeInfo* typeInfo = LQTypeInfoByName("LQBaseUserData");
    if(typeInfo==0)
        typeInfo = new LQTypeInfo( "LQBaseUserData", 0 ,
                                  "LQObject" , (LQObjectConstructorFn)(LQBaseUserData::CreateObject));
    return typeInfo;
}*/

LQBaseUserData::LQBaseUserData():m_resIDC(-1),m_cfgdata(NULL),m_tag(0),m_pkey(0)
{
    aliasName = "LQBaseData";
    m_cfgdata = NULL;
    //m_keyarray = new vector<LQPropertyInfo>();
}

// 释放对象资源
LQBaseUserData::~LQBaseUserData()
{
    //CCLOG("LQBaseUserData::~LQBaseUserData");
	// in case you have something to dealloc, do it in this method
	// in this particular example nothing needs to be released.
	// cocos2d will automatically release all the children (Label)
	//delete(m_guid);
    
    //CC_SAFE_DELETE(m_keyarray);
}

//返回类名串，子类覆盖
const char* LQBaseUserData::getAliasName()
{
    return this->aliasName;
    //return "LQBaseData";
}

// 初始化数据
bool LQBaseUserData::init()
{
    bool bRet = false;
    do
    {
        m_guid = "";
        CCX_PROPERTY_IMPLEMENT_INIT(resIDC,m_resIDC)
        CCX_PROPERTY_IMPLEMENT_INIT(guid,m_guid)
        //m_values["resIDC"] = &m_resIDC;
        //m_values["guid"] = &m_guid;
        
        LQPropertyInfo p1 = {"resIDC", kLQPropertyType_Int};
        m_keyarray.push_back( p1 );
        LQPropertyInfo p2 = {"guid", kLQPropertyType_String};
        m_keyarray.push_back( p2 );
        
        
        //CCObject *obj = new int(1000081);
        this->setValue( m_resIDC, "resIDC" );
        
        bRet = true;
    } while(0);
    return bRet;
}

bool LQBaseUserData::initWithIDC(int resIDC)
{
    if( !this->init() )
        return false;
    m_guid = "";
    m_resIDC = resIDC;
    
    if(resIDC>0){
        LQBaseData* cfgdt_ = LQDataCache::sharedLQDataCache()->lqDataByName(this->getAliasName(), m_pkey, m_resIDC);
        this->setCfgData(cfgdt_);
    }
    this->putData();
    
    return true;
}

// 初始化数据
bool LQBaseUserData::initWithData(LQBaseData* data)
{
    if( !this->init() )
        return false;
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super's" return value
    if(data==NULL)
        return false;
	this->setCfgData(data);
    m_resIDC = m_cfgdata->getFID();
    this->putData();
    
    return true;
}


// 初始化数据,子类重写
void LQBaseUserData::putData()
{
    return;
}

void LQBaseUserData::_LQDEBUG(const char * format, ...)
{
    //if(type!=_DEBUG_TYPE)
        return;
    va_list args;
    va_start(args, format);
    log(format, args);
    va_end(args);
}
/*
//转成JSONString
std::string LQBaseUserData::JSONString()
{
    Json::Value root;
    Json::FastWriter writer;
    for (vector<LQPropertyInfo>::iterator it = m_keyarray.begin(); it != m_keyarray.end(); it++)
    {
        std::string name = it->m_propName;
        switch (it->m_propType) {
            case kLQPropertyType_String:
                root[name] = this->getStringValue(name);
                break;
            case kLQPropertyType_Int:
                root[name] = this->getIntValue(name);
                break;
            case kLQPropertyType_UInt:
                root[name] = this->getUintValue(name);
                break;
            case kLQPropertyType_Real:
                root[name] = this->getDoubleValue(name);;
                break;
            case kLQPropertyType_Boolean:
                root[name] = this->getBoolValue(name);;
                break;
            default:
                break;
        }
    }
    std::string json_str = writer.write(root);
    
    return json_str;
}

//解析JSONString数据
void LQBaseUserData::objectWithString(const std::string& str)
{
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(str, root, false))
        return;
    //void *value;
    for (vector<LQPropertyInfo>::iterator it = m_keyarray.begin(); it != m_keyarray.end(); it++)
    {
        std::string name = it->m_propName;
        switch (it->m_propType) {
            case kLQPropertyType_String:
                this->setValue( root[name].asString() , name);
                break;
            case kLQPropertyType_Int:
                this->setValue( root[name].asInt() , name);
                break;
            case kLQPropertyType_UInt:
                this->setValue( root[name].asUInt() , name);
                break;
            case kLQPropertyType_Real:
                this->setValue( root[name].asDouble() , name);
                break;
            case kLQPropertyType_Boolean:
                this->setValue( root[name].asBool() , name);
                break;
            default:
                break;
        }
        
    }
    m_resIDC = root["resIDC"].asInt();
    
    return;
}

*/

// 更改配置数据
void LQBaseUserData::changeCfgData(int resIDC)
{
    if(m_resIDC==resIDC) return;
    m_resIDC = resIDC;
    LQBaseData* cfgdt_ = LQDataCache::sharedLQDataCache()->lqDataByName(this->getAliasName(),m_resIDC);

    this->setCfgData(cfgdt_);
    //this->putData();
}

// 配置数据
void LQBaseUserData::setCfgData(LQBaseData* data)
{
    if (m_cfgdata!=NULL && m_cfgdata!=data)
        m_cfgdata = NULL;
    m_cfgdata = data;
}

//返回配置数据类，子类覆盖
LQBaseData* LQBaseUserData::getCfgdata()
{
    return m_cfgdata;
}

/*
int LQBaseUserData::getResIDC()
{
    return m_resIDC;
}

std::string LQBaseUserData::getGUID()
{
    return m_guid;
}*/
