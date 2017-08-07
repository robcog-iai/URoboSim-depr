// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "IUnrealRobots.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealRobots : public IUnrealRobots
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FUnrealRobots, UnrealRobots )



void FUnrealRobots::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FUnrealRobots::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

DEFINE_LOG_CATEGORY(LogRobot);
