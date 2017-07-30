// Fill out your copyright notice in the Description page of Project Settings.

#include "ROSBridgeTestGameModeBase.h"
#include "ROSBridgeTest.h"

void AROSBridgeTestGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    // AROSBridgeActor* Actor = GetWorld()->SpawnActor<AROSBridgeActor>(AROSBridgeActor::StaticClass());
    Handler = new FROSBridgeHandler(TEXT("127.0.0.1"), 9001);
    UE_LOG(LogTemp, Log, TEXT("Handler Created. "));

    Subscriber = new FROSStringSubScriber(TEXT("/chatter"));
    Handler->AddSubscriber(Subscriber);
    UE_LOG(LogTemp, Log, TEXT("Added chatter subscriber. "));

    /* Publisher = new FROSBridgePublisher(TEXT("sensor_msgs/JointState"), TEXT("/talker"));
    Handler->AddPublisher(Publisher); */

    ServiceClient = new FROSAddTwoIntsClient(TEXT("add_two_ints"));

    Handler->Connect();
    UE_LOG(LogTemp, Log, TEXT("Connected to WebSocket server. "));
}

void AROSBridgeTestGameModeBase::Tick(float DeltaSeconds)
{

    Super::Tick(DeltaSeconds);
    UE_LOG(LogTemp, Log, TEXT("GameMode Ticks, DeltaSeconds = %.3f."), DeltaSeconds);

    // FROSBridgeMsgStdmsgsString* StringMsgToSend = new
    //     FROSBridgeMsgStdmsgsString(TEXT("New Message at ") + FDateTime::Now().ToString());
    // Handler->PublishMsg(TEXT("/talker"), StringMsgToSend);
    // delete StringMsgToSend;

    /* FROSBridgeMsgStdmsgsHeader header(GFrameCounter, FROSTime::Now(), TEXT("1"));
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
    delete JointState;  */

    if (GFrameCounter % 10 == 0)
    {
        int NumA = FMath::RandRange(1, 10000);
        int NumB = FMath::RandRange(1, 10000);
        TSharedPtr<FROSBridgeSrv::SrvRequest> Request = MakeShareable(new FROSBridgeSrvRospytutorialsAddTwoInts::Request(NumA, NumB));
        TSharedPtr<FROSBridgeSrv::SrvResponse> Response = MakeShareable(new FROSBridgeSrvRospytutorialsAddTwoInts::Response());
        UE_LOG(LogTemp, Log, TEXT("Call Service in Tick: %d + %d"), NumA, NumB);
        Handler->CallService(ServiceClient, Request, Response);
    }

    Handler->Render();
}

void AROSBridgeTestGameModeBase::Logout(AController *Exiting)
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
