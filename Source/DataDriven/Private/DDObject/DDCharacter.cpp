// Fill out your copyright notice in the Description page of Project Settings.

#include "DDCharacter.h"

// Sets default values
ADDCharacter::ADDCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADDCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//自动注册
	RegisterToModule(ModuleName, ObjectName, ClassName);
}


void ADDCharacter::DDRelease()
{
	//能调用这个方法那么一定是注册到了框架,获取的世界一定不为空
	GetDDWorld()->DestroyActor(this);
}