//
// ChallengeRivalRequest.h
// 挑战对手
// 消息号：11855
// 
// Created by  globjoy at 2015-07-11 
// 
// 

#ifndef __GLOBJOY__ChallengeRivalRequest__
#define __GLOBJOY__ChallengeRivalRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class ChallengeRivalRequest : public LQClientRequest {

public:
	ChallengeRivalRequest();
	/**
	 * 挑战对手
	 * @param rivalRoleId 对手角色ID
	 * @param isWin 是否胜利，如果胜利了重新推送11852
	 * @param fightData 战斗数据
	 */
	ChallengeRivalRequest(	string rivalRoleId,	bool isWin,	string fightData);
	~ChallengeRivalRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setRivalRoleId(string rivalRoleId) { _rivalRoleId = rivalRoleId; }
	inline void setIsWin(bool isWin) { _isWin = isWin; }
	inline void setFightData(string fightData) { _fightData = fightData; }

private:
	string _rivalRoleId; // 对手角色ID
	bool _isWin; // 是否胜利，如果胜利了重新推送11852
	string _fightData; // 战斗数据

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__ChallengeRivalRequest__) */
