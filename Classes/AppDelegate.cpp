#include "AppDelegate.h"

#include "HelloWorldScene.h"
#include "ZipUtils.h"

#include "UpdateRes.h"
#include "UpdateLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    ZipUtils::setPvrEncryptionKey(0x0315c680, 0x11a408b1, 0xf21417ad, 0xc923d44b);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("SuperWarriors",Rect(0,0,designResolutionSize.width,designResolutionSize.height));
        director->setOpenGLView(glview);
    }
    
    auto winSize = director->getWinSize();
    auto frameSize = glview->getFrameSize();
    //auto winSizeInPixels = director->getWinSizeInPixels();
    
    float ratioW = frameSize.width / designResolutionSize.width;
    float ratioH = frameSize.height / designResolutionSize.height;
    float width = frameSize.width * ratioH;
    float height = frameSize.height * ratioW;
    float twidth = designResolutionSize.width * ratioH;
    float theight = designResolutionSize.height * ratioH;
    
    if (theight == frameSize.height){
        if (twidth > frameSize.width){
            width = frameSize.width;
            height = theight * frameSize.width / twidth;
            // this ratio equals to design
        } else {
            height = theight;
            width = frameSize.width;
        }
    }
    
    if (width / height == designResolutionSize.width / designResolutionSize.height){
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    } else {
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    }
    
    CCLOG("ContentScaleFactor: %g, %g", glview->getScaleX(), glview->getScaleY());
    
    auto point = director->getVisibleOrigin();
    auto size = director->getVisibleSize();
    CCLOG("visible origin: x=%g,y=%g, visible size: width=%g,height=%g", point.x, point.y, size.width, size.height);
    
    UserDefault::getInstance()->setIntegerForKey("UD_MUSICBG_KEY",0);
    UserDefault::getInstance()->setIntegerForKey("UD_SOUND_KEY",0);
    
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = Scene::create();

    static bool isFirst = false;
    if(isFirst){
        isFirst = false;
        std::vector<std::string> searchPaths = FileUtils::getInstance()->getSearchPaths();
        std::vector<std::string>::iterator iter = searchPaths.begin();
        searchPaths.insert(iter, FileUtils::getInstance()->getWritablePath());
        FileUtils::getInstance()->setSearchPaths(searchPaths);
        
        UpdateLayer *layer = UpdateLayer::create();
        layer->setshowType(1);//设置展示类别为下载
        layer->setLatestVersion("http://192.168.1.8/SWSKILLTEST/down/testdata/",1);
        scene->addChild(layer,1000);
        layer->starDownload();
        //auto scene = HelloWorld::createScene()
    }else{
        scene = HelloWorld::createScene();
    }
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
