// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "DDOO.h"
#include "DDWheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class DATADRIVEN_API ADDWheeledVehicle : public AWheeledVehicle, public IDDOO
{
	GENERATED_BODY()
	
public:

	ADDWheeledVehicle();

	//重写释放资源函数
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
