//
//  UpdateLayer.h
//  SuperWarriors
//
//  Created by lequ on 14-10-20.
//
//

#ifndef __SuperWarriors__UpdateLayer__
#define __SuperWarriors__UpdateLayer__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#include "UpdateRes.h"
#include "LQConfig.h"

class UpdateLayer : public Layer ,public UpdateResDelegate
{
public:
    CC_SYNTHESIZE(int, _showType, showType);// 展示类别，1是更新 2是解压
    static UpdateLayer *create();
    ~UpdateLayer();
    bool init();
//    virtual bool init();
//    static cocos2d::Scene* scene();
// 
//    CREATE_FUNC(UpdateLayer);
    
    void setLatestVersion(std::string url,int var); // 设置最新的资源版本
    void starDownload(); // 开始下载
    void starReleaseZIP(); // 开始解压ZIP包
    
private:
    std::string downUrl; // 下载的url
    int latestVersion; // 资源升级的最新版本
    bool zipOperateEnd;//解压是否完成
    
    
    // 进度条
    ProgressTimer *progress;
    // 显示下载进度的label
    Label *labelDownloadInfo;
    // 显示资源的版本号 当前版本和服务器版本
    Label *labelVersion;
    
    
    // 初始化UI
    void initUI();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual bool onTouchBegan(Touch* touch, Event  *event) override;
    virtual void onTouchMoved(Touch* touch, Event  *event) override;
    virtual void onTouchEnded(Touch* touch, Event  *event) override;
    virtual void onTouchCancelled(Touch *touch, Event *event) override;
   
    // updateRes回调处理
    void errProcess(int errCode, std::string errInfo); // 显示异常信息
    void curentResProcess(int currentNumber,int totalNumber, unsigned long currentBytes, unsigned long totalBytes); // 显示下载进度
    void curentReleaseProcess(int currentNumber,int totalNumber); // 显示解压
    void releaseZIPEnd(); // 解压完成了
    void updateResEnd(); // 更新完成了
    //参数 当前的版本号 要更新到的版本号
    void pushSVersion(std::string currentVer,std::string targetVer);

    void menuCloseCallback(cocos2d::Ref* pSender);
};

#endif /* defined(__SuperWarriors__UpdateLayer__) */
