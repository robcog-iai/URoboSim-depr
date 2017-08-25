// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "IURoboSimEd.h"
#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FURoboSimEdModeToolkit : public FModeToolkit
{
public:


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
