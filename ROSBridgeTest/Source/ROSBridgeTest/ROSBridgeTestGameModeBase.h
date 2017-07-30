// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/GameModeBase.h"
#include "Core.h"
#include "Engine.h"

#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "ROSStringSubscriber.h"
#include "ROSAddTwoIntsClient.h"

#include "sensor_msgs/JointState.h"
#include "std_msgs/Header.h"

#include "ROSBridgeTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ROSBRIDGETEST_API AROSBridgeTestGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
	
public:
    FROSBridgeHandler* Handler;
    FROSStringSubScriber* Subscriber;
    FROSBridgePublisher* Publisher;
    FROSAddTwoIntsClient* ServiceClient;

    AROSBridgeTestGameModeBase(const FObjectInitializer &ObjectInitializer) :
        AGameModeBase(ObjectInitializer){
        PrimaryActorTick.bCanEverTick = true;
        PrimaryActorTick.bStartWithTickEnabled = true;
    }

    void BeginPlay() override;
    void Tick(float DeltaSeconds) override;
    void Logout(AController *Exiting) override;

};
