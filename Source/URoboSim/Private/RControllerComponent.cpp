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


// Start of PR2 Implementation

URPR2ControllerComponent::URPR2ControllerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features

    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void URPR2ControllerComponent::BeginPlay()
{
    Super::BeginPlay();
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
        Link->Controller->SetTargetOrientation();
        // Link->Controller->TargetOrientation = Link->GetLocalTransform();


        if(Link->GetName().Equals("br_caster_rotation_link"))
        {
            UE_LOG(LogTemp, Log, TEXT("%s"),*Link->Controller->TargetOrientation.ToString());
        }
    }
    InputController = ControllerFactory->CreateInputController("pr2", Owner);
    InputController->InitController();
}



