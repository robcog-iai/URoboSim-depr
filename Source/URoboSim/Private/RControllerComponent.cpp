// Fill out your copyright notice in the Description page of Project Settings.


#include "RControllerComponent.h"
#include "RRobot.h"

// Sets default values for this component's properties
URControllerComponent::URControllerComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;
  ControllerFactory = CreateDefaultSubobject<URControllerFactory>(FName(TEXT("ControllerFactory")));
  // ...
}


// Called when the game starts
void URControllerComponent::BeginPlay()
{
  Super::BeginPlay();
  Owner = Cast<ARRobot>(GetOwner());

}


// Called every frame
void URControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}


// void URControllerComponent::SetupPlayerInputComponent()
// {
//     //Super::SetupPlayerInputComponent(InputComponent);
//
//
//
// EnableInput(this)
// check(InputComponent);
//
//     InputComponent->BindAxis("MoveForward", this, &URControllerComponent::MoveForward);

//     InputComponent->BindAxis("TurnWheels", this, &URControllerComponent::TurnWheels);
// }
//
void URPR2ControllerComponent::TurnWheels(float AxisValue)
{
    Owner->WheelTurnSpeed = FRotator(0.0f,AxisValue, 0.0f ) * 80;
}


void URPR2ControllerComponent::MoveForward(float AxisValue)
{
    UE_LOG(LogTemp, Log, TEXT("Axis Value %f"),AxisValue);
     Owner->WheelSpinnSpeed = FVector(0.0f, AxisValue, 0.0f) * 1000;
} 

// Start of PR2 Implementation

URPR2ControllerComponent::URPR2ControllerComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features

  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  // ...
}


void URPR2ControllerComponent::SetupRobotInputs()
{

  Owner = Cast<ARRobot>(GetOwner());
  // Owner->InputComponent->BindAxis("MoveForward", Owner, &ARRobot::MoveForward);
  // Owner->InputComponent->BindAxis("TurnWheels", Owner, &ARRobot::TurnWheels);


}



// Called when the game starts
void URPR2ControllerComponent::BeginPlay()
{
  Super::BeginPlay();
  UE_LOG(LogTemp, Error, TEXT("Begin Controller component"));
//  CreateController();
}


// Called every frame
void URPR2ControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void URPR2ControllerComponent::CreateController()
{
    URStaticMeshComponent* Link;
    for(auto& CD : Owner->ControllerDescriptionList)
        {
            Link = Owner->LinkComponents.FindRef(CD.TargetName);
            Link->Controller = ControllerFactory->CreateController(CD.ControllerType, Link);
            Link->Controller->InitController();
            Link->Controller->TargetOrientation = Link->GetLocalTransform();


            if(Link->GetName().Equals("br_caster_rotation_link"))
            {
              UE_LOG(LogTemp, Log, TEXT("%s"),*Link->Controller->TargetOrientation.ToString());
            }
        }
}



// void ARRobot::MoveForward(float AxisValue)
// {
//     WheelSpinnSpeed = FVector(0.0f, AxisValue, 0.0f) * 1000;
// }
//
// void ARRobot::TurnWheels(float AxisValue)
// {
//     WheelTurnSpeed = FRotator(0.0f,AxisValue, 0.0f ) * 80;
// }
//
