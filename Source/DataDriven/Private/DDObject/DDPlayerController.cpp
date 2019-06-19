// Fill out your copyright notice in the Description page of Project Settings.

#include "DDPlayerController.h"



ADDPlayerController::ADDPlayerController()
{
	//必须开启Controller或者Character的帧函数才能够检测按键, 本框架只开启Controller的
	PrimaryActorTick.bCanEverTick = true;
}

void ADDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//自动注册
	RegisterToModule(ModuleName, ObjectName, ClassName);
}
