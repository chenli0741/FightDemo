//
//  LoadFightScene.cpp
//  SuperWarriors
//
//  Created by lequdev1 on 14-8-12.
//
//
#include "LoadFightScene.h"
#include "LQBattleSession.h"
#include "LQConfig.h"
#include "LQTipInfoData.h"

#if defined(LQ_DEMO_FIGHT)
#include "LQTFormatinfoData.h"
#endif

int fighttype = 3;  //0:演示关卡 1: 普通关卡 2: 列传 3: 排行榜挑战 4: 经验关卡 5 良驹争夺 6 奇遇战斗 7:掠夺拦截 8:南蛮入侵
int unittype = 1;

//控制是否装载测试阵型，姚果测试需要
bool islloadformat = true;

#if defined(LQ_DEMO_FIGHT)
int _formatIndex1 = -1;
int _formatIndex2 = -1;
#endif

//90041501 孙策 90041502 关羽 90041503 张辽 90041504 孙尚香 90041505 华雄
//90041506 刘备 90041507 孙坚 90041508 曹操 90041509 ？？   90041510 徐晃

//演示关卡
//int attackHeroList[]  = {-1,90041501,90041502,90041503,-1, -1,-1,-1,90041505,-1, -1,-1,90041504,-1,-1};
//int defenceHeroList[] = {-1,90041506,90041507,90041508,-1, -1,90041509,-1,90041510,-1, -1,-1,-1,-1,-1};
//int attackHeroList[]  = {-1,90041501,90041502,-1,90041503, -1,-1,-1,90041505,-1, -1,-1,-1,90041504,-1};
//int defenceHeroList[] = {90041506,-1,90041507,90041508,-1, -1,-1,-1,90041510,-1, -1,-1,90041509,-1,-1};

//单个武将 90010018 90010019 90010008 90041938
//int attackHeroList[]   = {90042092,90042093,90042094,90042095,90042096, -1,90042097,90042098,90042099,90042086, -1,-1,-1,-1,-1};
//int attackHeroList[]   = {-1,90041917,90042097,-1,-1, -1,-1,-1,-1,-1, -1,-1,90041919,-1,-1};
//int defenceHeroList[]  = {-1,-1,90041917,90041917,-1, -1,-1,-1,-1,-1, -1,-1,90041919,-1,-1};
//落雷的声音 90042006 90041994
//白刃效果 90041962 90041963

//绕后测试
//int attackHeroList[]   = {90041916,90041917,90041914,-1,-1, -1,-1,90041913,-1,-1, -1,-1,-1,-1,90041915};
//int defenceHeroList[]  = {-1,90041921,90041922,90041920,-1, -1,-1,-1,-1,-1, -1,90041919,90041918,-1,-1};

//int attackHeroList[]   = {-1,90042009,90042016,90042009,-1, -1,90042009,-1,90042009,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[]  = {-1,90042009,90042015,90042009,-1, -1,90042009,-1,90042009,-1, -1,-1,-1,-1,-1};

//声音测试-蜀5骑
//int attackHeroList[]   = {-1,90042009,90042015,90042009,-1, -1,90042009,-1,90042009,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[]  = {-1,90042009,90042017,90042009,-1, -1,90042009,-1,90042009,-1, -1,-1,-1,-1,-1};

//孙坚测试
//int attackHeroList[]    = {-1,90042009,90042053,90042009,-1, -1,90042009,-1,90042009,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[]   = {-1,90042009,90042015,90042009,-1,- 1,90042009,-1,90042009,-1, -1,-1,-1,-1,-1};

//技能测试24
//int attackHeroList[]  = {-1,90010024,90010019,90010024,-1, -1,90010024,-1,90010024,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {90010018,90010018,90010018,90010018,90010018, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1};
//int attackHeroList[]  = {-1,90010019,90010019,90010019,-1, -1,90010019,-1,90010019,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1,90010019,90010019,90010019,-1, -1,90010019,-1,90010019,-1, -1,-1,-1,-1,-1};
//int attackHeroList[]  = {-1,90010018,90010018,90010018,-1, -1,90010018,-1,90010018,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1,90010018,90010018,90010018,-1, -1,90010018,-1,90010018,-1, -1,-1,-1,-1,-1};

//华雄测试
int attackHeroList[]  = {-1,-1,90041949,-1,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1};   //90041955
int defenceHeroList[] = {-1,90041952,90041950,90041954,-1, -1,-1,90041510,90041510,-1,-1, -1,-1,-1,-1,-1};

//双祝融
//int attackHeroList[]  = {-1,-1,90041924,90041924,-1, -1,-1,90041957,-1,-1, -1,-1,-1,-1,-1};   //90041955
//int defenceHeroList[] = {-1,90041950,90041952,90041953,-1, -1,90041940,90041954,-1,-1, -1,-1,-1,-1,-1};

//阵型冲锋速度测试
//int attackHeroList[]   = {-1,-1,90041501,-1,-1, -1,-1,90041501,-1,-1, -1,90041505,90041501,90041504,-1};
//int defenceHeroList[]  = {-1,-1,90041506,-1,-1, -1,-1,90041502,-1,-1, -1,-1,90041504,-1,-1};

//阵型包抄测试
//int attackHeroList[]   = {-1,-1,90041510,-1,-1, -1,-1,-1,-1,-1, -1,-1,90041504,-1,-1};
//int defenceHeroList[]  = {90041507,-1,90041506,-1,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1};
//int attackHeroList[]   = {-1,-1,90041505,-1,-1, -1,-1,-1,-1,-1, 90041501,-1,-1,-1,90041501};
//int defenceHeroList[]  = {-1,90041510,-1,90041510,-1, -1,-1,-1,-1,-1,  -1,-1,90041504,-1,-1};
//int attackHeroList[]   = {90041505,-1,-1,-1,90041505, -1,-1,-1,-1,-1, -1,-1,90041501,-1,-1};
//int defenceHeroList[]  = {-1,90041510,-1,90041510,-1, -1,-1,-1,-1,-1,  -1,-1,90041504,-1,-1};

//新武将
//int attackHeroList[]  = {-1,90010019,90010009,90010011,-1, -1,90010002,90010010,-1,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1,90010055,90010049,90010063,-1, -1,-1,90010010,90010001,-1, -1,-1,-1,-1,-1};

//BUG0 大乔 OK
//int attackHeroList[]  = {-1,90041950,90041952,90041953,-1, -1,90041951,90041954,-1,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1,-1,90041998,-1,-1, 90041999,90041999,-1,90041999,90041999, -1,-1,-1,-1,-1};

//BUG1 速度关平
//int attackHeroList[]  = {-1,-1,90041924,90041990,-1, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1,90041950,90041952,90041953,-1, -1,90041940,90041951,-1,-1, -1,-1,-1,-1,-1};

//BUG2 骑兵的冲锋伤害公式
//int attackHeroList[]  = {-1,90041996,90041993,90041994,-1, -1,90041995,-1,90041997,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1,-1,90041992,-1,-1, 90041991,90041991,-1,90041991,90041991, -1,-1,-1,-1,-1};

//4.9 BUG1 步兵冲锋的问题
//int attackHeroList[]   = {-1, 90041950,90041952,-1,-1, -1, 90041940, -1,-1,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[]  = {-1, 90041950,-1,-1,-1, -1, 90041951, 90041951,-1,-1, -1,-1,90041951,-1,-1};

//4.9 BUG2 骑兵增加移动速度的问题
//int attackHeroList[]   = {-1, 90041996,90041993, 90041994,-1, -1, 90041995, -1,90041997, -1, -1,-1,-1,-1,-1};
//int defenceHeroList[]  = {-1, 90041992,-1,-1, 90041991, 90041991, -1, 90041991, 90041991,-1, -1,-1,-1,-1,-1};

//4.9 BUG7 溅射问题
//int attackHeroList[]  = {90041967, 90041968, 90041969, 90041970, 90041977, -1, -1, -1,-1,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {90041972, 90041973, 90041974, 90041975, 90041976, -1, -1, -1,-1,-1, -1,-1,-1,-1,-1};

//4.9 BUG6 贾诩技能问题
//int attackHeroList[]  = {-1, -1, 90042000, -1, -1, -1, -1, -1,-1,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1, 90041991, 90041991, 90041991, -1, -1, -1, -1,-1,-1, -1,-1,-1,-1,-1};

//4.9 BUG8 董卓技能DOT问题
//int attackHeroList[]  = {-1, 90041991, 90042001, 90041991, -1, -1, -1, -1,-1,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1, 90041991, 90041991, 90041991, -1, -1, -1, -1,-1,-1, -1,-1,-1,-1,-1};

//4.10 BUG1 荀彧技能结束后，攻防变化bug
//int attackHeroList[]   = {-1,-1,-1,-1,-1, -1,90041972, 90042002, 90041972,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[]  = {90041991,-1,90041991,-1,90041991, -1,-1,-1,-1,-1, -1,-1,-1,-1,-1};

//int attackHeroList[]  = {-1,90041950,90041924,90041990,-1, -1,-1,-1,90041995,-1, -1,-1,90041997,-1,-1};
//int defenceHeroList[] = {-1,90041941,90041941,90041942,-1, -1,-1,90041943,90041943,-1, -1,-1,-1,-1,-1};

//近战测试
//int attackHeroList[]  = {-1,90042009,90042016,90042009,-1, -1,90042009,-1,90042009,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1,90042009,90042001,90042009,-1, -1,90042009,-1,90042009,-1, -1,-1,-1,-1,-1};

//int attackHeroList[]  = {90042103,90042103,-1,90042103,90042103, -1,-1,90042102,-1,-1, -1,-1,-1,-1,-1};
//int defenceHeroList[] = {-1,90042085,90042009,90042088,-1, 90042084,-1,90042090,-1,-1, -1,-1,-1,-1,-1};

Scene* LoadFightScene::createScene()
{
    Scene* scene = Scene::create();
    LoadFightScene* layer = LoadFightScene::create();
    scene->addChild(layer);
    
    layer->initData("");
    return scene;
}

LoadFightScene::LoadFightScene():_fightlayer(NULL)
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LoadFightScene::LoadFightScene  %p", this);
}

LoadFightScene::~LoadFightScene()
{
    if (_DEBUG_TYPE==_DEBUG_TYPE_SAFE)
        CCLOG("LoadFightScene::~LoadFightScene  %p", this);
}

void LoadFightScene::initCloud()
{
    float scale = 3.0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* c1 = Sprite::create("res/level/cloud_battle.png"); //左下
    c1->setAnchorPoint(Point(1,1));
    c1->setPosition(Point(visibleSize.width*0.7,visibleSize.height*0.7));
    c1->setScale(scale);
    this->addChild(c1);
    
    Sprite* c2 =  Sprite::create("res/level/cloud_battle.png"); //--右下
    c2->setAnchorPoint(Point(0,1));
    c2->setPosition(Point(visibleSize.width*0.3,visibleSize.height*0.7));
    c2->setScale(scale);
    this->addChild(c2);
    
    Sprite* c3 = Sprite::create("res/level/cloud_battle.png");// --左上
    c3->setAnchorPoint(Point(1,0));
    c3->setPosition(Point(visibleSize.width*0.7,visibleSize.height*0.3));
    c3->setScale(scale);
    this->addChild(c3);
    
    Sprite* c4 = Sprite::create("res/level/cloud_battle.png"); //--右上
    c4->setAnchorPoint(Point(0,0));
    c4->setPosition(Point(visibleSize.width*0.3,visibleSize.height*0.3));
    c4->setScale(scale);
    this->addChild(c4);
}

void LoadFightScene::initData(int attackList[], int defenceList[],int ftype)
{
    for (int i= 0; i<15; i++) {
        attackHeroList[i] = attackList[i];
        defenceHeroList[i] = defenceList[i];
    }
    islloadformat = false;
    
    initData("",ftype);
}
bool LoadFightScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    valueBattle = "";
    boolBattle = false;
    initUI();
    return true;
}
    

void LoadFightScene::initData(string valuestr,int ftype)
{
    valueBattle = valuestr;
    boolBattle = ftype;
}

void LoadFightScene::initDataToBattle()
{

    std::string valuestr = valueBattle;
    bool ftype = boolBattle;
    if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
        CCLOG("LoadFightScene::initData  begin  %f", getCurrentMillSecond());
    
    if ( LQDataCache::sharedLQDataCache()->getTipDatas()==NULL ){
        LQDataMap& tipdatas = *(LQDataMap*)LQDataCache::sharedLQDataCache()->lqAllDatasByName(LQTipInfoData::getAliasName());
        LQTipMap* tipMap = new LQTipMap();
        LQDataMap::iterator iter;
        for(iter = tipdatas.begin(); iter != tipdatas.end(); iter++)
        {
            LQTipInfoData* pcfgData = (LQTipInfoData*)iter->second;
            tipMap->insert(pair<int, string>(pcfgData->getFID(), pcfgData->getWord()));
            delete pcfgData;
        }
        //内存处理
        delete  &tipdatas;
        
        LQDataCache::sharedLQDataCache()->setTipDatas(tipMap);
    }
    
    m_valueStr = valuestr;
    int zoneid = 22010103;
    string m_cond = "";  //完成条件
    
    //m_isPvp = ispvp;
    if (ftype>0)
        fighttype = ftype; //战斗类型ftype传入-1按valuestr判断
    
    
#if defined(LQ_DEMO_FIGHT)
    if (islloadformat){
        //读数据
        LQDataMap& fmtdatas = *(LQDataMap*)LQDataCache::sharedLQDataCache()->lqAllDatasByName(LQTFormatinfoData::getAliasName());
        LQDataMap::iterator iter;
        int value = 0;
        for(iter = fmtdatas.begin();iter!=fmtdatas.end();++iter)
        {
            LQTFormatinfoData* pfmtData = (LQTFormatinfoData*)iter->second;
            CCLOG("ID:%d %s used:%d", pfmtData->getFID(), pfmtData->getName().c_str(), pfmtData->getUsed());
            if (pfmtData->getUsed()>2){
                valuestr += pfmtData->getVictoryCond();
            }else if(pfmtData->getUsed()>0){  //我方
                for (int i=0; i<15; i++) {
                    std::string p = StringUtils::format("IDX%d",i+1);
                    pfmtData->getValue(p.c_str(), &value);
                    if(value<=0) value = -1;
                    if (pfmtData->getIdx()==1)
                        attackHeroList[i] = value;
                    else if (pfmtData->getIdx()==2)
                        defenceHeroList[i] = value;
                }
                if (pfmtData->getZoneId()>0)
                    zoneid = pfmtData->getZoneId();
                if (pfmtData->getVictoryCond()>"")
                    m_cond = pfmtData->getVictoryCond();
            }
        }
        //内存处理
        delete &fmtdatas;
    }
#else
    if (valuestr.length()<=0){
        fighttype = 0;
        UserDefault::getInstance()->setIntegerForKey("UD_SOUND_KEY",0);
        //90041501 孙策 90041502 关羽 90041503 张辽 90041504 孙尚香 90041505 华雄
        //90041506 刘备 90041507 孙坚 90041508 曹操 90041509 ？？   90041510 徐晃
        attackHeroList[0] = -1; attackHeroList[1] = 90041501; attackHeroList[2] = 90041502; attackHeroList[3] = 90041503; attackHeroList[4] = -1;
        attackHeroList[5] = -1; attackHeroList[6] = -1; attackHeroList[7] = -1; attackHeroList[8] = 90041505; attackHeroList[9] = -1;
        attackHeroList[10] = -1; attackHeroList[11] = -1; attackHeroList[12] = 90041504; attackHeroList[13] = -1; attackHeroList[14] = -1;
        defenceHeroList[0] = -1; defenceHeroList[1] = 90041508; defenceHeroList[2] = 90041506; defenceHeroList[3] = 90041507; defenceHeroList[4] = -1;
        defenceHeroList[5] = -1; defenceHeroList[6] = 90041509; defenceHeroList[7] = -1; defenceHeroList[8] = 90041510; defenceHeroList[9] = -1;
        defenceHeroList[10] = -1; defenceHeroList[11] = -1; defenceHeroList[12] = -1; defenceHeroList[13] = -1; defenceHeroList[14] = -1;
    }else fighttype = 1;
#endif
    //测试数据   等级 resID 位置 兵种 兵数 (兵力 武力 智力 统帅)
    if(valuestr.length()<=0)
    {
        int mansize = 15;
        string s0;//"Vs_forest.jpg", "Vs_ship.jpg",
        string maps[] = { "Vs_grass.jpg", "Vs_snow.jpg", "Vs_city.jpg", "Vs_ship.jpg", "Vs_forest.jpg" };
        int midx = randomInt(1, 5)-1;
        if(midx>4) midx = 4;    if(midx<0) midx = 0;
        if (fighttype==0) midx = 2;
        stringf(s0, "%d;%d;0;%s;-1;40028001;0;0;1;%s#1,0;1,0", zoneid, fighttype, maps[midx].c_str(), m_cond.c_str() );
        string s1 = "";
        for(int i=0;i<15;i++){
            if(attackHeroList[i]<0)
                continue;
            string str = StringUtils::format("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 1, 0, attackHeroList[i], 1, i+1, -1, mansize, -1, -1, -1);
            //sprintf(str, "%s,15:%d$17:%d$18:%d", str, 40000, 2000, 1000 );
            //没有其他属性
            //兵的类型ATTR_ADDVALUE
            //sprintf(str, "%s$512:%d$513:%d$514:%d$41:%d$43:%d$45:%d$47:%d", str, 15000, 15000, 15000, 3000, 30000, 3000, 30000  );
            s1 = StringUtils::format("%s%s;", s1.c_str(), str.c_str());
            //CCLOG("str.c_str():%s", str.c_str());
            //CCLOG("\ns1.c_str():%s", s1.c_str());
        }
        //测试数据
        string s2 = "";
        if(true){
            for(int i=0;i<15;i++){
                if(defenceHeroList[i]<0)
                    continue;
                //敌人： 兵力 426 武勇 106 统帅 96
                //其它生命 10061 其它攻击 2515 其它防御 2263
                string str = StringUtils::format( "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 1, unittype, defenceHeroList[i], fighttype==1?0:1, i+1, -1, mansize, 400, 2, -1 );
                //sprintf(str, "%s,15:%d$17:%d$18:%d", str, 40000, 2000, 1000 );
                //没有其他属性
                //兵的类型ATTR_ADDVALUE
                //sprintf(str, "%s$512:%d$513:%d$514:%d$41:%d$43:%d$45:%d$47:%d", str, 15000, 15000, 15000, 3000, 30000, 3000, 30000);
                s2 = StringUtils::format( "%s%s;", s2.c_str(), str.c_str());
            }
        }else{
            int tempMonsterId = 21040025;//LQBattleSession::getInstance()->getBattleMonster();//m_currZoneData->getBattleMonster();
            LQMonsterGroupInfoData* groupData = (LQMonsterGroupInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQMonsterGroupInfoData::getAliasName(), tempMonsterId);
            
            CCLOG("MonsterGroup %d: %d %d %d; %d %d %d; %d %d %d;", tempMonsterId, groupData->getMonster1(), groupData->getMonster2(), groupData->getMonster3(),
                  groupData->getMonster4(), groupData->getMonster5(), groupData->getMonster6(),
                  groupData->getMonster7(), groupData->getMonster8(), groupData->getMonster9());
            int monsterId = -1;
            string str;
            for(int i=0;i<15;i++){
                stringf(str, "Monster%d", i+1);
                groupData->getValue(str.c_str(), &monsterId);
                if(monsterId<=0)
                    continue;
                string str;
                str = StringUtils::format( "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 1, groupData->getUseType(), monsterId, 0, i, -1, mansize, -1, -1, -1 );
                s2 = StringUtils::format( "%s%s;", s2.c_str(), str.c_str());
            }
        }
        valuestr = StringUtils::format("%s#%s#%s", s0.c_str(), s1.c_str(), s2.c_str() );
        
        //valuestr = "22010102;1;-1;Vs_snow.jpg;21040050;40028001;930;66;0#21,21591;1,5188#19,0,90010018,1,3,90001006,15,4854,2,2,12:9700$13:7600$14:8500$15:743862$17:63730$18:23497$25:0$26:0$27:0$28:0$31:0$32:0$41:0$42:0$43:30000$44:0$45:3000$46:0$47:30000$48:0$49:0$50:0$51:30000$52:0$53:0$54:0$55:30000$56:0$57:0$58:0$59:30000$60:0;19,0,90010049,1,2,90001002,15,5733,3,2,12:9000$13:7900$14:9400$15:1045519$17:63279$18:9656$25:0$26:0$27:0$28:0$31:0$32:0$41:0$42:0$43:30000$44:0$45:0$46:0$47:30000$48:0$49:3480$50:0$51:30000$52:0$53:0$54:0$55:30000$56:0$57:0$58:0$59:30000$60:0;19,0,90010004,1,4,90001002,15,4020,3,2,12:6400$13:8800$14:7900$15:616579$17:43090$18:9162$25:0$26:0$27:0$28:0$31:0$32:0$41:0$42:0$43:30000$44:0$45:0$46:0$47:30000$48:0$49:3000$50:0$51:30000$52:0$53:0$54:0$55:30000$56:0$57:0$58:0$59:30000$60:0;19,0,90010002,1,8,90001014,15,3483,2,2,12:3700$13:7300$14:5300$15:521173$17:37447$18:7959$25:0$26:0$27:0$28:0$31:0$32:0$41:3000$42:0$43:30000$44:0$45:0$46:0$47:30000$48:0$49:0$50:0$51:30000$52:0$53:0$54:0$55:30000$56:0$57:0$58:0$59:30000$60:0;19,0,90010028,1,7,90001002,15,3501,2,2,12:8200$13:7200$14:8000$15:491898$17:34099$18:7153$25:0$26:0$27:0$28:0$31:0$32:0$41:0$42:0$43:30000$44:0$45:0$46:0$47:30000$48:0$49:3000$50:0$51:30000$52:0$53:0$54:0$55:30000$56:0$57:0$58:0$59:30000$60:0;#0,1,91103172,0,3,-1,-1,2594,1,1,-1;0,1,91103171,0,8,-1,-1,2594,1,1,-1;";
        
        //valuestr = "3;10;61080000;Vs_grass.jpg;21040001;40028001;228;30;1#54,93477;0,0#52,0,90010037,1,8,90001006,15,28838,4,3,12:8500$13:7800$14:9500$15:6856423$17:367898$18:79424$25:0$26:0$27:0$28:0$31:0$32:0$41:500$42:0$43:30000$44:0$45:3000$46:0$47:30000$48:0$49:500$50:0$51:30000$52:0$53:0$54:0$55:0$56:0$57:0$58:0$59:0$60:0$31:100$15:11000;51,0,90010045,1,1,90001006,15,22414,2,2,12:7800$13:7400$14:8300$15:5328938$17:245482$18:73598$25:0$26:0$27:0$28:0$31:0$32:0$41:500$42:0$43:30000$44:0$45:3000$46:0$47:30000$48:0$49:500$50:0$51:30000$52:0$53:0$54:0$55:0$56:0$57:0$58:0$59:0$60:0$31:100$15:11000;51,0,90010006,1,3,90001006,15,25233,3,8,12:10000$13:2600$14:9700$15:5987810$17:301916$18:82157$25:0$26:0$27:0$28:0$31:0$32:0$41:500$42:0$43:30000$44:0$45:3000$46:0$47:30000$48:0$49:500$50:0$51:30000$52:0$53:0$54:0$55:0$56:0$57:0$58:0$59:0$60:0$31:100$15:11000;51,0,90010051,1,15,90001014,15,9076,2,3,12:8600$13:7000$14:7600$15:1680800$17:103073$18:26867$25:0$26:0$27:0$28:0$31:0$32:0$41:3000$42:0$43:30000$44:0$45:500$46:0$47:30000$48:0$49:500$50:0$51:30000$52:0$53:0$54:0$55:0$56:0$57:0$58:0$59:0$60:0$31:100$15:11000;51,0,90010017,1,6,90001006,15,7916,1,3,12:7700$13:7800$14:8100$15:1573656$17:95555$18:25172$25:0$26:0$27:0$28:0$31:0$32:0$41:500$42:0$43:30000$44:0$45:3000$46:0$47:30000$48:0$49:500$50:0$51:30000$52:0$53:0$54:0$55:0$56:0$57:0$58:0$59:0$60:0$31:100$15:11000;#15,12,91603014,0,1,-1,-1,1962,2,2;15,12,91603012,0,3,-1,-1,1962,1,2;15,12,91603011,0,8,-1,-1,1962,3,2;";
        
        //valuestr = "22010102;3;0;Vs_grass.jpg;-1;40028001;0;0;1#1,0;1,0#1,0,90041950,1,2,-1,15,-1,-1,-1;1,0,90041953,1,3,-1,15,-1,-1,-1;1,0,90041952,1,8,-1,15,-1,-1,-1;1,0,90041940,1,9,-1,15,-1,-1,-1;1,0,90041954,1,10,-1,15,-1,-1,-1;#1,1,90041950,1,2,-1,15,400,2,-1;1,1,90041952,1,3,-1,15,400,2,-1;1,1,90041953,1,4,-1,15,400,2,-1;1,1,90041940,1,7,-1,15,400,2,-1;1,1,90041954,1,8,-1,15,400,2,-1;";  //迎击BUG
        
        //valuestr = "3;3;08fdd2ec76ec466fbb53d130ac23b623;Vs_snow.jpg;21040034;40028001;281;30;1#33,45130;33,0#33,0,90010018,1,4,90001009,15,8621,3,4,12:29600$13:28000$14:28500$15:5032847$17:205128$18:56165$27:0$28:0$31:0$32:10000$33:688$41:624$42:124$43:30400$44:400$45:3124$46:124$47:30400$48:3400$49:624$50:124$51:30400$52:400$53:0$54:0$55:0$56:0$57:124$58:124$59:400$60:400$71:0$72:0$73:0$74:0$104:20000$25:590$26:590;33,0,90010029,1,5,90001009,15,8505,3,4,12:21400$13:24100$14:24560$15:5041492$17:202428$18:54397$27:0$28:0$31:0$32:10000$33:320$41:624$42:124$43:30400$44:400$45:3124$46:124$47:30400$48:400$49:624$50:124$51:33400$52:400$53:0$54:0$55:0$56:0$57:124$58:124$59:400$60:400$71:0$72:0$73:0$74:0$104:20000$25:500$26:885;33,0,90010006,1,1,90001009,15,12750,4,4,12:32300$13:22600$14:29600$15:6909403$17:324603$18:97279$27:0$28:0$31:0$32:10000$33:0$41:624$42:124$43:30400$44:946$45:3124$46:124$47:30400$48:946$49:1024$50:124$51:30400$52:946$53:0$54:0$55:0$56:0$57:124$58:124$59:400$60:946$71:0$72:0$73:0$74:546$104:20000$25:1900$26:1020;33,0,90010059,1,3,90001014,15,7706,3,4,12:13700$13:17600$14:16200$15:4974311$17:180316$18:49442$27:0$28:0$31:0$32:10000$33:688$41:3124$42:124$43:30400$44:400$45:624$46:124$47:30400$48:400$49:624$50:124$51:32400$52:400$53:0$54:0$55:0$56:0$57:124$58:124$59:400$60:400$71:0$72:0$73:0$74:0$104:20000$25:590$26:590;33,0,90010028,1,2,90001012,15,7548,3,4,12:12450$13:12100$14:12900$15:4122342$17:174370$18:49613$27:0$28:0$31:0$32:10000$33:688$41:624$42:2524$43:30400$44:400$45:624$46:124$47:30400$48:3400$49:3124$50:124$51:30400$52:400$53:0$54:0$55:0$56:0$57:124$58:124$59:400$60:400$71:0$72:0$73:0$74:0$104:20000$25:590$26:590;#33,-1,90010018,1,6,90001009,-1,4841,3,4,12:10800$13:8700$14:9400$15:2550932$17:136608$18:35334$25:250$26:500$32:10000$33:320$41:500$43:30000$45:3000$47:30000$48:1000$49:500$51:30000$104:20000;33,-1,90010006,1,1,90001009,-1,5076,3,4,12:10300$13:3000$14:10000$15:2629098$17:148372$18:37068$25:340$26:680$32:10000$33:368$41:500$43:30000$45:3000$47:30000$49:500$51:30000$104:20000;33,-1,90010014,1,3,90001012,-1,5414,3,4,12:5000$13:2450$14:4600$15:2794233$17:151401$18:39138$25:250$26:590$32:10000$33:344$41:500$43:33000$45:500$47:30000$49:3000$51:33000$104:20000;33,-1,90010059,1,2,90001014,-1,5562,3,4,12:3900$13:7600$14:6000$15:2641920$17:160120$18:42089$25:590$26:750$32:10000$33:344$41:3000$43:30000$44:462$45:500$47:30000$48:462$49:500$51:30000$52:462$60:462$74:462$104:20000;33,-1,90010002,1,8,90001014,-1,5047,3,4,12:1850$13:3650$14:2650$15:2605430$17:137744$18:35462$25:250$26:500$32:10000$33:320$41:3000$43:30000$45:2100$47:30000$48:3000$49:500$51:30000$104:20000;##1";
        
        //CCLOG("valuestr...:%s", valuestr.c_str() );
    }
    
    //初始化
    LQBattleSession::getInstance()->initBattleData(valuestr);
    
    
    
    //云
    //    initCloud();
    if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
        CCLOG("LoadFightScene::initData  end  %ld", getCurrentMillSecond());
}
void LoadFightScene::initUI()
{
    int adsId = randomInt(6, 18);
    LQAdsInfoData* groupData = (LQAdsInfoData*)LQDataCache::sharedLQDataCache()->lqDataByName(LQAdsInfoData::getAliasName(), adsId);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite *bg = Sprite::create("res/background/bigbeijing.jpg");
    bg->setColor(Color3B(100,100,100));
    bg->setAnchorPoint(Point(0.5, 0.5));
    bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    bg->setScaleX(visibleSize.width / designResolutionSize.width);
    bg->setScaleY(visibleSize.height / designResolutionSize.height);
    this->addChild(bg);
    
    std::string LTJM_PLIST = "files/ccbiui/ccbires/resv4/V4UNIVERSAL.plist";
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(LTJM_PLIST);
    SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("ty_guangbian.png");
    Scale9Sprite *basecolor = Scale9Sprite::createWithSpriteFrame(frame);
    basecolor->setAnchorPoint(Point(0,0));
    int margin = 80;
    basecolor->setPosition(Point(0, margin));
    basecolor->setPreferredSize(Size(visibleSize.width,visibleSize.height-margin*2));
    this->addChild(basecolor);
    
    SpriteFrame *frameline=SpriteFrameCache::getInstance()->getSpriteFrameByName("ty_weizidiguang.png");
    Sprite *spline = Sprite::createWithSpriteFrame(frameline);
    spline->setScale(3, 0.3);
    spline->setAnchorPoint(Point(0,0));
    spline->setPosition(Point(visibleSize.width*0.3, visibleSize.height*0.3));
    this->addChild(spline);


    //string filename = cardname[(int)randomInt(0, 13)].c_str();
    //string cardtipstr = cardtip[(int)randomInt(0, 10)].c_str();
    string filename = groupData->getCardIcon().c_str();
    string cardtipstr = groupData->getAdWords().c_str();
    
    std::string sname = StringUtils::format("res/cards/%s", filename.c_str());
    Sprite *spGrid = Sprite::create(sname.c_str());
    spGrid->setAnchorPoint(Point(0.5,0.5));
    spGrid->setPosition(Point(-spGrid->getContentSize().width/2,visibleSize.height*0.5));
    spGrid->setScale(1.3);
    this->addChild(spGrid,100);
    spGrid->runAction(MoveTo::create(0.2,Point(visibleSize.width*0.2,visibleSize.height*0.5)));
    
    std::string stip = StringUtils::format("%s",cardtipstr.c_str());
    
    Label *labelinfo1 = Label::createWithSystemFont(stip.c_str(),"Helvetica", 30,Size(visibleSize.width*0.45,0),TextHAlignment::LEFT);
    labelinfo1->setColor(Color3B(0,0,0));
    labelinfo1->setAnchorPoint(Point(0,0.5));
    labelinfo1->setPosition(Point(visibleSize.width + 2,visibleSize.height/2 - 2));
    this->addChild(labelinfo1,99);
    labelinfo1->runAction(MoveTo::create(0.2, Point(visibleSize.width*0.42 + 2,visibleSize.height/2 - 2)));
    
    Label *labelinfo = Label::createWithSystemFont(stip.c_str(),"Helvetica", 30,Size(visibleSize.width*0.45,0),TextHAlignment::LEFT);
    labelinfo->setColor(Color3B(255,255,255));
    //labelinfo->enableOutline(Color4B(64,153,182,255),2);
    labelinfo->setAnchorPoint(Point(0,0.5));
    labelinfo->setPosition(Point(visibleSize.width,visibleSize.height/2));
    this->addChild(labelinfo,100);
    labelinfo->runAction(MoveTo::create(0.2, Point(visibleSize.width*0.42,visibleSize.height/2)));
    
    
    loadInfoLabel = Label::createWithSystemFont("","Helvetica", 25);
    loadInfoLabel->enableOutline(Color4B(64,153,182,255),2);
    loadInfoLabel->setColor(Color3B(255,255,255));
    loadInfoLabel->setAnchorPoint(Point(1,0));
    loadInfoLabel->setPosition(Point(visibleSize.width*0.9,visibleSize.height*0.3-30));
    this->addChild(loadInfoLabel,100);

//    particle = ParticleSystemQuad::create("res/particle/triedfire.plist");
//    particle->setAnchorPoint((Point(0, 0.5)));
//    particle->setPosition(Point(-400, 0));
//    this->addChild(particle,100);
//    particle->runAction(MoveTo::create(0.5, Point(1024,0)));

}

void LoadFightScene::onEnter()
{
    initDataToBattle();
    if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
        CCLOG("LoadFightScene::onEnter (装载进入) %ld", getCurrentMillSecond());
    Layer::onEnter();
    m_index = 0;
    m_resFile[1] = "res/soldier/td-special.plist";
    m_resFile[2] = "res/skill/skillbuffer.plist";
    m_resFile[3] = "res/skill/fightlayer.plist";
    if (fighttype==0)
        m_resFile[4] = "files/ccbiui/ccbires/resv4/V4JUQING.plist";
    int begini = fighttype==0?5:4;
    for (int i=0; i<LQBattleSession::getInstance()->getHeroResourses().size(); i++) {
        string res = LQBattleSession::getInstance()->getHeroResourses().at(i);
        m_resFile[begini+i] = res;
    }
    
    scheduleUpdate();
}

void LoadFightScene::onExit()
{
    Layer::onExit();
    if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
        CCLOG("LoadFightScene::onExit (装载退出) %ld", getCurrentMillSecond());
}

void LoadFightScene::update(float dt)
{
    m_index++;
    string filename = m_resFile[m_index].asString();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename);
   
    if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
        CCLOG("LoadFightScene::update  m_index = %d  %ld",m_index, getCurrentMillSecond());
    int mload = ((float)m_index / (float)m_resFile.size())*100;
    loadInfoLabel->setString(StringUtils::format("正在努力加载中:%d%%",mload));
//    float px = Director::getInstance()->getVisibleSize().width * mload + particle->getPositionX();
//    particle->setPosition(Point(px,0));
    
    if (m_index >= m_resFile.size()) {
        if(_DEBUG_TYPE==_DEBUG_TYPE_LOAD)
            CCLOG("LoadFightScene Is Load End. (装载完毕切换场景) %ld", getCurrentMillSecond());
        
        unscheduleUpdate();
        _fightlayer = LQFightLayer::create();
        _fightlayer->initData(m_valueStr);
        
        showLayer();
        //Director::getInstance()->replaceScene(scene);
    }
}

void LoadFightScene::showLayer()
{
    Scene *scene = Director::getInstance()->getRunningScene();
    scene->addChild(_fightlayer);
    this->removeFromParent();
}