// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "WebSocket.h"
#include "WebSocketRunnable.h"
#include "WebSocketTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WEBSOCKETTEST_API AWebSocketTestGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AWebSocketTestGameModeBase(const FObjectInitializer &ObjectInitializer);
    void StartPlay() override;
    void Tick(float DeltaSeconds) override;
    void BeginDestroy() override;
    void Destroyed() override;
    void Logout(AController *Exiting) override;

private:
    TSharedPtr<FWebSocket> Client;
    TSharedPtr<FWebSocketRunnable> Runnable;
    FRunnableThread* Thread;
};
