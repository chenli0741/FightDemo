//
// ErrorOperationUnit.h
// 操作失败
// 消息号：
// 
// Created by  globjoy at 2014-05-04 
// 
// 

#ifndef __GLOBJOY__ErrorOperationUnit__
#define __GLOBJOY__ErrorOperationUnit__

#include "../../Socket/LQUnitResponse.h"

STD_NAMESPACE_USE
LQ_NAMESPACE

class ErrorOperationUnit : public LQUnitResponse {

public:
	ErrorOperationUnit(LQSocketBuffer *buffer);
	~ErrorOperationUnit();

	// read message from socket channel 
	void readMessage();

	// 错误ID
	inline int errorID() { return _errorID; }
	// 可替换字符数量
	inline short replaceSize() { return _replaceSize; }
	// 替换字符
	inline vector<string>& replaceChars() { return _replaceChars; }

private:
	int _errorID; // 错误ID
	short _replaceSize; // 可替换字符数量
	vector<string> _replaceChars; // 替换字符

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__ErrorOperationUnit__) */
