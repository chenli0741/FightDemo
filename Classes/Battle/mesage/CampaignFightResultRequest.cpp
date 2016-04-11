//
// CampaignFightResultRequest.h
// 征战挑战
// 消息号：12811
// 
// Created by  globjoy at 2015-06-27 
// 
// 

#include "CampaignFightResultRequest.h"

LQ_NAMESPACE


CampaignFightResultRequest::CampaignFightResultRequest() : LQClientRequest(), _isWin(0), _recStar(0) {
}

CampaignFightResultRequest::CampaignFightResultRequest(	bool isWin,	short recStar,	string selfResult,	string vivalResult)
	: LQClientRequest(), _isWin(isWin),
_recStar(recStar),
_selfResult(selfResult),
_vivalResult(vivalResult) {
}

CampaignFightResultRequest::~CampaignFightResultRequest() {
}

LQSocketBuffer* CampaignFightResultRequest::toMessage() {
	setMsgCode(12811);
	writeBoolean(_isWin); //战斗结果,是否胜利
	if(_isWin) { //战斗胜利
		writeByte(_recStar); //获得的星数
	} //end if 战斗胜利
	writeString(_selfResult); //我方战斗结果串，（伤血，当前怒气；）
	writeString(_vivalResult); //敌人战斗结果串，（伤血，当前怒气；）
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
