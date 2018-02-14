#include "RController.h"
#include "RRobot.h"

void URRevoluteController::ControllComand(float DeltaTime)
{

  FQuat Orientation = Target->GetComponentTransform().GetRotation();
  FQuat DeltaQ = Orientation.Inverse() * TargetOrientation;
  FVector Axis;
  float Angle;
  DeltaQ.ToAxisAndAngle(Axis, Angle);
  Angle = FMath::RadiansToDegrees(Angle);
  FVector w = Orientation.RotateVector((Axis * Angle) / DeltaTime);
  Target->SetPhysicsAngularVelocityInDegrees(w,false);

}

void URWheelController::ControllComand(float DeltaTime)
{
  FString Name = Target->GetName();
  FQuat Orientation = Target->GetComponentTransform().GetRotation();

  if(Target->owner->WheelSpinnSpeed.Size()==0)
    {

      if(Caster->GetName().Contains(Name.Left(9)))
        {

          float AngularVelocityWheel = Target->owner->WheelTurnSpeed.Yaw * Target->owner->DistanceWheelCaster /Target->owner->WheelRadius;

          FQuat OrientationCaster = Caster->GetComponentTransform().GetRotation();
          FQuat CasterTargetOrientation = Caster->Controller->TargetOrientation;
          FQuat DeltaA = OrientationCaster.Inverse() * CasterTargetOrientation;
          FVector WTarget;
          FVector WActual;
          FVector DeltaW;
          //WActual = Target->GetPhysicsAngularVelocity();
          if (Name.Contains("r_wheel"))
            {
              WTarget = OrientationCaster.RotateVector(FVector(0.0f, AngularVelocityWheel, 0.0f));
            }
          else if (Name.Contains("l_wheel"))
            {
              WTarget = OrientationCaster.RotateVector(FVector(0.0f,-1* AngularVelocityWheel, 0.0f));
            }
          //DeltaW = WTarget - WActual;
          Target->SetPhysicsAngularVelocityInDegrees(WTarget,false);
        }


    }
  else
    {
      Target->SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(Target->owner->WheelSpinnSpeed),false);
    }


}

void URCasterController::ControllComand(float DeltaTime)
{

  Target->SetWorldRotation(Target->Controller->TargetOrientation, false, nullptr ,ETeleportType::None);

}


void URWheelController::InitController()
{
  for(auto& caster : Target->owner->WheelTurnComponents)
        {
          if(caster->GetName().Contains(GetName().Left(9)))
            {

              Caster = caster;
            }
       }

}

void URCasterOrientationController::ControllComand(float DeltaTime)
{
  FRotator AngularRotation(Target->owner->WheelTurnSpeed * DeltaTime);


  //UE_LOG(LogTemp, Error, TEXT("Target Orientation %s"), *Target->owner->WheelTurnComponents[0]->TargetOrientation.ToString());


  for (auto& Caster : Target->owner->WheelTurnComponents)
    {
      Caster->Controller->TargetOrientation = Caster->Controller->TargetOrientation* AngularRotation.Quaternion();
    }
}


URController*  URControllerFactory::CreateController(FString Type, URStaticMeshComponent* Target)
{
  URController* Controller = nullptr;
  if (Type.Equals("revolute", ESearchCase::IgnoreCase) || Type.Equals("continuous", ESearchCase::IgnoreCase))
    {
      Controller = NewObject<URRevoluteController>(Target->owner, FName(Target->GetName().Append("_Controller").GetCharArray().GetData()));
    }
  else if (Type.Equals("wheel", ESearchCase::IgnoreCase))
    {
      Controller = NewObject<URWheelController>(Target->owner, FName(Target->GetName().Append("_Controller").GetCharArray().GetData()));
    }
  else if (Type.Equals("caster", ESearchCase::IgnoreCase))
    {
      Controller = NewObject<URCasterController>(Target->owner, FName(Target->GetName().Append("_Controller").GetCharArray().GetData()));

    }
  else if (Type.Equals("orientation", ESearchCase::IgnoreCase))
    {
      Controller = NewObject<URCasterOrientationController>(Target->owner, FName(Target->GetName().Append("_Controller").GetCharArray().GetData()));
    }

  if(Controller)
    {
      //UE_LOG(LogTemp, Error, TEXT("Target Name %s"), *Target->GetName());
      //UE_LOG(LogTemp, Error, TEXT("Target Owner Name %s"), *Target->owner->GetName());
      Controller->Target = Target;
      Target->ControllerName = Controller->GetName();
      Target->owner->ControllerList.Add(Controller->GetName(),Controller);

    }

  return Controller;
}
