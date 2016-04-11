//
// ZoneFightResultResponse.h
// 关卡战斗结果
// 消息号：11108
// 
// Created by  globjoy at 2014-06-06 
// 
// 

#ifndef __GLOBJOY__ZoneFightResultResponse__
#define __GLOBJOY__ZoneFightResultResponse__

#include "../../Socket/LQServerResponse.h"
#include "../../Socket/LQInnerClassResponse.h"
#include "FightAwardUnit.h"
#include "ErrorOperationUnit.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class ZoneFightResultResponse : public LQServerResponse {

DECLARE_CLASS(ZoneFightResultResponse);

public:
	ZoneFightResultResponse();
	~ZoneFightResultResponse();

	// read message from socket channel 
	void readMessage(const char* data, size_t length);

	// 是否成功
	inline bool isSuccess() { return _isSuccess; }
	// 是否是扫荡
	inline bool isRaids() { return _isRaids; }
	// 战斗结果个数
	inline short resultCount() { return _resultCount; }
	// 战斗奖励单元
	inline vector<FightAwardUnit*>& fightAwardUnits() { return _fightAwardUnits; }
	// 失败原因
	inline ErrorOperationUnit* errorUnit() { return _errorUnit; }

private:
	bool _isSuccess; // 是否成功
	bool _isRaids; // 是否是扫荡
	short _resultCount; // 战斗结果个数
	vector<FightAwardUnit*> _fightAwardUnits; // 战斗奖励单元
	ErrorOperationUnit* _errorUnit; // 失败原因

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__ZoneFightResultResponse__) */
