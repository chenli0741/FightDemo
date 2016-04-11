//
// FightResultRequest.h
// 战斗结果
// 消息号：11139
// 
// Created by  globjoy at 2015-07-11 
// 
// 

#ifndef __GLOBJOY__FightResultRequest__
#define __GLOBJOY__FightResultRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class FightResultRequest : public LQClientRequest {

public:
	FightResultRequest();
	/**
	 * 战斗结果
	 * @param type 战斗类型
	 * @param isWin 战斗结果,是否胜利
	 * @param recStar 获得的星数
	 * @param param 对手或者关卡ID,如果有冗余参数再往后拼，需约定
	 * @param fightData 战斗数据
	 */
	FightResultRequest(	short type,	bool isWin,	short recStar,	string param,	string fightData);
	~FightResultRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setType(short type) { _type = type; }
	inline void setIsWin(bool isWin) { _isWin = isWin; }
	inline void setRecStar(short recStar) { _recStar = recStar; }
	inline void setParam(string param) { _param = param; }
	inline void setFightData(string fightData) { _fightData = fightData; }

private:
	short _type; // 战斗类型
	bool _isWin; // 战斗结果,是否胜利
	short _recStar; // 获得的星数
	string _param; // 对手或者关卡ID,如果有冗余参数再往后拼，需约定
	string _fightData; // 战斗数据

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__FightResultRequest__) */
