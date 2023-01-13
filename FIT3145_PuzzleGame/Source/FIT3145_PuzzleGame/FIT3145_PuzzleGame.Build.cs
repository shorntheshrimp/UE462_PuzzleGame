// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FIT3145_PuzzleGame : ModuleRules
{
	public FIT3145_PuzzleGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
	}
}
