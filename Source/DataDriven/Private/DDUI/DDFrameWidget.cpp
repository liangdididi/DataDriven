// Fill out your copyright notice in the Description page of Project Settings.


#include "DDFrameWidget.h"
#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "Image.h"
#include "WidgetTree.h"
#include "Overlay.h"
#include "OverlaySlot.h"
#include "DDUI/DDPanelWidget.h"

bool UDDFrameWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	//获取根节点
	RootCanvas = Cast<UCanvasPanel>(GetRootWidget());
	RootCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//生成遮罩
	MaskPanel = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());

	//设置透明度
	NormalLucency = FLinearColor(1.f, 1.f, 1.f, 0.f);
	TranslucenceLucency = FLinearColor(0.f, 0.f, 0.f, 0.6f);
	ImPenetrableLucency = FLinearColor(0.f, 0.f, 0.f, 0.3f);

	WaitShowTaskName = FName("WaitShowTask");

	return true;
}

void UDDFrameWidget::AdvanceLoadPanel(FName PanelName)
{
	//如果已经存在全部组存在该面板或则已经加载面板名组存在该面板名
	if (AllPanelGroup.Contains(PanelName) || LoadedPanelName.Contains(PanelName))
		return;
	//进行异步加载
	BuildSingleClassWealth(EWealthType::Widget, PanelName, "AcceptAdvancePanel");
	//添加面板名到已经加载名字组
	LoadedPanelName.Push(PanelName);
}

void UDDFrameWidget::ShowUIPanel(FName PanelName)
{
	//如果面板是否已经显示在界面上
	if (ShowPanelGroup.Contains(PanelName) || PopPanelStack.Contains(PanelName))
		return;
	//如果判断是否已经加载该面板
	if (!AllPanelGroup.Contains(PanelName) && !LoadedPanelName.Contains(PanelName))
	{
		BuildSingleClassWealth(EWealthType::Widget, PanelName, "AcceptPanelWidget");
		LoadedPanelName.Push(PanelName);
		return;
	}

	//如果预加载未完成, 就调用显示命令, 启动循环检测函数, 检测到预加载完成的时候, 显示UI面板
	if (!AllPanelGroup.Contains(PanelName) && LoadedPanelName.Contains(PanelName) && !WaitShowPanelName.Contains(PanelName))
	{
		//添加名字到预显示名字组
		WaitShowPanelName.Push(PanelName);
		//启动循环检测加载完毕则显示函数, 每0.3秒检测一次
		InvokeRepeat(WaitShowTaskName, 0.3f, 0.3f, this, &UDDFrameWidget::WaitShowPanel);
		return;
	}


	//如果存在该UI面板
	if (AllPanelGroup.Contains(PanelName))
	{
		//判定是否是第一次显示在界面上
		UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
		//如果没有父控件, 说明没有进入过界面
		if (PanelWidget->GetParent())
			DoShowUIPanel(PanelName);
		else
			DoEnterUIPanel(PanelName);
	}
}



void UDDFrameWidget::AcceptAdvancePanel(FName BackName, UUserWidget* BackWidget)
{
	UDDPanelWidget* PanelWidget = Cast<UDDPanelWidget>(BackWidget);

	//如果不是继承PanelWidget
	if (!PanelWidget)
	{
		DDH::Debug() << "Load UI Panel : " << BackName << " Is Not DDPanelWidget" << DDH::Endl();
		return;
	}

	//注册到框架,不注册类名, BackName必须是面板名以及ObjectName
	PanelWidget->RegisterToModule(ModuleIndex, BackName);

	//添加到全部组
	AllPanelGroup.Add(BackName, PanelWidget);
}

void UDDFrameWidget::AcceptPanelWidget(FName BackName, UUserWidget* BackWidget)
{
	UDDPanelWidget* PanelWidget = Cast<UDDPanelWidget>(BackWidget);

	//如果不是继承PanelWidget
	if (!PanelWidget)
	{
		DDH::Debug() << "Load UI Panel : " << BackName << " Is Not DDPanelWidget" << DDH::Endl();
		return;
	}

	//注册到框架,不注册类名, BackName必须是面板名以及ObjectName
	PanelWidget->RegisterToModule(ModuleIndex, BackName);

	//添加到全部组
	AllPanelGroup.Add(BackName, PanelWidget);

	//进行第一次显示, 执行进入界面方法
	DoEnterUIPanel(BackName);
}

void UDDFrameWidget::ExitCallBack(ELayoutType LayoutType, UPanelWidget* InLayout)
{
	if (LayoutType == ELayoutType::Canvas)
	{
		UCanvasPanel* WorkLayout = Cast<UCanvasPanel>(InLayout);
		if (WorkLayout->GetChildrenCount() == 0)
		{
			WorkLayout->RemoveFromParent();
			ActiveCanvas.Remove(WorkLayout);
			UnActiveCanvas.Push(WorkLayout);
		}
	}
	else
	{
		UOverlay* WorkLayout = Cast<UOverlay>(InLayout);
		if (WorkLayout->GetChildrenCount() == 0)
		{
			WorkLayout->RemoveFromParent();
			ActiveOverlay.Remove(WorkLayout);
			UnActiveOverlay.Push(WorkLayout);
		}
	}
}

void UDDFrameWidget::WaitShowPanel()
{
	TArray<FName> CompleteName;
	for (int i = 0; i < WaitShowPanelName.Num(); ++i)
	{
		if (AllPanelGroup.Contains(WaitShowPanelName[i]))
		{
			//执行进入界面方法
			DoEnterUIPanel(WaitShowPanelName[i]);
			//添加到完成组
			CompleteName.Push(WaitShowPanelName[i]);
		}
	}
	//移除完成的UI
	for (int i = 0; i < CompleteName.Num(); ++i)
		WaitShowPanelName.Remove(CompleteName[i]);
	//如果没有等待显示的UI了, 停止该循环函数
	if (WaitShowPanelName.Num() == 0)
		StopInvoke(WaitShowTaskName);
}

void UDDFrameWidget::DoEnterUIPanel(FName PanelName)
{
	//获取面板实例
	UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
	//区分布局类型进行添加到界面
	if (PanelWidget->UINature.LayoutType == ELayoutType::Canvas)
	{
		//获取布局控件, 父控件
		UCanvasPanel* WorkLayout = NULL;

		//判断最底层的布局控件是否是Canvas
		if (RootCanvas->GetChildrenCount() > 0)
			WorkLayout = Cast<UCanvasPanel>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));

		if (!WorkLayout)
		{
			if (UnActiveCanvas.Num() == 0)
			{
				WorkLayout = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
				WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
				WorkLayout = UnActiveCanvas.Pop();
			//添加布局控件到界面最顶层
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			//添加到激活画布组
			ActiveCanvas.Push(WorkLayout);
		}

		switch (PanelWidget->UINature.PanelShowType)
		{
		case EPanelShowType::DoNothing:
			EnterPanelDoNothing(WorkLayout, PanelWidget);
			break;
		case EPanelShowType::HideOther:
			EnterPanelHideOther(WorkLayout, PanelWidget);
			break;
		case EPanelShowType::Reverse:
			EnterPanelReverse(WorkLayout, PanelWidget);
			break;
		}
	}
	else
	{
		UOverlay* WorkLayout = NULL;

		//如果存在布局控件, 试图把最后一个布局控件转换成Overlay
		if (RootCanvas->GetChildrenCount() > 0)
			WorkLayout = Cast<UOverlay>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));

		if (!WorkLayout)
		{
			if (UnActiveOverlay.Num() == 0)
			{
				WorkLayout = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
				WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
				WorkLayout = UnActiveOverlay.Pop();
			//添加布局控件到界面最顶层
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			//添加到激活画布组
			ActiveOverlay.Push(WorkLayout);
		}

		switch (PanelWidget->UINature.PanelShowType)
		{
		case EPanelShowType::DoNothing:
			EnterPanelDoNothing(WorkLayout, PanelWidget);
			break;
		case EPanelShowType::HideOther:
			EnterPanelHideOther(WorkLayout, PanelWidget);
			break;
		case EPanelShowType::Reverse:
			EnterPanelReverse(WorkLayout, PanelWidget);
			break;
		}
	}
}

void UDDFrameWidget::EnterPanelDoNothing(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//添加UI面板到父控件
	UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(PanelWidget);
	PanelSlot->SetAnchors(PanelWidget->UINature.Anchors);
	PanelSlot->SetOffsets(PanelWidget->UINature.Offsets);

	//把UI面板添加到显示组, UI面板的GetObjectName(), PanelName, 资源系统下的WealthName必须一致
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	//调用UI面板的进入界面生命周期
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelDoNothing(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//添加UI面板到Overlay布局
	UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(PanelWidget);
	PanelSlot->SetPadding(PanelWidget->UINature.Offsets);
	PanelSlot->SetHorizontalAlignment(PanelWidget->UINature.HAlign);
	PanelSlot->SetVerticalAlignment(PanelWidget->UINature.VAlign);

	//把UI面板添加到显示组, UI面板的GetObjectName(), PanelName, 资源系统下的WealthName必须一致
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	//调用UI面板的进入界面生命周期
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelHideOther(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//将显示组的同一层级的其他对象都隐藏, 如果是Level_All就全部隐藏, Level_All优先级高
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All || PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelHidden();

	//添加UI面板到Layout
	UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(PanelWidget);
	PanelSlot->SetAnchors(PanelWidget->UINature.Anchors);
	PanelSlot->SetOffsets(PanelWidget->UINature.Offsets);

	//添加UI面板添加到显示组
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	//调用进入界面生命周期函数
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelHideOther(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//将显示组的同一层级的其他对象都隐藏, 如果是Level_All就全部隐藏, Level_All优先级高
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All || PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelHidden();

	//添加到UOverlay
	UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(PanelWidget);
	PanelSlot->SetPadding(PanelWidget->UINature.Offsets);
	PanelSlot->SetHorizontalAlignment(PanelWidget->UINature.HAlign);
	PanelSlot->SetVerticalAlignment(PanelWidget->UINature.VAlign);

	//添加UI面板添加到显示组
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	//调用进入界面生命周期函数
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelReverse(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//把栈内最后一个节点冻结
	if (PopPanelStack.Num() > 0)
	{
		TArray<UDDPanelWidget*> PanelStack;
		PopPanelStack.GenerateValueArray(PanelStack);
		PanelStack[PanelStack.Num() - 1]->PanelFreeze();
	}

	//激活遮罩
	ActiveMask(WorkLayout, PanelWidget->UINature.PanelLucenyType);

	//添加弹窗到界面
	UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(PanelWidget);
	PanelSlot->SetAnchors(PanelWidget->UINature.Anchors);
	PanelSlot->SetOffsets(PanelWidget->UINature.Offsets);

	//添加弹窗到栈, 并且运行进入生命周期函数
	PopPanelStack.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelReverse(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//把栈内最后一个节点冻结
	if (PopPanelStack.Num() > 0)
	{
		TArray<UDDPanelWidget*> PanelStack;
		PopPanelStack.GenerateValueArray(PanelStack);
		PanelStack[PanelStack.Num() - 1]->PanelFreeze();
	}

	//激活遮罩
	ActiveMask(WorkLayout, PanelWidget->UINature.PanelLucenyType);

	//添加弹窗到界面
	UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(PanelWidget);
	PanelSlot->SetPadding(PanelWidget->UINature.Offsets);
	PanelSlot->SetHorizontalAlignment(PanelWidget->UINature.HAlign);
	PanelSlot->SetVerticalAlignment(PanelWidget->UINature.VAlign);

	//添加弹窗到栈, 并且运行进入生命周期函数
	PopPanelStack.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelEnter();
}



void UDDFrameWidget::DoShowUIPanel(FName PanelName)
{
	//从全部组获取对象
	UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);

	//根据UI面板类型调用不同的显示方法
	switch (PanelWidget->UINature.PanelShowType)
	{
	case EPanelShowType::DoNothing:
		ShowPanelDoNothing(PanelWidget);
		break;
	case EPanelShowType::HideOther:
		ShowPanelHideOther(PanelWidget);
		break;
	case EPanelShowType::Reverse:
		ShowPanelReverse(PanelWidget);
		break;
	}
}

void UDDFrameWidget::ShowPanelDoNothing(UDDPanelWidget* PanelWidget)
{
	//添加UI面板到显示组
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelDisplay();
}

void UDDFrameWidget::ShowPanelHideOther(UDDPanelWidget* PanelWidget)
{
	//将显示组的同一层级的其他对象都隐藏, 如果是Level_All就全部隐藏, Level_All优先级高
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All || PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelHidden();

	//添加到显示组
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelDisplay();
}

void UDDFrameWidget::ShowPanelReverse(UDDPanelWidget* PanelWidget)
{
	//如果弹窗栈里有元素, 冻结最顶层的弹窗
	if (PopPanelStack.Num() > 0)
	{
		TArray<UDDPanelWidget*> PanelStack;
		PopPanelStack.GenerateValueArray(PanelStack);
		PanelStack[PanelStack.Num() - 1]->PanelFreeze();
	}

	//弹窗对象必须从当前父控件移除, 再重新添加到最顶层的界面
	if (PanelWidget->UINature.LayoutType == ELayoutType::Canvas)
	{
		UCanvasPanel* PreWorkLayout = Cast<UCanvasPanel>(PanelWidget->GetParent());
		UCanvasPanelSlot* PrePanelSlot = Cast<UCanvasPanelSlot>(PanelWidget->Slot);
		FAnchors PreAnchors = PrePanelSlot->GetAnchors();
		FMargin PreOffsets = PrePanelSlot->GetOffsets();

		//将PanelWidget从当前父控件移除
		PanelWidget->RemoveFromParent();
		//处理父控件
		if (PreWorkLayout->GetChildrenCount() == 0)
		{
			PreWorkLayout->RemoveFromParent();
			ActiveCanvas.Remove(PreWorkLayout);
			UnActiveCanvas.Push(PreWorkLayout);
		}

		//寻找最顶层的WorkLayout
		UCanvasPanel* WorkLayout = NULL;

		//判断最底层的布局控件是否是Canvas
		if (RootCanvas->GetChildrenCount() > 0)
			WorkLayout = Cast<UCanvasPanel>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));

		if (!WorkLayout)
		{
			if (UnActiveCanvas.Num() == 0)
			{
				WorkLayout = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
				WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
				WorkLayout = UnActiveCanvas.Pop();
			//添加布局控件到界面最顶层
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			//添加到激活画布组
			ActiveCanvas.Push(WorkLayout);
		}

		//激活遮罩到最顶层弹窗
		ActiveMask(WorkLayout, PanelWidget->UINature.PanelLucenyType);

		//把弹窗添加到获取的最顶层的父控件
		UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(PanelWidget);
		PanelSlot->SetAnchors(PreAnchors);
		PanelSlot->SetOffsets(PreOffsets);
	}
	else
	{
		UOverlay* PreWorkLayout = Cast<UOverlay>(PanelWidget->GetParent());
		UOverlaySlot* PrePanelSlot = Cast<UOverlaySlot>(PanelWidget->Slot);
		FMargin PrePadding = PrePanelSlot->Padding;
		TEnumAsByte<EHorizontalAlignment> PreHAlign = PrePanelSlot->HorizontalAlignment;
		TEnumAsByte<EVerticalAlignment> PreVAlign = PrePanelSlot->VerticalAlignment;

		//将PanelWidget从当前父控件移除
		PanelWidget->RemoveFromParent();
		//处理父控件
		if (PreWorkLayout->GetChildrenCount() == 0)
		{
			PreWorkLayout->RemoveFromParent();
			ActiveOverlay.Remove(PreWorkLayout);
			UnActiveOverlay.Push(PreWorkLayout);
		}

		UOverlay* WorkLayout = NULL;

		//如果存在布局控件, 试图把最后一个布局控件转换成Overlay
		if (RootCanvas->GetChildrenCount() > 0)
			WorkLayout = Cast<UOverlay>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));

		if (!WorkLayout)
		{
			if (UnActiveOverlay.Num() == 0)
			{
				WorkLayout = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
				WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
				WorkLayout = UnActiveOverlay.Pop();
			//添加布局控件到界面最顶层
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			//添加到激活画布组
			ActiveOverlay.Push(WorkLayout);
		}

		//激活遮罩到最顶层弹窗
		ActiveMask(WorkLayout, PanelWidget->UINature.PanelLucenyType);

		//添加弹窗到获取到的最顶层的布局控件
		UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(PanelWidget);
		PanelSlot->SetPadding(PrePadding);
		PanelSlot->SetHorizontalAlignment(PreHAlign);
		PanelSlot->SetVerticalAlignment(PreVAlign);
	}

	//添加弹窗到栈
	PopPanelStack.Add(PanelWidget->GetObjectName(), PanelWidget);
	//显示弹窗
	PanelWidget->PanelDisplay();
}


void UDDFrameWidget::HideUIPanel(FName PanelName)
{
	//判断UI面板是否在显示组或者弹窗栈
	if (!ShowPanelGroup.Contains(PanelName) && !PopPanelStack.Contains(PanelName))
		return;

	//获取UI面板
	UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);

	//根据UI面板类型调用不同的隐藏方法
	switch (PanelWidget->UINature.PanelShowType)
	{
	case EPanelShowType::DoNothing:
		HidePanelDoNothing(PanelWidget);
		break;
	case EPanelShowType::HideOther:
		HidePanelHideOther(PanelWidget);
		break;
	case EPanelShowType::Reverse:
		HidePanelReverse(PanelWidget);
		break;
	}
}


void UDDFrameWidget::HidePanelDoNothing(UDDPanelWidget* PanelWidget)
{
	//从显示组移除
	ShowPanelGroup.Remove(PanelWidget->GetObjectName());
	//运行隐藏生命周期
	PanelWidget->PanelHidden();
}

void UDDFrameWidget::HidePanelHideOther(UDDPanelWidget* PanelWidget)
{
	//从显示组移除
	ShowPanelGroup.Remove(PanelWidget->GetObjectName());

	//显示同一层级下的其他UI面板, 如果该面板是Level_All层级, 显示所有显示组的面板
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All || PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelDisplay();

	//运行隐藏生命周期
	PanelWidget->PanelHidden();
}

void UDDFrameWidget::HidePanelReverse(UDDPanelWidget* PanelWidget)
{
	//获取弹窗栈到数组
	TArray<UDDPanelWidget*> PopStack;
	PopPanelStack.GenerateValueArray(PopStack);

	//如果不是最上层的弹窗, 直接返回
	if (PopStack[PopStack.Num() - 1] != PanelWidget)
	{
		DDH::Debug() << PanelWidget->GetObjectName() << " Is Not Last Panel In PopPanelStack" << DDH::Endl();
		return;
	}

	//从栈中移除
	PopPanelStack.Remove(PanelWidget->GetObjectName());
	//执行隐藏函数
	PanelWidget->PanelHidden();

	//调整弹窗栈
	PopStack.Pop();
	if (PopStack.Num() > 0)
	{
		UDDPanelWidget* PrePanelWidget = PopStack[PopStack.Num() - 1];
		//转移遮罩到新的最顶层的弹窗的下一层
		TransferMask(PrePanelWidget);
		//回复被冻结的最顶层的弹窗
		PrePanelWidget->PanelResume();
	}
	else
		RemoveMaskPanel();
}

void UDDFrameWidget::ExitUIPanel(FName PanelName)
{
	//如果正在预加载但是没有加载完成,  这种情况出现在执行第一次显示或者提前加载后就马上执行销毁界面,  后期会进行完善
	if (!AllPanelGroup.Contains(PanelName) && LoadedPanelName.Contains(PanelName))
	{
		DDH::Debug() << "Do Not Exit UI Panel when Loading Panel" << DDH::Endl();
		return;
	}

	//如果这个UI面板没有加载到全部组
	if (!AllPanelGroup.Contains(PanelName))
		return;

	//获取UI面板
	UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
	//是否在显示组或者弹窗栈内
	if (!ShowPanelGroup.Contains(PanelName) && !PopPanelStack.Contains(PanelName))
	{
		AllPanelGroup.Remove(PanelName);
		LoadedPanelName.Remove(PanelName);
		//运行PanelExit生命周期, 具体的内存释放代码在该周期函数里面
		PanelWidget->PanelExit();
		//直接返回
		return;
	}

	//处理隐藏UI面板相关的流程
	switch (PanelWidget->UINature.PanelShowType)
	{
	case EPanelShowType::DoNothing:
		ExitPanelDoNothing(PanelWidget);
		break;
	case EPanelShowType::HideOther:
		ExitPanelHideOther(PanelWidget);
		break;
	case EPanelShowType::Reverse:
		ExitPanelReverse(PanelWidget);
		break;
	}
}

void UDDFrameWidget::ExitPanelDoNothing(UDDPanelWidget* PanelWidget)
{
	//从现实组, 全部组, 加载名字组移除
	ShowPanelGroup.Remove(PanelWidget->GetObjectName());
	AllPanelGroup.Remove(PanelWidget->GetObjectName());
	LoadedPanelName.Remove(PanelWidget->GetObjectName());

	//运行销毁生命周期
	PanelWidget->PanelExit();
}

void UDDFrameWidget::ExitPanelHideOther(UDDPanelWidget* PanelWidget)
{
	//从现实组, 全部组, 加载名字组移除
	ShowPanelGroup.Remove(PanelWidget->GetObjectName());
	AllPanelGroup.Remove(PanelWidget->GetObjectName());
	LoadedPanelName.Remove(PanelWidget->GetObjectName());

	//显示同一层级下的其他UI面板, 如果该面板是Level_All层级, 显示所有显示组的面板
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All || PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelDisplay();

	//运行销毁生命周期
	PanelWidget->PanelExit();

}

void UDDFrameWidget::ExitPanelReverse(UDDPanelWidget* PanelWidget)
{
	//获取弹窗栈到数组
	TArray<UDDPanelWidget*> PopStack;
	PopPanelStack.GenerateValueArray(PopStack);

	//如果不是最上层的弹窗, 直接返回
	if (PopStack[PopStack.Num() - 1] != PanelWidget)
	{
		DDH::Debug() << PanelWidget->GetObjectName() << " Is Not Last Panel In PopPanelStack" << DDH::Endl();
		return;
	}

	//从栈, 全部组, 加载名字组中移除
	PopPanelStack.Remove(PanelWidget->GetObjectName());
	AllPanelGroup.Remove(PanelWidget->GetObjectName());
	LoadedPanelName.Remove(PanelWidget->GetObjectName());
	//运行销毁生命周期函数
	PanelWidget->PanelExit();

	//调整弹窗栈
	PopStack.Pop();
	if (PopStack.Num() > 0)
	{
		UDDPanelWidget* PrePanelWidget = PopStack[PopStack.Num() - 1];
		//转移遮罩到新的最顶层的弹窗的下一层
		TransferMask(PrePanelWidget);
		//回复被冻结的最顶层的弹窗
		PrePanelWidget->PanelResume();
	}
	else
		RemoveMaskPanel();
}

void UDDFrameWidget::ActiveMask(UCanvasPanel* WorkLayout, EPanelLucenyType LucenyType)
{
	//移出遮罩
	RemoveMaskPanel(WorkLayout);

	//添加遮罩到新的父控件
	UCanvasPanelSlot* MaskSlot = WorkLayout->AddChildToCanvas(MaskPanel);
	MaskSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
	MaskSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));

	//根据透明类型设置透明度
	switch (LucenyType)
	{
	case EPanelLucenyType::Lucency:
		MaskPanel->SetVisibility(ESlateVisibility::Visible);
		MaskPanel->SetColorAndOpacity(NormalLucency);
		break;
	case EPanelLucenyType::Translucence:
		MaskPanel->SetVisibility(ESlateVisibility::Visible);
		MaskPanel->SetColorAndOpacity(TranslucenceLucency);
		break;
	case EPanelLucenyType::ImPenetrable:
		MaskPanel->SetVisibility(ESlateVisibility::Visible);
		MaskPanel->SetColorAndOpacity(ImPenetrableLucency);
		break;
	case EPanelLucenyType::Pentrate:
		MaskPanel->SetVisibility(ESlateVisibility::Hidden);
		MaskPanel->SetColorAndOpacity(NormalLucency);
		break;
	}
}

void UDDFrameWidget::ActiveMask(UOverlay* WorkLayout, EPanelLucenyType LucenyType)
{
	//移出遮罩
	RemoveMaskPanel(WorkLayout);

	//添加遮罩到新的父控件
	UOverlaySlot* MaskSlot = WorkLayout->AddChildToOverlay(MaskPanel);
	MaskSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
	MaskSlot->SetHorizontalAlignment(HAlign_Fill);
	MaskSlot->SetVerticalAlignment(VAlign_Fill);


	//根据透明类型设置透明度
	switch (LucenyType)
	{
	case EPanelLucenyType::Lucency:
		MaskPanel->SetVisibility(ESlateVisibility::Visible);
		MaskPanel->SetColorAndOpacity(NormalLucency);
		break;
	case EPanelLucenyType::Translucence:
		MaskPanel->SetVisibility(ESlateVisibility::Visible);
		MaskPanel->SetColorAndOpacity(TranslucenceLucency);
		break;
	case EPanelLucenyType::ImPenetrable:
		MaskPanel->SetVisibility(ESlateVisibility::Visible);
		MaskPanel->SetColorAndOpacity(ImPenetrableLucency);
		break;
	case EPanelLucenyType::Pentrate:
		MaskPanel->SetVisibility(ESlateVisibility::Hidden);
		MaskPanel->SetColorAndOpacity(NormalLucency);
		break;
	}
}

void UDDFrameWidget::TransferMask(UDDPanelWidget* PanelWidget)
{
	//临时保存PanelWidget以及它上层的所有UI面板
	TArray<UDDPanelWidget*> AbovePanelStack;
	//临时保存PanelWidget以及它上层的所有UI面板的布局数据
	TArray<FUINature> AboveNatureStack;

	//区分布局
	if (PanelWidget->UINature.LayoutType == ELayoutType::Canvas)
	{
		UCanvasPanel* WorkLayout = Cast<UCanvasPanel>(PanelWidget->GetParent());

		int32 StartOrder = WorkLayout->GetChildIndex(PanelWidget);
		for (int i = StartOrder; i < WorkLayout->GetChildrenCount(); ++i)
		{
			UDDPanelWidget* TempPanelWidget = Cast<UDDPanelWidget>(WorkLayout->GetChildAt(i));
			//如果不是DDPanelWidget
			if (!TempPanelWidget)
				continue;
			//保存UI面板以及布局数据
			AbovePanelStack.Push(TempPanelWidget);
			FUINature TempUINature;
			UCanvasPanelSlot* TempPanelSlot = Cast<UCanvasPanelSlot>(TempPanelWidget);
			TempUINature.Anchors = TempPanelSlot->GetAnchors();
			TempUINature.Offsets = TempPanelSlot->GetOffsets();
			AboveNatureStack.Push(TempUINature);
		}

		//循环移除上层UI面板
		for (int i = 0; i < AbovePanelStack.Num(); ++i)
			AbovePanelStack[i]->RemoveFromParent();

		//添加遮罩到新的父控件
		UCanvasPanelSlot* MaskSlot = WorkLayout->AddChildToCanvas(MaskPanel);
		MaskSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		MaskSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));

		//根据透明类型设置透明度
		switch (PanelWidget->UINature.PanelLucenyType)
		{
		case EPanelLucenyType::Lucency:
			MaskPanel->SetVisibility(ESlateVisibility::Visible);
			MaskPanel->SetColorAndOpacity(NormalLucency);
			break;
		case EPanelLucenyType::Translucence:
			MaskPanel->SetVisibility(ESlateVisibility::Visible);
			MaskPanel->SetColorAndOpacity(TranslucenceLucency);
			break;
		case EPanelLucenyType::ImPenetrable:
			MaskPanel->SetVisibility(ESlateVisibility::Visible);
			MaskPanel->SetColorAndOpacity(ImPenetrableLucency);
			break;
		case EPanelLucenyType::Pentrate:
			MaskPanel->SetVisibility(ESlateVisibility::Hidden);
			MaskPanel->SetColorAndOpacity(NormalLucency);
			break;
		}

		//把刚才移出的UI面板按顺序重新添加到布局控件
		for (int i = 0; i < AbovePanelStack.Num(); ++i)
		{
			UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(AbovePanelStack[i]);
			PanelSlot->SetAnchors(AboveNatureStack[i].Anchors);
			PanelSlot->SetOffsets(AboveNatureStack[i].Offsets);
		}
	}
	else
	{
		UOverlay* WorkLayout = Cast<UOverlay>(PanelWidget->GetParent());

		int32 StartOrder = WorkLayout->GetChildIndex(PanelWidget);
		for (int i = StartOrder; i < WorkLayout->GetChildrenCount(); ++i)
		{
			UDDPanelWidget* TempPanelWidget = Cast<UDDPanelWidget>(WorkLayout->GetChildAt(i));
			if (!TempPanelWidget)
				continue;
			//保存UI面板以及布局数据
			AbovePanelStack.Push(TempPanelWidget);
			FUINature TempUINature;
			UOverlaySlot* TempPanelSlot = Cast<UOverlaySlot>(TempPanelWidget->Slot);
			TempUINature.Offsets = TempPanelSlot->Padding;
			TempUINature.HAlign = TempPanelSlot->HorizontalAlignment;
			TempUINature.VAlign = TempPanelSlot->VerticalAlignment;
			AboveNatureStack.Push(TempUINature);
		}

		//循环移除上层UI面板
		for (int i = 0; i < AbovePanelStack.Num(); ++i)
			AbovePanelStack[i]->RemoveFromParent();

		//添加遮罩到新的父控件
		UOverlaySlot* MaskSlot = WorkLayout->AddChildToOverlay(MaskPanel);
		MaskSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
		MaskSlot->SetHorizontalAlignment(HAlign_Fill);
		MaskSlot->SetVerticalAlignment(VAlign_Fill);


		//根据透明类型设置透明度
		switch (PanelWidget->UINature.PanelLucenyType)
		{
		case EPanelLucenyType::Lucency:
			MaskPanel->SetVisibility(ESlateVisibility::Visible);
			MaskPanel->SetColorAndOpacity(NormalLucency);
			break;
		case EPanelLucenyType::Translucence:
			MaskPanel->SetVisibility(ESlateVisibility::Visible);
			MaskPanel->SetColorAndOpacity(TranslucenceLucency);
			break;
		case EPanelLucenyType::ImPenetrable:
			MaskPanel->SetVisibility(ESlateVisibility::Visible);
			MaskPanel->SetColorAndOpacity(ImPenetrableLucency);
			break;
		case EPanelLucenyType::Pentrate:
			MaskPanel->SetVisibility(ESlateVisibility::Hidden);
			MaskPanel->SetColorAndOpacity(NormalLucency);
			break;
		}

		//将UI面板填充回布局控件
		for (int i = 0; i < AbovePanelStack.Num(); ++i)
		{
			UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(AbovePanelStack[i]);
			PanelSlot->SetPadding(AboveNatureStack[i].Offsets);
			PanelSlot->SetHorizontalAlignment(AboveNatureStack[i].HAlign);
			PanelSlot->SetVerticalAlignment(AboveNatureStack[i].VAlign);
		}
	}
}

void UDDFrameWidget::RemoveMaskPanel(UPanelWidget* WorkLayout /*= NULL*/)
{
	//获取遮罩当前父控件
	UPanelWidget* MaskParent = MaskPanel->GetParent();
	if (MaskParent)
	{
		//比较当前父控件与将要插入的父控件是否相同, 当前父控件的之控件为1
		if (MaskParent != WorkLayout && MaskParent->GetChildrenCount() == 1)
		{
			MaskParent->RemoveFromParent();
			UCanvasPanel* ParentCanvas = Cast<UCanvasPanel>(MaskParent);
			UOverlay* ParentOverlay = Cast<UOverlay>(MaskParent);
			if (ParentCanvas)
			{
				ActiveCanvas.Remove(ParentCanvas);
				UnActiveCanvas.Push(ParentCanvas);
			}
			else if (ParentOverlay)
			{
				ActiveOverlay.Remove(ParentOverlay);
				UnActiveOverlay.Push(ParentOverlay);
			}
		}
		//将遮罩从父级移除
		MaskPanel->RemoveFromParent();
	}
}