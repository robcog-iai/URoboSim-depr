// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Components/StaticMeshComponent.h"
#include "Structs.h"
#include "RStaticMeshComponent.generated.h"

class URJointController;

USTRUCT()
struct FMySecondaryTickFunction : public FTickFunction
{
    GENERATED_USTRUCT_BODY()

    class URStaticMeshComponent* Target;

    UROBOSIM_API virtual void ExecuteTick(
            float DeltaTime,
            ELevelTick TickType,
            ENamedThreads::Type CurrentThread,
            const FGraphEventRef& MyCompletionGraphEvent) override;

    UROBOSIM_API virtual FString DiagnosticMessage() override;
};

template<>
struct TStructOpsTypeTraits<FMySecondaryTickFunction> : public TStructOpsTypeTraitsBase2<FMySecondaryTickFunction>
{
    enum
    {
        WithCopy = false
    };
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URStaticMeshComponent : public UStaticMeshComponent
{
    GENERATED_BODY()

public:
    URStaticMeshComponent();
    ~URStaticMeshComponent()
    {
    };


    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void TickPostPhysics(float DeltaSeconds, ELevelTick TickType, FMySecondaryTickFunction& ThisTickFunction);

    // float StartH;
    // FVector StartPos;
    // FRotator StartRot;
    // FQuat TargetOrientation;
    // FTransform StartRelativeTransform;

    UPROPERTY(EditDefaultsOnly, Category = "Tick")
        FMySecondaryTickFunction SecondaryComponentTick;

    UPROPERTY(EditAnywhere)
        TMap<FString, FRConnectedJoint> ConnectedJoints;

    void AddConnectedJoint(FRJoint Joint);
    UPROPERTY(EditAnywhere)
        URStaticMeshComponent* Parent;

    bool CheckJointType(FString Type);
    FQuat GetLocalTransform();

    UPROPERTY(EditAnywhere)
        URJointController* Controller;
    UPROPERTY(EditAnywhere)
        FString ControllerName;
    UPROPERTY(EditAnywhere)
        FString ControllerType;
    class ARRobot *Owner;



private:
    FCalculateCustomPhysics OnCalculateCustomPhysics;

    void SubstepTick(float DeltaTime, FBodyInstance* BodyInstance);
    void DoPhysics(float DeltaTime, bool InSubstep);


    int32 FrameCount;

    physx::PxRigidBody* PRigidBody;



    FORCEINLINE FVector  GetCurrentLocation();
    FORCEINLINE FRotator GetCurrentRotation();
    FORCEINLINE FVector  GetCurrentVelocity();
    FORCEINLINE FVector  GetCurrentAngularVelocity();

    FORCEINLINE void ScreenMsg(const FString& Msg)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg);
    }

    FORCEINLINE void ScreenMsg(const FString& Msg, const float Msg2)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %f"), *Msg, Msg2));
    }

    FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2));
    }
};
