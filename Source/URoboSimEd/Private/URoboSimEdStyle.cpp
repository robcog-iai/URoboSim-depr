// Copyright 2017, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "URoboSimEdStyle.h"
#include "URoboSimEdModule.h"
#include "SlateGameResources.h"
#include "IPluginManager.h"
#include "Styling/SlateTypes.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FURoboSimEdStyle::StyleSetInstance = NULL;

void FURoboSimEdStyle::Initialize()
{
	if (!StyleSetInstance.IsValid())
	{
		StyleSetInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSetInstance);
	}
};

void FURoboSimEdStyle::Shutdown()
{
	if (StyleSetInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSetInstance);
		ensure(StyleSetInstance.IsUnique());
		StyleSetInstance.Reset();
	}
}

FName FURoboSimEdStyle::GetStyleSetName()
{
	static FName RoboSimStyleName(TEXT("URoboSimEdStyle"));
	return RoboSimStyleName;
}

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FURoboSimEdStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("URoboSim"))->GetContentDir());

	Style->Set("LevelEditor.URoboSimEd", new IMAGE_BRUSH(TEXT("Icons/icon_Mode_URoboSim_40px"), Icon40x40));
	Style->Set("LevelEditor.URoboSimEd.Small", new IMAGE_BRUSH(TEXT("Icons/icon_Mode_URoboSim_40px"), Icon20x20));

	return Style;
}

#undef IMAGE_BRUSH

void FURoboSimEdStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FURoboSimEdStyle::Get()
{
	return *StyleSetInstance;
}