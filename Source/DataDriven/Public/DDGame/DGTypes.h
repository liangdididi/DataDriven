// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "DGTypes.generated.h"


#pragma region GamePart

/************************************************************************/
/* 开发的游戏的内容放这里              Begin            开发的游戏的内容放这里*/
/************************************************************************/

//模组约定, 如果要使用DataDriven本身的UI框架, 请一定要把HUD模组放在第二位, UIFrame框架管理器始终在HUD模组下

//坦克游戏的菜单模组
UENUM()
enum class ETKMenuModule : uint8 {
	Center = 0, //中央模组
	HUD,        //HUD, 约定放在第二位
	Controller, //控制器
	Data,       //游戏数据
};


UENUM()
enum class ETKGameModule : uint8 {
	Center = 0,  //中央模组
	HUD,         //HUD, 约定放在第二位
	Data,        //数据模组
	Player,      //玩家模组
	Bots,        //机器人模组
};


//人称视角
UENUM()
enum class EViewModel : uint8 {
	First = 0, //第一人称
	Third,     //第三人称
};


//先黄后蓝, 按字母来
UENUM()
enum class ETankModel : uint8 {
	ISU152 = 0,
	KV2,
	T26,
	T34,
	Pz35,
	PzIV,
	StVI,
	Tiger,
};


//坦克数据结构体
USTRUCT(BlueprintType)
struct FTankData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		ETankModel TankModel;

	UPROPERTY(EditAnywhere)
		int32 TankPrice;

	UPROPERTY(EditAnywhere)
		int32 MotorSpeed;

	UPROPERTY(EditAnywhere)
		int32 TurretSpeed;

	UPROPERTY(EditAnywhere)
		int32 ReloadSpeed;

	UPROPERTY(EditAnywhere)
		int32 ArmoredDefense;


};

//炮弹枚举
UENUM()
enum class EAmmoModel : uint8
{
	Addition = 0, //无
	Micro,        //微型导弹
	Medium,       //中型导弹
	Steeltoe,     //刚头导弹
	Armour,       //穿甲导弹
	Frost,        //冷冻导弹
	Flame,        //火焰导弹
	Hollow,       //黑洞导弹
	Giant,        //巨人导弹
};

//炮弹数据结构体
USTRUCT(BlueprintType)
struct FAmmoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	//类型
	UPROPERTY(EditAnywhere)
		EAmmoModel AmmoModel;
	//价格
	UPROPERTY(EditAnywhere)
		int32 Price;
	//加载难度
	UPROPERTY(EditAnywhere)
		int32 LoadCost;
	//伤害
	UPROPERTY(EditAnywhere)
		int32 Damage;
	//范围
	UPROPERTY(EditAnywhere)
		int32 Scope;
	//信息
	UPROPERTY(EditAnywhere)
		FText Info;

};

//道具枚举
UENUM()
enum class EPropModel : uint8 {
	Addition = 0,  //无
	Cure,          //治愈
	Speed,         //速度
	Power,         //力量
	Shield,        //防御
	Teleport,      //传送
	Infinite,      //无敌
};

//道具数据结构体
USTRUCT(BlueprintType)
struct FPropData : public FTableRowBase
{
	GENERATED_BODY()

public:
	//类型
	UPROPERTY(EditAnywhere)
		EPropModel PropModel;
	//价格
	UPROPERTY(EditAnywhere)
		int32 Price;
	//效果
	UPROPERTY(EditAnywhere)
		int32 Effect;
	//持续时间
	UPROPERTY(EditAnywhere)
		int32 Duration;
	//信息
	UPROPERTY(EditAnywhere)
		FText Info;

};

//坦克存储数据结构
USTRUCT()
struct FTankSave
{
	GENERATED_BODY()

public:

	UPROPERTY()
		ETankModel TankModel;

	UPROPERTY()
		TArray<int32> EquipValue;

	UPROPERTY()
		TArray<int32> EquipLevel;

	int32 MotorSpeed()
	{
		return EquipValue[0];
	}

	int32 TurretSpeed()
	{
		return EquipValue[1];
	}

	int32 ReloadSpeed()
	{
		return EquipValue[2];
	}

	int32 ArmoredDefense()
	{
		return EquipValue[3];
	}

public:

	void InitTankData(FTankData InData)
	{
		TankModel = InData.TankModel;
		EquipValue.Push(InData.MotorSpeed);
		EquipValue.Push(InData.TurretSpeed);
		EquipValue.Push(InData.ReloadSpeed);
		EquipValue.Push(InData.ArmoredDefense);
		for (int i = 0; i < 4; ++i)
			EquipLevel.Push(0);
	}

};


//炮弹存储数据结构
USTRUCT()
struct FAmmoSave
{
	GENERATED_BODY()

public:

	UPROPERTY()
		EAmmoModel AmmoModel;

	UPROPERTY()
		int32 Count;

	FAmmoSave()
	{
		AmmoModel = EAmmoModel::Addition;
		Count = 0;
	}

};


//道具存储数据结构
USTRUCT()
struct FPropSave
{
	GENERATED_BODY()

public:

	UPROPERTY()
		EPropModel PropModel;

	UPROPERTY()
		int32 Count;

	FPropSave()
	{
		PropModel = EPropModel::Addition;
		Count = 0;
	}

};


//Bot坦克运动状态
UENUM()
enum class EBotSteer : uint8 {
	Patrol = 0,
	Pursue,
	Escape
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
