// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/GameEngine.h"
#include "Engine/DataAsset.h"
#include "Anchors.h"
#include "DDTypes.generated.h"

class UDDObject;
class ADDActor;
class UDDUserWidget;

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
	//最终输出的字符串
	FString RecordInfo;
	//显示时间
	float ShowTime;
	//显示颜色
	FColor ShowColor;

public:

	//状态模式, 0 : Debug, 1 : Log, 2 : Warning, 3 : Error
	uint8 PatternID;

public:

	//构造函数
	inline DDRecord() {};

	~DDRecord() {}

	static TSharedPtr<DDRecord> Get();

	//设定模式
	inline void InitParam(float InTime, FColor InColor)
	{
		ShowTime = InTime;
		ShowColor = InColor;
	}

	//结果输出函数
	inline void Output() {
		switch (PatternID)
		{
		case 0:
		{
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, ShowTime, ShowColor, RecordInfo);
			}
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
	inline DDRecord &operator<<(FString Info) { RecordInfo.Append(Info); return *this; }
	inline DDRecord &operator<<(FName Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FText Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(const char* Info) { RecordInfo += Info; return *this; }
	inline DDRecord &operator<<(const char Info) { RecordInfo.AppendChar(Info); return *this; }
	inline DDRecord &operator<<(int32 Info) { RecordInfo.Append(FString::FromInt(Info)); return *this; }
	inline DDRecord &operator<<(float Info) { RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }
	inline DDRecord &operator<<(double Info) { RecordInfo.Append(FString::SanitizeFloat(Info)); return *this; }
	inline DDRecord &operator<<(bool Info) { RecordInfo.Append(Info ? FString("true") : FString("false")); return *this; }
	inline DDRecord &operator<<(FVector2D Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FVector Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FRotator Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FQuat Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FTransform Info) { RecordInfo.Append(Info.ToString()); return *this; }
	/*inline DDRecord &operator<<(FMargin Info) {
		RecordInfo.Append(FString::Printf("( %f , %f , %f, %f )", Info.Left, Info.Top, Info.Right, Info.Bottom));
		return *this;
	}
	inline DDRecord &operator<<(FAnchors Info) {
		RecordInfo.Append(FString::Printf("( %f , %f , %f, %f )", Info.Minimum.X, Info.Minimum.Y, Info.Maximum.X, Info.Maximum.Y));
		return *this;
	}*/
	inline DDRecord &operator<<(FMatrix Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FColor Info) { RecordInfo.Append(Info.ToString()); return *this; }
	inline DDRecord &operator<<(FLinearColor Info) { RecordInfo.Append(Info.ToString()); return *this; }

	//输出
	inline void operator<<(DDRecord& Record) { Record.Output(); }
};


#pragma endregion


#pragma region LifeTimePart

//BaseObject生命周期
UENUM()
enum class EBaseObjectLife : uint8
{
	None = 0,//初始化的状态
	Init,
	Loading,
	Register,
	Enable,
	Disable,
	UnRegister,
	UnLoading
};

//BaseObject生命周期
UENUM()
enum class EBaseObjectState : uint8
{
	Active = 0, //激活进程
	Stable,     //稳定进程
	Destroy     //销毁进程
};

#pragma endregion

#pragma region ReflectPart

//通信协议,只用于对象通信
UENUM()
enum class EAgreementType : uint8 {
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
	NoModule = 0,     //如果什么都没有改变说明没有对应模组
	LackObject,  //缺失对象
	NoFunction,  //没有对应方法
	Succeed      //成功调用
};

//消息通信机制
struct DDParam
{

public:

	//回调的结果
	ECallResult CallResult;

	//参数指针
	void* ParamPtr;
};


struct DDModuleAgreement
{

public:
	//模组对象
	int32 ModuleIndex;
	//方法名
	FName FunctionName;
};



struct DDObjectAgreement
{

public:
	//模组对象
	int32 ModuleIndex;
	//协议类型
	EAgreementType AgreementType;
	//对象组
	TArray<FName> ObjectGroup;
	//方法名
	FName FunctionName;
};

#pragma endregion

#pragma region MessageEventPart



#pragma region DDAnyFun

//任意方法结构体
struct DDAnyFun {
	struct BaseFun {
	public:
		virtual ~BaseFun() {};
	};
	template<typename RetType, typename... VarTypes>
	struct ValFun : public BaseFun {
	public:
		TFunction<RetType(VarTypes...)> TarFun;
		ValFun(const TFunction<RetType(VarTypes...)> InsFun) : TarFun(InsFun) {}
		RetType Execute(VarTypes... Params) {
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
	RetType	Execute(VarTypes... Params) {
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
//方法节点
struct DDMsgNode
{
	//被调用接口数量
	int32 CallCount;
	//方法列表
	TMap<int32, DDAnyFun*> FunQuene;
	//注册方法
	template<typename RetType, typename... VarTypes>
	int32 RegisterFun(TFunction<RetType(VarTypes...)> InsFun);
	//注销方法
	void UnRegisterFun(int32 FunID)
	{
		//直接从序列里移除对象即可
		DDAnyFun* DesPtr = *FunQuene.Find(FunID);
		FunQuene.Remove(FunID);
		delete DesPtr;
	}
	//执行方法
	template<typename RetType, typename... VarTypes>
	RetType Execute(VarTypes... Params);
	//判断是否绑定有函数
	bool IsBound() { return FunQuene.Num() > 0; }
	//如果绑定有函数就执行
	template<typename RetType, typename... VarTypes>
	bool ExecuteIsBound(VarTypes... Params);
	//构造函数
	DDMsgNode() {
		//初始化为0, 手动添加计数
		CallCount = 0;
	}
};


template<typename RetType, typename... VarTypes>
int32 DDMsgNode::RegisterFun(TFunction<RetType(VarTypes...)> InsFun)
{
	//获取方法序列里所有键
	TArray<int32> FunKeyQuene;
	FunQuene.GenerateKeyArray(FunKeyQuene);
	//查找一个没有被注册的新的键
	int32 NewID;
	for (int32 i = FunKeyQuene.Num(); i >= 0; --i)
		if (!FunKeyQuene.Contains(i)) {
			NewID = i;
			break;
		}
	//将新的方法添加到方法序列
	FunQuene.Add(NewID, new DDAnyFun(InsFun));
	//返回键值
	return NewID;
}


template<typename RetType, typename... VarTypes>
RetType DDMsgNode::Execute(VarTypes... Params)
{
	TMap<int32, DDAnyFun*>::TIterator It(FunQuene);
	++It;
	for (; It; ++It)
	{
		It.Value()->Execute<RetType, VarTypes...>(Params...);
	}
	TMap<int32, DDAnyFun*>::TIterator IBegin(FunQuene);
	return IBegin.Value()->Execute<RetType, VarTypes...>(Params...);
}

template<typename RetType, typename... VarTypes>
bool DDMsgNode::ExecuteIsBound(VarTypes... Params)
{
	if (!IsBound()) return false;
	for (TMap<int32, DDAnyFun*>::TIterator It(FunQuene); It; ++It)
	{
		It.Value()->Execute<RetType, VarTypes...>(Params...);
	}
	return true;
}

#pragma endregion

#pragma region DDMsgHandle


struct DDMsgQueue;


//返回的消息句柄
template<typename RetType, typename... VarTypes>
struct DDCallHandle
{
	//消息队列
	DDMsgQueue* MsgQuene;
	//调用名字
	FName CallName;
	//是否有效
	TSharedPtr<bool> IsActived;
	//执行接口
	RetType Execute(VarTypes... Params);
	//是否已经绑定
	bool IsBound();
	//如果绑定就执行
	bool ExecuteIfBound(VarTypes... Params);
	//注销接口
	void UnRegister();
	//无参构造
	DDCallHandle() { }
	//构造函数
	DDCallHandle(DDMsgQueue* MQ, FName CN) {
		MsgQuene = MQ;
		CallName = CN;
		//构建时状态为激活状态
		IsActived = MakeShareable<bool>(new bool(true));
	}
	//重写赋值
	DDCallHandle<RetType, VarTypes...>& operator=(const DDCallHandle<RetType, VarTypes...>& Other)
	{
		if (this == &Other) return *this;
		MsgQuene = Other.MsgQuene;
		CallName = Other.CallName;
		IsActived = Other.IsActived;
		return *this;
	}
};

template<typename RetType, typename... VarTypes>
bool DDCallHandle<RetType, VarTypes...>::ExecuteIfBound(VarTypes... Params)
{
	if (!IsBound()) return false;
	MsgQuene->Execute<RetType, VarTypes...>(CallName, Params...);
	return true;
}

template<typename RetType, typename... VarTypes>
inline void DDCallHandle<RetType, VarTypes...>::UnRegister()
{
	if (*IsActived.Get()) MsgQuene->UnRegisterCallPort(CallName);
	*IsActived.Get() = false;
}

template<typename RetType, typename... VarTypes>
bool DDCallHandle<RetType, VarTypes...>::IsBound()
{
	return MsgQuene->IsBound(CallName);
}

template<typename RetType, typename... VarTypes>
RetType DDCallHandle<RetType, VarTypes...>::Execute(VarTypes... Params)
{
	return MsgQuene->Execute<RetType, VarTypes...>(CallName, Params...);
}

#pragma endregion

#pragma region DDMsgQueue

struct DDFunHandle;

//消息队列
struct DDMsgQueue {
	//节点队列
	TMap<FName, DDMsgNode> MsgQuene;
	//注册调用接口
	template<typename RetType, typename... VarTypes>
	DDCallHandle<RetType, VarTypes...> RegisterCallPort(FName CallName);
	//注册调用方法
	template<typename RetType, typename... VarTypes>
	DDFunHandle RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun);
	//移除调用接口
	void UnRegisterCallPort(FName CallName)
	{
		//让对应的接口计数器减一, 如果计数器小于等于0, 就移除调用接口
		MsgQuene.Find(CallName)->CallCount--;
		if (MsgQuene.Find(CallName)->CallCount <= 0)
			MsgQuene.Remove(CallName);
	}
	//移除调用方法
	void UnRegisterFunPort(FName CallName, int32 FunID)
	{
		MsgQuene.Find(CallName)->UnRegisterFun(FunID);
	}
	//执行方法接口
	template<typename RetType, typename... VarTypes>
	RetType Execute(FName CallName, VarTypes... Params);
	//是否已经绑定方法
	bool IsBound(FName CallName) { return MsgQuene.Find(CallName)->IsBound(); };
};



template<typename RetType, typename... VarTypes>
DDCallHandle<RetType, VarTypes...>
DDMsgQueue::RegisterCallPort(FName CallName)
{
	//如果已经存在对应调用接口, 设置接口调用计数器加一
	if (MsgQuene.Contains(CallName))
	{
		MsgQuene.Find(CallName)->CallCount++;
	}
	else
	{
		//创建新的调用接口并且添加到队列
		MsgQuene.Add(CallName, DDMsgNode());
		//计数加一
		MsgQuene.Find(CallName)->CallCount++;
	}
	//返回调用接口句柄
	return DDCallHandle<RetType, VarTypes...>(this, CallName);
}

template<typename RetType, typename... VarTypes>
DDFunHandle DDMsgQueue::RegisterFunPort(FName CallName, TFunction<RetType(VarTypes...)> InsFun)
{
	//得到的ID
	int32 FunID;
	//如果已经存在对应接口
	if (MsgQuene.Contains(CallName))
	{
		//直接将TFunction注册进接口
		FunID = MsgQuene.Find(CallName)->RegisterFun(InsFun);
	}
	else
	{
		//如果没有接口, 先创建接口
		MsgQuene.Add(CallName, DDMsgNode());
		//再将TFunction注册进接口
		FunID = MsgQuene.Find(CallName)->RegisterFun(InsFun);
	}
	//返回方法句柄,用于在注销时取消注册
	return DDFunHandle(this, CallName, FunID);
}

template<typename RetType, typename... VarTypes>
RetType DDMsgQueue::Execute(FName CallName, VarTypes... Params)
{
	return MsgQuene.Find(CallName)->Execute<RetType, VarTypes...>(Params...);
}

//void DDMsgQueue::UnRegisterCallPort(FName CallName)
//{
//	//让对应的接口计数器减一, 如果计数器小于等于0, 就移除调用接口
//	MsgQuene.Find(CallName)->CallCount--;
//	if (MsgQuene.Find(CallName)->CallCount <= 0)
//		MsgQuene.Remove(CallName);
//}
//
//void DDMsgQueue::UnRegisterFunPort(FName CallName, int32 FunID)
//{
//	MsgQuene.Find(CallName)->UnRegisterFun(FunID);
//}

//bool DDMsgQueue::IsBound(FName CallName)
//{
//	return MsgQuene.Find(CallName)->IsBound();
//}

#pragma endregion

#pragma region DDFunHandle

//返回的方法句柄
struct DDFunHandle
{
	//消息队列
	DDMsgQueue* MsgQuene;
	//调用名字
	FName CallName;
	//方法ID
	int32 FunID;
	//是否还有效
	TSharedPtr<bool> IsActived;
	//注销方法
	void UnRegister()
	{
		if (*IsActived.Get()) MsgQuene->UnRegisterFunPort(CallName, FunID);
		//设置成失活状态
		*IsActived.Get() = false;
	}
	//无参构造
	DDFunHandle() {}
	//构造函数
	DDFunHandle(DDMsgQueue* MQ, FName CN, int32 FI) {
		MsgQuene = MQ;
		CallName = CN;
		FunID = FI;
		//构建时是激活状态
		IsActived = MakeShareable<bool>(new bool(true));
	}
	//重写赋值
	DDFunHandle& operator=(const DDFunHandle& Other)
	{
		if (this == &Other) return *this;
		MsgQuene = Other.MsgQuene;
		CallName = Other.CallName;
		FunID = Other.FunID;
		IsActived = Other.IsActived;
		return *this;
	}
};

#pragma endregion


#pragma endregion

#pragma region WealthPart


UENUM()
enum class EWealthType : uint8
{
	Object,
	Actor,
	Widget
};

USTRUCT()
struct FObjectWealthEntry
{
	GENERATED_BODY()

public:

	//对资源的唯一标识,通过标识获取指定的资源
	UPROPERTY(EditAnywhere)
		FName WealthName;
	//资源的类别, 相同属性的资源放在一起
	UPROPERTY(EditAnywhere)
		FName WealthKind;
	//资源链接
	UPROPERTY(EditAnywhere)
		FStringAssetReference WealthPath;
	//加载出来的对象
	UPROPERTY()
		UObject* WealthObject;
};

USTRUCT()
struct FClassWealthEntry
{
	GENERATED_BODY()

public:

	//资源类型
	UPROPERTY(EditAnywhere)
		EWealthType WealthType;
	//对资源的唯一标识,通过标识获取指定的资源
	UPROPERTY(EditAnywhere)
		FName WealthName;
	//资源的类别, 相同属性的资源放在一起
	UPROPERTY(EditAnywhere)
		FName WealthKind;
	//资源链接
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<UObject> WealthPtr;
	//加载出来的对象
	UPROPERTY()
		UClass* WealthClass;
};

USTRUCT()
struct FWealthItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		FName ObjectName;
	UPROPERTY(EditAnywhere)
		FName ClassName;
};

USTRUCT()
struct FWealthObject : public FWealthItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDDObject> WealthClass;
};

USTRUCT()
struct FWealthActor : public FWealthItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<ADDActor> WealthClass;

	UPROPERTY(EditAnywhere)
		FTransform Transform;
};

USTRUCT()
struct FWealthWidget : public FWealthItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDDUserWidget> WealthClass;

};

USTRUCT()
struct FWealthURL
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		FName WealthName;

	UPROPERTY(EditAnywhere)
		FName WealthKind;

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

	//模组名字, 这个DataAsset下所有自动生成的资源都要求位于这个模组下, 
	//如果为空则生成在引用这个资源的模组下, 如果不是多个模组引用的话最好填入内容, 避免混乱
	UPROPERTY(EditAnywhere)
		FName ModuleName;

	//自动生成的Actor
	UPROPERTY(EditAnywhere)
		TArray<FWealthObject> AutoObjectData;

	//自动加载的Object
	UPROPERTY(EditAnywhere)
		TArray<FWealthActor> AutoActorData;

	//自动加载的Widget
	UPROPERTY(EditAnywhere)
		TArray<FWealthWidget> AutoWidgetData;

	//DD资源链接集合
	UPROPERTY(EditAnywhere)
		TArray<FClassWealthEntry> ClassWealthData;

	//普通资源链接集合
	UPROPERTY(EditAnywhere)
		TArray<FObjectWealthEntry> ObjectWealthData;

	//资源链接集合
	UPROPERTY(EditAnywhere)
		TArray<FWealthURL> WealthURL;

};

#pragma endregion


#pragma region Coroutine

//判断条件委托
DECLARE_DELEGATE_RetVal(bool, FCoroCondition)

struct DDCoroNode
{
	//激活状态
	bool IsActive;
	//剩余时间或者帧
	float RemainTime;
	//条件委托方法
	FCoroCondition ConditionDele;
	//表达式
	TFunction<bool()> ConditionFun;
	//构造函数, 默认为未激活状态
	DDCoroNode() : IsActive(false) {}
	//帧更新, 传入间隔帧
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
	//时间更新, 传入每帧时间间隔和间隔时间
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
	//变量更新
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
			{
				return true;
			}
			else
			{
				IsActive = false;
				return false;
			}
		}
	}
	//方法更新 : 函数
	template<typename UserClass>
	bool UpdateOperate(UserClass* UserObj, typename FCoroCondition::TUObjectMethodDelegate<UserClass>::FMethodPtr InMethod)
	{
		if (!IsActive)
		{
			if (!ConditionDele.IsBound()) ConditionDele.BindUObject(UserObj, InMethod);
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
	//方法更新 : 表达式
	bool UpdateOperate(TFunction<bool()> InFunction)
	{
		if (!IsActive)
		{
			if (!ConditionFun) ConditionFun = InFunction;
			IsActive = true;
			return true;
		}
		else
		{
			if (ConditionFun())
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
	TArray<DDCoroNode*> CoroStack;
	DDCoroTask(int32 CoroCount)
	{
		for (int i = 0; i <= CoroCount; ++i)
			CoroStack.Add(new DDCoroNode());
	}
	virtual ~DDCoroTask()
	{
		for (int i = 0; i < CoroStack.Num(); ++i)
			delete CoroStack[i];
	}
	virtual void Work(float DeltaTime) {}
	bool IsFinish()
	{
		bool Flag = true;
		for (int i = 0; i < CoroStack.Num(); ++i)
			if (CoroStack[i]->IsActive) Flag = false;
		return Flag;
	}
};

DECLARE_DELEGATE(FDDInvokeEvent)

struct DDInvokeTask
{
	//开始暂停时间
	float DelayTime;
	//是否循环
	bool IsRepeat;
	//循环间隔
	float RepeatTime;
	//实际执行方法
	FDDInvokeEvent InvokeEvent;
	//是否在循环阶段
	bool IsRepeatState;
	//计时器
	float TimeCount;
	//构造函数
	DDInvokeTask(float InDelayTime, bool InIsRepeat, float InRepeatTime)
	{
		DelayTime = InDelayTime;
		IsRepeat = InIsRepeat;
		RepeatTime = InRepeatTime;
		IsRepeatState = false;
		TimeCount = 0.f;
	}
	//帧更新函数, 如果结束了就会返回true
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


//窗口显示类型
UENUM()
enum class EPanelShowType : uint8 {
	DoNothing,   //不做任何改变
	HideOther,   //隐藏其他
	Reverse,     //关闭自己
};

//窗口透明度类型
UENUM()
enum class EPanelLucenyType : uint8 {
	Lucency,        //全透明, 不能穿透
	Translucence,   //半透明，不能穿透
	ImPenetrable,   //低透明度，不能穿透
	Pentrate,       //全透明, 可以穿透
};

//控件属性
USTRUCT()
struct FUINature
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		ELayoutType LayoutType;

	UPROPERTY(EditAnywhere)
		ELayoutLevel LayoutLevel;

	UPROPERTY(EditAnywhere)
		EPanelShowType PanelShowType;

	UPROPERTY(EditAnywhere)
		EPanelLucenyType PanelLucenyType;

	UPROPERTY(EditAnywhere)
		FAnchors Anchors;

	UPROPERTY(EditAnywhere)
		FMargin Offsets;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EHorizontalAlignment> HAlign;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EVerticalAlignment> VAlign;

};



#pragma endregion