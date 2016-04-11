//
//  LQStringExtend.h
//  SuperWarriors
//
//  Created by ChenLi on 13-4-7.
//
//

#ifndef SuperWarriors_CStringExtend_h
#define SuperWarriors_CStringExtend_h

#include <LQCore.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdarg>
#include <cstdio>
#include <zlib.h>
//#include <openssl/des.h>
//#include "CCPlatformMacros.h"
//#include "ZBase64.h"

using namespace std;

LQ_NAMESPACE

typedef std::vector<Node*> NodeVector;

//字符串替换
//  string str = "hello world.";
//  string_replace(str,"world.","world!");
static void string_replace(std::string& strBig, const std::string & strsrc, const std::string &strdst)
{
    std::string::size_type pos = 0;
    while( (pos = strBig.find(strsrc, pos)) != string::npos)
    {
        strBig.replace(pos, strsrc.length(), strdst);
        pos += strdst.length();
    }
}

//=============================================================================
// string result;
// stringf(result, "string length is %d", 0);
// 输出结果：result = string length is 0.
static void stringf(string& stgt, const char *sformat, ... )
{
    int size = 2048;
    va_list ap;
    while (1) {
        stgt.resize(size);
        va_start(ap, sformat);
        int n = vsnprintf((char *)stgt.c_str(), size, sformat, ap);
        va_end(ap);
        if (n > -1 && n < size) {
            stgt.resize(n);
            return;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
};

// va_list myArgs; // {"1", "2", "3"}
// va_start(myArgs, msg);
// string outString;
// stringf(outString, myArgs, "a%sb%sc%s);
// 输出结果 outString = "a1b2c3"
static void stringflist(string& stgt, const char *sformat, va_list ap)
{
    int size = 2048;
    while (1) {
        stgt.resize(size);
        int n = vsnprintf((char *)stgt.c_str(), size, sformat, ap);
        if (n > -1 && n < size) {
            stgt.resize(n);
            return;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
};

//=============================================================================
// string result = "default string. ";
// stringf(result, "string length is %d.", 0);
// 输出结果：result = default string. string length is 0.
static void stringfappend(string& stgt, const char *sformat, ... )
{
    std::string ss;
    int n, size=2048;
    bool b=false;
    va_list marker;
    
    while (!b)
    {
        ss.resize(size);
        va_start(marker, sformat);
        n = vsnprintf((char*)ss.c_str(), size, sformat, marker);
        va_end(marker);
        if ((n>0) && ((b=(n<size))==true)) ss.resize(n); else size*=2;
    }
    stgt += ss;
}

// std::vector<std::string> x;
// split("one:two::three", ':', x);
static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// std::vector<std::string> x = split("one:two::three", ':');
static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}


inline const char* getCharPtr(string& str)
{
    str.size();
    //define a new memory space for saving this char array
    char *cha = new char[str.size() + 1];
    memcpy(cha,str.c_str(),str.size() + 1);
    
    const char *p = cha;    //point to this new allocated  memory
    
    return p;
}

/**
 *
 *
 * 用新子串newstr替换源字符串src中所包含的oldstr子串
 *
 * @param const char* src    源字符串，被替换的字符串
 * @param const char* oldstr 旧的子串，将被替换的子串
 * @param const char* newstr 新的子串
 *
 * @return char* dest  返回新串的地址
 *
 */
inline char *str_replace(const char *src, const char *oldstr, const char *newstr)
{
    if(NULL == src || NULL == oldstr || NULL == newstr)
    {
        return NULL;
    }
    
    //从源串中拷贝一份到目的串
    char *dest=strdup(src);
    //如果串相等，则直接返回
    if(strcmp(oldstr, newstr)==0)
    {
        return dest;
    }
    
    //子串位置指针
    char *needle;
    //临时内存区
    char *tmp,*dest_tmp=dest;
    
    //如果找到子串, 并且子串位置在前len个子串范围内, 则进行替换, 否则直接返回
    while((needle = strstr(dest, oldstr)))
    {
        //分配新的空间: +1 是为了添加串尾的'\0'结束符
        tmp=(char*)malloc(strlen(dest)+strlen(newstr)-strlen(oldstr)+1);
        //把src内的前needle-dest个内存空间的数据，拷贝到arr
        strncpy(tmp,dest,needle-dest);
        //标识串结束
        tmp[needle-dest]='\0';
        //连接arr和newstr, 即把newstr附在arr尾部, 从而组成新串(或说字符数组)arr
        strcat(tmp, newstr);
        //把src中 从oldstr子串位置后的部分和arr连接在一起，组成新串arr
        strcat(tmp, needle+strlen(oldstr));
        //把用malloc分配的内存，复制给指针retv
        dest = strdup(tmp);
        free(dest_tmp);
        //释放malloc分配的内存空间
        free(tmp);
        dest_tmp = dest;
    }
    return dest;
}

/**
 *  判断字符串是否为空
 */
inline bool is_empty_str(const char *x) {
    if(NULL == x || '\0' == x[0]) {return true;} else {return false;}
}

//字符串颜色
static Color3B Color3BFromString(const std::string& str)
{
    Color3B ret = Color3B::WHITE;
    if (str.size()<=0)
        return ret;
    std::vector<std::string> strs = split(str.c_str(), ',');  //';'
    
    if (strs.size()!=3 || strs[0].length() == 0 || strs[1].length() == 0 || strs[2].length() == 0 )
        return ret;
    float r = (float) utils::atof(strs[0].c_str());
    float g = (float) utils::atof(strs[1].c_str());
    float b = (float) utils::atof(strs[2].c_str());
    
    ret = Color3B(r, g, b);
    
    return ret;
}


LQ_NAMESPACE_END

#endif
