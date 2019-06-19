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

	//重写释放函数
	virtual void DDRelease() override;


};



//UserWidget的构造函数重写方式
//UDDUserWidget(const FObjectInitializer& ObjectInitializer);

//UDDUserWidget::UDDUserWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
//{
//
//}


////模组名字,如果为空,说明要手动指定,不为空就是自动指定
//UPROPERTY(EditAnywhere, Category = "DataDriven")
//FName ModuleName;
////对象名字,如果为空,说明要手动指定,不为空就是自动指定
//UPROPERTY(EditAnywhere, Category = "DataDriven")
//FName ObjectName;
////类名字,如果为空,说明要手动指定,不为空就是自动指定
//UPROPERTY(EditAnywhere, Category = "DataDriven")
//FName ClassName;