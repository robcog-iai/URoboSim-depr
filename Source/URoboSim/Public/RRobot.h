
// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once


#include "IURoboSim.h"
// #include "IURoboSimEd.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "GameFramework/Actor.h"
#include "RConstraint.h"
#include "RMeshHandler.h"
#include "Structs.h"
#include "RController.h"
#include "RRobot.generated.h"



// A structure representing a URDF Joint

UCLASS(Blueprintable)
class UROBOSIM_API ARRobot : public AActor//, public IURoboSimEd
{
GENERATED_BODY()
public:
    // All the links that are attached to this Robot. Key is Name of link, Value is the link.


    URMeshFactory* MeshFactory;

    // The root component
    UPROPERTY(EditAnywhere)
        URStaticMeshComponent* Root;

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

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
        TArray<URStaticMeshComponent*> WheelComponents;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
        TArray<URStaticMeshComponent*> WheelTurnComponents;

    // UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
    //     TMap<FString,URController*> ControllerList;

    UPROPERTY(VisibleAnywhere, Category = "Map")
        TArray<FRControllerDesciption> ControllerDescriptionList;

    float Time = 0.f;

    FRotator WheelTurnSpeed;
    FVector WheelSpinnSpeed;
    float DistanceWheelCaster = 5.0f;
    float WheelRadius = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Substepping Parameters")
        bool bSubstepEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Substepping Parameters")
        bool bEnableLogging;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drive Parameter")
        float AngularVelocityOrTorque = 100000000.0f;

    TArray<FString> OuterWheel;
    TArray<FString> InnerWheel;

    //URoboSimEd Variables
    TArray<FString> CollisionFilterArr; //holds links on which self-collision should be disabled
    bool bEnableUROSBridge; //holds links on which self-collision should be disabled
    bool bEnableCollisionTags;
    bool bEnableAngularMotors;

    bool bEnableShapeCollisions;




    // Sets default values for this actor's properties
    ARRobot();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    // Called when the Robot is constructed
    virtual void OnConstruction(const FTransform &Transform) override;


    // Adds the Link data to the Robot
    bool AddLink(FRLink Link);

    // Adds the Joint data to the Robot
    bool AddJoint(FRJoint Joint);

    // Creates the Robot using the links and joints added by AddLink and AddJoint
    bool CreateRobot();

    // Create the Constraint
    URConstraint* CreateConstraint(URMeshHandler* MeshHandler);

    UPROPERTY(EditAnywhere, Export)
        float GlobalVarA;


    // The material used for all robot links
    UPROPERTY(EditAnywhere)
        UMaterial* BasicMaterial;

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



