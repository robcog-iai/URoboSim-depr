#include "RConstraint.h"
#include "RRobot.h"
#include "RMeshHandler.h"

URConstraint::URConstraint()
{

}

URRevoluteConstraint::URRevoluteConstraint()
{
    Type = "revolute";
}
URFixedConstraint::URFixedConstraint()
{
    Type = "fixed";
}
URContinuousConstraint::URContinuousConstraint()
{
    Type = "continuous";

}
URFloatingConstraint::URFloatingConstraint()
{
    Type = "floating";
}

URPlanarConstraint::URPlanarConstraint()
{
    Type = "planar";
}
URPrismaticConstraint::URPrismaticConstraint()
{
    Type = "prismatic";
}

void URFixedConstraint::Init(URMeshHandler* MeshH)
{
    MeshHandler = MeshH;
    ConstraintInstance.SetDisableCollision(true);
    ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0);
    ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0);
    ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0);
    ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
    ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
    ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
    ConstraintInstance.AngularRotationOffset = FRotator(0, 0, 0);
    ConstraintInstance.ProfileInstance.TwistLimit.bSoftConstraint = false;
    ConstraintInstance.ProfileInstance.ConeLimit.bSoftConstraint = false;
}

float URFixedConstraint::CreateContraintLimit()
{
    float SimpleLimit  = (FMath::Abs(MeshHandler->Joint->LowerLimit) + FMath::Abs(MeshHandler->Joint->UpperLimit))/2.0f;
    if(SimpleLimit>180.)
    {
        SimpleLimit = 180.;
    }
    return SimpleLimit;
}

void URFixedConstraint::SetupConstraint()
{
    SetDisableCollision(true);
    AttachToComponent(MeshHandler->ParentComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
    RegisterComponent();
    ApplyWorldOffset(FVector(0), false);
    MeshHandler->PositionLinkAndJoint();

    SetRelativeRotation(MeshHandler->Joint->Rotation);

    // Connect joint to parent and child
    ConstraintActor1 = (AActor*)MeshHandler->Owner;
    ConstraintActor2 = (AActor*)MeshHandler->Owner;

    SetWorldLocation(MeshHandler->MeshComp->GetComponentLocation());
    SetConstrainedComponents(MeshHandler->ParentComp, NAME_None, MeshHandler->MeshComp, NAME_None);
    ChildName = MeshHandler->Link->Name;

    FRotator ParentRotation = MeshHandler->ParentComp->GetComponentRotation();
    FRotator ChildRotation =  MeshHandler->MeshComp->GetComponentRotation();
    FQuat InitialRotationRel = FQuat(ParentRotation).Inverse() * FQuat(ChildRotation);

    MeshHandler->Owner->OriginRotations.Add(MeshHandler->Joint->Name, InitialRotationRel);
    MeshHandler->Owner->JointComponents.Add(MeshHandler->Joint->Name, this);

}

void URFloatingConstraint::Init(URMeshHandler* MeshH)
{
    URFixedConstraint::Init(MeshH);
    ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.f);
    ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.f);
    ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.f);

}


void URPrismaticConstraint::Init(URMeshHandler* MeshH)
{
    URFixedConstraint::Init(MeshH);
    //Currently simplified limit (lower + upper as a value).
    //lower, upper A(radians for revolute joints, meters for prismatic joints)

    float SimpleLimit  = CreateContraintLimit();
    ELinearConstraintMotion LinearConstraintMotion = ELinearConstraintMotion::LCM_Limited;

    if (MeshHandler->Joint->Axis.X == 1)
    {
        ConstraintInstance.SetLinearXLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.XDrive.MaxForce = MeshHandler->Joint->Effort;
    }

    else if (MeshHandler->Joint->Axis.Y == 1)
    {
        ConstraintInstance.SetLinearYLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.YDrive.MaxForce = MeshHandler->Joint->Effort;
    }

    else if (MeshHandler->Joint->Axis.Z == 1)
    {
        ConstraintInstance.SetLinearZLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.MaxForce = MeshHandler->Joint->Effort;
    }

}


void URPrismaticConstraint::InitDrive()
{
    ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.bEnablePositionDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.LinearDrive.YDrive.bEnablePositionDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.LinearDrive.XDrive.bEnablePositionDrive = bEnableMotor;
    SetLinearDriveParams(10000.0f, 10000.0f, 10000.0f);

}

void URRevoluteConstraint::Init(URMeshHandler* MeshH)
{
    URFixedConstraint::Init(MeshH);
    //Currently simplified limit (lower + upper as a value).
    //lower, upper A(radians for revolute joints, meters for prismatic joints)

    float SimpleLimit  = CreateContraintLimit();
    EAngularConstraintMotion AngularConstraintMotion = EAngularConstraintMotion::ACM_Limited;

    if (MeshHandler->Joint->Axis.X == 1)
    {
        // Angular motor on X axis needs SLERP drive mode
        ConstraintInstance.SetAngularSwing1Limit(AngularConstraintMotion, 0.1f);
        ConstraintInstance.SetAngularSwing2Limit(AngularConstraintMotion, 0.1f);
        ConstraintInstance.SetAngularTwistLimit(AngularConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.AngularDrive.AngularDriveMode = EAngularDriveMode::SLERP;
        ConstraintInstance.ProfileInstance.AngularDrive.SlerpDrive.MaxForce = MeshHandler->Joint->Effort;
    }

    else if (MeshHandler->Joint->Axis.Y == 1)
    {
        ConstraintInstance.SetAngularSwing2Limit(AngularConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.AngularDrive.AngularDriveMode = EAngularDriveMode::TwistAndSwing;
        ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.MaxForce = MeshHandler->Joint->Effort;
    }
    else if (MeshHandler->Joint->Axis.Z == 1)
    {
        ConstraintInstance.SetAngularSwing1Limit(AngularConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.AngularDrive.AngularDriveMode = EAngularDriveMode::TwistAndSwing;
        ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.MaxForce = MeshHandler->Joint->Effort;
    }
}

void URRevoluteConstraint::InitDrive()
{
    float Strength = 100000000.0f;
    SetAngularDriveParams(Strength, Strength, Strength);
    SetLinearDriveParams(Strength, Strength, Strength);
    ConstraintInstance.ProfileInstance.AngularDrive.SlerpDrive.bEnablePositionDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.AngularDrive.SlerpDrive.bEnableVelocityDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.bEnableVelocityDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.bEnablePositionDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.bEnablePositionDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.LinearDrive.YDrive.bEnablePositionDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.LinearDrive.XDrive.bEnablePositionDrive = bEnableMotor;
}


void URPlanarConstraint::Init(URMeshHandler* MeshH)
{
    URFixedConstraint::Init(MeshH);
    // A Constraint for planar type. This joint allows motion in a plane perpendicular to the axis.

    float SimpleLimit  = FMath::Abs(MeshHandler->Joint->LowerLimit) + FMath::Abs(MeshHandler->Joint->UpperLimit);
    ELinearConstraintMotion LinearConstraintMotion = ELinearConstraintMotion::LCM_Limited;

    if (MeshHandler->Joint->Axis.X == 1)
    {
        ConstraintInstance.SetLinearYLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.YDrive.bEnablePositionDrive = bEnableMotor;
        ConstraintInstance.ProfileInstance.LinearDrive.YDrive.MaxForce = MeshHandler->Joint->Effort;
        ConstraintInstance.SetLinearZLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.bEnablePositionDrive = bEnableMotor;
        ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.MaxForce = MeshHandler->Joint->Effort;
    }

    else if (MeshHandler->Joint->Axis.Y == 1)
    {
        ConstraintInstance.SetLinearZLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.bEnablePositionDrive = bEnableMotor;
        ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.MaxForce = MeshHandler->Joint->Effort;
        ConstraintInstance.SetLinearXLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.XDrive.bEnablePositionDrive = bEnableMotor;
        ConstraintInstance.ProfileInstance.LinearDrive.XDrive.MaxForce = MeshHandler->Joint->Effort;

    }

    else if (MeshHandler->Joint->Axis.Z == 1)
    {
        ConstraintInstance.SetLinearYLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.YDrive.bEnablePositionDrive = bEnableMotor;
        ConstraintInstance.ProfileInstance.LinearDrive.YDrive.MaxForce = MeshHandler->Joint->Effort;
        ConstraintInstance.SetLinearXLimit(LinearConstraintMotion, SimpleLimit);
        ConstraintInstance.ProfileInstance.LinearDrive.XDrive.bEnablePositionDrive = bEnableMotor;
        ConstraintInstance.ProfileInstance.LinearDrive.XDrive.MaxForce = MeshHandler->Joint->Effort;
    }
}


void URContinuousConstraint::Init(URMeshHandler* MeshH)
{
    URFixedConstraint::Init(MeshH);

    if (MeshHandler->Joint->Axis.Z == 1)
    {
        ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0);
    }
    else if (MeshHandler->Joint->Axis.X == 1)
    {
        ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0);
    }
    else if (MeshHandler->Joint->Axis.Y == 1)
    {
        ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0);
    }
}

void URContinuousConstraint::InitDrive()
{
    float Strength = 1000000.0f;
    SetAngularDriveParams(Strength, Strength, Strength);

    ConstraintInstance.ProfileInstance.AngularDrive.SlerpDrive.bEnableVelocityDrive = bEnableMotor;
    ConstraintInstance.ProfileInstance.AngularDrive.SlerpDrive.bEnablePositionDrive = bEnableMotor;
}


// URConstraintFactory::URConstraintFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
// {
//   bCreateNew = true;
//   //SupportedClass = URConstraint::StaticClass();


// }


// URConstraint* URConstraintFactory::MakeConstraint(URMeshHandler* MeshHandler)
// {
//   if (MeshHandler->Joint->Type.Equals("fixed", ESearchCase::IgnoreCase))
// 	{
// 	  URConstraint* Constraint = NewObject<URFixedConstraint>(ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
// 	  Constraint->Init(MeshH);
// 	}
//   else if (MeshHandler->Joint->Type.Equals("floating", ESearchCase::IgnoreCase))
// 	{
// 	  URConstraint* Constraint = NewObject<URFloatingConstraint>(ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
// 	  Constraint->Init(MeshH);
// 	}
//   else if (MeshHandler->Joint->Type.Equals("prismatic", ESearchCase::IgnoreCase))
// 	{
// 	  URConstraint* Constraint = NewObject<URPrismaticConstraint>(ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
// 	  Constraint->Init(MeshH);
// 	}
//   else if (MeshHandler->Joint->Type.Equals("revolute", ESearchCase::IgnoreCase))
// 	{
// 	  URConstraint* Constraint = NewObject<URRevoluteConstraint>(ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
// 	  Constraint->Init(MeshH);
// 	}
//   else if (MeshHandler->Joint->Type.Equals("planar",ESearchCase::IgnoreCase))
// 	{
// 	  URConstraint* Constraint = NewObject<URPlanarConstraint>(ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
// 	  Constraint->Init(MeshH);

// 	}
// 	  return Constraint;
// }
