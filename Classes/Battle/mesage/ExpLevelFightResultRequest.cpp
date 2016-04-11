//
// ExpLevelFightResultRequest.h
// 经验关卡战斗结果
// 消息号：11133
// 
// Created by  globjoy at 2014-09-17 
// 
// 

#include "ExpLevelFightResultRequest.h"

LQ_NAMESPACE


ExpLevelFightResultRequest::ExpLevelFightResultRequest() : LQClientRequest(), _resId(0), _isWin(0) {
}

ExpLevelFightResultRequest::ExpLevelFightResultRequest(	int resId,	bool isWin)
	: LQClientRequest(), _resId(resId),
_isWin(isWin) {
}

ExpLevelFightResultRequest::~ExpLevelFightResultRequest() {
}

LQSocketBuffer* ExpLevelFightResultRequest::toMessage() {
	setMsgCode(11133);
	writeInt(_resId); //关卡ID
	writeBoolean(_isWin); //战斗结果,是否胜利
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
