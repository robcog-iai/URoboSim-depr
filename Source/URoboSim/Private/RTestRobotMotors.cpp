// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RTestRobotMotors.h"
#include "IURoboSim.h"

ARTestRobotMotors::ARTestRobotMotors() : ARRobot()
{
	// Test robot to test the motors
	XmlUrdf = "<robot name=\"test\">"
		"<link name=\"first_link\"><inertial><mass value=\"100\"/></inertial><visual><geometry><box size=\"3.0 1.5 0.5\"/></geometry><material name=\"blue\"><color rgba=\"0 0 .8 1\"/></material></visual></link>"
		"<link name=\"second_link\"><inertial><mass value=\"50\"/></inertial><visual><geometry><box size=\"1 1 1\"/></geometry><material name=\"blue\"><color rgba=\"0 0 .8 1\"/></material></visual></link>"
		"<joint name=\"first_to_second_joint\" type=\"prismatic\"><parent link=\"first_link\"/><child link=\"second_link\"/>"
		"<origin xyz=\"-1 0 0.5\" rpy=\"0 0 0\"/><axis xyz=\"1 0 0\"/><limit lower=\"0\" upper=\"2\" effort=\"500\" velocity=\"10\"/></joint>"
		"<link name=\"third_link\"><inertial><mass value=\"25\"/></inertial><visual><geometry><box size=\"0.5 0.5 0.5\"/></geometry><material name=\"blue\"><color rgba=\"0 0 .8 1\"/></material></visual></link>"
		"<joint name=\"second_to_third_joint\" type=\"continuous\"><parent link=\"second_link\"/><child link=\"third_link\"/><origin xyz=\"0 0 0.5\" rpy=\"0 0 0\"/><axis xyz=\"0 0 1\"/></joint>"
		//"<link name=\""
		"</robot>";
	TargetRotation = FRotator(0.f, 45.f, 0.f);
	TargetLocation = FVector(50.f, 0.f, 0.f);
}

// Called when the game starts or when spawned
void ARTestRobotMotors::BeginPlay()
{
	Super::BeginPlay();
	RotateJoint(FString("second_to_third_joint"), TargetRotation);
	MovePrismaticJoint(FString("first_to_second_joint"), TargetLocation);
}

void ARTestRobotMotors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

