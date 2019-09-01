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

	//注册到框架
	void RegisterToModule(FName ModName, FName ObjName = FName(), FName ClsName = FName());

	//同上, 只转入模组Index
	void RegisterToModule(int32 ModIndex, FName ObjName = FName(), FName ClsName = FName());

	//获取对象名
	FName GetObjectName();

	//获取类名
	FName GetClassName();

	//获取模组Index
	int32 GetModuleIndex() const;

	//获取Obejct主体
	UObject* GetObjectBody() const;

	//框架获取世界函数
	UWorld* GetDDWorld() const;

	//从外部指定模组
	void AssignModule(UDDModule* Mod);

	//激活生命周期, 激活成功后返回true, 停止调用
	bool ActiveLife();

	//销毁生命周期函数, 销毁成功后返回true, 从数据模块注销
	bool DestroyLife();

	//生命周期,由模组管理器调用
	virtual void DDInit();//初始化
	virtual void DDLoading();//加载绑定的资源
	virtual void DDRegister();//注册数据或者事件
	virtual void DDEnable();//激活对象

	virtual void DDTick(float DeltaSeconds);//帧函数

	virtual void DDDisable();//失活对象
	virtual void DDUnRegister();//注销数据或者事件
	virtual void DDUnLoading();//销毁绑定资源
	virtual void DDRelease();//释放自己

	//激活对象
	virtual void OnEnable();

	//失活对象
	virtual void OnDisable();

	//销毁自己
	void DDDestroy();





public:

	//是否允许帧运行,如果要允许帧运行需要在构造函数或者BeginPlay设置,在UE4里默认为false
	bool IsAllowTickEvent;

	//生命周期状态
	EBaseObjectLife LifeState;

	//生命运行状态
	EBaseObjectState RunState;

protected:

	//执行反射方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	//执行反射方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

	//注册调用接口
	template<typename RetType, typename... VarTypes>
	DDCallHandle<RetType, VarTypes...> RegisterCallPort(FName CallName);

	//注册方法接口
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun);

	//开启一个协程, 返回true说明开启成功, 返回false说明已经有同对象名同协程任务名的协程存在
	bool StartCoroutine(FName CoroName, DDCoroTask* CoroTask);

	//停止一个协程, 返回true说明停止协程成功, 返回false说明协程早已经停止
	bool StopCoroutine(FName CoroName);

	//停止该对象的所有协程
	void StopAllCorotine();

	//延时运行
	template<class UserClass>
	bool InvokeDelay(FName InvokeName, float DelayTime, UserClass* UserObj, typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod);

	//延时循环运行
	template<class UserClass>
	bool InvokeRepeat(FName InvokeName, float DelayTime, float RepeatTime, UserClass* UserObj, typename FDDInvokeEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod);

	//关闭延时方法
	bool StopInvoke(FName InvokeName);

	//关闭对象下所有延时方法
	void StopAllInvoke();

	//绑定Axis按键事件
	template<class UserClass>
	FInputAxisBinding& DDBindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName AxisName);

	//绑定触摸事件
	template<class UserClass>
	FInputTouchBinding& DDBindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const EInputEvent KeyEvent);

	//绑定Antion按键事件
	template<class UserClass>
	FInputActionBinding& DDBindAction(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName ActionName, const EInputEvent KeyEvent);

	//绑定单个按键事件
	template<class UserClass>
	FInputKeyBinding& DDBindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FKey Key, const EInputEvent KeyEvent);

	//绑定多个按键
	template<class UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II);

	template<class UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III);

	template<class UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III, FKey Key_IV);

	template<class UserClass>
	UDDInputBinder& DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III, FKey Key_IV, FKey Key_V);

	//解绑该对象所有多个按键
	void UnBindInput();

	//外部方法单纯获取资源链接
	FWealthURL* GainWealthURL(FName WealthName);
	void GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL);

	//加载Object类型资源接口
	void LoadObjectWealth(FName WealthName, FName FunName);
	void LoadObjectWealthKind(FName WealthKind, FName FunName);

	//加载Class类型资源接口
	void LoadClassWealth(FName WealthName, FName FunName);
	void LoadClassWealthKind(FName WealthKind, FName FunName);

	//创建一个对象实例
	void BuildSingleClassWealth(EWealthType WealthType, FName WealthName, FName FunName);
	void BuildSingleClassWealth(EWealthType WealthType, FName WealthName, FName FunName, FTransform SpawnTransform);

	//创建同资源种类名的对象实例, 同种类名下的每个资源链接创建一个对象实例
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName);
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName, FTransform SpawnTransform);
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName FunName, TArray<FTransform> SpawnTransforms);

	//创建多个同资源名的对象实例
	void BuildMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName);
	void BuildMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName, FTransform SpawnTransform);
	void BuildMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName FunName, TArray<FTransform> SpawnTransforms);


protected:

	//保存自身的UObject
	UObject* IBody;

	//保存对应的模组
	UDDModule* IModule;

	//保存驱动器
	ADDDriver* IDriver;

	//对象名字
	FName IObjectName;

	//对象类名字
	FName IClassName;

	//对应模组的序号
	int32 ModuleIndex;


};

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


template<class UserClass>
FInputAxisBinding& IDDOO::DDBindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName AxisName)
{
	return IModule->BindAxis(UserObj, InMethod, AxisName);
}

template<class UserClass>
FInputTouchBinding& IDDOO::DDBindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const EInputEvent KeyEvent)
{
	return IModule->BindTouch(UserObj, InMethod, KeyEvent);
}

template<class UserClass>
FInputActionBinding& IDDOO::DDBindAction(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName ActionName, const EInputEvent KeyEvent)
{
	return IModule->BindAction(UserObj, InMethod, ActionName, KeyEvent);
}

template<class UserClass>
FInputKeyBinding& IDDOO::DDBindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FKey Key, const EInputEvent KeyEvent)
{
	return IModule->BindInput(UserObj, InMethod, Key, KeyEvent);
}

template<class UserClass>
UDDInputBinder& IDDOO::DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II)
{
	TArray<FKey> KeyGroup;
	KeyGroup.Push(Key_I);
	KeyGroup.Push(Key_II);
	return IModule->BindInput(UserObj, InMethod, KeyGroup, GetObjectName());
}

template<class UserClass>
UDDInputBinder& IDDOO::DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III)
{
	TArray<FKey> KeyGroup;
	KeyGroup.Push(Key_I);
	KeyGroup.Push(Key_II);
	KeyGroup.Push(Key_III);
	return IModule->BindInput(UserObj, InMethod, KeyGroup, GetObjectName());
}

template<class UserClass>
UDDInputBinder& IDDOO::DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III, FKey Key_IV)
{
	TArray<FKey> KeyGroup;
	KeyGroup.Push(Key_I);
	KeyGroup.Push(Key_II);
	KeyGroup.Push(Key_III);
	KeyGroup.Push(Key_IV);
	return IModule->BindInput(UserObj, InMethod, KeyGroup, GetObjectName());
}

template<class UserClass>
UDDInputBinder& IDDOO::DDBindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, FKey Key_I, FKey Key_II, FKey Key_III, FKey Key_IV, FKey Key_V)
{
	TArray<FKey> KeyGroup;
	KeyGroup.Push(Key_I);
	KeyGroup.Push(Key_II);
	KeyGroup.Push(Key_III);
	KeyGroup.Push(Key_IV);
	KeyGroup.Push(Key_V);
	return IModule->BindInput(UserObj, InMethod, KeyGroup, GetObjectName());
}
