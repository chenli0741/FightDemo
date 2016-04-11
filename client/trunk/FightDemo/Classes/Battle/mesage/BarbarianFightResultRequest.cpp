//
// BarbarianFightResultRequest.h
// 南蛮入侵主界面
// 消息号：12705
// 
// Created by  globjoy at 2015-03-23 
// 
// 

#include "BarbarianFightResultRequest.h"

LQ_NAMESPACE


BarbarianFightResultRequest::BarbarianFightResultRequest() : LQClientRequest(), _hurtHp(0) {
}

BarbarianFightResultRequest::BarbarianFightResultRequest(	string hurtHp)
	: LQClientRequest(), _hurtHp(hurtHp) {
}

BarbarianFightResultRequest::~BarbarianFightResultRequest() {
}

LQSocketBuffer* BarbarianFightResultRequest::toMessage() {
	setMsgCode(12705);
	writeString(_hurtHp); //伤血值
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
