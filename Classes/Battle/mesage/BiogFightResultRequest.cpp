//
// BiogFightResultRequest.h
// 列传关卡战斗结果
// 消息号：11125
// 
// Created by  globjoy at 2014-09-17 
// 
// 

#include "BiogFightResultRequest.h"

LQ_NAMESPACE


BiogFightResultRequest::BiogFightResultRequest() : LQClientRequest(), _zoneID(0), _isWin(0) {
}

BiogFightResultRequest::BiogFightResultRequest(	int zoneID,	bool isWin)
	: LQClientRequest(), _zoneID(zoneID),
_isWin(isWin) {
}

BiogFightResultRequest::~BiogFightResultRequest() {
}

LQSocketBuffer* BiogFightResultRequest::toMessage() {
	setMsgCode(11125);
	writeInt(_zoneID); //关卡ID
	writeBoolean(_isWin); //战斗结果,是否胜利
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
