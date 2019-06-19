// Fill out your copyright notice in the Description page of Project Settings.

#include "DDWealth.h"
#include "DDObject.h"
#include "DDActor.h"
#include "DDPawn.h"
#include "DDCharacter.h"
#include "DDUserWidget.h"

//加载单个资源的节点
struct ClassSingleLoadNode
{
	//资源句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//资源结构体
	FClassWealthEntry* WealthEntry;
	//目标对象名
	FName ObjectName;
	//方法名
	FName FunName;
	//生成位置
	FTransform SpawnTransform;
	//是否只加载Class
	bool LoadClass;
	//构造函数
	ClassSingleLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, FClassWealthEntry* InWealthEntry, FName InObjectName, FName InFunName, FTransform InTransform)
	{
		WealthHandle = InWealthHandle;
		WealthEntry = InWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
		SpawnTransform = InTransform;
		LoadClass = false;
	}
	ClassSingleLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, FClassWealthEntry* InWealthEntry, FName InObjectName, FName InFunName)
	{
		WealthHandle = InWealthHandle;
		WealthEntry = InWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
		LoadClass = true;
	}
};

//加载多个同类资源的节点
struct ClassMultiLoadNode
{
	//资源句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//资源结构体
	FClassWealthEntry* WealthEntry;
	//加载数量
	int32 Amount;
	//目标对象名
	FName ObjectName;
	//方法名
	FName FunName;
	//生成位置
	TArray<FTransform> SpawnTransforms;
	//保存生成的队列, 根据类型来选择
	TArray<UObject*> ObjectGroup;
	TArray<AActor*> ActorGroup;
	TArray<UUserWidget*> WidgetGroup;
	//构造函数一  :   未加载时使用
	ClassMultiLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, FClassWealthEntry* InWealthEntry, int32 InAmount, FName InObjectName, FName InFunName, TArray<FTransform> InTransforms)
	{
		WealthHandle = InWealthHandle;
		WealthEntry = InWealthEntry;
		Amount = InAmount;
		ObjectName = InObjectName;
		FunName = InFunName;
		SpawnTransforms = InTransforms;
	}
	//构造函数二  :  已经加载时使用
	ClassMultiLoadNode(FClassWealthEntry* InWealthEntry, int32 InAmount, FName InObjectName, FName InFunName, TArray<FTransform> InTransforms)
	{
		WealthEntry = InWealthEntry;
		Amount = InAmount;
		ObjectName = InObjectName;
		FunName = InFunName;
		SpawnTransforms = InTransforms;
	}
};




//加载同个WealthKind的节点
struct ClassKindLoadNode
{
	//资源句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//还没有加载的资源
	TArray<FClassWealthEntry*> UnLoadWealthEntry;
	//已经加载的资源
	TArray<FClassWealthEntry*> LoadWealthEntry;
	//目标对象名
	FName ObjectName;
	//方法名
	FName FunName;
	//生成位置
	TArray<FTransform> SpawnTransforms;
	//是否只加载Class
	bool LoadClass;
	//保存生成的队列
	TArray<FName> NameGroup;
	TArray<UObject*> ObjectGroup;
	TArray<AActor*> ActorGroup;
	TArray<UUserWidget*> WidgetGroup;
	//构造函数
	ClassKindLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, TArray<FClassWealthEntry*>& InUnLoadWealthEntry, TArray<FClassWealthEntry*>& InLoadWealthEntry, FName InObjectName, FName InFunName, TArray<FTransform> InTransforms)
	{
		WealthHandle = InWealthHandle;
		UnLoadWealthEntry = InUnLoadWealthEntry;
		LoadWealthEntry = InLoadWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
		SpawnTransforms = InTransforms;
		LoadClass = false;
	}
	ClassKindLoadNode(TSharedPtr<FStreamableHandle> InWealthHandle, TArray<FClassWealthEntry*>& InUnLoadWealthEntry, TArray<FClassWealthEntry*>& InLoadWealthEntry, FName InObjectName, FName InFunName)
	{
		WealthHandle = InWealthHandle;
		UnLoadWealthEntry = InUnLoadWealthEntry;
		LoadWealthEntry = InLoadWealthEntry;
		ObjectName = InObjectName;
		FunName = InFunName;
		LoadClass = true;
	}
};

struct ObjectSingleLoadNode
{
	//资源句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//资源结构体
	FObjectWealthEntry* WealthEntry;
	//目标对象名
	FName ObjectName;
	//方法名
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

//加载同个WealthKind的节点
struct ObjectKindLoadNode
{
	//资源句柄
	TSharedPtr<FStreamableHandle> WealthHandle;
	//还没有加载的资源
	TArray<FObjectWealthEntry*> UnLoadWealthEntry;
	//已经加载的资源
	TArray<FObjectWealthEntry*> LoadWealthEntry;
	//目标对象名
	FName ObjectName;
	//方法名
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


void UDDWealth::WealthBeginPlay()
{
	//循环遍历生成默认资源
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		//生成Object资源
		for (int j = 0; j < WealthData[i]->AutoObjectData.Num(); ++j)
		{
			//根据获取到的UClass生成指定的DD资源
			UDDObject* NewObj = NewObject<UDDObject>(this, WealthData[i]->AutoObjectData[j].WealthClass);
			NewObj->AddToRoot();
			NewObj->RegisterToModule(
				WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
				WealthData[i]->AutoObjectData[j].ObjectName,
				WealthData[i]->AutoObjectData[j].ClassName
			);
		}
		//生成Actor资源
		for (int j = 0; j < WealthData[i]->AutoActorData.Num(); ++j)
		{
			//根据获取到的UClass生成指定的DD资源
			ADDActor* NewAct = GetDDWorld()->SpawnActor<ADDActor>(WealthData[i]->AutoActorData[j].WealthClass, WealthData[i]->AutoActorData[j].Transform);
			NewAct->RegisterToModule(
				WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
				WealthData[i]->AutoActorData[j].ObjectName,
				WealthData[i]->AutoActorData[j].ClassName
			);
		}
		//生成Widget资源
		for (int j = 0; j < WealthData[i]->AutoWidgetData.Num(); ++j)
		{
			//根据获取到的UClass生成指定的DD资源
			UDDUserWidget* NewWidget = CreateWidget<UDDUserWidget>(GetDDWorld(), WealthData[i]->AutoWidgetData[j].WealthClass);
			//避免回收
			GCWidgetGroup.Push(NewWidget);
			NewWidget->RegisterToModule(
				WealthData[i]->ModuleName.IsNone() ? IModule->GetFName() : WealthData[i]->ModuleName,
				WealthData[i]->AutoWidgetData[j].ObjectName,
				WealthData[i]->AutoWidgetData[j].ClassName
			);
		}

#if WITH_EDITOR
		//循环将数据置空, 只在编辑器模式下使用
		for (int j = 0; j < WealthData[i]->ClassWealthData.Num(); ++j)
			WealthData[i]->ClassWealthData[j].WealthClass = NULL;
		for (int j = 0; j < WealthData[i]->ObjectWealthData.Num(); ++j)
			WealthData[i]->ObjectWealthData[j].WealthObject = NULL;
#endif
	}
}



void UDDWealth::AssignData(TArray<UWealthData*>& InWealthData)
{
	WealthData = InWealthData;
}


FClassWealthEntry* UDDWealth::GetClassSinglePath(FName WealthName)
{
	//循环遍历寻找资源
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		for (int j = 0; j < WealthData[i]->ClassWealthData.Num(); ++j)
		{
			if (WealthData[i]->ClassWealthData[j].WealthName.IsEqual(WealthName))
				return &(WealthData[i]->ClassWealthData[j]);
		}
	}
	return NULL;
}

TArray<FClassWealthEntry*> UDDWealth::GetClassKindPath(FName WealthKind)
{
	TArray<FClassWealthEntry*> WealthGroup;
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		for (int j = 0; j < WealthData[i]->ClassWealthData.Num(); ++j)
		{
			if (WealthData[i]->ClassWealthData[j].WealthKind.IsEqual(WealthKind))
				WealthGroup.Add(&(WealthData[i]->ClassWealthData[j]));
		}
	}
	return WealthGroup;
}

FObjectWealthEntry* UDDWealth::GetObjectSinglePath(FName WealthName)
{
	//循环遍历寻找资源
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		for (int j = 0; j < WealthData[i]->ObjectWealthData.Num(); ++j)
		{
			if (WealthData[i]->ObjectWealthData[j].WealthName.IsEqual(WealthName))
				return &(WealthData[i]->ObjectWealthData[j]);
		}
	}
	return NULL;
}

TArray<FObjectWealthEntry*> UDDWealth::GetObjectKindPath(FName WealthKind)
{
	TArray<FObjectWealthEntry*> WealthGroup;
	for (int i = 0; i < WealthData.Num(); ++i)
	{
		for (int j = 0; j < WealthData[i]->ObjectWealthData.Num(); ++j)
		{
			if (WealthData[i]->ObjectWealthData[j].WealthKind.IsEqual(WealthKind))
				WealthGroup.Add(&(WealthData[i]->ObjectWealthData[j]));
		}
	}
	return WealthGroup;
}

void UDDWealth::BuildSingleClassWealth(EWealthType WealthType, FName WealthName, FName ObjectName, FName FunName, FTransform SpawnTransform)
{
	//获取对应的资源结构体
	FClassWealthEntry* WealthEntry = GetClassSinglePath(WealthName);
	//如果为空
	if (!WealthEntry)
	{
		DDH::Debug() << ObjectName << " Get Null Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源不可用, 输出错误
	if (!WealthEntry->WealthPtr.ToSoftObjectPath().IsValid())
	{
		DDH::Debug() << ObjectName << " Get Not Valid : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源类型和DDObject不匹配
	if (WealthEntry->WealthType != WealthType)
	{
		DDH::Debug() << ObjectName << " Get Error Type : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源已经加载进内存, 直接生成新的资源
	if (WealthEntry->WealthClass)
	{
		if (WealthType == EWealthType::Object)
		{
			UObject * InstObject = NewObject<UObject>(this, WealthEntry->WealthClass);
			InstObject->AddToRoot();
			//给请求对象传递资源
			BackObject(ModuleIndex, ObjectName, FunName, WealthName, InstObject);
		}
		else if (WealthType == EWealthType::Actor)
		{
			AActor * InstActor = GetDDWorld()->SpawnActor<AActor>(WealthEntry->WealthClass, SpawnTransform);
			//给请求的对象传递资源
			BackActor(ModuleIndex, ObjectName, FunName, WealthName, InstActor);
		}
		else if (WealthType == EWealthType::Widget)
		{
			UUserWidget * InstWidget = CreateWidget<UUserWidget>(GetDDWorld(), WealthEntry->WealthClass);
			//避免回收
			GCWidgetGroup.Push(InstWidget);
			//给请求的对象传递资源
			BackWidget(ModuleIndex, ObjectName, FunName, WealthName, InstWidget);
		}
	}
	else
	{
		//如果资源没有加载, 先去异步加载
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPtr.ToSoftObjectPath());
		//加到加载栈
		ClassSingleLoadStack.Add(new ClassSingleLoadNode(WealthHandle, WealthEntry, ObjectName, FunName, SpawnTransform));
	}
}


void UDDWealth::BuildMultiClassWealth(EWealthType WealthType, FName WealthName, int32 Amount, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms)
{
	//获取对应的资源结构体
	FClassWealthEntry* WealthEntry = GetClassSinglePath(WealthName);
	//如果为空
	if (!WealthEntry)
	{
		DDH::Debug() << ObjectName << " Get Null Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源不可用, 输出错误
	if (!WealthEntry->WealthPtr.ToSoftObjectPath().IsValid())
	{
		DDH::Debug() << ObjectName << " Get Not Valid : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源类型和WealthType不匹配
	if (WealthEntry->WealthType != WealthType)
	{
		DDH::Debug() << ObjectName << " Get Error Type : " << WealthName << DDH::Endl();
		return;
	}
	//如果SpawnTransform的数量不为1而且不为Amount
	if (WealthType == EWealthType::Actor && SpawnTransforms.Num() != 1 && SpawnTransforms.Num() != Amount)
	{
		DDH::Debug() << ObjectName << " Get Error Spawn Count : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源已经加载了
	if (WealthEntry->WealthClass)
	{
		//直接注册到加载栈
		ClassMultiLoadStack.Add(new ClassMultiLoadNode(WealthEntry, Amount, ObjectName, FunName, SpawnTransforms));
	}
	else
	{
		//先进行异步加载, 再注册到加载栈
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPtr.ToSoftObjectPath());
		ClassMultiLoadStack.Add(new ClassMultiLoadNode(WealthHandle, WealthEntry, Amount, ObjectName, FunName, SpawnTransforms));
	}
}

void UDDWealth::BuildKindClassWealth(EWealthType WealthType, FName WealthKind, FName ObjectName, FName FunName, TArray<FTransform> SpawnTransforms)
{
	TArray<FClassWealthEntry*> WealthEntryGroup = GetClassKindPath(WealthKind);
	//如果数量为0, 输出错误并且返回
	if (WealthEntryGroup.Num() == 0)
	{
		DDH::Debug() << ObjectName << " Get Null WealthKind : " << WealthKind << DDH::Endl();
		return;
	}
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		//如果资源不可用, 输出错误
		if (!WealthEntryGroup[i]->WealthPtr.ToSoftObjectPath().IsValid())
		{
			DDH::Debug() << ObjectName << " Get Not Valid in Kind : " << WealthKind << " For Name : " << WealthEntryGroup[i]->WealthName << DDH::Endl();
			return;
		}
		//如果资源类型和WealthType不匹配
		if (WealthEntryGroup[i]->WealthType != WealthType)
		{
			DDH::Debug() << ObjectName << " Get Error Type in Kind : " << WealthKind << " For Name : " << WealthEntryGroup[i]->WealthName << DDH::Endl();
			return;
		}
	}
	//如果SpawnTransform的数量不为1而且不为Amount
	if (WealthType == EWealthType::Actor && SpawnTransforms.Num() != 1 && SpawnTransforms.Num() != WealthEntryGroup.Num())
	{
		DDH::Debug() << ObjectName << " Get Error Spawn Count : " << WealthKind << DDH::Endl();
		return;
	}
	//还没有加载的资源
	TArray<FClassWealthEntry*> UnLoadWealthEntry;
	//已经加载的资源
	TArray<FClassWealthEntry*> LoadWealthEntry;
	//给拿到的资源分类
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (WealthEntryGroup[i]->WealthClass)
		{
			LoadWealthEntry.Add(WealthEntryGroup[i]);
		}
		else
		{
			UnLoadWealthEntry.Add(WealthEntryGroup[i]);
		}
	}
	//先声明一个空的加载句柄共享指针
	TSharedPtr<FStreamableHandle> WealthHandle;
	//如果未加载对象不为0, 进行异步加载
	if (UnLoadWealthEntry.Num() > 0)
	{
		//先获取路径资源
		TArray<FSoftObjectPath> WealthPaths;
		for (int i = 0; i < UnLoadWealthEntry.Num(); ++i)
			WealthPaths.Add(UnLoadWealthEntry[i]->WealthPtr.ToSoftObjectPath());
		//给异步加载句柄赋值
		WealthHandle = WealthLoader.RequestAsyncLoad(WealthPaths);
	}
	//将加载数据推入WealthKind加载栈
	ClassKindLoadStack.Add(new ClassKindLoadNode(WealthHandle, UnLoadWealthEntry, LoadWealthEntry, ObjectName, FunName, SpawnTransforms));
}

void UDDWealth::LoadClassWealth(FName WealthName, FName ObjectName, FName FunName)
{
	//获取对应的资源结构体
	FClassWealthEntry* WealthEntry = GetClassSinglePath(WealthName);
	//如果为空
	if (!WealthEntry)
	{
		DDH::Debug() << ObjectName << " Get Null Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源不可用, 输出错误
	if (!WealthEntry->WealthPtr.ToSoftObjectPath().IsValid())
	{
		DDH::Debug() << ObjectName << " Get Not Valid : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源已经加载进内存, 直接生成新的资源
	if (WealthEntry->WealthClass)
	{
		BackClassWealth(ModuleIndex, ObjectName, FunName, WealthName, WealthEntry->WealthClass);
	}
	else
	{
		//如果资源没有加载, 先去异步加载
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPtr.ToSoftObjectPath());
		//加到加载栈
		ClassSingleLoadStack.Add(new ClassSingleLoadNode(WealthHandle, WealthEntry, ObjectName, FunName));
	}
}

void UDDWealth::LoadClassWealthKind(FName WealthKind, FName ObjectName, FName FunName)
{
	TArray<FClassWealthEntry*> WealthEntryGroup = GetClassKindPath(WealthKind);

	//如果数量为0, 输出错误并且返回
	if (WealthEntryGroup.Num() == 0)
	{
		DDH::Debug() << ObjectName << " Get Null WealthKind : " << WealthKind << DDH::Endl();
		return;
	}
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		//如果资源不可用, 输出错误
		if (!WealthEntryGroup[i]->WealthPtr.ToSoftObjectPath().IsValid())
		{
			DDH::Debug() << ObjectName << " Get Not Valid in Kind : " << WealthKind << " For Name : " << WealthEntryGroup[i]->WealthName << DDH::Endl();
			return;
		}
	}
	//还没有加载的资源
	TArray<FClassWealthEntry*> UnLoadWealthEntry;
	//已经加载的资源
	TArray<FClassWealthEntry*> LoadWealthEntry;
	//给拿到的资源分类
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (WealthEntryGroup[i]->WealthClass)
		{
			LoadWealthEntry.Add(WealthEntryGroup[i]);
		}
		else
		{
			UnLoadWealthEntry.Add(WealthEntryGroup[i]);
		}
	}
	//如果未加载数量为0, 直接返回数据
	if (UnLoadWealthEntry.Num() == 0)
	{
		//直接获取所有的对象返回给请求者
		TArray<FName> NameGroup;
		TArray<UClass*> WealthGroup;
		for (int i = 0; i < LoadWealthEntry.Num(); ++i)
		{
			NameGroup.Add(LoadWealthEntry[i]->WealthName);
			WealthGroup.Add(LoadWealthEntry[i]->WealthClass);
		}
		BackClassWealthKind(ModuleIndex, ObjectName, FunName, NameGroup, WealthGroup);
	}
	else
	{
		//先获取路径资源
		TArray<FSoftObjectPath> WealthPaths;
		for (int i = 0; i < UnLoadWealthEntry.Num(); ++i)
			WealthPaths.Add(UnLoadWealthEntry[i]->WealthPtr.ToSoftObjectPath());
		//给异步加载句柄赋值
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthPaths);
		//将加载数据推入WealthKind加载栈
		ClassKindLoadStack.Add(new ClassKindLoadNode(WealthHandle, UnLoadWealthEntry, LoadWealthEntry, ObjectName, FunName));
	}
}

void UDDWealth::LoadObjectWealth(FName WealthName, FName ObjectName, FName FunName)
{
	//获取资源结构体
	FObjectWealthEntry* WealthEntry = GetObjectSinglePath(WealthName);
	//如果为空
	if (!WealthEntry)
	{
		DDH::Debug() << ObjectName << " Get Null Wealth : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源不可用, 输出错误
	if (!WealthEntry->WealthPath.IsValid())
	{
		DDH::Debug() << ObjectName << " Get Not Valid : " << WealthName << DDH::Endl();
		return;
	}
	//如果资源已经加载
	if (WealthEntry->WealthObject)
	{
		//直接返回给请求对象
		BackObjectWealth(ModuleIndex, ObjectName, FunName, WealthName, WealthEntry->WealthObject);
	}
	else
	{
		//如果没有加载就进行异步加载
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthEntry->WealthPath);
		//添加到加载序列
		ObjectSingleLoadStack.Add(new ObjectSingleLoadNode(WealthHandle, WealthEntry, ObjectName, FunName));
	}
}

void UDDWealth::LoadObjectWealthKind(FName WealthKind, FName ObjectName, FName FunName)
{
	TArray<FObjectWealthEntry*> WealthEntryGroup = GetObjectKindPath(WealthKind);
	//如果数量为0, 输出错误并且返回
	if (WealthEntryGroup.Num() == 0)
	{
		DDH::Debug() << ObjectName << " Get Null WealthKind : " << WealthKind << DDH::Endl();
		return;
	}
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		//如果资源不可用, 输出错误
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
	//给拿到的资源分类
	for (int i = 0; i < WealthEntryGroup.Num(); ++i)
	{
		if (WealthEntryGroup[i]->WealthObject)
		{
			LoadWealthEntry.Add(WealthEntryGroup[i]);
		}
		else
		{
			UnLoadWealthEntry.Add(WealthEntryGroup[i]);
		}
	}
	//如果未加载资源为0
	if (UnLoadWealthEntry.Num() == 0)
	{
		//直接获取所有的对象返回给请求者
		TArray<FName> NameGroup;
		TArray<UObject*> WealthGroup;
		for (int i = 0; i < LoadWealthEntry.Num(); ++i)
		{
			NameGroup.Add(LoadWealthEntry[i]->WealthName);
			WealthGroup.Add(LoadWealthEntry[i]->WealthObject);
		}
		BackObjectWealthKind(ModuleIndex, ObjectName, FunName, NameGroup, WealthGroup);
	}
	else
	{
		//先获取路径资源
		TArray<FSoftObjectPath> WealthPaths;
		for (int i = 0; i < UnLoadWealthEntry.Num(); ++i)
			WealthPaths.Add(UnLoadWealthEntry[i]->WealthPath);
		//给异步加载句柄赋值
		TSharedPtr<FStreamableHandle> WealthHandle = WealthLoader.RequestAsyncLoad(WealthPaths);
		//注册加载节点到加载栈
		ObjectKindLoadStack.Add(new ObjectKindLoadNode(WealthHandle, UnLoadWealthEntry, LoadWealthEntry, ObjectName, FunName));
	}
}

FWealthURL* UDDWealth::GainWealthURL(FName WealthName)
{
	for (int i = 0; i < WealthData.Num(); ++i)
		for (int j = 0; j < WealthData[i]->WealthURL.Num(); ++j)
			if (WealthData[i]->WealthURL[j].WealthName.IsEqual(WealthName) && WealthData[i]->WealthURL[j].WealthPath.IsValid())
				return &WealthData[i]->WealthURL[j];
	return NULL;
}

void UDDWealth::GainWealthURL(FName WealthKind, TArray<FWealthURL*>& OutURL)
{
	for (int i = 0; i < WealthData.Num(); ++i)
		for (int j = 0; j < WealthData[i]->WealthURL.Num(); ++j)
			if (WealthData[i]->WealthURL[j].WealthKind.IsEqual(WealthKind) && WealthData[i]->WealthURL[j].WealthPath.IsValid())
				OutURL.Add(&WealthData[i]->WealthURL[j]);
}

void UDDWealth::WealthTick(float DeltaSeconds)
{
	//处理ClassSingleLoadStack
	DealClassSingleLoadStack();
	//处理ClassMultiLoadStack
	DealClassMultiLoadStack();
	//处理ClassKindLoadStack
	DealClassKindLoadStack();
	//处理单独加载Object资源队列
	DealObjectSingleLoadStack();
	//处理Object资源种类加载队列
	DealObjectKindLoadStack();
}

void UDDWealth::DealClassSingleLoadStack()
{
	//定义加载完成的序列
	TArray<ClassSingleLoadNode*> CompleteStack;
	for (int i = 0; i < ClassSingleLoadStack.Num(); ++i)
	{
		//遍历判断是否已经加载完毕
		if (ClassSingleLoadStack[i]->WealthHandle->HasLoadCompleted())
		{
			//设置对应资源已经加载完成
			ClassSingleLoadStack[i]->WealthEntry->WealthClass = Cast<UClass>(ClassSingleLoadStack[i]->WealthHandle->GetLoadedAsset());
			//如果是加载Class
			if (ClassSingleLoadStack[i]->LoadClass)
			{
				//直接返回Class给请求者
				BackClassWealth(ModuleIndex, ClassSingleLoadStack[i]->ObjectName, ClassSingleLoadStack[i]->FunName, ClassSingleLoadStack[i]->WealthEntry->WealthName, ClassSingleLoadStack[i]->WealthEntry->WealthClass);
			}
			else
			{
				//根据资源类型生成对应资源
				if (ClassSingleLoadStack[i]->WealthEntry->WealthType == EWealthType::Object)
				{
					UObject * InstObject = NewObject<UObject>(this, ClassSingleLoadStack[i]->WealthEntry->WealthClass);
					InstObject->AddToRoot();
					//给请求对象传递资源
					BackObject(ModuleIndex, ClassSingleLoadStack[i]->ObjectName, ClassSingleLoadStack[i]->FunName, ClassSingleLoadStack[i]->WealthEntry->WealthName, InstObject);
				}
				else if (ClassSingleLoadStack[i]->WealthEntry->WealthType == EWealthType::Actor)
				{
					AActor * InstActor = GetDDWorld()->SpawnActor<AActor>(ClassSingleLoadStack[i]->WealthEntry->WealthClass, ClassSingleLoadStack[i]->SpawnTransform);
					//给请求的对象传递资源
					BackActor(ModuleIndex, ClassSingleLoadStack[i]->ObjectName, ClassSingleLoadStack[i]->FunName, ClassSingleLoadStack[i]->WealthEntry->WealthName, InstActor);
				}
				else if (ClassSingleLoadStack[i]->WealthEntry->WealthType == EWealthType::Widget)
				{
					UUserWidget * InstWidget = CreateWidget<UUserWidget>(GetDDWorld(), ClassSingleLoadStack[i]->WealthEntry->WealthClass);
					//避免回收
					GCWidgetGroup.Push(InstWidget);
					//给请求的对象传递资源
					BackWidget(ModuleIndex, ClassSingleLoadStack[i]->ObjectName, ClassSingleLoadStack[i]->FunName, ClassSingleLoadStack[i]->WealthEntry->WealthName, InstWidget);
				}
			}
			//添加到已经加载完成的序列
			CompleteStack.Add(ClassSingleLoadStack[i]);
		}
	}
	//遍历已经加载完成的序列
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		//将加载完成的序列从栈中移除
		ClassSingleLoadStack.Remove(CompleteStack[i]);
		//释放资源
		delete CompleteStack[i];
	}
}

void UDDWealth::DealClassMultiLoadStack()
{
	//定义加载完成的序列
	TArray<ClassMultiLoadNode*> CompleteStack;
	for (int i = 0; i < ClassMultiLoadStack.Num(); ++i)
	{
		//如果对应资源没有加载, 说明这个Node的WealthHandle有效
		if (!ClassMultiLoadStack[i]->WealthEntry->WealthClass)
		{
			//通过判断WealthHandle有没有加载完成设置WealthClass值
			if (ClassMultiLoadStack[i]->WealthHandle->HasLoadCompleted())
				ClassMultiLoadStack[i]->WealthEntry->WealthClass = Cast<UClass>(ClassMultiLoadStack[i]->WealthHandle->GetLoadedAsset());
		}
		//再次判断WealthClass来和已经生成的数量来决定是否添加到完成序列
		if (ClassMultiLoadStack[i]->WealthEntry->WealthClass)
		{
			//区分类型
			if (ClassMultiLoadStack[i]->WealthEntry->WealthType == EWealthType::Object)
			{
				UObject * InstObject = NewObject<UObject>(this, CompleteStack[i]->WealthEntry->WealthClass);
				InstObject->AddToRoot();
				ClassMultiLoadStack[i]->ObjectGroup.Add(InstObject);
				//如果数量已经足够
				if (ClassMultiLoadStack[i]->ObjectGroup.Num() >= ClassMultiLoadStack[i]->Amount)
				{
					//给请求的对象传递资源
					BackObjects(ModuleIndex, ClassMultiLoadStack[i]->ObjectName, ClassMultiLoadStack[i]->FunName, ClassMultiLoadStack[i]->WealthEntry->WealthName, ClassMultiLoadStack[i]->ObjectGroup);
					//添加到完成序列
					CompleteStack.Add(ClassMultiLoadStack[i]);
				}
			}
			else if (ClassMultiLoadStack[i]->WealthEntry->WealthType == EWealthType::Actor)
			{
				//获取生成位置
				FTransform SpawnTransform = ClassMultiLoadStack[i]->SpawnTransforms.Num() == 1 ? ClassMultiLoadStack[i]->SpawnTransforms[0] : ClassMultiLoadStack[i]->SpawnTransforms[ClassMultiLoadStack[i]->ActorGroup.Num()];
				//生成对象
				AActor * InstActor = GetDDWorld()->SpawnActor<AActor>(ClassMultiLoadStack[i]->WealthEntry->WealthClass, SpawnTransform);
				ClassMultiLoadStack[i]->ActorGroup.Add(InstActor);
				//如果数量已经足够
				if (ClassMultiLoadStack[i]->ActorGroup.Num() >= ClassMultiLoadStack[i]->Amount)
				{
					//给请求的对象传递资源
					BackActors(ModuleIndex, ClassMultiLoadStack[i]->ObjectName, ClassMultiLoadStack[i]->FunName, ClassMultiLoadStack[i]->WealthEntry->WealthName, ClassMultiLoadStack[i]->ActorGroup);
					//添加到完成序列
					CompleteStack.Add(ClassMultiLoadStack[i]);
				}
			}
			else if (ClassMultiLoadStack[i]->WealthEntry->WealthType == EWealthType::Widget)
			{
				UUserWidget * InstWidget = CreateWidget<UUserWidget>(GetDDWorld(), ClassMultiLoadStack[i]->WealthEntry->WealthClass);
				//避免回收
				GCWidgetGroup.Push(InstWidget);
				ClassMultiLoadStack[i]->WidgetGroup.Add(InstWidget);
				//如果数量已经足够
				if (ClassMultiLoadStack[i]->WidgetGroup.Num() >= ClassMultiLoadStack[i]->Amount)
				{
					//给请求的对象传递资源
					BackWidgets(ModuleIndex, ClassMultiLoadStack[i]->ObjectName, ClassMultiLoadStack[i]->FunName, ClassMultiLoadStack[i]->WealthEntry->WealthName, ClassMultiLoadStack[i]->WidgetGroup);
					//添加到完成序列
					CompleteStack.Add(ClassMultiLoadStack[i]);
				}
			}
		}
	}
	//遍历已经加载完成的序列
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		//将加载完成的序列从栈中移除
		ClassMultiLoadStack.Remove(CompleteStack[i]);
		//释放资源
		delete CompleteStack[i];
	}
}

void UDDWealth::DealClassKindLoadStack()
{
	//定义加载完成的序列
	TArray<ClassKindLoadNode*> CompleteStack;
	for (int i = 0; i < ClassKindLoadStack.Num(); ++i)
	{
		//如果WealthHandle有效并且加载完成而且未加载序列有数据, 说明有异步加载并且是第一次加载完成
		if (ClassKindLoadStack[i]->WealthHandle.IsValid() && ClassKindLoadStack[i]->WealthHandle->HasLoadCompleted() && ClassKindLoadStack[i]->UnLoadWealthEntry.Num() > 0)
		{
			//如果已经加载完成, 把未加载序列设置成已经加载
			for (int j = 0; j < ClassKindLoadStack[i]->UnLoadWealthEntry.Num(); ++j)
			{
				ClassKindLoadStack[i]->UnLoadWealthEntry[j]->WealthClass = Cast<UClass>(ClassKindLoadStack[i]->UnLoadWealthEntry[j]->WealthPtr.ToSoftObjectPath().ResolveObject());
			}
			//将未加载序列转移到已加载序列
			ClassKindLoadStack[i]->LoadWealthEntry.Append(ClassKindLoadStack[i]->UnLoadWealthEntry);
			//清空未加载序列
			ClassKindLoadStack[i]->UnLoadWealthEntry.Empty();
		}

		//如果未加载序列已经为0, 说明已经在生成资源阶段
		if (ClassKindLoadStack[i]->UnLoadWealthEntry.Num() == 0)
		{
			//如果只是加载Class
			if (ClassKindLoadStack[i]->LoadClass)
			{
				//直接传送数据
				TArray<FName> NameGroup;
				TArray<UClass*> WealthGroup;
				//遍历已经加载的序列填充数组
				for (int j = 0; j < ClassKindLoadStack[i]->LoadWealthEntry.Num(); ++j)
				{
					NameGroup.Add(ClassKindLoadStack[i]->LoadWealthEntry[j]->WealthName);
					WealthGroup.Add(ClassKindLoadStack[i]->LoadWealthEntry[j]->WealthClass);
				}
				//返回数据给请求对象
				BackClassWealthKind(ModuleIndex, ClassKindLoadStack[i]->ObjectName, ClassKindLoadStack[i]->FunName, NameGroup, WealthGroup);
				//添加到已经加载完成的序列
				CompleteStack.Add(ClassKindLoadStack[i]);
			}
			else //如果要生成资源
			{
				//从已加载序列提取出第一个
				FClassWealthEntry* WealthEntry = ClassKindLoadStack[i]->LoadWealthEntry[0];
				//将第一个从已加载序列中移除
				ClassKindLoadStack[i]->LoadWealthEntry.RemoveAt(0);
				//根据提取出的资源类型生成对应资源并且添加到生产序列
				if (WealthEntry->WealthType == EWealthType::Object)
				{
					UObject * InstObject = NewObject<UObject>(this, WealthEntry->WealthClass);
					InstObject->AddToRoot();
					//添加到序列
					ClassKindLoadStack[i]->NameGroup.Add(WealthEntry->WealthName);
					ClassKindLoadStack[i]->ObjectGroup.Add(InstObject);
					//如果已加载序列容量已经为0, 说明已经加载完毕
					if (ClassKindLoadStack[i]->LoadWealthEntry.Num() == 0)
					{
						//给请求的对象传递资源
						BackObjectKind(ModuleIndex, ClassKindLoadStack[i]->ObjectName, ClassKindLoadStack[i]->FunName, ClassKindLoadStack[i]->NameGroup, ClassKindLoadStack[i]->ObjectGroup);
						//添加到完成序列
						CompleteStack.Add(ClassKindLoadStack[i]);
					}
				}
				else if (WealthEntry->WealthType == EWealthType::Actor)
				{
					//获取生成位置
					FTransform SpawnTransform = ClassKindLoadStack[i]->SpawnTransforms.Num() == 1 ? ClassKindLoadStack[i]->SpawnTransforms[0] : ClassKindLoadStack[i]->SpawnTransforms[ClassKindLoadStack[i]->ActorGroup.Num()];
					//生成对象
					AActor * InstActor = GetDDWorld()->SpawnActor<AActor>(WealthEntry->WealthClass, SpawnTransform);
					//添加到序列
					ClassKindLoadStack[i]->NameGroup.Add(WealthEntry->WealthName);
					ClassKindLoadStack[i]->ActorGroup.Add(InstActor);
					//如果已加载序列容量已经为0, 说明已经加载完毕
					if (ClassKindLoadStack[i]->LoadWealthEntry.Num() == 0)
					{
						//给请求的对象传递资源
						BackActorKind(ModuleIndex, ClassKindLoadStack[i]->ObjectName, ClassKindLoadStack[i]->FunName, ClassKindLoadStack[i]->NameGroup, ClassKindLoadStack[i]->ActorGroup);
						//添加到完成序列
						CompleteStack.Add(ClassKindLoadStack[i]);
					}
				}
				else if (WealthEntry->WealthType == EWealthType::Widget)
				{
					UUserWidget * InstWidget = CreateWidget<UUserWidget>(GetDDWorld(), WealthEntry->WealthClass);
					//避免回收
					GCWidgetGroup.Push(InstWidget);
					//添加到序列
					ClassKindLoadStack[i]->NameGroup.Add(WealthEntry->WealthName);
					ClassKindLoadStack[i]->WidgetGroup.Add(InstWidget);
					//如果已加载序列容量已经为0, 说明已经加载完毕
					if (ClassKindLoadStack[i]->LoadWealthEntry.Num() == 0)
					{
						//给请求的对象传递资源
						BackWidgetKind(ModuleIndex, ClassKindLoadStack[i]->ObjectName, ClassKindLoadStack[i]->FunName, ClassKindLoadStack[i]->NameGroup, ClassKindLoadStack[i]->WidgetGroup);
						//添加到完成序列
						CompleteStack.Add(ClassKindLoadStack[i]);
					}
				}
			}
		}
	}
	//遍历已经加载完成的序列
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		//将加载完成的序列从栈中移除
		ClassKindLoadStack.Remove(CompleteStack[i]);
		//释放资源
		delete CompleteStack[i];
	}
}

void UDDWealth::DealObjectSingleLoadStack()
{
	//定义加载完成的序列
	TArray<ObjectSingleLoadNode*> CompleteStack;
	for (int i = 0; i < ObjectSingleLoadStack.Num(); ++i)
	{
		//遍历判断是否已经加载完毕
		if (ObjectSingleLoadStack[i]->WealthHandle->HasLoadCompleted())
		{
			//设置对应资源已经加载完成
			ObjectSingleLoadStack[i]->WealthEntry->WealthObject = ObjectSingleLoadStack[i]->WealthEntry->WealthPath.ResolveObject();
			BackObjectWealth(ModuleIndex, ObjectSingleLoadStack[i]->ObjectName, ObjectSingleLoadStack[i]->FunName, ObjectSingleLoadStack[i]->WealthEntry->WealthName, ObjectSingleLoadStack[i]->WealthEntry->WealthObject);
			//添加到已经加载完成的序列
			CompleteStack.Add(ObjectSingleLoadStack[i]);
		}
	}
	//遍历已经加载完成的序列
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		//将加载完成的序列从栈中移除
		ObjectSingleLoadStack.Remove(CompleteStack[i]);
		//释放资源
		delete CompleteStack[i];
	}
}

void UDDWealth::DealObjectKindLoadStack()
{
	//定义加载完成的序列
	TArray<ObjectKindLoadNode*> CompleteStack;
	for (int i = 0; i < ObjectKindLoadStack.Num(); ++i)
	{
		//如果已经加载完成
		if (ObjectKindLoadStack[i]->WealthHandle->HasLoadCompleted())
		{
			//遍历设置未加载组为加载状态
			for (int j = 0; j < ObjectKindLoadStack[i]->UnLoadWealthEntry.Num(); ++j)
			{
				ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthObject = ObjectKindLoadStack[i]->UnLoadWealthEntry[j]->WealthPath.ResolveObject();
			}
			//将未加载序列转移到已加载序列
			ObjectKindLoadStack[i]->LoadWealthEntry.Append(ObjectKindLoadStack[i]->UnLoadWealthEntry);
			//清空未加载序列
			ObjectKindLoadStack[i]->UnLoadWealthEntry.Empty();
			//返回的资源名和资源数组
			TArray<FName> NameGroup;
			TArray<UObject*> WealthGroup;
			//遍历已经加载的序列填充数组
			for (int j = 0; j < ObjectKindLoadStack[i]->LoadWealthEntry.Num(); ++j)
			{
				NameGroup.Add(ObjectKindLoadStack[i]->LoadWealthEntry[j]->WealthName);
				WealthGroup.Add(ObjectKindLoadStack[i]->LoadWealthEntry[j]->WealthObject);
			}
			//返回数据给请求对象
			BackObjectWealthKind(ModuleIndex, ObjectKindLoadStack[i]->ObjectName, ObjectKindLoadStack[i]->FunName, NameGroup, WealthGroup);
			//添加到已经加载完成的序列
			CompleteStack.Add(ObjectKindLoadStack[i]);
		}
	}
	//遍历已经加载完成的序列
	for (int i = 0; i < CompleteStack.Num(); ++i)
	{
		//将加载完成的序列从栈中移除
		ObjectKindLoadStack.Remove(CompleteStack[i]);
		//释放资源
		delete CompleteStack[i];
	}
}
