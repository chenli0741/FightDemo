//
// ChallengeRivalRequest.h
// 挑战对手
// 消息号：11855
// 
// Created by  globjoy at 2015-07-11 
// 
// 

#include "ChallengeRivalRequest.h"

LQ_NAMESPACE


ChallengeRivalRequest::ChallengeRivalRequest() : LQClientRequest(), _isWin(0) {
}

ChallengeRivalRequest::ChallengeRivalRequest(	string rivalRoleId,	bool isWin,	string fightData)
	: LQClientRequest(), _rivalRoleId(rivalRoleId),
_isWin(isWin),
_fightData(fightData) {
}

ChallengeRivalRequest::~ChallengeRivalRequest() {
}

LQSocketBuffer* ChallengeRivalRequest::toMessage() {
	setMsgCode(11855);
	writeString(_rivalRoleId); //对手角色ID
	writeBoolean(_isWin); //是否胜利，如果胜利了重新推送11852
	writeString(_fightData); //战斗数据
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
