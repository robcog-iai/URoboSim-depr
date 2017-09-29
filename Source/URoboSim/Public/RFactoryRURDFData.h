// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "UnrealEd.h"
#include "RURDFData.h"
#include "RFactoryRURDFData.generated.h"

/**
 * This class provides the factory and reimport functionality to read an XML (URDF) file.
 * The generated uAsset is a RURDFData for further processing.
 */
UCLASS()
class UROBOSIM_API URFactoryRURDFData : public UFactory, public FReimportHandler
{
	GENERATED_BODY()
	
private:

	URFactoryRURDFData(const FObjectInitializer& ObjectInitializer);
	virtual ~URFactoryRURDFData();

#if WITH_EDITOR
	// Begin UFactory Interface
	
	/**
	* Creates the RURDFData (uAsset). The overridden factory method generates the RURDFData,
	* checks for reimport, and passes the converted XML to the new asset.
	*/
	virtual UObject* FactoryCreateText(UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		const TCHAR* Type,
		const TCHAR*& Buffer,
		const TCHAR* BufferEnd,
		FFeedbackContext* Warn) override;

	/** True if it supports this class. */
	virtual bool DoesSupportClass(UClass* Class) override;

	// End UFactory Interface


	// Begin FReimportHandler interface

	/*
	* Functions of the ReimportHandler (override). Allows reimporting via drag and drop.
	*/
	virtual bool FactoryCanImport(const FString& Filename) override;
	virtual bool CanReimport(UObject* Object, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Object, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Object) override;

	// End FReimportHandler interface

	/** Loads information for Reimport. 
	* @param ForAsset Holds the Information for reimport. 
	* @return true if data could be loaded. Otherwise false.
	*/
	bool ImportSourceFile(class URURDFData* ForAsset) const;

	/** Edits Xml and loads .fbx files.
	* @param ConvertXml Holds the xml content (URDF).
	* @param Flags Flags of the action.
	* @param InOuter To save the location.
	* @return true if data could be loaded. Otherwise false.	
	*/
	FString AdeptXml(const FString PathToXml, const FString ConvertXml, const EObjectFlags Flags, UObject* InOuter) const;

	// To decide whether it is a reimport.
	bool bIsReimport;
#endif
};

