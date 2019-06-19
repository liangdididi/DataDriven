// Fill out your copyright notice in the Description page of Project Settings.

#include "DDPanelWidget.h"


FName UDDPanelWidget::UIFrameName(TEXT("UIFrame"));

FName UDDPanelWidget::ShowUIPanelName(TEXT("ShowUIPanel"));

FName UDDPanelWidget::HideUIPanelName(TEXT("HideUIPanel"));

FName UDDPanelWidget::ExitUIPanelName(TEXT("ExitUIPanel"));

FName UDDPanelWidget::ExitCallBackName(TEXT("ExitCallBack"));

bool UDDPanelWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	//UIFrame默认放在第二个模组, 一般序号为 Center = 0 , HUD = 1
	UIFrameModuleIndex = 1;

	return true;
}

void UDDPanelWidget::PanelEnter()
{
	//设置Visible
	SetVisibility(ESlateVisibility::Visible);
	//直接运行进入动画
	DisplayEnterMovie();
}

void UDDPanelWidget::PanelDisplay()
{
	//设置Visible
	SetVisibility(ESlateVisibility::Visible);
	//运行进入动画
	DisplayEnterMovie();
}

void UDDPanelWidget::PanelFreeze()
{

}

void UDDPanelWidget::PanelResume()
{

}

void UDDPanelWidget::PanelHidden()
{
	//运行完动画后延时设置Hidden
	InvokeDelay("PanelHidden", DisplayLeaveMove(), this, &UDDPanelWidget::SetSelfHidden);
}

void UDDPanelWidget::PanelExit()
{
	//进行延时动画和回调, 如果已经是隐藏状态就直接进行回调, 不播放动画了
	if (GetVisibility() != ESlateVisibility::Hidden)
		InvokeDelay("PanelHidden", DisplayLeaveMove(), this, &UDDPanelWidget::RemoveCallBack);
	else
		RemoveCallBack();
}

void UDDPanelWidget::ShowSelfPanel()
{
	ShowUIPanel(GetObjectName());
}

void UDDPanelWidget::HideSelfPanel()
{
	HideUIPanel(GetObjectName());
}

void UDDPanelWidget::ExitSelfPanel()
{
	ExitUIPanel(GetObjectName());
}

void UDDPanelWidget::ShowUIPanel(FName PanelName)
{
	OperatorUIPanel(UIFrameModuleIndex, UIFrameName, ShowUIPanelName, PanelName);
}

void UDDPanelWidget::HideUIPanel(FName PanelName)
{
	OperatorUIPanel(UIFrameModuleIndex, UIFrameName, HideUIPanelName, PanelName);
}

void UDDPanelWidget::ExitUIPanel(FName PanelName)
{
	OperatorUIPanel(UIFrameModuleIndex, UIFrameName, ExitUIPanelName, PanelName);
}

void UDDPanelWidget::RemoveCallBack()
{
	UPanelWidget* WorkLayout = GetParent();
	RemoveFromParent();
	ExitCallBack(UIFrameModuleIndex, UIFrameName, ExitCallBackName, UINature.LayoutType, WorkLayout);
	DDDestroy();
}

void UDDPanelWidget::SetSelfHidden()
{
	SetVisibility(ESlateVisibility::Hidden);
}

