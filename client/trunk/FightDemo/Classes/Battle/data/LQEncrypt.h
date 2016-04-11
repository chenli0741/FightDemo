//
//  LQEncrypt.h
//  MyCppGame
//
//  Created by lequdev2 on 14-10-31.
//
//

#ifndef __MyCppGame__LQEncrypt__
#define __MyCppGame__LQEncrypt__


#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

//加密文件的后缀名，不能是plist，否则Xcode编译直接报错
#define LQEncryptSuffix "dat"
//json文件不需要改后缀
#define LQEncryptSuffixJson "bin"
//加密用的密钥字符串，加密解密需要用同样的密钥才能正确解开
#define LQEncryptCode 0xa62dd44

class LQEncrypt{
public:
    static std::string  getEncryptFilePath(const std::string& filename);//获取加密文件的全路径
    static bool writeFile(std::string data,const std::string& writePath);//将传入的解密后数据写入传入的地址
    static std::string decryptFile(const std::string& fullPath);//根据传入的全路径读取文件，进行解密，之后返回解密后的数据
    static bool  deleteFile(const std::string& fullPath);//文件读取后调用本方法把杠杠创建的文件删除，传入文件的全路径
    static bool decodeAndWriteFile(const std::string& readPath,const std::string& writePath);//直接解码然后写文件,第一个参数是读文件的路径，第二个参数是写文件的路径
};



#endif /* defined(__MyCppGame__LQEncrypt__) */
