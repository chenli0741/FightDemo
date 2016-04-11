//
// ItemUnit.h
// 赠送物品数据
// 消息号：
// 
// Created by  globjoy at 2014-07-27 
// 
// 

#ifndef __GLOBJOY__ItemUnit__
#define __GLOBJOY__ItemUnit__

#include "../../Socket/LQUnitResponse.h"

STD_NAMESPACE_USE
LQ_NAMESPACE

class ItemUnit : public LQUnitResponse {

public:
	ItemUnit(LQSocketBuffer *buffer);
	~ItemUnit();

	// read message from socket channel 
	void readMessage();

	// 赠送物品类型: 1-15
	inline short type() { return _type; }
	// 赠送物品resID
	inline int itemID() { return _itemID; }
	// 赠送物品数量
	inline int count() { return _count; }

private:
	short _type; // 赠送物品类型: 1-15
	int _itemID; // 赠送物品resID
	int _count; // 赠送物品数量

};

LQ_NAMESPACE_END

#endif /* defined(__GLOBJOY__ItemUnit__) */
