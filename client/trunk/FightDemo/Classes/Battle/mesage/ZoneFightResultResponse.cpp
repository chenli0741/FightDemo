//
// ZoneFightResultResponse.h
// 关卡战斗结果
// 消息号：11108
// 
// Created by  globjoy at 2014-06-06 
// 
// 

#include "ZoneFightResultResponse.h"

LQ_NAMESPACE

IMPLEMENT_CLASS(ZoneFightResultResponse);

ZoneFightResultResponse::ZoneFightResultResponse() : LQServerResponse(), _isSuccess(0), _isRaids(0), _resultCount(0), _errorUnit(NULL) {
}

ZoneFightResultResponse::~ZoneFightResultResponse() {
	deleteInVector(_fightAwardUnits);
	delete _errorUnit;
}

// read message from socket channel 
void ZoneFightResultResponse::readMessage(const char* data, size_t length) {
	LQServerResponse::readMessage(data, length);

	_isSuccess = readBoolean(); //是否成功
	if(_isSuccess) { //成功
		_isRaids = readBoolean(); //是否是扫荡
		_resultCount = readShort(); //战斗结果个数
		if(_resultCount >0 ) { //战斗结果>0
			for(int i = 0; i < _resultCount; i++){
				FightAwardUnit* fightAwardUnit = new FightAwardUnit(readBytes());
				_fightAwardUnits.push_back(fightAwardUnit);
			}
		} //end if 战斗结果>0
	} //end if 成功
	if(!_isSuccess) { //失败
		_errorUnit = new ErrorOperationUnit(readBytes()); //失败原因
	} //end if 失败
}


LQ_NAMESPACE_END
