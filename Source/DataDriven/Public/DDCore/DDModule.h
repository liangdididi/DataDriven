// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DDCommon/DDCommon.h"

#include "DDMessage.h"

#include "DDModule.generated.h"

class UDDModel;
class UDDWealth;
class IDDOO;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DATADRIVEN_API UDDModule : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDDModule();

	//Module的Init函数
	virtual void ModuleInit();

	//Module的BeginPlay函数
	virtual void ModuleBeginPlay();

	//Module的Tick函数
	virtual void ModuleTick(float DeltaSeconds);

	void RegisterObject(IDDOO* Object);

	void CreateManager();

	void ChangeModuleType(FName ModuleType);

	//调动模组方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);
	//调用对象方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

	//注册调用接口
	template<typename RetType, typename... VarTypes>
	DDCallHandle<RetType, VarTypes...> RegisterCallPort(FName CallName);

	//注册调用方法
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun);

	//开始一个协程, 返回true说明开启协程成功, 返回false说明已经有相同名字的协程在运行
	bool StartCoroutine(FName ObjectName, FName CoroName, DDCoroTask* CoroTask);

	//停止一个协程, 放回true说明停止协程成功, 返回false说明协程早已经停止
	bool StopCoroutine(FName ObjectName, FName CoroName);

	//停止该对象的所有协程
	void StopAllCoroutine(FName ObjectName);

	//开始一个延时方法, 返回true说明开启成功, 返回false说明已经有相同名字的方法在运行
	bool StartInvoke(FName ObjectName, FName InvokeName, DDInvokeTask* InvokeTask);

	//停止一个延时方法, 返回true说明停止延时成功, 返回false说明延时早已经停止
	bool StopInvoke(FName ObjectName, FName InvokeName);

	//停止该对象的所有延时方法
	void StopAllInvoke(FName ObjectName);

	//获取所有模块到数组
	void IterGatherModule(UDDModule* Module, TArray<UDDModule*>& GatherGroup);

	//注册Axis按键事件
	template<class UserClass>
	FInputAxisBinding& BindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const FName AxisName);

	//注册触摸事件
	template<class UserClass>
	FInputTouchBinding& BindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const EInputEvent KeyEvent);

	//注册单个按键事件
	template<class UserClass>
	FInputKeyBinding& BindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const FKey Key, const EInputEvent KeyEvent);

	//注册多个按键事件
	template<class UserClass>
	UDDInputBinder& BindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, TArray<FKey>& KeyGroup, FName ObjectName);

	//按键事件注销
	void UnBindInput(FName ObjectName);

	//子集对象销毁自身函数
	void ChildDestroy(FName ObjectName);

	//外部方法, 创建新的Class资源
	void BulidSingleClassWealth(EWealthType WealthType, FName WealthName, FName ObjectName, FName FunName, FTransform SpawnTransform);
	void BulidMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms);
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms);

	//外部方法获取UE资源
	void LoadClassWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadClassWealthKind(FName WealthKind, FName ObjectName, FName FunName);

	//外部方法获取UE资源
	void LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName);

	//外部方法单纯的获取资源链接
	FWealthURL* GainWealthURL(FName WealthName);
	void GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL);

public:

	UPROPERTY(EditAnywhere, Category = "DataDriven")
		TArray<UWealthData*> WealthData;

	UPROPERTY(VisibleAnywhere, Category = "DataDriven")
		int32 ModuleIndex;

	TArray<UDDModule*> ChildrenModule;

protected:

	//执行单个对象方法
	void ExecuteSelfObject(DDObjectAgreement Agreement, DDParam* Param);
	//执行其余对象方法
	void ExecuteOtherObject(DDObjectAgreement Agreement, DDParam* Param);
	//支持相同类的区域对象方法
	void ExecuteClassOtherObject(DDObjectAgreement Agreement, DDParam* Param);
	//执行类对象的方法
	void ExecuteSelfClass(DDObjectAgreement Agreement, DDParam* Param);
	//执行其他类对象的方法
	void ExecuteOtherClass(DDObjectAgreement Agreement, DDParam* Param);
	//执行所有对象的方法
	void ExecuteAll(DDObjectAgreement Agreement, DDParam* Param);

protected:

	//保存数据指针
	UDDModel * Model;
	//保存消息指针
	UDDMessage* Message;
	//保存资源指针
	UDDWealth* Wealth;


};

template<typename RetType, typename... VarTypes>
DDCallHandle<RetType, VarTypes...>
UDDModule::RegisterCallPort(FName CallName)
{
	return Message->RegisterCallPort<RetType, VarTypes...>(CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle UDDModule::RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	return Message->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
}

template<class UserClass>
FInputAxisBinding& UDDModule::BindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const FName AxisName)
{
	return Message->BindAxis(UserObj, InMethod, AxisName);
}

template<class UserClass>
FInputTouchBinding& UDDModule::BindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const EInputEvent KeyEvent)
{
	return Message->BindTouch(UserObj, InMethod, KeyEvent);
}

template<class UserClass>
FInputKeyBinding& UDDModule::BindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const FKey Key, const EInputEvent KeyEvent)
{
	return Message->BindInput(UserObj, InMethod, Key, KeyEvent);
}

template<class UserClass>
UDDInputBinder& UDDModule::BindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, TArray<FKey>& KeyGroup, FName ObjectName)
{
	return Message->BindInput(UserObj, InMethod, KeyGroup, ObjectName);
}

