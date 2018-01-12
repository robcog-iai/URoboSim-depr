
#include "RArticulation.h"
#include "PhysXIncludes.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ARArticulation::ARArticulation()
{
  //PrimaryActorTick.bCanEverTick = true;
  //SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
  //RootComponent = SphereComponent;
  //SphereComponent->InitSphereRadius(40.0f);
  //SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
  UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rootcomponent"));
  RootComponent = SphereVisual;
  SphereVisual->SetCollisionProfileName(TEXT("Pawn"));
  //SphereVisual->InitSphereRadius(40.0f);
  //SphereVisual->SetupAttachment(RootComponent);
  static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
  if (SphereVisualAsset.Succeeded())
    {
      SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
      //SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
      //SphereVisual->SetWorldScale3D(FVector(0.8f));
      SphereVisual->SetSimulatePhysics(true);
      //SphereVisual->SetMaterial(0, BasicMaterial);
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("no visiual"));

    }

}

void ARArticulation::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  ScreenMsg("Got into PhysX!!!");
  PxRigidDynamic* PActor = SphereComponent->GetBodyInstance(FName("FirstComponent"),true)->GetPxRigidDynamic_AssumesLocked();
  PxTransform Trans = PActor->getGlobalPose();
  PxVec3& PxLoc = Trans.p;
  FVector Location(PxLoc.x,PxLoc.y,PxLoc.z);

  
  ScreenMsg("physx loc", Location.ToString());

}


void ARArticulation::MakeArticulation()
{
  // articulation = physics.createArticulation();
  // link = articulation->createLink(parent, linkPose);

}
