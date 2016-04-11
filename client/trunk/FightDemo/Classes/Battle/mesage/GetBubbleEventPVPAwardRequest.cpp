//
// GetBubbleEventPVPAwardRequest.h
// 获取气泡事件PVP奖励
// 消息号：12241
// 
// Created by  globjoy at 2015-03-04 
// 
// 

#include "GetBubbleEventPVPAwardRequest.h"

LQ_NAMESPACE


GetBubbleEventPVPAwardRequest::GetBubbleEventPVPAwardRequest() : LQClientRequest(), _eventID(0), _state(0) {
}

GetBubbleEventPVPAwardRequest::GetBubbleEventPVPAwardRequest(	int eventID,	short state)
	: LQClientRequest(), _eventID(eventID),
_state(state) {
}

GetBubbleEventPVPAwardRequest::~GetBubbleEventPVPAwardRequest() {
}

LQSocketBuffer* GetBubbleEventPVPAwardRequest::toMessage() {
	setMsgCode(12241);
	writeInt(_eventID); //事件ID
	writeByte(_state); //事件状态 ,2:战败,3:战胜
	return LQClientRequest::toMessage();
}


LQ_NAMESPACE_END
