// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDOO.h"
#include "DDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDUserWidget : public UUserWidget, public IDDOO
{
	GENERATED_BODY()

public:

	//ÖØÐ´ÊÍ·Åº¯Êý
	virtual void DDRelease() override;

	
};
