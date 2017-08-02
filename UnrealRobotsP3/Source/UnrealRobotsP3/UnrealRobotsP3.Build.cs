// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealRobotsP3 : ModuleRules
{
	public UnrealRobotsP3(TargetInfo Target)
	{

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "UnrealRobotsP3/Public", "ROSBridgePlugin/Public", "ROSBridgePlugin/Classes" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "XmlParser", "UnrealEd", "ROSBridgePlugin", "Json", "JsonUtilities" });

        PrivateDependencyModuleNames.AddRange(new string[] { "ROSBridgePlugin" });
    }
}
