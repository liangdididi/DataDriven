// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDMM.h"
#include "DDModel.generated.h"

class UDDModule;
class IDDOO;

/**
 *
 */
UCLASS()
class DATADRIVEN_API UDDModel : public UObject, public IDDMM
{
	GENERATED_BODY()

public:

	virtual void ModelInit() {}

	virtual void ModelBeginPlay() {}

	////注册父类模组
	//void RegisterSuperModule(UDDModule* SuperMod);
	////注册单个子类模组
	//void RegisterChildModule(UDDModule* ChildMod);
	//注册对象
	void RegisterObject(IDDOO* ObjectInst);
	//数据的Tick函数
	virtual void ModelTick(float DeltaSeconds);
	//根据名字获取对象
	void GetSelfObject(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	//根据名字获取其他对象,返回全部对象的数量
	int32 GetOtherObject(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	//根据名字数组获取相同类的其他对象,返回这个类的对象的数量
	int32 GetClassOtherObject(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	//根据名字获取类的对象
	void GetSelfClass(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	//根据名字获取类以外的类的对象
	void GetOtherClass(TArray<FName> ObjectNameGroup, TArray<IDDOO*>& TargetObjectGroup);
	//获取所有对象
	void GetAll(TArray<IDDOO*>& TargetObjectGroup);
	
	//销毁对象
	void DestroyObject(EAgreementType Agreement, TArray<FName> ObjectNameGroup);
	//激活对象
	void EnableObject(EAgreementType Agreement, TArray<FName> ObjectNameGroup);
	//失活对象
	void DisableObject(EAgreementType Agreement, TArray<FName> ObjectNameGroup);

	//返回全部对象的数量
	int32 GetObjectGroupNum() const;
	//返回某个类的对象的数量
	int32 GetClassObjectGroupNum(FName ObjectClassName) const;

protected:

	//根据名字删除对象
	void RemoveObject(FName ObjectName);
	//根据类名删除类集合
	void RemoveObjectClass(FName ObjectClassName);

protected:

	////父级模组
	//UDDModule* SuperModule;
	////子集模组
	//TMap<int32, UDDModule*> ChildModule;
	//模组对象
	TMap<FName, IDDOO*> ObjectGroup;
	//模组类对象集
	TMap<FName, TArray<IDDOO*>> ObjectClassGroup;

	//等待生命周期的对象集
	TArray<IDDOO*> ObjectLifeGroup;
	//运行Tick函数的对象集
	TArray<IDDOO*> ObjectTickGroup;

	//销毁生命周期的对象集
	TArray<IDDOO*> ObjectDestroyGroup;
	//释放周期函数,只执行一次就全部释放
	TArray<IDDOO*> ObjectReleaseGroup;
	//临时销毁生命周期的对象集,确保已经激活才销毁
	TArray<IDDOO*> PreObjectDestroyGroup;

};
