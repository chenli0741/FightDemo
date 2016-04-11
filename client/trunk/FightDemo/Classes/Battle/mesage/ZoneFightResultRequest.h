//
// ZoneFightResultRequest.h
// 关卡战斗结果
// 消息号：11107
// 
// Created by  globjoy at 2014-07-27 
// 
// 

#ifndef __GLOBJOY__ZoneFightResultRequest__
#define __GLOBJOY__ZoneFightResultRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class ZoneFightResultRequest : public LQClientRequest {

public:
	ZoneFightResultRequest();
	/**
	 * 关卡战斗结果
	 * @param zoneID 关卡ID
	 * @param isRaids 是否是扫荡
	 * @param isWin 战斗结果,是否胜利
	 * @param recStar 获得的星数
	 */
	ZoneFightResultRequest(	int zoneID,	bool isRaids,	bool isWin,	short recStar);
	~ZoneFightResultRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setZoneID(int zoneID) { _zoneID = zoneID; }
	inline void setIsRaids(bool isRaids) { _isRaids = isRaids; }
	inline void setIsWin(bool isWin) { _isWin = isWin; }
	inline void setRecStar(short recStar) { _recStar = recStar; }

private:
	int _zoneID; // 关卡ID
	bool _isRaids; // 是否是扫荡
	bool _isWin; // 战斗结果,是否胜利
	short _recStar; // 获得的星数

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__ZoneFightResultRequest__) */
