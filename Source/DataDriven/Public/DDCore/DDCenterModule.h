// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDCore/DDModule.h"
#include "DDCenterModule.generated.h"


/**
 *
 */
UCLASS()
class DATADRIVEN_API UDDCenterModule : public UDDModule
{
	GENERATED_BODY()

public:

	UDDCenterModule();

	//virtual void InitializeComponent() override;

	//迭代调用本模组以及子模组的EditChangeModuleType方法
	virtual void IterChangeModuleType(UDDModule* Module, FName ModType);

	//提取所有模组到模组数组, 传入枚举名字
	void TotalGatherModule(FName ModType);

	//迭代实例化管理器组件
	virtual void IterCreateManager(UDDModule* Module);

	//迭代调用本模组以及子模组的BeginPlay方法,不用反射
	virtual void IterModuleInit(UDDModule* Module);

	//迭代调用本模组以及子模组的BeginPlay方法,不用反射
	virtual void IterModuleBeginPlay(UDDModule* Module);

	//迭代调用本模组以及子模组的Tick方法,不用反射
	virtual void IterModuleTick(UDDModule* Module, float DeltaSeconds);

	//迭代注册物品到模组
	virtual bool RegisterToModule(IDDOO* Object);

	//迭代执行模组方法
	virtual void AllotExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);

	//迭代执行对象方法
	virtual void AllotExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

	//注册调用方法
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun);


protected:

	//保存模组的数组, 顺序与枚举相同
	TArray<UDDModule*> ModuleGroup;

};

template<typename RetType, typename... VarTypes>
DDFunHandle UDDCenterModule::RegisterFunPort(int32 ModuleID, FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	if (ModuleGroup[ModuleID])
		return ModuleGroup[ModuleID]->RegisterFunPort<RetType, VarTypes...>(CallName, InsFun);
	return DDFunHandle();
}
