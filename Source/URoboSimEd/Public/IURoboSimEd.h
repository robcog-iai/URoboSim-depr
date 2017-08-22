// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "URoboSimEd.h"
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

/**
* The public interface to this module
*/
class IURoboSimEd : public IModuleInterface
{

public:
	static FString someString;
	FString someString2;

	FString getTest2() {
		return someString2;
	}

	void setTest2(FString s) {
		someString2 = s;
	}

	static FString getTest() {		
		return someString;
	}

	static void setTest(FString& s) {
		someString = s;
	}

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FURoboSimEdModule& GetEd()
	{
		return FModuleManager::LoadModuleChecked< FURoboSimEdModule >("URoboSimEd");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailableEd()
	{
		return FModuleManager::Get().IsModuleLoaded("URoboSimEd");
	}
};

//DECLARE_LOG_CATEGORY_EXTERN(LogRobot, Log, All);

