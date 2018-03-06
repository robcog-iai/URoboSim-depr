// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "URoboSimEdModule.h"
#include "URoboSimEdMode.h"
#include "URoboSimEdStyle.h"

#define LOCTEXT_NAMESPACE "FURoboSimEdModule"

void FURoboSimEdModule::StartupModule()
{
	// Register slate style overrides
	FURoboSimEdStyle::Initialize();
	FURoboSimEdStyle::ReloadTextures();

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FURoboSimEdMode>(
		FURoboSimEdMode::EM_URoboSimEdModeId,
		LOCTEXT("URoboSimEdModeName", "URoboSim"),
		FSlateIcon(FURoboSimEdStyle::Get().GetStyleSetName(),
		"LevelEditor.URoboSimEd", 
		"LevelEditor.URoboSimEd.Small"),
		true);

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module	
 	//FEditorModeRegistry::Get().RegisterMode<FURoboSimEdMode>(FURoboSimEdMode::EM_URoboSimEdModeId, LOCTEXT("URoboSimEdModeName", "URoboSimEdMode"), FSlateIcon(), true);
}

void FURoboSimEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FURoboSimEdMode::EM_URoboSimEdModeId);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FURoboSimEdModule, URoboSimEd)