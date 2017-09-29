// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RRobot.h"
#include "RTestRobotMotors.generated.h"

/**
 * 
 */
UCLASS()
class UROBOSIM_API ARTestRobotMotors : public ARRobot
{
	GENERATED_BODY()

	ARTestRobotMotors();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Target rotation for motor of second_to_third_joint
	UPROPERTY(EditAnywhere)
	FRotator TargetRotation;

	// Target location for motor of first_to_second_joint
	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

};
