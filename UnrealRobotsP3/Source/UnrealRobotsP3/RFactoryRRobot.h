// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealEd.h"
#include "RFactoryRRobot.generated.h"

/**
* Own ActorFactory which creates a robot from URURDFData Asset.
*/
UCLASS()
class UNREALROBOTSP3_API URFactoryRRobot : public UActorFactory
{
	GENERATED_BODY()

private:
	URFactoryRRobot(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR

	/** 
	* Checks if AssetData is intended for the factory.
	* Checks if an RRobot can be built by AssetData.
	*/
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;

	/** Initialize NewActorClass if necessary, and return default actor for that class. */
	virtual AActor* GetDefaultActor(const FAssetData& AssetData) override;

	/** Spawns the robot */
	virtual AActor* SpawnActor(UObject* Asset, ULevel* InLevel, const FTransform& Transform, EObjectFlags ObjectFlags, const FName Name) override;
#endif
};
