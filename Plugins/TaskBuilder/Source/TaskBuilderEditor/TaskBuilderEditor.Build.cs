// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TaskBuilderEditor : ModuleRules
{
	public TaskBuilderEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			});
			
			PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Engine",
                "CoreUObject",
                "UnrealEd",
                "AIGraph",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "GraphEditor",
                "BlueprintGraph",
                "PropertyEditor",
                "ToolMenus",
                "InputCore",
				"AssetTools",
                "Kismet",
                "KismetCompiler",
            });
	}
}
