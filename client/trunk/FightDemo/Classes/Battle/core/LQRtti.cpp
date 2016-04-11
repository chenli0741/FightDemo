//
//  LQRtti.cpp
//  SuperWarriors
//
//  Created by chenli on 13-4-2.
//
//
#include <map>
#include "LQRtti.h"

static std::map< string,LQTypeInfo*> *classInfoMap = NULL;

LQObject::~LQObject()
{
   //
}

void* LQObject::getValue(const std::string& key)
{
    void *p = m_values[key];
    return p;
}

int LQObject::getIntValue(const std::string& key)
{
    return  *((int*)this->getValue(key));
}

double LQObject::getDoubleValue(const std::string& key)
{
    return  *((double*)this->getValue(key));
}

float LQObject::getFloatValue(const std::string& key)
{
    return  *((float*)this->getValue(key));
}

std::string LQObject::getStringValue(const std::string& key)
{
    return  *((std::string*)this->getValue(key));
}

bool LQObject::getBoolValue(const std::string& key)
{
    return  *((bool*)this->getValue(key));
}

unsigned int LQObject::getUintValue(const std::string& key)
{
    return  *((unsigned int*)this->getValue(key));
}

long LQObject::getLongValue(const std::string& key)
{
    return  *((long*)this->getValue(key));
}

long long LQObject::getLLongValue(const std::string& key)
{
    return  *((long long*)this->getValue(key));
}

Ref LQObject::getCCObjectValue(const std::string& key)
{
    return  *((Ref*)this->getValue(key));
}

void LQObject::setValue(int value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    int* p = (int*)m_values[key];
    //m_values->insert(std::map< string,void*>::value_type(key,value));
    *p = value;
}

void LQObject::setValue(std::string value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    std::string* p = (std::string*)m_values[key];
    *p = value;
}

void LQObject::setValue(bool value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    bool* p = (bool*)m_values[key];
    *p = value;
}

void LQObject::setValue(double value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    double* p = (double*)m_values[key];
    *p = value;
}

void LQObject::setValue(float value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    float* p = (float*)m_values[key];
    *p = value;
}

void LQObject::setValue(unsigned int value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    unsigned int* p = (unsigned int*)m_values[key];
    *p = value;
}

void LQObject::setValue(long value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    long* p = (long*)m_values[key];
    *p = value;
}

void LQObject::setValue(long long value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    long long* p = (long long*)m_values[key];
    *p = value;
}

void LQObject::setValue(Ref value,const std::string& key)
{
    if(m_values[key]==0)
        m_values[key] = &value;
    
    Ref* p = (Ref*)m_values[key];
    *p = value;
}

//内部递增类ID
static int TypeInfoOrder = 0;

bool LQObject::Register(LQTypeInfo* ci)
{
	if(!classInfoMap)	{
		classInfoMap = new std::map< string,LQTypeInfo*>();
	}
	if(ci)	{
        std::map< string,LQTypeInfo*>::const_iterator iter = classInfoMap->find(ci->m_className);
		if(iter == classInfoMap->end()){
            TypeInfoOrder++;
            ci->m_typeid = TypeInfoOrder;
			classInfoMap->insert(std::map< string,LQTypeInfo*>::value_type(ci->m_className,ci));
		}else{
            ci->m_typeid = iter->second->m_typeid;
        }
	}
	return true;
}

LQObject* LQObject::CreateObject(std::string name)
{
	std::map< string,LQTypeInfo*>::const_iterator iter = classInfoMap->find(name);
	if(classInfoMap->end() != iter)	{
		return iter->second->CreateObject();
	}
	return NULL;
}

//静态注册
bool Register(LQTypeInfo* ci)
{
    return LQObject::Register(ci);
}

//静态获取
LQTypeInfo* LQTypeInfoByName(const char* className)
{
    if(!classInfoMap)	{
		classInfoMap = new std::map< string,LQTypeInfo*>();
	}
    std::map< string,LQTypeInfo*>::const_iterator iter = classInfoMap->find(className);
    if(classInfoMap->end() != iter)	{
		return iter->second;
	}
	return 0;
}
