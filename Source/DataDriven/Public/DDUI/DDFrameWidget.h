// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDUserWidget.h"
#include "DDFrameWidget.generated.h"


class UOverlay;
class UCanvasPanel;
class UDDPanelWidget;
class UImage;
/**
 *
 */
UCLASS()
class DATADRIVEN_API UDDFrameWidget : public UDDUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	//显示UI
	UFUNCTION()
		void ShowUIPanel(FName PanelName);

	//隐藏UI
	UFUNCTION()
		void HideUIPanel(FName PanelName);

	//销毁UI
	UFUNCTION()
		void ExitUIPanel(FName PanelName);

	//处理PanelWidget销毁后的WorkLayout
	UFUNCTION()
		void ExitCallBack(ELayoutType LayoutType, UPanelWidget* InLayout);

	//获取加载的PanelWidget
	UFUNCTION()
		void AcceptPanelWidget(FName BackName, UUserWidget* BackWidget);

	//获取提前加载的PanelWidget, 不显示只加载到栈
	UFUNCTION()
		void AcceptAdvancePanel(FName BackName, UUserWidget* BackWidget);

public:



protected:

	//提前加载Panel到内存
	void AdvanceLoadPanel(FName PanelName);

	//执行进入UI
	void DoEnterUIPanel(FName PanelName);

	//执行显示UI
	void DoShowUIPanel(FName PanelName);

	//插入UI, 第一次
	void EnterPanelDoNothing(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget);
	void EnterPanelDoNothing(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget);

	void EnterPanelHideOther(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget);
	void EnterPanelHideOther(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget);

	void EnterPanelReverse(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget);
	void EnterPanelReverse(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget);

	//显示UI, UI已经在库中
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

	//激活遮罩到WorkLayout
	void ActiveMask(UCanvasPanel* WorkLayout, EPanelLucenyType LucenyType);
	void ActiveMask(UOverlay* WorkLayout, EPanelLucenyType LucenyType);
	//转移遮罩到其他位置, 就是把MaskPanel放在这一个Panel的上一级
	void TransferMask(UDDPanelWidget* PanelWidget);

	//将MaskPanel移出, 传入的Layout如果不为空, 说明MaskPanel准备添加到这个Layout
	void RemoveMaskPanel(UPanelWidget* WorkLayout = NULL);

protected:

	//根节点
	UCanvasPanel* RootCanvas;

	TArray<UOverlay*> ActiveOverlay;

	TArray<UOverlay*> UnActiveOverlay;

	TArray<UCanvasPanel*> ActiveCanvas;

	TArray<UCanvasPanel*> UnActiveCanvas;

	//所有UIPanel组, 本框架强制PanelName必须和ObjectName一致
	TMap<FName, UDDPanelWidget*> AllPanelGroup;

	//UIPanel显示组, 本框架强制PanelName必须和ObjectName一致
	TMap<FName, UDDPanelWidget*> ShowPanelGroup;

	//栈结构窗体
	TMap<FName, UDDPanelWidget*> PopPanelStack;

	//已进入过加载进程的对象
	TArray<FName> LoadedPanelName;

	//遮罩图片
	UPROPERTY()
		UImage* MaskPanel;

	//透明度值
	FLinearColor NormalLucency;
	FLinearColor TranslucenceLucency;
	FLinearColor ImPenetrableLucency;
};
