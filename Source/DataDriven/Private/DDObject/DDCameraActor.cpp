// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCameraActor.h"

ADDCameraActor::ADDCameraActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDCameraActor::BeginPlay()
{
	Super::BeginPlay();

	//自动注册
	RegisterToModule(ModuleName, ObjectName, ClassName);
}

void ADDCameraActor::DDRelease()
{
	IDDOO::DDRelease();
	//能调用这个方法那么一定是注册到了框架,获取的世界一定不为空
	GetDDWorld()->DestroyActor(this);
}


