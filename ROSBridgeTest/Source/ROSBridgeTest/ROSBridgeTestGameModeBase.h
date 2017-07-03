// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "ROSBridgeHandler.h"
#include "ROSStringSubscriber.h"
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

    void StartPlay() override
    {
        Super::BeginPlay();
        Handler = new FROSBridgeHandler(TEXT("127.0.0.1"), 9001);
        UE_LOG(LogTemp, Log, TEXT("Handler Created. "));
        Handler->Connect();
        UE_LOG(LogTemp, Log, TEXT("Connected to WebSocket server. "));

        Handler->AddSubscriber(new UROSStringSubScriber(TEXT("/chatter")));
        UE_LOG(LogTemp, Log, TEXT("Added chatter subscriber. "));
    }

    void Logout(AController *Exiting) override
    {
        AGameModeBase::Logout(Exiting);
        UE_LOG(LogTemp, Log, TEXT("On Logout() function. "));
        // UE_LOG(LogTemp, Log, TEXT("SharedPtr RefCount = %d. "), Handler.GetSharedReferenceCount());
        Handler->Disconnect();
        delete Handler;
        UE_LOG(LogTemp, Log, TEXT("After Logout() function. "));
    }

};
