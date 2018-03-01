
// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "IURoboSimEd.h"
#include "IURoboSim.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "GameFramework/Actor.h"
#include "RConstraint.h"
#include "RMeshHandler.h"
#include "Structs.h"
#include "RController.h"
#include "RRobot.generated.h"



// Robot that is created from joint and link information. Obtained through urdf.
// TODO add compatibility with virtual links/ robots other than PR2
// TODO add documentation to cpp
UCLASS(Blueprintable)
class UROBOSIM_API ARRobot : public AActor, public IURoboSimEd
{
GENERATED_BODY()
public:
    // All the links that are attached to this Robot. Key is Name of link, Value is the link.

    // Factory for the MeshHandler which creates and initializes the Link/Mesh
    URMeshFactory* MeshFactory;

    // The root component
    UPROPERTY(EditAnywhere)
        URStaticMeshComponent* Root;

    // Contains the Links/Meshs of the robot
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
        TMap<FString, URStaticMeshComponent*> LinkComponents;

    // All the joints that connect the links together. Key is Name of joint, Value is the joint.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
        TMap<FString, UPhysicsConstraintComponent*> JointComponents;

    // Initial Relative Rotation (Quaternion)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
        TMap<FString, FQuat> OriginRotations;

    // Original relative locations of links that are constrained with prismatic type
    TMap<FString, FVector> OriginLocations;

    // List of all Wheel of the Robot
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
        TArray<URStaticMeshComponent*> WheelComponents;

    //List of all caster/links responsible for the orientation of the wheels
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
        TArray<URStaticMeshComponent*> WheelTurnComponents;

    // List of Controller descibtions. Used by Controller component to create the controllers
    UPROPERTY(VisibleAnywhere, Category = "Map")
        TArray<FRControllerDesciption> ControllerDescriptionList;

    float Time = 0.f;

    // Angular velocity of the wheels resulting in a forward motion of the robot
    // TODO move to the controller component
    FRotator WheelTurnSpeed;
    // Angular velocity of the wheels to change the direction of the robot
    // TODO move to the controller component
    FVector WheelSpinnSpeed;

    // Parameter for claculating angular velocity of wheels
    // TODO move to the controller component
    float DistanceWheelCaster = 5.0f;
    // Parameter for claculating angular velocity of wheels ;
    // TODO move to the controller component
    float WheelRadius = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Substepping Parameters")
        bool bSubstepEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Substepping Parameters")
        bool bEnableLogging;

    //URoboSimEd Variables
    TArray<FString> CollisionFilterArr; //holds links on which self-collision should be disabled
    bool bEnableUROSBridge; //holds links on which self-collision should be disabled
    bool bEnableCollisionTags;
    bool bEnableAngularMotors;
    bool bEnableShapeCollisions;

    // The material used for all robot links
    UPROPERTY(EditAnywhere)
        UMaterial* BasicMaterial;

    // Sets default values for this actor's properties
    ARRobot();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    // Adds the Link data to the Robot
    bool AddLink(FRLink Link);

    // Adds the Joint data to the Robot
    bool AddJoint(FRJoint Joint);

    // Creates the Robot using the links and joints added by AddLink and AddJoint
    bool CreateRobot();

    // Create the Constraint/Joints
    URConstraint* CreateConstraint(URMeshHandler* MeshHandler);

    FORCEINLINE void ScreenMsg(const FString& Msg)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg);
    }

    FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2));
    }

private:
    // Array of links added with AddLink. Is cleared in the process of creating the Robot
    TArray<FRLink> Links;

    // Array of joints added with AddJoint. Is cleared in the process of creating the Robot
    TArray<FRJoint> Joints;

    // The BaseNode that holds the topmost link and has no parent or joint
    FRNode* BaseNode = nullptr;

    // Builds a tree with the Elements in the Arrays Links and Joints (breadth-first)
    void BuildTree(FRNode* Node);

    // Recursively creates the Robots physical links and joints
    bool CreateActorsFromNode(FRNode* Node);

};



