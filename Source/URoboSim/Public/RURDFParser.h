// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "IURoboSim.h"
#include "RRobot.h"
#include "Runtime/XmlParser/Public/FastXml.h"

/**
* Parser who gets the information from the XML for the robot.
*/
class UROBOSIM_API FRURDFParser : private IFastXmlCallback
{
private:

	ARRobot* Robot;

	/** Stack to Save the current Element.*/
	TArray<FString> Stack;

	/** Stores the Name and the Value of a Element. */
	TMap<FString, FString> AttMap;

	/** Helper to Create FVectors from AttMap.
	* @param Element Element to find the values.
	* @return The calculated FVector.
	*/
	FVector GenerateVectors(const FString Element);

	/** Helper to Create FRotator from AttMap.
	* @param Element Element to find the values.
	* @return The calculated FRotator.
	*/
	FRotator GenerateRotaters(const FString Element);

	/** Helper to Create a CylinderVector from AttMap. 
	* @param ElementRadiust Element to find the value radius.
	* @param ElementLength Element to find the value length.
	* @return The calculated FVector for Cylinder
	*/
	FVector GenerateCylinderScale(const FString ElementRadius, const FString ElementLength);

	/*Method that loads the mesh or scale for visual and/or collision. 
	* Scale is optional for .fbx meshes.
	* @param Element String that indicates whether collision or visual is searched.
	* @return TPair with the mesh as FString and the Scale as FVector.
	*/
	TPair<FString, FVector> MeshAndScale(FString Element);

	/** Called when an XML attribute is encountered for the current scope's element.
	* Elements are added to the map (according to the stack).
	* @param AttributeName Name of the attribute.
	* @param AttributeValue Value of the attribute
	* @return true if successful, false otherwise.
	*/
	bool ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue);

	/** Called when an element's scope ends in the XML file. 
	* Use the stack and the map to pass the information to the robot and build the links and joints accordingly.
	* @param ElementName Name of the element which ends. 
	* @return true if successful, false otherwise.
	*/
	bool ProcessClose(const TCHAR* ElementName);

	/** Called when a comment is encountered. This can happen pretty much anywhere in the file. 
	* Always true because for RRobot comments are unimportant.
	* @param Comment The Comment 
	* @return always true.
	*/
	bool ProcessComment(const TCHAR* Comment);

	/** Called when a new XML element is encountered, starting a new scope. 
	* @param ElementName Name of the Element.
	* @param ElementData  
	* @param XmlFileLineNumber Line of the xml
	* @return true if successful, false otherwise.
	*/
	bool ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber);

	/** Called after XML header is parsed. 
	* @param ElementData
	* @param XmlFileLineNumber
	* @return always true.
	*/
	bool ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber);

public:

	FRURDFParser(ARRobot* Robot);

	virtual ~FRURDFParser();
};
