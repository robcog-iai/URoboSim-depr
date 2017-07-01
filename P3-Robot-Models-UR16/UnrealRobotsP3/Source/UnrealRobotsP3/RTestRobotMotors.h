// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RRobot.h"
#include "RTestRobotMotors.generated.h"

/**
 * 
 */
UCLASS()
class UNREALROBOTSP3_API ARTestRobotMotors : public ARRobot
{
	GENERATED_BODY()

	ARTestRobotMotors();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
		// Target rotation for motor of second_to_third_joint
		FRotator TargetRotation;

	UPROPERTY(EditAnywhere)
		// Target location for motor of first_to_second_joint
		FVector TargetLocation;

};
