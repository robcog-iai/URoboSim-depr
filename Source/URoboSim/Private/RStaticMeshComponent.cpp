// Fill out your copyright notice in the Description page of Project Settings.
#include "RStaticMeshComponent.h"
#include "RRobot.h"
#include "GameFramework/Pawn.h"
#include "PhysXIncludes.h"
#include "PhysicsPublic.h"
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


	for (auto& Joint : ConnectedJoints)
		{
			if(!Joint.Value.IsParent && CheckJointType(Joint.Value.Type))
				{
					FTransform ParentTransform = Parent->GetComponentTransform();
					TargetOrientation = GetLocalTransform();

					ParentTransform.ToString();
					//UE_LOG(LogTemp, Log, TEXT("Origin %s"), *ParentTransform.ToString());
				}
		}
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
	//if (owner->bEnableLogging)
		//UE_LOG(LogClass, Log, TEXT("%d URStaticMeshComponent::SubstepTick DeltaTime : %f, Z : %f"), FrameCount, DeltaTime, GetCurrentLocation().Z);

	DoPhysics(DeltaTime, true);
}

void URStaticMeshComponent::DoPhysics(float DeltaTime, bool InSubstep)
{

	if (PRigidBody)
	{
		for (auto& Joint : ConnectedJoints)
			{
				if(!Joint.Value.IsParent && CheckJointType(Joint.Value.Type))
					{
						if(Joint.Value.Name.Contains("caster_rotation_joint"))
							{

								for(auto& wheel : owner->WheelTurnComponents)
									{

										FQuat Orientation = wheel->GetComponentTransform().GetRotation();
										wheel->SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(owner->WheelTurnSpeed),false);
									}
							}

						else if(Joint.Value.Name.Contains("wheel_joint"))
							{
								// TArray<URStaticMeshComponent*> Wheels =owner->WheelComponents;
								// FQuat Orientation = Wheels[0]->GetComponentTransform().GetRotation();
								// wheel->SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(owner->WheelSpinnSpeed),false);
								// for(int i = 1, i<WheelComponents.Num(), i++)
								// 	{

								// 	}

								for(auto& wheel : owner->WheelComponents)
									{

										FQuat Orientation = wheel->GetComponentTransform().GetRotation();
									wheel->SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(owner->WheelSpinnSpeed),false);
									}


							}
						else if(!Joint.Value.Name.Contains("wheel"))
							{
								//PRigidBody->setAngularVelocity(w);
								//UE_LOG(LogTemp, Display, TEXT("Apply Velocities"));
								FQuat Orientation = GetLocalTransform();
								FQuat OrientationWorld = GetComponentTransform().GetRotation();
								// FRotator CurrError;
								// //CurrError = StartRot - GetCurrentRotation();
								// CurrError = StartRot - GetComponentRotation();

								// //FVector w = CurrError.Vector()/ DeltaTime;
								FQuat DeltaQ = Orientation.Inverse() * TargetOrientation;
								FVector Axis;
								float Angle;
								DeltaQ.ToAxisAndAngle(Axis, Angle);
								Angle = FMath::RadiansToDegrees(Angle);
								FVector w = OrientationWorld.RotateVector((Axis * Angle) / DeltaTime);

								FVector pw = 0.1 * w;
								if(Joint.Value.Name.Contains("l_upper_arm_roll_joint"))
									{
										UE_LOG(LogTemp, Display, TEXT("Delta %s"), *DeltaQ.Euler().ToString());
									}

								// ScreenMsg("Delta", DeltaQ.ToString());

								SetPhysicsAngularVelocityInDegrees(w,false);
								//AddAngularImpulse(pw);
								//AddTorqueInDegrees(w);
							}

					}

			}
	}
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
