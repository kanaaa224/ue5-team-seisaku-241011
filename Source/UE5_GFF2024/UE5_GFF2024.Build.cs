// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5_GFF2024 : ModuleRules
{
	public UE5_GFF2024(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule", "GameplayTasks", "NavigationSystem", "Niagara" });
	}
}
