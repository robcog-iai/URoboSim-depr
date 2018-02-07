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
  MeshComp = nullptr;
  ShapeComp = nullptr;
  Mesh = nullptr;
  static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshCy(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
  //static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshCu(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshCu(TEXT("/Engine/BasicShapes/Cube.Cube"));
  //static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSp(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere")); // Pivot point is not in the center
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

    //OriginLocations = owner->OriginLocations;
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
		ParentComp = owner->LinkComponents.FindRef(Node->Parent->Link.Name);
        IsNotRoot = true;
      }
	else
      {
		// This node is the topmost so the parent has to be the RootComponent
		ParentComp = owner->Root;
        IsNotRoot = false;
      }

    if (owner->LinkComponents.Contains(Link->Name))
      {
		UE_LOG(LogTemp, Error, TEXT("Link already in LinkComponents contained"));
        return false;
      }


    CreateMesh();
    CreateMeshComponent();
    ConfigureMeshComponent();
    ConfigureLinkPhysics();

    //CreateConstraint();
    return true;
}

void URMeshHandler::CreateMeshComponent()
{
  MeshComp = NewObject<URStaticMeshComponent>(ShapeComp, FName((Link->Name + "_Visual").GetCharArray().GetData()));
}

void URMeshHandler::ConfigureMeshComponent()
{
  MeshComp->SetStaticMesh(Mesh);
  //MeshComp->SetMaterial(0, BasicMaterial);
  MeshComp->SetSimulatePhysics(true);
  MeshComp->GetBodyInstance(FName(Link->Name.GetCharArray().GetData()),false)->PositionSolverIterationCount = 255;
  MeshComp->GetBodyInstance(FName(Link->Name.GetCharArray().GetData()),false)->VelocitySolverIterationCount = 32;
  MeshComp->RegisterComponent();

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
      ShapeComp->SetMassOverrideInKg(FName("ShapeComp"), Link->Inertial.Mass, true);
    }

  ShapeComp->SetSimulatePhysics(true);
  ShapeComp->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
  ShapeComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  ShapeComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
  ShapeComp->RegisterComponent();

  //Disabling collisions for all shape components turned out to be necessary for the pr2.
  if (!bEnableShapeCollisions)
    {
      ShapeComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    }

  MeshComp->WeldTo(ParentComp);

  ShapeComp->SetRelativeScale3D(FVector(1, 1, 1));

  ShapeComp->SetWorldLocation(ParentComp->GetComponentLocation());
  ShapeComp->SetWorldRotation(ParentComp->GetComponentRotation());
  ShapeComp->AddLocalOffset(LocationCollision);
  ShapeComp->AddLocalRotation(Link->Collision.Rotation);

  // Weld visual part to the collision body
  MeshComp->SetSimulatePhysics(false);
  MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
  MeshComp->WeldTo(ShapeComp);

  MeshComp->SetWorldLocation(ParentComp->GetComponentLocation());
  MeshComp->SetWorldRotation(ParentComp->GetComponentRotation());
  MeshComp->AddLocalOffset(LocationVisual);
  MeshComp->AddLocalRotation(Link->Visual.Rotation);
  MeshComp->SetWorldScale3D(Scale);
}


// void URMeshHandler::ConfigureConstraint()
// {
//   Constraint->InitDrive();
//   Constraint->SetDisableCollision(true);
//   Constraint->AttachToComponent(ParentComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

//   Constraint->RegisterComponent();
//   Constraint->ApplyWorldOffset(FVector(0), false);
// }

void URMeshHandler::ConnectPositionLink()
{
  FVector LocationJoint = Joint->Location;
  //LocationJoint contains the location values as seen in the URDF, must be scaled by 100 when importing to UE4
  LocationJoint.Z = (LocationJoint.Z * 100);
  LocationJoint.X = (LocationJoint.X * 100);
  LocationJoint.Y = (LocationJoint.Y * 100);
  if (ShapeComp)
    {
      ShapeComp->AddRelativeLocation(LocationJoint);
      ShapeComp->AddRelativeRotation(Joint->Rotation);
      LinkOriginLocation = ShapeComp->GetRelativeTransform().GetLocation();
    }
  else
    {
      MeshComp->AddRelativeLocation(LocationJoint);
      MeshComp->AddRelativeRotation(Joint->Rotation);
      LinkOriginLocation = MeshComp->GetRelativeTransform().GetLocation();
    }

  // Prismatic joints need special positioning due to the style of constraint limits
  if (Joint->Type.Equals("prismatic", ESearchCase::IgnoreCase))
    {
      FVector MovementVector(Joint->Axis * (Joint->LowerLimit + Joint->UpperLimit));
      owner->OriginLocations.Add(Joint->Child, MovementVector);
      if (ShapeComp)
        {
          ShapeComp->AddRelativeLocation(MovementVector);
        }
      else
        {
          MeshComp->AddRelativeLocation(MovementVector);
        }
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
  TempJoint.IsParent = IsParent;

  return TempJoint;
}

void URMeshHandlerBox::CreateMesh()
{
    Mesh = CubeMesh;
    ShapeComp = NewObject<UBoxComponent>(owner->Root, FName(Link->Name.GetCharArray().GetData()));
    FVector BoxSize(Scale);
    BoxSize *= 50.f;
    ((UBoxComponent*)ShapeComp)->InitBoxExtent(BoxSize);
    ((UBoxComponent*)ShapeComp)->SetBoxExtent(BoxSize);

}

void URMeshHandlerSphere::CreateMesh()
{
    Mesh = SphereMesh;
    ShapeComp = NewObject<USphereComponent>(owner->Root, FName(Link->Name.GetCharArray().GetData()));
    float Radius = Scale.X * 50.f;
    ((USphereComponent*)ShapeComp)->InitSphereRadius(Radius);
    ((USphereComponent*)ShapeComp)->SetSphereRadius(Radius);
}

void URMeshHandlerCylinder::CreateMesh()
{
    Mesh = CylinderMesh;
    ShapeComp = NewObject<UCapsuleComponent>(owner->Root, FName(Link->Name.GetCharArray().GetData()));
    float Radius = Scale.X * 50.f;
    float Height = Scale.Z * 50.f;
    ((UCapsuleComponent*)ShapeComp)->InitCapsuleSize(Radius, Height);
    ((UCapsuleComponent*)ShapeComp)->SetCapsuleSize(Radius, Height);
}

void URMeshHandlerCustom::CreateMesh()
{
    if (bUseVisual)
        Mesh = LoadMeshFromPath(FName(*Link->Visual.Mesh));
    else
        Mesh = LoadMeshFromPath(FName(*Link->Collision.Mesh));
}



void URMeshHandlerCustom::CreateMeshComponent()
{
  MeshComp = NewObject<URStaticMeshComponent>(owner->Root, FName(Link->Name.GetCharArray().GetData()));
}

void URMeshHandlerCustom::ConfigureLinkPhysics()
{
  if (Link->Inertial.Mass > 0)
    {
      MeshComp->SetMassOverrideInKg(NAME_None, Link->Inertial.Mass, true);
    }
  MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);


  for (auto& Tag : GravityDisabledTags)
    {
      if (Link->Name.Contains(Tag))
        {
          //UE_LOG(LogTemp, Display, TEXT("Disable Gravity"));
          MeshComp->SetEnableGravity(true);
        }
      else
        {
          MeshComp->SetEnableGravity(false);
        }
    }
  //MeshComp->SetEnableGravity(false);


  MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

  // for (FString linkName : collisionFilterArr)
  //   {
  //     if (Link->Name.Contains(linkName))
  //       {
  //         MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
  //         MeshComp->WeldTo(ParentComp);
  //         break;
  //       }
  //   }

  //if (Link->Name.Contains("wheel_link") || Link->Name.Contains("shoulder") || Link->Name.Contains("arm") || Link->Name.Contains("finger_link")) {
  //	//Prevents certain links from colliding with themselves.
  //	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
  //	MeshComp->WeldTo(ParentComp);
  //}

  MeshComp->SetWorldLocation(ParentComp->GetComponentLocation());
  //UE_LOG(LogTemp, Warning, TEXT("Add MeshComp [%s], Parent [%s], Parent WorldRotation = [%s]"), *MeshComp->GetName(), *ParentComp->GetName(), *ParentComp->GetComponentRotation().ToString());
  MeshComp->SetWorldRotation(ParentComp->GetComponentRotation());
  MeshComp->AddRelativeLocation(LocationVisual);
  MeshComp->SetRelativeRotation(Link->Visual.Rotation);
}








URMeshHandler* URMeshFactory::CreateMeshHandler(ARRobot* Owner, FRNode* Node)
//URMeshHandler* URMeshFactory::CreateMeshHandler(URStaticMeshComponent* RootComponent, FRNode* Node)
{
  URMeshHandler* MeshHandler = nullptr;

  FRLink* Link = &(Node->Link);
  bool bUseVisual = !(Link->Visual.Mesh.IsEmpty());
  bool bUseCollision = false;

  // Collision and Visual are the same
  if (!bUseCollision && !bUseVisual)
    {

    }
  else
    {
      if (Link->Visual.Mesh.Equals("box", ESearchCase::IgnoreCase) || Link->Collision.Mesh.Equals("box", ESearchCase::IgnoreCase))
        {
          MeshHandler = NewObject<URMeshHandlerBox>(Owner->Root);
        }
      else if (Link->Visual.Mesh.Equals("cylinder", ESearchCase::IgnoreCase))// || Link->Collision.Mesh.Equals("cylinder", ESearchCase::IgnoreCase))
        {
          MeshHandler = NewObject<URMeshHandlerCylinder>(Owner->Root);
        }
      else if (Link->Visual.Mesh.Equals("sphere", ESearchCase::IgnoreCase) || Link->Collision.Mesh.Equals("sphere", ESearchCase::IgnoreCase))
        {
          MeshHandler = NewObject<URMeshHandlerSphere>(Owner->Root);
        }
      else
        {
          MeshHandler = NewObject<URMeshHandlerCustom>(Owner->Root);
        }

      MeshHandler->Node = Node;

      MeshHandler->owner = Owner;
    }


  return MeshHandler;
}
