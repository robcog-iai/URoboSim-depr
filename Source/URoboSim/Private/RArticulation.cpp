
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

  static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
  if (SphereVisualAsset.Succeeded())
    {

      BoxVisual->SetStaticMesh(CubeVisualAsset.Object);
      //SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
      BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 4.0f));
      BoxVisual->SetSimulatePhysics(true);
      BoxVisual->SetEnableGravity(false);

      SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
      //SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
      SphereVisual->SetWorldScale3D(FVector(1.0f));
      SphereVisual->SetSimulatePhysics(true);
      SphereVisual->SetEnableGravity(false);


      SphereVisual->SetMassOverrideInKg(FName("Weight"), 1, true);
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
  Constraint->SetAbsolute( /*location*/ false, /*rotation*/ false, /*scale*/ true);
  Constraint->AddRelativeLocation(FVector(0.0f,0.0f,50.0f));


  StartRelativeTransform = SphereVisual->GetRelativeTransform();
  StartOrientation = SphereVisual->GetComponentTransform().GetRelativeTransform(BoxVisual->GetComponentTransform()).GetRotation();
  StartLocation = SphereVisual->GetComponentTransform().GetRelativeTransform(BoxVisual->GetComponentTransform()).GetLocation();

  //TestQuat = BoxVisual->

  Ki = 1000;
  Kd = 1000;
  Kp = 1000;

}

void ARArticulation::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);


  // FQuat Orientation = SphereVisual->GetComponentTransform().GetRelativeTransform(BoxVisual->GetComponentTransform()).GetRotation();
  // FQuat OrientationWorld = SphereVisual->GetComponentTransform().GetRotation();

  // FVector Location = SphereVisual->GetComponentTransform().GetRelativeTransform(BoxVisual->GetComponentTransform()).GetLocation();
  // FQuat DeltaQ = Orientation.Inverse() *StartOrientation;
  // FVector DeltaL = StartLocation - Location;
  // FVector Axis;
  // float Angle;
  // DeltaQ.ToAxisAndAngle(Axis, Angle);
  // Angle = FMath::RadiansToDegrees(Angle);
  // FVector w = OrientationWorld.RotateVector((Axis * Angle) / DeltaTime);
  // FVector v = DeltaL/ DeltaTime;
  // FVector f = DeltaL * 10000;
  // FVector p = v * SphereVisual->GetMass();

  // FVector pw = w;

  // SphereVisual->SetPhysicsAngularVelocityInDegrees(w,false);
  // //SphereVisual->SetPhysicsLinearVelocity(v,false);
  // //SphereVisual->AddForce(f);
  // //SphereVisual->AddImpulse(p);

  // FVector w_regler = pid(w, DeltaTime);

  // //SphereVisual->AddAngularImpulse(w*1000*Angle);
  // //SphereVisual->AddTorqueInDegrees(w);
  // FTransform t = SphereVisual->GetRelativeTransform();
  // ScreenMsg("Axis", Axis.ToString());

  // ScreenMsg("angle", FString::SanitizeFloat(Angle));
  // //ScreenMsg("DeltaQ", DeltaQ.Euler().ToString());
  // //ScreenMsg("w_regler", w_regler.ToString());

  FRotator Rotation(00.0f, 0.0f, 10.0f);
  Rotation = Rotation * DeltaTime;

  FQuat OrientationLocal = BoxVisual->GetComponentTransform().GetRelativeTransform(BoxVisual->GetComponentTransform()).GetRotation();



  FQuat OrientationWorld = BoxVisual->GetComponentTransform().GetRotation();
  OrientationWorld = OrientationWorld* Rotation.Quaternion();

  // FVector Axis;
  // float Angle;
  // OrientationWorld.ToAxisAndAngle(Axis, Angle);
  //FVector w = OrientationWorld.RotateVector((Axis * Angle) / DeltaTime);
  //OrientationWorld =  OrientationWorld.Euler() + Rotation.Quaternion();



  BoxVisual->SetWorldRotation(OrientationWorld);
  //BoxVisual->AddWorldRotation(Rotation);
  //ScreenMsg("Time", FString::SanitizeFloat(DeltaTime));
  ScreenMsg("Rotation", BoxVisual->GetComponentTransform().GetRotation().ToString());
  ScreenMsg("Rotation", Rotation.Quaternion().ToString());


  // FQuat OrientationLocal = BoxVisual->GetComponentTransform().GetRelativeTransform(BoxVisual->GetComponentTransform()).GetRotation();
  // OrientationLocal = OrientationLocal * Rotation.ToOrientationQuat();

  // BoxVisual->SetWorldRotation(OrientationLocal);















}


void ARArticulation::MakeArticulation()
{
  // articulation = physics.createArticulation();
  // link = articulation->createLink(parent, linkPose);

}

FVector ARArticulation::pid(FVector e, float Ta)
{
  FVector y;

  esum1 = esum1 + e.X;
  y.X = Kp * e.X + Ki * Ta * esum1;
  ealt1 = e.X;

  esum2 = esum2+ e.Y;
  y.Y = Kp * e.Y + Ki * Ta * esum2;
  ealt2 = e.Y;

  esum3 = esum3 + e.Z;
  y.Y = Kp * e.Z + Ki * Ta * esum3;
  ealt3 = e.Z;
  return y;

}
