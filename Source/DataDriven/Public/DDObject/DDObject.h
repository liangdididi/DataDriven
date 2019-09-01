// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDOO.h"
#include "DDObject.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDObject : public UObject , public IDDOO
{
	GENERATED_BODY()

public:
	
	//ÖØÐ´ÊÍ·Åº¯Êý
	virtual void DDRelease() override;

};

