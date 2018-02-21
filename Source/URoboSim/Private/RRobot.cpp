// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RRobot.h"
#include "RStaticMeshComponent.h"
#include "RURDFParser.h"
#include "math.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "RControllerComponent.h"
#include "UnrealEd.h"
#include <stdexcept>
#include "Engine/EngineTypes.h"



// Sets default values
ARRobot::ARRobot()
{
    // Init parameters for substepping in RStaticMeshComponent for which this is the parent actor
    StartVelocity = 1000.0f;
    KSpring = 100.0f;
    Damping = 0;

    PrimaryActorTick.bCanEverTick = true;
    CollisionFilterArr = { "torso","wheel_link", "shoulder", "arm", "finger_link", "caster", "base" };

    // OuterWheel = {"fl_caster_l_wheel_link","fr_caster_r_wheel_link", "bl_caster_l_wheel_link", "br_caster_r_wheel_link"};

    // InnerWheel = {"fl_caster_r_wheel_link","fr_caster_l_wheel_link","br_caster_l_wheel_link","bl_caster_r_wheel_link"};

    // AutoPossessPlayer = EAutoReceiveInput::Player0;

    bEnableShapeCollisions = false;

    // Create a URStaticMeshComponent to be the RootComponent
    Root = CreateDefaultSubobject<URStaticMeshComponent>(TEXT("Root"));
    Root->SetupAttachment(this->RootComponent, TEXT("Root"));
    this->SetRootComponent(Root);
    Root->bVisualizeComponent = true;


    MeshFactory = CreateDefaultSubobject<URMeshFactory>(FName(TEXT("MeshFactory")));
}

// Called when the game starts or when spawned
void ARRobot::BeginPlay()
{
    Super::BeginPlay();
    URControllerComponent* ControllerComp = nullptr;
    ControllerComp=FindComponentByClass<URControllerComponent>();
    if(ControllerComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Begin Controller component"));
        ControllerComp->CreateController();
    }

}

// Called every frame (currently not active)
void ARRobot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARRobot::OnConstruction(const FTransform &Transform)
{
    // if(!bAlreadyCreated)
    //     {
    //         ParseURDF();
    //         UE_LOG(LogTemp, Error, TEXT("test %d"), WheelTurnComponents.Num());
    //
    //     }
}

bool ARRobot::AddLink(FRLink Link)
{
    FRLink LinkToAdd(Link);
    Links.Add(LinkToAdd);
    return true;
}

bool ARRobot::AddJoint(FRJoint Joint)
{
    FRJoint JointToAdd(Joint);
    Joints.Add(JointToAdd);
    return true;
}

bool ARRobot::CreateRobot()
{
    if (Links.Num() <= 1)
    {
        return false;
    }

    // Since every Link other than the base link has to be connected to another link the number of joints is fixed
    UE_LOG(LogTemp, Display, TEXT("Number Joints %d"), Joints.Num());
    UE_LOG(LogTemp, Display, TEXT("Number L %d"), Links.Num());
    if (Joints.Num() != Links.Num() - 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mismatch number of links and joints"));
        //return false;
    }

    FRJoint CurrentJoint(Joints[0]);

    // Search for upmost link
    while (true)
    {
        bool FoundHigherElement = false;
        for (int c = 0; c < Joints.Num(); c++)
        {
            if (CurrentJoint.Parent.Equals(Joints[c].Child))
            {
                CurrentJoint = Joints[c];
                FoundHigherElement = true;
            }
        }
        if (!FoundHigherElement) break;
    }

    // If there already is a tree free its memory and continue
    if (BaseNode)
    {
        delete BaseNode;
    }

    // Setup the BaseNode
    BaseNode = new FRNode();
    BaseNode->Parent = nullptr;

    for (int c = 0; c < Links.Num(); c++)
    {
        if (Links[c] == CurrentJoint.Parent)
        {
            BaseNode->Link = Links[c];
            Links.RemoveAt(c);
        }
    }

    BuildTree(BaseNode);

    bool bSuccess = CreateActorsFromNode(BaseNode);



    return bSuccess;
}


bool ARRobot::CreateActorsFromNode(FRNode* Node)
{
    if (!Node)
    {
        return false;
    }

    URMeshHandler* MeshHandler = MeshFactory->CreateMeshHandler(this, Node);
    if(MeshHandler)
    {
        if (MeshHandler->CreateLink())
        {




            if (MeshHandler->IsNotRoot)
            {
                URConstraint* Constraint = CreateConstraint(MeshHandler);
                //	  URConstraint* Constraint = ConstraintFactory.MakeConstraint(ParentComp, Joint, Link);
                Constraint->InitDrive();
                Constraint->SetupConstraint();

                FRotator ParentRotation = MeshHandler->ParentComp->GetComponentRotation();
                FRotator ChildRotation = MeshHandler->ShapeComp ? (MeshHandler->ShapeComp->GetComponentRotation()) : (MeshHandler->MeshComp->GetComponentRotation());
                FQuat InitialRotationRel = FQuat(ParentRotation).Inverse() * FQuat(ChildRotation);

                OriginRotations.Add(MeshHandler->Joint->Name, InitialRotationRel);
                JointComponents.Add(MeshHandler->Joint->Name, Constraint);

                MeshHandler->AddConnectedJoint();

                if(MeshHandler->Link->Name.Contains("wheel_link"))
                {
                    WheelComponents.Add(MeshHandler->MeshComp);
                }
                if(MeshHandler->Link->Name.Contains("caster_rotation_link"))
                {
                    WheelTurnComponents.Add(MeshHandler->MeshComp);
                }
            }

            LinkComponents.Add(MeshHandler->Link->Name, MeshHandler->MeshComp);
        }
    }
    // Create children
    for (int c = 0; c < Node->Children.Num(); c++)
    {
        CreateActorsFromNode(Node->Children[c]);
    }


    return true;
}

void ARRobot::BuildTree(FRNode* Node)
{
    if (!Node)
    {
        return;
    }
    // Go through all joints to find the ones that use the current Node as parent
    for (int c = 0; c < Joints.Num(); c++)
    {
        bool bHasFoundMatch = false;
        if (Joints[c].Parent.Equals(Node->Link.Name))
        {
            // Find the child link which parent is the current node
            for (int i = 0; i < Links.Num(); i++)
            {
                if (Links[i].Name.Equals(Joints[c].Child))
                {
                    FRNode* ChildNode = new FRNode();
                    ChildNode->Parent = Node;
                    ChildNode->Joint = Joints[c];
                    ChildNode->Link = Links[i];
                    Node->Children.Add(ChildNode);

                    Joints.RemoveAt(c);
                    Links.RemoveAt(i);
                    bHasFoundMatch = true;
                    break;
                }
            }
        }
        // Decrement the joint counter as the array automatically shrinks on removal
        if (bHasFoundMatch) c--;
    }

    for (int c = 0; c < Node->Children.Num(); c++)
    {
        BuildTree(Node->Children[c]);
    }
}


bool ARRobot::RotateJoint(FString Name, FRotator TargetRotation)
{
    // UE_LOG(LogTemp, Display, TEXT("RotateJoint %s"), *Name);
    // TArray<UActorComponent*> Children = GetComponents().Array();
    // for (int c = 0; c < Children.Num(); c++)
    // {
    // 	UActorComponent* SceneComp = Children[c];
    // 	if (SceneComp->GetName().Equals(Name, ESearchCase::IgnoreCase))
    // 	{
    // 		UPhysicsConstraintComponent* Constraint = (UPhysicsConstraintComponent*)SceneComp;

    // 		FConstraintInstance* ConstraintInstance = &(Constraint->ConstraintInstance);
    // 		Constraint->SetAngularOrientationTarget(TargetRotation);
    // 		return true;
    // 	}

    // }


    return false;
}

bool ARRobot::MovePrismaticJoint(FString Name, FVector TargetPosition)
{
    // UE_LOG(LogTemp, Display, TEXT("MovePrismaticJoint %s"), *Name);
    // TArray<UActorComponent*> Children = GetComponents().Array();
    // for (int c = 0; c < Children.Num(); c++)
    // {
    // 	UActorComponent* ActorComp = Children[c];
    // 	if (ActorComp->GetName().Equals(Name, ESearchCase::IgnoreCase))
    // 	{
    // 		UPhysicsConstraintComponent* Constraint = (UPhysicsConstraintComponent*)ActorComp;

    // 		FConstraintInstance* ConstraintInstance = &(Constraint->ConstraintInstance);
    // 		ConstraintInstance->SetLinearPositionTarget(TargetPosition);

    // 		return true;
    // 	}

    // }

    return true;
}

// Get Joint Position in *Degrees*
float ARRobot::GetJointPosition(FString JointName)
{
    // UPhysicsConstraintComponent* Joint = JointComponents[JointName];

    // FString ParentCompName = Joint->ComponentName1.ComponentName.ToString();
    // FString ChildCompName = Joint->ComponentName2.ComponentName.ToString();
    // UPrimitiveComponent* ParentComponent = LinkComponents[ParentCompName];
    // UPrimitiveComponent* ChildComponent = LinkComponents[ChildCompName];
    // if (ParentComponent && ChildComponent)
    // {
    //     FRotator ParentRotation = ParentComponent->GetComponentRotation();
    //     FRotator ChildRotation = ChildComponent->GetComponentRotation();
    //     FQuat CurrentRotationRel = FQuat(ParentRotation).Inverse() * FQuat(ChildRotation);
    //     FQuat InitialRotationRel = OriginRotations[Joint->GetName()];
    //     FQuat QRel = CurrentRotationRel * InitialRotationRel.Inverse();
    //     FVector Axis; float Angle;
    //     QRel.ToAxisAndAngle(Axis, Angle);

    //     // Get Axis
    //     auto MotionSwing1 = Joint->ConstraintInstance.GetAngularSwing1Motion();
    //     auto MotionSwing2 = Joint->ConstraintInstance.GetAngularSwing2Motion();
    //     auto MotionTwist = Joint->ConstraintInstance.GetAngularTwistMotion();
    //     bool rotationX = false, rotationY = false, rotationZ = false;
    //     if (MotionSwing1 == EAngularConstraintMotion::ACM_Free || MotionSwing1 == EAngularConstraintMotion::ACM_Limited)
    //         rotationZ = true;
    //     if (MotionSwing2 == EAngularConstraintMotion::ACM_Free || MotionSwing2 == EAngularConstraintMotion::ACM_Limited)
    //         rotationY = true;
    //     if (MotionTwist == EAngularConstraintMotion::ACM_Free || MotionTwist == EAngularConstraintMotion::ACM_Limited)
    //         rotationX = true;

    //     if ((!rotationX && !rotationY && !rotationZ) || (rotationX && rotationY) || (rotationX && rotationZ) || (rotationY && rotationZ))
    //     {
    //         // not a hinged joint
    //         // UE_LOG(LogTemp, Error, TEXT("Joint [%s] is not a hinged joint with DOF=1"), *Joint->GetName());
    //         return 0;
    //     }
    //     else
    //     {
    //         // a hinged joint
    //         FVector RefAxis;
    //         if (rotationX) RefAxis = FVector(1, 0, 0);
    //         if (rotationY) RefAxis = FVector(0, 1, 0);
    //         if (rotationZ) RefAxis = FVector(0, 0, 1);

    //         float ResultAngle = FVector::DotProduct(Axis.GetSafeNormal(), RefAxis) * FMath::RadiansToDegrees(Angle);
    //         while (ResultAngle > 180)
    //             ResultAngle -= 360;
    //         while (ResultAngle < -180 )
    //             ResultAngle += 360;



    //         return ResultAngle;
    //     }
    // }
    // else
    // {
    //     UE_LOG(LogTemp, Error, TEXT("Joint [%s] doesn't have parent or child."), *Joint->GetName());
    //     return 0.0;
    // }
    return 0.0;
}

// Get Joint Velocity in Deg/s
float ARRobot::GetJointVelocity(FString JointName)
{
    // UPhysicsConstraintComponent* Joint = JointComponents[JointName];

    // FString ParentCompName = Joint->ComponentName1.ComponentName.ToString();
    // FString ChildCompName = Joint->ComponentName2.ComponentName.ToString();
    // UPrimitiveComponent* ParentComponent = LinkComponents[ParentCompName];
    // UPrimitiveComponent* ChildComponent = LinkComponents[ChildCompName];

    // if (ParentComponent && ChildComponent)
    // {
    //     // Get Rotation Axis
    //     auto MotionSwing1 = Joint->ConstraintInstance.GetAngularSwing1Motion();
    //     auto MotionSwing2 = Joint->ConstraintInstance.GetAngularSwing2Motion();
    //     auto MotionTwist = Joint->ConstraintInstance.GetAngularTwistMotion();
    //     bool rotationX = false, rotationY = false, rotationZ = false;
    //     if (MotionSwing1 == EAngularConstraintMotion::ACM_Free || MotionSwing1 == EAngularConstraintMotion::ACM_Limited)
    //         rotationZ = true;
    //     if (MotionSwing2 == EAngularConstraintMotion::ACM_Free || MotionSwing2 == EAngularConstraintMotion::ACM_Limited)
    //         rotationY = true;
    //     if (MotionTwist == EAngularConstraintMotion::ACM_Free || MotionTwist == EAngularConstraintMotion::ACM_Limited)
    //         rotationX = true;

    //     if ((!rotationX && !rotationY && !rotationZ) || (rotationX && rotationY) || (rotationX && rotationZ) || (rotationY && rotationZ))
    //     {
    //         // not a hinged joint
    //         UE_LOG(LogTemp, Error, TEXT("Joint [%s] is not a hinged joint with DOF=1"), *Joint->GetName());
    //         return 0.0;
    //     }
    //     else
    //     {
    //         // a hinged joint
    //         FVector RefAxis;
    //         if (rotationX) RefAxis = FVector(1, 0, 0);
    //         if (rotationY) RefAxis = FVector(0, 1, 0);
    //         if (rotationZ) RefAxis = FVector(0, 0, 1);

    //         // Get Axis ?
    //         FQuat JointQuat = Joint->GetComponentTransform().GetRotation();
    //         FVector GlobalAxis = JointQuat.RotateVector(RefAxis); // Rotation Axis in Global Frame

    //         FVector ParentAvel = ParentComponent->GetPhysicsAngularVelocityInRadians();
    //         FVector ChildAvel = ChildComponent->GetPhysicsAngularVelocityInRadians();
    //         float HingeVel = FVector::DotProduct(ChildAvel - ParentAvel, GlobalAxis);

    //         /* UE_LOG(LogTemp, Warning, TEXT("Joint [%s]: HingeVel = %.3f; ChildAvel [%s], ParentAvel [%s], Axis [%s] (global)"),
    //             *Joint->GetName(), HingeVel, *ChildAvel.ToString(), *ParentAvel.ToString(), *GlobalAxis.ToString()); */

    //         return HingeVel;
    //     }
    // }
    // else
    // {
    //     UE_LOG(LogTemp, Error, TEXT("Joint [%s] doesn't have parent or child."), *Joint->GetName());
    //     return 0.0;
    // }
    return 0.0;
}

void ARRobot::AddForceToJoint(FString JointName, float Force)
{
    // if (!JointComponents.Find(JointName))
    // {
    //     UE_LOG(LogTemp, Error, TEXT("Unable to find Joint [%s]!"), *JointName);
    //     return;
    // }

    // UPhysicsConstraintComponent* Joint = JointComponents[JointName];

    // FString ParentCompName = Joint->ComponentName1.ComponentName.ToString();
    // FString ChildCompName = Joint->ComponentName2.ComponentName.ToString();
    // UPrimitiveComponent* ParentComponent = LinkComponents[ParentCompName];
    // UPrimitiveComponent* ChildComponent = LinkComponents[ChildCompName];

    // if (ParentComponent && ChildComponent)
    // {
    //     // Get Rotation Axis
    //     auto MotionSwing1 = Joint->ConstraintInstance.GetAngularSwing1Motion();
    //     auto MotionSwing2 = Joint->ConstraintInstance.GetAngularSwing2Motion();
    //     auto MotionTwist = Joint->ConstraintInstance.GetAngularTwistMotion();
    //     bool rotationX = false, rotationY = false, rotationZ = false;
    //     if (MotionSwing1 == EAngularConstraintMotion::ACM_Free || MotionSwing1 == EAngularConstraintMotion::ACM_Limited)
    //         rotationZ = true;
    //     if (MotionSwing2 == EAngularConstraintMotion::ACM_Free || MotionSwing2 == EAngularConstraintMotion::ACM_Limited)
    //         rotationY = true;
    //     if (MotionTwist == EAngularConstraintMotion::ACM_Free || MotionTwist == EAngularConstraintMotion::ACM_Limited)
    //         rotationX = true;

    //     if ((!rotationX && !rotationY && !rotationZ) || (rotationX && rotationY) || (rotationX && rotationZ) || (rotationY && rotationZ))
    //     {
    //         // not a hinged joint
    //         // UE_LOG(LogTemp, Error, TEXT("Joint [%s] is not a hinged joint with DOF=1"), *Joint->GetName());
    //     }
    //     else
    //     {
    //         // a hinged joint
    //         FVector RefAxis;
    //         if (rotationX) RefAxis = FVector(1, 0, 0);
    //         if (rotationY) RefAxis = FVector(0, 1, 0);
    //         if (rotationZ) RefAxis = FVector(0, 0, 1);

    //         // Get Axis ?
    //         FQuat JointQuat = Joint->GetComponentTransform().GetRotation();
    //         FVector GlobalAxis = JointQuat.RotateVector(RefAxis); // Rotation Axis in Global Frame

    //         FVector TorqueToAdd = GlobalAxis * Force;
    //         ChildComponent->AddTorqueInRadians(TorqueToAdd);
    //         ParentComponent->AddTorqueInRadians(-TorqueToAdd);
    //     }
    // }
}




URConstraint* ARRobot::CreateConstraint(URMeshHandler* MeshHandler)
{
    URConstraint* Constraint;
    if (MeshHandler->Joint->Type.Equals("fixed", ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URFixedConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("floating", ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URFloatingConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("prismatic", ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URPrismaticConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("revolute", ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URRevoluteConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("planar",ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URPlanarConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else if (MeshHandler->Joint->Type.Equals("continuous",ESearchCase::IgnoreCase))
    {
        Constraint = NewObject<URContinuousConstraint>(MeshHandler->ParentComp, FName(MeshHandler->Joint->Name.GetCharArray().GetData()));
    }
    else
    {
        UE_LOG(LogTemp, Fatal, TEXT("Not a supported Constraint Type"));
        Constraint = nullptr;
    }
    if(Constraint)
    {
        Constraint->Init(MeshHandler);
    }
    return Constraint;
}

