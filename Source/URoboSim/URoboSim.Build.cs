// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class URoboSim : ModuleRules
	{
		public URoboSim (TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					"URoboSim/Public"
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
                    "URoboSim/Public",
					"URoboSim/Private"
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
