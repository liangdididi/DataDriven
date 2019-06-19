// Fill out your copyright notice in the Description page of Project Settings.

#include "DDHUD.h"




ADDHUD::ADDHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADDHUD::BeginPlay()
{
	Super::BeginPlay();

	//×Ô¶¯×¢²á
	RegisterToModule(ModuleName, ObjectName, ClassName);
}
