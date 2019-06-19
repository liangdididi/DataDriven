// Fill out your copyright notice in the Description page of Project Settings.

#include "DDCommon.h"

UDDCommon* UDDCommon::DDInst = NULL;

UDDCommon* UDDCommon::Get()
{
	if (!DDInst) {
		DDInst = NewObject<UDDCommon>();
		DDInst->AddToRoot();
	}
	return DDInst;
}

void UDDCommon::InitCommon(ADDDriver* InDriver)
{
	Driver = InDriver;
}

void UDDCommon::InitController(APlayerController* InController)
{
	PlayerController = InController;
}

void UDDCommon::SetPauseGame(bool IsPause)
{
	PlayerController->SetPause(IsPause);
}

const bool UDDCommon::IsPauseGame() const
{
	return PlayerController->IsPaused();
}

ADDDriver* UDDCommon::GetDriver()
{
	return Driver;
}

APlayerController* UDDCommon::GetController()
{
	return PlayerController;
}




