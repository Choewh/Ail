// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "AilGameMode.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef AIL_AilGameMode_generated_h
#error "AilGameMode.generated.h already included, missing '#pragma once' in AilGameMode.h"
#endif
#define AIL_AilGameMode_generated_h

#define FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAAilGameMode(); \
	friend struct Z_Construct_UClass_AAilGameMode_Statics; \
public: \
	DECLARE_CLASS(AAilGameMode, AGameModeBase, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/Ail"), AIL_API) \
	DECLARE_SERIALIZER(AAilGameMode)


#define FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AAilGameMode(AAilGameMode&&); \
	AAilGameMode(const AAilGameMode&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(AIL_API, AAilGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AAilGameMode); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AAilGameMode) \
	AIL_API virtual ~AAilGameMode();


#define FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_9_PROLOG
#define FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_12_INCLASS_NO_PURE_DECLS \
	FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> AIL_API UClass* StaticClass<class AAilGameMode>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
