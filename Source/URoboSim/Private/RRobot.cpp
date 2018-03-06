// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RRobot.h"
#include "RStaticMeshComponent.h"
#include "RURDFParser.h"
#include "math.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "RControllerComponent.h"
#include "UnrealEd.h"
#include <stdexcept>
#include "Engine/EngineTypes.h"



// Sets default values
ARRobot::ARRobot()
{

    PrimaryActorTick.bCanEverTick = true;
    CollisionFilterArr = { "torso","wheel_link", "shoulder", "arm", "finger_link", "caster", "base" };

    bEnableShapeCollisions = false;

    // Create a URStaticMeshComponent to be the RootComponent
    Root = CreateDefaultSubobject<URStaticMeshComponent>(TEXT("Root"));
    Root->SetupAttachment(this->RootComponent, TEXT("Root"));
    this->SetRootComponent(Root);
    Root->bVisualizeComponent = true;


    MeshFactory = CreateDefaultSubobject<URMeshFactory>(FName(TEXT("MeshFactory")));
}

// Called when the game starts or when spawned
void ARRobot::BeginPlay()
{
    Super::BeginPlay();
    URControllerComponent* ControllerComp = nullptr;
    ControllerComp=FindComponentByClass<URControllerComponent>();
    if(ControllerComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Begin Controller component"));
        ControllerComp->CreateController();
    }

}

// Called every frame (currently not active)
void ARRobot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


bool ARRobot::AddLink(FRLink Link)
{
    FRLink LinkToAdd(Link);
    Links.Add(LinkToAdd);
    return true;
}

bool ARRobot::AddJoint(FRJoint Joint)
{
    FRJoint JointToAdd(Joint);
    Joints.Add(JointToAdd);
    return true;
}

bool ARRobot::CreateRobot()
{
    if (Links.Num() <= 1)
    {
        return false;
    }
    // Since every Link other than the base link has to be connected to another link the number of joints is fixed
    UE_LOG(LogTemp, Display, TEXT("Number Joints %d"), Joints.Num());
    UE_LOG(LogTemp, Display, TEXT("Number L %d"), Links.Num());
    if (Joints.Num() != Links.Num() - 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mismatch number of links and joints"));
        //return false;
    }

    FRJoint CurrentJoint(Joints[0]);

    // Search for upmost link
    while (true)
    {
        bool FoundHigherElement = false;
        for (int c = 0; c < Joints.Num(); c++)
        {
            if (CurrentJoint.Parent.Equals(Joints[c].Child))
            {
                CurrentJoint = Joints[c];
                FoundHigherElement = true;
            }
        }
        if (!FoundHigherElement) break;
    }

    // If there already is a tree free its memory and continue
    if (BaseNode)
    {
        delete BaseNode;
    }

    // Setup the BaseNode
    BaseNode = new FRNode();
    BaseNode->Parent = nullptr;

    for (int c = 0; c < Links.Num(); c++)
    {
        if (Links[c] == CurrentJoint.Parent)
        {
            BaseNode->Link = Links[c];
            Links.RemoveAt(c);
        }
    }

    BuildTree(BaseNode);

    bool bSuccess = CreateActorsFromNode(BaseNode);



    return bSuccess;
}


bool ARRobot::CreateActorsFromNode(FRNode* Node)
{
    if (!Node)
    {
        return false;
    }

    URMeshHandler* MeshHandler = MeshFactory->CreateMeshHandler(this, Node);
    if(MeshHandler)
    {
        if (MeshHandler->CreateLink())
        {
            if (MeshHandler->bIsNotRoot)
            {
                URConstraint* Constraint = CreateConstraint(MeshHandler);
                //	  URConstraint* Constraint = ConstraintFactory.MakeConstraint(ParentComp, Joint, Link);
                Constraint->InitDrive();
                Constraint->SetupConstraint();
                MeshHandler->AddConnectedJoint();

            //     if(MeshHandler->Link->Name.Contains("wheel_link"))
            //     {
            //         WheelComponents.Add(MeshHandler->MeshComp);
            //     }
            //     if(MeshHandler->Link->Name.Contains("caster_rotation_link"))
            //     {
            //         WheelTurnComponents.Add(MeshHandler->MeshComp);
            //     }
            }

            LinkComponents.Add(MeshHandler->Link->Name, MeshHandler->MeshComp);
        }
    }
    // Create children
    for (int c = 0; c < Node->Children.Num(); c++)
    {
        CreateActorsFromNode(Node->Children[c]);
    }


    return true;
}

void ARRobot::BuildTree(FRNode* Node)
{
    if (!Node)
    {
        return;
    }
    // Go through all joints to find the ones that use the current Node as parent
    for (int c = 0; c < Joints.Num(); c++)
    {
        bool bHasFoundMatch = false;
        if (Joints[c].Parent.Equals(Node->Link.Name))
        {
            // Find the child link which parent is the current node
            for (int i = 0; i < Links.Num(); i++)
            {
                if (Links[i].Name.Equals(Joints[c].Child))
                {
                    FRNode* ChildNode = new FRNode();
                    ChildNode->Parent = Node;
                    ChildNode->Joint = Joints[c];
                    ChildNode->Link = Links[i];
                    Node->Children.Add(ChildNode);

                    Joints.RemoveAt(c);
                    Links.RemoveAt(i);
                    bHasFoundMatch = true;
                    break;
                }
            }
        }
        // Decrement the joint counter as the array automatically shrinks on removal
        if (bHasFoundMatch) c--;
    }

    for (int c = 0; c < Node->Children.Num(); c++)
    {
        BuildTree(Node->Children[c]);
    }
}

URConstraint* ARRobot::CreateConstraint(URMeshHandler* MeshHandler)
{
    URConstraint* Constraint;
    if (MeshHandler->Joint->Type.Equals("fixed", ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URFixedConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("floating", ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URFloatingConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("prismatic", ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URPrismaticConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("revolute", ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URRevoluteConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("planar",ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URPlanarConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("continuous",ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URContinuousConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else
    {
        UE_LOG(LogTemp, Fatal, TEXT("Not a supported Constraint Type"));
        Constraint = nullptr;
    }
    if(Constraint)
    {
        Constraint->Init(MeshHandler);
    }
    return Constraint;
}

