// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDMM.h"
#include "DDModel.generated.h"


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

	virtual void ModelTick(float DeltaSeconds);

	//注册对象到数据模块
	void RegisterObject(IDDOO* ObjectInst);

	//销毁对象, 销毁对象本体调用
	void DestroyObject(FName ObjectName);

	//销毁对象
	void DestroyObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);

	//激活对象
	void EnableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);

	//失活对象
	void DisableObject(EAgreementType Agreement, TArray<FName> TargetNameGroup);

	//根据传入的对象名获取对象
	void GetSelfObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);

	//根据传入的对象名获取这些对象名对应对象外的其他对象
	int32 GetOtherObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);

	//根据名字数组获取相同类的其他对象,返回这个类的对象的数量
	int32 GetClassOtherObject(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);

	//根据名字获取类的对象
	void GetSelfClass(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);

	//根据名字获取类以外的类的对象
	void GetOtherClass(TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);

	//获取所有对象
	void GetAll(TArray<IDDOO*>& TargetObjectGroup);

	//根据协议获取对象集合
	void GetAgreementObject(EAgreementType Agreement, TArray<FName> TargetNameGroup, TArray<IDDOO*>& TargetObjectGroup);

protected:

	//框架对象数组, Key : ObjectName,  Value : *
	TMap<FName, IDDOO*> ObjectGroup;

	//框架对象类集合, Key : ClassName , Value : TArray<IDDOO*>
	TMap<FName, TArray<IDDOO*>> ObjectClassGroup;

	//激活生命周期的对象集
	TArray<IDDOO*> ObjectActiveGroup;

	//运行需要运行Tick函数的对象集合
	TArray<IDDOO*> ObjectTickGroup;

	//销毁生命周期的对象集
	TArray<IDDOO*> ObjectDestroyGroup;
	
	//释放周期函数对象集合, 只执行一次就全部释放
	TArray<IDDOO*> ObjectReleaseGroup;

	//预销毁对象组, 这些对象还处于激活生命周期, 但是在激活生命周期没运行完之前就已经设定为要进行销毁
	TArray<IDDOO*> PreObjectDestroyGroup;
};
