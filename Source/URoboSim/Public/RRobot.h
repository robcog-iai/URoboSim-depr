// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "IURoboSimEd.h"
#include "IURoboSim.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "RRobot.generated.h"

UCLASS(Blueprintable)
class UROBOSIM_API ARRobot : public AActor, public IURoboSimEd
{
	GENERATED_BODY()

public:
	// All the links that are attached to this Robot. Key is Name of link, Value is the link.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
	TMap<FString, UPrimitiveComponent*> LinkComponents;

	// All the joints that connect the links together. Key is Name of joint, Value is the joint.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
	TMap<FString, UPhysicsConstraintComponent*> JointComponents;

    // Initial Relative Rotation (Quaternion) 
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map")
    TMap<FString, FQuat> OriginRotations; 

	// Original relative locations of links that are constrained with prismatic type
	TMap<FString, FVector> OriginLocations;

	UStaticMesh* CylinderMesh;
	UStaticMesh* CubeMesh;
	UStaticMesh* SphereMesh;
	float Time = 0.f;
	bool bMotorSet = false;
	FConstraintInstance NewConstraintInstanceFixed();

	//TEMPLATE Load Obj From Path
	template <typename ObjClass>
	FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path);

	// Load Static Mesh From Path 
	FORCEINLINE UStaticMesh* LoadMeshFromPath(const FName& Path);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Substepping Parameters")
	bool bSubstepEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Substepping Parameters")
	float StartVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubsteppingParameters")
	float KSpring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Substepping Parameters")
	float Damping;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Substepping Parameters")
	bool bEnableLogging;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS Bridge") - 
	//bool bEnableROSBridge;

	//URoboSimEd Variables
	TArray<FString> collisionFilterArr; //holds links on which self-collision should be disabled
	bool bEnableUROSBridge; //holds links on which self-collision should be disabled
	bool bEnableCollisionTags;
	bool bEnableAngularMotors;
	bool bEnableShapeCollisions;
	
	// A structure representing a URDF Joint
	struct FRJoint
	{
		FString Name;
		FString Type;

		FVector Location;
		FRotator Rotation;

		FString Parent;
		FString Child;

		FVector Axis;		

		float LowerLimit;
		float UpperLimit;
		float Effort;
		float Velocity;

		//safety_controller
		float k_velocity;
		//dynamics
		float damping;
		float friction;

		bool operator== (const FRJoint &Joint)
		{
			return Name.Equals(Joint.Name);
		}

		bool operator== (const FString &String)
		{
			return Name.Equals(String);
		}
	};

	// A structure representing a URDF Link
	struct FRLink
	{
		FString Name;

		struct
		{
			FVector Location;
			FRotator Rotation;
			float Mass;
		} Inertial;

		struct
		{
			FVector Location;
			FRotator Rotation;
			FString Mesh;
			FVector Scale;

			// Optional. The material of the visual element.
			struct
			{
				FString Name;
				FColor Color;
				FString Texture;
			} Material;
		} Visual;

		struct
		{
			FVector Location;
			FRotator Rotation;
			FString Mesh;
			FVector Scale;
		} Collision;

		bool operator== (const FRLink &Link)
		{
			return Name.Equals(Link.Name);
		}
		bool operator== (const FString &String)
		{
			return Name.Equals(String);
		}
	};

	// Sets default values for this actor's properties
	ARRobot();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called when the Robot is constructed
	virtual void OnConstruction(const FTransform &Transform) override;

	// The root component
	USceneComponent* Root;

	// Adds the Link data to the Robot
	bool AddLink(FRLink Link);

	// Adds the Joint data to the Robot
	bool AddJoint(FRJoint Joint);

	// Creates the Robot using the links and joints added by AddLink and AddJoint
	bool CreateRobot();

	// Parses the URDF code written into property XmlUrdf
	void ParseURDF();


	UPROPERTY(EditAnywhere, Export)
	float GlobalVarA;

	// Copy the XML URDF Code
	UPROPERTY(EditAnywhere, Export)
	FString XmlUrdf;

	// The material used for all robot links
	UPROPERTY(EditAnywhere)
	UMaterial* BasicMaterial;

	// Rotates the joint to the targeted rotation
	bool RotateJoint(FString Name, FRotator TargetRotation);

	// Moves the prismatic joint to the targeted position
	bool MovePrismaticJoint(FString Name, FVector TargetTranslation);

    // Get Joint Position in *Degrees*
    UFUNCTION(BlueprintCallable, Category="Robot")
    float GetJointPosition(FString JointName);

    // Get Joint Velocity in Deg/s
    UFUNCTION(BlueprintCallable, Category="Robot")
    float GetJointVelocity(FString JointName);

    // Add force / torque to given Joint
    UFUNCTION(BlueprintCallable, Category="Robot")
    void AddForceToJoint(FString JointName, float Force);

private:
	// Array of links added with AddLink. Is cleared in the process of creating the Robot
	TArray<FRLink> Links;

	// Array of joints added with AddJoint. Is cleared in the process of creating the Robot
	TArray<FRJoint> Joints;

	// Node to represent the Robot virtually as a tree
	struct FRNode
	{
		FRLink Link;
		FRJoint Joint;

		FRNode* Parent;
		TArray<FRNode*> Children;

		~FRNode()
		{
			for (int c = 0; c < Children.Num(); c++)
			{
				delete Children[c];
			}
		}
	};

	// The BaseNode that holds the topmost link and has no parent or joint
	FRNode* BaseNode = nullptr;

	// Configures the constraint using the given Joint
	FConstraintInstance SetConstraint(FRJoint* Joint);

	// Builds a tree with the Elements in the Arrays Links and Joints (breadth-first)
	void BuildTree(FRNode* Node);

	// Recursively creates the Robots physical links and joints
	bool CreateActorsFromNode(FRNode* Node);
};
