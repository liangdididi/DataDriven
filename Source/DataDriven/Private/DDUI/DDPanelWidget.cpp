// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPanelWidget.h"

FName UDDPanelWidget::PanelHiddenName(TEXT("PanelHiddenTask"));

int32 UDDPanelWidget::UIFrameModuleIndex(1);

FName UDDPanelWidget::UIFrameName(TEXT("UIFrame"));

FName UDDPanelWidget::ExitCallBackName(TEXT("ExitCallBack"));

FName UDDPanelWidget::ShowUIPanelName(TEXT("ShowUIPanel"));

FName UDDPanelWidget::HideUIPanelName(TEXT("HideUIPanel"));

FName UDDPanelWidget::ExitUIPanelName(TEXT("ExitUIPanel"));

FName UDDPanelWidget::AdvanceLoadPanelName(TEXT("AdvanceLoadPanel"));

void UDDPanelWidget::PanelEnter()
{
	SetVisibility(ESlateVisibility::Visible);
	//调用进入界面动画
	DisplayEnterMovie();
}

void UDDPanelWidget::PanelDisplay()
{
	SetVisibility(ESlateVisibility::Visible);
	//调用进入界面动画
	DisplayEnterMovie();
}

void UDDPanelWidget::PanelHidden()
{
	//运行完移出界面动画后调用隐藏函数
	InvokeDelay(PanelHiddenName, DisplayLeaveMovie(), this, &UDDPanelWidget::SetSelfHidden);
}

void UDDPanelWidget::PanelFreeze()
{

}

void UDDPanelWidget::PanelResume()
{

}

void UDDPanelWidget::PanelExit()
{
	//如果UI面板正在显示
	if (GetVisibility() != ESlateVisibility::Hidden)
		InvokeDelay(PanelHiddenName, DisplayLeaveMovie(), this, &UDDPanelWidget::RemoveCallBack);
	else
		RemoveCallBack();
}

void UDDPanelWidget::SetSelfHidden()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UDDPanelWidget::RemoveCallBack()
{
	//获取父控件
	UPanelWidget* WorkLayout = GetParent();
	//已经加载了UI面板, 但是一直没有运行显示命令的情况下, WorkLayout为空
	if (WorkLayout)
	{
		RemoveFromParent();
		//告诉UI管理器处理父控件
		ExitCallBack(UIFrameModuleIndex, UIFrameName, ExitCallBackName, UINature.LayoutType, WorkLayout);
	}
	//执行销毁
	DDDestroy();
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

void UDDPanelWidget::AdvanceLoadPanel(FName PanelName)
{
	OperatorUIPanel(UIFrameModuleIndex, UIFrameName, AdvanceLoadPanelName, PanelName);
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

