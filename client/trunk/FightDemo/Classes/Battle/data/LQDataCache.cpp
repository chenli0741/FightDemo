    //
//  LQBaseDataCache.m
//  CZSQ
//
//  Created by chenli on 12-11-23.
//  Copyright (c) 2012年 LequGame. All rights reserved.
//
#include "LQDataCache.h"
#include "LQEncrypt.h"

using namespace std;
using namespace cocos2d;


#define LQEncryptEnable false
//是否需要解压出数据文件，解压完是明文，读取完后会删掉，建议不解压，直接内存读取转换，如果程序运行出现异常，可以临时把这个值变为true
#define needWriteDataFile  false

JsonValue::JsonValue(rapidjson::Value& value)
{
    _value = value;
}

JsonValue::~JsonValue()
{
    //CC_SAFE_FREE(_value);
}

JsonValue* JsonValue::create(rapidjson::Value& value)
{
    JsonValue *pRet = new JsonValue(value);
    if (pRet)
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

#pragma mark LQBaseDataCache - Alloc, Init & Dealloc

static LQDataCache *m_sharedLQDataCache = NULL;

LQDataCache* LQDataCache::sharedLQDataCache(void)
{
    if (! m_sharedLQDataCache)
    {
        m_sharedLQDataCache = new LQDataCache();
        m_sharedLQDataCache->init();
    }
    
	return m_sharedLQDataCache;
}

// LQDataCache
LQDataCache::LQDataCache():m_tipDatas(NULL)
{
#if LQ_DATA_FORMAT>0
    fileext = ".plist";
#else
    fileext = ".bin";
#endif
}

LQDataCache::~LQDataCache()
{
    CCAssert(m_sharedLQDataCache!= NULL, "Attempted to allocate a second instance of a singleton.   --chenli");
    
    CC_SAFE_DELETE(m_tipDatas);
    
    deleteInMap(*lqDatadict_);
    CC_SAFE_DELETE(lqDatadict_);
	CC_SAFE_RELEASE(lqData_);
	CC_SAFE_RELEASE(lqDataMetadata_);
    CC_SAFE_DELETE(loadedFilenames_);
}

bool LQDataCache::init()
{
    bool bRet = false;
    do
    {
        lqDatadict_ = new LQDataMap();
		lqData_ = new __Dictionary();
		lqDataMetadata_ = new __Dictionary();
		loadedFilenames_ = new std::set<std::string>();
        
        // success
        bRet = true;
    } while(0);
    return bRet;
}

void LQDataCache::purgeSharedLQDataCache()
{
	CC_SAFE_RELEASE_NULL(m_sharedLQDataCache);;
}

//CCString* LQDataCache::Description()
//{
    //return new CCString("");
    //return CCString::createWithFormat("<%@ = %p | num of datas =  %lu>", this->, this, (unsigned long)lqData_->count)->getCString());
//}

#pragma mark LQDataCache - loading lequ datas

__Dictionary* LQDataCache::addDatasWithFile(const char * file)
{
    CCAssert(file, "file filename should not be nil. --chenli");
	const char* key = this->getKey(file);
    
    if (loadedFilenames_->find(key) == loadedFilenames_->end())
	{
        std::string fullPath;
        if(!LQEncryptEnable){
            std::string pszFileName = "res/data/"+getFile(file);
            fullPath = CCFileUtils::getInstance()->fullPathForFilename(pszFileName.c_str());
        }else{
            //不需要解压文件，直接读取加密文件
            if(!needWriteDataFile){
                std::string pszFileName = "res/data/"+getFile(file);
                fullPath = CCFileUtils::getInstance()->fullPathForFilename(pszFileName.c_str());
            }else{
                //需要解压文件，那么解密后输出文件
                std::string pathName ="res/data/"+string(file);
                std::string enPath = LQEncrypt::getEncryptFilePath(pathName);
                std::string writePath =  FileUtils::getInstance()->getWritablePath();
                fullPath =writePath+getFile(file);
                LQEncrypt::decodeAndWriteFile(enPath, fullPath);
            }
        }
        
#if LQ_DATA_FORMAT>0
        //读取plist文件
        __Dictionary *dict = __Dictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
        __Dictionary *metadataDict = (__Dictionary*)dict->objectForKey("metadata");
        __Dictionary *fieldDict = (__Dictionary*)metadataDict->objectForKey("fields");
        __Dictionary *recordsDict = (__Dictionary*)dict->objectForKey("records");
        
        lqData_->setObject(recordsDict, key);
        lqDataMetadata_->setObject(fieldDict, key);
        
        loadedFilenames_->insert(key);
        
        dict->release();
        CC_SAFE_DELETE_ARRAY(key);
        if(LQEncryptEnable){
            LQEncrypt::deleteFile(fullPath);
        }
        return recordsDict;
#else
        //读取json文件
        rapidjson::Document doc;
        if (!FileUtils::getInstance()->isFileExist(fullPath.c_str())) //判断文件是否存在
        {
            CCLOGERROR("LQError BIN data file is not find [%s]", fullPath.c_str());
            return NULL;
        }
        std::string data;
        if(!LQEncryptEnable){
             data= FileUtils::getInstance()->getStringFromFile(fullPath.c_str()); //读取文件数据，初始化doc
        }else{
            //不需要解压文件的话，直接在内存里面解码
            if(!needWriteDataFile){
                //解密
                Data readInfo =FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->fullPathForFilename(fullPath.c_str()));
                unsigned char* resultData;
                resultData = readInfo.getBytes();
                for (int i = 0; i < readInfo.getSize(); i++){
                    resultData[i] ^= LQEncryptCode;
                }
                
                char* tempData =(char*)resultData;
                char* tempSubData=(char*)resultData;
                //把转格式后多出的多余内容截取掉，然后写一个结束符
                int n =readInfo.getSize();
                char *p = tempData;
                char *q = tempSubData;
                int len = strlen(tempData);
                if(n>len) n = len;
                /*p += (len-n);*/   /*从右边第n个字符开始*/
                while(n--) *(q++) = *(p++);
                *(q++)='\0'; /*有必要吗？很有必要*/
                
                //截取后的字符串已经没问题，可以被读取了
                data =  string(tempSubData);
            }else{
                //需要解压的话直接读取明文文件就好
                data= FileUtils::getInstance()->getStringFromFile(fullPath.c_str()); //读取文件数据，初始化doc
            }
            
        }
        
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        if (doc.HasParseError())   //判断读取成功与否
        {
            CCLOGERROR("LQError [%s] get BIN data err! ", file);
            return NULL;
        }
        
        rapidjson::Value &fieldValue = doc["fields"];
        rapidjson::Value &recordsValue = doc["records"];
        //JsonValue* fieldDict = JsonValue::create(fieldValue);
        //JsonValue* recordsDict = JsonValue::create(recordsValue);
        
        map<string, string> fields;
        __Dictionary* fieldDict = __Dictionary::create();
        string fldname;
        for(int i=0; i<fieldValue.Capacity(); i++)
        {
            __Dictionary* _field = __Dictionary::create();
            rapidjson::Value& arraydoc = fieldValue[i];
            __Integer* _dataType = __Integer::create(arraydoc["DataType"].GetInt());
            _field->setObject(_dataType,"DataType");
            __Integer* _dataLang = __Integer::create(arraydoc["IsLang"].GetInt());
            _field->setObject(_dataLang,"IsLang");
            fieldDict->setObject(_field, arraydoc["Name"].GetString());
            //CCLOG("_field: %s %d %d", arraydoc["Name"].GetString(), _dataType->getValue(), _dataLang->getValue());
            stringf(fldname, "F%d", arraydoc["Index"].GetInt());
            fields.insert(pair<string,string>(fldname.c_str(),arraydoc["Name"].GetString()));
        }
        __Dictionary *recordsDict = parseDictFromJsonValue(recordsValue, fields);
        
        //CCLOG("parseDictFromJsonValue: %s count:%d", key, recordsDict->count());
        lqData_->setObject(recordsDict, key);
        lqDataMetadata_->setObject(fieldDict, key);
        
        loadedFilenames_->insert(key);
        
        CC_SAFE_DELETE_ARRAY(key);
        if(LQEncryptEnable){
            if(needWriteDataFile){
                LQEncrypt::deleteFile(fullPath);
            }
        }
        return NULL;
#endif
	}
	else
		CCLOG("cocos2d: CCSpriteFrameCache: file already loaded: %s", key);
    CC_SAFE_DELETE_ARRAY(key);
    return NULL;
}

#pragma mark LQDataCache - removing

void LQDataCache::removeAllDatass()
{
    lqDatadict_->clear();
    lqData_->removeAllObjects();
    lqDataMetadata_->removeAllObjects();
    loadedFilenames_->clear();
}

void LQDataCache::removeDatassFromFile(const char * file)
{
    const char* key = this->getKey(file);
    
	lqData_->removeObjectForKey(key);
    lqDataMetadata_->removeObjectForKey(key);
    
    set<string>::iterator ret = loadedFilenames_->find(key);
    if (ret != loadedFilenames_->end())
    {
        loadedFilenames_->erase(ret);
    }
    CC_SAFE_DELETE_ARRAY(key);
}

#pragma mark LQDataCache - getting
const char* LQDataCache::getKey(const char * file)
{
    std::string filestr = file;
    std::string keystr = filestr;
    int b = filestr.rfind(fileext.c_str());
    if (b>0)
        keystr = filestr.substr(0, filestr.rfind(fileext.c_str()));
    return getCharPtr(keystr);
}

std::string LQDataCache::getClassKey(const char * file)
{
    std::string filestr = file;
    std::string keystr = filestr;
    int b = filestr.rfind(fileext.c_str());
    if (b>0)
        keystr = filestr.substr(0, filestr.rfind(fileext.c_str()));
    LQ::stringf(filestr, "LQ%sData" , keystr.c_str() );
    return filestr;
}

std::string LQDataCache::getFile(const char * file)
{
    std::string filestr = file;
    int b = filestr.rfind(fileext.c_str());
    if (b<=0)
        filestr = filestr+fileext;
    return filestr;
}

std::string LQDataCache::getRecordKey(int id)
{
    string str;
    LQ::stringf(str, "%d", id);
    return str;
}

const char* LQDataCache::getRecordID(const char * file,int id)
{
    string str;
    LQ::stringf(str, "%s-%d", file, id);
    return getCharPtr(str);
}

const char* LQDataCache::getRecordID(const char * file,const char* id)
{
    string str;
    LQ::stringf(str, "%s-%s", file, id);
    return getCharPtr(str);
}

const char* LQDataCache::getFileID(const char * file,int pkey)
{
    string str;
    LQ::stringf(str, "%s%d", file, pkey);
    return getCharPtr(str);
}

/**根据表名获取数据集合
 */
__Dictionary*  LQDataCache::lqDatasByName(const char * file)
{
    const char* key = this->getKey(file);
    if (loadedFilenames_->find(key) == loadedFilenames_->end())
        this->addDatasWithFile(file);

	__Dictionary* data = (__Dictionary*)lqData_->objectForKey(key);
	if( ! data ) {
		CCLOG("LequGame: LQDataCache: file data '%s' not found", key);
	}
    
    CC_SAFE_DELETE_ARRAY(key);
	return data;
}

/**根据表名获取元数据集合
 */
__Dictionary*  LQDataCache::lqMetadatasByName(const char * file)
{
    const char* key = this->getKey(file);
    __Dictionary* data = (__Dictionary*)lqDataMetadata_->objectForKey(key);
	if( ! data ) {
		CCLOG("LequGame: LQDataCache: file Metadata '%s' not found", key);
	}
    CC_SAFE_DELETE_ARRAY(key);
	return data;
}
/**根据表名获取数据集合
 */
//const rapidjson::Value& LQDataCache::lqJsonDatasByName(const char * file)
//{
//    const char* key = this->getKey(file);
//    if (loadedFilenames_->find(key) == loadedFilenames_->end())
//        this->addDatasWithFile(file);
//    
//    JsonValue* data = (JsonValue*)lqData_->objectForKey(key);
//    if( ! data ) {
//        CCLOG("LequGame: LQDataCache: file data '%s' not found", key);
//    }
//    
//    CC_SAFE_DELETE_ARRAY(key);
//    return data->getJsonValue();
//}

/**根据表名，ID获取单个数据
 */
LQBaseData* LQDataCache::lqDataByName(const char * file, int pkey, int id)
{
    const char* key = getRecordID(file, id);
    LQDataMap::iterator it = lqDatadict_->find(key);
    CC_SAFE_DELETE_ARRAY(key);
    if(it!=lqDatadict_->end())
        return (LQBaseData*)it->second;
    return this->lqDataByName( file, pkey, id, NULL );
}

LQBaseData* LQDataCache::lqDataByName(const char * file, int id)
{
    const char* key = getRecordID(file, id);
    LQDataMap::iterator it = lqDatadict_->find(key);
    CC_SAFE_DELETE_ARRAY(key);
    if(it!=lqDatadict_->end())
        return (LQBaseData*)it->second;
    return this->lqDataByName( file, 0, id, NULL );
}

LQBaseData* LQDataCache::lqDataByName(const char * file, int pkey, int id, LQBaseData* obj)
{
    const char* key = getRecordID(file, id);
    const char* _file = file;
    if(pkey>0)
        _file = getFileID(file, pkey);
    
    LQBaseData* obj1 = obj;
    if(obj1==NULL){
        std::string clsname = this->getClassKey(file);
        obj1 = (LQBaseData*)(UObject::CreateObject(clsname));
    }
    CCAssert( obj1!= NULL, "file Class should not be nil." );
    
//#if LQ_DATA_FORMAT>=0
    __Dictionary* dict = this->lqDatasByName( _file );
    if( dict ) {
        __Dictionary *rcd = (__Dictionary*)dict->objectForKey(this->getRecordKey(id)); //
        //CCAssert(rcd!= NULL, "数据不存在.");
        if(rcd==NULL)
        {
            CCLOGERROR("%s 数据不存在. %d", file , id);
            //CC_SAFE_DELETE(obj1);
            return NULL;
            //return obj1;
        }

        obj1->initWithDict(rcd,lqMetadatasByName(_file),m_tipDatas);

        //lqDatadict_->setObject(obj1, key);
        lqDatadict_->insert(pair<string, LQBaseData*>(key, obj1));
        CC_SAFE_DELETE_ARRAY(key);
        return obj1;
    }
//#else
//    const rapidjson::Value &dict = this->lqJsonDatasByName( _file );
//    if( &dict ) {
//        rapidjson::Value &rcd = (rapidjson::Value&)dict[this->getRecordKey(id).c_str()]; //
//        //CCAssert(rcd!= NULL, "数据不存在.");
//        if(&rcd==NULL)
//        {
//            CCLOGERROR("%s 数据不存在. %d", file , id);
//            //CC_SAFE_DELETE(obj1);
//            return NULL;
//            //return obj1;
//        }
//        
//        obj1->initWithJson(rcd,lqMetadatasByName(_file));
//        
//        lqDatadict_->insert(pair<string, LQBaseData*>(key, obj1));
//        CC_SAFE_DELETE_ARRAY(key);
//        return obj1;
//    }
//#endif

    CC_SAFE_DELETE_ARRAY(key);
	return obj1;
}

/**根据表名，ID获取单个数据
 */
LQDataMap*  LQDataCache::lqAllDatasByName(const char * file)
{
    //Object* obj = Object::CreateObject(this->getClassKey(file));
    //CCAssert(cs, "file Class LQ%@Data should not be nil. --chenli.",[self getKey:file]);
//#if LQ_DATA_FORMAT>=0
    return this->lqAllDatasByName( file, this->getClassKey(file).c_str() );
//#else
//    return this->lqAllJsonDatasByName( file, this->getClassKey(file).c_str() );
//#endif
}

LQDataMap* LQDataCache::lqAllDatasByName(const char * file,const char* clsname )
{
    __Dictionary *dict = this->lqDatasByName( file );
    
    LQDataMap *records = new LQDataMap();
    DictElement* pElement;
    CCDICT_FOREACH(dict, pElement)
    {
        const char* id = pElement->getStrKey();
        const char* key = getRecordID(file, atoi(id));
        LQBaseData *record = NULL;
        LQDataMap::iterator it = lqDatadict_->find(key);
        if(it==lqDatadict_->end())
        {
            __Dictionary *data = (__Dictionary*)dict->objectForKey(id);
            record = (LQBaseData*)UObject::CreateObject(clsname);
            record->initWithDict(data,lqMetadatasByName(file),m_tipDatas);
            //lqDatadict_->setObject(record, key);
            lqDatadict_->insert(pair<string, LQBaseData*>(key, record));
        }else
            record = (LQBaseData*)it->second;
        records->insert(pair<string, LQBaseData*>(key, record));
        CC_SAFE_DELETE_ARRAY(key);
    }
	return records;
}
/*
LQDataMap* LQDataCache::lqAllJsonDatasByName(const char * file,const char* clsname )
{
    const rapidjson::Value &dict = this->lqJsonDatasByName( file );
    
    LQDataMap *records = new LQDataMap();
    
    for (rapidjson::Value::ConstMemberIterator itr = dict.MemberonBegin(); itr != dict.MemberonEnd(); ++itr)
    {
        //auto el = parseValueFromJsonValue(itr->value);
        //dict[jsonKey] = el;
        
        const char* id = itr->name.GetString();
        const char* key = getRecordID(file, id);
        LQBaseData *record = NULL;
        LQDataMap::iterator it = lqDatadict_->find(key);
        if(it==lqDatadict_->end())
        {
            const rapidjson::Value &data = itr->value;
            //__Dictionary *data = (__Dictionary*)dict->objectForKey(id);
            record = (LQBaseData*)UObject::CreateObject(clsname);
            record->initWithJson(data,lqMetadatasByName(file));
            //lqDatadict_->setObject(record, key);
            lqDatadict_->insert(pair<string, LQBaseData*>(key, record));
        }else
            record = (LQBaseData*)it->second;
        records->insert(pair<string, LQBaseData*>(key, record));
        CC_SAFE_DELETE_ARRAY(key);

    }
    return records;
}*/

// TODO: add any necessary 'const', 'const' & references, or error checking
Value LQDataCache::parseValueFromJsonValue(const rapidjson::Value& value)
{
    // parse by type
    auto t = value.GetType();
    
    if(t == rapidjson::Type::kFalseType) {
        return Value(false);
    }
    
    if(t == rapidjson::Type::kTrueType) {
        return Value(true);
    }
    
    if(t == rapidjson::Type::kStringType) {
        return Value(value.GetString());
    }
    
    if(t == rapidjson::Type::kNumberType) {
        if(value.IsDouble()) {
            return Value(value.GetDouble());
        } else if(value.IsUint()) {
            int temp = value.GetUint();
            return Value(temp);
        } else if(value.IsInt()) {
            return Value(value.GetInt());
        }
    }
    
    if(t == rapidjson::Type::kObjectType) {
        ValueMap dict;
        for (rapidjson::Value::ConstMemberIterator itr = value.MemberonBegin(); itr != value.MemberonEnd(); ++itr)
        {
            auto jsonKey = itr->name.GetString();
            auto el = parseValueFromJsonValue(itr->value);
            dict[jsonKey] = el;
        }
        return Value(dict);
    }
    if(t == rapidjson::Type::kArrayType) {
        ValueVector arr;
        for (rapidjson::Value::ConstValueIterator itr = value.onBegin(); itr != value.onEnd(); ++itr)
        {
            CCLOG("%d ", itr->GetInt());
            auto el = parseValueFromJsonValue(itr);
            arr.push_back(el);
        }
        return Value(arr);
    }
    
    // none
    return Value();
}

// TODO: add any necessary 'const', 'const' & references, or error checking
__Dictionary* LQDataCache::parseDictFromJsonValue(const rapidjson::Value& value,map<string, string> fields)
{
    __Dictionary* records = __Dictionary::create();
    //每条记录
    for (rapidjson::Value::ConstMemberIterator itr = value.MemberonBegin(); itr != value.MemberonEnd(); ++itr)
    {
        std::string key = itr->name.GetString();
        //int idx = key.find("R"); if(idx>=0) key = key.erase(idx, 1);
        //int id = atoi(key.c_str());
        const rapidjson::Value &rcddata = itr->value;
        __Dictionary* record = __Dictionary::create();
        record->setObject(cocos2d::__String::create(key), "FID");
        for (rapidjson::Value::ConstMemberIterator itr1 = rcddata.MemberonBegin(); itr1 != rcddata.MemberonEnd(); ++itr1)
        {
            const rapidjson::Value &data = itr1->value;
            Ref* value = getJsonValue(data);
            string fname = fields[itr1->name.GetString()];
            if (fname.length()<=0)
                fname = itr1->name.GetString();
            record->setObject(value, fname.c_str());
        }
        
        records->setObject(record, key);
    }
    return records;
}



