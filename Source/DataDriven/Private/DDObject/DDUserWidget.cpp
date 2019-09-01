// Fill out your copyright notice in the Description page of Project Settings.


#include "DDUserWidget.h"

void UDDUserWidget::DDRelease()
{
	IDDOO::DDRelease();
	//再检查一次从父类移除
	RemoveFromParent();
	//从Root移除
	RemoveFromRoot();
	//准备回收资源
	ConditionalBeginDestroy();
}
