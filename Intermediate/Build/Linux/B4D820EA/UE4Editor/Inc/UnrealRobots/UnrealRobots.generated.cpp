// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/IUnrealRobots.h"
#include "UnrealRobots.generated.dep.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCode1UnrealRobots() {}
#if USE_COMPILED_IN_NATIVES
// Cross Module References
	UNREALED_API class UClass* Z_Construct_UClass_UActorFactory();
	COREUOBJECT_API class UClass* Z_Construct_UClass_UObject();
	ENGINE_API class UClass* Z_Construct_UClass_UAssetImportData_NoRegister();
	UNREALED_API class UClass* Z_Construct_UClass_UFactory();
	ENGINE_API class UClass* Z_Construct_UClass_AActor();
	ENGINE_API class UClass* Z_Construct_UClass_UMaterial_NoRegister();
	COREUOBJECT_API class UScriptStruct* Z_Construct_UScriptStruct_FQuat();
	ENGINE_API class UClass* Z_Construct_UClass_UPhysicsConstraintComponent_NoRegister();
	ENGINE_API class UClass* Z_Construct_UClass_UPrimitiveComponent_NoRegister();
	COREUOBJECT_API class UScriptStruct* Z_Construct_UScriptStruct_FVector();
	COREUOBJECT_API class UScriptStruct* Z_Construct_UScriptStruct_FRotator();

	UNREALROBOTS_API class UClass* Z_Construct_UClass_URFactoryRRobot_NoRegister();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_URFactoryRRobot();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_URURDFData_NoRegister();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_URURDFData();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_URFactoryRURDFData_NoRegister();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_URFactoryRURDFData();
	UNREALROBOTS_API class UFunction* Z_Construct_UFunction_ARRobot_AddForceToJoint();
	UNREALROBOTS_API class UFunction* Z_Construct_UFunction_ARRobot_GetJointPosition();
	UNREALROBOTS_API class UFunction* Z_Construct_UFunction_ARRobot_GetJointVelocity();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_ARRobot_NoRegister();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_ARRobot();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_ARTestRobotMotors_NoRegister();
	UNREALROBOTS_API class UClass* Z_Construct_UClass_ARTestRobotMotors();
	UNREALROBOTS_API class UPackage* Z_Construct_UPackage__Script_UnrealRobots();
	void URFactoryRRobot::StaticRegisterNativesURFactoryRRobot()
	{
	}
	UClass* Z_Construct_UClass_URFactoryRRobot_NoRegister()
	{
		return URFactoryRRobot::StaticClass();
	}
	UClass* Z_Construct_UClass_URFactoryRRobot()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UActorFactory();
			Z_Construct_UPackage__Script_UnrealRobots();
			OuterClass = URFactoryRRobot::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x2010308C;


				OuterClass->ClassConfigName = FName(TEXT("Editor"));
				static TCppClassTypeInfo<TCppClassTypeTraits<URFactoryRRobot> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("HideCategories"), TEXT("Object"));
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("RFactoryRRobot.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/RFactoryRRobot.h"));
				MetaData->SetValue(OuterClass, TEXT("ObjectInitializerConstructorDeclared"), TEXT(""));
				MetaData->SetValue(OuterClass, TEXT("ToolTip"), TEXT("Own ActorFactory which creates a robot from URURDFData Asset."));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(URFactoryRRobot, 1615043241);
	static FCompiledInDefer Z_CompiledInDefer_UClass_URFactoryRRobot(Z_Construct_UClass_URFactoryRRobot, &URFactoryRRobot::StaticClass, TEXT("/Script/UnrealRobots"), TEXT("URFactoryRRobot"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URFactoryRRobot);
	void URURDFData::StaticRegisterNativesURURDFData()
	{
	}
	UClass* Z_Construct_UClass_URURDFData_NoRegister()
	{
		return URURDFData::StaticClass();
	}
	UClass* Z_Construct_UClass_URURDFData()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UObject();
			Z_Construct_UPackage__Script_UnrealRobots();
			OuterClass = URURDFData::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20900080;


PRAGMA_DISABLE_DEPRECATION_WARNINGS
				UProperty* NewProp_AssetImportData = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("AssetImportData"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(AssetImportData, URURDFData), 0x00120000000a0009, Z_Construct_UClass_UAssetImportData_NoRegister());
				UProperty* NewProp_Xml = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("Xml"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(Xml, URURDFData), 0x0010010000020001);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
				static TCppClassTypeInfo<TCppClassTypeTraits<URURDFData> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("RURDFData.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/RURDFData.h"));
				MetaData->SetValue(OuterClass, TEXT("ToolTip"), TEXT("This object is created as uAsset and secures the information for the construction of RRobots."));
				MetaData->SetValue(NewProp_AssetImportData, TEXT("Category"), TEXT("ImportSettings"));
				MetaData->SetValue(NewProp_AssetImportData, TEXT("EditInline"), TEXT("true"));
				MetaData->SetValue(NewProp_AssetImportData, TEXT("ModuleRelativePath"), TEXT("Public/RURDFData.h"));
				MetaData->SetValue(NewProp_AssetImportData, TEXT("ToolTip"), TEXT("For reimport. Includes the reimport information."));
				MetaData->SetValue(NewProp_Xml, TEXT("Category"), TEXT("RURDFData"));
				MetaData->SetValue(NewProp_Xml, TEXT("ModuleRelativePath"), TEXT("Public/RURDFData.h"));
				MetaData->SetValue(NewProp_Xml, TEXT("ToolTip"), TEXT("Saves the XML"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(URURDFData, 2365008528);
	static FCompiledInDefer Z_CompiledInDefer_UClass_URURDFData(Z_Construct_UClass_URURDFData, &URURDFData::StaticClass, TEXT("/Script/UnrealRobots"), TEXT("URURDFData"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URURDFData);
	void URFactoryRURDFData::StaticRegisterNativesURFactoryRURDFData()
	{
	}
	UClass* Z_Construct_UClass_URFactoryRURDFData_NoRegister()
	{
		return URFactoryRURDFData::StaticClass();
	}
	UClass* Z_Construct_UClass_URFactoryRURDFData()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UFactory();
			Z_Construct_UPackage__Script_UnrealRobots();
			OuterClass = URFactoryRURDFData::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20100080;


				static TCppClassTypeInfo<TCppClassTypeTraits<URFactoryRURDFData> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("RFactoryRURDFData.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/RFactoryRURDFData.h"));
				MetaData->SetValue(OuterClass, TEXT("ObjectInitializerConstructorDeclared"), TEXT(""));
				MetaData->SetValue(OuterClass, TEXT("ToolTip"), TEXT("This class provides the factory and reimport functionality to read an XML (URDF) file.\nThe generated uAsset is a RURDFData for further processing."));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(URFactoryRURDFData, 1393102100);
	static FCompiledInDefer Z_CompiledInDefer_UClass_URFactoryRURDFData(Z_Construct_UClass_URFactoryRURDFData, &URFactoryRURDFData::StaticClass, TEXT("/Script/UnrealRobots"), TEXT("URFactoryRURDFData"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URFactoryRURDFData);
	void ARRobot::StaticRegisterNativesARRobot()
	{
		UClass* Class = ARRobot::StaticClass();
		static const TNameNativePtrPair<ANSICHAR> AnsiFuncs[] = {
			{ "AddForceToJoint", (Native)&ARRobot::execAddForceToJoint },
			{ "GetJointPosition", (Native)&ARRobot::execGetJointPosition },
			{ "GetJointVelocity", (Native)&ARRobot::execGetJointVelocity },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, AnsiFuncs, 3);
	}
	UFunction* Z_Construct_UFunction_ARRobot_AddForceToJoint()
	{
		struct RRobot_eventAddForceToJoint_Parms
		{
			FString JointName;
			float Force;
		};
		UObject* Outer=Z_Construct_UClass_ARRobot();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("AddForceToJoint"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), NULL, 0x04020401, 65535, sizeof(RRobot_eventAddForceToJoint_Parms));
			UProperty* NewProp_Force = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("Force"), RF_Public|RF_Transient|RF_MarkAsNative) UFloatProperty(CPP_PROPERTY_BASE(Force, RRobot_eventAddForceToJoint_Parms), 0x0010000000000080);
			UProperty* NewProp_JointName = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("JointName"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(JointName, RRobot_eventAddForceToJoint_Parms), 0x0010000000000080);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("Robot"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
			MetaData->SetValue(ReturnFunction, TEXT("ToolTip"), TEXT("Add force / torque to given Joint"));
#endif
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_ARRobot_GetJointPosition()
	{
		struct RRobot_eventGetJointPosition_Parms
		{
			FString JointName;
			float ReturnValue;
		};
		UObject* Outer=Z_Construct_UClass_ARRobot();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("GetJointPosition"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), NULL, 0x04020401, 65535, sizeof(RRobot_eventGetJointPosition_Parms));
			UProperty* NewProp_ReturnValue = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("ReturnValue"), RF_Public|RF_Transient|RF_MarkAsNative) UFloatProperty(CPP_PROPERTY_BASE(ReturnValue, RRobot_eventGetJointPosition_Parms), 0x0010000000000580);
			UProperty* NewProp_JointName = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("JointName"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(JointName, RRobot_eventGetJointPosition_Parms), 0x0010000000000080);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("Robot"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
			MetaData->SetValue(ReturnFunction, TEXT("ToolTip"), TEXT("Get Joint Position in *Degrees*"));
#endif
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_ARRobot_GetJointVelocity()
	{
		struct RRobot_eventGetJointVelocity_Parms
		{
			FString JointName;
			float ReturnValue;
		};
		UObject* Outer=Z_Construct_UClass_ARRobot();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("GetJointVelocity"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), NULL, 0x04020401, 65535, sizeof(RRobot_eventGetJointVelocity_Parms));
			UProperty* NewProp_ReturnValue = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("ReturnValue"), RF_Public|RF_Transient|RF_MarkAsNative) UFloatProperty(CPP_PROPERTY_BASE(ReturnValue, RRobot_eventGetJointVelocity_Parms), 0x0010000000000580);
			UProperty* NewProp_JointName = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("JointName"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(JointName, RRobot_eventGetJointVelocity_Parms), 0x0010000000000080);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("Robot"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
			MetaData->SetValue(ReturnFunction, TEXT("ToolTip"), TEXT("Get Joint Velocity in Deg/s"));
#endif
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_ARRobot_NoRegister()
	{
		return ARRobot::StaticClass();
	}
	UClass* Z_Construct_UClass_ARRobot()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_AActor();
			Z_Construct_UPackage__Script_UnrealRobots();
			OuterClass = ARRobot::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20900080;

				OuterClass->LinkChild(Z_Construct_UFunction_ARRobot_AddForceToJoint());
				OuterClass->LinkChild(Z_Construct_UFunction_ARRobot_GetJointPosition());
				OuterClass->LinkChild(Z_Construct_UFunction_ARRobot_GetJointVelocity());

PRAGMA_DISABLE_DEPRECATION_WARNINGS
				UProperty* NewProp_BasicMaterial = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("BasicMaterial"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(BasicMaterial, ARRobot), 0x0010000000000001, Z_Construct_UClass_UMaterial_NoRegister());
				UProperty* NewProp_XmlUrdf = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("XmlUrdf"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(XmlUrdf, ARRobot), 0x0010000000000009);
				UProperty* NewProp_globalVarA = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("globalVarA"), RF_Public|RF_Transient|RF_MarkAsNative) UFloatProperty(CPP_PROPERTY_BASE(globalVarA, ARRobot), 0x0010000000000009);
				UProperty* NewProp_OriginRotations = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("OriginRotations"), RF_Public|RF_Transient|RF_MarkAsNative) UMapProperty(CPP_PROPERTY_BASE(OriginRotations, ARRobot), 0x0010000000020005);
				UProperty* NewProp_OriginRotations_Key_KeyProp = new(EC_InternalUseOnlyConstructor, NewProp_OriginRotations, TEXT("OriginRotations_Key"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(FObjectInitializer(), EC_CppProperty, 0, 0x0000000000020001);
				UProperty* NewProp_OriginRotations_ValueProp = new(EC_InternalUseOnlyConstructor, NewProp_OriginRotations, TEXT("OriginRotations"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(FObjectInitializer(), EC_CppProperty, 1, 0x0000000000020001, Z_Construct_UScriptStruct_FQuat());
				UProperty* NewProp_JointComponents = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("JointComponents"), RF_Public|RF_Transient|RF_MarkAsNative) UMapProperty(CPP_PROPERTY_BASE(JointComponents, ARRobot), 0x001000800002000d);
				UProperty* NewProp_JointComponents_Key_KeyProp = new(EC_InternalUseOnlyConstructor, NewProp_JointComponents, TEXT("JointComponents_Key"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(FObjectInitializer(), EC_CppProperty, 0, 0x00000000000a0009);
				UProperty* NewProp_JointComponents_ValueProp = new(EC_InternalUseOnlyConstructor, NewProp_JointComponents, TEXT("JointComponents"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(FObjectInitializer(), EC_CppProperty, 1, 0x00000000000a0009, Z_Construct_UClass_UPhysicsConstraintComponent_NoRegister());
				UProperty* NewProp_LinkComponents = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("LinkComponents"), RF_Public|RF_Transient|RF_MarkAsNative) UMapProperty(CPP_PROPERTY_BASE(LinkComponents, ARRobot), 0x001000800002000d);
				UProperty* NewProp_LinkComponents_Key_KeyProp = new(EC_InternalUseOnlyConstructor, NewProp_LinkComponents, TEXT("LinkComponents_Key"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(FObjectInitializer(), EC_CppProperty, 0, 0x00000000000a0009);
				UProperty* NewProp_LinkComponents_ValueProp = new(EC_InternalUseOnlyConstructor, NewProp_LinkComponents, TEXT("LinkComponents"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(FObjectInitializer(), EC_CppProperty, 1, 0x00000000000a0009, Z_Construct_UClass_UPrimitiveComponent_NoRegister());
PRAGMA_ENABLE_DEPRECATION_WARNINGS
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_ARRobot_AddForceToJoint(), "AddForceToJoint"); // 149874951
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_ARRobot_GetJointPosition(), "GetJointPosition"); // 3747251252
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_ARRobot_GetJointVelocity(), "GetJointVelocity"); // 2270122017
				static TCppClassTypeInfo<TCppClassTypeTraits<ARRobot> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("BlueprintType"), TEXT("true"));
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("RRobot.h"));
				MetaData->SetValue(OuterClass, TEXT("IsBlueprintBase"), TEXT("true"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
				MetaData->SetValue(NewProp_BasicMaterial, TEXT("Category"), TEXT("RRobot"));
				MetaData->SetValue(NewProp_BasicMaterial, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
				MetaData->SetValue(NewProp_BasicMaterial, TEXT("ToolTip"), TEXT("The material used for all robot links"));
				MetaData->SetValue(NewProp_XmlUrdf, TEXT("Category"), TEXT("RRobot"));
				MetaData->SetValue(NewProp_XmlUrdf, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
				MetaData->SetValue(NewProp_XmlUrdf, TEXT("ToolTip"), TEXT("Copy the XML URDF Code"));
				MetaData->SetValue(NewProp_globalVarA, TEXT("Category"), TEXT("RRobot"));
				MetaData->SetValue(NewProp_globalVarA, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
				MetaData->SetValue(NewProp_OriginRotations, TEXT("Category"), TEXT("Map"));
				MetaData->SetValue(NewProp_OriginRotations, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
				MetaData->SetValue(NewProp_OriginRotations, TEXT("ToolTip"), TEXT("Initial Relative Rotation (Quaternion)"));
				MetaData->SetValue(NewProp_JointComponents, TEXT("Category"), TEXT("Map"));
				MetaData->SetValue(NewProp_JointComponents, TEXT("EditInline"), TEXT("true"));
				MetaData->SetValue(NewProp_JointComponents, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
				MetaData->SetValue(NewProp_JointComponents, TEXT("ToolTip"), TEXT("All the joints that connect the links together. Key is Name of joint, Value is the joint."));
				MetaData->SetValue(NewProp_LinkComponents, TEXT("Category"), TEXT("Map"));
				MetaData->SetValue(NewProp_LinkComponents, TEXT("EditInline"), TEXT("true"));
				MetaData->SetValue(NewProp_LinkComponents, TEXT("ModuleRelativePath"), TEXT("Public/RRobot.h"));
				MetaData->SetValue(NewProp_LinkComponents, TEXT("ToolTip"), TEXT("All the links that are attached to this Robot. Key is Name of link, Value is the link."));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(ARRobot, 1024075662);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ARRobot(Z_Construct_UClass_ARRobot, &ARRobot::StaticClass, TEXT("/Script/UnrealRobots"), TEXT("ARRobot"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ARRobot);
	void ARTestRobotMotors::StaticRegisterNativesARTestRobotMotors()
	{
	}
	UClass* Z_Construct_UClass_ARTestRobotMotors_NoRegister()
	{
		return ARTestRobotMotors::StaticClass();
	}
	UClass* Z_Construct_UClass_ARTestRobotMotors()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_ARRobot();
			Z_Construct_UPackage__Script_UnrealRobots();
			OuterClass = ARTestRobotMotors::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20900080;


PRAGMA_DISABLE_DEPRECATION_WARNINGS
				UProperty* NewProp_TargetLocation = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("TargetLocation"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(CPP_PROPERTY_BASE(TargetLocation, ARTestRobotMotors), 0x0040000000000001, Z_Construct_UScriptStruct_FVector());
				UProperty* NewProp_TargetRotation = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("TargetRotation"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(CPP_PROPERTY_BASE(TargetRotation, ARTestRobotMotors), 0x0040000000000001, Z_Construct_UScriptStruct_FRotator());
PRAGMA_ENABLE_DEPRECATION_WARNINGS
				static TCppClassTypeInfo<TCppClassTypeTraits<ARTestRobotMotors> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("RTestRobotMotors.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/RTestRobotMotors.h"));
				MetaData->SetValue(NewProp_TargetLocation, TEXT("Category"), TEXT("RTestRobotMotors"));
				MetaData->SetValue(NewProp_TargetLocation, TEXT("ModuleRelativePath"), TEXT("Public/RTestRobotMotors.h"));
				MetaData->SetValue(NewProp_TargetLocation, TEXT("ToolTip"), TEXT("Target location for motor of first_to_second_joint"));
				MetaData->SetValue(NewProp_TargetRotation, TEXT("Category"), TEXT("RTestRobotMotors"));
				MetaData->SetValue(NewProp_TargetRotation, TEXT("ModuleRelativePath"), TEXT("Public/RTestRobotMotors.h"));
				MetaData->SetValue(NewProp_TargetRotation, TEXT("ToolTip"), TEXT("Target rotation for motor of second_to_third_joint"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(ARTestRobotMotors, 1317677361);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ARTestRobotMotors(Z_Construct_UClass_ARTestRobotMotors, &ARTestRobotMotors::StaticClass, TEXT("/Script/UnrealRobots"), TEXT("ARTestRobotMotors"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ARTestRobotMotors);
	UPackage* Z_Construct_UPackage__Script_UnrealRobots()
	{
		static UPackage* ReturnPackage = nullptr;
		if (!ReturnPackage)
		{
			ReturnPackage = CastChecked<UPackage>(StaticFindObjectFast(UPackage::StaticClass(), nullptr, FName(TEXT("/Script/UnrealRobots")), false, false));
			ReturnPackage->SetPackageFlags(PKG_CompiledIn | 0x00000080);
			FGuid Guid;
			Guid.A = 0xCAD26BAC;
			Guid.B = 0x14831F34;
			Guid.C = 0x00000000;
			Guid.D = 0x00000000;
			ReturnPackage->SetGuid(Guid);

		}
		return ReturnPackage;
	}
#endif
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
