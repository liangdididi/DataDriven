// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDUserWidget.h"
#include "DDFrameWidget.generated.h"


class UCanvasPanel;
class UImage;
class UOverlay;
class UDDPanelWidget;
/**
 *
 */
UCLASS()
class DATADRIVEN_API UDDFrameWidget : public UDDUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	//提前加载
	UFUNCTION()
		void AdvanceLoadPanel(FName PanelName);

	//显示面板 面板 = UI功能面板
	UFUNCTION()
		void ShowUIPanel(FName PanelName);

	//隐藏UI
	UFUNCTION()
		void HideUIPanel(FName PanelName);

	//销毁UI
	UFUNCTION()
		void ExitUIPanel(FName PanelName);

	//提前加载面板回调函数
	UFUNCTION()
		void AcceptAdvancePanel(FName BackName, UUserWidget* BackWidget);

	//显示时加载回调函数
	UFUNCTION()
		void AcceptPanelWidget(FName BackName, UUserWidget* BackWidget);

	//处理UI面板销毁后的父控件
	UFUNCTION()
		void ExitCallBack(ELayoutType LayoutType, UPanelWidget* InLayout);

protected:

	//执行进入UI
	void DoEnterUIPanel(FName PanelName);

	//执行显示UI
	void DoShowUIPanel(FName PanelName);

	//正在预加载但是收到显示到界面命令时, 进行循环检测是否加载完毕, 加载完毕则进行显示
	void WaitShowPanel();

	//进入界面, 第一次
	void EnterPanelDoNothing(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget);
	void EnterPanelDoNothing(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget);

	void EnterPanelHideOther(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget);
	void EnterPanelHideOther(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget);

	void EnterPanelReverse(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget);
	void EnterPanelReverse(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget);

	//显示UI
	void ShowPanelDoNothing(UDDPanelWidget* PanelWidget);
	void ShowPanelHideOther(UDDPanelWidget* PanelWidget);
	void ShowPanelReverse(UDDPanelWidget* PanelWidget);

	//隐藏UI
	void HidePanelDoNothing(UDDPanelWidget* PanelWidget);
	void HidePanelHideOther(UDDPanelWidget* PanelWidget);
	void HidePanelReverse(UDDPanelWidget* PanelWidget);

	//销毁UI
	void ExitPanelDoNothing(UDDPanelWidget* PanelWidget);
	void ExitPanelHideOther(UDDPanelWidget* PanelWidget);
	void ExitPanelReverse(UDDPanelWidget* PanelWidget);

	//激活遮罩
	void ActiveMask(UCanvasPanel* WorkLayout, EPanelLucenyType LucenyType);
	void ActiveMask(UOverlay* WorkLayout, EPanelLucenyType LucenyType);

	//转移遮罩, 将遮罩放置在传入的UI面板的下一层
	void TransferMask(UDDPanelWidget* PanelWidget);

	//将MaskPanel移出, 传入的Layout如果不为空, 说明MaskPanel准备添加到这个Layout
	void RemoveMaskPanel(UPanelWidget* WorkLayout = NULL);

protected:

	//根节点
	UCanvasPanel* RootCanvas;

	//保存Overlay控件
	UPROPERTY()
		TArray<UOverlay*> ActiveOverlay;
	UPROPERTY()
		TArray<UOverlay*> UnActiveOverlay;

	//保存Canvas控件
	UPROPERTY()
		TArray<UCanvasPanel*> ActiveCanvas;
	UPROPERTY()
		TArray<UCanvasPanel*> UnActiveCanvas;

	//所有UI面板, 键FName必须是该面板注册到框架的ObjectName
	TMap<FName, UDDPanelWidget*> AllPanelGroup;

	//已经显示的UI
	TMap<FName, UDDPanelWidget*> ShowPanelGroup;

	//弹窗栈
	TMap<FName, UDDPanelWidget*> PopPanelStack;

	//已经加载过的UI面板的名字
	TArray<FName> LoadedPanelName;

	//正在提前加载但是已经收到显示命令的界面名, 简称预显示组
	TArray<FName> WaitShowPanelName;

	//遮罩图片
	UPROPERTY()
		UImage* MaskPanel;

	//透明度值
	FLinearColor NormalLucency;
	FLinearColor TranslucenceLucency;
	FLinearColor ImPenetrableLucency;

	//保存循环检测加载完毕则显示方法的延时循环任务名字
	FName WaitShowTaskName;

};
