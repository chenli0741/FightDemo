//
// ZoneFightResultRequest.h
// 关卡战斗结果
// 消息号：11107
// 
// Created by  globjoy at 2014-07-27 
// 
// 

#include "ZoneFightResultRequest.h"

LQ_NAMESPACE


ZoneFightResultRequest::ZoneFightResultRequest() : LQClientRequest(), _zoneID(0), _isRaids(0), _isWin(0), _recStar(0) {
}

ZoneFightResultRequest::ZoneFightResultRequest(	int zoneID,	bool isRaids,	bool isWin,	short recStar)
	: LQClientRequest(), _zoneID(zoneID),
_isRaids(isRaids),
_isWin(isWin),
_recStar(recStar) {
}

ZoneFightResultRequest::~ZoneFightResultRequest() {
}

LQSocketBuffer* ZoneFightResultRequest::toMessage() {
	setMsgCode(11107);
	writeInt(_zoneID); //关卡ID
	writeBoolean(_isRaids); //是否是扫荡
	if(!_isRaids) { //不是扫荡
		writeBoolean(_isWin); //战斗结果,是否胜利
		if(_isWin) { //战斗胜利
			writeByte(_recStar); //获得的星数
		} //end if 战斗胜利
	} //end if 不是扫荡
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
