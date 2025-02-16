// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Ail/AilGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAilGameMode() {}

// Begin Cross Module References
AIL_API UClass* Z_Construct_UClass_AAilGameMode();
AIL_API UClass* Z_Construct_UClass_AAilGameMode_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UPackage* Z_Construct_UPackage__Script_Ail();
// End Cross Module References

// Begin Class AAilGameMode
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
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "AilGameMode.h" },
		{ "ModuleRelativePath", "AilGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AAilGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AAilGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_Ail,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AAilGameMode_Statics::DependentSingletons) < 16);
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
// End Class AAilGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AAilGameMode, AAilGameMode::StaticClass, TEXT("AAilGameMode"), &Z_Registration_Info_UClass_AAilGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AAilGameMode), 3371214683U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_2566648614(TEXT("/Script/Ail"),
	Z_CompiledInDeferFile_FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Coding_Unreal_Ail_Source_Ail_AilGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
