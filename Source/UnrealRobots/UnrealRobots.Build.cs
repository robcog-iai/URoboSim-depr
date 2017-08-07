// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class UnrealRobots : ModuleRules
	{
		public UnrealRobots (TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					"UnrealRobots/Public"
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
                    "UnrealRobots/Public",
					"UnrealRobots/Private"
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "XmlParser", "UnrealEd"
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "XmlParser", "UnrealEd"
                }
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
				}
				);
		}
	}
}
