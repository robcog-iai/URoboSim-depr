// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RController.h"
#include "Structs.h"
#include "RControllerComponent.generated.h"

class ARRobot;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URControllerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URControllerFactory* ControllerFactory;
    ARRobot* Owner;
    URInputController* InputController;

    UPROPERTY(VisibleAnywhere, Category = "Map")
        TArray<FRControllerDesciption> ControllerDescriptionList;


    URControllerComponent();
    virtual void CreateController(){};
    virtual void CreateControllerDesciptionList(){};
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

    // Called when the game starts
    virtual void BeginPlay() override;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URPR2ControllerComponent : public URControllerComponent
{
    GENERATED_BODY()

public:

    // List of all Wheel of the Robot
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
    TArray<URStaticMeshComponent*> WheelComponents;

    //List of all caster/links responsible for the orientation of the wheels
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
    TArray<URStaticMeshComponent*> WheelTurnComponents;

    // Angular velocity of the wheels resulting in a forward motion of the robot
    FRotator WheelTurnSpeed;

    // Angular velocity of the wheels to change the direction of the robot
    FVector WheelSpinnSpeed;

    // Parameter for claculating angular velocity of wheels
    float DistanceWheelCaster = 5.0f;

    // Parameter for claculating angular velocity of wheels ;
    float WheelRadius = 8.0f;


    // Sets default values for this component's properties
    URPR2ControllerComponent();
    virtual void CreateController();
    virtual void CreateControllerDesciptionList();
    virtual void FindWheelComponents();
    virtual void FindCasterComponents();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



};



