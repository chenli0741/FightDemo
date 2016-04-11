//
//  DynBase.h
//  sgmz
//
//  Created by chenli on 13-3-28.
//
//
#ifndef __DYNBASE_H__
#define __DYNBASE_H__
#include <string>
#include "cocos2d.h"

class UObject;
class ClassInfo;

typedef UObject* (*ObjectConstructorFn)(void);
bool Register(ClassInfo* ci);
using namespace std;

    
class ClassInfo 
{
public:
	ClassInfo(const std::string className,ObjectConstructorFn ctor)
	:m_className(className)	,m_objectConstructor(ctor)
	{
		Register(this);
	}
	virtual ~ClassInfo(){}
	UObject* CreateObject()const { return m_objectConstructor ? (*m_objectConstructor)() : 0; 	}
	bool IsDynamic()const { return NULL != m_objectConstructor;}
	const std::string GetClassName()const { return m_className;}
	ObjectConstructorFn GetConstructor()const{ return m_objectConstructor;}
public:
	string m_className;
	ObjectConstructorFn m_objectConstructor;
};

#define LQ_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: inline varType get##funName(void){ \
this->getValue(#funName, &varName);\
return varName;\
}\

#define LQ_SYNTHESIZE_LANG_READONLY(varType, varName, funName, langName)\
protected: varType varName;\
public: inline varType get##funName(void){ \
    this->getValue(#langName, &varName);\
    return varName;\
}\

#define DECLARE_CLASS(name) \
protected: \
static ClassInfo ms_classinfo; \
public:  \
virtual ClassInfo* GetClassInfo() const; \
static UObject* CreateObject();

#define IMPLEMENT_CLASS_COMMON(name,func) \
ClassInfo name::ms_classinfo((#name), \
(ObjectConstructorFn) func); \
\
ClassInfo *name::GetClassInfo() const \
{return &name::ms_classinfo;}

#define IMPLEMENT_CLASS(name)			 \
IMPLEMENT_CLASS_COMMON(name,name::CreateObject) \
UObject* name::CreateObject()                   \
{ return new name;}

class UObject //: public cocos2d::CCObject
{
	DECLARE_CLASS(UObject)
    static ClassInfo rttiTypeInfo; 
public:
	UObject(){}
	virtual ~UObject(){}
	static bool Register(ClassInfo* ci);
	static UObject* CreateObject(string name);
};


#endif
