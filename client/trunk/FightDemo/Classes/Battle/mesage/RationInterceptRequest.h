//
// RationInterceptRequest.h
// 偷军粮拦截界面
// 消息号：12287
// 
// Created by  globjoy at 2015-03-05 
// 
// 

#ifndef __GLOBJOY__RationInterceptRequest__
#define __GLOBJOY__RationInterceptRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class RationInterceptRequest : public LQClientRequest {

public:
	RationInterceptRequest();
	/**
	 * 偷军粮拦截界面
	 * @param cityResId 偷军粮掠夺城池
	 * @param roleId 拦截对象roleId
	 * @param fightRoleId 战斗的roleID,如果是自己战斗的，则为空
	 * @param isWin 是否赢了
	 */
	RationInterceptRequest(	int cityResId,	string roleId,	string fightRoleId,	bool isWin);
	~RationInterceptRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setCityResId(int cityResId) { _cityResId = cityResId; }
	inline void setRoleId(string roleId) { _roleId = roleId; }
	inline void setFightRoleId(string fightRoleId) { _fightRoleId = fightRoleId; }
	inline void setIsWin(bool isWin) { _isWin = isWin; }

private:
	int _cityResId; // 偷军粮掠夺城池
	string _roleId; // 拦截对象roleId
	string _fightRoleId; // 战斗的roleID,如果是自己战斗的，则为空
	bool _isWin; // 是否赢了

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__RationInterceptRequest__) */
