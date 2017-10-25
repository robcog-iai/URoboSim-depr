// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RFactoryRURDFData.h"
#include "IURoboSim.h"
#include "RURDFData.h"
#include "Factories/FbxFactory.h"
#include "AssetRegistryModule.h"


URFactoryRURDFData::URFactoryRURDFData(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// If the factory can currently create a new object from scratch.
	bCreateNew = false;

	// True if the factory imports objects from text.
	bText = true;

	// True if the factory imports objects from files.
	bEditorImport = true;

	// Tells the Editor which file types we can import.
	Formats.Add(TEXT("xml;XML Files"));
	Formats.Add(TEXT("urdf;URDF Files"));

	// Tells the Editor which Asset type this UFactory can import.
	SupportedClass = URURDFData::StaticClass();

	// Increases the factory priority.
	ImportPriority = DefaultImportPriority - 1;

	// For reimport.
	bIsReimport = false;
}

URFactoryRURDFData::~URFactoryRURDFData() 
{
}

UObject* URFactoryRURDFData::FactoryCreateText(UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	const TCHAR* Type,
	const TCHAR*& Buffer,
	const TCHAR* BufferEnd,
	FFeedbackContext* Feed)
{
	if (!DoesSupportClass(InClass)) return nullptr;
		
	// For reimport via import button.
	TArray <FAssetData> SelectionList;

	FString PathPart, FilenamePart, ExtensionPart;
	FPaths::Split(InParent->GetPathName(), PathPart, FilenamePart, ExtensionPart);

	FAssetRegistryModule& AssetRegistryModule
		= FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	AssetRegistry.GetAssetsByPath(*PathPart, SelectionList, false, false);

	// New UAsset for RRobot. 
	URURDFData* NewObjectAsset = NewObject<URURDFData>(InParent, InName, Flags | RF_Transactional);

	if (SelectionList.Num() != 0) {
		for (auto Item : SelectionList) {
			if (Item.ObjectPath.IsEqual(*(NewObjectAsset->GetPathName(nullptr)))) {
				bIsReimport = true;
				break;
			}
		}
	}
	NewObjectAsset->Xml = AdeptXml(FPaths::GetPath(GetCurrentFilename()), Buffer, Flags, InParent);
	CleanUp();

	// Factory only generates reimports (drag and drop, import button creates new factory).
	bIsReimport = true;

	// Return the Asset.
	return NewObjectAsset;
}

bool URFactoryRURDFData::DoesSupportClass(UClass* Class)
{
	return Class->IsChildOf(SupportedClass);
}

bool URFactoryRURDFData::FactoryCanImport(const FString & Filename)
{
	return (Filename.EndsWith(".xml", ESearchCase::IgnoreCase)) ? true : Filename.EndsWith(".urdf", ESearchCase::IgnoreCase);
}

bool URFactoryRURDFData::CanReimport(UObject* Object, TArray<FString>& OutFilenames)
{
	URURDFData* Asset = Cast<URURDFData>(Object);
	if (Asset)
	{
		const FString &Filename = Asset->GetAssetFilename();
		if (!Filename.IsEmpty())
		{
			OutFilenames.Add(Filename);
		}

		return true;
	}
	return false;
}

void URFactoryRURDFData::SetReimportPaths(UObject* Object, const TArray<FString>& NewReimportPaths)
{
	URURDFData* Asset = Cast<URURDFData>(Object);

	if (Asset && ensure(NewReimportPaths.Num() == 1))
	{
		Asset->SetAssetFilename(NewReimportPaths[0]);

		// For reimport in a new session (Editor - Drag and drop).
		bIsReimport = true;
	}
}

EReimportResult::Type URFactoryRURDFData::Reimport(UObject* Object)
{
	if (ImportSourceFile(Cast<URURDFData>(Object)))
	{
		// Find the outer package so we cam make it dirty.
		if (Object->GetOuter())
		{
			Object->GetOuter()->MarkPackageDirty();
		}
		else
		{
			Object->MarkPackageDirty();
		}
		return EReimportResult::Succeeded;
	}
	else
	{
		return EReimportResult::Failed;
	}
}

bool URFactoryRURDFData::ImportSourceFile(URURDFData* ForAsset) const
{

	FString AssetFilename = ForAsset->GetAssetFilename();
	if (ForAsset == nullptr || AssetFilename.IsEmpty())
	{
		return false;
	}

	FString ReadString;
	if (!FFileHelper::LoadFileToString(ReadString, *AssetFilename, FFileHelper::EHashOptions::None))
	{
		return false;
	}
	// Passing the XML.
	ForAsset->SetXml(AdeptXml(FPaths::GetPath(AssetFilename),
		ReadString, ForAsset->GetFlags(), ForAsset->GetOuter()));

	return true;
}

FString URFactoryRURDFData::AdeptXml(const FString PathToXml,
	const FString ConvertXml, const EObjectFlags Flags,
	UObject* InOuter) const
{
	//To create the uAsset meshes from .fbx.
	TMap<FString, FString> MeshMap;

	// New XML has the adapted UAsset path and is used for building.
	FString NewXml = "";

	TArray<FString> XMLInLines;
	FString(ConvertXml).ParseIntoArrayLines(XMLInLines, TEXT(">"));
	bool bInComment = false;

	for (FString Line : XMLInLines) {

		// Checks if .fbx is in a comment.
		if (Line.Contains("<!--")) {
			bInComment = true;
		}
		if (Line.Contains("-->")) {
			bInComment = false;
		}

		if (Line.Contains("mesh filename") && (Line.Contains("fbx") || Line.Contains("stl") || Line.Contains("dae"))&& !bInComment) {

			// Standardizes the file paths for the Xml
			Line = Line.Replace((L"\\"), (L"/"), ESearchCase::IgnoreCase);
			Line = Line.Replace((L"package://"), (L"/"), ESearchCase::IgnoreCase);
			Line = Line.Replace((L".stl"), (L".fbx"), ESearchCase::IgnoreCase);
			Line = Line.Replace((L".dae"), (L".fbx"), ESearchCase::IgnoreCase);
			FString SaveLine = Line;

			// Delete everything before the path.
			Line.RemoveAt(0, Line.Find("\"") + 1, true);

			FString PathToFbx = Line;

			// Deletes information to get the path. 
			Line.RemoveAt(0, Line.Find("\""), true);
			PathToFbx = PathToFbx.Replace(*Line, (L""), ESearchCase::IgnoreCase);

			// FString that stores the mesh name. Only fbx files are allowed.
			FString FbxName = FPaths::GetBaseFilename(PathToFbx, true);

			// New Path for the UAsset.
			FString AssetPath = InOuter->GetPathName() + "/" + FbxName + "." + FbxName;

			// Replaces the old path to the fbx to the new fbx.
			SaveLine = SaveLine.Replace(*PathToFbx, *AssetPath, ESearchCase::IgnoreCase);

			Line = SaveLine;

			if (!MeshMap.Contains(FbxName)) {

				// if for /Meshes/myfbx,fbx ... else for full path 
				int32 Index = 0;
				if (PathToFbx.StartsWith(FString("/"), ESearchCase::IgnoreCase)) {

					MeshMap.Add(FbxName, PathToXml + PathToFbx);
				}
				else {
					MeshMap.Add(FbxName, PathToFbx);
				}
			}
		}
		// Creates the new XML.
		NewXml = NewXml + Line + "\n";
	}
	// If .fbx files should be loaded.
	if (MeshMap.Num() > 0) {

		// Only at Reimport. 
		if (!bIsReimport ? true : FMessageDialog::Open(EAppMsgType::YesNo,
			FText::FromString("Do you want to load the .fbx files?"), nullptr) == EAppReturnType::Yes) {

			UFbxFactory* Factory
				= NewObject<UFbxFactory>(UFbxFactory::StaticClass(), FName("FBXFactory"), RF_NoFlags);
			Factory->ReinitializeProperties();

			// If options are to be offered before loading.
			Factory->EnableShowOption();

			const uint8* NewBuffer = 0;
			bool bAndOutOperationCancelled = false;

			UPackage* PackageForMeshes
				= CreatePackage(InOuter->GetOuter(), *(InOuter->GetPathName(nullptr) + "/"));
			PackageForMeshes->SetFlags(RF_Standalone | RF_Public);
			PackageForMeshes->SetFolderName(FName(*(InOuter->GetPathName() + "/")));

			// If a .fbx path is wrong (for the output).
			FString FbxNotLoadedList = "";

			for (TPair<FString, FString>& Elem : MeshMap) {

				if (IFileManager::Get().FileExists(*Elem.Value)) {

					// The FBXImporter works with this .fbx path.
					Factory->CurrentFilename = Elem.Value;

					UObject* Mesh = Factory->FactoryCreateBinary(UStaticMesh::StaticClass(),
						PackageForMeshes, FName(*Elem.Key), Flags | RF_Transactional,
						nullptr, TEXT("fbx"), NewBuffer, nullptr,
						GWarn, bAndOutOperationCancelled);

					// If Operation Cancelled
					if (bAndOutOperationCancelled) {
						break;
					}
					FAssetRegistryModule::AssetCreated(Mesh);
					Mesh->MarkPackageDirty();
				}
				else {
					FbxNotLoadedList = FbxNotLoadedList + "The .fbx " + Elem.Key + " :: " + Elem.Value + "\n";
				}
			}
			// Dialog with list of faulty .fbx paths.
			if (!FbxNotLoadedList.IsEmpty()) {
				FText FbxErrorText = FText::FromString(FbxNotLoadedList + "could not be found.");
				FMessageDialog::Debugf(FbxErrorText, nullptr);
			}
		}
	}
	// Return the new Xml. 
	return NewXml;
}


