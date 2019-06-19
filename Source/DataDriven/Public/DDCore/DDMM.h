// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DDCommon/DDCommon.h"
#include "DDMM.generated.h"


class ADDDriver;
class UDDModule;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDDMM : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DATADRIVEN_API IDDMM
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//指定模组
	virtual void AssignModule(UDDModule* Mod);
	
	//发布方法
	void ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param);
	//发布方法
	void ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param);

	//获取世界
	UWorld* GetDDWorld() const;

protected:

	//保存对应的模组
	UDDModule* IModule;

	//保存GameMode
	ADDDriver* IDriver;

	//保存对应模组的Index
	int32 ModuleIndex;

};
