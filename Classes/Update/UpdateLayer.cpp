//
//  UpdateLayer.cpp
//  SuperWarriors
//
//  Created by lequ on 14-10-20.
//
//

#include "HelloWorldScene.h"

#include "UpdateLayer.h"
#include "LQConfig.h"

#define UD UserDefault::getInstance() // UserDefault 实例

// 存储资源版本的key值
#define RESDOWNVERKEY "RESVERSIONKEY"

//Scene* UpdateLayer::scene()
//{
//    Scene *scene = Scene::create();
//    UpdateLayer *layer = UpdateLayer::create();
//    scene->addChild(layer);
//    return scene;
//}

UpdateLayer* UpdateLayer::create()
{
    UpdateLayer *ret = new UpdateLayer();
    if (ret && ret->init()){
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

UpdateLayer::~UpdateLayer()
{
    
}

bool UpdateLayer::init()
{
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(UpdateLayer::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(UpdateLayer::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(UpdateLayer::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    
    // 初始化UI
    this->initUI();
    
    // 启动下载
    
    return true;
}

void UpdateLayer::setLatestVersion(std::string url,int var)
{
    downUrl = url;
    latestVersion = var;
}

void UpdateLayer::starDownload()
{
    UpdateRes *update = UpdateRes::getInstance();
    update->setDelegate(this);
    update->starUpdate(downUrl);
}

void UpdateLayer::starReleaseZIP()
{
//    zipOperateEnd = false;
//    auto releaseWritePath = FileUtils::getInstance()->getWritablePath();
//    std::string zip_path = FileUtils::getInstance()->fullPathForFilename("res/"+RELEASEZIPNAME).c_str();
//    if(!FileUtils::getInstance()->isFileExist("res/"+RELEASEZIPNAME)){
//        CCLOG("坑爹啊，压缩文件不存在");
//        this->errProcess(errCode_releaseZipFileNotFound, "");
//    }else{
//        labelDownloadInfo ->setString("解压准备中，请稍等。。");
//        LQZipUtil* lqzip = LQZipUtil::getInstance();
//        lqzip->setDelegate(this);
//        lqzip->unzipOpenFile(zip_path,LQZIPPASSWORD);
//        lqzip->releaseThreadStart();//启用新线程开始解压文件
//        
//        
//        
//    }
}
bool UpdateLayer::onTouchBegan(Touch* touch, Event  *event)
{
//    Scene *scene = StartLayer::createScene();
//    Director::getInstance()->replaceScene(scene);
    if(zipOperateEnd){//如果已经解压完成，那么从父节点移除
        this->removeFromParent();
    }
    return true;
}
void UpdateLayer::onTouchMoved(Touch* touch, Event  *event){}
void UpdateLayer::onTouchEnded(Touch* touch, Event  *event){}
void UpdateLayer::onTouchCancelled(Touch *touch, Event *event){}


void UpdateLayer::initUI()
{
    Size visibleSize =  Director::getInstance()->getVisibleSize();
    
    const Size GAMESCENE = designResolutionSize;
//    std::string LTJM_PLIST = "files/ccbiui/ccbires/resv3/LOGIN.plist";
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST);
//    SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("beijing.png");
    Sprite *loginBG = Sprite::create("HelloWorld.png");
    loginBG->setPositionX(-(visibleSize.width- GAMESCENE.width)/2);
    loginBG->setPositionY(-(visibleSize.height-GAMESCENE.height)/2);
    loginBG->setScaleX(visibleSize.width / GAMESCENE.width);
    loginBG->setScaleY(visibleSize.height / GAMESCENE.height);
    loginBG->setAnchorPoint(Point(0.5, 0.5));
    loginBG->setPosition(Point(GAMESCENE.width/2, GAMESCENE.height/2));
    this->addChild(loginBG);
    
    Sprite *pSpriteBG = Sprite::create("CloseNormal.png");
    pSpriteBG->setAnchorPoint(Point(0.5, 0));
    pSpriteBG->setPosition(Point(visibleSize.width/2,0));
    this->addChild(pSpriteBG);
    Sprite *pline = Sprite::create("CloseSelected.png");
    //int lineWidth = pline->boundingBox().size.width;
    progress = ProgressTimer::create(pline);
    progress->setBarChangeRate(Point(1, 0));    //设置进度条的长度和高度开始变化的大小
    progress->setMidpoint(Point(0, 0));
    progress->setType(ProgressTimer::Type::BAR);    //设置进度条为水平
    progress->setPercentage(0.0f);    //设置初始百分比的值
    progress->setPosition(Point(pSpriteBG->getContentSize().width/2,
                                pSpriteBG->getContentSize().height/2));    //放置进度条位置
    pSpriteBG->addChild(progress);    //加入Layer中
    
    
    // 添加显示下载信息的label
    labelDownloadInfo = Label::createWithSystemFont("", "Helvetica", 30);
    labelDownloadInfo->setPosition(Point(visibleSize.width/2, 100));
    labelDownloadInfo->setAnchorPoint(Point(0.5, 0.5));
    labelDownloadInfo->setAlignment(TextHAlignment::CENTER);
    this->addChild(labelDownloadInfo);
    
    //  显示资源的版本号 当前版本和服务器版本
    labelVersion = Label::createWithSystemFont("", "Helvetica", 20);
    labelVersion->setPosition(Point(visibleSize.width-5,visibleSize.height-5));
    labelVersion->setAnchorPoint(Point(1,1));
    this->addChild(labelVersion);
}

void UpdateLayer::onEnter()
{
    Layer::onEnter();
}
void UpdateLayer::onExit()
{
    Layer::onExit();
}

// 显示更细资源的版本好
void UpdateLayer::pushSVersion(std::string currentVer,std::string targetVer)
{
    std::string sv=StringUtils::format("当前版本:%s",currentVer.c_str());
    std::string tv="";
    if (targetVer.compare("") != 0) {
        tv = StringUtils::format("最新版本:%s",targetVer.c_str());
    }
    labelVersion->setString(StringUtils::format("%s\n%s",sv.c_str(),tv.c_str()));
}
// 显示现在的异常信息
void UpdateLayer::errProcess(int errCode, std::string errInfo)
{
    if (errCode == errCode_getMainJson) {
        labelDownloadInfo->setString("请求更新文件列表失败");
    }else if(errCode == errCode_uploadErr) {
        labelDownloadInfo->setString("更新失败...");
    }else if(errCode == errCode_upversion) {
        labelDownloadInfo->setString("需要更新客户端了");
    }else if(errCode == errCode_releaseZipError){
        labelDownloadInfo->setString("文件解压失败:"+errInfo);
    }else if (errCode == errCode_releaseZipFileNotFound){
        labelDownloadInfo->setString("找不到资源文件，请重新下载安装");
    }
}

// UpdateRes 获得下载进度用来显示用
void UpdateLayer::curentResProcess(int currentNumber,int totalNumber, unsigned long currentBytes, unsigned long totalBytes)
{
    labelDownloadInfo->setString(StringUtils::format("(%lu/%lu)\n%d/%d",currentBytes,totalBytes,currentNumber,totalNumber));
    
    float value = float(currentBytes)/float(totalBytes);
    progress->setPercentage((value)*100);
}

// 当前的解压进度
void UpdateLayer::curentReleaseProcess(int currentNumber,int totalNumber)
{
    int percentnum = currentNumber*100/totalNumber;
    labelDownloadInfo->setString(StringUtils::format("%d %% 解压中",percentnum));
    //    labelDownloadInfo->setString(StringUtils::format("%d/%d",currentNumber,totalNumber));
    
    float value = float(currentNumber)/float(totalNumber);
    progress->setPercentage((value)*100);
}
//解压完成了
void UpdateLayer::releaseZIPEnd()
{
    labelDownloadInfo->setString("文件解压完成,请点击继续。");
    zipOperateEnd = true;
    
    CCLOG("release zip end");
   // this->removeFromParent();
}

// 更新完成了
void UpdateLayer::updateResEnd()
{
    labelDownloadInfo->setString("文件更新完成");
    
//    std::string latestVer = StringUtils::format("%d",latestVersion);
//    UD->setStringForKey(RESDOWNVERKEY, latestVer);
//    UD->flush();
    
    //this->removeFromParent();
    
//    auto delay = DelayTime::create(3);
//    auto callFunc = CallFunc::create(CC_CALLBACK_0(UpdateLayer::sequenceToGame,this));
//    self:runAction(Sequence::create(delay,callFunc, NULL));
    
    
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(UpdateLayer::menuCloseCallback, this));
//    
//    closeItem->setPosition(Point(100, 100));
//    
//    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Point::ZERO);
//    this->addChild(menu, 1);
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);

    
}
                           
void UpdateLayer::menuCloseCallback(cocos2d::Ref* pSender)
{
//    auto scene = HelloWorld::createScene();
//    Director::getInstance()->replaceScene(scene);

}
