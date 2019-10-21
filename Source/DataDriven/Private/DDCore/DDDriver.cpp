// Fill out your copyright notice in the Description page of Project Settings.


#include "DDDriver.h"
#include "DDOO.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADDDriver::ADDDriver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Center = CreateDefaultSubobject<UDDCenterModule>(TEXT("Center"));
	Center->SetupAttachment(RootComponent);
}

void ADDDriver::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//注册Driver到UDDCommon单例
	UDDCommon::Get()->InitDriver(this);
	//在游戏运行之前必须进行一次模组	ID的设定, 在这里面会注册子模组到数组
	Center->IterChangeModuleType(Center, ModuleType);
	//指定完模组ID后收集模组到总数组
	Center->TotalGatherModule(ModuleType);
	//创建所用模组的模块
	Center->IterCreateManager(Center);
}

// Called when the game starts or when spawned
void ADDDriver::BeginPlay()
{
	Super::BeginPlay();

	//注册GamePlay到框架
	RegisterGamePlay();

	//迭代调用Init函数
	Center->IterModuleInit(Center);
}

void ADDDriver::RegisterGamePlay()
{
	//获取GameInstance
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	//如果存在并且继承自IDDOO,就注册进Center,类名和对象名都是GameInstance
	if (GameInstance && Cast<IDDOO>(GameInstance))
		Cast<IDDOO>(GameInstance)->RegisterToModule("Center", "GameInstance", "GameInstance");

	//获取Controller并且注册到DDCommon
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//注册到Common
	if (!PlayerController)
		DDH::Debug() << "No PlayerController" << DDH::Endl();
	else
		UDDCommon::Get()->InitController(PlayerController);
}

// Called every frame
void ADDDriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsBeginPlay)
	{
		//迭代调用Begin函数
		Center->IterModuleBeginPlay(Center);
		//只执行第一帧
		IsBeginPlay = true;
	}
	else
		//迭代调用Tick函数
		Center->IterModuleTick(Center, DeltaTime);
}

bool ADDDriver::RegisterToModule(IDDOO* ObjectInst)
{
	//调用中央模组进行注册
	return Center->RegisterToModule(ObjectInst);
}

#if WITH_EDITOR

void ADDDriver::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ADDDriver, ModuleType))
		Center->IterChangeModuleType(Center, ModuleType);
}

#endif

void ADDDriver::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	Center->AllotExecuteFunction(Agreement, Param);
}

void ADDDriver::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	Center->AllotExecuteFunction(Agreement, Param);
}
