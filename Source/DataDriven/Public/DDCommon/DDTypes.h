// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/GameEngine.h"
#include "Engine/DataAsset.h"
#include "Anchors.h"

#include "DDTypes.generated.h"

/**
 *
 */
UCLASS()
class DATADRIVEN_API UDDTypes : public UObject
{
	GENERATED_BODY()

};






//框架自定义Debug输出类
#pragma region LogDebug

class DATADRIVEN_API DDRecord
{
private:
	//自身单例
	static TSharedPtr<DDRecord> RecordInst;

	//生成的字符串
	FString RecordInfo;

	//显示时间
	float ShowTime;

	//显示颜色
	FColor ShowColor;

public:

	//打印模式, 0 : Debug, 1 : Log, 2 : Warning, 3 : Error
	uint8 PatternID;

public:

	//构造函数
	inline DDRecord() {}

	~DDRecord() {}

	static TSharedPtr<DDRecord> Get();

	//设定参数
	inline void InitParam(float InTime, FColor InColor)
	{
		ShowTime = InTime;
		ShowColor = InColor;
	}

	//执行打印方法
	inline void Output() {
		switch (PatternID)
		{
		case 0:
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, ShowTime, ShowColor, RecordInfo);
		}
		break;
		case 1:
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *RecordInfo);
		}
		break;
		case 2:
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *RecordInfo);
		}
		break;
		case 3:
		{
			UE_LOG(LogTemp, Error, TEXT("%s"), *RecordInfo);
		}
		break;
		}
		//清空字符串
		RecordInfo.Empty();
	}

	//重写操作符<<
	inline DDRecord& operator<<(FString Info) { RecordInfo.Append(Info); return *this; }
	inline DDRecord& operator<<(FName Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FText Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(const char* Info) { RecordInfo += Info; return *this; }
	inline DDRecord& operator<<(const char Info) { RecordInfo.AppendChar(Info); return *this; }
	inline DDRecord& operator<<(int32 Info) { RecordInfo.Append(FString::FromInt(Info)); return *this; }
	inline DDRecord& operator<<(float Info) { RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }
	inline DDRecord& operator<<(double Info) { RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }
	inline DDRecord& operator<<(bool Info) { RecordInfo.Append(Info ? FString("true") : FString("false")); return *this; }
	inline DDRecord& operator<<(FVector2D Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FVector Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FRotator Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FQuat Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FTransform Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FMatrix Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FColor Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FLinearColor Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord& operator<<(FMargin Info) { RecordInfo.Append((FString::SanitizeFloat(Info.Left) + FString(" , ") + FString::SanitizeFloat(Info.Top) + FString(" , ") + FString::SanitizeFloat(Info.Right) + FString(" , ") + FString::SanitizeFloat(Info.Bottom))); return *this; }

	//输出
	inline void operator<<(DDRecord& Record) { Record.Output(); }
};


//DDRecord::Get() << FString("sfdewhe") << FName << int << float << DDRecord::Get();


#pragma endregion


#pragma region LifeTimePart

//BaseObject生命周期
UENUM()
enum class EBaseObjectLife : uint8
{
	None = 0, //初始化状态
	Init,
	Loading,
	Register,
	Enable,
	Disable,
	UnRegister,
	UnLoading
};


//BaseObject生命周期状态
UENUM()
enum class EBaseObjectState : uint8
{
	Active = 0, //激活
	Stable,     //稳定
	Destroy     //销毁
};

#pragma endregion


#pragma region ReflectPart

//对象调用协议
UENUM()
enum class EAgreementType : uint8
{
	SelfObject,        //给传入的对象通信
	OtherObject,       //给传入的对象之外的对象通信
	ClassOtherObject,  //给传入的对象的相同类的其他对象通信,调用这个方法要确保传过来的对象都是同一类的,如果不同类就多次通信
	SelfClass,         //给这个类的对象通信
	OtherClass,        //给这个类之外的类通信
	All                //给所有的对象通信
};


//调用结果,项目开发时请确保每次都能调用成功
UENUM()
enum class ECallResult : uint8
{
	NoModule = 0, //缺失模组
	LackObject,   //缺失部分对象
	NoFunction,   //缺失方法
	Succeed       //调用成功
};

//通信参数结构体基类
struct DDParam
{
public:

	//调用结果
	ECallResult CallResult;

	//参数指针
	void* ParamPtr;
};

//通信协议, Module方法
struct DDModuleAgreement
{
public:

	//模组ID
	int32 ModuleIndex;

	//方法名
	FName FunctionName;

};

//通信协议, DDOO方法
struct DDObjectAgreement
{
public:

	//模组ID
	int32 ModuleIndex;

	//协议类型
	EAgreementType AgreementType;

	//对象组名
	TArray<FName> ObjectGroup;

	//方法名
	FName FunctionName;
};



#pragma endregion


#pragma region DDAnyFun

//存储任意类型方法结构体
struct DDAnyFun
{
	struct BaseFun
	{
	public:
		virtual ~BaseFun() {}
	};
	template<typename RetType, typename... VarTypes>
	struct ValFun : public BaseFun
	{
	public:
		TFunction<RetType(VarTypes...)> TarFun;
		ValFun(const TFunction<RetType(VarTypes...)> InsFun) : TarFun(InsFun) {}
		RetType Execute(VarTypes... Params)
		{
			return TarFun(Params...);
		}
	};
	BaseFun* FunPtr;
public:
	DDAnyFun() : FunPtr(NULL) {}
	template<typename RetType, typename... VarTypes>
	DDAnyFun(const TFunction<RetType(VarTypes...)> InsFun) : FunPtr(new ValFun<RetType, VarTypes...>(InsFun)) {}
	~DDAnyFun() { delete FunPtr; }
	template<typename RetType, typename... VarTypes>
	RetType Execute(VarTypes... Params)
	{
		ValFun<RetType, VarTypes...>* SubFunPtr = static_cast<ValFun<RetType, VarTypes...>*>(FunPtr);
		return SubFunPtr->Execute(Params...);
	}
	template<typename RetType, typename... VarTypes>
	TFunction<RetType(VarTypes...)>& GetFun()
	{
		ValFun<RetType, VarTypes...>* SubFunPtr = static_cast<ValFun<RetType, VarTypes...>*>(FunPtr);
		return SubFunPtr->TarFun;
	}
};

#pragma endregion

#pragma region DDMsgNode

//事件节点
struct DDMsgNode
{
	//被调用的接口数量
	int32 CallCount;
	//方法列表
	TMap<int32, DDAnyFun*> FunQuene;
	//注册方法
	template<typename RetType, typename... VarTypes>
	int32 RegisterFun(TFunction<RetType(VarTypes...)> InsFun);
	//注销方法
	void UnRegisterFun(int32 FunID)
	{
		//从列表移除对象
		DDAnyFun* DesPtr = *FunQuene.Find(FunID);
		FunQuene.Remove(FunID);
		delete DesPtr;
	}
	//清空节点
	void ClearNode()
	{
		for (TMap<int32, DDAnyFun*>::TIterator It(FunQuene); It; ++It)
		{
			delete It.Value();
		}
	}
	//执行方法
	template<typename RetType, typename... VarTypes>
	RetType Execute(VarTypes... Params);
	//判断是否有绑定的函数
	bool IsBound() { return FunQuene.Num() > 0; }
	//如果有绑定函数就去执行
	template<typename RetType, typename... VarTypes>
	bool ExecuteIfBound(VarTypes... Params);
	//构造函数, 初始化CallCount为0
	DDMsgNode() : CallCount(0) {}
};

template<typename RetType, typename... VarTypes>
int32 DDMsgNode::RegisterFun(TFunction<RetType(VarTypes...)> InsFun)
{
	//获取方法序列里的所有下标
	TArray<int32> FunKeyQuene;
	FunQuene.GenerateKeyArray(FunKeyQuene);
	//获取新下标
	int32 NewID;
	for (int32 i = FunKeyQuene.Num(); i >= 0; --i)
	{
		if (!FunKeyQuene.Contains(i))
		{
			NewID = i;
			break;
		}
	}
	//将新方法添加到节点
	FunQuene.Add(NewID, new DDAnyFun(InsFun));
	return NewID;
}

template<typename RetType, typename... VarTypes>
RetType DDMsgNode::Execute(VarTypes... Params)
{
	//遍历执行第二个到最后一个方法
	TMap<int32, DDAnyFun*>::TIterator It(FunQuene);
	++It;
	for (; It; ++It)
	{
		It.Value()->Execute<RetType, VarTypes...>(Params...);
	}
	//获取序列第一个方法
	TMap<int32, DDAnyFun*>::TIterator IBegin(FunQuene);
	return IBegin.Value()->Execute<RetType, VarTypes...>(Params...);
}

template<typename RetType, typename... VarTypes>
bool DDMsgNode::ExecuteIfBound(VarTypes... Params)
{
	if (!IsBound()) return false;
	for (TMap<int32, DDAnyFun*>::TIterator It(FunQuene); It; ++It)
	{
		It.Value()->Execute<RetType, VarTypes...>(Params...);
	}
	return true;
}

#pragma endregion


#pragma region DDCallHandle

struct DDMsgQuene;

//调用句柄
template<typename RetType, typename... VarTypes>
struct DDCallHandle
{
	//事件队列
	DDMsgQuene* MsgQuene;
	//节点名, 调用名
	FName CallName;
	//是否有效, 用于重写等于操作符保存状态
	TSharedPtr<bool> IsActived;
	//执行方法
	RetType Execute(VarTypes... Params);
	//是否已经绑定
	bool IsBound();
	//如果绑定就执行
	bool ExecuteIfBound(VarTypes... Params);
	//注销调用接口
	void UnRegister();
	//无参构造函数
	DDCallHandle() {}
	//构造函数
	DDCallHandle(DDMsgQuene* MQ, FName CN) {
		MsgQuene = MQ;
		CallName = CN;
		//构建时状态为激活状态
		IsActived = MakeShareable<bool>(new bool(true));
	}
	//重写操作符
	DDCallHandle<RetType, VarTypes...>& operator=(const DDCallHandle<RetType, VarTypes...>& Other)
	{
		if (this == &Other)
			return *this;
		MsgQuene = Other.MsgQuene;
		CallName = Other.CallName;
		IsActived = Other.IsActived;
		return *this;
	}
};


template<typename RetType, typename... VarTypes>
void DDCallHandle<RetType, VarTypes...>::UnRegister()
{
	if (*IsActived.Get())
		MsgQuene->UnRegisterCallPort(CallName);
	*IsActived.Get() = false;
}

template<typename RetType, typename... VarTypes>
bool DDCallHandle<RetType, VarTypes...>::ExecuteIfBound(VarTypes... Params)
{
	if (!IsBound() || !*IsActived.Get())
		return false;
	MsgQuene->Execute<RetType, VarTypes...>(CallName, Params...);
	return true;
}

template<typename RetType, typename... VarTypes>
bool DDCallHandle<RetType, VarTypes...>::IsBound()
{
	if (!*IsActived.Get())
		return false;
	return MsgQuene->IsBound(CallName);
}

//运行Execute前必须运行IsBound()函数判断是否有绑定
template<typename RetType, typename... VarTypes>
RetType DDCallHandle<RetType, VarTypes...>::Execute(VarTypes... Params)
{
	return MsgQuene->Execute<RetType, VarTypes...>(CallName, Params...);
}

#pragma endregion

#pragma region DDMsgQuene

struct DDFunHandle;

//事件队列
struct DDMsgQuene
{
	//节点序列
	TMap<FName, DDMsgNode> MsgQuene;
	//注册调用接口
	template<typename RetType, typename... VarTypes>
	DDCallHandle<RetType, VarTypes...> RegisterCallPort(FName CallName);
	//注册方法接口
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun);
	//注销调用接口
	void UnRegisterCallPort(FName CallName)
	{
		if (!MsgQuene.Contains(CallName))
			return;
		//获取事件节点
		DDMsgNode* MsgNode = MsgQuene.Find(CallName);
		//让对应的节点调用计数器减一, 如果计数器小于等于0, 就移除调用接口
		MsgNode->CallCount--;
		if (MsgNode->CallCount <= 0)
		{
			MsgNode->ClearNode();
			MsgQuene.Remove(CallName);
		}
	}
	//注销方法接口
	void UnRegisterFunPort(FName CallName, int32 FunID)
	{
		if (MsgQuene.Contains(CallName))
			MsgQuene.Find(CallName)->UnRegisterFun(FunID);
	}
	//执行方法接口
	template<typename RetType, typename... VarTypes>
	RetType Execute(FName CallName, VarTypes... Params);
	//是否已经绑定方法
	bool IsBound(FName CallName) { return MsgQuene.Find(CallName)->IsBound(); }
};

template<typename RetType, typename... VarTypes>
DDCallHandle<RetType, VarTypes...>
DDMsgQuene::RegisterCallPort(FName CallName)
{
	//如果已经存在对应CallName的调用接口, 就把调用计数器+1
	if (MsgQuene.Contains(CallName))
	{
		MsgQuene.Find(CallName)->CallCount++;
	}
	else
	{
		//创建新的事件节点并且添加到队列
		MsgQuene.Add(CallName, DDMsgNode());
		//计数器加1
		MsgQuene.Find(CallName)->CallCount++;
	}
	//返回调用句柄
	return DDCallHandle<RetType, VarTypes...>(this, CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle DDMsgQuene::RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	//获取新的方法下标
	int32 FunID;
	//如果不存在CallName对应的节点
	if (!MsgQuene.Contains(CallName))
	{
		//创建新的事件节点并且添加到队列
		MsgQuene.Add(CallName, DDMsgNode());
	}
	//直接将新的方法注册到节点
	FunID = MsgQuene.Find(CallName)->RegisterFun(InsFun);
	//返回方法句柄
	return DDFunHandle(this, CallName, FunID);
}

template<typename RetType, typename... VarTypes>
RetType DDMsgQuene::Execute(FName CallName, VarTypes... Params)
{
	return MsgQuene.Find(CallName)->Execute<RetType, VarTypes...>(Params...);
}

#pragma endregion

#pragma region DDFunHandle

//方法句柄
struct DDFunHandle
{
	//消息队列
	DDMsgQuene* MsgQuene;
	//调用名字
	FName CallName;
	//方法ID
	int32 FunID;
	//是否有效
	TSharedPtr<bool> IsActived;
	//注销方法
	void UnRegister()
	{
		if (*IsActived.Get())
			MsgQuene->UnRegisterFunPort(CallName, FunID);
		//设置失活
		*IsActived.Get() = false;
	}
	//无参构造函数
	DDFunHandle() {}
	//有参构造函数
	DDFunHandle(DDMsgQuene* MQ, FName CN, int32 FI)
	{
		MsgQuene = MQ;
		CallName = CN;
		FunID = FI;
		//设置状态为激活
		IsActived = MakeShareable<bool>(new bool(true));
	}
	//重写=操作符
	DDFunHandle& operator=(const DDFunHandle& Other)
	{
		if (this == &Other)
			return *this;
		MsgQuene = Other.MsgQuene;
		CallName = Other.CallName;
		FunID = Other.FunID;
		IsActived = Other.IsActived;
		return *this;
	}
};

#pragma endregion


#pragma region Coroutine

//判断条件委托
DECLARE_DELEGATE_RetVal(bool, FCoroCondition)

struct DDCoroNode
{
	//激活状态
	bool IsActive;
	//剩余时间, 剩余帧
	float RemainTime;
	//条件委托
	FCoroCondition ConditionDele;
	//构造函数
	DDCoroNode() : IsActive(false) {}

	//延迟多少帧继续执行
	bool UpdateOperate(int32 SpaceTick)
	{
		if (!IsActive)
		{
			RemainTime = SpaceTick;
			IsActive = true;
			return true;
		}
		else
		{
			RemainTime -= 1;
			if (RemainTime > 0)
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//延迟秒条件 ,帧更新函数, 返回true, 就继续挂起, 返回false, 执行后续代码
	bool UpdateOperate(float DeltaTime, float SpaceTime)
	{
		if (!IsActive)
		{
			RemainTime = SpaceTime;
			IsActive = true;
			return true;
		}
		else
		{
			RemainTime -= DeltaTime;
			if (RemainTime > 0)
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//bool变量指针挂起, 变量为true则继续挂起, 为false则执行后续代码
	bool UpdateOperate(bool* Condition)
	{
		if (!IsActive)
		{
			IsActive = true;
			return true;
		}
		else
		{
			if (*Condition)
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//委托函数挂起, 函数返回true则继续挂起, 返回false则执行后续代码
	template<typename UserClass>
	bool UpdateOperate(UserClass* UserObj, typename FCoroCondition::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
	{
		if (!IsActive)
		{
			if (!ConditionDele.IsBound())
				ConditionDele.BindUObject(UserObj, InMethod);
			IsActive = true;
			return true;
		}
		else
		{
			if (ConditionDele.Execute())
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//lambuda表达式挂起
	bool UpdateOperate(TFunction<bool()> InFunction)
	{
		if (!IsActive)
		{
			IsActive = true;
			return true;
		}
		else
		{
			if (InFunction())
				return true;
			else
			{
				IsActive = false;
				return false;
			}
		}
	}

	//停止协程
	bool UpdateOperate()
	{
		IsActive = false;
		return true;
	}

};

struct DDCoroTask
{
	//是否销毁
	bool IsDestory;
	//多个协程节点
	TArray<DDCoroNode*> CoroStack;
	//构造函数
	DDCoroTask(int32 CoroCount)
	{
		IsDestory = false;
		for (int i = 0; i <= CoroCount; ++i)
			CoroStack.Push(new DDCoroNode());
	}
	//析构函数
	virtual ~DDCoroTask()
	{
		for (int i = 0; i < CoroStack.Num(); ++i)
			delete CoroStack[i];
	}
	//实例运行的帧函数
	virtual void Work(float DeltaTime) {}
	//是否完结状态
	bool IsFinish()
	{
		bool Flag = true;
		for (int i = 0; i < CoroStack.Num(); ++i)
		{
			if (CoroStack[i]->IsActive)
			{
				Flag = false;
				break;
			}
		}
		return Flag;
	}
};

#pragma endregion


#pragma region Invoke

DECLARE_DELEGATE(FDDInvokeEvent)

struct DDInvokeTask
{
	//是否销毁
	bool IsDestroy;
	//延迟执行的世界
	float DelayTime;
	//是否循环
	bool IsRepeat;
	//循环时间间隔
	float RepeatTime;
	//是否在循环阶段
	bool IsRepeatState;
	//计时器
	float TimeCount;
	//方法委托
	FDDInvokeEvent InvokeEvent;
	//构造函数
	DDInvokeTask(float InDelayTime, bool InIsRepeat, float InRepeatTime)
	{
		DelayTime = InDelayTime;
		IsRepeat = InIsRepeat;
		RepeatTime = InRepeatTime;
		IsRepeatState = false;
		TimeCount = 0.f;
		IsDestroy = false;
	}
	//帧更新操作函数
	bool UpdateOperate(float DeltaSeconds)
	{
		TimeCount += DeltaSeconds;
		if (!IsRepeatState)
		{
			if (TimeCount >= DelayTime)
			{
				InvokeEvent.ExecuteIfBound();
				TimeCount = 0.f;
				if (IsRepeat)
					IsRepeatState = true;
				else
					return true;
			}
		}
		else
		{
			if (TimeCount >= RepeatTime)
			{
				InvokeEvent.ExecuteIfBound();
				TimeCount = 0.f;
			}
		}
		return false;
	}
};


#pragma endregion


#pragma region Wealth

USTRUCT()
struct FWealthItem
{
	GENERATED_BODY()

public:

	//对象名
	UPROPERTY(EditAnywhere)
		FName ObjectName;

	//类名
	UPROPERTY(EditAnywhere)
		FName ClassName;

};

USTRUCT()
struct FWealthObject : public FWealthItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<UObject> WealthClass;

};

USTRUCT()
struct FWealthActor : public FWealthItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> WealthClass;

	UPROPERTY(EditAnywhere)
		FTransform Transform;

};

USTRUCT()
struct FWealthWidget : public FWealthItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WealthClass;

};

//Object资源结构体
USTRUCT()
struct FObjectWealthEntry
{
	GENERATED_BODY()

public:

	//资源名
	UPROPERTY(EditAnywhere)
		FName WealthName;

	//资源种类名
	UPROPERTY(EditAnywhere)
		FName WealthKind;

	//资源链接
	UPROPERTY(EditAnywhere)
		FStringAssetReference WealthPath;

	//加载出来的对象
	UPROPERTY()
		UObject* WealthObject;
};

//UClass类型枚举
UENUM()
enum class EWealthType : uint8 {
	Object,
	Actor,
	Widget
};

//Class资源结构体
USTRUCT()
struct FClassWealthEntry
{
	GENERATED_BODY()

public:

	//资源类别
	UPROPERTY(EditAnywhere)
		EWealthType WealthType;

	//资源名
	UPROPERTY(EditAnywhere)
		FName WealthName;

	//资源种类名
	UPROPERTY(EditAnywhere)
		FName WealthKind;

	//资源链接
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<UObject> WealthPtr;

	//加载出来的对象
	UPROPERTY()
		UClass* WealthClass;
};

//纯获取链接结构体, 不进行同异步加载
USTRUCT()
struct FWealthURL
{
	GENERATED_BODY()

public:

	//资源名
	UPROPERTY(EditAnywhere)
		FName WealthName;

	//资源种类名
	UPROPERTY(EditAnywhere)
		FName WealthKind;

	//资源链接
	UPROPERTY(EditAnywhere)
		FStringAssetReference WealthPath;

	//资源链接
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<UObject> WealthPtr;

};


UCLASS()
class DATADRIVEN_API UWealthData : public UDataAsset
{
	GENERATED_BODY()

public:

	//模组名字, 这个DataAsset下的资源生成的对象默认注册到ModuleName对应的模组
	//如果为空则说明该Asset使用于多个模组下, 自动生成的对象注册到该Asset放置的模组下
	UPROPERTY(EditAnywhere)
		FName ModuleName;

	//自动生成的Object
	UPROPERTY(EditAnywhere)
		TArray<FWealthObject> AutoObjectData;

	//自动生成的Actor
	UPROPERTY(EditAnywhere)
		TArray<FWealthActor> AutoActorData;

	//自动生成的Widget
	UPROPERTY(EditAnywhere)
		TArray<FWealthWidget> AutoWidgetData;

	//Object资源链接集合
	UPROPERTY(EditAnywhere)
		TArray<FObjectWealthEntry> ObjectWealthData;

	//Class资源链接集合
	UPROPERTY(EditAnywhere)
		TArray<FClassWealthEntry> ClassWealthData;

	//资源链接集合
	UPROPERTY(EditAnywhere)
		TArray<FWealthURL> WealthURL;

};

#pragma endregion


#pragma region UIFrame


//布局类型
UENUM()
enum class ELayoutType : uint8 {
	Canvas,     //对应CanvasPanel
	Overlay,	//对应Overlay
};

//UI层级类型, 自己动态添加, 一般6层够用了
UENUM()
enum class ELayoutLevel : uint8
{
	Level_0 = 0,
	Level_1,
	Level_2,
	Level_3,
	Level_All,//这个层级会隐藏所有ShowGroup的对象
};


//面板类型
UENUM()
enum class EPanelShowType : uint8 {
	DoNothing,   //不影响其他面板
	HideOther,   //隐藏其他
	Reverse,     //反向切换, 弹窗类型
};

//弹窗遮罩透明度
UENUM()
enum class EPanelLucenyType : uint8 {
	Lucency,        //全透明, 不能穿透
	Translucence,   //半透明，不能穿透
	ImPenetrable,   //低透明度，不能穿透
	Pentrate,       //全透明, 可以穿透
};

//面板属性
USTRUCT()
struct FUINature
{
	GENERATED_BODY()

public:

	//布局类型
	UPROPERTY(EditAnywhere)
		ELayoutType LayoutType;

	//UI层级, 给HideOther类型的面板使用, 指定影响的范围
	UPROPERTY(EditAnywhere)
		ELayoutLevel LayoutLevel;

	//面板类型
	UPROPERTY(EditAnywhere)
		EPanelShowType PanelShowType;

	//弹窗遮罩透明度
	UPROPERTY(EditAnywhere)
		EPanelLucenyType PanelLucenyType;

	//Canvas锚点
	UPROPERTY(EditAnywhere)
		FAnchors Anchors;

	//Canvas的Offset(pos, size)  Overlay的padding
	UPROPERTY(EditAnywhere)
		FMargin Offsets;

	//Overlay的水平布局
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EHorizontalAlignment> HAlign;

	//Overlay的垂直布局
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EVerticalAlignment> VAlign;

};



#pragma endregion