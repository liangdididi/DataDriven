// Fill out your copyright notice in the Description page of Project Settings.


#include "DDWealth.h"
#include "DDOO.h"
#include "Blueprint/UserWidget.h"


struct ObjectSingleLoadNode
{
	//加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//资源结构体
	FObjectWealthEntry* WealthEntry;
	//请求对象名
	FName ObjectName;
	//回调方法名
	FName FunName;
	//构造函数
	ObjectSingleLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, FObjectWealthEntry* InWealthEntry, FName InObjectName, FName InFunName)
	{
		WealthHandle = InWealthHandle;
		WealthEntry = InWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
	}
};


struct ObjectKindLoadNode
{
	//加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//没有加载的资源
	TArray<FObjectWealthEntry*> UnLoadWealthEntry;
	//已经加载的资源的数组
	TArray<FObjectWealthEntry*> LoadWealthEntry;
	//请求对象名
	FName ObjectName;
	//回调方法名
	FName FunName;
	//构造函数
	ObjectKindLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, TArray<FObjectWealthEntry*>& InUnLoadWealthEntry, TArray<FObjectWealthEntry*>& InLoadWealthEntry, FName InObjectName, FName InFunName)
	{
		WealthHandle = InWealthHandle;
		UnLoadWealthEntry = InUnLoadWealthEntry;
		LoadWealthEntry = InLoadWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
	}
};


struct ClassSingleLoadNode
{
	//加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//资源结构体
	FClassWealthEntry* WealthEntry;
	//请求对象名
	FName ObjectName;
	//回调方法名
	FName FunName;
	//生成位置
	FTransform SpawnTransform;
	//是否只加载UClass
	bool IsLoadClass;
	//构造函数
	ClassSingleLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, FClassWealthEntry* InWealthEntry, FName InObjectName, FName InFunName)
	{
		WealthHandle = InWealthHandle;
		WealthEntry = InWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
		IsLoadClass = true;
	}
	ClassSingleLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, FClassWealthEntry* InWealthEntry, FName InObjectName, FName InFunName, FTransform InSpawnTransform)
	{
		WealthHandle = InWealthHandle;
		WealthEntry = InWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
		SpawnTransform = InSpawnTransform;
		IsLoadClass = false;
	}
};


struct ClassKindLoadNode
{
	//加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//没有加载的资源
	TArray<FClassWealthEntry*> UnLoadWealthEntry;
	//已经加载的资源的数组
	TArray<FClassWealthEntry*> LoadWealthEntry;
	//请求对象名
	FName ObjectName;
	//回调方法名
	FName FunName;
	//生成位置
	TArray<FTransform> SpawnTransforms;
	//是否只加载UClass
	bool IsLoadClass;
	//保存生成的对象与名字
	TArray<FName> NameGroup;
	TArray<UObject*> ObjectGroup;
	TArray<AActor*> ActorGroup;
	TArray<UUserWidget*> WidgetGroup;
	//构造函数
	ClassKindLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, TArray<FClassWealthEntry*>& InUnLoadWealthEntry, TArray<FClassWealthEntry*>& InLoadWealthEntry, FName InObjectName, FName InFunName)
	{
		WealthHandle = InWealthHandle;
		UnLoadWealthEntry = InUnLoadWealthEntry;
		LoadWealthEntry = InLoadWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
		IsLoadClass = true;
	}
	ClassKindLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, TArray<FClassWealthEntry*>& InUnLoadWealthEntry, TArray<FClassWealthEntry*>& InLoadWealthEntry, FName InObjectName, FName InFunName, TArray<FTransform> InSpawnTransforms)
	{
		WealthHandle = InWealthHandle;
		UnLoadWealthEntry = InUnLoadWealthEntry;
		LoadWealthEntry = InLoadWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
		SpawnTransforms = InSpawnTransforms;
		IsLoadClass = false;
	}
};

struct ClassMultiLoadNode
{
	//加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//资源结构体
	FClassWealthEntry* WealthEntry;
	//请求对象名
	FName ObjectName;
	//回调方法名
	FName FunName;
	//生成数量
	int32 Amount;
	//生成位置
	TArray<FTransform> SpawnTransforms;
	//保存生成的对象
	TArray<UObject*> ObjectGroup;
	TArray<AActor*> ActorGroup;
	TArray<UUserWidget*> WidgetGroup;
	//构造函数, 未加载时使用
	ClassMultiLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, FClassWealthEntry* InWealthEntry, int32 InAmount, FName InObjectName, FName InFunName, TArray<FTransform> InSpawnTransforms)
	{
		WealthHandle = InWealthHandle;
		WealthEntry = InWealthEntry;
		Amount = InAmount;
		ObjectName = InObjectName;
		FunName = InFunName;
		SpawnTransforms = InSpawnTransforms;
	}
	//构造函数, 已加载时使用
	ClassMultiLoadNode(FClassWealthEntry* InWealthEntry, int32 InAmount, FName InObjectName, FName InFunName, TArray<FTransform> InSpawnTransforms)
	{
		WealthEntry = InWealthEntry;
		Amount = InAmount;
		ObjectName = InObjectName;
		FunName = InFunName;
		SpawnTransforms = InSpawnTransforms;
	}
};


void UDDWealth::WealthBeginPlay()
{
	//遍历自动生成对象
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		//生成Object对象
		for (int j = 0; j < WealthData[i]->AutoObjectData.Num(); ++j)
		{
			//根据获取到的UClass生成指定的对象
			UObject* NewObj = NewObject<UObject>(this, WealthData[i]->AutoObjectData[j].WealthClass);
			NewObj->AddToRoot();
			IDDOO* InstPtr = Cast<IDDOO>(NewObj);
			if (InstPtr)
			{
				InstPtr->RegisterToModule(
					WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
					WealthData[i]->AutoObjectData[j].ObjectName,
					WealthData[i]->AutoObjectData[j].ClassName
				);
			}
		}
		//生成Actor对象
		for (int j = 0; j < WealthData[i]->AutoActorData.Num(); ++j)
		{
			AActor* NewAct = GetDDWorld()->SpawnActor<AActor>(WealthData[i]->AutoActorData[j].WealthClass, WealthData[i]->AutoActorData[j].Transform);
			IDDOO* InstPtr = Cast<IDDOO>(NewAct);
			if (InstPtr)
			{
				InstPtr->RegisterToModule(
					WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
					WealthData[i]->AutoActorData[j].ObjectName,
					WealthData[i]->AutoActorData[j].ClassName
				);
			}
		}
		//生成Widget对象
		for (int j = 0; j < WealthData[i]->AutoWidgetData.Num(); ++j)
		{
			UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetDDWorld(), WealthData[i]->AutoWidgetData[j].WealthClass);
			//避免回收
			GCWidgetGroup.Push(NewWidget);
			IDDOO* InstPtr = Cast<IDDOO>(NewWidget);
			if (InstPtr)
			{
				InstPtr->RegisterToModule(
					WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
					WealthData[i]->AutoWidgetData[j].ObjectName,
					WealthData[i]->AutoWidgetData[j].ClassName
				);
			}
		}

#if WITH_EDITOR
		//循环设置WealthObject和WealthClass为空, 目的在于每次从编辑器启动游戏时资源Asset的状态都重置
		for (int j = 0; j < WealthData[i]->ObjectWealthData.Num(); ++j)
			WealthData[i]->ObjectWealthData[j].WealthObject = NULL;
		for (int j = 0; j < WealthData[i]->ClassWealthData.Num(); ++j)
			WealthData[i]->ClassWealthData[j].WealthClass = NULL;
#endif
	}
}

void UDDWealth::WealthTick(float DeltaSeconds)
{
	DealObjectSingleLoadStack();
	DealObjectKindLoadStack();
	DealClassSingleLoadStack();
	DealClassKindLoadStack();
	DealClassMultiLoadStack();
}

void UDDWealth::AssignData(TArray<UWealthData*>& InWealthData)
{
	WealthData = InWealthData;
}

FWealthURL* UDDWealth::GainWealthURL(FName WealthName)
{
	for (int i = 0; i < WealthData.Num(); ++i)
		for (int j = 0; j < WealthData[i]->WealthURL.Num(); ++j)
			if (WealthData[i]->WealthURL[j].WealthName.IsEqual(WealthName))
				return &WealthData[i]->WealthURL[j];
	return NULL;
}

void UDDWealth::GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL)
{
	for (int i = 0; i < WealthData.Num(); ++i)
		for (int j = 0; j < WealthData[i]->WealthURL.Num(); ++j)
			if (WealthData[i]->WealthURL[j].WealthKind.IsEqual(WealthKind))
				OutURL.Push(&WealthData[i]->WealthURL[j]);
}

void UDDWealth::LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName)
{
	//获取资源结构体
	FObjectWealthEntry* WealthEntry = GetObjectSingleEntry(WealthName);
	//如果为空
	if (!WealthEntry)
	{
		DDH::Debug() << ObjectName << " Get Null Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源不可用
	if (!WealthEntry->WealthPath.IsValid())
	{
		DDH::Debug() << ObjectName << " Get UnValid Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源已经加载
	if (WealthEntry->WealthObject)
	{
		//直接返回已经存在的资源给对象
		BackObjectWealth(ModuleIndex, ObjectName, FunName, WealthName, WealthEntry->WealthObject);
	}
	else
	{
		//进行异步加载
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPath);
		//添加新节点到加载序列
		ObjectSingleLoadStack.Push(new ObjectSingleLoadNode(WealthHandle, WealthEntry, ObjectName, FunName));
	}
}


void UDDWealth::LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName)
{
	TArray<FObjectWealthEntry*> WealthEntryGroup = GetObjectKindEntry(WealthKind);
	//如果数量为0
	if (WealthEntryGroup.Num() == 0)
	{
		DDH::Debug() << ObjectName << " Get Null WealthKind : " << WealthKind << DDH::Endl();
		return;
	}
	//判断资源可用性
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (!WealthEntryGroup[i]->WealthPath.IsValid())
		{
			DDH::Debug() << ObjectName << " Get Not Valid in Kind : " << WealthKind << " For Name : " << WealthEntryGroup[i]->WealthName << DDH::Endl();
			return;
		}
	}
	//还没有加载的资源
	TArray<FObjectWealthEntry*> UnLoadWealthEntry;
	//已经加载的资源
	TArray<FObjectWealthEntry*> LoadWealthEntry;
	//资源加载与否归类
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (WealthEntryGroup[i]->WealthObject)
			LoadWealthEntry.Push(WealthEntryGroup[i]);
		else
			UnLoadWealthEntry.Push(WealthEntryGroup[i]);
	}
	//如果未加载的资源为0
	if (UnLoadWealthEntry.Num() == 0)
	{
		//直接获取所有资源给请求对象
		TArray<FName> NameGroup;
		TArray<UObject*> WealthGroup;
		for (int i = 0; i < LoadWealthEntry.Num(); ++i)
		{
			NameGroup.Push(LoadWealthEntry[i]->WealthName);
			WealthGroup.Push(LoadWealthEntry[i]->WealthObject);
		}
		BackObjectWealthKind(ModuleIndex, ObjectName, FunName, NameGroup, WealthGroup);
	}
	else
	{
		//获取资源路径
		TArray<FSoftObjectPath> WealthPaths;
		for (int i = 0; i < UnLoadWealthEntry.Num(); ++i)
			WealthPaths.Push(UnLoadWealthEntry[i]->WealthPath);
		//进行异步加载获取句柄
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthPaths);
		//生成加载节点
		ObjectKindLoadStack.Push(new ObjectKindLoadNode(WealthHandle, UnLoadWealthEntry, LoadWealthEntry, ObjectName, FunName));
	}
}

void UDDWealth::LoadClassWealth(FName WealthName, FName ObjectName, FName FunName)
{
	//获取资源结构体
	FClassWealthEntry* WealthEntry = GetClassSingleEntry(WealthName);
	//如果为空
	if (!WealthEntry)
	{
		DDH::Debug() << ObjectName << " Get Null Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源不可用
	if (!WealthEntry->WealthPtr.ToSoftObjectPath().IsValid())
	{
		DDH::Debug() << ObjectName << " Get UnValid Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源已经加载
	if (WealthEntry->WealthClass)
	{
		//直接把资源返回给申请对象
		BackClassWealth(ModuleIndex, ObjectName, FunName, WealthName, WealthEntry->WealthClass);
	}
	else
	{
		//进行异步加载
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPtr.ToSoftObjectPath());
		//添加节点
		ClassSingleLoadStack.Push(new ClassSingleLoadNode(WealthHandle, WealthEntry, ObjectName, FunName));
	}
}

void UDDWealth::LoadClassWealthKind(FName WealthKind, FName ObjectName, FName FunName)
{
	TArray<FClassWealthEntry*> WealthEntryGroup = GetClassKindEntry(WealthKind);
	//如果数量为0
	if (WealthEntryGroup.Num() == 0)
	{
		DDH::Debug() << ObjectName << " Get Null WealthKind : " << WealthKind << DDH::Endl();
		return;
	}
	//判断资源可用性
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (!WealthEntryGroup[i]->WealthPtr.ToSoftObjectPath().IsValid())
		{
			DDH::Debug() << ObjectName << " Get Not Valid in Kind : " << WealthKind << " For Name : " << WealthEntryGroup[i]->WealthName << DDH::Endl();
			return;
		}
	}
	//未加载资源序列
	TArray<FClassWealthEntry*> UnLoadWealthEntry;
	//已加载资源序列
	TArray<FClassWealthEntry*> LoadWealthEntry;
	//分类保存
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (WealthEntryGroup[i]->WealthClass)
			LoadWealthEntry.Push(WealthEntryGroup[i]);
		else
			UnLoadWealthEntry.Push(WealthEntryGroup[i]);
	}
	//判断所有资源是否都已经加载
	if (UnLoadWealthEntry.Num() == 0)
	{
		//填充参数
		TArray<FName> NameGroup;
		TArray<UClass*> WealthGroup;
		for (int i = 0; i < LoadWealthEntry.Num(); ++i)
		{
			NameGroup.Push(LoadWealthEntry[i]->WealthName);
			WealthGroup.Push(LoadWealthEntry[i]->WealthClass);
		}
		//返回资源给请求对象
		BackClassWealthKind(ModuleIndex, ObjectName, FunName, NameGroup, WealthGroup);
	}
	else
	{
		//获取未加载资源路径数组
		TArray<FSoftObjectPath> WealthPaths;
		for (int i = 0; i < UnLoadWealthEntry.Num(); ++i)
			WealthPaths.Push(UnLoadWealthEntry[i]->WealthPtr.ToSoftObjectPath());
		//进行异步加载获取句柄
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthPaths);
		//添加新的加载节点
		ClassKindLoadStack.Push(new ClassKindLoadNode(WealthHandle, UnLoadWealthEntry, LoadWealthEntry, ObjectName, FunName));
	}
}

void UDDWealth::BuildSingleClassWealth(EWealthType WealthType, FName WealthName, FName ObjectName, FName FunName, FTransform SpawnTransform)
{
	//获取对应的资源结构体
	FClassWealthEntry* WealthEntry = GetClassSingleEntry(WealthName);
	//如果为空
	if (!WealthEntry)
	{
		DDH::Debug() << ObjectName << " Get Null Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源不可用
	if (!WealthEntry->WealthPtr.ToSoftObjectPath().IsValid())
	{
		DDH::Debug() << ObjectName << " Get UnValid Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//资源类型是否匹配
	if (WealthEntry->WealthType != WealthType)
	{
		DDH::Debug() << ObjectName << " Get Error Type : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源已经加载
	if (WealthEntry->WealthClass)
	{
		//生成并且传递对象到请求者
		if (WealthType == EWealthType::Object)
		{
			UObject* InstObject = NewObject<UObject>(this, WealthEntry->WealthClass);
			InstObject->AddToRoot();
			BackObjectSingle(ModuleIndex, ObjectName, FunName, WealthName, InstObject);
		}
		else if (WealthType == EWealthType::Actor)
		{
			AActor* InstActor = GetDDWorld()->SpawnActor<AActor>(WealthEntry->WealthClass, SpawnTransform);
			BackActorSingle(ModuleIndex, ObjectName, FunName, WealthName, InstActor);
		}
		else if (WealthType == EWealthType::Widget)
		{
			UUserWidget* InstWidget = CreateWidget<UUserWidget>(GetDDWorld(), WealthEntry->WealthClass);
			//避免回收
			GCWidgetGroup.Push(InstWidget);
			BackWidgetSingle(ModuleIndex, ObjectName, FunName, WealthName, InstWidget);
		}
	}
	else
	{
		//异步加载, 获取加载句柄
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPtr.ToSoftObjectPath());
		//创建新加载节点
		ClassSingleLoadStack.Push(new ClassSingleLoadNode(WealthHandle, WealthEntry, ObjectName, FunName, SpawnTransform));
	}
}

void UDDWealth::BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms)
{
	TArray<FClassWealthEntry*> WealthEntrytGroup = GetClassKindEntry(WealthKind);
	//判断为0
	if (WealthEntrytGroup.Num() == 0)
	{
		DDH::Debug() << ObjectName << " Get Null WealthKind : " << WealthKind << DDH::Endl();
		return;
	}
	for (int i = 0; i < WealthEntrytGroup.Num(); ++i)
	{
		//资源可用性
		if (!WealthEntrytGroup[i]->WealthPtr.ToSoftObjectPath().IsValid())
		{
			DDH::Debug() << ObjectName << " Get Not Vaild In Kind : " << WealthKind << " For Name : " << WealthEntrytGroup[i]->WealthName << DDH::Endl();
			return;
		}
		//资源类型匹配
		if (WealthEntrytGroup[i]->WealthType != WealthType)
		{
			DDH::Debug() << ObjectName << " Get Error Type In Kind : " << WealthKind << " For Name : " << WealthEntrytGroup[i]->WealthName << DDH::Endl();
			return;
		}
	}
	//判断Transform数组是否为1或者是否为WealthEntrytGroup的数量
	if (WealthType == EWealthType::Actor && SpawnTransforms.Num() != 1 && SpawnTransforms.Num() != WealthEntrytGroup.Num())
	{
		DDH::Debug() << ObjectName << " Send Error Spawn Count : " << WealthKind << DDH::Endl();
		return;
 	}
	//未加载的资源链接
	TArray<FClassWealthEntry*> UnLoadWealthEntry;
	//已加载资源链接
	TArray<FClassWealthEntry*> LoadWealthEntry;
	//资源分类
	for (int i = 0; i < WealthEntrytGroup.Num(); ++i)
	{
		if (WealthEntrytGroup[i]->WealthClass)
			LoadWealthEntry.Push(WealthEntrytGroup[i]);
		else
			UnLoadWealthEntry.Push(WealthEntrytGroup[i]);
	}
	//声明一个加载句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//如果有未加载的资源
	if (UnLoadWealthEntry.Num() > 0)
	{
		//获取资源路径
		TArray<FSoftObjectPath> WealthPaths;
		for (int i = 0; i < UnLoadWealthEntry.Num(); ++i)
			WealthPaths.Push(UnLoadWealthEntry[i]->WealthPtr.ToSoftObjectPath());
		//获取加载句柄
		WealthHandle = WealthLoader.RequestAsyncLoad(WealthPaths);
	}
	//创建帧处理的节点
	ClassKindLoadStack.Push(new ClassKindLoadNode(WealthHandle, UnLoadWealthEntry, LoadWealthEntry, ObjectName, FunName, SpawnTransforms));
}

void UDDWealth::BuildMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms)
{
	//获取对应的资源结构体
	FClassWealthEntry* WealthEntry = GetClassSingleEntry(WealthName);
	//如果为空
	if (!WealthEntry)
	{
		DDH::Debug() << ObjectName << " Get Null Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源不可用
	if (!WealthEntry->WealthPtr.ToSoftObjectPath().IsValid())
	{
		DDH::Debug() << ObjectName << " Get UnValid Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//资源类型是否匹配
	if (WealthEntry->WealthType != WealthType)
	{
		DDH::Debug() << ObjectName << " Get Error Type : " << WealthName << DDH::Endl();
		return;
	}
	//验证Transform数组的数量是否为1或者为Amount, 或则Amount = 0
	if ((WealthType == EWealthType::Actor && SpawnTransforms.Num() != 1 && SpawnTransforms.Num() != Amount) || Amount == 0)
	{
		DDH::Debug() << ObjectName << " Send Error Spawn Count : " << WealthName << DDH::Endl();
		return;
	}
	//如果已经加载资源
	if (WealthEntry->WealthClass)
		ClassMultiLoadStack.Push(new ClassMultiLoadNode(WealthEntry, Amount, ObjectName, FunName, SpawnTransforms));
	else
	{
		//异步加载
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPtr.ToSoftObjectPath());
		//添加新节点
		ClassMultiLoadStack.Push(new ClassMultiLoadNode(WealthHandle, WealthEntry, Amount, ObjectName, FunName, SpawnTransforms));
	}
}

FObjectWealthEntry* UDDWealth::GetObjectSingleEntry(FName WealthName)
{
	for (int i = 0; i < WealthData.Num(); ++i)
		for (int j = 0; j < WealthData[i]->ObjectWealthData.Num(); ++j)
			if (WealthData[i]->ObjectWealthData[j].WealthName.IsEqual(WealthName))
				return &(WealthData[i]->ObjectWealthData[j]);
	return NULL;
}

TArray<FObjectWealthEntry*> UDDWealth::GetObjectKindEntry(FName WealthKind)
{
	TArray<FObjectWealthEntry*> WealthGroup;
	for (int i = 0; i < WealthData.Num(); ++i)
		for (int j = 0; j < WealthData[i]->ObjectWealthData.Num(); ++j)
			if (WealthData[i]->ObjectWealthData[j].WealthKind.IsEqual(WealthKind))
				WealthGroup.Push(&(WealthData[i]->ObjectWealthData[j]));
	return WealthGroup;
}

FClassWealthEntry* UDDWealth::GetClassSingleEntry(FName WealthName)
{
	for (int i = 0; i < WealthData.Num(); ++i)
		for (int j = 0; j < WealthData[i]->ClassWealthData.Num(); ++j)
			if (WealthData[i]->ClassWealthData[j].WealthName.IsEqual(WealthName))
				return &(WealthData[i]->ClassWealthData[j]);
	return NULL;
}

TArray<FClassWealthEntry*> UDDWealth::GetClassKindEntry(FName WealthKind)
{
	TArray<FClassWealthEntry*> WealthGroup;
	for (int i = 0; i < WealthData.Num(); ++i)
		for (int j = 0; j < WealthData[i]->ClassWealthData.Num(); ++j)
			if (WealthData[i]->ClassWealthData[j].WealthKind.IsEqual(WealthKind))
				WealthGroup.Push(&(WealthData[i]->ClassWealthData[j]));
	return WealthGroup;
}

void UDDWealth::DealObjectSingleLoadStack()
{
	//定义加载完成的序列
	TArray<ObjectSingleLoadNode*> CompleteStack;
	for (int i = 0; i < ObjectSingleLoadStack.Num(); ++i)
	{
		//判断是否已经加载完成
		if (ObjectSingleLoadStack[i]->WealthHandle->HasLoadCompleted())
		{
			//设置对应资源完成
			ObjectSingleLoadStack[i]->WealthEntry->WealthObject = ObjectSingleLoadStack[i]->WealthEntry->WealthPath.ResolveObject();
			//返回资源给对象
			BackObjectWealth(ModuleIndex, ObjectSingleLoadStack[i]->ObjectName, ObjectSingleLoadStack[i]->FunName, ObjectSingleLoadStack[i]->WealthEntry->WealthName, ObjectSingleLoadStack[i]->WealthEntry->WealthObject);
			//添加已经加载完成的节点到临时序列
			CompleteStack.Push(ObjectSingleLoadStack[i]);
		}
	}
	//销毁已经完成的节点
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		//移除出节点序列
		ObjectSingleLoadStack.Remove(CompleteStack[i]);
		//释放内存
		delete CompleteStack[i];
	}
}

void UDDWealth::DealObjectKindLoadStack()
{
	//定义加载完成的序列
	TArray<ObjectKindLoadNode*> CompleteStack;
	for (int i = 0; i < ObjectKindLoadStack.Num(); ++i)
	{
		//判断是否已经加载完成
		if (ObjectKindLoadStack[i]->WealthHandle->HasLoadCompleted())
		{
			//返回资源参数
			TArray<FName> NameGroup;
			TArray<UObject*> WealthGroup;
			//填充已加载资源
			for (int j = 0; j < ObjectKindLoadStack[i]->LoadWealthEntry.Num(); ++j)
			{
				NameGroup.Push(ObjectKindLoadStack[i]->LoadWealthEntry[j]->WealthName);
				WealthGroup.Push(ObjectKindLoadStack[i]->LoadWealthEntry[j]->WealthObject);
			}
			//遍历设置所有未加载资源结构体为已加载状态
			for (int j = 0; j < ObjectKindLoadStack[i]->UnLoadWealthEntry.Num(); ++j)
			{
				ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthObject = ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthPath.ResolveObject();
				//填充已加载资源
				NameGroup.Push(ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthName);
				WealthGroup.Push(ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthObject);
			}
			//返回数据给请求对象
			BackObjectWealthKind(ModuleIndex, ObjectKindLoadStack[i]->ObjectName, ObjectKindLoadStack[i]->FunName, NameGroup, WealthGroup);
			//添加节点到已完成序列
			CompleteStack.Push(ObjectKindLoadStack[i]);
		}
	}
	//销毁已经完成的节点
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		//移除出节点序列
		ObjectKindLoadStack.Remove(CompleteStack[i]);
		//释放内存
		delete CompleteStack[i];
	}
}

void UDDWealth::DealClassSingleLoadStack()
{
	//定义加载完成的序列
	TArray<ClassSingleLoadNode*> CompleteStack;
	for (int i = 0; i < ClassSingleLoadStack.Num(); ++i)
	{
		//判断是否已经加载完成
		if (ClassSingleLoadStack[i]->WealthHandle->HasLoadCompleted())
		{
			//设置对应资源完成
			ClassSingleLoadStack[i]->WealthEntry->WealthClass = Cast<UClass>(ClassSingleLoadStack[i]->WealthEntry->WealthPtr.ToSoftObjectPath().ResolveObject());

			//判断是否生成对象
			if (ClassSingleLoadStack[i]->IsLoadClass)
			{
				//返回资源给对象
				BackClassWealth(ModuleIndex, ClassSingleLoadStack[i]->ObjectName, ClassSingleLoadStack[i]->FunName, ClassSingleLoadStack[i]->WealthEntry->WealthName, ClassSingleLoadStack[i]->WealthEntry->WealthClass);
			}
			else
			{
				//生成并且传递对象到请求者
				if (ClassSingleLoadStack[i]->WealthEntry->WealthType == EWealthType::Object)
				{
					UObject* InstObject = NewObject<UObject>(this, ClassSingleLoadStack[i]->WealthEntry->WealthClass);
					InstObject->AddToRoot();
					BackObjectSingle(ModuleIndex, ClassSingleLoadStack[i]->ObjectName, ClassSingleLoadStack[i]->FunName, ClassSingleLoadStack[i]->WealthEntry->WealthName, InstObject);
				}
				else if (ClassSingleLoadStack[i]->WealthEntry->WealthType == EWealthType::Actor)
				{
					AActor* InstActor = GetDDWorld()->SpawnActor<AActor>(ClassSingleLoadStack[i]->WealthEntry->WealthClass, ClassSingleLoadStack[i]->SpawnTransform);
					BackActorSingle(ModuleIndex, ClassSingleLoadStack[i]->ObjectName, ClassSingleLoadStack[i]->FunName, ClassSingleLoadStack[i]->WealthEntry->WealthName, InstActor);
				}
				else if (ClassSingleLoadStack[i]->WealthEntry->WealthType == EWealthType::Widget)
				{
					UUserWidget* InstWidget = CreateWidget<UUserWidget>(GetDDWorld(), ClassSingleLoadStack[i]->WealthEntry->WealthClass);
					//避免回收
					GCWidgetGroup.Push(InstWidget);
					BackWidgetSingle(ModuleIndex, ClassSingleLoadStack[i]->ObjectName, ClassSingleLoadStack[i]->FunName, ClassSingleLoadStack[i]->WealthEntry->WealthName, InstWidget);
				}
			}
			//添加已经加载完成的节点到临时序列
			CompleteStack.Push(ClassSingleLoadStack[i]);
		}
	}
	//销毁已经完成的节点
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		//移除出节点序列
		ClassSingleLoadStack.Remove(CompleteStack[i]);
		//释放内存
		delete CompleteStack[i];
	}
}

void UDDWealth::DealClassKindLoadStack()
{
	//定义已完成加载节点序列
	TArray<ClassKindLoadNode*> CompleteStack;
	for (int i = 0; i < ClassKindLoadStack.Num(); ++i)
	{
		//判断第一次加载完成, WealthHandle已经加载完成, UnLoadWealthEntry数量大于0
		if (ClassKindLoadStack[i]->WealthHandle.IsValid() && ClassKindLoadStack[i]->WealthHandle->HasLoadCompleted() && ClassKindLoadStack[i]->UnLoadWealthEntry.Num() > 0)
		{
			//如果已经加载完成, 设置未加载序列的资源指针
			for (int j = 0; j < ClassKindLoadStack[i]->UnLoadWealthEntry.Num(); ++j)
				ClassKindLoadStack[i]->UnLoadWealthEntry[j]->WealthClass = Cast<UClass>(ClassKindLoadStack[i]->UnLoadWealthEntry[j]->WealthPtr.ToSoftObjectPath().ResolveObject());
			//将未加载完成序列里的资源填充到已加载资源序列
			ClassKindLoadStack[i]->LoadWealthEntry.Append(ClassKindLoadStack[i]->UnLoadWealthEntry);
			//清空UnLoadWealthEntry
			ClassKindLoadStack[i]->UnLoadWealthEntry.Empty();
		}

		//如果未加载序列为0, 说明已经加载完成
		if (ClassKindLoadStack[i]->UnLoadWealthEntry.Num() == 0)
		{
			//加载UClass或者直接生成资源的情况来处理
			if (ClassKindLoadStack[i]->IsLoadClass)
			{
				//设置反射参数
				TArray<FName> NameGroup;
				TArray<UClass*> WealthGroup;
				for (int j = 0; j < ClassKindLoadStack[i]->LoadWealthEntry.Num(); ++j)
				{
					NameGroup.Push(ClassKindLoadStack[i]->LoadWealthEntry[j]->WealthName);
					WealthGroup.Push(ClassKindLoadStack[i]->LoadWealthEntry[j]->WealthClass);
				}
				//返回资源给请求对象
				BackClassWealthKind(ModuleIndex, ClassKindLoadStack[i]->ObjectName, ClassKindLoadStack[i]->FunName, NameGroup, WealthGroup);
				//添加该节点到已完成序列
				CompleteStack.Push(ClassKindLoadStack[i]);
			}
			else //如果要生成对象
			{
				//从已加载的资源数组中取出第一个
				FClassWealthEntry* WealthEntry = ClassKindLoadStack[i]->LoadWealthEntry[0];
				//移除出序列
				ClassKindLoadStack[i]->LoadWealthEntry.RemoveAt(0);
				//根据资源类型生成对象
				if (WealthEntry->WealthType == EWealthType::Object)
				{
					UObject* InstObject = NewObject<UObject>(this, WealthEntry->WealthClass);
					InstObject->AddToRoot();
					//添加找参数数组
					ClassKindLoadStack[i]->NameGroup.Push(WealthEntry->WealthName);
					ClassKindLoadStack[i]->ObjectGroup.Push(InstObject);
					//判断是否生成了全部的对象
					if (ClassKindLoadStack[i]->LoadWealthEntry.Num() == 0)
					{
						//给请求者传递生成的对象
						BackObjectKind(ModuleIndex, ClassKindLoadStack[i]->ObjectName, ClassKindLoadStack[i]->FunName, ClassKindLoadStack[i]->NameGroup, ClassKindLoadStack[i]->ObjectGroup);
						//添加到完成序列
						CompleteStack.Push(ClassKindLoadStack[i]);
					}
				}
				else if (WealthEntry->WealthType == EWealthType::Actor)
				{
					//获取生成位置
					FTransform SpawnTransform = ClassKindLoadStack[i]->SpawnTransforms.Num() == 1 ? ClassKindLoadStack[i]->SpawnTransforms[0] : ClassKindLoadStack[i]->SpawnTransforms[ClassKindLoadStack[i]->ActorGroup.Num()];
					//生成对象
					AActor* InstActor = GetDDWorld()->SpawnActor<AActor>(WealthEntry->WealthClass, SpawnTransform);
					//添加找参数数组
					ClassKindLoadStack[i]->NameGroup.Push(WealthEntry->WealthName);
					ClassKindLoadStack[i]->ActorGroup.Push(InstActor);
					//判断是否生成了全部的对象
					if (ClassKindLoadStack[i]->LoadWealthEntry.Num() == 0)
					{
						//给请求者传递生成的对象
						BackActorKind(ModuleIndex, ClassKindLoadStack[i]->ObjectName, ClassKindLoadStack[i]->FunName, ClassKindLoadStack[i]->NameGroup, ClassKindLoadStack[i]->ActorGroup);
						//添加到完成序列
						CompleteStack.Push(ClassKindLoadStack[i]);
					}
				}
				else if (WealthEntry->WealthType == EWealthType::Widget)
				{
					UUserWidget* InstWidget = CreateWidget<UUserWidget>(GetDDWorld(), WealthEntry->WealthClass);
					//避免回收
					GCWidgetGroup.Push(InstWidget);
					//添加找参数数组
					ClassKindLoadStack[i]->NameGroup.Push(WealthEntry->WealthName);
					ClassKindLoadStack[i]->WidgetGroup.Push(InstWidget);
					//判断是否生成了全部的对象
					if (ClassKindLoadStack[i]->LoadWealthEntry.Num() == 0)
					{
						//给请求者传递生成的对象
						BackWidgetKind(ModuleIndex, ClassKindLoadStack[i]->ObjectName, ClassKindLoadStack[i]->FunName, ClassKindLoadStack[i]->NameGroup, ClassKindLoadStack[i]->WidgetGroup);
						//添加到完成序列
						CompleteStack.Push(ClassKindLoadStack[i]);
					}
				}
			}
		}
	}
	//清空已完成节点
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		ClassKindLoadStack.Remove(CompleteStack[i]);
		delete CompleteStack[i];
	}
}

void UDDWealth::DealClassMultiLoadStack()
{
	//定义完成的节点
	TArray<ClassMultiLoadNode*> CompleteStack;
	for (int i = 0; i < ClassMultiLoadStack.Num(); ++i)
	{
		//如果没有加载UClass, 说明加载句柄有效
		if (!ClassMultiLoadStack[i]->WealthEntry->WealthClass)
		{
			//如果加载句柄加载完毕
			if (ClassMultiLoadStack[i]->WealthHandle->HasLoadCompleted())
				ClassMultiLoadStack[i]->WealthEntry->WealthClass = Cast<UClass>(ClassMultiLoadStack[i]->WealthHandle->GetLoadedAsset());
		}
		//再次判断WealthClass是否存在, 如果存在进入生成对象阶段
		if (ClassMultiLoadStack[i]->WealthEntry->WealthClass)
		{
			//区分类型生成对
			if (ClassMultiLoadStack[i]->WealthEntry->WealthType == EWealthType::Object)
			{
				UObject* InstObject = NewObject<UObject>(this, ClassMultiLoadStack[i]->WealthEntry->WealthClass);
				InstObject->AddToRoot();
				ClassMultiLoadStack[i]->ObjectGroup.Push(InstObject);
				//如果生产完毕
				if (ClassMultiLoadStack[i]->ObjectGroup.Num() == ClassMultiLoadStack[i]->Amount)
				{
					//返回对象给请求者
					BackObjectMulti(ModuleIndex, ClassMultiLoadStack[i]->ObjectName, ClassMultiLoadStack[i]->FunName, ClassMultiLoadStack[i]->WealthEntry->WealthName, ClassMultiLoadStack[i]->ObjectGroup);
					//添加到完成序列
					CompleteStack.Push(ClassMultiLoadStack[i]);
				}
			}
			else if (ClassMultiLoadStack[i]->WealthEntry->WealthType == EWealthType::Actor)
			{
				//获取生成位置
				FTransform SpawnTransform = ClassMultiLoadStack[i]->SpawnTransforms.Num() == 1 ? ClassMultiLoadStack[i]->SpawnTransforms[0] : ClassMultiLoadStack[i]->SpawnTransforms[ClassMultiLoadStack[i]->ActorGroup.Num()];
				//生成对象
				AActor* InstActor = GetDDWorld()->SpawnActor<AActor>(ClassMultiLoadStack[i]->WealthEntry->WealthClass, SpawnTransform);
				//添加参数数组
				ClassMultiLoadStack[i]->ActorGroup.Push(InstActor);
				//判断是否生成了全部的对象
				if (ClassMultiLoadStack[i]->ActorGroup.Num() == ClassMultiLoadStack[i]->Amount)
				{
					//给请求者传递生成的对象
					BackActorMulti(ModuleIndex, ClassMultiLoadStack[i]->ObjectName, ClassMultiLoadStack[i]->FunName, ClassMultiLoadStack[i]->WealthEntry->WealthName, ClassMultiLoadStack[i]->ActorGroup);
					//添加到完成序列
					CompleteStack.Push(ClassMultiLoadStack[i]);
				}
			}
			else if (ClassMultiLoadStack[i]->WealthEntry->WealthType == EWealthType::Widget)
			{
				UUserWidget* InstWidget = CreateWidget<UUserWidget>(GetDDWorld(), ClassMultiLoadStack[i]->WealthEntry->WealthClass);
				//避免回收
				GCWidgetGroup.Push(InstWidget);
				//添加参数数组
				ClassMultiLoadStack[i]->WidgetGroup.Push(InstWidget);
				//判断是否生成了全部的对象
				if (ClassMultiLoadStack[i]->WidgetGroup.Num() == ClassMultiLoadStack[i]->Amount)
				{
					//给请求者传递生成的对象
					BackWidgetMulti(ModuleIndex, ClassMultiLoadStack[i]->ObjectName, ClassMultiLoadStack[i]->FunName, ClassMultiLoadStack[i]->WealthEntry->WealthName, ClassMultiLoadStack[i]->WidgetGroup);
					//添加到完成序列
					CompleteStack.Push(ClassMultiLoadStack[i]);
				}
			}
		}
	}
	//清空已完成节点
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		ClassMultiLoadStack.Remove(CompleteStack[i]);
		delete CompleteStack[i];
	}
}
