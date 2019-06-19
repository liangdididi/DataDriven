// Fill out your copyright notice in the Description page of Project Settings.

#include "DDDriver.h"
#include "DDOO.h"
#include "DDCenterModule.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
ADDDriver::ADDDriver()
{
	// 允许帧运行
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Center = CreateDefaultSubobject<UDDCenterModule>(TEXT("Center"));
	Center->SetupAttachment(RootComponent);
}

void ADDDriver::PostInitializeComponents()
{
	//先调用一次父类
	Super::PostInitializeComponents();
	//注册世界和Driver到UDDCommon单例
	UDDCommon::Get()->InitCommon(this);
	//在游戏运行之前必须进行一次模组	ID的设定, 在这里面会注册子模组到数组
	Center->IterChangeModuleType(Center, ModuleType);
	//指定完模组ID后收集模组到总数组
	Center->TotalGatherModule(ModuleType);
	//创建Manager组件以及下属组件
	Center->IterCreateManager(Center);
}

void ADDDriver::BeginPlay()
{
	Super::BeginPlay();

	//注册GamePlay到框架
	RegisterGamePlay();

	//调用中央模组的ModuleInit迭代
	Center->IterModuleInit(Center);
}

void ADDDriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsBeginPlay) {
		//调用中央模组的BeginPlay迭代
		Center->IterModuleBeginPlay(Center);
		//第一帧运行全体的BeginPlay函数
		IsBeginPlay = true;
	}
	else {
		//调用中央模组的Tick迭代
		Center->IterModuleTick(Center, DeltaTime);
	}
}

void ADDDriver::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	Center->AllotExecuteFunction(Agreement, Param);
}

void ADDDriver::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	Center->AllotExecuteFunction(Agreement, Param);
}

#if WITH_EDITOR
void ADDDriver::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ADDDriver, ModuleType))
	{
		Center->IterChangeModuleType(Center, ModuleType);
	}
}
#endif

bool ADDDriver::RegisterToModule(IDDOO* Object)
{
	//调用中央模组的注册对象功能
	return Center->RegisterToModule(Object);
}

void ADDDriver::RegisterGamePlay()
{
	//获取GameInstance
	UGameInstance* GameInst = UGameplayStatics::GetGameInstance(GetWorld());
	//如果存在并且继承自IDDOO,就注册进Center,类名和对象名都是GameInstance
	if (GameInst && Cast<IDDOO>(GameInst))
		Cast<IDDOO>(GameInst)->RegisterToModule("Center", "GameInstacne", "GameInstacne");
 
	//获取Controller并且注册到DDCommon, 用于提供事件调用接口
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) DDH::Debug() << "No PlayerController" << DDH::Endl();
	UDDCommon::Get()->InitController(PlayerController);
}

