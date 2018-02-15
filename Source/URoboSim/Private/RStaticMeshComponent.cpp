// Fill out your copyright notice in the Description page of Project Settings.
#include "RStaticMeshComponent.h"
#include "RRobot.h"
#include "GameFramework/Pawn.h"
#include "PhysXIncludes.h"
#include "PhysicsPublic.h"
#include "RController.h"
#include "Runtime/Engine/Private/PhysicsEngine/PhysXSupport.h"

void FMySecondaryTickFunction::ExecuteTick(
    float DeltaTime,
    ELevelTick TickType,
    ENamedThreads::Type CurrentThread,
    const FGraphEventRef& MyCompletionGraphEvent)
{
  if (Target && !Target->IsPendingKill() && !Target->IsUnreachable())
  {
    FScopeCycleCounterUObject ActorScope(Target);
    Target->TickPostPhysics(DeltaTime /* * Target->CustomTimeDilation*/, TickType, *this);
  }
}

FString FMySecondaryTickFunction::DiagnosticMessage()
{
  return Target->GetFullName() + TEXT("[TickActor2]");
}

URStaticMeshComponent::URStaticMeshComponent()
{
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.bStartWithTickEnabled = true;

  SecondaryComponentTick.TickGroup = TG_PostPhysics;
  SecondaryComponentTick.bCanEverTick = true;
  SecondaryComponentTick.bStartWithTickEnabled = true;

  OnCalculateCustomPhysics.BindUObject(this, &URStaticMeshComponent::SubstepTick);

  FrameCount = 0;

}

void URStaticMeshComponent::BeginPlay()
{
  Super::BeginPlay();
  UE_LOG(LogTemp, Log, TEXT("%s"), *GetName());

  owner = Cast<ARRobot>(GetOwner());
  if (!IsTemplate() && SecondaryComponentTick.bCanEverTick)
  {
    SecondaryComponentTick.Target = this;
    SecondaryComponentTick.SetTickFunctionEnable(SecondaryComponentTick.bStartWithTickEnabled);
    SecondaryComponentTick.RegisterTickFunction(owner->GetLevel());
  }

  PRigidBody = GetBodyInstance()->GetPxRigidBody_AssumesLocked();

  StartH = GetComponentLocation().Z;
  StartPos = GetComponentLocation();
  StartRot = GetComponentRotation();

  StartRelativeTransform = GetRelativeTransform();
  StartPos = StartRelativeTransform.GetLocation();
}

void URStaticMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  FrameCount++;

  if (owner->bEnableLogging)
    UE_LOG(LogClass, Log, TEXT("%d URStaticMeshComponent::TickComponent DeltaTime : %f, Z : %f"), FrameCount, DeltaTime, GetCurrentLocation().Z);

  if (owner->bSubstepEnabled)
  {
    //Add custom physics forces each tick
    GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
  }
  else 
  {
    //DoPhysics(DeltaTime, false);
  }
}

void URStaticMeshComponent::SubstepTick(float DeltaTime, FBodyInstance* InBodyInstance)
{
  DoPhysics(DeltaTime, true);
}

void URStaticMeshComponent::DoPhysics(float DeltaTime, bool InSubstep)
{
  if(Controller)
  {
    //ScreenMsg("Command");
    Controller->ControllComand(DeltaTime);
  }
  // else
  // 	{
  // 		ScreenMsg("No Command");
  // 	}

  // FVector DeltaW(0.0f);
  // FString Name = GetName();
  // FQuat Orientation = GetComponentTransform().GetRotation();

  // if(Name.Equals("base_link"))
  // 	{

  // 		FRotator AngularRotation(owner->WheelTurnSpeed * DeltaTime);
  // 		for (auto& Caster : owner->WheelTurnComponents)
  // 			{
  // 				Caster->TargetOrientation = Caster->TargetOrientation* AngularRotation.Quaternion();
  // 			}
  // 	}

  // else if (Name.Contains("caster_rotation_link"))
  // 	{
  // 		SetWorldRotation(TargetOrientation, false, nullptr ,ETeleportType::None);
  // 	}

  // else if(Name.Contains("wheel"))
  // 	{
  // 		if(owner->WheelSpinnSpeed.Size()==0)
  // 			{
  // 				for (auto& Caster : owner->WheelTurnComponents)
  // 					{
  // 						if(Caster->GetName().Contains(Name.Left(9)))
  // 							{
  // 								FRotator AngularRotation(owner->WheelTurnSpeed * DeltaTime);
  // 								float AngularVelocityWheel = owner->WheelTurnSpeed.Yaw * owner->DistanceWheelCaster /owner->WheelRadius;
  // 								FQuat OrientationCaster = Caster->GetComponentTransform().GetRotation();
  // 								FQuat CasterTargetOrientation = Caster->TargetOrientation;
  // 								FQuat DeltaA = OrientationCaster.Inverse() * CasterTargetOrientation;
  // 								FVector WTarget;
  // 								FVector WActual;
  // 								FVector DeltaW;
  // 								if (Name.Contains("r_wheel"))
  // 									{
  // 										WTarget = OrientationCaster.RotateVector(FVector(0.0f, AngularVelocityWheel, 0.0f));
  // 										WActual = GetPhysicsAngularVelocity();
  // 										DeltaW = WTarget - WActual;
  // 										SetPhysicsAngularVelocityInDegrees(WTarget,false);
  // 									}
  // 								else if (Name.Contains("l_wheel"))
  // 									{
  // 										WTarget = OrientationCaster.RotateVector(FVector(0.0f,-1* AngularVelocityWheel, 0.0f));
  // 										WActual = GetPhysicsAngularVelocity();
  // 										DeltaW = WTarget - WActual;
  // 										SetPhysicsAngularVelocityInDegrees(WTarget,false);
  // 									}

  // 							}
  // 					}

  // 			}
  // 		else
  // 			{
  // 				SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(owner->WheelSpinnSpeed),false);
  // 			}
  // 	}

  // else
  // 	{
  // 		for (auto& Joint : ConnectedJoints)
  // 			{
  // 				if(!Joint.Value.IsParent && CheckJointType(Joint.Value.Type))
  // 					{
  // 						if(!Joint.Value.Name.Contains("wheel") && !Joint.Value.Name.Contains("caster"))
  // 							{
  // 								FQuat OrientationWorld = GetComponentTransform().GetRotation();
  // 								FQuat DeltaQ = Orientation.Inverse() * TargetOrientation;
  // 								FVector Axis;
  // 								float Angle;
  // 								DeltaQ.ToAxisAndAngle(Axis, Angle);
  // 								Angle = FMath::RadiansToDegrees(Angle);
  // 								FVector w = OrientationWorld.RotateVector((Axis * Angle) / DeltaTime);
  // 								SetPhysicsAngularVelocityInDegrees(w,false);
  // 						}


  // 					}

  // 			}

  // 	}

}

void URStaticMeshComponent::TickPostPhysics(
    float DeltaSeconds,
    ELevelTick TickType,
    FMySecondaryTickFunction& ThisTickFunction
    )
{
  // Non-player update.
  const bool bShouldTick =
    ((TickType != LEVELTICK_ViewportsOnly) || owner->ShouldTickIfViewportsOnly());
  if (bShouldTick)
  {
    if (!IsPendingKill() && GetWorld())
    {
      if (owner->GetWorldSettings() != NULL && !IsRunningDedicatedServer())
      {
        // Here your post physics tick stuff

        // if (GetName().Contains("fr_caster_r_wheel_link"))
        // 	{
        // 		ScreenMsg("Angular Speed",  GetPhysicsAngularVelocity().ToString());
        // 	}


        if (owner->bEnableLogging)
          UE_LOG(LogClass, Log, TEXT("%d URStaticMeshComponent::TickPostPhysics DeltaTime: %f, Z: %f"), FrameCount, DeltaSeconds, GetCurrentLocation().Z);
      }
    }
  }
}

FVector URStaticMeshComponent::GetCurrentLocation() 
{
  PxTransform PTransform = PRigidBody->getGlobalPose();
  return FVector(PTransform.p.x, PTransform.p.y, PTransform.p.z);
}

FRotator URStaticMeshComponent::GetCurrentRotation() 
{
  PxTransform PTransform = PRigidBody->getGlobalPose();
  return FRotator(FQuat(PTransform.q.x, PTransform.q.y, PTransform.q.z, PTransform.q.w));
}

FVector URStaticMeshComponent::GetCurrentAngularVelocity() 
{
  PxVec3 PAngVelocity = PRigidBody->getAngularVelocity();
  return FMath::RadiansToDegrees(FVector(PAngVelocity.x, PAngVelocity.y, PAngVelocity.z));
}

FVector URStaticMeshComponent::GetCurrentVelocity() 
{
  PxVec3 PVelocity = PRigidBody->getLinearVelocity();
  return FVector(PVelocity.x, PVelocity.y, PVelocity.z);
}

void URStaticMeshComponent::AddConnectedJoint(FRJoint Joint)
{

}

bool URStaticMeshComponent::CheckJointType(FString Type)
{
  bool check = false;
  if (Type.Equals("revolute", ESearchCase::IgnoreCase) || Type.Equals("continuous", ESearchCase::IgnoreCase))
  {
    check = true;
  }
  return check;
}

FQuat URStaticMeshComponent::GetLocalTransform()
{
  FQuat LocalTransform;
  LocalTransform = GetComponentTransform().GetRelativeTransform(Parent->GetComponentTransform()).GetRotation();
  return  LocalTransform;
}


