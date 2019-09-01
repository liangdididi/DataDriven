// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DGTypes.generated.h"


#pragma region GamePart

/************************************************************************/
/* 开发的游戏的内容放这里              Begin            开发的游戏的内容放这里*/
/************************************************************************/

//示例模组枚举, 模组约定, 如果要使用DataDriven本身的UI框架, 请一定要把HUD模组放在第二位, UIFrame框架管理器始终在HUD模组下
UENUM()
enum class EDDExampleModule : uint8
{
	Center = 0,    //中央模组, 约定放在第一位
	HUD,           //UI模组, 约定放在第二位, UI框架会直接使用序号1去获取HUD模组, 如果不使用UI框架, 不需要强制在第二位
	Data,          //数据模组
	Player,        //玩家模组
	NPC,           //NPC模组
	Bots,          //敌对模组
};



/************************************************************************/
/* 开发的游戏的内容放这里               End             开发的游戏的内容放这里*/
/************************************************************************/


#pragma endregion


/**
 * 
 */
UCLASS()
class DATADRIVEN_API UDGTypes : public UObject
{
	GENERATED_BODY()
	
};
