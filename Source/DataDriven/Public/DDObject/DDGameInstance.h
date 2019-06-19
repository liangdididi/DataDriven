// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DDOO.h"
#include "DDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDGameInstance : public UGameInstance, public IDDOO
{
	GENERATED_BODY()
	
public:

	//GameInstance不允许销毁不允许重写销毁周期函数,重写生命周期函数的话要慎重考虑,避免在转换场景时重复调用
	//virtual void DDRelease() override;
	
	
};
