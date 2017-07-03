#pragma once

#include "ROSBridgeSubscriber.h"
#include "std_msgs/String.h"
#include "CoreMinimal.h"

class UROSStringSubScriber : public UROSBridgeSubscriber {

public:
    UROSStringSubScriber(FString Topic_) :
        UROSBridgeSubscriber(TEXT("std_msgs/String"), Topic_)
    {

    }

    TSharedPtr<FROSBridgeMsg> ParseMessage(const FJsonObject *JsonObject) override
    {
        TSharedPtr<FROSBridgeMsgStdmsgsString> StringMessage =
            MakeShareable(new FROSBridgeMsgStdmsgsString());
        StringMessage->FromJson(JsonObject);

        return StaticCastSharedPtr<FROSBridgeMsg>(StringMessage);
    }

    void CallBack(const FROSBridgeMsg* msg) override {
        TSharedPtr<FROSBridgeMsgStdmsgsString> StringMessage =
                StaticCastSharedPtr<FROSBridgeMsgStdmsgsString> ( MakeShareable (msg) );

        // do something
        UE_LOG(LogTemp, Log, TEXT("Message received! Content: %s"), *StringMessage->GetData());

        return;
    }

};
