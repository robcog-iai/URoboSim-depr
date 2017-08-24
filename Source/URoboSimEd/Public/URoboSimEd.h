#pragma once

#include "CoreMinimal.h"
#include "URoboSimEdMode.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"



class FURoboSimEdModule : public IModuleInterface
{
public:
	
	
	bool bEnableUROSBridge = false; //provided for convenience, it will need to be implemented outside of URoboSim (ie in UROSBridge or in project source)
	bool bEnableAngularMotors = false; //should match the default state of the corresponding checkbox (ie if unchecked, this should be set to false)
	bool bEnableCollisionTags = false; 
	bool bEnableShapeCollisions = false;

	TArray<FString> collisionFilterArr = { "wheel_link", "shoulder", "arm", "finger_link" }; //initialized with default settings for pr2 

	static FString someVariable;

	static FString getSomeVariable()
	{
		return someVariable;
	}


	

	static TArray<FString> collisionFilter;

	static TArray<FString> getCollisionFilter() {
		return collisionFilter;
	}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};




