// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "URoboSimEdMode.h"
#include "URoboSimEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FURoboSimEdMode::EM_URoboSimEdModeId = TEXT("EM_URoboSimEdMode");

FURoboSimEdMode::FURoboSimEdMode()
{
}

FURoboSimEdMode::~FURoboSimEdMode()
{
}

void FURoboSimEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FURoboSimEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FURoboSimEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FURoboSimEdMode::UsesToolkits() const
{
	return true;
}




