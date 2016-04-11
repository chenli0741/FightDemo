//
//  LQEncrypt.cpp
//  MyCppGame
//
//  Created by lequdev2 on 14-10-31.
//
//

#include "LQEncrypt.h"


/**
 * 获取加密文件的全路径，如最终需要A.plist 传入A 即可 会返回A.加密后缀
 */
std::string LQEncrypt::getEncryptFilePath(const std::string &filename){
#if   LQ_DATA_FORMAT>0
    //plist文件
    std:string encryptName = filename+"."+LQEncryptSuffix;//获取加密后的文件名
#else
    //json文件
    std:string encryptName = filename+"."+LQEncryptSuffixJson;//获取加密后的文件名
#endif
    string  encryptFullPath=FileUtils::getInstance()->fullPathForFilename(encryptName);
    return encryptFullPath;
}


/**
 * 传入要写的数据和绝对地址，写文件
 */
bool LQEncrypt::writeFile(std::string data,const std::string& writePath){
    const char * cpath = static_cast<const char*>(writePath.c_str());
    std::FILE  *fp = std::fopen(cpath, "wb");
    std::fwrite(data.c_str(), 1, data.size(), fp);
    std::fflush(fp);
    std::fclose(fp);
    return true;
}


/**
 * 解密文件，传入要解密的文件全路径，返回解密后的数据
 */
std::string LQEncrypt::decryptFile(const std::string& fullPath){
    Data readInfo =FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->fullPathForFilename(fullPath));
	unsigned char* resultData;
	resultData = readInfo.getBytes();
	for (int i = 0; i < readInfo.getSize(); i++){
		resultData[i] ^= LQEncryptCode;
	}

    std::string ret((const char*)resultData);
    return ret;
}


/**
 * 删除文件，在文件读取完毕之后调用，用来删除掉刚刚解密出来的明文文件
 */
bool LQEncrypt::deleteFile(const std::string& fullPath){
    const char * cpath = static_cast<const char*>(fullPath.c_str());
    std::remove(cpath);
    return  true;
}

//直接读取文件，解密，然后直接写
bool LQEncrypt::decodeAndWriteFile(const std::string& readPath,const std::string& writePath){
    Data readInfo =FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->fullPathForFilename(readPath));
	unsigned char* resultData;
	resultData = readInfo.getBytes();
	for (int i = 0; i < readInfo.getSize(); i++){
		resultData[i] ^= LQEncryptCode;
	}
    
    const char * cpath = static_cast<const char*>(writePath.c_str());
    std::FILE  *fp = std::fopen(cpath, "wb");
    std::fwrite(resultData, 1, readInfo.getSize(), fp);
    std::fflush(fp);
    std::fclose(fp);
    return true;
    
}