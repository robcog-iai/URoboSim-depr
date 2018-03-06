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
}


// Start of PR2 Implementation

URPR2ControllerComponent::URPR2ControllerComponent()
    :URControllerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features

    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URPR2ControllerComponent::BeginPlay()
{
    Super::BeginPlay();
}


// Called every frame
void URPR2ControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPR2ControllerComponent::CreateController()
{
    URStaticMeshComponent* Link;

    CreateControllerDesciptionList();
    FindWheelComponents();
    FindCasterComponents();
    for(auto& CD : ControllerDescriptionList)
    {
        Link = Owner->LinkComponents.FindRef(CD.TargetName);
        if (Link)
        {
            Link->Controller = ControllerFactory->CreateController(CD.ControllerType, Link);
            if(Link->Controller)
            {
                Link->Controller->SetControllerComponent(this);
                Link->Controller->InitController();
                Link->Controller->SetTargetOrientation();
            }
        }
    }
    InputController = ControllerFactory->CreateInputController("pr2", Owner);
    InputController->InitController();
    InputController->ControllerComponent = this;
}

void URPR2ControllerComponent::CreateControllerDesciptionList()
{
    for(auto& Joint : Owner->JointComponents)
    {
        FString ControllerType ="";
        if(Joint.Value->ChildName.Contains("caster_rotation_link"))
        {
            ControllerType = TEXT("caster");
        }
        else if(Joint.Value->ChildName.Contains("wheel_link"))
        {
            ControllerType = TEXT("wheel");
        }
        else if(Joint.Value->ChildName.Contains("base_link"))
        {
            ControllerType = TEXT("orientation");
        }
        else if (Joint.Value->Type.Equals("revolute", ESearchCase::IgnoreCase) ||
                Joint.Value->Type.Equals("continuous", ESearchCase::IgnoreCase))
        {
            ControllerType = Joint.Value->Type;
        }

        if(!ControllerType.Equals(""))
        {
            FRControllerDesciption ControllerDescription;
            ControllerDescription.Set(Joint.Value->ChildName, ControllerType);
            ControllerDescriptionList.Add(ControllerDescription);
        }
    }
}


void URPR2ControllerComponent::FindWheelComponents()
{
    for(auto& Link : Owner->LinkComponents)
    {
        if(Link.Value->GetName().Contains("wheel_link"))
        {
            WheelComponents.Add(Link.Value);
        }
    }
}

void URPR2ControllerComponent::FindCasterComponents()
{
    for(auto& Link : Owner->LinkComponents)
    {
        if(Link.Value->GetName().Contains("caster_rotation_link"))
        {
            WheelTurnComponents.Add(Link.Value);
        }
    }
}

