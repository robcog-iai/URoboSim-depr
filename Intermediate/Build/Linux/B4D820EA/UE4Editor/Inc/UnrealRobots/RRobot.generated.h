// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef UNREALROBOTS_RRobot_generated_h
#error "RRobot.generated.h already included, missing '#pragma once' in RRobot.h"
#endif
#define UNREALROBOTS_RRobot_generated_h

#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execAddForceToJoint) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_JointName); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Force); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->AddForceToJoint(Z_Param_JointName,Z_Param_Force); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetJointVelocity) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_JointName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=this->GetJointVelocity(Z_Param_JointName); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetJointPosition) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_JointName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=this->GetJointPosition(Z_Param_JointName); \
		P_NATIVE_END; \
	}


#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execAddForceToJoint) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_JointName); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Force); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->AddForceToJoint(Z_Param_JointName,Z_Param_Force); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetJointVelocity) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_JointName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=this->GetJointVelocity(Z_Param_JointName); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execGetJointPosition) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_JointName); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(float*)Z_Param__Result=this->GetJointPosition(Z_Param_JointName); \
		P_NATIVE_END; \
	}


#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesARRobot(); \
	friend UNREALROBOTS_API class UClass* Z_Construct_UClass_ARRobot(); \
public: \
	DECLARE_CLASS(ARRobot, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/UnrealRobots"), NO_API) \
	DECLARE_SERIALIZER(ARRobot) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_INCLASS \
private: \
	static void StaticRegisterNativesARRobot(); \
	friend UNREALROBOTS_API class UClass* Z_Construct_UClass_ARRobot(); \
public: \
	DECLARE_CLASS(ARRobot, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/UnrealRobots"), NO_API) \
	DECLARE_SERIALIZER(ARRobot) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ARRobot(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ARRobot) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARRobot); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARRobot); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ARRobot(ARRobot&&); \
	NO_API ARRobot(const ARRobot&); \
public:


#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ARRobot(ARRobot&&); \
	NO_API ARRobot(const ARRobot&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ARRobot); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ARRobot); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ARRobot)


#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_PRIVATE_PROPERTY_OFFSET
#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_9_PROLOG
#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_PRIVATE_PROPERTY_OFFSET \
	UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_RPC_WRAPPERS \
	UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_INCLASS \
	UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_PRIVATE_PROPERTY_OFFSET \
	UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_INCLASS_NO_PURE_DECLS \
	UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID UnrealRobotsP3_Plugins_UnrealRobots_Source_UnrealRobots_Public_RRobot_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
