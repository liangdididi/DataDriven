// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DDObject/DDUserWidget.h"
#include "DDPanelWidget.generated.h"


/**
 *
 */
UCLASS()
class DATADRIVEN_API UDDPanelWidget : public UDDUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	//Panel生命周期
	virtual void PanelEnter();  //第一次进入界面
	virtual void PanelDisplay();//显示界面
	virtual void PanelFreeze(); //在栈中被冻结
	virtual void PanelResume(); //解冻
	virtual void PanelHidden(); //隐藏, 不在栈中
	virtual void PanelExit(); //销毁, 移出界面

	//动画回调函数
	UFUNCTION(BlueprintImplementableEvent)
	float DisplayEnterMovie();
	UFUNCTION(BlueprintImplementableEvent)
	float DisplayLeaveMove();

public:

	UPROPERTY(EditAnywhere)
		FUINature UINature;

protected:

	void ShowSelfPanel();

	void HideSelfPanel();

	void ExitSelfPanel();

	void ShowUIPanel(FName PanelName);

	void HideUIPanel(FName PanelName);

	void ExitUIPanel(FName PanelName);

	//移出UI, 执行回调
	void RemoveCallBack();

	//隐藏函数
	void SetSelfHidden();

protected:

	//这个值是UIFrame框架所在的模组ID
	int32 UIFrameModuleIndex;
	//UIFrame的名字
	static FName UIFrameName;
	//显示UI的方法名
	static FName ShowUIPanelName;
	//关闭UI的方法名
	static FName HideUIPanelName;
	//销毁UI的方法名
	static FName ExitUIPanelName;
	//销毁回调函数名
	static FName ExitCallBackName;

protected:

	DDOBJFUNC_ONE(OperatorUIPanel, FName, PanelName);
	DDOBJFUNC_TWO(ExitCallBack, ELayoutType, LayoutType, UPanelWidget*, WorkLayout);

};


