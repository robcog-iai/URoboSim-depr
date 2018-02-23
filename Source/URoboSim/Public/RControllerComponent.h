// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RController.h"
#include "RControllerComponent.generated.h"

class ARRobot;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URControllerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URControllerComponent();
    URControllerFactory* ControllerFactory;
    ARRobot* Owner;
    URInputController* InputController;

    virtual void CreateController(){};
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
    // Sets default values for this component's properties
    URPR2ControllerComponent();
    virtual void CreateController();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



};



