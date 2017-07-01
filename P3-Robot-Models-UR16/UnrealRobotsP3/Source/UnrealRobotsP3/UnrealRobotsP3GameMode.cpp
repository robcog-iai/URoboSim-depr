// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnrealRobotsP3.h"
#include "UnrealRobotsP3GameMode.h"
#include "UnrealRobotsP3HUD.h"
#include "UnrealRobotsP3Character.h"

AUnrealRobotsP3GameMode::AUnrealRobotsP3GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUnrealRobotsP3HUD::StaticClass();
}
