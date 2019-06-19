// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "DDDriver.h"
#include "DDModule.h"

#include "DDOO.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDOO : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class DATADRIVEN_API IDDOO
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//通过传入的ModName是否为空来判定是否自动注册
	void RegisterToModule(FName ModName, FName ObjectName = FName(), FName ClassName = FName());

	//同上, 只转入模组Index
	void RegisterToModule(int32 ModIndex, FName ObjectName = FName(), FName ClassName = FName());

	//从外部指定模组
	void AssignModule(UDDModule* Mod);

	//获取世界
	UWorld* GetDDWorld() const;

	//获取Obejct主体
	UObject* GetObjectBody() const;

	//获取模组Index
	int32 GetModuleIndex() const;

	//获取对象名字,允许重写,自己设定格式
	virtual FName GetObjectName() const;

	//获取类的名字
	virtual FName GetClassName();

	//DDOO的Tick函数
	virtual void DDTick(float DeltaSeconds);

	//激活生命周期,激活成功的时候返回true,停止调用
	bool ActiveLife();

	//销毁生命周期,销毁成功后返回true,并且从模组注销
	bool DestroyLife();

	//运行释放函数
	virtual void OnRealse();
	//激活对象
	virtual void OnEnable();
	//失活对象
	virtual void OnDisable();

	//生命周期,由模组管理器调用
	virtual void DDInit();//初始化
	virtual void DDLoading();//加载绑定的资源
	virtual void DDRegister();//注册数据或者事件
	virtual void DDEnable();//激活对象
	virtual void DDDisable();//失活对象
	virtual void DDUnRegister();//注销数据或者事件
	virtual void DDUnLoading();//销毁绑定资源
	virtual void DDRelease();//释放自己

	//销毁自己
	void DDDestroy();


public:

	//是否允许帧运行,如果要允许帧运行需要在构造函数或者BeginPlay设置,在UE4里默认为false
	bool IsAllowTickEvent;

	//生命状态
	EBaseObjectLife LifeState;

	//运行状态
	EBaseObjectState RunState;

protected:

	//发布方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);
	//发布方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

	bool StartCoroutine(FName CoroName, DDCoroTask* CoroTask);

	bool StopCoroutine(FName CoroName);

	void StopAllCoroutine();

	template<class UserClass>
	bool InvokeDelay(FName InvokeName, float DelayTime, UserClass* UserObj, typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod);

	template<class UserClass>
	bool InvokeRepeat(FName InvokeName, float DelayTime, float RepeatTime, UserClass* UserObj, typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod);

	bool StopInvoke(FName InvokeName);

	void StopAllInvoke();

	//创建新的Class资源
	void BulidSingleClassWealth(EWealthType WealthType, FName WealthName, FName FunName);
	void BulidSingleClassWealth(EWealthType WealthType, FName WealthName, FName FunName, FTransform SpawnTransform);
	void BulidMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName);
	void BulidMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName, FTransform SpawnTransform);
	void BulidMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName, TArray<FTransform> SpawnTransforms);
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName);
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName, FTransform SpawnTransform);
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName, TArray<FTransform> SpawnTransforms);

	//获取UE资源
	void LoadClassWealth(FName WealthName, FName FunName);
	void LoadClassWealthKind(FName WealthKind, FName FunName);

	//获取UE资源
	void LoadObjectWealth(FName WealthName, FName FunName);
	void LoadObjectWealthKind(FName WealthKind, FName FunName);

	//单纯获取资源链接
	FWealthURL* GainWealthURL(FName WealthName);
	void GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL);

	//注册调用接口
	template<typename RetType, typename... VarTypes>
	DDCallHandle<RetType, VarTypes...> RegisterCallPort(FName CallName);
	//注册调用方法
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun);

	//注册Axis按键事件
	template<class UserClass>
	FInputAxisBinding& DDBindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const FName AxisName);

	//注册触摸事件
	template<class UserClass>
	FInputTouchBinding& DDBindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const EInputEvent KeyEvent);

	//注册单个按键事件
	template<class UserClass>
	FInputKeyBinding& DDBindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const FKey Key, const EInputEvent KeyEvent);

	//注册多个按键
	template<class UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II);
	template<class UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III);
	template<class UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III, FKey Key_IV);
	template<class UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III, FKey Key_IV, FKey Key_V);

protected:

	//保存自身UObject
	UObject * IBody;

	//保存对应的模组
	UDDModule* IModule;

	//保存驱动器Driver
	ADDDriver* IDriver;

	//保存对应模组的Index
	int32 ModuleIndex;

	//物品的名字,如果不为空,获取物品名就返回这个,并且此物品在对应的Module唯一
	FName IObjectName;

	//类名,这个在初始化时一定要指定
	FName IClassName;


};

template<class UserClass>
bool IDDOO::InvokeDelay(FName InvokeName, float DelayTime, UserClass* UserObj, typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
{
	DDInvokeTask* InvokeTask = new DDInvokeTask(DelayTime, false, 0.f);
	InvokeTask->InvokeEvent.BindUObject(UserObj, InMethod);
	return IModule->StartInvoke(GetObjectName(), InvokeName, InvokeTask);
}

template<class UserClass>
bool IDDOO::InvokeRepeat(FName InvokeName, float DelayTime, float RepeatTime, UserClass* UserObj, typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
{
	DDInvokeTask* InvokeTask = new DDInvokeTask(DelayTime, true, RepeatTime);
	InvokeTask->InvokeEvent.BindUObject(UserObj, InMethod);
	return IModule->StartInvoke(GetObjectName(), InvokeName, InvokeTask);
}

template<typename RetType, typename... VarTypes>
DDCallHandle<RetType, VarTypes...>
IDDOO::RegisterCallPort(FName CallName)
{
	return IModule->RegisterCallPort<RetType, VarTypes...>(CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle IDDOO::RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	if (ModuleIndex == ModuleID)
		return IModule->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
	else
		return IDriver->RegisterFunPort<RetType, VarTypes...>(ModuleID, CallName, InsFun);
}

template<class UserClass>
FInputAxisBinding& IDDOO::DDBindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const FName AxisName)
{
	return IModule->BindAxis(UserObj, InMethod, AxisName);
}

template<class UserClass>
FInputTouchBinding& IDDOO::DDBindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const EInputEvent KeyEvent)
{
	return IModule->BindTouch(UserObj, InMethod, KeyEvent);
}

template<class UserClass>
FInputKeyBinding& IDDOO::DDBindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const FKey Key, const EInputEvent KeyEvent)
{
	return IModule->BindInput(UserObj, InMethod, Key, KeyEvent);
}

template<class UserClass>
UDDInputBinder& IDDOO::DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II)
{
	TArray<FKey> KeyGroup;
	KeyGroup.Add(Key_I);
	KeyGroup.Add(Key_II);
	return IModule->BindInput(UserObj, InMethod, KeyGroup, GetObjectName());
}

template<class UserClass>
UDDInputBinder& IDDOO::DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III)
{
	TArray<FKey> KeyGroup;
	KeyGroup.Add(Key_I);
	KeyGroup.Add(Key_II);
	KeyGroup.Add(Key_III);
	return IModule->BindInput(UserObj, InMethod, KeyGroup, GetObjectName());
}

template<class UserClass>
UDDInputBinder& IDDOO::DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III, FKey Key_IV)
{
	TArray<FKey> KeyGroup;
	KeyGroup.Add(Key_I);
	KeyGroup.Add(Key_II);
	KeyGroup.Add(Key_III);
	KeyGroup.Add(Key_IV);
	return IModule->BindInput(UserObj, InMethod, KeyGroup, GetObjectName());
}

template<class UserClass>
UDDInputBinder& IDDOO::DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III, FKey Key_IV, FKey Key_V)
{
	TArray<FKey> KeyGroup;
	KeyGroup.Add(Key_I);
	KeyGroup.Add(Key_II);
	KeyGroup.Add(Key_III);
	KeyGroup.Add(Key_IV);
	KeyGroup.Add(Key_V);
	return IModule->BindInput(UserObj, InMethod, KeyGroup, GetObjectName());
}
