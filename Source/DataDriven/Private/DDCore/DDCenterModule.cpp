// Fill out your copyright notice in the Description page of Project Settings.

#include "DDCenterModule.h"
#include "DDOO.h"





UDDCenterModule::UDDCenterModule() : Super()
{

}

void UDDCenterModule::IterCreateManager(UDDModule* Module)
{
	Module->CreateManager();
	for (int i = 0; i < Module->ChildrenModule.Num(); ++i)
		IterCreateManager(Module->ChildrenModule[i]);
}

void UDDCenterModule::IterModuleInit(UDDModule* Module)
{
	Module->ModuleInit();
	for (int i = 0; i < Module->ChildrenModule.Num(); ++i)
		IterModuleInit(Module->ChildrenModule[i]);
}

void UDDCenterModule::IterModuleBeginPlay(UDDModule* Module)
{
	Module->ModuleBeginPlay();
	for (int i = 0; i < Module->ChildrenModule.Num(); ++i)
		IterModuleBeginPlay(Module->ChildrenModule[i]);
}

void UDDCenterModule::IterModuleTick(UDDModule* Module, float DeltaSeconds)
{
	Module->ModuleTick(DeltaSeconds);
	for (int i = 0; i < Module->ChildrenModule.Num(); ++i)
		IterModuleTick(Module->ChildrenModule[i], DeltaSeconds);
}

bool UDDCenterModule::RegisterToModule(IDDOO* Object)
{
	//如果注册成功,直接返回true
	if (Object->GetModuleIndex() < ModuleGroup.Num() && ModuleGroup[Object->GetModuleIndex()])
	{
		ModuleGroup[Object->GetModuleIndex()]->RegisterObject(Object);
		return true;
	}
	return false;
}

void UDDCenterModule::IterChangeModuleType(UDDModule* Module, FName ModType)
{
	Module->ChangeModuleType(ModType);
	for (int i = 0; i < Module->GetAttachChildren().Num(); ++i)
	{
		UDDModule* ChildModule = Cast<UDDModule>(Module->GetAttachChildren()[i]);
		if (ChildModule)
		{
			//添加这个对象到这个Module的ChildModule
			Module->ChildrenModule.Add(ChildModule);
			IterChangeModuleType(ChildModule, ModType);
		}
	}
}

void UDDCenterModule::AllotExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	if (ModuleGroup[Agreement.ModuleIndex])
		ModuleGroup[Agreement.ModuleIndex]->ExecuteFunction(Agreement, Param);
}

void UDDCenterModule::AllotExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	if (ModuleGroup[Agreement.ModuleIndex])
		ModuleGroup[Agreement.ModuleIndex]->ExecuteFunction(Agreement, Param);
}

void UDDCenterModule::TotalGatherModule(FName ModType)
{
	if (ModType.IsNone()) return;
	//先获取所有的模组到GatherGroup
	TArray<UDDModule*> GatherGroup;
	IterGatherModule(this, GatherGroup);
	//获取枚举的元素数量
	int32 ModuleNum = FindObject<UEnum>((UObject*)ANY_PACKAGE, *(ModType.ToString()), true)->GetMaxEnumValue();
	//填充空对象到ModuleGroup
	for (int i = 0; i < ModuleNum; ++i)
		ModuleGroup.Add(NULL);
	//按模组ID填充模组到ModuleGroup
	for (int i = 0; i < GatherGroup.Num(); ++i)
		ModuleGroup[GatherGroup[i]->ModuleIndex] = GatherGroup[i];
}

