// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGameStateBase.h"

ADDGameStateBase::ADDGameStateBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	//×Ô¶¯×¢²á
	RegisterToModule(ModuleName, ObjectName, ClassName);
}