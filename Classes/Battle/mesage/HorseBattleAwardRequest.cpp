//
// HorseBattleAwardRequest.h
// 碎片掠夺翻拍奖励
// 消息号：11515
// 
// Created by  globjoy at 2014-11-14 
// 
// 

#include "HorseBattleAwardRequest.h"

LQ_NAMESPACE


HorseBattleAwardRequest::HorseBattleAwardRequest() : LQClientRequest() {
}

HorseBattleAwardRequest::~HorseBattleAwardRequest() {
}

LQSocketBuffer* HorseBattleAwardRequest::toMessage() {
	setMsgCode(11515);
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
