// Fill out your copyright notice in the Description page of Project Settings.


#include "DDTypes.h"

TSharedPtr<DDRecord> DDRecord::RecordInst = NULL;

TSharedPtr<DDRecord> DDRecord::Get()
{
	if (!RecordInst.IsValid())
		RecordInst = MakeShareable(new DDRecord());
	return RecordInst;
}
