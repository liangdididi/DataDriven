// Fill out your copyright notice in the Description page of Project Settings.

#include "DDModel.h"
#include "DDOO.h"
#include "DDModule.h"



//void UDDModel::RegisterSuperModule(UDDModule* SuperMod)
//{
//	SuperModule = SuperMod;
//}
//
//void UDDModel::RegisterChildModule(UDDModule* ChildMod)
//{
//	//添加单个模组
//	ChildModule.Add(ChildMod->ModuleIndex, ChildMod);
//}

void UDDModel::RegisterObject(IDDOO* ObjectInst)
{
	//如果不重复就添加到模组对象
	if (!ObjectGroup.Contains(ObjectInst->GetObjectName()))
	{
		ObjectGroup.Add(ObjectInst->GetObjectName(), ObjectInst);
		//添加到模组类对象集
		FName ObejctClassName = ObjectInst->GetClassName();
		if (ObjectClassGroup.Contains(ObejctClassName))
		{
			ObjectClassGroup.Find(ObejctClassName)->Add(ObjectInst);
		}
		else {
			TArray<IDDOO*> ObejctArray;
			ObjectClassGroup.Add(ObejctClassName, ObejctArray);
			ObjectClassGroup.Find(ObejctClassName)->Add(ObjectInst);
		}
		//添加对象到生命周期组
		ObjectLifeGroup.Add(ObjectInst);
	}
	else {
		//如果有重复的就输出个Bug
		DDH::Debug() << "Object Repeated --> " << ObjectInst->GetObjectName() << DDH::Endl();
	}
}

void UDDModel::ModelTick(float DeltaSeconds)
{
	//运行生命周期组的生命周期函数
	TArray<IDDOO*> TempObjectGroup;
	for (int i = 0; i < ObjectLifeGroup.Num(); ++i)
	{
		//如果激活成功
		if (ObjectLifeGroup[i]->ActiveLife())
			TempObjectGroup.Add(ObjectLifeGroup[i]);
	}
	//将运行完生命周期的对象移出生命周期组,并且将运行Tick运行的对象添加到Tick组
	for (int i = 0; i < TempObjectGroup.Num(); ++i)
	{
		ObjectLifeGroup.Remove(TempObjectGroup[i]);
		if (TempObjectGroup[i]->IsAllowTickEvent)
			ObjectTickGroup.Add(TempObjectGroup[i]);
	}
	//运行Tick组的Tick函数
	for (int i = 0; i < ObjectTickGroup.Num(); ++i)
		ObjectTickGroup[i]->DDTick(DeltaSeconds);


	//运行释放对象组的释放函数,运行完后清空释放对象组,因为这些指针会释放,不需要再保存
	for (int i = 0; i < ObjectReleaseGroup.Num(); ++i)
		ObjectReleaseGroup[i]->OnRealse();
	ObjectReleaseGroup.Empty();


	//清空临时序列
	TempObjectGroup.Empty();
	//处理预函数,如果是稳定状态,就转到临时对象组
	for (int i = 0; i < PreObjectDestroyGroup.Num(); ++i)
	{
		if (PreObjectDestroyGroup[i]->RunState == EBaseObjectState::Stable)
		{
			//添加稳定运行状态的对象到临时对象组
			TempObjectGroup.Add(PreObjectDestroyGroup[i]);
			//添加稳定运行状态的对象到销毁进程组
			ObjectDestroyGroup.Add(PreObjectDestroyGroup[i]);
		}
	}
	//从预处理组中清除进入稳定状态的数组
	for (int i = 0; i < TempObjectGroup.Num(); ++i)
	{
		PreObjectDestroyGroup.Remove(TempObjectGroup[i]);
	}

	//清空临时序列
	TempObjectGroup.Empty();
	//运行销毁对象组的销毁函数
	for (int i = 0; i < ObjectDestroyGroup.Num(); ++i) {
		//如果已经运行到释放状态
		if (ObjectDestroyGroup[i]->DestroyLife()) {
			//添加到释放对象组
			ObjectReleaseGroup.Add(ObjectDestroyGroup[i]);
			//从ObjectGroup与ObjectTickGroup和ObjectClassGroup移除数据
			ObjectGroup.Remove(ObjectDestroyGroup[i]->GetObjectName());
			ObjectTickGroup.Remove(ObjectDestroyGroup[i]);
			if (ObjectClassGroup.Contains(ObjectDestroyGroup[i]->GetClassName())) 
				ObjectClassGroup.Find(ObjectDestroyGroup[i]->GetClassName())->Remove(ObjectDestroyGroup[i]);
			//添加销毁完毕对象到临时对象组
			TempObjectGroup.Add(ObjectDestroyGroup[i]);
		}
	}
	//从销毁组中清除进入销毁完毕的数组
	for (int i = 0; i < TempObjectGroup.Num(); ++i)
		ObjectDestroyGroup.Remove(TempObjectGroup[i]);
}

void UDDModel::GetSelfObject(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	for (int i = 0; i < ObjectNameGroup.Num(); ++i)
		if (ObjectGroup.Contains(ObjectNameGroup[i]))
			TargetObjectGroup.Add(*ObjectGroup.Find(ObjectNameGroup[i]));
}

int32 UDDModel::GetOtherObject(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	//把对象组里与传入对象名组不相同的对象全部存到TargetObjectGroup
	for (TMap<FName, IDDOO*>::TIterator It(ObjectGroup); It; ++It) {
		bool IsSame = false;
		for (int i = 0; i < ObjectNameGroup.Num(); ++i)
		{
			//只要检查出相同的就马上跳出
			if (ObjectNameGroup[i].IsEqual(It->Key)) { IsSame = true; break; }
		}
		if (!IsSame) TargetObjectGroup.Add(It->Value);
	}
	//返回全部对象的数量
	return GetObjectGroupNum();
}

int32 UDDModel::GetClassOtherObject(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	//现获取类名
	FName ObjectClassName = (*ObjectGroup.Find(ObjectNameGroup[0]))->GetClassName();
	//迭代类组对象
	for (TArray<IDDOO*>::TIterator It(*ObjectClassGroup.Find(ObjectClassName)); It; ++It)
	{
		bool IsSame = false;
		for (int i = 0; i < ObjectNameGroup.Num(); ++i)
		{
			//如果存储的对象相同就跳出
			if (*It == *ObjectGroup.Find(ObjectNameGroup[i])) { IsSame = true; break; }
		}
		if (!IsSame) TargetObjectGroup.Add(*It);
	}
	//返回这个类的对象数量
	return GetClassObjectGroupNum(ObjectClassName);
}

void UDDModel::GetSelfClass(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	for (int i = 0; i < ObjectNameGroup.Num(); ++i) {
		//如果不包含这个类,直接跳到下一个循环
		if (!ObjectClassGroup.Contains(ObjectNameGroup[i])) continue;
		//循环这个类去填入TargetObjectGroup
		for (TArray<IDDOO*>::TIterator It(*ObjectClassGroup.Find(ObjectNameGroup[i])); It; ++It)
			TargetObjectGroup.Add(*It);
	}
}

void UDDModel::GetOtherClass(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup)
{
	for (TMap<FName, TArray<IDDOO*>>::TIterator It(ObjectClassGroup); It; ++It) {
		bool IsSame = false;
		for (int i = 0; i < ObjectNameGroup.Num(); ++i) {
			if (ObjectNameGroup[i].IsEqual(It->Key)) { IsSame = true; break; }
		}
		//如果迭代到的这个类组与传入的类名相同,跳到下一个循环
		if (IsSame) continue;
		//如果迭代到的这个类组与传入的类名不相同,添加到TargetObjectGroup
		for (TArray<IDDOO*>::TIterator Ih(It->Value); Ih; ++Ih)
			TargetObjectGroup.Add(*Ih);
	}
}

void UDDModel::GetAll(TArray<IDDOO*>& TargetObjectGroup)
{
	//把对象组里与传入对象名组不相同的对象全部存到TargetObjectGroup
	for (TMap<FName, IDDOO*>::TIterator It(ObjectGroup); It; ++It)
		TargetObjectGroup.Add(It->Value);
}

void UDDModel::DestroyObject(EAgreementType Agreement, TArray<FName> ObjectNameGroup)
{
	TArray<IDDOO*> TargetObjectGroup;
	switch (Agreement)
	{
	case EAgreementType::SelfObject:
		GetSelfObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::OtherObject:
		GetOtherObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::ClassOtherObject:
		GetClassOtherObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::SelfClass:
		GetSelfClass(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::OtherClass:
		GetOtherClass(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::All:
		GetAll(TargetObjectGroup);
		break;
	}
	//迭代将对象添加到PreObjectDestroyGroup或者ObjectDestroyGroup
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		if (!ObjectDestroyGroup.Contains(TargetObjectGroup[i]) && !PreObjectDestroyGroup.Contains(TargetObjectGroup[i]))
		{
			//如果是稳定状态就添加到ObjectDestroyGroup,如果是激活状态就添加到PreObjectDestroyGroup
			switch (TargetObjectGroup[i]->RunState)
			{
			case EBaseObjectState::Active:
				PreObjectDestroyGroup.Add(TargetObjectGroup[i]);
				break;
			case EBaseObjectState::Stable:
				ObjectDestroyGroup.Add(TargetObjectGroup[i]);
				break;
			}
		}
	}
}

void UDDModel::EnableObject(EAgreementType Agreement, TArray<FName> ObjectNameGroup)
{
	TArray<IDDOO*> TargetObjectGroup;
	switch (Agreement)
	{
	case EAgreementType::SelfObject:
		GetSelfObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::OtherObject:
		GetOtherObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::ClassOtherObject:
		GetClassOtherObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::SelfClass:
		GetSelfClass(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::OtherClass:
		GetOtherClass(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::All:
		GetAll(TargetObjectGroup);
		break;
	}
	//迭代运行
	for (int i = 0; i < TargetObjectGroup.Num(); ++i) {
		//如果这个对象处于稳定与失活状态就运行他的激活状态函数
		if (TargetObjectGroup[i]->RunState == EBaseObjectState::Stable && TargetObjectGroup[i]->LifeState == EBaseObjectLife::Disable)
			TargetObjectGroup[i]->OnEnable();
	}
}

void UDDModel::DisableObject(EAgreementType Agreement, TArray<FName> ObjectNameGroup)
{
	TArray<IDDOO*> TargetObjectGroup;
	switch (Agreement)
	{
	case EAgreementType::SelfObject:
		GetSelfObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::OtherObject:
		GetOtherObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::ClassOtherObject:
		GetClassOtherObject(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::SelfClass:
		GetSelfClass(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::OtherClass:
		GetOtherClass(ObjectNameGroup, TargetObjectGroup);
		break;
	case EAgreementType::All:
		GetAll(TargetObjectGroup);
		break;
	}
	//迭代运行
	for (int i = 0; i < TargetObjectGroup.Num(); ++i) {
		//如果这个对象处于稳定与激活状态就运行他的失活状态函数
		if (TargetObjectGroup[i]->RunState == EBaseObjectState::Stable && TargetObjectGroup[i]->LifeState == EBaseObjectLife::Enable)
			TargetObjectGroup[i]->OnDisable();
	}
}

int32 UDDModel::GetObjectGroupNum() const
{
	return ObjectGroup.Num();
}

int32 UDDModel::GetClassObjectGroupNum(FName ObjectClassName) const
{
	if (ObjectClassGroup.Contains(ObjectClassName)) 
		return (*ObjectClassGroup.Find(ObjectClassName)).Num();
	return 0;
}

void UDDModel::RemoveObject(FName ObjectName)
{
	//如果没有这个对象,直接返回
	if (!ObjectGroup.Contains(ObjectName)) return;
	//先获取对象指针
	IDDOO* TargetObject = *ObjectGroup.Find(ObjectName);
	//从生命组移除对象
	ObjectLifeGroup.Remove(TargetObject);
	//从帧组移除对象
	ObjectTickGroup.Remove(TargetObject);
	//从类组移除对象
	ObjectClassGroup.Find(TargetObject->GetClassName())->Remove(TargetObject);
	//从对象组移除对象
	ObjectGroup.Remove(ObjectName);
}

void UDDModel::RemoveObjectClass(FName ObjectClassName)
{
	//如果没有这个对象类,直接返回
	if (!ObjectClassGroup.Contains(ObjectClassName)) return;
	//获取类对象数组
	TArray<IDDOO*>* TargetClassGroup = ObjectClassGroup.Find(ObjectClassName);
	//迭代移除对象
	for (int i = 0; i < (*TargetClassGroup).Num(); ++i) {
		//从生命组与帧组移除对象
		ObjectLifeGroup.Remove((*TargetClassGroup)[i]);
		ObjectTickGroup.Remove((*TargetClassGroup)[i]);
		//从对象组移除对象
		ObjectGroup.Remove((*TargetClassGroup)[i]->GetObjectName());
	}
	//最后从类组移除
	ObjectClassGroup.Remove(ObjectClassName);
}
