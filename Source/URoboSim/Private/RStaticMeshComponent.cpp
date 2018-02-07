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
	FQuat OrientationWheel = owner->WheelTurnComponents[0]->GetComponentTransform().GetRotation();
	FVector DeltaW(0.0f);

	FVector Speed2(0.0f, owner->WheelTurnSpeed.Yaw, 0.0f);
	// if (GetName().Contains("r_wheel"))
	// 	{
	// 		//DeltaW[1] = Speed2[1] - GetPhysicsAngularVelocity()[1];
	// 		//AddTorque(DeltaW*100);
	// 		//AddTorque(FVector(0,10000,0));
	// 		//SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(DeltaW),true);

	// 		SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector( Speed2),false);

	// 	}
	// else if (GetName().Contains("l_wheel"))
	// 	{

	// 		//DeltaW[1] = -1 * Speed2[1] - GetPhysicsAngularVelocity()[1];
	// 		//AddTorque(DeltaW*100);
	// 		//AddTorque(FVector(0,-10000,0));
	// 		//SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(DeltaW),true);

	// 		SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(-1 *Speed2),false);
	// 	}
	// else
	FString Name = GetName();
	FQuat Orientation = GetComponentTransform().GetRotation();

	if(Name.Equals("base_link"))
		{
			FRotator AngularRotation(owner->WheelTurnSpeed * DeltaTime);
			for (auto& Caster : owner->WheelTurnComponents)
				{
					Caster->TargetOrientation = Caster->TargetOrientation* AngularRotation.Quaternion();
				}
		}

	else if (Name.Contains("caster_rotation_link"))
		{
			SetWorldRotation(TargetOrientation, false, nullptr ,ETeleportType::None);
			// if (owner->WheelTurnSpeed.Yaw ==0 )
			// 	{
			// 		//ScreenMsg("Target", TargetOrientation.ToString());
			// 		SetWorldRotation(OrientationWheel, false, nullptr ,ETeleportType::None);
			// 	}
			// else
			// 	{
			// 		OrientationWheel = OrientationWheel* Speed.Quaternion();
			// 		SetWorldRotation(OrientationWheel, false, nullptr ,ETeleportType::None);
			// 		TargetOrientation = OrientationWheel;
			// 	}
		}

	else if(Name.Contains("wheel"))
		{
			if(owner->WheelSpinnSpeed.Size()==0)
				{
					//ScreenMsg("Turn Mode");
					for (auto& Caster : owner->WheelTurnComponents)
						{
							if(Caster->GetName().Contains(Name.Left(9)))
								{
									FRotator AngularRotation(owner->WheelTurnSpeed * DeltaTime);
									float AngularVelocityWheel = owner->WheelTurnSpeed.Yaw * owner->DistanceWheelCaster /owner->WheelRadius;
									FQuat OrientationCaster = Caster->GetComponentTransform().GetRotation();
									FQuat CasterTargetOrientation = Caster->TargetOrientation;
									FQuat DeltaA = OrientationCaster.Inverse() * CasterTargetOrientation;
									FVector WTarget;
									FVector WActual;
									FVector DeltaW;
									if (Name.Contains("r_wheel"))
										{
											// WTarget = OrientationCaster.RotateVector(FVector(0.0f, DeltaA.GetAngle()*DeltaA.GetRotationAxis()[2] ,0.0f)*300);
											WTarget = OrientationCaster.RotateVector(FVector(0.0f, AngularVelocityWheel, 0.0f));
											WActual = GetPhysicsAngularVelocity();
											DeltaW = WTarget - WActual;
											//AddAngularImpulseInRadians(DeltaW);
											SetPhysicsAngularVelocityInDegrees(WTarget,false);
										}
									else if (Name.Contains("l_wheel"))
										{
											//WTarget = OrientationCaster.RotateVector(FVector(0.0f, DeltaA.GetAngle()*DeltaA.GetRotationAxis()[2] ,0.0f)*(-1*300));
											WTarget = OrientationCaster.RotateVector(FVector(0.0f,-1* AngularVelocityWheel, 0.0f));
											WActual = GetPhysicsAngularVelocity();
											DeltaW = WTarget - WActual;
											//AddAngularImpulseInRadians(DeltaW);
											SetPhysicsAngularVelocityInDegrees(WTarget,false);
										}
									// if(GetName().Contains("fr_caster_l_wheel"))
									// 	{
									// 		//ScreenMsg("Target", WTarget.ToString());
									// 		//ScreenMsg("Actual", WActual.ToString());
									// 		// ScreenMsg("DeltaA Axis", DeltaA.GetRotationAxis().ToString());
									// 		ScreenMsg("DeltaA Angle", DeltaA.GetAngle());


									// 		ScreenMsg("Actual", OrientationCaster.GetNormalized().ToString());
									// 		ScreenMsg("Target", CasterTargetOrientation.ToString());
									// 	}

								}
						}

				}
			else
				{
					//	ScreenMsg("Spin mode");
					SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(owner->WheelSpinnSpeed),false);
				}




			// for (auto& Caster : owner->WheelTurnComponents)
			// 	{
			// 		if(Caster->GetName().Contains(GetName().Left(9)))
			// 			{
			// 				FQuat Orientation = Caster->GetComponentTransform().GetRotation();
			// 			}
			// 	}

			//DeltaW = Orientation.RotateVector(owner->WheelSpinnSpeed) - GetPhysicsAngularVelocity();
			// FVector AngularVelocity = Orientation.RotateVector(100*DeltaW);
			// if(GetName().Contains("fr_caster_l_wheel"))
			// 	{
			// 		ScreenMsg("Angular Velocity", DeltaW.ToString());
			// 		//ScreenMsg("Angular Velocity", Orientation.RotateVector(owner->WheelSpinnSpeed).ToString());
			// 	}

	//SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(DeltaW),true);

			//AddAngularImpulseInDegrees(AngularVelocity);


			//AddAngularImpulseInDegrees(DeltaW);
		}

	else
		{
			for (auto& Joint : ConnectedJoints)
				{
					if(!Joint.Value.IsParent && CheckJointType(Joint.Value.Type))
						{
							if(!Joint.Value.Name.Contains("wheel") && !Joint.Value.Name.Contains("caster"))
								{
									//PRigidBody->setAngularVelocity(w);
									//UE_LOG(LogTemp, Display, TEXT("Joint %s"), *Joint.Value.Name);

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

									FVector pw =  w;
									// if(Joint.Value.Name.Contains("l_upper_arm_roll_joint"))
									// 	{
									// 		UE_LOG(LogTemp, Display, TEXT("Delta %s"), *DeltaQ.Euler().ToString());
									// 	}

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
