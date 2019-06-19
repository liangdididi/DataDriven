// Fill out your copyright notice in the Description page of Project Settings.

#include "DDFrameWidget.h"
#include "WidgetTree.h"
#include "Overlay.h"
#include "OverlaySlot.h"
#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "DDPanelWidget.h"
#include "Image.h"



bool UDDFrameWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	//获取根节点, UE4默认都是UCanvasPanel
	RootCanvas = Cast<UCanvasPanel>(GetRootWidget());
	RootCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//生成遮罩
	MaskPanel = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());

	//设置透明度的值
	NormalLucency = FLinearColor(1.f, 1.f, 1.f, 0.f);
	TranslucenceLucency = FLinearColor(0.f, 0.f, 0.f, 0.6f);
	ImPenetrableLucency = FLinearColor(0.f, 0.f, 0.f, 0.3f);

	return true;
}


void UDDFrameWidget::ShowUIPanel(FName PanelName)
{
	//如果这个面板已经在显示或者已经在Pop栈中, 直接返回
	if (ShowPanelGroup.Contains(PanelName) || PopPanelStack.Contains(PanelName)) return;
	//如果这个对象没有在全部组中, 并且也没有进行过加载, 进行异步加载
	if (!AllPanelGroup.Contains(PanelName) && !LoadedPanelName.Contains(PanelName))
	{
		BulidSingleClassWealth(EWealthType::Widget, PanelName, "AcceptPanelWidget");
		LoadedPanelName.Push(PanelName);
		//直接返回
		return;
	}

	//如果全部组已经存在该Panel, LoadedPanelName组中一定存在, 不进行检查
	if (AllPanelGroup.Contains(PanelName))
	{
		//通过PanelWidget的父类是否存在判定Panel是否已经进入过UI
		UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
		if (PanelWidget->GetParent())
			DoShowUIPanel(PanelName);
		else
			DoEnterUIPanel(PanelName);
	}
}

void UDDFrameWidget::HideUIPanel(FName PanelName)
{
	//如果这个面板不在显示组也不在Pop栈中, 直接返回
	if (!ShowPanelGroup.Contains(PanelName) && !PopPanelStack.Contains(PanelName)) return;
	//如果这个面板没有加载, 直接返回
	if (!AllPanelGroup.Contains(PanelName)) return;
	//根据类型处理
	UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
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

void UDDFrameWidget::ExitUIPanel(FName PanelName)
{
	//如果这个面板没有加载, 直接返回
	if (!AllPanelGroup.Contains(PanelName)) return;
	//运行到这里说明在AllPanel中但, 如果这个面板不在显示组也不在Pop栈中, 直接进行数据移除并且返回
	if (!ShowPanelGroup.Contains(PanelName) && !PopPanelStack.Contains(PanelName))
	{
		//获取对象
		UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
		//移除数据并且销毁PanelWidget
		AllPanelGroup.Remove(PanelName);
		LoadedPanelName.Remove(PanelName);
		PanelWidget->PanelExit();
		return;
	}

	//根据类型处理
	UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
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

void UDDFrameWidget::AcceptPanelWidget(FName BackName, UUserWidget* BackWidget)
{
	UDDPanelWidget* PanelWidget = Cast<UDDPanelWidget>(BackWidget);

	//如果PanelWidget不是UDDPanelWidget类型
	if (!PanelWidget)
	{
		DDH::Debug() << "Load Panel : " << BackName << " Is Not DDPanelWidget" << DDH::Endl();
		return;
	}

	//注册到框架, 不注册类名
	PanelWidget->RegisterToModule(ModuleIndex, BackName);

	//添加到全部组
	AllPanelGroup.Add(BackName, PanelWidget);
	//因为是第一次加载UI, 所以执行EnterUI方法
	DoEnterUIPanel(BackName);
}



void UDDFrameWidget::DoShowUIPanel(FName PanelName)
{
	//从全部组获取对象
	UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
	//区分类型进行显示
	if (PanelWidget->UINature.LayoutType == ELayoutType::Canvas)
	{
		//获取工作Layout
		UCanvasPanel* WorkLayout = NULL;
		if (RootCanvas->GetChildrenCount() > 0)
		{
			WorkLayout = Cast<UCanvasPanel>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));
			if (!WorkLayout)
			{
				if (UnActiveCanvas.Num() == 0)
				{
					WorkLayout = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
					WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
					FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
					FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
				}
				else
					WorkLayout = UnActiveCanvas.Pop();
				ActiveCanvas.Push(WorkLayout);
			}
		}
		else
		{
			WorkLayout = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
			WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			ActiveCanvas.Push(WorkLayout);
		}

		//根据类型执行显示
		switch (PanelWidget->UINature.PanelShowType)
		{
		case EPanelShowType::DoNothing:
			ShowPanelDoNothing(PanelWidget);
			return;
		case EPanelShowType::HideOther:
			ShowPanelHideOther(PanelWidget);
			return;
		case EPanelShowType::Reverse:
			ShowPanelReverse(PanelWidget);
			return;
		}
	}
	else if (PanelWidget->UINature.LayoutType == ELayoutType::Overlay)
	{
		UOverlay* WorkLayout = NULL;
		if (RootCanvas->GetChildrenCount() > 0)
		{
			WorkLayout = Cast<UOverlay>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));
			if (!WorkLayout)
			{
				if (UnActiveOverlay.Num() == 0)
				{
					WorkLayout = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
					WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
					FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
					FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
				}
				else
					WorkLayout = UnActiveOverlay.Pop();
				ActiveOverlay.Push(WorkLayout);
			}
		}
		else
		{
			WorkLayout = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
			WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			ActiveOverlay.Push(WorkLayout);
		}

		//根据类型执行显示
		switch (PanelWidget->UINature.PanelShowType)
		{
		case EPanelShowType::DoNothing:
			ShowPanelDoNothing(PanelWidget);
			return;
		case EPanelShowType::HideOther:
			ShowPanelHideOther(PanelWidget);
			return;
		case EPanelShowType::Reverse:
			ShowPanelReverse(PanelWidget);
			return;
		}
	}
}

void UDDFrameWidget::AdvanceLoadPanel(FName PanelName)
{
	//如果已经在全部组内了, 直接返回
	if (AllPanelGroup.Contains(PanelName) && LoadedPanelName.Contains(PanelName)) return;
	//进行异步加载
	BulidSingleClassWealth(EWealthType::Widget, PanelName, "AcceptAdvancePanel");
	LoadedPanelName.Push(PanelName);
}

void UDDFrameWidget::AcceptAdvancePanel(FName BackName, UUserWidget* BackWidget)
{
	UDDPanelWidget* PanelWidget = Cast<UDDPanelWidget>(BackWidget);

	//如果PanelWidget不是UDDPanelWidget类型
	if (!PanelWidget)
	{
		DDH::Debug() << "Load Panel : " << BackName << " Is Not DDPanelWidget" << DDH::Endl();
		return;
	}

	//注册到框架, 不注册类名
	PanelWidget->RegisterToModule(ModuleIndex, BackName);

	//添加到全部组, 但是不进行显示
	AllPanelGroup.Add(BackName, PanelWidget);
}

void UDDFrameWidget::DoEnterUIPanel(FName PanelName)
{
	//从全部组获取对象
	UDDPanelWidget* PanelWidget = *AllPanelGroup.Find(PanelName);
	//区分类型进行显示
	if (PanelWidget->UINature.LayoutType == ELayoutType::Canvas)
	{
		//获取工作Layout
		UCanvasPanel* WorkLayout = NULL;
		if (RootCanvas->GetChildrenCount() > 0)
		{
			WorkLayout = Cast<UCanvasPanel>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));
			if (!WorkLayout)
			{
				if (UnActiveCanvas.Num() == 0)
				{
					WorkLayout = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
					WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
					FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
					FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
				}
				else
					WorkLayout = UnActiveCanvas.Pop();
				ActiveCanvas.Push(WorkLayout);
			}
		}
		else
		{
			WorkLayout = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
			WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			ActiveCanvas.Push(WorkLayout);
		}

		//根据类型执行显示
		switch (PanelWidget->UINature.PanelShowType)
		{
		case EPanelShowType::DoNothing:
			EnterPanelDoNothing(WorkLayout, PanelWidget);
			return;
		case EPanelShowType::HideOther:
			EnterPanelHideOther(WorkLayout, PanelWidget);
			return;
		case EPanelShowType::Reverse:
			EnterPanelReverse(WorkLayout, PanelWidget);
			return;
		}
	}
	else
	{
		UOverlay* WorkLayout = NULL;
		if (RootCanvas->GetChildrenCount() > 0)
		{
			WorkLayout = Cast<UOverlay>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));
			if (!WorkLayout)
			{
				if (UnActiveOverlay.Num() == 0)
				{
					WorkLayout = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
					WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
					FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
					FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
				}
				else
					WorkLayout = UnActiveOverlay.Pop();
				ActiveOverlay.Push(WorkLayout);
			}
		}
		else
		{
			WorkLayout = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
			WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			ActiveOverlay.Push(WorkLayout);
		}

		//根据类型执行显示
		switch (PanelWidget->UINature.PanelShowType)
		{
		case EPanelShowType::DoNothing:
			EnterPanelDoNothing(WorkLayout, PanelWidget);
			return;
		case EPanelShowType::HideOther:
			EnterPanelHideOther(WorkLayout, PanelWidget);
			return;
		case EPanelShowType::Reverse:
			EnterPanelReverse(WorkLayout, PanelWidget);
			return;
		}
	}
}

void UDDFrameWidget::EnterPanelDoNothing(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//添加Panel到Layout
	UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(PanelWidget);
	PanelSlot->SetAnchors(PanelWidget->UINature.Anchors);
	PanelSlot->SetOffsets(PanelWidget->UINature.Offsets);

	//添加Panel显示组, 本框架强制PanelName必须和ObjectName一致
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelDoNothing(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//添加Panel到Layout
	UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(PanelWidget);
	PanelSlot->SetPadding(PanelWidget->UINature.Offsets);
	PanelSlot->SetHorizontalAlignment(PanelWidget->UINature.HAlign);
	PanelSlot->SetVerticalAlignment(PanelWidget->UINature.VAlign);

	//添加Panel到显示组, 本框架强制PanelName必须和ObjectName一致
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelHideOther(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//将显示组的同一层级的其他对象都隐藏, 如果是Level_All就全部隐藏, Level_All优先级高
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All)
			It.Value()->PanelHidden();
		else if (PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelHidden();

	//添加Panel到Layout
	UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(PanelWidget);
	PanelSlot->SetAnchors(PanelWidget->UINature.Anchors);
	PanelSlot->SetOffsets(PanelWidget->UINature.Offsets);

	//添加到显示组并且显示
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelHideOther(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//将显示组的同一层级的其他对象都隐藏, 如果是Level_All就全部隐藏, Level_All优先级高
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All)
			It.Value()->PanelHidden();
		else if (PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelHidden();

	//添加Panel到Layout
	UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(PanelWidget);
	PanelSlot->SetPadding(PanelWidget->UINature.Offsets);
	PanelSlot->SetHorizontalAlignment(PanelWidget->UINature.HAlign);
	PanelSlot->SetVerticalAlignment(PanelWidget->UINature.VAlign);

	//添加到显示组并且显示
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::EnterPanelReverse(UCanvasPanel* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//判断是Pop栈内是否有对象, 有的话就将最后一个冻结
	if (PopPanelStack.Num() > 0)
	{
		TArray<UDDPanelWidget*> PanelStack;
		PopPanelStack.GenerateValueArray(PanelStack);
		PanelStack[PanelStack.Num() - 1]->PanelFreeze();
	}

	//激活遮罩
	ActiveMask(WorkLayout, PanelWidget->UINature.PanelLucenyType);

	//添加Panel到Layout
	UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(PanelWidget);
	PanelSlot->SetAnchors(PanelWidget->UINature.Anchors);
	PanelSlot->SetOffsets(PanelWidget->UINature.Offsets);

	//添加Panel到栈并且显示
	PopPanelStack.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelEnter();

}

void UDDFrameWidget::EnterPanelReverse(UOverlay* WorkLayout, UDDPanelWidget* PanelWidget)
{
	//判断是Pop栈内是否有对象, 有的话就将最后一个冻结
	if (PopPanelStack.Num() > 0)
	{
		TArray<UDDPanelWidget*> PanelStack;
		PopPanelStack.GenerateValueArray(PanelStack);
		PanelStack[PanelStack.Num() - 1]->PanelFreeze();
	}

	//激活遮罩
	ActiveMask(WorkLayout, PanelWidget->UINature.PanelLucenyType);

	//添加Panel到Layout
	UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(PanelWidget);
	PanelSlot->SetPadding(PanelWidget->UINature.Offsets);
	PanelSlot->SetHorizontalAlignment(PanelWidget->UINature.HAlign);
	PanelSlot->SetVerticalAlignment(PanelWidget->UINature.VAlign);

	//添加Panel到栈并且显示
	PopPanelStack.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelEnter();
}

void UDDFrameWidget::ShowPanelDoNothing(UDDPanelWidget* PanelWidget)
{
	//添加Panel到显示组
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelDisplay();
}

void UDDFrameWidget::ShowPanelHideOther(UDDPanelWidget* PanelWidget)
{
	//将显示组的同一层级的其他对象都隐藏, 如果是Level_All就隐藏所有
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All)
			It.Value()->PanelHidden();
		else if (PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelHidden();

	//添加Panel到显示组
	ShowPanelGroup.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelDisplay();
}

void UDDFrameWidget::ShowPanelReverse(UDDPanelWidget* PanelWidget)
{
	//判断是Pop栈内是否有对象, 有的话就将最后一个冻结
	if (PopPanelStack.Num() > 0)
	{
		TArray<UDDPanelWidget*> PanelStack;
		PopPanelStack.GenerateValueArray(PanelStack);
		PanelStack[PanelStack.Num() - 1]->PanelFreeze();
	}

	//弹窗对象必须从原来的父级移除, 然后再次添加
	if (PanelWidget->UINature.LayoutType == ELayoutType::Canvas)
	{
		UCanvasPanel* PreWorkLayout = Cast<UCanvasPanel>(PanelWidget->GetParent());
		UCanvasPanelSlot* PrePanelSlot = Cast<UCanvasPanelSlot>(PanelWidget->Slot);
		FAnchors PreAnchors = PrePanelSlot->GetAnchors();
		FMargin PreOffsets = PrePanelSlot->GetOffsets();

		//将PanelWidget从父类移除
		PanelWidget->RemoveFromParent();
		//处理这个父类的资源
		if (PreWorkLayout->GetChildrenCount() == 0)
		{
			PreWorkLayout->RemoveFromParent();
			ActiveCanvas.Remove(PreWorkLayout);
			UnActiveCanvas.Push(PreWorkLayout);
		}

		//寻找一个能用的WorkLayout
		UCanvasPanel* WorkLayout = NULL;
		if (RootCanvas->GetChildrenCount() > 0)
		{
			WorkLayout = Cast<UCanvasPanel>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));
			if (!WorkLayout)
			{
				if (UnActiveCanvas.Num() == 0)
				{
					WorkLayout = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
					WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
					FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
					FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
				}
				else
					WorkLayout = UnActiveCanvas.Pop();
				ActiveCanvas.Push(WorkLayout);
			}
		}
		else
		{
			WorkLayout = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
			WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			ActiveCanvas.Push(WorkLayout);
		}

		//激活Mask到获取到的WorkLayout
		ActiveMask(WorkLayout, PanelWidget->UINature.PanelLucenyType);

		//把PanelWidget添加到WorkLayout
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

		//将PanelWidget从父类移除
		PanelWidget->RemoveFromParent();
		//处理这个父类的资源
		if (PreWorkLayout->GetChildrenCount() == 0)
		{
			PreWorkLayout->RemoveFromParent();
			ActiveOverlay.Remove(PreWorkLayout);
			UnActiveOverlay.Push(PreWorkLayout);
		}

		//寻找一个能用的WorkLayout
		UOverlay* WorkLayout = NULL;
		if (RootCanvas->GetChildrenCount() > 0)
		{
			WorkLayout = Cast<UOverlay>(RootCanvas->GetChildAt(RootCanvas->GetChildrenCount() - 1));
			if (!WorkLayout)
			{
				if (UnActiveOverlay.Num() == 0)
				{
					WorkLayout = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
					WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
					FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
					FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
				}
				else
					WorkLayout = UnActiveOverlay.Pop();
				ActiveOverlay.Push(WorkLayout);
			}
		}
		else
		{
			WorkLayout = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
			WorkLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UCanvasPanelSlot* FrameCanvasSlot = RootCanvas->AddChildToCanvas(WorkLayout);
			FrameCanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			FrameCanvasSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
			ActiveOverlay.Push(WorkLayout);
		}

		//激活Mask到获取到的WorkLayout
		ActiveMask(WorkLayout, PanelWidget->UINature.PanelLucenyType);

		//把PanelWidget添加到WorkLayout
		UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(PanelWidget);
		PanelSlot->SetPadding(PrePadding);
		PanelSlot->SetHorizontalAlignment(PreHAlign);
		PanelSlot->SetVerticalAlignment(PreVAlign);
	}

	//添加Panel到栈并且显示
	PopPanelStack.Add(PanelWidget->GetObjectName(), PanelWidget);
	PanelWidget->PanelDisplay();
}


void UDDFrameWidget::HidePanelDoNothing(UDDPanelWidget* PanelWidget)
{
	//从显示组移除
	ShowPanelGroup.Remove(PanelWidget->GetObjectName());
	//运行隐藏函数
	PanelWidget->PanelHidden();
}

void UDDFrameWidget::HidePanelHideOther(UDDPanelWidget* PanelWidget)
{
	//从显示组移除
	ShowPanelGroup.Remove(PanelWidget->GetObjectName());

	//显示同一层级下的其他Panel, 如果是Level_All就全部显示
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All)
			It.Value()->PanelDisplay();
		else if (PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelDisplay();

	//运行隐藏函数
	PanelWidget->PanelHidden();
}

void UDDFrameWidget::HidePanelReverse(UDDPanelWidget* PanelWidget)
{
	TArray<UDDPanelWidget*> PopStack;
	PopPanelStack.GenerateValueArray(PopStack);
	//如果最后一个不是这个PanelWidget, 提交错误
	if (PopStack[PopStack.Num() - 1] != PanelWidget)
	{
		DDH::Debug() << PanelWidget->GetObjectName() << " Is Not Last Panel In PopPanelStack" << DDH::Endl();
		return;
	}

	//从PopPanelStack移除
	PopPanelStack.Remove(PanelWidget->GetObjectName());
	//执行隐藏
	PanelWidget->PanelHidden();

	//调整栈
	PopStack.Pop();
	if (PopStack.Num() > 0)
	{
		UDDPanelWidget* PrePanelWidget = PopStack[PopStack.Num() - 1];
		//转移MaskPanel到PrePanelWidget的上一级
		TransferMask(PrePanelWidget);
		PrePanelWidget->PanelResume();
	}
	else
		RemoveMaskPanel();
}

void UDDFrameWidget::ExitPanelDoNothing(UDDPanelWidget* PanelWidget)
{
	//从显示组和All组以及加载组移除
	ShowPanelGroup.Remove(PanelWidget->GetObjectName());
	AllPanelGroup.Remove(PanelWidget->GetObjectName());
	LoadedPanelName.Remove(PanelWidget->GetObjectName());
	//执行销毁
	PanelWidget->PanelExit();
}

void UDDFrameWidget::ExitPanelHideOther(UDDPanelWidget* PanelWidget)
{
	//从显示组和All组以及加载组移除
	ShowPanelGroup.Remove(PanelWidget->GetObjectName());
	AllPanelGroup.Remove(PanelWidget->GetObjectName());
	LoadedPanelName.Remove(PanelWidget->GetObjectName());

	//重新显示同一层级的其他Panel, 如果是Level_All就全部显示
	for (TMap<FName, UDDPanelWidget*>::TIterator It(ShowPanelGroup); It; ++It)
		if (PanelWidget->UINature.LayoutLevel == ELayoutLevel::Level_All)
			It.Value()->PanelDisplay();
		else if (PanelWidget->UINature.LayoutLevel == It.Value()->UINature.LayoutLevel)
			It.Value()->PanelDisplay();

	//显示完其他组件后再对PanelWidget执行销毁
	PanelWidget->PanelExit();
}

void UDDFrameWidget::ExitPanelReverse(UDDPanelWidget* PanelWidget)
{
	TArray<UDDPanelWidget*> PopStack;
	PopPanelStack.GenerateValueArray(PopStack);
	//如果最后一个不是这个PanelWidget, 提交错误
	if (PopStack[PopStack.Num() - 1] != PanelWidget)
	{
		DDH::Debug() << PanelWidget->GetObjectName() << " Is Not Last Panel In PopPanelStack" << DDH::Endl();
		return;
	}

	AllPanelGroup.Remove(PanelWidget->GetObjectName());
	PopPanelStack.Remove(PanelWidget->GetObjectName());
	LoadedPanelName.Remove(PanelWidget->GetObjectName());
	PopStack.Pop();
	//执行销毁
	PanelWidget->PanelExit();

	//处理遮罩与栈
	if (PopStack.Num() > 0)
	{
		UDDPanelWidget* PrePanelWidget = PopStack[PopStack.Num() - 1];
		//转移MaskPanel到PrePanelWidget的上一级
		TransferMask(PrePanelWidget);
		PrePanelWidget->PanelResume();
	}
	else
		RemoveMaskPanel();
}

void UDDFrameWidget::ActiveMask(UCanvasPanel* WorkLayout, EPanelLucenyType LucenyType)
{
	//移出遮罩
	RemoveMaskPanel(WorkLayout);

	//添加Mask到Layout
	UCanvasPanelSlot* MaskSlot = WorkLayout->AddChildToCanvas(MaskPanel);
	MaskSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
	MaskSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));

	//根据LucenyType设置Mask的属性
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

	//添加Mask到Layout
	UOverlaySlot* MaskSlot = WorkLayout->AddChildToOverlay(MaskPanel);
	MaskSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
	MaskSlot->SetHorizontalAlignment(HAlign_Fill);
	MaskSlot->SetVerticalAlignment(VAlign_Fill);

	//根据LucenyType设置Mask的属性
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
	//临时保存PanelWidget以及上层的Panel
	TArray<UDDPanelWidget*> AbovePanelStack;
	//临时保存PanelWidget以及上层的Panel的数据
	TArray<FUINature> AboveNatureStack;

	if (PanelWidget->UINature.LayoutType == ELayoutType::Canvas)
	{
		UCanvasPanel* WorkLayout = Cast<UCanvasPanel>(PanelWidget->GetParent());

		int32 StartOrder = WorkLayout->GetChildIndex(PanelWidget);
		for (int i = StartOrder; i < WorkLayout->GetChildrenCount(); ++i)
		{
			UDDPanelWidget* TempPanelWidget = Cast<UDDPanelWidget>(WorkLayout->GetChildAt(i));
			//如果不是UDDPanelWidget类型就跳到下一个循环
			if (!TempPanelWidget) continue;
			AbovePanelStack.Push(TempPanelWidget);
			FUINature TempUINature;
			UCanvasPanelSlot* TempPanelSlot = Cast<UCanvasPanelSlot>(TempPanelWidget->Slot);
			TempUINature.Anchors = TempPanelSlot->GetAnchors();
			TempUINature.Offsets = TempPanelSlot->GetOffsets();
			AboveNatureStack.Push(TempUINature);
		}

		//循环移除Panel
		for (int i = AbovePanelStack.Num() - 1; i >= 0; --i)
			AbovePanelStack[i]->RemoveFromParent();

		//添加遮罩
		UCanvasPanelSlot* MaskSlot = WorkLayout->AddChildToCanvas(MaskPanel);
		MaskSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		MaskSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));

		//设置透明度
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

		//将Panel栈填充回WorkLayout
		for (int i = 0; i < AbovePanelStack.Num(); ++i)
		{
			UCanvasPanelSlot* PanelSlot = WorkLayout->AddChildToCanvas(AbovePanelStack[i]);
			PanelSlot->SetAnchors(AboveNatureStack[i].Anchors);
			PanelSlot->SetOffsets(AboveNatureStack[i].Offsets);
		}
	}
	else if (PanelWidget->UINature.LayoutType == ELayoutType::Overlay)
	{
		UOverlay* WorkLayout = Cast<UOverlay>(PanelWidget->GetParent());

		int32 StartOrder = WorkLayout->GetChildIndex(PanelWidget);
		for (int i = StartOrder; i < WorkLayout->GetChildrenCount(); ++i)
		{
			UDDPanelWidget* TempPanelWidget = Cast<UDDPanelWidget>(WorkLayout->GetChildAt(i));
			//如果不是UDDPanelWidget类型就跳到下一个循环
			if (!TempPanelWidget) continue;
			AbovePanelStack.Push(TempPanelWidget);
			FUINature TempUINature;
			UOverlaySlot* TempPanelSlot = Cast<UOverlaySlot>(TempPanelWidget->Slot);
			TempUINature.Offsets = TempPanelSlot->Padding;
			TempUINature.HAlign = TempPanelSlot->HorizontalAlignment;
			TempUINature.VAlign = TempPanelSlot->VerticalAlignment;
			AboveNatureStack.Push(TempUINature);
		}

		//循环移除Panel
		for (int i = AbovePanelStack.Num() - 1; i >= 0; --i)
			AbovePanelStack[i]->RemoveFromParent();

		//将Mask移入WorkLayout
		UOverlaySlot* MaskSlot = WorkLayout->AddChildToOverlay(MaskPanel);
		MaskSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
		MaskSlot->SetHorizontalAlignment(HAlign_Fill);
		MaskSlot->SetVerticalAlignment(VAlign_Fill);

		//设置透明度
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

		//将Panel栈填充回WorkLayout
		for (int i = 0; i < AbovePanelStack.Num(); ++i)
		{
			UOverlaySlot* PanelSlot = WorkLayout->AddChildToOverlay(AbovePanelStack[i]);
			PanelSlot->SetPadding(AboveNatureStack[i].Offsets);
			PanelSlot->SetHorizontalAlignment(AboveNatureStack[i].HAlign);
			PanelSlot->SetVerticalAlignment(AboveNatureStack[i].VAlign);
		}
	}
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

void UDDFrameWidget::RemoveMaskPanel(UPanelWidget* WorkLayout)
{
	//处理遮罩的父级
	UPanelWidget* MaskParent = MaskPanel->GetParent();
	if (MaskParent)
	{
		//如果Mask的父级与即将添加Mask的父级相同的话就不用去移除父级了
		if (MaskParent->GetChildrenCount() == 1 && MaskParent != WorkLayout)
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

