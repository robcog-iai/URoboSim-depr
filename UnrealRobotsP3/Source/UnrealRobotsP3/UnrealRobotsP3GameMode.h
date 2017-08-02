// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"

#include "Core.h"
#include "Engine.h"



#include "RRobot.h"


#include "UnrealRobotsP3GameMode.generated.h"

//This class is selected to run on startup in DefaultEngine.ini


UCLASS(minimalapi)
class AUnrealRobotsP3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealRobotsP3GameMode();
	
};



