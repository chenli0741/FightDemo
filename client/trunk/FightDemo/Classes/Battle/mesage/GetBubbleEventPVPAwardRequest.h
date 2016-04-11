//
// GetBubbleEventPVPAwardRequest.h
// 获取气泡事件PVP奖励
// 消息号：12241
// 
// Created by  globjoy at 2015-03-04 
// 
// 

#ifndef __GLOBJOY__GetBubbleEventPVPAwardRequest__
#define __GLOBJOY__GetBubbleEventPVPAwardRequest__

#include "../../Socket/LQClientRequest.h"

STD_NAMESPACE_USE
LQ_NAMESPACE


class GetBubbleEventPVPAwardRequest : public LQClientRequest {

public:
	GetBubbleEventPVPAwardRequest();
	/**
	 * 获取气泡事件PVP奖励
	 * @param eventID 事件ID
	 * @param state 事件状态 ,0:战败,1:战胜
	 */
	GetBubbleEventPVPAwardRequest(	int eventID,	short state);
	~GetBubbleEventPVPAwardRequest();

	// the message data = sizeof(short) + body(char*)
	// the returan value (buffer object) should delete manualy
	LQSocketBuffer* toMessage();

	inline void setEventID(int eventID) { _eventID = eventID; }
	inline void setState(short state) { _state = state; }

private:
	int _eventID; // 事件ID
	short _state; // 事件状态 ,2:战败,3:战胜

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__GetBubbleEventPVPAwardRequest__) */
