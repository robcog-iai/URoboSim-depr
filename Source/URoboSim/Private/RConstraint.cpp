#include "RConstraint.h"

void URFixedConstraint::Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link)
{
  Constraint = NewObject<UPhysicsConstraintComponent>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
  
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
  Constraint->ConstraintInstance = ConstraintInstance;
}

void URFloatingConstraint::Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link)
{
  URFixedConstraint::Init(ParentComp, Joint, Link);
  ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.f);
  ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.f);
  ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.f);
  Constraint->ConstraintInstance = ConstraintInstance;
}


void URPrismaticConstraint::Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link)
{
  URFixedConstraint::Init(ParentComp, Joint, Link);
  //Currently simplified limit (lower + upper as a value).
  //lower, upper A(radians for revolute joints, meters for prismatic joints)	
  
  // TODO: Make Helpfunction Create Simple limit
  
  float SimpleLimit  = FMath::Abs(Joint->LowerLimit) + FMath::Abs(Joint->UpperLimit);
  ELinearConstraintMotion LinearConstraintMotion = ELinearConstraintMotion::LCM_Limited;


  if (Joint->Axis.X == 1)
	{
	  ConstraintInstance.SetLinearXLimit(LinearConstraintMotion, SimpleLimit);
	  ConstraintInstance.ProfileInstance.LinearDrive.XDrive.bEnablePositionDrive = true;
	  ConstraintInstance.ProfileInstance.LinearDrive.XDrive.MaxForce = Joint->Effort;
	}
  else if (Joint->Axis.Y == 1)
	{
	  ConstraintInstance.SetLinearYLimit(LinearConstraintMotion, SimpleLimit);
	  ConstraintInstance.ProfileInstance.LinearDrive.YDrive.bEnablePositionDrive = true;
	  ConstraintInstance.ProfileInstance.LinearDrive.YDrive.MaxForce = Joint->Effort;
	}

  else if (Joint->Axis.Z == 1)
	{
	  ConstraintInstance.SetLinearZLimit(LinearConstraintMotion, SimpleLimit);
	  ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.bEnablePositionDrive = true;
	  ConstraintInstance.ProfileInstance.LinearDrive.ZDrive.MaxForce = Joint->Effort;
	}
  Constraint->ConstraintInstance = ConstraintInstance;
}



void URRevoluteConstraint::Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link)
{
  URFixedConstraint::Init(ParentComp, Joint, Link);
	//Currently simplified limit (lower + upper as a value).
	//lower, upper A(radians for revolute joints, meters for prismatic joints)	
  
  // TODO: Make Helpfunction Create Simple limit
  float SimpleLimit  = FMath::Abs(Joint->LowerLimit) + FMath::Abs(Joint->UpperLimit);
  EAngularConstraintMotion AngularConstraintMotion = EAngularConstraintMotion::ACM_Limited; 


  
  if (Joint->Axis.X == 1)
	{
	  // Angular motor on X axis needs SLERP drive mode
	  ConstraintInstance.SetAngularSwing1Limit(AngularConstraintMotion, 0.1f);
	  ConstraintInstance.SetAngularSwing2Limit(AngularConstraintMotion, 0.1f);
	  ConstraintInstance.SetAngularTwistLimit(AngularConstraintMotion, SimpleLimit);
	  ConstraintInstance.ProfileInstance.AngularDrive.AngularDriveMode = EAngularDriveMode::SLERP;
	  ConstraintInstance.ProfileInstance.AngularDrive.SlerpDrive.MaxForce = Joint->Effort;
	  ConstraintInstance.ProfileInstance.AngularDrive.SlerpDrive.bEnablePositionDrive = true;
	}
	
  else if (Joint->Axis.Y == 1)
	{
	  ConstraintInstance.SetAngularSwing2Limit(AngularConstraintMotion, SimpleLimit);
	  ConstraintInstance.ProfileInstance.AngularDrive.AngularDriveMode = EAngularDriveMode::TwistAndSwing;
	  ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.MaxForce = Joint->Effort;
	  ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.bEnablePositionDrive = true;
	}
  else if (Joint->Axis.Z == 1)
	{
	  ConstraintInstance.SetAngularSwing1Limit(AngularConstraintMotion, SimpleLimit);
	  ConstraintInstance.ProfileInstance.AngularDrive.AngularDriveMode = EAngularDriveMode::TwistAndSwing;
	  ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.MaxForce = Joint->Effort;
	  ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.bEnablePositionDrive = true;
	}
  Constraint->ConstraintInstance = ConstraintInstance;
}

void URRevoluteConstraint::InitDrive()
{
  	ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.Damping = 1000;
	ConstraintInstance.ProfileInstance.AngularDrive.SwingDrive.Stiffness = 1000;

}


void URPlanarConstraint::Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link)
{
  URFixedConstraint::Init(ParentComp, Joint, Link);
  // A Constraint for planar type. This joint allows motion in a plane perpendicular to the axis.
  if (Joint->Axis.Z == 1) {
	ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0);
	ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0);
	ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
  }
  else if (Joint->Axis.X == 1) {
	ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0);
	ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0);
  }
  else if (Joint->Axis.Y == 1) {
	ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0);
	ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0);
  }
  Constraint->ConstraintInstance = ConstraintInstance;
}



// URConstraint* URConstraintFactory::MakeConstraint(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link)
// {
//   if (Joint->Type.Equals("fixed", ESearchCase::IgnoreCase))
// 	{
// 	  return new URFixedConstraint(ParentComp, Joint, Link);
// 	}
//   else if (Joint->Type.Equals("floating", ESearchCase::IgnoreCase))
// 	{
// 	  return new URFloatingConstraint(ParentComp, Joint, Link);
// 	}
//   else if (Joint->Type.Equals("prismatic", ESearchCase::IgnoreCase))
// 	{
// 	  return new URPrismaticConstraint(ParentComp, Joint, Link);
// 	}
//   else if (Joint->Type.Equals("revolute", ESearchCase::IgnoreCase))
// 	{
// 	  return new URRevoluteConstraint(ParentComp, Joint, Link);
// 	}
//   else if (Joint->Type.Equals("planar",ESearchCase::IgnoreCase))
// 	{
// 	  return new URPlanarConstraint(ParentComp, Joint, Link);
// 	}
//   else
// 	{
// 	  return nullptr;
// 	}
// }

URConstraintFactory::URConstraintFactory(const ObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  bCreateNew = true;
  //SupportedClass = URConstraint::StaticClass();

  
}


URConstraint* URConstraintFactory::MakeConstraint(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link)
{
  if (Joint->Type.Equals("fixed", ESearchCase::IgnoreCase))
	{
	  URConstraint* Constraint = NewObject<URFixedConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  Constraint->Init(ParentComp, Joint, Link);
	  return  Constraint;
	}
  else if (Joint->Type.Equals("floating", ESearchCase::IgnoreCase))
	{
	  URConstraint* Constraint = NewObject<URFloatingConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  Constraint->Init(ParentComp, Joint, Link);
	  return Constraint;
	}
  else if (Joint->Type.Equals("prismatic", ESearchCase::IgnoreCase))
	{
	  URConstraint* Constraint = NewObject<URPrismaticConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  Constraint->Init(ParentComp, Joint, Link);
	  return Constraint;
	}
  else if (Joint->Type.Equals("revolute", ESearchCase::IgnoreCase))
	{
	  URConstraint* Constraint = NewObject<URRevoluteConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  Constraint->Init(ParentComp, Joint, Link);
	  return Constraint;
	}
  else if (Joint->Type.Equals("planar",ESearchCase::IgnoreCase))
	{
	  URConstraint* Constraint = NewObject<URPlanarConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  Constraint->Init(ParentComp, Joint, Link);
	  return Constraint;
	}
  else
	{
	  return nullptr;
	}
}
