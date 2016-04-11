//
// HorseBattleFightResultRequest.h
// 坐骑碎片掠夺数据
// 消息号：11513
// 
// Created by  globjoy at 2014-09-22 
// 
// 

#ifndef __GLOBJOY__HorseBattleFightResultRequest__
#define __GLOBJOY__HorseBattleFightResultRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class HorseBattleFightResultRequest : public LQClientRequest {

public:
	HorseBattleFightResultRequest();
	/**
	 * 坐骑碎片掠夺数据
	 * @param fragmentResId 坐骑碎片资源ID
     * @param fragmentID 被抢人ID
	 * @param isWin 战斗结果,是否胜利
	 */
	HorseBattleFightResultRequest(	int fragmentResId, std::string fragmentID,bool isWin);
	~HorseBattleFightResultRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setFragmentResId(int fragmentResId) { _fragmentResId = fragmentResId; }
    inline void setFragmentId(std::string fragmentID) { _fragmentID = fragmentID; }
	inline void setIsWin(bool isWin) { _isWin = isWin; }
    

private:
	int _fragmentResId; // 坐骑碎片资源ID
    std::string _fragmentID;
	bool _isWin; // 战斗结果,是否胜利

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__HorseBattleFightResultRequest__) */
