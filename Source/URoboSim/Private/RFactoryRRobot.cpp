// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RFactoryRRobot.h"
#include "IURoboSim.h"
#include "RRobot.h"


URFactoryRRobot::URFactoryRRobot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// AActor subclass this ActorFactory creates. 
	NewActorClassName = FString("ARRobot");
	NewActorClass = ARRobot::StaticClass();
}

bool URFactoryRRobot::CanCreateActorFrom(const FAssetData & AssetData, FText & OutErrorMsg)
{
	// Only designed for RURDFData Asset. 
	return AssetData.AssetClass.IsEqual(FName("RURDFData"));
}

AActor* URFactoryRRobot::GetDefaultActor(const FAssetData & AssetData)
{
	return NewActorClass->GetDefaultObject<ARRobot>();
}


AActor* URFactoryRRobot::SpawnActor(UObject* Asset, ULevel* InLevel, const FTransform & Transform, EObjectFlags InObjectFlags, const FName Name)
{
	AActor* DefaultActor = GetDefaultActor(FAssetData(Asset));
	if (DefaultActor)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.OverrideLevel = InLevel;
		SpawnInfo.ObjectFlags = InObjectFlags;
		SpawnInfo.Name = Name;

		// Creates RRobot Actor. 
		ARRobot* NewRobot = (ARRobot*)InLevel->OwningWorld->SpawnActor(DefaultActor->GetClass(), &Transform, SpawnInfo);

		// Searches for the XML from the asset and delivers it to the robot.
		NewRobot->XmlUrdf = FAssetData(Asset).GetTagValueRef<FString>(FName("Xml"));
		NewRobot->ParseURDF();

		return NewRobot;
	}

	// Creates RRobot Actor. 
	UE_LOG(LogTemp, Warning, TEXT("No default Robot Actor available\n"));
	return nullptr;
}


