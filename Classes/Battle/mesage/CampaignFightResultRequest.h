//
// CampaignFightResultRequest.h
// 征战挑战
// 消息号：12811
// 
// Created by  globjoy at 2015-06-27 
// 
// 

#ifndef __GLOBJOY__CampaignFightResultRequest__
#define __GLOBJOY__CampaignFightResultRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class CampaignFightResultRequest : public LQClientRequest {

public:
	CampaignFightResultRequest();
	/**
	 * 征战挑战
	 * @param isWin 战斗结果,是否胜利
	 * @param recStar 获得的星数
	 * @param selfResult 我方战斗结果串，（伤血，当前怒气；）
	 * @param vivalResult 敌人战斗结果串，（伤血，当前怒气；）
	 */
	CampaignFightResultRequest(	bool isWin,	short recStar,	string selfResult,	string vivalResult);
	~CampaignFightResultRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setIsWin(bool isWin) { _isWin = isWin; }
	inline void setRecStar(short recStar) { _recStar = recStar; }
	inline void setSelfResult(string selfResult) { _selfResult = selfResult; }
	inline void setVivalResult(string vivalResult) { _vivalResult = vivalResult; }

private:
	bool _isWin; // 战斗结果,是否胜利
	short _recStar; // 获得的星数
	string _selfResult; // 我方战斗结果串，（伤血，当前怒气；）
	string _vivalResult; // 敌人战斗结果串，（伤血，当前怒气；）

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__CampaignFightResultRequest__) */
