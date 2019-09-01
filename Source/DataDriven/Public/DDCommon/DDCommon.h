// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDCommon/DDDefine.h"
#include "DDGame/DGCommon.h"
#include "DDCommon.generated.h"

class ADDDriver;
class APlayerController;

namespace DDH
{
	FORCEINLINE DDRecord& Debug(float InTime = 3000.f, FColor InColor = FColor::Yellow)
	{
		DDRecord::Get()->PatternID = 0;
		DDRecord::Get()->InitParam(InTime, InColor);
		return *DDRecord::Get();
	}

	FORCEINLINE DDRecord& Debug(FColor InColor)
	{
		return Debug(3000.f, InColor);
	}

	FORCEINLINE DDRecord& Log()
	{
		DDRecord::Get()->PatternID = 1;
		return *DDRecord::Get();
	}

	FORCEINLINE DDRecord& Warning()
	{
		DDRecord::Get()->PatternID = 2;
		return *DDRecord::Get();
	}

	FORCEINLINE DDRecord& Error()
	{
		DDRecord::Get()->PatternID = 3;
		return *DDRecord::Get();
	}


	FORCEINLINE DDRecord& Endl()
	{
		return *DDRecord::Get();
	}

	//将传入的Enum值对应的FString输出, 直接输出Value对应的值
	template<typename TEnum>
	FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!EnumPtr)
			return FString("InValid");
		return EnumPtr->GetEnumName((int32)Value);
	}

	//将传入的Enum值对应的FName输出
	template<typename TEnum>
	FORCEINLINE FName GetEnumValueAsName(const FString& Name, TEnum Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!EnumPtr) {
			return FName("InValid");
		}
		return FName(*EnumPtr->GetEnumName((int32)Value));
	}

	//将传入的FName对应的Enum输出
	template<typename TEnum>
	FORCEINLINE TEnum GetEnumValueFromName(const FString& Name, FName Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!EnumPtr) {
			return TEnum(0);
		}
		return (TEnum)EnumPtr->GetIndexByName(Value);
	}

	//将传入的FName对应的Enum的序号输出
	FORCEINLINE int32 GetEnumIndexFromName(const FString& Name, FName Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>((UObject*)ANY_PACKAGE, *Name, true);
		if (!EnumPtr) {
			return -1;
		}
		return EnumPtr->GetIndexByName(Value);
	}

	//获取方射调用结果
	FORCEINLINE FString GetCallResult(ECallResult Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>((UObject*)ANY_PACKAGE, *FString("ECallResult"), true);
		return EnumPtr->GetEnumName((int32)Value);
	}
}


/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDDCommon : public UObject
{
	GENERATED_BODY()

public:

	static UDDCommon* Get();

	void InitDriver(ADDDriver* InDriver);

	ADDDriver* GetDriver();

	void InitController(APlayerController* InController);

	APlayerController* GetController();

	//暂停游戏
	void SetPauseGame(bool IsPause);

	//获取是否暂停了游戏
	const bool IsPauseGame() const;

private:

	static UDDCommon* DDInst;

	ADDDriver* Driver;

	APlayerController* PlayerController;


	
};
