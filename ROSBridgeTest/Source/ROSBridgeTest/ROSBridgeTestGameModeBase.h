// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Engine.h"

#include "ROSBridgeActor.h"
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
    FROSStringSubScriber* Subscriber;

    AROSBridgeTestGameModeBase(const FObjectInitializer &ObjectInitializer) :
        AGameModeBase(ObjectInitializer){
        PrimaryActorTick.bCanEverTick = true;
        PrimaryActorTick.bStartWithTickEnabled = true;
    }

    void BeginPlay() override
    {
        Super::BeginPlay();
        AROSBridgeActor* Actor = GetWorld()->SpawnActor<AROSBridgeActor>(AROSBridgeActor::StaticClass());
        Handler = new FROSBridgeHandler(TEXT("127.0.0.1"), 9001);
        UE_LOG(LogTemp, Log, TEXT("Handler Created. "));

        Subscriber = new FROSStringSubScriber(TEXT("/chatter"));
        Handler->AddSubscriber(Subscriber);
        UE_LOG(LogTemp, Log, TEXT("Added chatter subscriber. "));

        Handler->Connect();
        UE_LOG(LogTemp, Log, TEXT("Connected to WebSocket server. "));
    }

    void Tick(float DeltaSeconds) override {
        Super::Tick(DeltaSeconds);
        UE_LOG(LogTemp, Log, TEXT("GameMode Ticks, DeltaSeconds = %.3f."), DeltaSeconds);
        Handler->Render();
    }

    void Logout(AController *Exiting) override
    {
        UE_LOG(LogTemp, Log, TEXT("On Logout() function. "));
        // UE_LOG(LogTemp, Log, TEXT("SharedPtr RefCount = %d. "), Handler.GetSharedReferenceCount());
        Handler->Disconnect();
        // delete Handler;
        // delete Subscriber;
        UE_LOG(LogTemp, Log, TEXT("After Logout() function. "));
        AGameModeBase::Logout(Exiting);
    }

};
