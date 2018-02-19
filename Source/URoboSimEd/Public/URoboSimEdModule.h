// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "URoboSimEdMode.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FURoboSimEdModule : public IModuleInterface
{
public:	
	// Provided for convenience, it will need to be implemented outside of URoboSim (i.e. in UROSBridge or in project source)
	bool bEnableUROSBridge = false; 
	// Should match the default state of the corresponding check box (i.e. if unchecked, this should be set to false)
	bool bEnableAngularMotors = false; 
	bool bEnableCollisionTags = false; 
	bool bEnableShapeCollisions = false;

	TArray<FString> collisionFilterArr = { "wheel_link", "shoulder", "arm", "finger_link" }; //initialized with default settings for pr2 

	static FString SomeVariable;
	static FString GetSomeVariable()
	{
		return SomeVariable;
	}

	static TArray<FString> CollisionFilter;
	static TArray<FString> GetCollisionFilter() {
		return CollisionFilter;
	}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};




