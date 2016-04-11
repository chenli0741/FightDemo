//
// ItemUnit.h
// 赠送物品数据
// 消息号：
// 
// Created by  globjoy at 2014-07-27 
// 
// 

#include "ItemUnit.h"

LQ_NAMESPACE


ItemUnit::ItemUnit(LQSocketBuffer *buffer) : LQUnitResponse(buffer), _type(0), _itemID(0), _count(0) {
	readMessage();
}

ItemUnit::~ItemUnit() {
}

// read message from socket channel 
void ItemUnit::readMessage() {
	LQUnitResponse::readMessage();

	_type = readByte(); //赠送物品类型: 1-15
	_itemID = readInt(); //赠送物品resID
	_count = readInt(); //赠送物品数量
}

LQ_NAMESPACE_END
