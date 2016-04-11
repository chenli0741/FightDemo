//
//  LQPoltLayer.cpp
//  SuperWarriors
//
//  Created by lequ on 14-9-18.
//
//

#include "LQPoltLayer.h"
#include "LQBattleSession.h"
#include "LQHelp.h"
#include "LQCastListInfoData.h"
#if !defined(LQ_DEMO_FIGHT)
#include "HclcData.h"
#endif
using namespace cocosbuilder;
USING_NS_CC;

cocos2d::SEL_MenuHandler LQPoltLayer::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char * pSelectorName)
{
    return NULL;
}

cocos2d::extension::Control::Handler LQPoltLayer::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char * pSelectorName)
{
    
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBackButtonClicked", LQPVEWinLayer::onBackButtonClicked);
    return NULL;
}

bool LQPoltLayer::onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * pNode)
{
    log("绑定 node === %s",pMemberVariableName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nameLabel", Label *, nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "valueLabel", Label *, valueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "dirLayer", Layer *, dirLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "taskBG", Layer *, taskBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "downSP", Sprite *, downSP);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layerAni", Layer *, layerAni);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "touchLayer", Layer *, touchLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bottomNode", Layer *, bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "dixia", Scale9Sprite *, dixia);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "dishang", Scale9Sprite *, dishang);
    
    return true;
}

void LQPoltLayer::onNodeLoaded(cocos2d::Node * node, cocosbuilder::NodeLoader * nodeLoader){}


Scene* LQPoltLayer::createScene()
{
    Scene * scene = Scene::create();
    LQPoltLayer* layer = LQPoltLayer::create();
    layer->initLayer();
    scene->addChild(layer);
    return scene;
}

LQPoltLayer* LQPoltLayer::createLayer()
{
    LQPoltLayer* layer = LQPoltLayer::create();
    layer->initLayer();
    return layer;
}
bool LQPoltLayer::initLayer()
{
    m_triggerEvent = "";
    
    NodeLoaderLibrary *lib = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    cocosbuilder::CCBReader *reader = new cocosbuilder::CCBReader(lib);
#if defined(LQ_DEMO_FIGHT)
    reader->setCCBRootPath("files/");
#endif
    lib->registerNodeLoader("LQPoltLayer", LayerLoader::loader());
    Node *node = reader->readNodeGraphFromFile("files/ccbiui/polt/PoltC.ccbi",this);
    reader->autorelease();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    node->setAnchorPoint(Point(0.5, 0.5));
    node->ignoreAnchorPointForPosition(false);
    node->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(node);
    

    
    return true;
}

void LQPoltLayer::onEnter()
{
    Layer::onEnter();
}
void LQPoltLayer::onExit()
{
    Layer::onExit();
}
void LQPoltLayer::initUI()
{
}

bool LQPoltLayer::getIsThereAre(int pid)
{
    pidKey = pid;
    strKey = StringUtils::format("POLT_MODE_KEY%d",pidKey);
    int ret = UserDefault::getInstance()->getIntegerForKey(strKey.c_str());
    if(ret == 0) {
        return true;
    }else{
        return false;
    }
}

void LQPoltLayer::setPolt(std::string value)
{
//    CCLOG("setPolt value:%s", value.c_str());
    string awardStr = value;
    std::vector<std::string> Award3;
    split(awardStr, ';', Award3);
    for (int i = 0; i < Award3.size(); i++) {
        string items = Award3.at(i);
        CCLOG("items:%s", items.c_str());
        std::vector<std::string> vItem;
        split(items, '@', vItem);
        //for (int j = 0; j < vItem.size(); j++) {
            int ptypes = atoi(vItem[0].c_str());
            string value = vItem[1];
                    int cardid = atoi(vItem[2].c_str());
            string name = "";
            string headname = "";
            if(cardid == 41009999) {  // || cardid == 40109999
                //std::string sindex=HclcData::getInstance()->getLuaVarString("playInitData");
                //int snumber = std::atoi(sindex.c_str());
                int snumber = UserDefault::getInstance()->getIntegerForKey("INITCARDID");
                if(snumber == 1 || snumber == 2 || snumber == 3) {
                    if(snumber == 1)  cardid = 41000036; // 张辽 90010037
                    else if(snumber == 2)  cardid = 41000051; // 孙策 90010052
                    else if(snumber == 3)  cardid = 41000017; // 关羽 90010018
                    //LQCardInfoData* m_heroData = (LQCardInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQCardInfoData::getAliasName(),cardid);
                    LQCastListInfoData* m_heroData = (LQCastListInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQCastListInfoData::getAliasName(),cardid);
                    name = m_heroData->getName();
                    headname = m_heroData->getCardID();
                }else{
                    name = "我";
                    headname = "card-lvbu.png";
                }
            }else if(cardid == 41009998) {
                int snumber = UserDefault::getInstance()->getIntegerForKey("INITCARDID");
                if(snumber == 1 || snumber == 2 || snumber == 3) {
                    if(snumber == 1)  cardid = 41000047; // 曹丕
                    else if(snumber == 2)  cardid = 41000062; // 凌统
                    else if(snumber == 3)  cardid = 41000027; // 关平
                    //LQCardInfoData* m_heroData = (LQCardInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQCardInfoData::getAliasName(),cardid);
                    LQCastListInfoData* m_heroData = (LQCastListInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQCastListInfoData::getAliasName(),cardid);
                    name = m_heroData->getName();
                    headname = m_heroData->getCardID();
                }else{
                    name = "副将";
                    headname = "card-lvbu.png";
                }
            } else {
                //LQCardInfoData* m_heroData = (LQCardInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQCardInfoData::getAliasName(),cardid);
                LQCastListInfoData* m_heroData = (LQCastListInfoData *)LQDataCache::sharedLQDataCache()->lqDataByName(LQCastListInfoData::getAliasName(),cardid);
                name = m_heroData->getName();
                headname = m_heroData->getCardID();
            }
            int dir = atoi(vItem[3].c_str());

            MSG_PDATA msg = {ptypes,value,cardid,name,headname,dir};
            m_pdata.push_back(msg);
        //}
    }
    /**
    for (int i = 0;i<m_pdata.size();i++) {
        int types = m_pdata[i].types;
        string value = m_pdata[i].value;
        int cardid = m_pdata[i].cardid;
        string name = m_pdata[i].name;
        string headname = m_pdata[i].headname;
        int dir = m_pdata[i].dir;
        CCLOG("value = %s",value.c_str());
    }
    **/ 
    Size visibleSize = Director::getInstance()->getVisibleSize();
    int bardHeiht = 165;
    int bardHeiht2= 165;
    int pwidth = visibleSize.width;
    
    int posx = (1024-pwidth)/2;
    
    Scale9Sprite * layer1=dixia;//LayerColor::create(Color4B(0,0,0,255),visibleSize.width+500,bardHeiht); //上
    layer1->setContentSize(Size(pwidth,bardHeiht));
    layer1->setAnchorPoint(Point(0,0.5));
    layer1->setPosition(Point(posx,visibleSize.height+bardHeiht/2));
    //layerAni->addChild(layer1);
    layer1->setFlippedY(true);
    layer1->setVisible(true);
    Scale9Sprite* layer2=dishang;//LayerColor::create(Color4B(0,0,0,255),visibleSize.width+500,bardHeiht); //上
    layer2->setContentSize(Size(pwidth,bardHeiht2));
    layer2->setAnchorPoint(Point(0,0));
    layer2->setPosition(Point(posx,-bardHeiht2));
    //layerAni->addChild(layer2);
    layer2->setVisible(true);
    auto move = MoveTo::create(0.8,Point(posx,visibleSize.height-bardHeiht+150));
    auto action = Sequence::create(move,CallFunc::create(CC_CALLBACK_0(LQPoltLayer::aniCallback,this)),NULL);
    layer1->runAction(action);
    layer2->runAction(MoveTo::create(0.8,Point(posx,0)));
    // 整个场景渐变进来
//    this->setOpacity(0);
//    FadeIn *fade = FadeIn::create(0.5);
//    this->runAction(fade);
//    // 整个场景渐变进来
//    auto mto1 = MoveTo::create(0.4,Point(932.0,60.0));
//    auto mto2 = MoveTo::create(0.4,Point(932.0,70.0));
//    auto seqMove = Sequence::create(mto1,mto2,NULL);
//    downSP->runAction(RepeatForever::create(seqMove));

    
    
}

void LQPoltLayer::aniCallback()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(LQPoltLayer::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LQPoltLayer::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LQPoltLayer::onTouchEnded, this);
    touchListener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    this->setNextPolt();
}

void LQPoltLayer::setNextPolt()
{
    if(sessionIndex >= m_pdata.size()) {
        // 到达最后一句了
        return;
    }
    nameLabel->setString(m_pdata[sessionIndex].name);
    valueLabel->setString(m_pdata[sessionIndex].value);

    dirLayer->removeAllChildren();
    dirLayer->setAnchorPoint(Point(0.5,0.5));
    Sprite *spHead = Sprite::create(StringUtils::format("res/cards/%s",m_pdata[sessionIndex].headname.c_str()).c_str() );
    spHead->setTag(10);
    spHead->setScale(1.3);
    spHead->setAnchorPoint(Point(0.5,0));
    dirLayer->addChild(spHead);
//    Sprite *spHeadAni = Sprite::create(StringUtils::format("res/cards/%s",m_pdata[sessionIndex].headname.c_str()).c_str() );
//    spHeadAni->setAnchorPoint(Point(0.5,0));
//    dirLayer->addChild(spHeadAni);
    
    taskBG->setVisible(true);
    dirLayer->setVisible(true);
    
    float time = 0.3;

    if(m_pdata[sessionIndex].dir == 0) { //无
        taskBG->setVisible(false);
        dirLayer->setVisible(false);
    }else if(m_pdata[sessionIndex].dir == 1) { //左
//        dirLayer->setPosition(195.0, dirLayer->getPositionY());
        taskBG->setPosition(80, taskBG->getPositionY());
        if(beforeDir != m_pdata[sessionIndex].dir){
            dirLayer->setPosition(0, dirLayer->getPositionY());
            dirLayer->runAction(MoveTo::create(time,Point(195.0,dirLayer->getPositionY())));
            spHead->setOpacity(10);
            spHead->runAction(FadeIn::create(time));
            bottomNode->setPosition(0, -200);
            bottomNode->runAction(MoveTo::create(time, Point(0, 0)));
        }else{
            dirLayer->setPosition(195.0, dirLayer->getPositionY());
            bottomNode->setPosition(0, 0);
        }
    }else if(m_pdata[sessionIndex].dir == 2) { //右
//        dirLayer->setPosition(1024, dirLayer->getPositionY());
        taskBG->setPosition(780, taskBG->getPositionY());
        if(beforeDir != m_pdata[sessionIndex].dir){
            dirLayer->setPosition(1024, dirLayer->getPositionY());
            dirLayer->runAction(MoveTo::create(time,Point(832.0,dirLayer->getPositionY())));
            spHead->setOpacity(10);
            spHead->runAction(FadeIn::create(time));
            bottomNode->setPosition(0, -200);
            bottomNode->runAction(MoveTo::create(time, Point(0, 0)));
        }else{
            dirLayer->setPosition(832.0, dirLayer->getPositionY());
            bottomNode->setPosition(0, 0);
        }
    }
    beforeDir = m_pdata[sessionIndex].dir;


//    spHeadAni->setScale(1.5);
//    spHeadAni->setOpacity(150.0);
//    auto scaleTo =  ScaleTo::create(0.2,1.0);
//    auto fade =  FadeOut::create(0.2);
//    auto action =  Spawn::create(scaleTo, fade, NULL);
//    auto seq =  Sequence::create(action ,RemoveSelf::create() ,NULL);
//    spHeadAni->runAction(seq);
    
    sessionIndex++;
}

void LQPoltLayer::saveThePoltId()
{
    UserDefault::getInstance()->setIntegerForKey(strKey.c_str(),pidKey);
    UserDefault::getInstance()->flush();
    
    int indexKey = UserDefault::getInstance()->getIntegerForKey("POLT_MODE_KEY_NUM");
    indexKey++;
    UserDefault::getInstance()->setIntegerForKey("POLT_MODE_KEY_NUM",indexKey);
    UserDefault::getInstance()->flush();
    std::string allkey = StringUtils::format("POLT_MODE_KEY_ALL%d",indexKey);
    UserDefault::getInstance()->setStringForKey(allkey.c_str(), strKey);
    UserDefault::getInstance()->flush();
}

void LQPoltLayer::aniEnd()
{
    saveThePoltId();
    
    if(this->_delegate) {
        this->_delegate->poltEnd(this);
    }
    this->stopAllActions();
    this->removeAllChildren();
    this->removeFromParent();
}

bool LQPoltLayer::onTouchBegan(Touch* touch, Event  *event)
{
    if(sessionIndex >= m_pdata.size()) {
        CCLOG("对话结束了");
        
        if(1==1){
            //aniEnd();
            
            auto delay = DelayTime::create(0.1);
            auto callfunc = CallFunc::create(CC_CALLBACK_0(LQPoltLayer::aniEnd,this));
            auto sequence =  Sequence::create(delay,callfunc,NULL);
            this->runAction(sequence);
            
            return true;
        }
        
        float time = 0.3;
        
        dixia->runAction(MoveTo::create(time, Point(dixia->getPositionX(), Director::getInstance()->getVisibleSize().height)));
        dishang->runAction(MoveTo::create(time, Point(dishang->getPositionX(), 0)));

        Sprite *spHead = (Sprite *)dirLayer->getChildByTag(10);
        if(spHead){
            spHead->setOpacity(255);
            spHead->runAction(FadeOut::create(time));
        }
        if(beforeDir == 1){ // 左边
            dirLayer->runAction(MoveTo::create(time,Point(0,dirLayer->getPositionY())));
        }else if(beforeDir == 2){ // 右边
            dirLayer->runAction(MoveTo::create(time,Point(1024,dirLayer->getPositionY())));
        }
        auto move = MoveTo::create(time, Point(0, -200));
        auto callfunc = CallFunc::create(CC_CALLBACK_0(LQPoltLayer::aniEnd,this));
        auto sequence =  Sequence::create(move,callfunc,NULL);

        bottomNode->runAction(sequence);

        
//        saveThePoltId();
//        
//        if(this->_delegate) {
//            this->_delegate->poltEnd(this);
//        }
//        this->stopAllActions();
//        this->removeAllChildren();
//        this->removeFromParent();
    }else{
        if(bottomNode->getPositionY() >= 0) {
            this->setNextPolt();
        }
    }
    return true;
}
void LQPoltLayer::onTouchMoved(Touch* touch, Event  *event){}
void LQPoltLayer::onTouchEnded(Touch* touch, Event  *event){}
void LQPoltLayer::onTouchCancelled(Touch *touch, Event *event){}
