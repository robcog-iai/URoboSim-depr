// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "IURoboSim.h"
#include "UnrealEd.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Structs.h"
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
  USceneComponent* Root;
  UStaticMesh* CylinderMesh;
  UStaticMesh* CubeMesh;
  UStaticMesh* SphereMesh;
  bool bEnableCollisionTags = true;
  bool bUseVisual;
  bool bUseCollision;

  URMeshHandler();
  void CreateMeshType();
  void ConfigureMeshComp();

  //TEMPLATE Load Obj From Path
  template <typename ObjClass>
	FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path);

  // Load Static Mesh From Path
  FORCEINLINE UStaticMesh* LoadMeshFromPath(const FName& Path);

  //Create the Link (MeshComponent) of the Robot
  virtual void CreateLink(FRNode* Node , USceneComponent* RootComponent, TMap<FString, UPrimitiveComponent*> LinkComponents);
  virtual void CreateBoxLink(){};
  virtual void CreateSphereLink(){};
  virtual void CreateCylinderLink(){};
  virtual void CreateCustomLink(){};


};


UCLASS()
class UROBOSIM_API URMeshCollisionOrVisual : public URMeshHandler
{
  GENERATED_BODY()
	public:
	URMeshCollisionOrVisual(){};

  //Create the Link (MeshComponent) of the Robot
  // void CreateLink(FRNode* Node , USceneComponent* Root, TMap<FString, UPrimitiveComponent*> LinkComponents);
  void CreateBoxLink();
  void CreateSphereLink();
  void CreateCylinderLink();
  void CreateCustomLink();

};

UCLASS()
class UROBOSIM_API URMeshCollisionAndVisual : public URMeshCollisionOrVisual
{
  GENERATED_BODY()
	public:
	URMeshCollisionAndVisual(){};
  void CreateMeshType();
  void CreateCustomLink();

};

UCLASS()
class UROBOSIM_API URMeshNoCollisionAndVisual : public URMeshHandler
{
  GENERATED_BODY()
	public:
	URMeshNoCollisionAndVisual(){};

};

UCLASS()
class UROBOSIM_API URMeshFactory : public UObject
{
  GENERATED_BODY()
	public:
	URMeshFactory(){};
  URMeshHandler* CreateMeshHandler(USceneComponent* RootComponent, bool bUseCollision, bool bUseVisual);

};
