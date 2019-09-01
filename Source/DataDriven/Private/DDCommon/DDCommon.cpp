// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCommon.h"

UDDCommon* UDDCommon::DDInst = NULL;

UDDCommon* UDDCommon::Get()
{
	if (!DDInst)
	{
		DDInst = NewObject<UDDCommon>();
		DDInst->AddToRoot();
	}
	return DDInst;
}

void UDDCommon::InitDriver(ADDDriver* InDriver)
{
	Driver = InDriver;
}

ADDDriver* UDDCommon::GetDriver()
{
	return Driver;
}

void UDDCommon::InitController(APlayerController* InController)
{
	PlayerController = InController;
}

APlayerController* UDDCommon::GetController()
{
	return PlayerController;
}

void UDDCommon::SetPauseGame(bool IsPause)
{
	PlayerController->SetPause(IsPause);
}

const bool UDDCommon::IsPauseGame() const
{
	return PlayerController->IsPaused();
}

