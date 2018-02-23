#pragma once


#include "IURoboSim.h"
#include "RStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "RController.generated.h"


//TODO bug if durned by 180degree


class ARRobot;
//implements the controller functions for the robot.
UCLASS()
    class UROBOSIM_API URController : public UObject
{
    GENERATED_BODY()
public:
    URController(){};
    ~URController(){};

    virtual void InitController();

    ARRobot* Owner;

};

// Handle the key input from the unreal editor and passes it to the Joint controller
UCLASS()
    class UROBOSIM_API URInputController : public URController
{
    GENERATED_BODY()
public:
    URInputController(){};

    virtual void InitController();
    virtual void SetupInputBindings(UInputComponent* IC){};
    UFUNCTION()
    virtual void MoveForward(float AxisValue){};
    UFUNCTION()
    virtual void TurnWheels(float AxisValue){};
};

// Controlls implements the joint motor behaivior
UCLASS()
    class UROBOSIM_API URJointController : public URController
{
    GENERATED_BODY()
public:
    URJointController(){};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        URStaticMeshComponent* Caster;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        URStaticMeshComponent* Target;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
        FQuat TargetOrientation;

    virtual void ControllComand(float DeltaTime){};
    virtual void SetTargetOrientation();
};

UCLASS()
    class UROBOSIM_API URPR2InputController : public URInputController
{
    GENERATED_BODY()
public:
    URPR2InputController(){};

    virtual void SetupInputBindings(UInputComponent* IC);

    UFUNCTION()
        virtual void MoveForward(float AxisValue);
    UFUNCTION()
        virtual void TurnWheels(float AxisValue);

};

UCLASS()
    class UROBOSIM_API URWheelController : public URJointController
{
    GENERATED_BODY()
public:
    URWheelController(){};

    virtual void ControllComand(float DeltaTime);
    virtual void InitController();
};

UCLASS()
    class UROBOSIM_API URRevoluteController : public URJointController
{
    GENERATED_BODY()
public:

    URRevoluteController(){};
    virtual void ControllComand(float DeltaTime);

};

UCLASS()
    class UROBOSIM_API URCasterController : public URJointController
{
    GENERATED_BODY()
public:

    URCasterController(){};
    virtual void ControllComand(float DeltaTime);
    virtual void SetTargetOrientation();
};

UCLASS()
    class UROBOSIM_API URCasterOrientationController : public URJointController
{
    GENERATED_BODY()
public:

    URCasterOrientationController(){};
    virtual void ControllComand(float DeltaTime);
};


UCLASS()
    class UROBOSIM_API URControllerFactory : public UObject
{
    GENERATED_BODY()
public:

    URControllerFactory(){};
    URJointController* CreateController(FString Type, URStaticMeshComponent* Target);
    URInputController* CreateInputController(FString Type, ARRobot* Owner);
};
