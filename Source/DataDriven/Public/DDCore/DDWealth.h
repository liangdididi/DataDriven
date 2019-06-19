// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDMM.h"
#include "UObject/NoExportTypes.h"
#include "SoftObjectPath.h"
#include "Engine/StreamableManager.h"
#include "DDWealth.generated.h"


struct ClassSingleLoadNode;
struct ClassMultiLoadNode;
struct ClassKindLoadNode;

struct ObjectSingleLoadNode;
struct ObjectKindLoadNode;

class AActor;
class UUserWidget;

class UDDObject;
class ADDActor;
class UDDUserWidget;

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

	void AssignData(TArray<UWealthData*>& InWealthData);

	//外部方法, 创建新的Class资源
	void BuildSingleClassWealth(EWealthType WealthType, FName WealthName, FName ObjectName, FName FunName, FTransform SpawnTransform);
	void BuildMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms);
	void BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms);

	//外部方法加载Class资源
	void LoadClassWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadClassWealthKind(FName WealthKind, FName ObjectName, FName FunName);

	//外部方法获取Object资源
	void LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName);
	void LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName);

	//外部方法单纯的获取资源链接
	FWealthURL* GainWealthURL(FName WealthName);
	void GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL);

public:

protected:

	//遍历器, 获取对应资源路径
	FClassWealthEntry* GetClassSinglePath(FName WealthName);

	//遍历器, 获取对应资源路径
	TArray<FClassWealthEntry*> GetClassKindPath(FName WealthKind);

	//遍历器, 获取对应资源路径
	FObjectWealthEntry* GetObjectSinglePath(FName WealthName);

	//遍历器, 获取对应资源路径
	TArray<FObjectWealthEntry*> GetObjectKindPath(FName WealthKind);

	//处理单独加载资源队列
	void DealClassSingleLoadStack();

	//处理批量生产的资源队列
	void DealClassMultiLoadStack();

	//处理资源种类生产队列
	void DealClassKindLoadStack();

	//处理单独加载UE资源队列
	void DealObjectSingleLoadStack();

	//处理UE资源种类加载队列
	void DealObjectKindLoadStack();

protected:

	FStreamableManager WealthLoader;

	//资源组
	TArray<UWealthData*> WealthData;

	//等待加载的队列
	TArray<ClassSingleLoadNode*> ClassSingleLoadStack;

	TArray<ClassMultiLoadNode*> ClassMultiLoadStack;

	TArray<ClassKindLoadNode*> ClassKindLoadStack;

	TArray<ObjectSingleLoadNode*> ObjectSingleLoadStack;

	TArray<ObjectKindLoadNode*> ObjectKindLoadStack;

	UPROPERTY()
		TArray<UUserWidget*> GCWidgetGroup;

protected:

	DDOBJFUNC_TWO(BackObject, FName, BackName, UObject*, BackObject);
	DDOBJFUNC_TWO(BackObjects, FName, BackName, TArray<UObject*>, BackObjects);
	DDOBJFUNC_TWO(BackObjectKind, TArray<FName>, BackNames, TArray<UObject*>, BackObjects);

	DDOBJFUNC_TWO(BackActor, FName, BackName, AActor*, BackActor);
	DDOBJFUNC_TWO(BackActors, FName, BackName, TArray<AActor*>, BackActors);
	DDOBJFUNC_TWO(BackActorKind, TArray<FName>, BackNames, TArray<AActor*>, BackActors);

	DDOBJFUNC_TWO(BackWidget, FName, BackName, UUserWidget*, BackWidget);
	DDOBJFUNC_TWO(BackWidgets, FName, BackName, TArray<UUserWidget*>, BackWidgets);
	DDOBJFUNC_TWO(BackWidgetKind, TArray<FName>, BackNames, TArray<UUserWidget*>, BackWidgets);

	DDOBJFUNC_TWO(BackClassWealth, FName, BackName, UClass*, BackWealth);
	DDOBJFUNC_TWO(BackClassWealthKind, TArray<FName>, BackNames, TArray<UClass*>, BackWealths);

	//资源类的不在这里Cast, 直接返回UObject
	DDOBJFUNC_TWO(BackObjectWealth, FName, BackName, UObject*, BackWealth);
	DDOBJFUNC_TWO(BackObjectWealthKind, TArray<FName>, BackNames, TArray<UObject*>, BackWealths);

};




//异步加载多个资源, 不返回句柄
/*template<typename UserClass>
void AsynMultiLoad(TArray<FStringAssetReference>& WealthPaths, UserClass InObject, typename FStreamableDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod);*/

//使用UObjectLibrary批量加载资源到内存
//void LibraryLoad(FString FilePath, TSubclassOf<UObject> ObjectClass);

//template<typename WealthClass>
//WealthClass* UDDWealth::SynSingleLoad(FStringAssetReference WealthPath)
//{
//	return Cast<WealthClass*>(WealthLoader->SynchronousLoad(WealthPath));
//}


//template<typename WealthClass>
//TArray<WealthClass*>&
//UDDWealth::SynMultiLoad(TArray<FStringAssetReference>& WealthPaths)
//{
//	TArray<FSoftObjectPath> SoftPaths;
//	for (int32 i = 0; i < WealthPaths.Num(); ++i)
//		SoftPaths.AddUnique(WealthPaths[i]);
//	TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader->RequestSyncLoad(SoftPaths);
//	TArray<WealthClass*> WealthGroup;
//	TArray<UObject*> ObjectGroup;
//	if (WealthHandle->HasLoadCompleted())
//		WealthHandle->GetLoadedAssets(ObjectGroup);
//	for (UObject* Obj : ObjectGroup)
//		WealthGroup.Add(Cast<WealthClass>(Obj));
//	return WealthGroup;
//}

//template<typename UserClass>
//void UDDWealth::AsynMultiLoad(TArray<FStringAssetReference>& WealthPaths, UserClass InObject, typename FStreamableDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
//{
//	TArray<FSoftObjectPath> SoftPaths;
//	for (int32 i = 0; i < WealthPaths.Num(); ++i)
//		SoftPaths.AddUnique(WealthPaths[i]);
//	WealthLoader->RequestAsyncLoad(SoftPaths, FStreamableDelegate::CreateUObject(InObject, InMethod));
//}

