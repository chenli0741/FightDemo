//
// BiogFightResultRequest.h
// 列传关卡战斗结果
// 消息号：11125
// 
// Created by  globjoy at 2014-09-17 
// 
// 

#ifndef __GLOBJOY__BiogFightResultRequest__
#define __GLOBJOY__BiogFightResultRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class BiogFightResultRequest : public LQClientRequest {

public:
	BiogFightResultRequest();
	/**
	 * 列传关卡战斗结果
	 * @param zoneID 关卡ID
	 * @param isWin 战斗结果,是否胜利
	 */
	BiogFightResultRequest(	int zoneID,	bool isWin);
	~BiogFightResultRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setZoneID(int zoneID) { _zoneID = zoneID; }
	inline void setIsWin(bool isWin) { _isWin = isWin; }

private:
	int _zoneID; // 关卡ID
	bool _isWin; // 战斗结果,是否胜利

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__BiogFightResultRequest__) */
