// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RURDFData.h"
#include "IURoboSim.h"
#include "EditorFramework/AssetImportData.h"

void URURDFData::SetAssetFilename(const FString & NewFilename)
{
	AssetImportData->UpdateFilenameOnly(NewFilename);

	TArray<FString> Filenames;
	AssetImportData->ExtractFilenames(Filenames);

	// Set Assetfilename if in Filenames
	if (Filenames.Num() > 0)
	{
		Assetfilename = Filenames[0];
	}
}

void URURDFData::SetXml(const FString NewXml)
{
	Xml = NewXml;
}


FString URURDFData::GetAssetFilename() const
{
	TArray<FString> Filenames;
	if (AssetImportData)
	{
		// ExtractFilenames and Get the AssetFilename
		AssetImportData->ExtractFilenames(Filenames);	
	}
	if (Filenames.Num() > 0)
	{
		return Filenames[0];
	}
	else
	{
		return Assetfilename;
	}
}

void  URURDFData::PostInitProperties()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}
	Super::PostInitProperties();
}
