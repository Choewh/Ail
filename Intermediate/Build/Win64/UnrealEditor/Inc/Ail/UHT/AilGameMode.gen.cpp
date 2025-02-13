// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Ail/AilGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAilGameMode() {}
// Cross Module References
	AIL_API UClass* Z_Construct_UClass_AAilGameMode();
	AIL_API UClass* Z_Construct_UClass_AAilGameMode_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Ail();
// End Cross Module References
	void AAilGameMode::StaticRegisterNativesAAilGameMode()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AAilGameMode);
	UClass* Z_Construct_UClass_AAilGameMode_NoRegister()
	{
		return AAilGameMode::StaticClass();
	}
	struct Z_Construct_UClass_AAilGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AAilGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Ail,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AAilGameMode_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AAilGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "AilGameMode.h" },
		{ "ModuleRelativePath", "AilGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AAilGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AAilGameMode>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AAilGameMode_Statics::ClassParams = {
		&AAilGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AAilGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AAilGameMode_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_AAilGameMode()
	{
		if (!Z_Registration_Info_UClass_AAilGameMode.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AAilGameMode.OuterSingleton, Z_Construct_UClass_AAilGameMode_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AAilGameMode.OuterSingleton;
	}
	template<> AIL_API UClass* StaticClass<AAilGameMode>()
	{
		return AAilGameMode::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AAilGameMode);
	AAilGameMode::~AAilGameMode() {}
	struct Z_CompiledInDeferFile_FID_Ail_Source_Ail_AilGameMode_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Ail_Source_Ail_AilGameMode_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AAilGameMode, AAilGameMode::StaticClass, TEXT("AAilGameMode"), &Z_Registration_Info_UClass_AAilGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AAilGameMode), 4143346231U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Ail_Source_Ail_AilGameMode_h_732901414(TEXT("/Script/Ail"),
		Z_CompiledInDeferFile_FID_Ail_Source_Ail_AilGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Ail_Source_Ail_AilGameMode_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
