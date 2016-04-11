//
// ExpLevelFightResultRequest.h
// 经验关卡战斗结果
// 消息号：11133
// 
// Created by  globjoy at 2014-09-17 
// 
// 

#ifndef __GLOBJOY__ExpLevelFightResultRequest__
#define __GLOBJOY__ExpLevelFightResultRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class ExpLevelFightResultRequest : public LQClientRequest {

public:
	ExpLevelFightResultRequest();
	/**
	 * 经验关卡战斗结果
	 * @param resId 关卡ID
	 * @param isWin 战斗结果,是否胜利
	 */
	ExpLevelFightResultRequest(	int resId,	bool isWin);
	~ExpLevelFightResultRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setResId(int resId) { _resId = resId; }
	inline void setIsWin(bool isWin) { _isWin = isWin; }

private:
	int _resId; // 关卡ID
	bool _isWin; // 战斗结果,是否胜利

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__ExpLevelFightResultRequest__) */
