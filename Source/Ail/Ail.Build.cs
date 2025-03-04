// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ail : ModuleRules
{
    public Ail(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core","CoreUObject","Engine","InputCore","EnhancedInput",
            "GeometryScriptingCore","GeometryFramework",//"GeometryProcessing" , //다이나믹 매쉬
        });

        PublicDependencyModuleNames.Add("GeometryCore");


        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[]
        {
            this.Name
        });
    }

}
