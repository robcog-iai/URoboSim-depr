// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

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
