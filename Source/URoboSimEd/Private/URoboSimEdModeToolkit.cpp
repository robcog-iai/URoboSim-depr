// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "URoboSimEdModeToolkit.h"

#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "FURoboSimEdModeToolkit"



FURoboSimEdModeToolkit::FURoboSimEdModeToolkit()
{
}

void FURoboSimEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	static TSharedPtr<SMultiLineEditableTextBox> collisionFilter;

	static TSharedPtr<SCheckBox> enableUROSBridge;

	struct Locals
	{		
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick2()
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();
			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			//FURoboSimEdModule::setCollisionFilter((*collisionFilter).GetText().ToString());			
			//FURoboSimEdModsetCollisionFilter((*collisionFilter).GetText().ToString());
			//testA = "starting";
			//Damping;
			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)

				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}

	};

	const float Factor = 256.0f;
	TAttribute<FSlateColor> InBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f));
	FText input;

	SAssignNew(ToolkitWidget, SBorder)
		.VAlign(VAlign_Top)
		.Padding(25)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Left)
		.Padding(50)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EnableUROSBridgeLabel", "Enable UROSBridge"))
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SCheckBox)
			.ToolTipText(LOCTEXT("EnableUROSBridgeToolTip", "If enabled, will try to connect to a server once game starts."))
		.IsChecked(ECheckBoxState::Checked)
		.OnCheckStateChanged_Static(&FURoboSimEdModeToolkit::SetUROSBridge)
		//.OnCheckStateChanged(this, Locals::SetUROSBridge)
		]
		]


	////Self Collision Filter
	+SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("CollisionFilterLabel", "Collision Filter:"))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SAssignNew(collisionFilter, SMultiLineEditableTextBox)
			.Text(LOCTEXT("CollisionFilterPlaceHolderLabel", "Joint1 Joint2 Joint3..."))
		.ToolTipText(LOCTEXT("CollisionFilterPlaceHolderToolTip", "For example, if the term \"wheel\" is entered, all joints which have that term in their name will have self collision disabled."))
		.BackgroundColor(InBackgroundColor)
		]

		]


	+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(SButton)
			.Text(LOCTEXT("SaveCollisionFilterLabel", "Save Collision Filter"))
		.ToolTipText(LOCTEXT("SaveCollisionFilterToolTip", "During robot generation if any joint name contains an entry from the collision filter, self-collision for that joint will be disabled. Entries can be separated by spaces or a new line. Note: Does not apply to robots which are already in the game."))
		.OnClicked_Static(&Locals::OnButtonClick2)
		]



		];

	FModeToolkit::Init(InitToolkitHost);
}


FName FURoboSimEdModeToolkit::GetToolkitFName() const
{
	return FName("URoboSimEdMode");
}

FText FURoboSimEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("URoboSimEdModeToolkit", "DisplayName", "URoboSimEdMode Tool");
}

class FEdMode* FURoboSimEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FURoboSimEdMode::EM_URoboSimEdModeId);
}

#undef LOCTEXT_NAMESPACE
