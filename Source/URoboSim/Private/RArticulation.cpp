
#include "RArticulation.h"
#include "PhysXIncludes.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ARArticulation::ARArticulation()
{
  PrimaryActorTick.bCanEverTick = true;
  //SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
  //RootComponent = SphereComponent;
  //SphereComponent->InitSphereRadius(40.0f);
  //SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

  BoxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rootcomponent"));
  RootComponent = BoxVisual;
  BoxVisual->SetCollisionProfileName(TEXT("Pawn"));
  BoxVisual->SetMassOverrideInKg(FName("RootComponent"), 10000.0f, true);

  SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weight"));
  SphereVisual->SetCollisionProfileName(TEXT("Pawn"));
  //SphereVisual->InitSphereRadius(40.0f);
  SphereVisual->AddRelativeLocation(FVector(200.0f,0.0f,50.0f));
  SphereVisual->SetAbsolute( /*location*/ false, /*rotation*/ false, /*scale*/ true);

  static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
  if (SphereVisualAsset.Succeeded())
    {

      BoxVisual->SetStaticMesh(CubeVisualAsset.Object);
      //SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
      BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 4.0f));
      BoxVisual->SetSimulatePhysics(true);

      SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
      //SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
      SphereVisual->SetWorldScale3D(FVector(1.0f));
      SphereVisual->SetSimulatePhysics(true);

      //SphereVisual->SetMaterial(0, BasicMaterial);
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("no visiual"));

    }

  SphereVisual->SetupAttachment(RootComponent);

  UPhysicsConstraintComponent* Constraint;
  Constraint = NewObject<UPhysicsConstraintComponent>(RootComponent, FName(TEXT("Constraint")));
  Constraint->SetConstrainedComponents(BoxVisual, NAME_None, SphereVisual, NAME_None);
  Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
  Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
  Constraint->AttachToComponent(BoxVisual, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
  Constraint->SetWorldLocation(BoxVisual->GetComponentLocation());
  Constraint->AddRelativeLocation(FVector(0.0f,0.0f,50.0f));

  StartOrientation = SphereVisual->GetComponentQuat();
}

void ARArticulation::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  // ScreenMsg("Got into PhysX!!!");
  // PxRigidDynamic* PActor = SphereComponent->GetBodyInstance(FName("FirstComponent"),true)->GetPxRigidDynamic_AssumesLocked();
  // PxTransform Trans = PActor->getGlobalPose();
  // PxVec3& PxLoc = Trans.p;
  // FVector Location(PxLoc.x,PxLoc.y,PxLoc.z);
  FQuat Orientation = SphereVisual->GetComponentQuat();
  FQuat DeltaQ = Orientation.Inverse() *StartOrientation;
  FVector Axis;
  float Angle;
  DeltaQ.ToAxisAndAngle(Axis, Angle);
  Angle = FMath::RadiansToDegrees(Angle);
  FVector w = Orientation.RotateVector((Axis * Angle) / DeltaTime);
  //SphereVisual->SetPhysicsAngularVelocityInDegrees(w,false);
  SphereVisual->AddTorqueInDegrees(w);

  ScreenMsg("physx loc", Orientation.ToString());

}


void ARArticulation::MakeArticulation()
{
  // articulation = physics.createArticulation();
  // link = articulation->createLink(parent, linkPose);

}
