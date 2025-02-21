// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ail : ModuleRules
{
    public Ail(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core","CoreUObject","Engine","InputCore","EnhancedInput",
            "GeometryScriptingCore","GeometryFramework","DynamicMesh", //다이나믹 매쉬
            "MeshDescription",       // ✅ 추가 (FMeshDescription 관련 모듈)
            "StaticMeshDescription",  // ✅ 추가 (FStaticMeshAttributes 관련 모듈)

        });


        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[]
        {
            this.Name
        });
    }

}
