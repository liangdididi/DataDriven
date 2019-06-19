// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDCommon/DDTypes.h"
#include "DDDefine.generated.h"

/**
 *
 */
UCLASS()
class DATADRIVEN_API UDDDefine : public UObject
{
	GENERATED_BODY()


};


#define DDMODFUNC(FuncName); \
	struct FuncName##Param : DDParam \
	{ \
		FuncName##Param() { ParamPtr = NULL; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_ONE(FuncName, ParamType1, ParamName1); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_TWO(FuncName, ParamType1, ParamName1, ParamType2, ParamName2); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_THREE(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_FOUR(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_FIVE(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_SIX(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_SEVEN(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6, ParamType7, ParamName7); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
			ParamType7 ParamName7; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		ParamType7 ParamName7() { return Parameter.ParamName7; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_EIGHT(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6, ParamType7, ParamName7, ParamType8, ParamName8); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
			ParamType7 ParamName7; \
			ParamType8 ParamName8; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		ParamType7 ParamName7() { return Parameter.ParamName7; } \
		ParamType8 ParamName8() { return Parameter.ParamName8; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_NINE(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6, ParamType7, ParamName7, ParamType8, ParamName8, ParamType9, ParamName9); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
			ParamType7 ParamName7; \
			ParamType8 ParamName8; \
			ParamType9 ParamName9; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		ParamType7 ParamName7() { return Parameter.ParamName7; } \
		ParamType8 ParamName8() { return Parameter.ParamName8; } \
		ParamType9 ParamName9() { return Parameter.ParamName9; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDMODFUNC_TEN(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6, ParamType7, ParamName7, ParamType8, ParamName8, ParamType9, ParamName9, ParamType10, ParamName10); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
			ParamType7 ParamName7; \
			ParamType8 ParamName8; \
			ParamType9 ParamName9; \
			ParamType10 ParamName10; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		ParamType7 ParamName7() { return Parameter.ParamName7; } \
		ParamType8 ParamName8() { return Parameter.ParamName8; } \
		ParamType9 ParamName9() { return Parameter.ParamName9; } \
		ParamType10 ParamName10() { return Parameter.ParamName10; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9, ParamType10 ParamName10) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		Param->Parameter.ParamName10 = ParamName10; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9, ParamType10 ParamName10) \
	{ \
		DDModuleAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		Param->Parameter.ParamName10 = ParamName10; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}







#define DDOBJFUNC(FuncName); \
	struct FuncName##Param : DDParam \
	{ \
		FuncName##Param() { ParamPtr = NULL; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_ONE(FuncName, ParamType1, ParamName1); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_TWO(FuncName, ParamType1, ParamName1, ParamType2, ParamName2); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}



#define DDOBJFUNC_THREE(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_FOUR(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_FIVE(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_SIX(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_SEVEN(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6, ParamType7, ParamName7); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
			ParamType7 ParamName7; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		ParamType7 ParamName7() { return Parameter.ParamName7; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_EIGHT(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6, ParamType7, ParamName7, ParamType8, ParamName8); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
			ParamType7 ParamName7; \
			ParamType8 ParamName8; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		ParamType7 ParamName7() { return Parameter.ParamName7; } \
		ParamType8 ParamName8() { return Parameter.ParamName8; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_NINE(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6, ParamType7, ParamName7, ParamType8, ParamName8, ParamType9, ParamName9); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
			ParamType7 ParamName7; \
			ParamType8 ParamName8; \
			ParamType9 ParamName9; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		ParamType7 ParamName7() { return Parameter.ParamName7; } \
		ParamType8 ParamName8() { return Parameter.ParamName8; } \
		ParamType9 ParamName9() { return Parameter.ParamName9; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}


#define DDOBJFUNC_TEN(FuncName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5, ParamType6, ParamName6, ParamType7, ParamName7, ParamType8, ParamName8, ParamType9, ParamName9, ParamType10, ParamName10); \
	struct FuncName##Param : DDParam \
	{ \
		struct \
		{ \
			ParamType1 ParamName1; \
			ParamType2 ParamName2; \
			ParamType3 ParamName3; \
			ParamType4 ParamName4; \
			ParamType5 ParamName5; \
			ParamType6 ParamName6; \
			ParamType7 ParamName7; \
			ParamType8 ParamName8; \
			ParamType9 ParamName9; \
			ParamType10 ParamName10; \
		} Parameter; \
		ParamType1 ParamName1() { return Parameter.ParamName1; } \
		ParamType2 ParamName2() { return Parameter.ParamName2; } \
		ParamType3 ParamName3() { return Parameter.ParamName3; } \
		ParamType4 ParamName4() { return Parameter.ParamName4; } \
		ParamType5 ParamName5() { return Parameter.ParamName5; } \
		ParamType6 ParamName6() { return Parameter.ParamName6; } \
		ParamType7 ParamName7() { return Parameter.ParamName7; } \
		ParamType8 ParamName8() { return Parameter.ParamName8; } \
		ParamType9 ParamName9() { return Parameter.ParamName9; } \
		ParamType10 ParamName10() { return Parameter.ParamName10; } \
		FuncName##Param() { ParamPtr = &Parameter; } \
	};\
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9, ParamType10 ParamName10) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		Param->Parameter.ParamName10 = ParamName10; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9, ParamType10 ParamName10) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		Param->Parameter.ParamName10 = ParamName10; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	FuncName##Param* FuncName##RT(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9, ParamType10 ParamName10) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		Param->Parameter.ParamName10 = ParamName10; \
		ExecuteFunction(Agreement, Param); \
		return Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, TArray<FName> ObjectGroup, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9, ParamType10 ParamName10) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup = ObjectGroup; \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		Param->Parameter.ParamName10 = ParamName10; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, EAgreementType AgreementType, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9, ParamType10 ParamName10) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = AgreementType; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		Param->Parameter.ParamName10 = ParamName10; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	} \
	void FuncName(int32 ModuleIndex, FName ObjName, FName FunctionName, ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5, ParamType6 ParamName6, ParamType7 ParamName7, ParamType8 ParamName8, ParamType9 ParamName9, ParamType10 ParamName10) \
	{ \
		DDObjectAgreement Agreement; \
		Agreement.ModuleIndex = ModuleIndex; \
		Agreement.AgreementType = EAgreementType::SelfObject; \
		Agreement.ObjectGroup.Add(ObjName); \
		Agreement.FunctionName = FunctionName; \
		FuncName##Param* Param = new FuncName##Param(); \
		Param->Parameter.ParamName1 = ParamName1; \
		Param->Parameter.ParamName2 = ParamName2; \
		Param->Parameter.ParamName3 = ParamName3; \
		Param->Parameter.ParamName4 = ParamName4; \
		Param->Parameter.ParamName5 = ParamName5; \
		Param->Parameter.ParamName6 = ParamName6; \
		Param->Parameter.ParamName7 = ParamName7; \
		Param->Parameter.ParamName8 = ParamName8; \
		Param->Parameter.ParamName9 = ParamName9; \
		Param->Parameter.ParamName10 = ParamName10; \
		ExecuteFunction(Agreement, Param); \
		delete Param; \
	}




#define DDCORO_PARAM(UserClass); \
struct DGCoroTask : public DDCoroTask \
{ \
	UserClass* D; \
	DGCoroTask(UserClass* Data, int32 CoroCount) : DDCoroTask(CoroCount) { D = Data; }


#define DDCORO_WORK_START \
virtual void Work(float DeltaTime) override \
{ \
	goto DDCORO_LABEL_PICKER; \
DDCORO_LABEL_START :


#define DDCORO_WORK_MIDDLE \
	goto DDCORO_LABEL_END; \
DDCORO_LABEL_PICKER :


#define DDCORO_WORK_END \
goto DDCORO_LABEL_START; \
	DDCORO_LABEL_END : \
		; \
	} \
}; \
return new DGCoroTask(this, DDYIELD_COUNT); 


#define DDCORO_BEGIN() "DataDriven/Public/DDCommon/DDCoroBegin.h"


#define DDCORO_END() "DataDriven/Public/DDCommon/DDCoroEnd.h"


#define	DDYIELD_READY() "DataDriven/Public/DDCommon/DDYieldReady.h"


#define DDYIELD_RETURN_TICK(Tick); \
if(CoroStack[DDYIELD_COUNT]->UpdateOperate(Tick)) \
	goto DDCORO_LABEL_END;


#define DDYIELD_RETURN_SECOND(Time); \
if(CoroStack[DDYIELD_COUNT]->UpdateOperate(DeltaTime, Time)) \
	goto DDCORO_LABEL_END;


#define DDYIELD_RETURN_BOOL(Param); \
if (CoroStack[DDYIELD_COUNT]->UpdateOperate(Param)) \
	goto DDCORO_LABEL_END;


#define DDYIELD_RETURN_FUNC(UserClass, UserFunc); \
if (CoroStack[DDYIELD_COUNT]->UpdateOperate(UserClass, UserFunc)) \
	goto DDCORO_LABEL_END;


#define DDYIELD_RETURN_LAMB(Expression); \
if (CoroStack[DDYIELD_COUNT]->UpdateOperate([this](){ return Expression; })) \
	goto DDCORO_LABEL_END;


#define DDYIELD_RETURN_STOP(); \
if (CoroStack[DDYIELD_COUNT]->UpdateOperate()) \
	goto DDCORO_LABEL_END;
