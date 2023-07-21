// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonStoryEditor : ModuleRules
{
    public CommonStoryEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
				// ... add other public dependencies that you statically link with here ...
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
				// ... add private dependencies that you statically link with here ...	
                "AssetTools",
                "UnrealEd",
                "ToolMenus",
                "GraphEditor",
                "ApplicationCore",
                "GameplayTags",
                "ContentBrowser",
                "ClassViewer",
                "AssetRegistry",
                "KismetWidgets",

                "AIGraph",        //@Todo: 暂时借用一下它的Class搜集器
                "CommonStoryRuntime",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
