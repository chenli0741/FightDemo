//
//  DownLoad.h
//  test
//
//  Created by lequ on 14-2-19.
//
//

#ifndef __test__UpdateRes__
#define __test__UpdateRes__

#include "cocos2d.h"
#include "../cocos2d/external/curl/include/ios/curl/curl.h"
#include "../extensions/cocos-ext.h"
#include "LQConfig.h"

#include "../extensions/cocos-ext.h"
#include "../network/HttpClient.h"
#include "../network/HttpRequest.h"

#include "../external/json/document.h"
#include "../external/json/writer.h"
#include "../external/json/stringbuffer.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d;
using namespace network;

enum enumErrCode
{
    errCode_getMainJson = 1,    // 请求更新文件列表失败
    errCode_uploadErr,          // 更新失败了 有文件未完成更新
    errCode_upversion,           // 不用更新了  直接下载客户端吧
    errCode_releaseZipError,    //解压ZIP出错
    errCode_releaseZipFileNotFound,    //ZIP文件木有，坑爹
};

// 存储下载文件结构
struct MSG_DOWNFINE {
    std::string path;   // 路径
    int size;           // 大小
    std::string url;    // 下载地址
};

class UpdateResDelegate
{
public:
    virtual void errProcess(int errCode, std::string errInfo) = 0;
    virtual void curentResProcess(int currentNumber,int totalNumber, unsigned long currentBytes, unsigned long totalBytes) = 0;
    virtual void curentReleaseProcess(int currentNumber,int totalNumber) = 0;
    virtual void updateResEnd() = 0;
    virtual void releaseZIPEnd() = 0;
    virtual void pushSVersion(std::string currentVer,std::string targetVer) = 0;
};


class UpdateRes:public Ref
{
public:
    static UpdateRes* getInstance();
    CC_SYNTHESIZE(UpdateResDelegate*, _delegate, Delegate);// 重新delegate set get方法
    void starUpdate(std::string url); // 开始更新
private:
    UpdateRes();
    ~UpdateRes();
    
    // 需要更新文件的总大小
    int totalSize;
    // 当期现在到第几个了
    CC_SYNTHESIZE(int, currentCount, CurrentCount);
    // 是否正常更新完成了
    CC_SYNTHESIZE(bool, isupload, Isupload);
    // 主URL
    CC_SYNTHESIZE(std::string, mainURL, MainURL);
    // 当前资源的版本号
    CC_SYNTHESIZE(std::string, currentVersion, CurrentVersion);
    // 服务器得到的资源更新版本
//    CC_SYNTHESIZE(std::string, serverVersion, ServerVersion);
    // 发送http请求统一入口
    void sendHTTP(std::string url,std::string postContent,SEL_CallFuncND  callBack);
    // 请求需要更新文件json所返回的数据
    void needUpdataJson(Node *sender, void *data);
    
    // 保存需要下载文件结构
    std::vector<MSG_DOWNFINE> vDownFile;
    // 保存下载失败的文件结构 下载失败了 然后放到这里 在下载一次
    std::vector<MSG_DOWNFINE> vAgainDownFile;
    
    // 开始下载文件方法 并 判断是否需要下载资源文件
    void doDownFile();
    // tickTime 计时器函数 绘制下载进度 主线程
    void tickTime(float dt);
    // 创建路径
    void createTargetFile(std::string path);
    //多线程下载，并且实时返回进度
    pthread_t pid;
    // 开启下载线程
    void requestResouceContent(const char* url);
    static void* downFile(void *r);
    // 下载文件处理 如果下载失败 在重新下载一次
    void judgDownload(MSG_DOWNFINE msg);
    
    // 删除文件操作
    void removeFile(char *file);
    void rm_file(char *path);
    void rm_dir(char *path);
};

#endif /* defined(__test__UpdateRes__) */
