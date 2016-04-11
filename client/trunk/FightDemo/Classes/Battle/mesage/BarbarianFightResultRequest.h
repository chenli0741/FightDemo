//
// BarbarianFightResultRequest.h
// 南蛮入侵主界面
// 消息号：12705
// 
// Created by  globjoy at 2015-03-23 
// 
// 

#ifndef __GLOBJOY__BarbarianFightResultRequest__
#define __GLOBJOY__BarbarianFightResultRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class BarbarianFightResultRequest : public LQClientRequest {

public:
	BarbarianFightResultRequest();
	/**
	 * 南蛮入侵主界面
	 * @param hurtHp 伤血值
	 */
	BarbarianFightResultRequest(	string hurtHp);
	~BarbarianFightResultRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setHurtHp(string hurtHp) { _hurtHp = hurtHp; }

private:
	string _hurtHp; // 伤血值

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__BarbarianFightResultRequest__) */
