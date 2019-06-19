// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDOO.h"
#include "DDActor.generated.h"


UCLASS()
class DATADRIVEN_API ADDActor : public AActor, public IDDOO
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADDActor();

	//重写释放函数
	virtual void DDRelease() override;

public:

	//模组名字,如果为空,说明要手动指定,不为空就是自动指定
	UPROPERTY(EditAnywhere, Category = "DataDriven")
		FName ModuleName;
	//对象名字,如果为空,说明要手动指定,不为空就是自动指定
	UPROPERTY(EditAnywhere, Category = "DataDriven")
		FName ObjectName;
	//类名字,如果为空,说明要手动指定,不为空就是自动指定
	UPROPERTY(EditAnywhere, Category = "DataDriven")
		FName ClassName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};





//struct DesObjParam : DDParam
//{
//	struct
//	{
//		EAgreementType Agree;
//		TArray<FString> ObjectNameGroup;
//	} Parameter;
//	EAgreementType Agree() { return Parameter.Agree; }
//	TArray<FString> ObjectNameGroup() { return Parameter.ObjectNameGroup; }
//	DesObjParam() { ParamPtr = &Parameter; }
//};
//DesObjParam* DesObj(int32 ModuleIndex, FString FunctionName, EAgreementType Agree, TArray<FString> ObjectNameGroup)
//{
//	DDModuleAgreement Agreement;
//	Agreement.ModuleIndex = ModuleIndex;
//	Agreement.FunctionName;
//
//	DesObjParam* Param = new DesObjParam();
//	Param->Parameter.Agree = Agree;
//	Param->Parameter.ObjectNameGroup = ObjectNameGroup;
//	ExecuteFunction(Agreement, Param);
//
//	return Param;
//}