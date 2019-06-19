// Fill out your copyright notice in the Description page of Project Settings.

#include "DDMM.h"
#include "DDDriver.h"
#include "DDModule.h"


void IDDMM::AssignModule(UDDModule* Mod)
{
	IModule = Mod;
	//模组的ModuleIndex统一在创建模组组件前指定, 所以这里直接赋值没有问题
	ModuleIndex = IModule->ModuleIndex;
	//获取Driver
	IDriver = UDDCommon::Get()->GetDriver();
}

// Add default functionality here for any IDDMM functions that are not pure virtual.


void IDDMM::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex) {
		IModule->ExecuteFunction(Agreement, Param);
	}
	else {
		IDriver->ExecuteFunction(Agreement, Param);
	}
}

void IDDMM::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex) {
		IModule->ExecuteFunction(Agreement, Param);
	}
	else {
		IDriver->ExecuteFunction(Agreement, Param);
	}
}

UWorld* IDDMM::GetDDWorld() const
{
	if (IDriver) return IDriver->GetWorld();
	return NULL;
}
