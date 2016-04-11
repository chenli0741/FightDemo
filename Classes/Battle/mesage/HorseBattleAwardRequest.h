//
// HorseBattleAwardRequest.h
// 碎片掠夺翻拍奖励
// 消息号：11515
// 
// Created by  globjoy at 2014-11-14 
// 
// 

#ifndef __GLOBJOY__HorseBattleAwardRequest__
#define __GLOBJOY__HorseBattleAwardRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class HorseBattleAwardRequest : public LQClientRequest {

public:
	HorseBattleAwardRequest();
	~HorseBattleAwardRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();


private:

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__HorseBattleAwardRequest__) */
