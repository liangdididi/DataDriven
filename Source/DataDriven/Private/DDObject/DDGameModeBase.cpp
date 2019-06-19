// Fill out your copyright notice in the Description page of Project Settings.

#include "DDGameModeBase.h"




ADDGameModeBase::ADDGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//×Ô¶¯×¢²á
	RegisterToModule(ModuleName, ObjectName, ClassName);
}
