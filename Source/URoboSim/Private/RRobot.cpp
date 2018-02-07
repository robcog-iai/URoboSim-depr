// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RRobot.h"
#include "RStaticMeshComponent.h"
#include "RURDFParser.h"
#include "math.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
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

    AutoPossessPlayer = EAutoReceiveInput::Player0;
    PrimaryActorTick.bCanEverTick = true;
	collisionFilterArr = { "torso","wheel_link", "shoulder", "arm", "finger_link", "caster", "base" };

    OuterWheel = {"fl_caster_l_wheel_link","fr_caster_r_wheel_link", "bl_caster_l_wheel_link", "br_caster_r_wheel_link"};

    InnerWheel = {"fl_caster_r_wheel_link","fr_caster_l_wheel_link","br_caster_l_wheel_link","bl_caster_r_wheel_link"};


	bEnableShapeCollisions = false;

	// Create a URStaticMeshComponent to be the RootComponent
	Root = CreateDefaultSubobject<URStaticMeshComponent>(TEXT("DefaultSceneComponent"));
	Root->SetupAttachment(this->RootComponent, TEXT("DefaultSceneComponent"));
	this->SetRootComponent(Root);
	Root->bVisualizeComponent = true;


     MeshFactory = NewObject<URMeshFactory>(Root, FName(TEXT("MeshFactory")));
}

// Called when the game starts or when spawned
void ARRobot::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame (currently not active)
void ARRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // TArray<URStaticMeshComponent*>& Wheels = owner->WheelTurnComponents;

    // for(int i = 1; i < Wheels.Num(); i++)
    //     {


    //     }
    // FQuat OrientationWheel = WheelTurnComponents[0]->GetComponentTransform().GetRotation();
    // FRotator Speed(WheelTurnSpeed * DeltaTime);
    // ScreenMsg("Speed", Speed.ToString());
    // for(auto& wheel : WheelTurnComponents)
    //     {
    //         OrientationWheel = OrientationWheel* Speed.Quaternion();
    //         wheel->SetWorldRotation(OrientationWheel);
    //         wheel->TargetOrientation = OrientationWheel;
    //     }
}

void ARRobot::MoveForward(float AxisValue)
{
    WheelSpinnSpeed = FVector(0.0f, AxisValue, 0.0f) * 1000;
}

void ARRobot::TurnWheels(float AxisValue)
{
    WheelTurnSpeed = FRotator(0.0f,AxisValue, 0.0f ) * 80;
}



void ARRobot::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);
    InputComponent->BindAxis("MoveForward", this, &ARRobot::MoveForward);
    InputComponent->BindAxis("TurnWheels", this, &ARRobot::TurnWheels);
}


void ARRobot::OnConstruction(const FTransform &Transform)
{
	ParseURDF();
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
		UE_LOG(LogTemp, Warning, TEXT("Robot is invalid. Abort creating.\n"));
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

    URMeshHandler* MeshHandler = MeshFactory->CreateMeshHandler(Root, Node);
    if(MeshHandler)
        {
            if (MeshHandler->CreateLink(Root, LinkComponents, OriginLocations,Node))
                {
                    if (MeshHandler->ParentLink)
                        {

                            URConstraint* Constraint = CreateConstraint(MeshHandler->ParentComp, MeshHandler->Joint, MeshHandler->Link);
                            //	  URConstraint* Constraint = ConstraintFactory.MakeConstraint(ParentComp, Joint, Link);
                            Constraint->InitDrive();
                            Constraint->SetDisableCollision(true);
                            Constraint->AttachToComponent(MeshHandler->ParentComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

                            Constraint->RegisterComponent();
                            Constraint->ApplyWorldOffset(FVector(0), false);

                            MeshHandler->ConnectPositionLink();

                            Constraint->SetRelativeRotation(MeshHandler->Joint->Rotation);

                            // Connect joint to parent and child
                            Constraint->ConstraintActor1 = this;
                            Constraint->ConstraintActor2 = this;
                            if (MeshHandler->ShapeComp)
                                {
                                    Constraint->SetWorldLocation(MeshHandler->ShapeComp->GetComponentLocation());
                                    Constraint->SetConstrainedComponents(MeshHandler->ParentLink, NAME_None, MeshHandler->ShapeComp, NAME_None);
                                }
                            else
                                {
                                    Constraint->SetWorldLocation(MeshHandler->MeshComp->GetComponentLocation());
                                    Constraint->SetConstrainedComponents(MeshHandler->ParentLink, NAME_None, MeshHandler->MeshComp, NAME_None);
                                }

                            FRotator ParentRotation = MeshHandler->ParentLink->GetComponentRotation();
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

FConstraintInstance ARRobot::NewConstraintInstanceFixed()
{
	// Set the constraint limits
	FConstraintInstance ConstraintInstance;
	ConstraintInstance.SetDisableCollision(true);
	ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0);
	ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0);
	ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0);
	ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	ConstraintInstance.AngularRotationOffset = FRotator(0, 0, 0);

	return ConstraintInstance;
}

void ARRobot::ParseURDF()
{
	// Callback Object for parser
	FRURDFParser Call(this);

	// Error and description Message
	FText Error = NSLOCTEXT("RFastXmlCallbackRobotError", "FFastXmlError", "Error in the execution of the XML-Parser");

	// Default Line for OutErrorLineNumber
	int32 LineNumb = -1;

	// indicates if the robot could be created or not
	bool Success;

	// Remove linebreaks and tabs
	//XmlUrdf = XmlUrdf.Trim().TrimTrailing();
	XmlUrdf.TrimStartAndEndInline();
	XmlUrdf = XmlUrdf.Replace(L"\n", L" ");
	XmlUrdf = XmlUrdf.Replace(L"\r", L"");
	XmlUrdf = XmlUrdf.Replace(L"\t", L" ");
	
	if (XmlUrdf.IsEmpty()) return;

	// Create a copy of the URDF since the parser modifies the input string
	FString StringToParse = XmlUrdf;

	Success = FFastXml::ParseXmlFile((IFastXmlCallback*)&Call, TEXT(""), StringToParse.GetCharArray().GetData(), nullptr, false, false, Error, LineNumb);

	CreateRobot();

	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create Robot\n"));
	}
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




URConstraint* ARRobot::CreateConstraint(URStaticMeshComponent* ParentComp, FRJoint* Joint, FRLink* Link)
{
  URConstraint* Constraint; 
	if (Joint->Type.Equals("fixed", ESearchCase::IgnoreCase))
	  {
		Constraint = NewObject<URFixedConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  }
	else if (Joint->Type.Equals("floating", ESearchCase::IgnoreCase))
	  {
		Constraint = NewObject<URFloatingConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  }
	else if (Joint->Type.Equals("prismatic", ESearchCase::IgnoreCase))
	  {
		Constraint = NewObject<URPrismaticConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  }
	else if (Joint->Type.Equals("revolute", ESearchCase::IgnoreCase))
	  {
		Constraint = NewObject<URRevoluteConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  }
	else if (Joint->Type.Equals("planar",ESearchCase::IgnoreCase))
	  {
		Constraint = NewObject<URPlanarConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  }
	else if (Joint->Type.Equals("continuous",ESearchCase::IgnoreCase))
	  {
		Constraint = NewObject<URContinuousConstraint>(ParentComp, FName(Joint->Name.GetCharArray().GetData()));
	  }
	else
	  {
		UE_LOG(LogTemp, Fatal, TEXT("Not a supported Constraint Type"));
		Constraint = nullptr;
	  }
	if(Constraint)
	  {
		Constraint->Init(ParentComp, Joint, Link);
	  }
	return Constraint;
}

