// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/GameModeBase.h"
#include "Core.h"
#include "Engine.h"

#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "ROSStringSubscriber.h"

// #include "ROSBridgeActor.h"
#include "ROSStringSubscriber.h"
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

    AROSBridgeTestGameModeBase(const FObjectInitializer &ObjectInitializer) :
        AGameModeBase(ObjectInitializer){
        PrimaryActorTick.bCanEverTick = true;
        PrimaryActorTick.bStartWithTickEnabled = true;
    }

    void BeginPlay() override
    {
        Super::BeginPlay();
        // AROSBridgeActor* Actor = GetWorld()->SpawnActor<AROSBridgeActor>(AROSBridgeActor::StaticClass());
        Handler = new FROSBridgeHandler(TEXT("127.0.0.1"), 9001);
        UE_LOG(LogTemp, Log, TEXT("Handler Created. "));

        Subscriber = new FROSStringSubScriber(TEXT("/chatter"));
        Handler->AddSubscriber(Subscriber);
        UE_LOG(LogTemp, Log, TEXT("Added chatter subscriber. "));

        Publisher = new FROSBridgePublisher(TEXT("sensor_msgs/JointState"), TEXT("/talker"));
        Handler->AddPublisher(Publisher);

        Handler->Connect();
        UE_LOG(LogTemp, Log, TEXT("Connected to WebSocket server. "));
    }

    void Tick(float DeltaSeconds) override {
        Super::Tick(DeltaSeconds);
        UE_LOG(LogTemp, Log, TEXT("GameMode Ticks, DeltaSeconds = %.3f."), DeltaSeconds);

        // FROSBridgeMsgStdmsgsString* StringMsgToSend = new
        //     FROSBridgeMsgStdmsgsString(TEXT("New Message at ") + FDateTime::Now().ToString());
        // Handler->PublishMsg(TEXT("/talker"), StringMsgToSend); 
        // delete StringMsgToSend;

        FROSBridgeMsgStdmsgsHeader header(GFrameCounter, FROSTime::Now(), TEXT("1"));
        TArray<FString> names = { TEXT("head"), TEXT("torso"), TEXT("gripper") }; 
        TArray<double > position, velocity, effort; 
        for (int i = 0; i < 3; i++)
        {
            position.Add(FMath::FRandRange(0.0f, 5.0f)); 
            velocity.Add(FMath::FRandRange(0.0f, 5.0f));
            effort.Add(FMath::FRandRange(0.0f, 5.0f));
        }
        FROSBridgeMsgSensormsgsJointState *JointState
            = new FROSBridgeMsgSensormsgsJointState(header, names, position, velocity, effort); 
        Handler->PublishMsg(TEXT("/talker"), JointState);
        delete JointState; 

        Handler->Render();
    }

    void Logout(AController *Exiting) override
    {
        UE_LOG(LogTemp, Log, TEXT("On Logout() function. "));
        // UE_LOG(LogTemp, Log, TEXT("SharedPtr RefCount = %d. "), Handler.GetSharedReferenceCount());
        Handler->Disconnect();
        // delete Handler;
        delete Subscriber;
        delete Publisher;
        UE_LOG(LogTemp, Log, TEXT("After Logout() function. "));
        AGameModeBase::Logout(Exiting);
    }

};
