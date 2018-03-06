// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

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

		static void SetUROSBridge(ECheckBoxState NewCheckedState) {			
			if (IURoboSimEd::IsAvailableEd()) {
				FURoboSimEdModule& placeHolder = IURoboSimEd::GetEd();
				placeHolder.bEnableUROSBridge = (NewCheckedState == ECheckBoxState::Checked) ? true : false;
			}			
		}

		static void SetShapeCollision(ECheckBoxState NewCheckedState) {
			if (IURoboSimEd::IsAvailableEd()) {
				FURoboSimEdModule& placeHolder = IURoboSimEd::GetEd();
				placeHolder.bEnableShapeCollisions = (NewCheckedState == ECheckBoxState::Checked) ? true : false;
			}
		}

		static void SetAngularMotors(ECheckBoxState NewCheckedState) {
			if (IURoboSimEd::IsAvailableEd()) {
				FURoboSimEdModule& placeHolder = IURoboSimEd::GetEd();
				placeHolder.bEnableAngularMotors = (NewCheckedState == ECheckBoxState::Checked) ? true : false;
			}
		}

		static void SetCollisionTags(ECheckBoxState NewCheckedState) {
			if (IURoboSimEd::IsAvailableEd()) {
				FURoboSimEdModule& placeHolder = IURoboSimEd::GetEd();
				placeHolder.bEnableCollisionTags = (NewCheckedState == ECheckBoxState::Checked) ? true : false;
			}
		}

		static FReply OnButtonClickSaveFilter()
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();
			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("SaveCollisionFilterName", "SaveCollisionFilter"));


			if (IURoboSimEd::IsAvailableEd()) {
				FURoboSimEdModule& placeHolder = IURoboSimEd::GetEd();
				(*collisionFilter).GetText().ToString().ParseIntoArray(placeHolder.collisionFilterArr, TEXT(" "), true);								
			}

			GEditor->EndTransaction();

			return FReply::Handled();
		}
	};

	TAttribute<FSlateColor> InBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f));

	SAssignNew(ToolkitWidget, SBorder)
		.VAlign(VAlign_Top)
		.Padding(25)
		[
			SNew(SVerticalBox)

	//Enable UROSBridge-----------------
			+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(5)
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
		.IsChecked(ECheckBoxState::Unchecked)
		.OnCheckStateChanged_Static(&Locals::SetUROSBridge)
		]
		]
	//-------------------------------------

	//Enable Shape Collisions-----------------
	+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EnableShapeCollisionsLabel", "Enable Shape Collisions"))
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SCheckBox)
			.ToolTipText(LOCTEXT("EnableShapeCollisionsToolTip", "If enabled, URDF shapes will have collisions too."))
		.IsChecked(ECheckBoxState::Unchecked)
		.OnCheckStateChanged_Static(&Locals::SetShapeCollision)
		]
		]
	//-------------------------------------


	//AngularMotors-------------------
	+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EnableAngularMotorLabel", "Enable AngularMotors"))
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SCheckBox)
			.ToolTipText(LOCTEXT("EnableAngularMotorsToolTip", "If enabled, will use UE4's built in angular motors for more rigid links. Should not be used with UROSBridge."))
		.IsChecked(ECheckBoxState::Unchecked)
		.OnCheckStateChanged_Static(&Locals::SetAngularMotors)
		]
		]
	//-----------------------------

	//CollisionTags-------------------
	+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EnableCollisionTagsLabel", "Enable Collision Tags"))
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SCheckBox)
			.ToolTipText(LOCTEXT("EnableCollisionTagsToolTip", "If enabled, parser will also try to use information in URDF collision tags."))
		.IsChecked(ECheckBoxState::Unchecked)
		.OnCheckStateChanged_Static(&Locals::SetCollisionTags)
		]
		]
	//-----------------------------

	//Self Collision Filter----------------
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
			.Text(LOCTEXT("CollisionFilterPlaceHolderLabel", "wheel_link shoulder arm finger_link"))
		.ToolTipText(LOCTEXT("CollisionFilterPlaceHolderToolTip", "For example, if the term \"wheel\" is entered, all joints which have that term in their name will have self collision disabled."))
		.BackgroundColor(InBackgroundColor)
		]

		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(10)
		[
			SNew(SButton)
			.Text(LOCTEXT("SaveCollisionFilterLabel", "Save Collision Filter"))
		.ToolTipText(LOCTEXT("SaveCollisionFilterToolTip", "During robot generation if any joint name contains an entry from the collision filter, self-collision for that joint will be disabled. Entries can be separated by spaces or a new line. Note: Does not apply to robots which are already in the game."))
		.OnClicked_Static(&Locals::OnButtonClickSaveFilter)
		]
	//--------------------------------



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
