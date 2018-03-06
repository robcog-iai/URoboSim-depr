#include "RMeshHandler.h"
#include "RURDFParser.h"
#include "math.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "UnrealEd.h"
#include <stdexcept>
#include "RRobot.h"
#include "Engine/EngineTypes.h"

URMeshHandler::URMeshHandler()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshCy(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshCu(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSp(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

    if (MeshCy.Succeeded())
    {
        CylinderMesh = MeshCy.Object;
    }
    if (MeshCu.Succeeded())
    {
        CubeMesh = MeshCu.Object;
    }
    if (MeshSp.Succeeded())
    {
        SphereMesh = MeshSp.Object;
    }
}

//TEMPLATE Load Obj From Path
    template <typename ObjClass>
FORCEINLINE ObjClass* URMeshHandler::LoadObjFromPath(const FName& Path)
{
    if (Path == NAME_None) return NULL;

    return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
}

// Load Static Mesh From Path
FORCEINLINE UStaticMesh* URMeshHandler::LoadMeshFromPath(const FName& Path)
{
    if (Path == NAME_None) return NULL;

    return LoadObjFromPath<UStaticMesh>(Path);
}

bool URMeshHandler::CreateLink()
{
    Link = &(Node->Link);
    Joint = &(Node->Joint);

    ParentComp = nullptr;
    Scale = Link->Visual.Scale;
    Joint->LowerLimit = FMath::RadiansToDegrees(Joint->LowerLimit);
    Joint->UpperLimit = FMath::RadiansToDegrees(Joint->UpperLimit);
    bUseVisual = !(Link->Visual.Mesh.IsEmpty());
    bUseCollision = !(Link->Collision.Mesh.IsEmpty());
    //
    //
    // why bUseCollision hard coded????   Maybe because collision in pr2
    //


    bUseCollision = bEnableCollisionTags;

    if (Node->Parent)
    {
        // Parent exists and is a UMeshComponent
        ParentComp = Owner->LinkComponents.FindRef(Node->Parent->Link.Name);
        bIsNotRoot = true;
    }
    else
    {
        // This node is the topmost so the parent has to be the RootComponent
        ParentComp = Owner->Root;
        bIsNotRoot = false;
        bWriteParentTFTag = false;
    }

    if (Owner->LinkComponents.Contains(Link->Name))
    {
        UE_LOG(LogTemp, Error, TEXT("Link already in LinkComponents contained"));
        return false;
    }


    CreateMesh();
    CreateMeshComponent();
    MeshComp->SetupAttachment(ParentComp);
    ConfigureMeshComponent();
    ConfigureLinkPhysics();

    return true;
}

void URMeshHandler::CreateMeshComponent()
{
    MeshComp = NewObject<URStaticMeshComponent>(ParentComp, FName((Link->Name).GetCharArray().GetData()));
    // MeshComp = NewObject<URStaticMeshComponent>(Owner->Root, FName((Link->Name).GetCharArray().GetData()));
    MeshComp->Owner = Owner;
    if (bWriteParentTFTag)
    {
        FString TFTag = FString::Printf(TEXT("TF;ParentFrameId,%s;ChildFrameId,%s;"),
                *ParentComp->GetName(), *Link->Name);
        MeshComp->ComponentTags.Add(FName(*TFTag));

    }
    else
    {
        FString TFTag = FString::Printf(TEXT("TF;ChildFrameId,%s;"),
                *Link->Name);
        MeshComp->ComponentTags.Add(FName(*TFTag));

    }
}

void URMeshHandler::ConfigureMeshComponent()
{

    MeshComp->SetStaticMesh(Mesh);

    MeshComp->SetSimulatePhysics(true);
    MeshComp->GetBodyInstance(FName(Link->Name.GetCharArray().GetData()),false)->PositionSolverIterationCount = 255;
    MeshComp->GetBodyInstance(FName(Link->Name.GetCharArray().GetData()),false)->VelocitySolverIterationCount = 32;
    FVector minBounds;
    FVector maxBounds;
    MeshComp->GetLocalBounds(minBounds, maxBounds);

    // Recalculate the numbers with the actual scale. Location value 1 equals maxBounds value in UE4
    LocationCollision = Link->Collision.Location; //All location information should be within the joints tag so LocationCollision and LocationVisual shouldn't be doing anything, should be removed later.
    LocationCollision.Z *= maxBounds.Z * 2;
    LocationCollision.X *= maxBounds.X * 2;
    LocationCollision.Y *= maxBounds.Y * 2;

    LocationVisual = Link->Visual.Location;
    LocationVisual.Z *= maxBounds.Z * 2;
    LocationVisual.X *= maxBounds.X * 2;
    LocationVisual.Y *= maxBounds.Y * 2;
}

void URMeshHandler::ConfigureLinkPhysics()
{

    if (Link->Inertial.Mass > 0)
    {
        MeshComp->SetMassOverrideInKg(NAME_None, Link->Inertial.Mass, true);
    }


    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
    // if (!bEnableShapeCollisions)
    // {
    //     MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    // }

    MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
    MeshComp->SetRelativeScale3D(FVector(1, 1, 1));
    MeshComp->SetWorldLocation(ParentComp->GetComponentLocation());
    MeshComp->SetWorldRotation(ParentComp->GetComponentRotation());
    MeshComp->AddLocalOffset(LocationVisual);
    MeshComp->AddLocalRotation(Link->Visual.Rotation);

    // MeshComp->SetWorldScale3D(Scale);
    MeshComp->bHiddenInGame = true;
    MeshComp->ToggleVisibility();
    MeshComp->RegisterComponent();
}

void URMeshHandler::PositionLinkAndJoint()
{
    FVector LocationJoint = Joint->Location;
    //LocationJoint contains the location values as seen in the URDF, must be scaled by 100 when importing to UE4
    LocationJoint.Z = (LocationJoint.Z * 100);
    LocationJoint.X = (LocationJoint.X * 100);
    LocationJoint.Y = (LocationJoint.Y * 100);

    MeshComp->AddRelativeLocation(LocationJoint);
    MeshComp->AddRelativeRotation(Joint->Rotation);
    LinkOriginLocation = MeshComp->GetRelativeTransform().GetLocation();


    // Prismatic joints need special positioning due to the style of constraint limits
    if (Joint->Type.Equals("prismatic", ESearchCase::IgnoreCase))
    {
        FVector MovementVector(Joint->Axis * (Joint->LowerLimit + Joint->UpperLimit));
        Owner->OriginLocations.Add(Joint->Child, MovementVector);
        MeshComp->AddRelativeLocation(MovementVector);
    }
}

void URMeshHandler::AddConnectedJoint()
{
    MeshComp->ConnectedJoints.Add(Joint->Name, CreateConnectedJoint(false));
    ParentComp->ConnectedJoints.Add(Joint->Name, CreateConnectedJoint(true));
    MeshComp->Parent = ParentComp;

}

FRConnectedJoint URMeshHandler::CreateConnectedJoint(bool IsParent)
{
    static FRConnectedJoint TempJoint;
    TempJoint.Name = Joint->Name;
    TempJoint.Type = Joint->Type;
    TempJoint.Location = Joint->Location;
    TempJoint.Rotation = Joint->Rotation;
    TempJoint.bIsParent = IsParent;

    return TempJoint;
}

void URMeshHandlerFoundation::CreateMesh()
{
    Mesh = CubeMesh;
}

void URMeshHandlerBox::CreateMesh()
{
    Mesh = CubeMesh;
}

void URMeshHandlerSphere::CreateMesh()
{
    Mesh = SphereMesh;
}

void URMeshHandlerCylinder::CreateMesh()
{
    Mesh = CylinderMesh;
}

void URMeshHandlerCustom::CreateMesh()
{
    if (bUseVisual)
    {
        Mesh = LoadMeshFromPath(FName(*Link->Visual.Mesh));
    }
    else
    {
        Mesh = LoadMeshFromPath(FName(*Link->Collision.Mesh));
    }
}

void URMeshHandlerCustom::CreateMeshComponent()
{

    MeshComp = NewObject<URStaticMeshComponent>(Owner->Root, FName(Link->Name.GetCharArray().GetData()));
    MeshComp->Owner = Owner;
    if (bWriteParentTFTag)
    {
        FString TFTag = FString::Printf(TEXT("TF;ParentFrameId,%s;ChildFrameId,%s;"),
                *ParentComp->GetName(), *Link->Name);
        MeshComp->ComponentTags.Add(FName(*TFTag));

    }
    else
    {
        FString TFTag = FString::Printf(TEXT("TF;ChildFrameId,%s;"),
                *Link->Name);
        MeshComp->ComponentTags.Add(FName(*TFTag));
    }

}

void URMeshHandlerCustom::ConfigureLinkPhysics()
{
    if (Link->Inertial.Mass > 0)
    {
        MeshComp->SetMassOverrideInKg(NAME_None, Link->Inertial.Mass, true);
    }
    MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
    for (auto& Tag : GravityEnableTags)
    {
        if (Tag.Equals("all"))
        {
            MeshComp->SetEnableGravity(true);
        }
        else if (Link->Name.Contains(Tag))
        {
            //UE_LOG(LogTemp, Display, TEXT("Disable Gravity"));
            MeshComp->SetEnableGravity(true);
        }
        else
        {
            MeshComp->SetEnableGravity(false);
        }
    }
    MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

    MeshComp->SetWorldLocation(ParentComp->GetComponentLocation());
    MeshComp->SetWorldRotation(ParentComp->GetComponentRotation());
    MeshComp->AddRelativeLocation(LocationVisual);
    MeshComp->SetRelativeRotation(Link->Visual.Rotation);
}

URMeshHandler* URMeshFactory::CreateMeshHandler(ARRobot* Owner, FRNode* Node)
{
    URMeshHandler* MeshHandler = nullptr;

    FRLink* Link = &(Node->Link);
    bool bUseVisual = !(Link->Visual.Mesh.IsEmpty());
    bool bUseCollision = !(Link->Collision.Mesh.IsEmpty());
    // bool bUseCollision = false;
    // Collision and Visual are the same
    if (!bUseCollision && !bUseVisual)
    {
        if ( Link->Name.Equals("world"))
        {
            MeshHandler = NewObject<URMeshHandlerFoundation>(Owner);
            MeshHandler->Node = Node;
            MeshHandler->Owner = Owner;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Mesh created for %s"), *Link->Name);
        }
    }
    else
    {
        if (Link->Visual.Mesh.Equals("box", ESearchCase::IgnoreCase))
        {
            MeshHandler = NewObject<URMeshHandlerBox>(Owner);
        }
        else if (Link->Visual.Mesh.Equals("cylinder", ESearchCase::IgnoreCase))
        {
            MeshHandler = NewObject<URMeshHandlerCylinder>(Owner);
        }
        else if (Link->Visual.Mesh.Equals("sphere", ESearchCase::IgnoreCase))
        {
            MeshHandler = NewObject<URMeshHandlerSphere>(Owner);
        }
        else
        {
            MeshHandler = NewObject<URMeshHandlerCustom>(Owner);
        }

        MeshHandler->Node = Node;
        MeshHandler->Owner = Owner;
    }
    return MeshHandler;
}
