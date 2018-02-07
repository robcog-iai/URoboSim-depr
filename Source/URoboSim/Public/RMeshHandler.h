// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "IURoboSim.h"
#include "UnrealEd.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Structs.h"
#include "RConstraint.h"
#include "RStaticMeshComponent.h"
#include "RMeshHandler.generated.h"

UCLASS()
class UROBOSIM_API URMeshHandler : public UObject
{
  GENERATED_BODY()
	public:

  FRLink* Link;
  FRJoint* Joint;
  UStaticMesh* Mesh;
  URStaticMeshComponent* MeshComp;
  UShapeComponent* ShapeComp;
  URStaticMeshComponent* Root;
  URStaticMeshComponent* ParentComp;
  URStaticMeshComponent* ParentLink;
  FVector Scale;
  FVector LocationCollision;
  FVector LocationVisual;
  FVector LinkOriginLocation;

  TMap<FString, FVector> OriginLocations;


  UStaticMesh* CylinderMesh;
  UStaticMesh* CubeMesh;
  UStaticMesh* SphereMesh;

  URConstraint* Constraint;

  bool bEnableCollisionTags = true;
  bool bUseVisual;
  bool bUseCollision;
  bool bEnableShapeCollisions = false;
  TArray<FString> collisionFilterArr = { "torso","wheel_link", "shoulder", "arm", "finger_link" };
  //TArray<FString> GravityDisabledTags = { "gripper","wrist", "arm", "elbow" };
TArray<FString> GravityDisabledTags = { "base"};
  URMeshHandler();




  //TEMPLATE Load Obj From Path
  template <typename ObjClass>
	FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path);

  // Load Static Mesh From Path
  FORCEINLINE UStaticMesh* LoadMeshFromPath(const FName& Path);

  //Create the Link (MeshComponent) of the Robot

  virtual bool CreateLink(URStaticMeshComponent* RootComponent, TMap<FString, URStaticMeshComponent*> LinkComponents, TMap<FString, FVector> OriginLocation, FRNode* Node);
  virtual void CreateMesh(){};
  virtual void CreateMeshComponent();
  virtual void ConfigureMeshComponent();
  virtual void ConfigureLinkPhysics();
  virtual void AddConnectedJoint();
  virtual FRConnectedJoint CreateConnectedJoint(bool IsParent);
  //virtual void CreateConstraint();
  //virtual void ConfigureConstraint();
  virtual void ConnectPositionLink();

 private:
  class ARRobot *owner;
  friend class URMeshFactory;

  //FRNode* Node;

};


UCLASS()
class UROBOSIM_API URMeshHandlerBox : public URMeshHandler
{
  GENERATED_BODY()
	public:
	URMeshHandlerBox(){};

  void CreateMesh();
};

UCLASS()
class UROBOSIM_API URMeshHandlerSphere : public URMeshHandler
{
  GENERATED_BODY()
	public:
	URMeshHandlerSphere(){};

  void CreateMesh();
};

UCLASS()
class UROBOSIM_API URMeshHandlerCylinder : public URMeshHandler
{
  GENERATED_BODY()
	public:
	URMeshHandlerCylinder(){};
  void CreateMesh();
};

UCLASS()
class UROBOSIM_API URMeshHandlerCustom: public URMeshHandler
{
  GENERATED_BODY()
	public:
	URMeshHandlerCustom(){};
  void CreateMesh();
  void CreateMeshComponent();
  void ConfigureLinkPhysics();

};


UCLASS()
class UROBOSIM_API URMeshFactory : public UObject
{
  GENERATED_BODY()
	public:
	URMeshFactory(){};
  //URMeshHandler* CreateMeshHandler(ARRobot* Owner, FRNode* Node);
  URMeshHandler* CreateMeshHandler(URStaticMeshComponent* RootComponent, FRNode* Node);
};
