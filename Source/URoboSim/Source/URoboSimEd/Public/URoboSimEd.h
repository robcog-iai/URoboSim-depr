#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

static bool enableUROSBridge;
class FURoboSimEdModule : public IModuleInterface
{
public:

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FURoboSimEdModule& GetEdModule()
	{
		return FModuleManager::LoadModuleChecked< FURoboSimEdModule >("URoboSimEd");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailableEdModule()
	{
		return FModuleManager::Get().IsModuleLoaded("URoboSimEd");
	}

	static FString someVariable;

	static FString getSomeVariable()
	{
		return someVariable;
	}


	

	static TArray<FString> collisionFilter;

	static TArray<FString> getCollisionFilter() {
		return collisionFilter;
	}

	//static void setCollisionFilter(FString collisionFilterString) {
		//collisionFilterString.ParseIntoArray(collisionFilter, TEXT(" "), true);
	//}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};




