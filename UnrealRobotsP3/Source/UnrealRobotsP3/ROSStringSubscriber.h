#pragma once

#include "ROSBridgeSubscriber.h"
#include "std_msgs/String.h"
#include "sensor_msgs/JointState.h"
#include "Core.h"

class FROSStringSubScriber : public FROSBridgeSubscriber {

public:
	FROSStringSubScriber(FString Topic_) :
		FROSBridgeSubscriber(TEXT("std_msgs/String"), Topic_)
	{

	}

	~FROSStringSubScriber() override {};

	FROSBridgeMsg* ParseMessage(TSharedPtr<FJsonObject> JsonObject) override
	{
		FROSBridgeMsgStdmsgsString* StringMessage = new FROSBridgeMsgStdmsgsString();
		StringMessage->FromJson(JsonObject);
		UE_LOG(LogTemp, Log, TEXT("Data in String Message: %s"), *StringMessage->GetData());

		return StringMessage;
	}

	void CallBack(FROSBridgeMsg* msg) override {
		FROSBridgeMsgStdmsgsString* StringMessage = static_cast<FROSBridgeMsgStdmsgsString*>(msg);
		// do something
		UE_LOG(LogTemp, Log, TEXT("Message received! Content: %s"), *StringMessage->GetData());

		return;
	}

};
