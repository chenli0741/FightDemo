//
//  DownLoad.cpp
//  test
//
//  Created by lequ on 14-2-19.
//
//

#include "UpdateRes.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

CURL* s_pCurl = NULL;//curl库
FILE* s_pfileWrite = NULL;//文件句柄
unsigned long s_iFileSize = 0;//总文件大小
unsigned long s_iCurrentFileSize = 0;//当前下载大小
unsigned long s_CurrentWJ; // 当前下载写入的大小
unsigned long s_fullSizeWJ; // 下载文件的总大小
bool s_fileBack = false;

//下载单个文件已消耗的时间
double s_downTime = 0;
unsigned long s_downSpeed = 0;

// 获取手机路径
std::string localPathWrith;

static UpdateRes *instance;
UpdateRes* UpdateRes::getInstance()
{
    if (NULL==instance) {
        instance = new UpdateRes;
    }
    if (NULL != instance) {
        return instance;
    }else{
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

UpdateRes::UpdateRes()
{
    // 初始化一些变量
    currentCount = 0;
    totalSize = 0;
    // 获取手机上的相对路径
    localPathWrith = CCFileUtils::getInstance()->getWritablePath();
    CCLOG("手机路径:%s",localPathWrith.c_str());
}

UpdateRes::~UpdateRes()
{
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


// 发送请求
void UpdateRes::sendHTTP(std::string url,std::string postContent,SEL_CallFuncND  callBack){
    HttpRequest* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    request->setResponseCallback(this, callBack);
    // write the post data
    const char* postData = postContent.c_str();
    request->setRequestData(postData, strlen(postData));
    request->setTag("POST test2");
    HttpClient::getInstance()->send(request);
    request->release();
}

void UpdateRes::needUpdataJson(Node *sender, void *data)
{
    // 请求需要更新什么文件的回调处理
    HttpResponse *response = (HttpResponse*)data;
    if(response->getResponseCode() == 200){
        std::vector<char> *buffer = response->getResponseData();
        std::string bufs(buffer->begin(),buffer->end());
        CCLOG("请求bufs:%s",bufs.c_str());
        
        std::vector<std::string> ajson;
        split(bufs.c_str(), ';', ajson);
        for (int i = 0; i < ajson.size(); i++) {
            if(i==110) {
//                // 验证版本是否需要更新 是app包要不要更新 不是在线更新资源
//                // need 的值:
//                // 0 最新版本
//                // 1 需要更新资源
//                // 2 需要下载客户端
//                rapidjson::Document d1;
//                std::string sjson = ajson.at(i);
//                d1.Parse<0>(sjson.c_str());
//                int need = d1["need"].GetInt(); //更新状态
//                serverVersion = d1["version"].GetString(); //服务器版本
//                _delegate->pushSVersion(currentVersion.c_str(), serverVersion.c_str());
//                CCLOG("\n更新状态:%d  服务器资源版本:%s",need,serverVersion.c_str());
//                if (need == 0){ // 直接算下载完成
//                    if(_delegate){
//                        _delegate->updateResEnd();
//                        break;
//                    }
//                }else if(need == 1){ // 不用搭理他  他自己一会就更新了
//                }else if(need == 2){ // 什么都不用干了  直接提示需要更新客户点
//                    if(_delegate){
//                        _delegate->errProcess(errCode_upversion, "");
//                        break;
//                    }
//                }
            }else if(i == 0){
                // 删除资源
                std::string sjson = ajson.at(i);
                rapidjson::Document _doc;
                _doc.Parse<0>(sjson.c_str());
                CC_BREAK_IF(_doc.HasParseError());
                const rapidjson::Value &pArray = _doc["entities"];
                for (rapidjson::SizeType i = 0; i < pArray.Size(); i++){
                    const rapidjson::Value &p = pArray[i];
                    const rapidjson::Value &valueEnt = p["entity"];
                    
                    const rapidjson::Value &jpath = valueEnt["p"];
                    std::string path = jpath.GetString();
                    CCLOG("有要删除的资源:%s",path.c_str());
                    
                    std::string strpath = StringUtils::format("%s%s",localPathWrith.c_str(),path.c_str());
                    std::string str = strpath;
                    char * writable = new char[str.size() + 1];
                    std::copy(str.begin(), str.end(), writable);
                    writable[str.size()] = '\0'; // don't forget the terminating 0
                    removeFile(writable);
                    delete[] writable;
                }
            }else if(i == 1){
                // 更新资源
                std::string sjson = ajson.at(i);
                rapidjson::Document _doc;
                _doc.Parse<0>(sjson.c_str());
                CC_BREAK_IF(_doc.HasParseError());
                const rapidjson::Value &pArray = _doc["entities"];
                for (rapidjson::SizeType i = 0; i < pArray.Size(); i++){
                    const rapidjson::Value &p = pArray[i];
                    const rapidjson::Value &valueEnt = p["entity"];
                    
                    const rapidjson::Value &jpath = valueEnt["p"];
                    std::string path = jpath.GetString();
                    const rapidjson::Value &jsize = valueEnt["s"];
                    int size = jsize.GetInt();
                    const rapidjson::Value &jurl = valueEnt["u"];
                    std::string url = jurl.GetString();
                    
//                    log("path: %s", path.c_str());
//                    log("size: %d", size);
//                    log("url: %s", url.c_str());
                    
                    totalSize += size; //计算出要下载文件的总大小
                    // 将要下载的资源放到缓存中
                    MSG_DOWNFINE msg = {path, size, url};
                    vDownFile.push_back(msg);
                }
                this->doDownFile();
            }
        }
        
    }else{
        this->_delegate->errProcess(errCode_getMainJson, "");
    }
}

void UpdateRes::removeFile(char *file)
{
    // 删除文件
    remove(file);
}

// 遍历目录  删除其下所有文件
void UpdateRes::rm_file(char *path)
{
    struct dirent* ent = NULL;
    DIR *dir;
    static char subdir[2048] = "";
    
    if (!strcmp(subdir, ""))
        strcpy(subdir, path);
    dir = opendir(path);
    
    if(dir == NULL) {
        CCLOG("ERR:dir is nill");
        return;
    }
    
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == 8 || ent->d_type == 10) {
            char file[2048] = "";
            strcpy(file, subdir);
            strcat(file, "/");
            strcat(file, ent->d_name);
            //printf("List file %s\n", ent->d_name);
            remove(file);
            CCLOG("删除文件 %s",ent->d_name);
        }
    }
    closedir(dir);
}

// 遍历目录  删除其下所有文件
void UpdateRes::rm_dir(char *path)
{
    struct dirent* ent = NULL;
    DIR *dir;
    static char subdir[2048] = "";
    
    if (!strcmp(subdir, ""))
        strcpy(subdir, path);
    dir = opendir(path);
    
    if(dir == NULL) {
        CCLOG("ERR:dir is nill");
        return;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == 8 || ent->d_type == 10) {
            char file[2048] = "";
            strcpy(file, subdir);
            strcat(file, "/");
            strcat(file, ent->d_name);
            //printf("List file %s\n", ent->d_name);
            remove(file);
            CCLOG("删除文件 %s",ent->d_name);
        }
        else {
            if(!strcmp(ent->d_name, "..") || !strcmp(ent->d_name, "."))
                continue;
            strcat(subdir, "/");
            strcat(subdir, ent->d_name);
            //printf("Sub dir %s %d dirpath = %s type = %d\n", ent->d_name, ent->d_reclen, subdir, ent->d_type);
            rm_dir(subdir);
            remove(subdir);
            subdir[(int)(strlen(subdir) - strlen(ent->d_name) - 1)] = '\0';
            //printf("Return %s %d\n", ent->d_name, ent->d_reclen);
            CCLOG("删除文件夹 %s",ent->d_name);
        }
    }
    closedir(dir);
}

// 下载文件
void UpdateRes::doDownFile()
{
    if(vDownFile.size() > 0){
        CCLOG("有%lu个资源文件需要更新 总大小:%d",vDownFile.size(),this->totalSize);
        
        // 开启下载线程 下载资源
        this->requestResouceContent(NULL);
        
        // 启动计时器 下载资源
        Scheduler* scheduler = Director::getInstance()->getScheduler();
        scheduler->scheduleSelector(schedule_selector(UpdateRes::tickTime),
                                    this,
                                    1.0 / 60.0,
                                    false);
    }else{
        CCLOG("没有需要下载的文件");
        if (_delegate) {
            _delegate->updateResEnd();
        }
    }
}

void UpdateRes::requestResouceContent(const char* url)
{
    //开启新线程下载文件
    pthread_create(&pid,NULL,downFile,NULL);
    pthread_detach(pid);
}

size_t downWriteCallBack(void *ptr, size_t size, size_t nmemb, void *stream)
{
    int written = fwrite(ptr, size, nmemb, (FILE *)s_pfileWrite);
    //CCLOG("size download=%ld",size*nmemb);
    s_iCurrentFileSize += size*nmemb;
    
    s_CurrentWJ += written;
    s_fullSizeWJ += written;
    //UpdateRes::getInstance()->progressCount(s_CurrentWJ);
    
    if (s_downTime != 0)
    {
        s_downSpeed = s_iCurrentFileSize * 1.0 / s_downTime;
        //CCLOG("s_downSpeed = %lu",s_downSpeed);
    }
    return written;
}

size_t downHeadCallBack(char *ptr, size_t size,size_t nmemb, void *stream)
{
    char buff[512] = {0};
    std::string pketword = "Content-Length: ";
    int i = 0;
    if(!ptr)
    {
        CCLOG("packet read error! \r\n");
        return size * nmemb;
    }
    
    while((ptr[i] == pketword[i])&&(ptr[i] != ' '))
    {
        i++;
    }
    
    if(ptr[i] == pketword[i])
    {
        //第一段到空格结束前的内容都忽略掉，第二段从空格开始换行结束
        sscanf((char*)ptr, "%*[^ ] %[^\r\n]", buff);
        //CCLOG("Size=%s\r\n",buff);
        s_iFileSize = atoi(buff);
        return size * nmemb;
    }
    else
    {
        return size * nmemb;
    }
}

// 开始现在资源文件
void* UpdateRes::downFile(void *r)
{
    // 计算一下重新下载的次数  如果超过3次就算了
    int reCount = 1;
    
    while(1){
        int count = UpdateRes::getInstance()->getCurrentCount();
        if (count >= UpdateRes::getInstance()->vDownFile.size())
        {
            if(UpdateRes::getInstance()->vAgainDownFile.size() > 0) {
                CCLOG("有%lu个下载失败了,要重新来",UpdateRes::getInstance()->vAgainDownFile.size());
                reCount--;
                if(reCount <= 0){
                    CCLOG("已经重新下载了3次  算了吧  估计没心了  更新失败");
                    for (int i=0;i<UpdateRes::getInstance()->vAgainDownFile.size();i++){
                        MSG_DOWNFINE errMsg=UpdateRes::getInstance()->vAgainDownFile[i];
                        CCLOG("   --->ERR: 下载失败的资源:%s",errMsg.path.c_str());
                    }
                    UpdateRes::getInstance()->setIsupload(false);
                    break;
                }

                UpdateRes::getInstance()->setCurrentCount(0);
                UpdateRes::getInstance()->vDownFile.erase(UpdateRes::getInstance()->vDownFile.begin());
                UpdateRes::getInstance()->vDownFile = UpdateRes::getInstance()->vAgainDownFile;
                UpdateRes::getInstance()->vAgainDownFile.erase(UpdateRes::getInstance()->vAgainDownFile.begin());

                continue;
            }
            // 没有问题 全部更新完成 正常模式  可以进行游戏了
            UpdateRes::getInstance()->setIsupload(true);
            break;
        }
        MSG_DOWNFINE msg = UpdateRes::getInstance()->vDownFile[count];
        UpdateRes::getInstance()->judgDownload(msg);
    }
    
    curl_easy_cleanup(s_pCurl);
    pthread_exit(NULL);
    return NULL;
}

void UpdateRes::judgDownload(MSG_DOWNFINE msg)
{
    //if(1==1)break;
    std::string pszPath = localPathWrith + msg.path.c_str();
    UpdateRes::getInstance()->createTargetFile(pszPath);
    
    bool isSave = false;
    if((s_pfileWrite = fopen(pszPath.c_str(),"w+")) == NULL) {
        CCLOG("libcurl init error! \n");
        CCLOG("pszPath=%s",pszPath.c_str());
        CCLOG("错误文件:%s",msg.path.c_str());
        isSave = false;
    } else {
        if (s_pCurl) {
            curl_easy_reset(s_pCurl);
        } else {
            s_pCurl = curl_easy_init();
        }
        
        isSave = true;
        std::string rUrl = msg.url.c_str();
        // CCLOG("下载文件 = %s",rUrl.c_str());
        
        curl_easy_setopt(s_pCurl, CURLOPT_TIMEOUT, 120);//设置超时时间
        curl_easy_setopt(s_pCurl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(s_pCurl, CURLOPT_URL, rUrl.c_str());//设置URL
        curl_easy_setopt(s_pCurl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);//设置URL的版本
        curl_easy_setopt(s_pCurl, CURLOPT_WRITEDATA, &s_pfileWrite);//设置写数据的文件指针
        curl_easy_setopt(s_pCurl, CURLOPT_WRITEHEADER,NULL);//设置写头文件的指针，这里要设置为NULL，否则有问题
        curl_easy_setopt(s_pCurl, CURLOPT_WRITEFUNCTION, downWriteCallBack);//设置数据写回调
        curl_easy_setopt(s_pCurl, CURLOPT_HEADERFUNCTION, downHeadCallBack);//设置头数据写回调
        curl_easy_setopt(s_pCurl, CURLOPT_VERBOSE, false);//打开联网log，发布的时候可以关闭
        curl_easy_perform(s_pCurl);//联网请求，会停留在这里直到完成
        //curl_easy_cleanup(s_pCurl);//关闭CURL
        fclose(s_pfileWrite);//关闭文件句柄
    }
    
    CCLOG("文件=%s size=%lu 下载的size=%lu",msg.path.c_str(),(long)msg.size,s_CurrentWJ);
    
//    if((long)msg.size == s_CurrentWJ) {
//        // 文件下载成功了
//        //CCLOG("OY~  下载成功一个\n");
//        
//    } else {
//        // 文件大小验证不一样 下载失败了
//        CCLOG("OY~NO~~ %s 尺寸验证失败 不算更新成功  网络尺寸:%lu  下载尺寸:%lu\n",msg.path.c_str(),(long)msg.size,s_CurrentWJ);
//        
//        // 下载失败了  把文件在存在一个缓存里  一会在下载一次试试
//        UpdateRes::getInstance()->vAgainDownFile.push_back(msg);
//    }
    UpdateRes::getInstance()->setCurrentCount(UpdateRes::getInstance()->getCurrentCount()+1);
    
    s_CurrentWJ = 0;
    s_downTime = 0;
    s_iCurrentFileSize = 0;
}

// 创建路径 判断然后没有创建 有了就算了
void UpdateRes::createTargetFile(std::string path)
{
    int e_pos = path.length();
    int f_pos = path.find("/",0);
    std::string subdir;
    do
    {
        e_pos = path.find("/",f_pos+1);
        if(e_pos != -1)
        {
            subdir = path.substr(0,e_pos);
            if(mkdir(subdir.c_str(),S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)==0)
            {
                CCLOG("%s目录创建成功",subdir.c_str());
            }
        }
        f_pos = e_pos;
    }while(f_pos!=std::string::npos);
}

// 计时器函数  下载资源
void UpdateRes::tickTime(float dt)
{
    
    if(getCurrentCount() >= this->vDownFile.size()){
        // 下载完成了  关闭计时器
        Scheduler* scheduler = Director::getInstance()->getScheduler();
        scheduler->unscheduleSelector(schedule_selector(UpdateRes::tickTime),
                                      this);

        if (_delegate) {
            if(this->getIsupload()){
                // 正常更新完成了
                // 保存一下服务器给的版本号
//                UD->setStringForKey(RESDOWNVERKEY, serverVersion.c_str());
//                UD->flush();
                // 通知界面切换
                _delegate->updateResEnd();
            }else{
                // 异常了  有文件更新失败了
                _delegate->errProcess(errCode_uploadErr, "");
            }
        }
    } else {
        if (_delegate) {
            // 单个文件的大小和单个文件的下载进度
            _delegate->curentResProcess(this->getCurrentCount(), this->vDownFile.size(), s_CurrentWJ, s_iFileSize);
            // 整个文件的大小和整个文件的下载进度
            //_delegate->curentResProcess(this->getCurrentCount(), this->vDownFile.size(),s_fullSizeWJ,this->totalSize);
        }
    }
}

void UpdateRes::starUpdate(std::string url)
{
    // 清除 所有缓存数据先
    // 这里面是所有要下载的东西
    if(vDownFile.size() > 0) {
        vDownFile.erase(vDownFile.begin());
    }
    if(vAgainDownFile.size() > 0) {
        vAgainDownFile.erase(vAgainDownFile.begin());
    }
    
    // 请求服务获取需要更新的json版本
    //std::string url = IP_URL_UPDATA;
    //    CCLOG("url = %s",url.c_str());
    //    sendHTTP(url, "", callfuncND_selector(UpdateRes::needUpdataJson));
    
    std::string phontPath = "res/data/";
    std::string str[] = {
        "TFormatinfo.bin",
        "ActionInfo.bin",
        "AnimationInfo.bin",
        "ArmsInfo.bin",
        "CardInfo.bin",
        "TipInfo.bin",
        "EffectInfo.bin",
        "SkillInfo.bin",
        "FormulaParam.bin"
    };
    for (int i = 0; i<9; i++){
        std::string p = StringUtils::format("%s%s",phontPath.c_str(),str[i].c_str());
        std::string u = StringUtils::format("%s%s",url.c_str(), str[i].c_str());
        CCLOG("p = %s   u = %s",p.c_str(),u.c_str());
        MSG_DOWNFINE msg = {p,0,u};
        vDownFile.push_back(msg);
    }
    this->doDownFile();
}
