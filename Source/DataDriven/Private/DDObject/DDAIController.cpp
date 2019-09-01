// Fill out your copyright notice in the Description page of Project Settings.


#include "DDAIController.h"

ADDAIController::ADDAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDAIController::BeginPlay()
{
	Super::BeginPlay();

	//×Ô¶¯×¢²á
	RegisterToModule(ModuleName, ObjectName, ClassName);
}
