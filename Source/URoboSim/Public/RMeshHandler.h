// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "IURoboSim.h"
#include "UnrealEd.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Structs.h"
#include "RConstraint.h"
#include "RStaticMeshComponent.h"
#include "RMeshHandler.generated.h"

//TODO move controller description list to function.
//TODO Rework generation to be more generl

class URMeshFactory;

UCLASS()
class UROBOSIM_API URMeshHandler : public UObject
{
    GENERATED_BODY()
public:

    bool bIsNotRoot;
    bool bWriteParentTFTag = true;

    ARRobot *Owner;
    FRLink* Link;
    FRJoint* Joint;

    //Visual Component of the link
    UStaticMesh* Mesh;
    //Link component wich is attached to the robot
    URStaticMeshComponent* MeshComp;
    //Parent Link
    URStaticMeshComponent* ParentComp;
    FVector Scale;
    FVector LocationCollision;
    FVector LocationVisual;
    FVector LinkOriginLocation;

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
    TArray<FString> GravityEnableTags = { "all"};
    URMeshHandler();
    ~URMeshHandler()
    {
    };

    //TEMPLATE Load Obj From Path
    template <typename ObjClass>
        FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path);

    // Load Static Mesh From Path
    FORCEINLINE UStaticMesh* LoadMeshFromPath(const FName& Path);

    //Create the Link (MeshComponent) of the Robot
    virtual bool CreateLink();
    // creates the mesh/visual part of the link
    virtual void CreateMesh(){};
    // creates the link which will be attached to the robot
    virtual void CreateMeshComponent();
    // Configures the Link incluing the visual mesh, location of collisions...
    virtual void ConfigureMeshComponent();
    // configures the physics behaviour of the link
    virtual void ConfigureLinkPhysics();

    virtual void AddConnectedJoint();

    virtual FRConnectedJoint CreateConnectedJoint(bool IsParent);

    // Configures the position of link and joint
    virtual void PositionLinkAndJoint();

protected:

    friend class URConstraint;
    friend class URMeshFactory;

    FRNode* Node;
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
class UROBOSIM_API URMeshHandlerFoundation : public URMeshHandler
{
    GENERATED_BODY()
public:
    URMeshHandlerFoundation(){};
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
    URMeshHandler* CreateMeshHandler(ARRobot* Owner, FRNode* Node);
};
