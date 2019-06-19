// Fill out your copyright notice in the Description page of Project Settings.

#include "DDObject.h"







void UDDObject::DDRelease()
{
	//从Root移除
	RemoveFromRoot();
	//准备回收资源
	ConditionalBeginDestroy();
}
