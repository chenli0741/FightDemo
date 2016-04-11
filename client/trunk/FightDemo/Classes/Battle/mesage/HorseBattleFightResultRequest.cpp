//
// HorseBattleFightResultRequest.h
// 坐骑碎片掠夺数据
// 消息号：11513
// 
// Created by  globjoy at 2014-09-22 
// 
// 

#include "HorseBattleFightResultRequest.h"

LQ_NAMESPACE


HorseBattleFightResultRequest::HorseBattleFightResultRequest() : LQClientRequest(), _fragmentResId(0), _isWin(0){
}

HorseBattleFightResultRequest::HorseBattleFightResultRequest(	int fragmentResId,std::string fragmentID,	bool isWin)
	: LQClientRequest(), _fragmentResId(fragmentResId),_fragmentID(fragmentID),
_isWin(isWin) {
}

HorseBattleFightResultRequest::~HorseBattleFightResultRequest() {
}

LQSocketBuffer* HorseBattleFightResultRequest::toMessage() {
	setMsgCode(11513);
	writeInt(_fragmentResId); //坐骑碎片资源ID
    writeString(_fragmentID);
	writeBoolean(_isWin); //战斗结果,是否胜利
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
