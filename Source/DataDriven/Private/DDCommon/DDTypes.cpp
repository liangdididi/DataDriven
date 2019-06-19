// Fill out your copyright notice in the Description page of Project Settings.

#include "DDTypes.h"


//
//void DDMsgNode::UnRegisterFun(int32 FunID)
//{
//	//直接从序列里移除对象即可
//	DDAnyFun* DesPtr = *FunQuene.Find(FunID);
//	FunQuene.Remove(FunID);
//	delete DesPtr;
//}

//bool DDMsgNode::IsBound()
//{
//	return FunQuene.Num() > 0;
//}


//void DDMsgQueue::UnRegisterCallPort(FString CallName)
//{
//	//让对应的接口计数器减一, 如果计数器小于等于0, 就移除调用接口
//	MsgQuene.Find(CallName)->CallCount--;
//	if(MsgQuene.Find(CallName)->CallCount <= 0)
//		MsgQuene.Remove(CallName);
//}
//
//void DDMsgQueue::UnRegisterFunPort(FString CallName, int32 FunID)
//{
//	MsgQuene.Find(CallName)->UnRegisterFun(FunID);
//}


//bool DDMsgQueue::IsBound(FString CallName)
//{
//	return MsgQuene.Find(CallName)->IsBound();
//}


//void DDFunHandle::UnRegister()
//{
//	if (MsgQuene) MsgQuene->UnRegisterFunPort(CallName, FunID);
//}

TSharedPtr<DDRecord> DDRecord::RecordInst = NULL;

TSharedPtr<DDRecord> DDRecord::Get()
{
	if (!RecordInst.IsValid())
		RecordInst = MakeShareable(new DDRecord());
	return RecordInst;
}

