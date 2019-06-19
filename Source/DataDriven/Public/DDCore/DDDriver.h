// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDCommon/DDCommon.h"
#include "DDCenterModule.h"
#include "DDDriver.generated.h"

class USceneComponent;
class IDDOO;

UCLASS()
class DATADRIVEN_API ADDDriver : public AActor
{
	GENERATED_BODY()
	
public:

	ADDDriver();

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	//调动模组方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);
	//调用对象方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

#if WITH_EDITOR
	//属性修改方法
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	//提供给资源们进行注册
	bool RegisterToModule(IDDOO* Object);


	//注册调用方法
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun);

public:

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "DataDriven")
		USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "DataDriven")
		UDDCenterModule* Center;

	//模组对应的枚举名字
	UPROPERTY(EditDefaultsOnly, Category = "DataDriven")
		FName ModuleType;

protected:

	virtual void BeginPlay() override;

	//注册GamePlay组件到框架
	void RegisterGamePlay();


protected:

	//是否已经运行BeginPlay函数
	bool IsBeginPlay;

	
	
};

template<typename RetType, typename... VarTypes>
DDFunHandle ADDDriver::RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	return Center->RegisterFunPort<RetType, VarTypes...>(ModuleID, CallName, InsFun);
}
