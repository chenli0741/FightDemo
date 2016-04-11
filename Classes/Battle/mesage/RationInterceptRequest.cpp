//
// RationInterceptRequest.h
// 偷军粮拦截界面
// 消息号：12287
// 
// Created by  globjoy at 2015-03-05 
// 
// 

#include "RationInterceptRequest.h"

LQ_NAMESPACE


RationInterceptRequest::RationInterceptRequest() : LQClientRequest(), _cityResId(0), _isWin(0) {
}

RationInterceptRequest::RationInterceptRequest(	int cityResId,	string roleId,	string fightRoleId,	bool isWin)
	: LQClientRequest(), _cityResId(cityResId),
_roleId(roleId),
_fightRoleId(fightRoleId),
_isWin(isWin) {
}

RationInterceptRequest::~RationInterceptRequest() {
}

LQSocketBuffer* RationInterceptRequest::toMessage() {
	setMsgCode(12287);
	writeInt(_cityResId); //偷军粮掠夺城池
	writeString(_roleId); //拦截对象roleId
	writeString(_fightRoleId); //战斗的roleID,如果是自己战斗的，则为空
	writeBoolean(_isWin); //是否赢了
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
