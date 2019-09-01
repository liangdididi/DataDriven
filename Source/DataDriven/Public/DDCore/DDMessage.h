// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDMM.h"
#include "GameFramework/PlayerController.h"
#include "DDMessage.generated.h"


#pragma region InputBinder

DECLARE_DELEGATE(FDDInputEvent)

UCLASS()
class DATADRIVEN_API UDDInputBinder : public UObject
{
	GENERATED_BODY()

public:

	UDDInputBinder();

	void PressEvent();

	void ReleaseEvent();

public:

	uint8 InputCount;

	uint8 TotalCount;

	//游戏暂停时是否执行按键事件
	uint8 bExecuteWhenPause;

	FDDInputEvent InputDele;

public:

	template<class UserClass>
	void InitBinder(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, uint8 InCount)
	{
		TotalCount = InCount;
		InputDele.BindUObject(UserObj, InMethod);
	}

};



#pragma endregion


/**
 *
 */
UCLASS()
class DATADRIVEN_API UDDMessage : public UObject, public IDDMM
{
	GENERATED_BODY()

public:

	UDDMessage();

	virtual void MessageInit();

	virtual void MessageBeginPlay();

	virtual void MessageTick(float DeltaSeconds);

	//注册调用接口
	template<typename RetType, typename... VarTypes>
	DDCallHandle<RetType, VarTypes...> RegisterCallPort(FName CallName);

	//注册方法接口
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun);

	//开启一个协程, 返回true说明开启成功, 返回false说明已经有同对象名同协程任务名的协程存在
	bool StartCoroutine(FName ObjectName, FName CoroName, DDCoroTask* CoroTask);

	//停止一个协程, 返回true说明停止协程成功, 返回false说明协程早已经停止
	bool StopCoroutine(FName ObjectName, FName CoroName);

	//停止该对象的所有协程
	void StopAllCorotine(FName ObjectName);

	//开始一个延时方法, 返回true说明成功, 返回false说明已经存在同对象名同任务名的任务
	bool StartInvoke(FName ObjectName, FName InvokeName, DDInvokeTask* InvokeTask);

	//停止一个延时
	bool StopInvoke(FName ObjectName, FName InvokeName);

	//停止某对象下的所有延时方法
	void StopAllInvoke(FName ObjectName);

	//绑定Axis按键事件
	template<class UserClass>
	FInputAxisBinding& BindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName AxisName);

	//绑定触摸事件
	template<class UserClass>
	FInputTouchBinding& BindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const EInputEvent KeyEvent);

	//绑定Antion按键事件
	template<class UserClass>
	FInputActionBinding& BindAction(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName ActionName, const EInputEvent KeyEvent);

	//绑定单个按键事件
	template<class UserClass>
	FInputKeyBinding& BindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FKey Key, const EInputEvent KeyEvent);

	//绑定多个按键
	template<class UserClass>
	UDDInputBinder& BindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, TArray<FKey>& KeyGroup, FName ObjectName);

	//解绑对象的所有按键事件
	void UnBindInput(FName ObjectName);


protected:

	//事件队列
	DDMsgQuene* MsgQuene;

	//协程序列, 键1保存对象名, 值的键FName对应的是协程任务的名字
	TMap<FName, TMap<FName, DDCoroTask*>> CoroStack;

	//延时序列
	TMap<FName, TMap<FName, DDInvokeTask*>> InvokeStack;

	//绑定按键事件序列
	TMap<FName, TArray<UDDInputBinder*>> BinderGroup;

	//PlayerController指针
	APlayerController* PlayerController;

};

template<typename RetType, typename... VarTypes>
DDCallHandle<RetType, VarTypes...>
UDDMessage::RegisterCallPort(FName CallName)
{
	return MsgQuene->RegisterCallPort<RetType, VarTypes...>(CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle UDDMessage::RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	return MsgQuene->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
}

template<class UserClass>
FInputAxisBinding& UDDMessage::BindAxis(UserClass* UserObj, typename FInputAxisHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName AxisName)
{
	return PlayerController->InputComponent->BindAxis(AxisName, UserObj, InMethod);
}

template<class UserClass>
FInputTouchBinding& UDDMessage::BindTouch(UserClass* UserObj, typename FInputTouchHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr InMethod, const EInputEvent KeyEvent)
{
	return PlayerController->InputComponent->BindTouch(KeyEvent, UserObj, InMethod);
}

template<class UserClass>
FInputActionBinding& UDDMessage::BindAction(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FName ActionName, const EInputEvent KeyEvent)
{
	return PlayerController->InputComponent->BindAction(ActionName ,KeyEvent, UserObj, InMethod);
}

template<class UserClass>
FInputKeyBinding& UDDMessage::BindInput(UserClass* UserObj, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, const FKey Key, const EInputEvent KeyEvent)
{
	return PlayerController->InputComponent->BindKey(Key, KeyEvent, UserObj, InMethod);
}

template<class UserClass>
UDDInputBinder& UDDMessage::BindInput(UserClass* UserObj, typename FDDInputEvent::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod, TArray<FKey>& KeyGroup, FName ObjectName)
{
	UDDInputBinder* InputBinder = NewObject<UDDInputBinder>();
	InputBinder->InitBinder(UserObj, InMethod, KeyGroup.Num());
	InputBinder->AddToRoot();
	for (int i = 0; i < KeyGroup.Num(); ++i)
	{
		PlayerController->InputComponent->BindKey(KeyGroup[i], IE_Pressed, InputBinder, &UDDInputBinder::PressEvent).bExecuteWhenPaused = true;
		PlayerController->InputComponent->BindKey(KeyGroup[i], IE_Released, InputBinder, &UDDInputBinder::ReleaseEvent).bExecuteWhenPaused = true;
	}
	if (!BinderGroup.Contains(ObjectName))
	{
		TArray<UDDInputBinder*> BinderList;
		BinderGroup.Add(ObjectName, BinderList);
	}
	BinderGroup.Find(ObjectName)->Push(InputBinder);
	return *InputBinder;
}
