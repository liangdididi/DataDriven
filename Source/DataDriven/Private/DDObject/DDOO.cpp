// Fill out your copyright notice in the Description page of Project Settings.

#include "DDOO.h"
#include "UObject/NoExportTypes.h"



void IDDOO::RegisterToModule(FName ModName, FName ObjectName, FName ClassName)
{
	//如果IDriver已经存在,说明已经注册过了,直接返回,避免重复存贮
	if (IDriver) return;
	//如果模组名为空,直接返回
	if (ModName.IsNone()) return;
	//注册类名和对象名
	if (!ObjectName.IsNone()) IObjectName = ObjectName;
	if (!ClassName.IsNone()) IClassName = ClassName;
	//获取UObject主体
	IBody = Cast<UObject>(this);
	//获取Driver
	IDriver = UDDCommon::Get()->GetDriver();
	//注册到模组
	if (IDriver)
	{
		//如果获得的ID为负直接返回
		ModuleIndex = DDH::GetEnumIndexFromName(IDriver->ModuleType.ToString(), ModName);
		if (ModuleIndex < 0) {
			DDH::Debug() << GetObjectName() << " Get " << ModName << " ModuleID Failed !" << DDH::Endl();
			return;
		}
		//如果注册不成功说明还没有实例化对应的Module
		if (!IDriver->RegisterToModule(this))
			DDH::Debug() << GetObjectName() << " Register To " << ModName << " Failed !" << DDH::Endl();
	}
	else {
		//DDriver不存在
		DDH::Debug() << GetObjectName() << " Get DDDriver Failed !" << DDH::Endl();
	}
}


void IDDOO::RegisterToModule(int32 ModIndex, FName ObjectName, FName ClassName)
{
	//如果IDriver已经存在,说明已经注册过了,直接返回,避免重复存贮
	if (IDriver) return;
	//注册类名和对象名
	if (!ObjectName.IsNone()) IObjectName = ObjectName;
	if (!ClassName.IsNone()) IClassName = ClassName;
	//获取UObject主体
	IBody = Cast<UObject>(this);
	//获取Driver
	IDriver = UDDCommon::Get()->GetDriver();
	//注册到模组
	if (IDriver)
	{
		//赋予模组ID
		ModuleIndex = ModIndex;
		//如果注册不成功说明还没有实例化对应的Module
		if (!IDriver->RegisterToModule(this))
			DDH::Debug() << GetObjectName() << " Register To ModuleIndex " << ModuleIndex << " Failed !" << DDH::Endl();
	}
	else {
		//DDriver不存在
		DDH::Debug() << GetObjectName() << " Get DDDriver Failed !" << DDH::Endl();
	}
}

void IDDOO::AssignModule(UDDModule* Mod)
{
	IModule = Mod;
}

UWorld* IDDOO::GetDDWorld() const
{
	if (IDriver) return IDriver->GetWorld();
	return NULL;
}

UObject* IDDOO::GetObjectBody() const
{
	return IBody;
}

int32 IDDOO::GetModuleIndex() const
{
	return ModuleIndex;
}

FName IDDOO::GetObjectName() const
{
	return IObjectName.IsNone() ? IBody->GetFName() : IObjectName;
}

FName IDDOO::GetClassName()
{
	if (!IClassName.IsNone()) return IClassName;
	IClassName = IBody->StaticClass()->GetFName();
	return IClassName;
}


bool IDDOO::ActiveLife()
{
	switch (LifeState)
	{
	case EBaseObjectLife::None:
		DDInit();
		LifeState = EBaseObjectLife::Init;
		break;
	case EBaseObjectLife::Init:
		DDLoading();
		LifeState = EBaseObjectLife::Loading;
		break;
	case EBaseObjectLife::Loading:
		DDRegister();
		LifeState = EBaseObjectLife::Register;
		break;
	case EBaseObjectLife::Register:
		DDEnable();
		LifeState = EBaseObjectLife::Enable;
		//设置运行状态为稳定
		RunState = EBaseObjectState::Stable;
		return true;
	}
	return false;
}

bool IDDOO::DestroyLife()
{
	switch (LifeState)
	{
	case EBaseObjectLife::Enable:
		DDDisable();
		LifeState = EBaseObjectLife::Disable;
		//设置运行状态为销毁
		RunState = EBaseObjectState::Destroy;
		break;
	case EBaseObjectLife::Disable:
		DDUnRegister();
		LifeState = EBaseObjectLife::UnRegister;
		//设置运行状态为销毁,避免从Disable状态下运行的对象没有修改RunState为销毁
		RunState = EBaseObjectState::Destroy;
		break;
	case EBaseObjectLife::UnRegister:
		DDUnLoading();
		LifeState = EBaseObjectLife::UnLoading;
		return true;
	}
	return false;
}

void IDDOO::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex)
		IModule->ExecuteFunction(Agreement, Param);
	else
		IDriver->ExecuteFunction(Agreement, Param);
}

void IDDOO::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex)
		IModule->ExecuteFunction(Agreement, Param);
	else
		IDriver->ExecuteFunction(Agreement, Param);
}

void IDDOO::OnRealse()
{
	//注销所有的协程, 延时和按键事件, 如果有的话
	StopAllCoroutine();
	StopAllInvoke();
	IModule->UnBindInput(GetObjectName());

	//运行DD框架的释放函数
	DDRelease();
}

void IDDOO::OnEnable()
{
	//一般运行一下DDEnable函数,可以重写
	DDEnable();
	//设置状态为激活状态
	LifeState = EBaseObjectLife::Enable;
}

void IDDOO::OnDisable()
{
	//一般运行一下DDDisable函数,可以重写
	DDDisable();
	//设置状态为失活状态
	LifeState = EBaseObjectLife::Disable;
}

void IDDOO::DDTick(float DeltaSeconds) {}

//生命周期系列函数
void IDDOO::DDInit() {}
void IDDOO::DDLoading() {}
void IDDOO::DDRegister() {}
void IDDOO::DDEnable() {}
void IDDOO::DDDisable() {}
void IDDOO::DDUnRegister() {}
void IDDOO::DDUnLoading() {}
void IDDOO::DDRelease() {}


void IDDOO::DDDestroy()
{
	IModule->ChildDestroy(GetObjectName());
}

bool IDDOO::StartCoroutine(FName CoroName, DDCoroTask* CoroTask)
{
	return IModule->StartCoroutine(GetObjectName(), CoroName, CoroTask);
}

bool IDDOO::StopCoroutine(FName CoroName)
{
	return IModule->StopCoroutine(GetObjectName(), CoroName);
}

void IDDOO::StopAllCoroutine()
{
	IModule->StopAllCoroutine(GetObjectName());
}

bool IDDOO::StopInvoke(FName InvokeName)
{
	return IModule->StopInvoke(GetObjectName(), InvokeName);
}

void IDDOO::StopAllInvoke()
{
	IModule->StopAllInvoke(GetObjectName());
}

void IDDOO::BulidSingleClassWealth(EWealthType WealthType, FName WealthName, FName FunName)
{
	IModule->BulidSingleClassWealth(WealthType, WealthName, GetObjectName(), FunName, FTransform::Identity);
}

void IDDOO::BulidSingleClassWealth(EWealthType WealthType, FName WealthName, FName FunName, FTransform SpawnTransform)
{
	IModule->BulidSingleClassWealth(WealthType, WealthName, GetObjectName(), FunName, SpawnTransform);
}

void IDDOO::BulidMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName)
{
	IModule->BulidMultiClassWealth(WealthType, WealthName, Amount, GetObjectName(), FunName, TArray<FTransform>{ FTransform::Identity });
}

void IDDOO::BulidMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName, FTransform SpawnTransform)
{
	IModule->BulidMultiClassWealth(WealthType, WealthName, Amount, GetObjectName(), FunName, TArray<FTransform>{ SpawnTransform });
}

void IDDOO::BulidMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName, TArray<FTransform> SpawnTransforms)
{
	IModule->BulidMultiClassWealth(WealthType, WealthName, Amount, GetObjectName(), FunName, SpawnTransforms);
}

void IDDOO::BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName)
{
	IModule->BuildKindClassWealth(WealthType, WealthKind, GetObjectName(), FunName, TArray<FTransform>{ FTransform::Identity });
}

void IDDOO::BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName, FTransform SpawnTransform)
{
	IModule->BuildKindClassWealth(WealthType, WealthKind, GetObjectName(), FunName, TArray<FTransform>{ SpawnTransform });
}

void IDDOO::BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName, TArray<FTransform> SpawnTransforms)
{
	IModule->BuildKindClassWealth(WealthType, WealthKind, GetObjectName(), FunName, SpawnTransforms);
}

void IDDOO::LoadClassWealth(FName WealthName, FName FunName)
{
	IModule->LoadClassWealth(WealthName, GetObjectName(), FunName);
}

void IDDOO::LoadClassWealthKind(FName WealthKind, FName FunName)
{
	IModule->LoadClassWealthKind(WealthKind, GetObjectName(), FunName);
}

void IDDOO::LoadObjectWealth(FName WealthName, FName FunName)
{
	IModule->LoadObjectWealth(WealthName, GetObjectName(), FunName);
}

void IDDOO::LoadObjectWealthKind(FName WealthKind, FName FunName)
{
	IModule->LoadObjectWealthKind(WealthKind, GetObjectName(), FunName);
}

FWealthURL* IDDOO::GainWealthURL(FName WealthName)
{
	return IModule->GainWealthURL(WealthName);
}

void IDDOO::GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL)
{
	IModule->GainWealthURL(WealthKind, OutURL);
}

