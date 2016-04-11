//
// FightResultRequest.h
// 战斗结果
// 消息号：11139
// 
// Created by  globjoy at 2015-07-11 
// 
// 

#include "FightResultRequest.h"

LQ_NAMESPACE


FightResultRequest::FightResultRequest() : LQClientRequest(), _type(0), _isWin(0), _recStar(0) {
}

FightResultRequest::FightResultRequest(	short type,	bool isWin,	short recStar,	string param,	string fightData)
	: LQClientRequest(), _type(type),
_isWin(isWin),
_recStar(recStar),
_param(param),
_fightData(fightData) {
}

FightResultRequest::~FightResultRequest() {
}

LQSocketBuffer* FightResultRequest::toMessage() {
	setMsgCode(11139);
	writeShort(_type); //战斗类型
	writeBoolean(_isWin); //战斗结果,是否胜利
	if(_isWin) { //战斗胜利
		writeByte(_recStar); //获得的星数
	} //end if 战斗胜利
	writeString(_param); //对手或者关卡ID,如果有冗余参数再往后拼，需约定
	writeString(_fightData); //战斗数据
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
