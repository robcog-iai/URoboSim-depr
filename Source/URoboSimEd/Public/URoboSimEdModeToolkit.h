// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FURoboSimEdModeToolkit : public FModeToolkit
{
public:
	//FURoboSim_GUIModule::EnableUROSBridge = true;

	static void SetUROSBridge(ECheckBoxState NewCheckedState) {
		//enableUROSBridge = !enableUROSBridge;
 	}

	static void SetCollisionFilter(FString collisionFilterString) {
		//collisionFilterString.ParseIntoArray(collisionFilter, TEXT(" "), true);
 	}

	FURoboSimEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:

	TSharedPtr<SWidget> ToolkitWidget;
};
