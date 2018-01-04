// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "IURoboSim.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Structs.h"
#include "RConstraint.generated.h"

UCLASS()
class UROBOSIM_API URConstraint : public UObject
{
  GENERATED_BODY()
	public:
	URConstraint(){};
  virtual void InitDrive();
  virtual void Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link);
  UPhysicsConstraintComponent* Constraint;
  FConstraintInstance ConstraintInstance;

};

UCLASS()
class UROBOSIM_API URFixedConstraint : public URConstraint
{
  GENERATED_BODY()
	public:
	URFixedConstraint(){};
  void Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link);
  void InitDrive(){};
};

UCLASS()
class UROBOSIM_API URFloatingConstraint : public URFixedConstraint
{
  GENERATED_BODY()
	public:
	URFloatingConstraint(){};
  void Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link);
  void InitDrive(){};
};

UCLASS()
class UROBOSIM_API URPrismaticConstraint : public URFixedConstraint
{
  GENERATED_BODY()
	public:
	URPrismaticConstraint(){};
  void Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link);
  void InitDrive(){};
};

UCLASS()
class UROBOSIM_API URRevoluteConstraint : public URFixedConstraint
{
  GENERATED_BODY()
	public:
	URRevoluteConstraint(){};
  void Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link);
  void InitDrive();
};

UCLASS()
class UROBOSIM_API URPlanarConstraint : public URFixedConstraint
{
  GENERATED_BODY()
	public:
	URPlanarConstraint(){};
  void Init(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link);
  void InitDrive(){};
};

UCLASS()
class  UROBOSIM_API URConstraintFactory : public UFactory
{
  GENERATED_BODY()
private:
	URConstraintFactory(const FObjectInitializer& ObjectInitializer);
  virtual ~URConstraintFactory();
	public:
    UPROPERTY()
	TSubclassOf<class URConstraint>  NewConstraintClass;
	static URConstraint* MakeConstraint(USceneComponent* ParentComp, FRJoint* Joint, FRLink* Link);
};
