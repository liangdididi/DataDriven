// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DGTypes.h"
#include "DGCommon.generated.h"

namespace DGHelper
{

}


/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDGCommon : public UObject
{
	GENERATED_BODY()
	
public:

	static UDGCommon* Get();
	
private:

	static UDGCommon* DGInst;
	
};


