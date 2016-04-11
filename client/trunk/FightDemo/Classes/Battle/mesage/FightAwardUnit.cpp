//
// FightAwardUnit.h
// 战斗奖励数据
// 消息号：
// 
// Created by  globjoy at 2014-05-28 
// 
// 

#include "FightAwardUnit.h"

LQ_NAMESPACE


FightAwardUnit::FightAwardUnit(LQSocketBuffer *buffer) : LQUnitResponse(buffer), _exp(0), _cash(0), _soul(0), _awardCount(0) {
	readMessage();
}

FightAwardUnit::~FightAwardUnit() {
	deleteInVector(_itemUnits);
}

// read message from socket channel 
void FightAwardUnit::readMessage() {
	LQUnitResponse::readMessage();

	_exp = readInt(); //奖励经验
	_cash = readInt(); //奖励金钱
	_soul = readInt(); //奖励将魂
	_awardCount = readShort(); //奖励道具个数
	if(_awardCount >0 ) { //奖励道具个数>0
		for(int i = 0; i < _awardCount; i++){
			ItemUnit* itemUnit = new ItemUnit(readBytes());
			_itemUnits.push_back(itemUnit);
		}
	} //end if 奖励道具个数>0
}

LQ_NAMESPACE_END
