// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDMM.h"
#include "Engine/StreamableManager.h"
#include "DDWealth.generated.h"



//加载Object节点
struct ObjectSingleLoadNode;
struct ObjectKindLoadNode;

//加载单个Class
struct ClassSingleLoadNode;
struct ClassKindLoadNode;
struct ClassMultiLoadNode;

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDWealth : public UObject, public IDDMM
{
	GENERATED_BODY()

public:

	virtual void WealthInit() {}

	virtual void WealthBeginPlay();

	//资源的Tick函数
	virtual void WealthTick(float DeltaSeconds);

	//指定资源组
	void AssignData(TArray<UWealthData*>& InWealthData);

	//外部方法单纯获取资源链接
	FWealthURL* GainWealthURL(FName WealthName);
	void GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL);

	//加载Object类型资源接口
	void LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName);

	//加载Class类型资源接口
	void LoadClassWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadClassWealthKind(FName WealthKind, FName ObjectName, FName FunName);

	//创建一个对象实例
	void BuildSingleClassWealth(EWealthType WealthType, FName WealthName, FName ObjectName, FName FunName, FTransform SpawnTransform);

	//创建同资源种类名的对象实例, 同种类名下的每个资源链接创建一个对象实例
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms);

	//创建多个同资源名的对象实例
	void BuildMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms);


protected:

	//获取Object资源结构体
	FObjectWealthEntry* GetObjectSingleEntry(FName WealthName);
	TArray<FObjectWealthEntry*> GetObjectKindEntry(FName WealthKind);

	//获取Class资源结构体
	FClassWealthEntry* GetClassSingleEntry(FName WealthName);
	TArray<FClassWealthEntry*> GetClassKindEntry(FName WealthKind);

	//处理加载单个Object节点的方法
	void DealObjectSingleLoadStack();
	//处理批量加载Object节点的方法
	void DealObjectKindLoadStack();
	//处理加载单个Class节点的方法
	void DealClassSingleLoadStack();
	//处理批量加载Class节点的方法
	void DealClassKindLoadStack();
	//处理创建多个对象的方法
	void DealClassMultiLoadStack();

protected:

	//加载器
	FStreamableManager WealthLoader;

	//资源组
	TArray<UWealthData*> WealthData;

	UPROPERTY()
		TArray<UUserWidget*> GCWidgetGroup;

	//加载节点队列
	TArray<ObjectSingleLoadNode*> ObjectSingleLoadStack;
	TArray<ObjectKindLoadNode*> ObjectKindLoadStack;
	TArray<ClassSingleLoadNode*> ClassSingleLoadStack;
	TArray<ClassKindLoadNode*> ClassKindLoadStack;
	TArray<ClassMultiLoadNode*> ClassMultiLoadStack;


protected:

	//加载UObject反射回调函数
	DDOBJFUNC_TWO(BackObjectWealth, FName, BackName, UObject*, BackWealth);
	DDOBJFUNC_TWO(BackObjectWealthKind, TArray<FName>, BackNames, TArray<UObject*>, BackWealths);

	//加载UClass反射回调函数
	DDOBJFUNC_TWO(BackClassWealth, FName, BackName, UClass*, BackWealth);
	DDOBJFUNC_TWO(BackClassWealthKind, TArray<FName>, BackNames, TArray<UClass*>, BackWealths);

	//生成对象反射回调函数
	DDOBJFUNC_TWO(BackObjectSingle, FName, BackName, UObject*, BackObject);
	DDOBJFUNC_TWO(BackObjectKind, TArray<FName>, BackNames, TArray<UObject*>, BackObjects);
	DDOBJFUNC_TWO(BackObjectMulti, FName, BackName, TArray<UObject*>, BackObjects);

	DDOBJFUNC_TWO(BackActorSingle, FName, BackName, AActor*, BackActor);
	DDOBJFUNC_TWO(BackActorKind, TArray<FName>, BackNames, TArray<AActor*>, BackActors);
	DDOBJFUNC_TWO(BackActorMulti, FName, BackName, TArray<AActor*>, BackActors);

	DDOBJFUNC_TWO(BackWidgetSingle, FName, BackName, UUserWidget*, BackWidget);
	DDOBJFUNC_TWO(BackWidgetKind, TArray<FName>, BackNames, TArray<UUserWidget*>, BackWidgets);
	DDOBJFUNC_TWO(BackWidgetMulti, FName, BackName, TArray<UUserWidget*>, BackWidgets);
	
};
