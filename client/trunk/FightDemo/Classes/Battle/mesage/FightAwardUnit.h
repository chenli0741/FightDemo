//
// FightAwardUnit.h
// 战斗奖励数据
// 消息号：
// 
// Created by  globjoy at 2014-05-28 
// 
// 

#ifndef __GLOBJOY__FightAwardUnit__
#define __GLOBJOY__FightAwardUnit__

#include "../../Socket/LQUnitResponse.h"
#include "ItemUnit.h"

STD_NAMESPACE_USE
LQ_NAMESPACE

class FightAwardUnit : public LQUnitResponse {

public:
	FightAwardUnit(LQSocketBuffer *buffer);
	~FightAwardUnit();

	// read message from socket channel 
	void readMessage();

	// 奖励经验
	inline int exp() { return _exp; }
	// 奖励金钱
	inline int cash() { return _cash; }
	// 奖励将魂
	inline int soul() { return _soul; }
	// 奖励道具个数
	inline short awardCount() { return _awardCount; }
	// 奖励单元
	inline vector<ItemUnit*>& itemUnits() { return _itemUnits; }

private:
	int _exp; // 奖励经验
	int _cash; // 奖励金钱
	int _soul; // 奖励将魂
	short _awardCount; // 奖励道具个数
	vector<ItemUnit*> _itemUnits; // 奖励单元

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__FightAwardUnit__) */
