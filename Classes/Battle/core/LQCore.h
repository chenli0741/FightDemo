//
//  LQCore.h
//  SuperWarriors
//
//  Created by ByronSong on 13-4-7.
//
//

#ifndef SuperWarriors_LQCore_h
#define SuperWarriors_LQCore_h

#pragma once
#pragma mark LQ Name Space

#define LQ_NAMESPACE_NAME LQ

#define LQ_NAMESPACE namespace LQ_NAMESPACE_NAME {
#define LQ_NAMESPACE_END };
#define LQ_NAMESPACE_USE using namespace LQ_NAMESPACE_NAME;

#define STD_NAMESPACE_USE using namespace std;
#define COCOS2D_NAMESPACE_USE using namespace cocos2d;

#define LQ_DATA_FORMAT 0      //1 plist 0 json

#include "cocos2d.h"

USING_NS_CC;

LQ_NAMESPACE

// cocoa null point
#ifndef nil
    #define nil NULL
#endif

#pragma mark
#pragma mark LQ Log level
typedef enum
{
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3
}LQLogLevel;

#define RATE_PERCENT    10000.00f     //万分比，所有比率都按这个原则走
#define CENTI_PERCENT    100.00f     //百分比，所有比率都按这个原则走

static const float MIN_DPS = 100.00f;   //最小的DPS

static const float MAP_WIDTH  = 64.0f;  //菱形方块宽
static const float MAP_HEIGTH = 32.0f;  //菱形方块高
static const float MAP_LENGTH = 64.0f;//35.7f;  //菱形方块长度
static const int MAP_COL = 32;          //地图的行数
static const int MAP_ROW = 32;          //地图的列数
static const int MAP_TILE_PASSMAX = 3;  //地图的Tile通过最大人数
static const int MAP_TILE_STANDMAX = 1; //地图的Tile停留最大人数

// 八方向
static const int DIRECTION_SIZE = 8;

static const int OVER_TILE_GRID = 1;

static std::string PointString(Point pt)
{
    char buffer[50];
	sprintf(buffer, "(%.3f,%.3f)", pt.x, pt.y);
    return buffer;
}

static std::string TileCoordString(Point pt)
{
    char buffer[50];
	sprintf(buffer, "(%.0f,%.0f)", pt.x, pt.y);
    return buffer;
}

//Point转换成TileCoord
static Point convertTileCoord(float x,float y)
{
    Point originP;
    originP.x = MAP_ROW * MAP_WIDTH /2;
    originP.y = 0;
    Point p;
    p.x = int( (x-originP.x)/MAP_WIDTH + y/MAP_HEIGTH );
    p.y = int( (originP.x-x)/MAP_WIDTH + y/MAP_HEIGTH );
    return p;
}

static Point convertTileCoord(const Point& pt)
{
    return convertTileCoord(pt.x,pt.y);
}

//TileCoord转换成Point
static Point convertPoint(int M,int N)
{
    Point originP;
    originP.x = MAP_ROW * MAP_WIDTH /2;
    originP.y = 0;
    Point p;
    p.x = originP.x + (M - N) * MAP_WIDTH/2;
    p.y = originP.y + (M + N) * MAP_HEIGTH/2;
    return p;
}

//TileCoord转换成Point
static Point convertPoint(const Point& tilep)
{
    return convertPoint(tilep.x,tilep.y);
}

//数字转字符串
static std::string intToString(int num) {
    std::stringstream ss;
    std::string str;
    ss<<num;
    ss>>str;
    return str;
}

//数字转字符串
static std::string intToTimeString(int num) {
    int m = num/60;
    int s = num%60;
    char buf[256];
    sprintf(buf,"%02d:%02d",m,s);
//    std::stringstream ss;
//    std::string str;
//    ss<<m;
//    ss<<':';
//    ss<<s;
//    ss>>str;
    return buf;
}

//获取当前时间（没有校对）
//  校对过的时间请用getCurrentTimeMillis()
//  单位:毫秒
static long long currentTimeMillis()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    unsigned long long second = now.tv_sec;
    long long milisec = (second * 1000) + (now.tv_usec / 1000.0);
    return(milisec);
}

//获取两点向量的角度
// end  目标点
// start 原点
static float getAngle(const Point& end, const Point& start)
{
    float angle = atan2f( end.y*2 - start.y*2, end.x - start.x );
    if( fabs(angle) < FLT_EPSILON ) return 0.f;
    return angle/(2*M_PI);
}

//获取两点45度的距离
// end  目标点
// start 原点
static float getDistance(const Point& end, const Point& start)
{
    //目标直线距离
    float endDist = end.getDistance(start);
    //目标角度
    float angle = getAngle(end,start);
    //目标角度椭圆的交点
    Point elppt = start + Point( MAP_WIDTH*cos( angle*2*M_PI ), MAP_WIDTH/2*sin( angle*2*M_PI ));
    //目标角度的标准单元的距离
    float elpDist = elppt.getDistance(start);
    //转换后的距离
    float dist = (endDist/elpDist)*MAP_WIDTH;
	//log("最后的长度是 %f %f d=%f", endDist, elpDist, dist);
    return dist;
}

//
static double getCurrentMillSecond()
{
    double lLastTime = 0;
    struct timeval stCurrentTime;
    
    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
    return lLastTime;
}

static timeval getCurrentTime()
{
    struct timeval stCurrentTime;
    
    gettimeofday(&stCurrentTime,NULL);
    return stCurrentTime;
}

LQ_NAMESPACE_END

#endif
