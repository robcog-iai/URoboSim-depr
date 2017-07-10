// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "WebSocket.h"

class FWebSocketRunnable : public FRunnable
{
public:
    FWebSocketRunnable();
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Exit() override;
    virtual void Stop() override;
    void CallbackOnConnected();

private:
    TSharedPtr<FWebSocket> Client;
    FThreadSafeCounter StopCounter;
    FThreadSafeBool bIsConnected;
    FThreadSafeBool bIsSubscribed;
};

