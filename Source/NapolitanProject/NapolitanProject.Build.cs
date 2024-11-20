// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NapolitanProject : ModuleRules
{
	public NapolitanProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "UMG", "NavigationSystem"});
	}
}
