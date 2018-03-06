#include "RController.h"
#include "RRobot.h"
#include "RControllerComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"


void URController::InitController()
{
    Owner = Cast<ARRobot>(GetOuter());
}

void URInputController::InitController()
{
    Super::InitController();
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        UInputComponent* IC = PC->InputComponent;
        if (IC)
        {
            SetupInputBindings(IC);
        }
    }
}

void URPR2InputController::SetupInputBindings(UInputComponent* IC)
{
    IC->BindAxis("MoveForward", this, &URPR2InputController::MoveForward);
    IC->BindAxis("TurnWheels", this, &URPR2InputController::TurnWheels);
}


void URPR2InputController::TurnWheels(float AxisValue)
{
    ControllerComponent->WheelTurnSpeed = FRotator(0.0f,AxisValue, 0.0f ) * 80;
}


void URPR2InputController::MoveForward(float AxisValue)
{
    ControllerComponent->WheelSpinnSpeed = FVector(0.0f, AxisValue, 0.0f) * 1000;
}



void URJointController::SetTargetOrientation()
{
    TargetOrientation = Target->GetLocalTransform();
}

void URRevoluteController::ControllComand(float DeltaTime)
{
    FQuat Orientation = Target->GetComponentTransform().GetRotation();
    FQuat OrientationLocal = Target->GetLocalTransform();
    FQuat DeltaQ =  TargetOrientation*OrientationLocal.Inverse();
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

    if(ControllerComponent->WheelSpinnSpeed.Size()==0)
    {
        float AngularVelocityWheel = ControllerComponent->WheelTurnSpeed.Yaw * ControllerComponent->DistanceWheelCaster /  ControllerComponent->WheelRadius;
        FQuat OrientationCaster = Caster->GetComponentTransform().GetRotation();
        FVector WTarget;
        if (Name.Contains("r_wheel"))
        {
            WTarget = OrientationCaster.RotateVector(FVector(0.0f, AngularVelocityWheel, 0.0f));
        }
        else if (Name.Contains("l_wheel"))
        {
            WTarget = OrientationCaster.RotateVector(FVector(0.0f,-1* AngularVelocityWheel, 0.0f));
        }
        Target->SetPhysicsAngularVelocityInDegrees(WTarget,false);
    }
    else
    {
        Target->SetPhysicsAngularVelocityInDegrees(Orientation.RotateVector(ControllerComponent->WheelSpinnSpeed),false);
    }
}

void URCasterController::SetTargetOrientation()
{
    TargetOrientation = Target->GetComponentTransform().GetRotation();
}

void URCasterController::ControllComand(float DeltaTime)
{
    Target->SetWorldRotation(Target->Controller->TargetOrientation, false, nullptr ,ETeleportType::None);
}

void URWheelController::InitController()
{
    Super::InitController();
    for(auto& caster : ControllerComponent->WheelTurnComponents)
    {
        if(caster->GetName().Contains(GetName().Left(9)))
        {
            Caster = caster;
        }
    }

}

void URCasterOrientationController::ControllComand(float DeltaTime)
{
    FRotator AngularRotation(  ControllerComponent->WheelTurnSpeed * DeltaTime);
    for (auto& Caster : ControllerComponent->WheelTurnComponents)
    {
        Caster->Controller->TargetOrientation = Caster->Controller->TargetOrientation* AngularRotation.Quaternion();
    }
}

URJointController*  URControllerFactory::CreateController(FString Type, URStaticMeshComponent* Target)
{
    URJointController* Controller = nullptr;
    if (Type.Equals("revolute", ESearchCase::IgnoreCase) || Type.Equals("continuous", ESearchCase::IgnoreCase))
    {
        Controller = NewObject<URRevoluteController>(Target->Owner, FName(Target->GetName().Append("_Controller").GetCharArray().GetData()));
    }
    else if (Type.Equals("wheel", ESearchCase::IgnoreCase))
    {
        Controller = NewObject<URWheelController>(Target->Owner, FName(Target->GetName().Append("_Controller").GetCharArray().GetData()));
    }
    else if (Type.Equals("caster", ESearchCase::IgnoreCase))
    {
        Controller = NewObject<URCasterController>(Target->Owner, FName(Target->GetName().Append("_Controller").GetCharArray().GetData()));

    }
    else if (Type.Equals("orientation", ESearchCase::IgnoreCase))
    {
        Controller = NewObject<URCasterOrientationController>(Target->Owner, FName(Target->GetName().Append("_Controller").GetCharArray().GetData()));
    }

    if(Controller)
    {
        Controller->Target = Target;
        Target->ControllerName = Controller->GetName();
    }
    return Controller;
}



URInputController* URControllerFactory::CreateInputController(FString Type, ARRobot* Owner)
{
    UE_LOG(LogTemp, Warning, TEXT("create Controller"));
    URInputController* Controller = nullptr;
    if (Type.Equals("pr2", ESearchCase::IgnoreCase))
    {
        Controller = NewObject<URPR2InputController>(Owner, FName(TEXT("InputController")));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Input Controller generated"));
    }
    return Controller;
}
