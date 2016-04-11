//
// ErrorOperationUnit.h
// 操作失败
// 消息号：
// 
// Created by  globjoy at 2014-05-04 
// 
// 

#include "ErrorOperationUnit.h"

LQ_NAMESPACE


ErrorOperationUnit::ErrorOperationUnit(LQSocketBuffer *buffer) : LQUnitResponse(buffer), _errorID(0), _replaceSize(0) {
	readMessage();
}

ErrorOperationUnit::~ErrorOperationUnit() {
}

// read message from socket channel 
void ErrorOperationUnit::readMessage() {
	LQUnitResponse::readMessage();

	_errorID = readInt(); //错误ID
	_replaceSize = readShort(); //可替换字符数量
	if(_replaceSize >0 ) { //替换异常提示中的字符
		for(int i = 0; i < _replaceSize; i++){
			string replaceChar = readString();
			_replaceChars.push_back(replaceChar);
		}
	} //end if 替换异常提示中的字符
}

LQ_NAMESPACE_END
