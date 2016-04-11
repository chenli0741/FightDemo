//
//  LQRtti.h
//  SuperWarriors
//
//  Created by chenli on 13-4-2.
//
//

#ifndef __LQRtti_H__
#define __LQRtti_H__

#include <string>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

typedef enum {
    kLQPropertyType_String = 1,
    kLQPropertyType_Int = 2,
    kLQPropertyType_UInt = 3,
    kLQPropertyType_Long = 4,
    kLQPropertyType_LLong = 5,
    kLQPropertyType_Real = 6,
    kLQPropertyType_Boolean = 7,
    kLQPropertyType_Array = 8,
    kLQPropertyType_Object = 9
} LQPropertyTypeEnum;

//属性字段结构
struct LQPropertyInfo
{
public:
    const char *m_propName;
    LQPropertyTypeEnum m_propType;
};

class LQObject;
class LQTypeInfo;

typedef LQObject* (*LQObjectConstructorFn)(void);
bool Register(LQTypeInfo* ci);
LQTypeInfo* LQTypeInfoByName(const char* className);

//类型信息结构
class LQTypeInfo
{
private:
    LQTypeInfo* pBaseClass;
public:
    LQTypeInfo(const char* className,int _typeid, const char* baseclassName, LQObjectConstructorFn ctor)
     :m_className(className), m_typeid(_typeid), m_baseclassName(baseclassName), m_objectConstructor(ctor),pBaseClass(0)
    {
        Register(this);
    }
    virtual ~LQTypeInfo(){}
    LQObject* CreateObject()const { return m_objectConstructor ? (*m_objectConstructor)() : 0; 	}
	bool IsDynamic()const { return NULL != m_objectConstructor;}
	const std::string GetClassName()const { return m_className;}
	LQObjectConstructorFn GetConstructor()const{ return m_objectConstructor;}
    LQTypeInfo* GetBaseClass(){
        if(pBaseClass==0)
            pBaseClass = LQTypeInfoByName(this->m_baseclassName);
        return pBaseClass;
    };
public:
    const char *m_className;
    const char *m_baseclassName;
    int m_typeid;
    LQObjectConstructorFn m_objectConstructor;
    
    int operator== (const LQTypeInfo& info){
        return this==&info;
    }
    int operator!= (const LQTypeInfo& info){
        return this!=&info;
    }
};

#define TYPEINFO_OF_CLASS(class_name) (class_name::GetTypeInfoClass())
#define TYPEINFO_OF_OBJ(obj_name) (obj_name.GetTypeInfo())
#define TYPEINFO_OF_PTR(ptr_name) (ptr_name->GetTypeInfo())

/**
 */
#define DECLARE_TYPEINFO(class_name) \
public:  \
static LQTypeInfo ms_classinfo;\
static class_name* CreateObject();\
static LQTypeInfo GetTypeInfoClass();\
bool IsKindOf(LQTypeInfo& cls);\
int GetTypeID(){return class_name::ms_classinfo.m_typeid;}\
const char* GetTypeName(){return class_name::ms_classinfo.m_className;}\
LQTypeInfo& GetTypeInfo(){return ms_classinfo;}\

/**
 */
#define LQ_PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: inline void set##funName(varType var){\
    varName = var;\
}\
public: inline varType get##funName(void){\
    return varName;\
}\

#define LQ_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName;\
public: inline varType get##funName(void){ \
this->getValue(#funName, &varName);\
return varName;\
}\

/**
 */
#define CCX_PROPERTY_IMPLEMENT_INIT(name, varName)\
m_values[#name] = &varName;\

#define CCX_PROPERTY_IMPLEMENT_INT_INIT(name, varName)\
m_values[#name] = &varName;\
varName = 0;\

#define CCX_PROPERTY_IMPLEMENT_STR_INIT(name, varName)\
m_values[#name] = &varName;\
varName = "";\

#define CCX_PROPERTY_IMPLEMENT_BOOL_INIT(name, varName)\
m_values[#name] = &varName;\
varName = false;\

#define CCX_PROPERTY_IMPLEMENT_FLOAT_INIT(name, varName)\
m_values[#name] = &varName;\
varName = 0.0f;\

/**
 */
#define IMPLEMENT_TYPEINFO(class_name, base_name )			 \
LQTypeInfo class_name::ms_classinfo((#class_name), 0 ,\
       (#base_name) , (LQObjectConstructorFn)(class_name::CreateObject));\
class_name* class_name::CreateObject(){\
    class_name* obj = new class_name();\
    return obj;\
}\
LQTypeInfo class_name::GetTypeInfoClass()\
{\
    return class_name::ms_classinfo;\
}\
bool class_name::IsKindOf(LQTypeInfo& cls)\
{\
    LQTypeInfo* p=&(this->GetTypeInfo());\
    while(p!=NULL){\
        if(p->m_typeid==cls.m_typeid)\
            return true;\
        p=p->GetBaseClass();\
    }\
    return false;\
}\

/**
 * define a create function for a specific type, such as LQObject
 * @__TYPE__ class type to add createObject(), such as LQObject
 */
#define CREATEDATA_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

class LQObject
{
public:
    virtual bool IsKindOf(LQTypeInfo& cls){ return false;};
    
    static LQTypeInfo* GetTypeInfoClass();
    static bool Register(LQTypeInfo* ci);
	static LQObject* CreateObject(string name);
    
    void* getValue(const std::string& key);
    int getIntValue(const std::string& key);
    double getDoubleValue(const std::string& key);
    float getFloatValue(const std::string& key);
    std::string getStringValue(const std::string& key);
    bool getBoolValue(const std::string& key);
    unsigned int getUintValue(const std::string& key);
    long getLongValue(const std::string& key);
    long long getLLongValue(const std::string& key);
    Ref getCCObjectValue(const std::string& key);
    void setValue(int value,const std::string& key);
    void setValue(double value,const std::string& key);
    void setValue(float value,const std::string& key);
    void setValue(std::string value,const std::string& key);
    void setValue(bool value,const std::string& key);
    void setValue(unsigned int value,const std::string& key);
    void setValue(long value,const std::string& key);
    void setValue(long long value,const std::string& key);
    void setValue(Ref value,const std::string& key);
public:
    //LQTypeInfo* ms_classinfo;
    ~LQObject();
    
protected:
    typedef std::map<std::string, void *> LQValuesMap;
    LQValuesMap m_values;
    
};
    
template <class T>
inline T* safe_cast(LQObject* ptr,LQTypeInfo& cls)
{
    return (ptr->IsKindOf(cls) ? (T*)ptr : NULL);
}
#endif
