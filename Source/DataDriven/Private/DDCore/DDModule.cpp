// Fill out your copyright notice in the Description page of Project Settings.


#include "DDModule.h"
#include "DDModel.h"
#include "DDWealth.h"
#include "DDOO.h"

// Sets default values for this component's properties
UDDModule::UDDModule()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


void UDDModule::CreateManager()
{
	//实例化组件, 这里用NewObject实例化组件时不能在括号内填this, 否则编辑器运行游戏退出时会奔溃
	Model = NewObject<UDDModel>();
	Message = NewObject<UDDMessage>();
	Wealth = NewObject<UDDWealth>();
	//避免销毁
	Model->AddToRoot();
	Message->AddToRoot();
	Wealth->AddToRoot();
	//指定模组
	Model->AssignModule(this);
	Message->AssignModule(this);
	Wealth->AssignModule(this);
}

void UDDModule::ModuleInit()
{
	//调用Init函数
	Model->ModelInit();
	Message->MessageInit();
	Wealth->WealthInit();

}

void UDDModule::ModuleBeginPlay()
{
	//给Wealth指定资源
	Wealth->AssignData(WealthData);
	//调用BeginPlay
	Model->ModelBeginPlay();
	Message->MessageBeginPlay();
	Wealth->WealthBeginPlay();

}

void UDDModule::ModuleTick(float DeltaSeconds)
{
	//调用Tick
	Model->ModelTick(DeltaSeconds);
	Wealth->WealthTick(DeltaSeconds);
	Message->MessageTick(DeltaSeconds);

}

void UDDModule::ChangeModuleType(FName ModuleType)
{
	ModuleIndex = DDH::GetEnumIndexFromName(ModuleType.ToString(), GetFName());

	if (ModuleIndex < 0)
		DDH::Debug() << "Generate Module Index Error --> " << GetName() << DDH::Endl();
}

void UDDModule::RegisterObject(IDDOO* ObjectInst)
{
	//注册对象到数据组件
	Model->RegisterObject(ObjectInst);
	//把自己注册到对象的模组
	ObjectInst->AssignModule(this);
}

void UDDModule::ChildDestroy(FName ObjectName)
{
	Model->DestroyObject(ObjectName);
}

void UDDModule::DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->DestroyObject(Agreement, TargetNameGroup);
}

void UDDModule::EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->EnableObject(Agreement, TargetNameGroup);
}

void UDDModule::DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup)
{
	Model->DisableObject(Agreement, TargetNameGroup);
}

void UDDModule::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	//调用Module的UFuntion
	UFunction* ExeFunc = FindFunction(Agreement.FunctionName);
	//如果方法存在
	if (ExeFunc)
	{
		//设置调用成功
		Param->CallResult = ECallResult::Succeed;
		//调用方法
		ProcessEvent(ExeFunc, Param->ParamPtr);
	}
	else
	{
		//设置方法不存在
		Param->CallResult = ECallResult::NoFunction;
	}
}

void UDDModule::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	//区分类型执行放射方法
	switch (Agreement.AgreementType)
	{
	case EAgreementType::SelfObject:
		ExecuteSelfObject(Agreement, Param);
		break;
	case EAgreementType::OtherObject:
		ExecuteOtherObject(Agreement, Param);
		break;
	case EAgreementType::ClassOtherObject:
		ExecuteClassOtherObject(Agreement, Param);
		break;
	case EAgreementType::SelfClass:
		ExecuteSelfClass(Agreement, Param);
		break;
	case EAgreementType::OtherClass:
		ExecuteOtherClass(Agreement, Param);
		break;
	case EAgreementType::All:
		ExecuteAll(Agreement, Param);
		break;
	}
}

bool UDDModule::StartCoroutine(FName ObjectName, FName CoroName, DDCoroTask* CoroTask)
{
	return Message->StartCoroutine(ObjectName, CoroName, CoroTask);
}

bool UDDModule::StopCoroutine(FName ObjectName, FName CoroName)
{
	return Message->StopCoroutine(ObjectName, CoroName);
}

void UDDModule::StopAllCorotine(FName ObjectName)
{
	return Message->StopAllCorotine(ObjectName);
}

bool UDDModule::StartInvoke(FName ObjectName, FName InvokeName, DDInvokeTask* InvokeTask)
{
	return Message->StartInvoke(ObjectName, InvokeName, InvokeTask);
}

bool UDDModule::StopInvoke(FName ObjectName, FName InvokeName)
{
	return Message->StopInvoke(ObjectName, InvokeName);
}

void UDDModule::StopAllInvoke(FName ObjectName)
{
	Message->StopAllInvoke(ObjectName);
}

void UDDModule::UnBindInput(FName ObjectName)
{
	Message->UnBindInput(ObjectName);
}

FWealthURL* UDDModule::GainWealthURL(FName WealthName)
{
	return Wealth->GainWealthURL(WealthName);
}

void UDDModule::GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL)
{
	Wealth->GainWealthURL(WealthKind, OutURL);
}

void UDDModule::LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName)
{
	Wealth->LoadObjectWealth(WealthName, ObjectName, FunName);
}

void UDDModule::LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName)
{
	Wealth->LoadObjectWealthKind(WealthKind, ObjectName, FunName);
}

void UDDModule::LoadClassWealth(FName WealthName, FName ObjectName, FName FunName)
{
	Wealth->LoadClassWealth(WealthName, ObjectName, FunName);
}

void UDDModule::LoadClassWealthKind(FName WealthKind, FName ObjectName, FName FunName)
{
	Wealth->LoadClassWealthKind(WealthKind, ObjectName, FunName);
}

void UDDModule::BuildSingleClassWealth(EWealthType WealthType, FName WealthName, FName ObjectName, FName FunName, FTransform SpawnTransform)
{
	Wealth->BuildSingleClassWealth(WealthType, WealthName, ObjectName, FunName, SpawnTransform);
}

void UDDModule::BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms)
{
	Wealth->BuildKindClassWealth(WealthType, WealthKind, ObjectName, FunName, SpawnTransforms);
}

void UDDModule::BuildMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms)
{
	Wealth->BuildMultiClassWealth(WealthType, WealthName, Amount, ObjectName, FunName, SpawnTransforms);
}

void UDDModule::ExecuteSelfObject(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据模组获取对象组
	Model->GetSelfObject(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//如果获取的对象有缺失, 设置结果为对象缺失, 这个结果的优先级最高
	if (TargetObjectGroup.Num() != Agreement.ObjectGroup.Num())
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteOtherObject(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据组件获取对象组
	int32 TotalObjectNum = Model->GetOtherObject(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//判断对象有没有缺失
	if (Agreement.ObjectGroup.Num() + TargetObjectGroup.Num() != TotalObjectNum)
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteClassOtherObject(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据组件获取对象组
	int32 TotalClassNum = Model->GetClassOtherObject(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//判断对象缺失
	if (Agreement.ObjectGroup.Num() + TargetObjectGroup.Num() != TotalClassNum)
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteSelfClass(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据组件获取对象组
	Model->GetSelfClass(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//判断对象缺失
	if(TargetObjectGroup.Num() == 0)
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteOtherClass(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据组件获取对象组
	Model->GetOtherClass(Agreement.ObjectGroup, TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
	//判断对象缺失
	if (TargetObjectGroup.Num() == 0)
		Param->CallResult = ECallResult::LackObject;
}

void UDDModule::ExecuteAll(DDObjectAgreement Agreement, DDParam* Param)
{
	//定义存储目标对象的组
	TArray<IDDOO*> TargetObjectGroup;
	//从数据组件获取对象组
	Model->GetAll(TargetObjectGroup);
	//循环调用反射事件
	for (int i = 0; i < TargetObjectGroup.Num(); ++i)
	{
		//获取反射方法
		UFunction* ExeFunc = TargetObjectGroup[i]->GetObjectBody()->FindFunction(Agreement.FunctionName);
		if (ExeFunc)
		{
			//设置调用成功
			Param->CallResult = ECallResult::Succeed;
			//执行方法
			TargetObjectGroup[i]->GetObjectBody()->ProcessEvent(ExeFunc, Param->ParamPtr);
		}
		else
		{
			//设置找不到方法
			Param->CallResult = ECallResult::NoFunction;
		}
	}
}

