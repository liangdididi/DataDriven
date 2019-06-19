// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDCommon/DDDefine.h"
#include "DDGame/DGCommon.h"
#include "DDCommon.generated.h"

class ADDDriver;
class APlayerController;

namespace DDH {

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
	//将传入的Enum值对应的FString输出, 直接输出Value对应的值
	template<typename TEnum>
	FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!EnumPtr) {
			return FString("InValid");
		}
		return EnumPtr->GetEnumName((int32)Value);
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
	//获取调用结果函数
	FORCEINLINE FString GetCallResult(ECallResult Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>((UObject*)ANY_PACKAGE, *FString("ECallResult"), true);
		return EnumPtr->GetEnumName((int32)Value);
	}
	//计算向量夹角, 返回 0 - 360 度, 第一个向量为标定向量, 按顺时针方向计算
	FORCEINLINE float CalcVectorAngle(FVector ArcVec, FVector DesVec)
	{
		ArcVec.Normalize();
		DesVec.Normalize();
		float ResAngle = (FVector::CrossProduct(ArcVec, DesVec).Z > 0 ? 1.f : -1.f) * FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ArcVec, DesVec)));
		if (ResAngle < 0.f) ResAngle += 360.f;
		return ResAngle;
	}
	FORCEINLINE float CalcVector2DAngle(FVector2D ArcVec, FVector2D DesVec)
	{
		ArcVec.Normalize();
		DesVec.Normalize();
		float ResAngle = (FVector2D::CrossProduct(ArcVec, DesVec) > 0 ? 1.f : -1.f) * FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(ArcVec, DesVec)));
		if (ResAngle < 0.f) ResAngle += 360.f;
		return ResAngle;
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

	ADDDriver* GetDriver();

	APlayerController* GetController();

	void InitCommon(ADDDriver* InDriver);

	void InitController(APlayerController* InController);

	//暂停游戏
	void SetPauseGame(bool IsPause);

	//获取游戏是否暂停了
	const bool IsPauseGame() const;

private:


private:

	ADDDriver * Driver;

	APlayerController* PlayerController;

	static UDDCommon* DDInst;

};





//#include "Kismet/GameplayStatics.h"
//#include "Kismet/BlueprintFunctionLibrary.h"
//#include "Components/LineBatchComponent.h"
//Debug方法
//FORCEINLINE void Debug(FString Message, float Duration = 10.f) {
//	if (GEngine) {
//		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Message);
//	}
//}

//下面这两个方法放在了UDDCommon,更加安全
////获取世界
//FORCEINLINE UWorld* GetDDWorld() {
//	if (GWorld) {
//		return GWorld.GetReference();
//	}
//	return NULL;
//}

////获取GameMode方法,这个方法作为获取GameMode的参考,不使用
//FORCEINLINE ADDGameMode* GetDDGameMode()
//{
//	if (GWorld) {
//		UWorld* WorldPtr = GWorld.GetReference();
//		if (WorldPtr) {
//			if (UGameplayStatics::GetGameMode(WorldPtr)) {
//				return (ADDGameMode*)(UGameplayStatics::GetGameMode(WorldPtr));
//			}
//		}
//	}
//	return NULL;
//}

#if 0
	//绘制图形
FORCEINLINE void DrawMarkPoint(FVector InPoint, FLinearColor InColor = FLinearColor::Red, float Duration = 3000.f, float InSize = 10.f)
{
	ULineBatchComponent* const LineBatcher = UDDCommon::Get()->GetDriver()->GetWorld()->PersistentLineBatcher;
	if (LineBatcher)
		LineBatcher->DrawPoint(InPoint, InColor, InSize, 0, Duration);
}

FORCEINLINE void DrawMarkLine(FVector StartPoint, FVector EndPoint, FLinearColor InColor = FLinearColor::Red, float Duration = 3000.f)
{
	ULineBatchComponent* const LineBatcher = UDDCommon::Get()->GetDriver()->GetWorld()->PersistentLineBatcher;
	if (LineBatcher)
		LineBatcher->DrawLine(StartPoint, EndPoint, InColor, 0, 0.f, Duration);
}

FORCEINLINE void DrawMarkLines(TArray<FBatchedLine> InLines)
{
	ULineBatchComponent* const LineBatcher = UDDCommon::Get()->GetDriver()->GetWorld()->PersistentLineBatcher;
	if (LineBatcher)
		LineBatcher->DrawLines(InLines);
}
#endif