// Fill out your copyright notice in the Description page of Project Settings.
#include "RStaticMeshComponent.h"
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
		float CurrError = GetCurrentLocation().Z - StartH;

		float Velocity = GetCurrentVelocity().Z;

		float force = -(CurrError * owner->KSpring + Velocity * owner->Damping);

		if (InSubstep) 
		{
			PRigidBody->addForce(PxVec3(0.0f, 0.0f, force), physx::PxForceMode::eFORCE, true);
		}
		else 
		{
			AddForce(FVector(0.0f, 0.0f, force));
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
