// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiMagicDungeon : ModuleRules
{
	public MultiMagicDungeon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Expose subdirectories so #include "Core/Foo.h" works across the module.
		PrivateIncludePaths.AddRange(new string[]
		{
			ModuleDirectory,
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			// Networking
			"NetCore",
			"OnlineSubsystem", "OnlineSubsystemUtils",
			// Gameplay Ability System
			"GameplayAbilities", "GameplayTags", "GameplayTasks",
		});
	}
}
