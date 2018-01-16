#include "RMeshHandler.h"
#include "RURDFParser.h"
#include "math.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "UnrealEd.h"
#include <stdexcept>
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


void URMeshHandler::CreateLink(FRNode* Node, USceneComponent* RootComponent, TMap<FString, UPrimitiveComponent*> LinkComponents)
{
	Link = &(Node->Link);
	Joint = &(Node->Joint);
    Root = RootComponent;
	USceneComponent* ParentComp;
	UPrimitiveComponent* ParentLink = nullptr;
	FVector LinkOriginLocation(0);
	//UStaticMeshComponent* MeshComp = nullptr;
	FVector Scale(Link->Visual.Scale);


	Joint->LowerLimit = FMath::RadiansToDegrees(Joint->LowerLimit);
	Joint->UpperLimit = FMath::RadiansToDegrees(Joint->UpperLimit);


	bUseVisual = !(Link->Visual.Mesh.IsEmpty());
	bUseCollision = !(Link->Collision.Mesh.IsEmpty());

	//
	//
	// why bUseCollision hard coded????   Maybe because collision in pr2
	//


	bUseCollision = bEnableCollisionTags;
	//UE_LOG(LogTemp, Warning, TEXT("enable Collision tags = [%s]"), (bEnableCollisionTags ? TEXT("True") : TEXT("False")));
	if (Node->Parent)
	{
		// Parent exists and is a UMeshComponent
		ParentComp = (USceneComponent*)LinkComponents.FindRef(Node->Parent->Link.Name);
		ParentLink = LinkComponents.FindRef(Node->Parent->Link.Name);
	}
	else
	{
		// This node is the topmost so the parent has to be the RootComponent
		ParentComp = Root;
	}

    CreateMeshType();
    ConfigureMeshComp();
}

void URMeshHandler::CreateMeshType()
{
  if (Link->Visual.Mesh.Equals("box", ESearchCase::IgnoreCase) || Link->Collision.Mesh.Equals("box", ESearchCase::IgnoreCase))
    {
      CreateBoxLink();
    }
  else if (Link->Visual.Mesh.Equals("cylinder", ESearchCase::IgnoreCase))// || Link->Collision.Mesh.Equals("cylinder", ESearchCase::IgnoreCase))
    {
      CreateCylinderLink();
    }
  else if (Link->Visual.Mesh.Equals("sphere", ESearchCase::IgnoreCase) || Link->Collision.Mesh.Equals("sphere", ESearchCase::IgnoreCase))
    {
      CreateSphereLink();
    }
  else
    {
      CreateCustomLink();
    }
}

void URMeshHandler::ConfigureMeshComp()
{
  MeshComp->SetStaticMesh(Mesh);
  MeshComp->SetMaterial(0, BasicMaterial);
  MeshComp->SetSimulatePhysics(true);
  MeshComp->GetBodyInstance(FName(Link->Name.GetCharArray().GetData()),false)->PositionSolverIterationCount = 255;
  MeshComp->GetBodyInstance(FName(Link->Name.GetCharArray().GetData()),false)->VelocitySolverIterationCount = 1;
  MeshComp->RegisterComponent();

  FVector minBounds;
  FVector maxBounds;
  MeshComp->GetLocalBounds(minBounds, maxBounds);

  // Recalculate the numbers with the actual scale. Location value 1 equals maxBounds value in UE4
  FVector LocationCollision = Link->Collision.Location; //All location information should be within the joints tag so LocationCollision and LocationVisual shouldn't be doing anything, should be removed later.
  LocationCollision.Z *= maxBounds.Z * 2;
  LocationCollision.X *= maxBounds.X * 2;
  LocationCollision.Y *= maxBounds.Y * 2;

  FVector LocationVisual = Link->Visual.Location;
  LocationVisual.Z *= maxBounds.Z * 2;
  LocationVisual.X *= maxBounds.X * 2;
  LocationVisual.Y *= maxBounds.Y * 2;

}

void URMeshCollisionOrVisual::CreateBoxLink()
{
    FVector Scale(Link->Visual.Scale);
    Mesh = CubeMesh;
    ShapeComp = NewObject<UBoxComponent>(Root, FName(Link->Name.GetCharArray().GetData()));
    FVector BoxSize(Scale);
    BoxSize *= 50.f;
    ((UBoxComponent*)ShapeComp)->InitBoxExtent(BoxSize);
    ((UBoxComponent*)ShapeComp)->SetBoxExtent(BoxSize);
    MeshComp = NewObject<URStaticMeshComponent>(ShapeComp, FName((Link->Name + "_Visual").GetCharArray().GetData()));
}

void URMeshCollisionOrVisual::CreateSphereLink()
{
    FVector Scale(Link->Visual.Scale);
    Mesh = SphereMesh;
    ShapeComp = NewObject<USphereComponent>(Root, FName(Link->Name.GetCharArray().GetData()));
    float Radius = Scale.X * 50.f;
    ((USphereComponent*)ShapeComp)->InitSphereRadius(Radius);
    ((USphereComponent*)ShapeComp)->SetSphereRadius(Radius);
    MeshComp = NewObject<URStaticMeshComponent>(ShapeComp, FName((Link->Name + "_Visual").GetCharArray().GetData()));
}
void URMeshCollisionOrVisual::CreateCylinderLink()
{
    FVector Scale(Link->Visual.Scale);
    Mesh = CylinderMesh;
    ShapeComp = NewObject<UCapsuleComponent>(Root, FName(Link->Name.GetCharArray().GetData()));
    float Radius = Scale.X * 50.f;
    float Height = Scale.Z * 50.f;
    ((UCapsuleComponent*)ShapeComp)->InitCapsuleSize(Radius, Height);
    ((UCapsuleComponent*)ShapeComp)->SetCapsuleSize(Radius, Height);
    MeshComp = NewObject<URStaticMeshComponent>(ShapeComp, FName((Link->Name + "_Visual").GetCharArray().GetData()));
}

void URMeshCollisionOrVisual::CreateCustomLink()
{
    if (bUseVisual)
        Mesh = LoadMeshFromPath(FName(*Link->Visual.Mesh));
    else
        Mesh = LoadMeshFromPath(FName(*Link->Collision.Mesh));

    MeshComp = NewObject<URStaticMeshComponent>(Root, FName(Link->Name.GetCharArray().GetData()));
}

void URMeshCollisionAndVisual::CreateMeshType()
{
  if (Link->Collision.Mesh.Equals("box", ESearchCase::IgnoreCase))
    {
      CreateBoxLink();
    }
  else if (Link->Collision.Mesh.Equals("cylinder", ESearchCase::IgnoreCase))
    {
      CreateCylinderLink();
    }
  else if (Link->Collision.Mesh.Equals("sphere", ESearchCase::IgnoreCase))
    {
      CreateSphereLink();
    }
  else
    {
      CreateCustomLink();
    }
}





void URMeshCollisionAndVisual::CreateCustomLink()
{
  Mesh = LoadMeshFromPath(FName(*Link->Visual.Mesh));
}






URMeshHandler* URMeshFactory::CreateMeshHandler(USceneComponent* RootComponent, bool bUseCollision, bool bUseVisual)
{
  URMeshHandler* MeshHandler = nullptr;
  // Collision and Visual are the same
  if ((bUseCollision && !bUseVisual) || (!bUseCollision && bUseVisual))
	{
      MeshHandler = NewObject<URMeshCollisionOrVisual>(RootComponent);
    }
  else if (bUseCollision && bUseVisual)
	{
      MeshHandler = NewObject<URMeshCollisionAndVisual>(RootComponent);
    }
  else if (!bUseCollision && !bUseVisual)
    {
      MeshHandler = NewObject<URMeshNoCollisionAndVisual>(RootComponent);
    }

  return MeshHandler;
}
