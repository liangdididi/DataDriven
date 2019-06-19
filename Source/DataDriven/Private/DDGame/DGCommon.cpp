// Fill out your copyright notice in the Description page of Project Settings.

#include "DGCommon.h"


UDGCommon* UDGCommon::DGInst = NULL;


UDGCommon* UDGCommon::Get()
{
	if (!DGInst)
	{
		DGInst = NewObject<UDGCommon>();
		DGInst->AddToRoot();
	}
	return DGInst;
}

void UDGCommon::SetPlayerPos(FVector InPos)
{
	PlayerPos = InPos;
}

void UDGCommon::SetCameraPos(FVector InPos)
{
	CameraPos = InPos;
}

const FVector UDGCommon::GainPlayerPos() const
{
	return PlayerPos;
}

const FVector UDGCommon::GainCameraPos() const
{
	return CameraPos;
}

