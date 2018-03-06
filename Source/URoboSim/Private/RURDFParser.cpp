// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RURDFParser.h"
#include "IURoboSim.h"
#include "RRobot.h"


FRURDFParser::FRURDFParser(ARRobot* Robot_)
{
    this->Robot = Robot_;
}

FRURDFParser::~FRURDFParser()
{
}

bool FRURDFParser::ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue)
{
    FString AttName(AttributeName);
    FString AttValue(AttributeValue);

    if (!AttName.IsEmpty() && !AttValue.IsEmpty())
    {

        if (Stack.Top().Equals("joint"))
        {
            FString stackOldTop = Stack.Pop();

            //Handle special case 1 of joint tags within a transmission tag
            if (Stack.FindLast("transmission") > Stack.FindLast("joint") && Stack.FindLast("transmission") > Stack.FindLast("link") && Stack.FindLast("transmission") > Stack.FindLast("gazebo") && Stack.FindLast("transmission") > Stack.FindLast("material")) {
                stackOldTop = "transmission_joint"; //When the parser later pops a joint tag from the stack, it needs to know that a joint tag within transmission is a different case from a regular joint tag
                AttMap.Add(stackOldTop + "_" + AttName, AttValue);
                Stack.Push(stackOldTop);
                return true;
            }
            //---

            //Handle special case 2 of joint tags within a gazebo tag
            if (Stack.FindLast("gazebo") > Stack.FindLast("joint") && Stack.FindLast("gazebo") > Stack.FindLast("link") && Stack.FindLast("gazebo") > Stack.FindLast("transmission") && Stack.FindLast("gazebo") > Stack.FindLast("material")) {
                stackOldTop = "gazebo_joint"; //When the parser later pops a joint tag from the stack, it needs to know that a joint tag within transmission is a different case from a regular joint tag
                AttMap.Add(stackOldTop + "_" + AttName, AttValue);
                Stack.Push(stackOldTop);
                return true;
            }
            //---

            Stack.Push(stackOldTop);
        }

        //Handles the sub categories for Links
        if (!Stack.Top().Equals("gazebo") && !Stack.Top().Equals("transmission") && !Stack.Top().Equals("material") && !Stack.Top().Equals("link") && Stack.FindLast("link") > Stack.FindLast("joint") ) {

            // Look which element is the current one.
            int32 IndexVisual = Stack.FindLast("visual");
            int32 IndexInertial = Stack.FindLast("inertial");
            int32 IndexCollision = Stack.FindLast("collision");

            // Check current context.
            if (IndexInertial > IndexCollision && IndexInertial > IndexVisual)
            {
                AttMap.Add("inertial_" + Stack.Top() + "_" + AttName, AttValue);
            }
            else if (IndexVisual > IndexCollision && IndexVisual > IndexInertial)
            {
                AttMap.Add("visual_" + Stack.Top() + "_" + AttName, AttValue);
            }
            else if (IndexCollision > IndexVisual && IndexCollision > IndexInertial)
            {
                AttMap.Add("collision_" + Stack.Top() + "_" + AttName, AttValue);
            }
            else
            {
                // if fails.
                UE_LOG(LogTemp, Warning, TEXT("Unexpected attribute in robot description.\n"));

                return false;
            }
            return true;
        }
        // Add Stackelement from Top, Attributname and Attributvalue to a Map.

        else
        {
            AttMap.Add(Stack.Top() + "_" + AttName, AttValue);
        }
        return true;
    }
    else
    {
        // If there a problem with XML attribute.
        UE_LOG(LogTemp, Warning, TEXT("Failed to read attribute of robot description.\n"));

        return false;
    }
}

bool FRURDFParser::ProcessClose(const TCHAR* ElementName)
{
    if (Stack.Top().IsEmpty())
    {
        // If we have no element in the stack.
        UE_LOG(LogTemp, Warning, TEXT("Error in robot description.\n"));

        return false;
    }
    else if (Stack.Top().Equals("joint") )
    {

        FString JointName = AttMap.FindRef("joint_name");			// required value (urdf).
        FString Type = AttMap.FindRef("joint_type");				// required value (urdf).
        FString Parent = AttMap.FindRef("parent_link");				// required value (urdf).
        FString Child = AttMap.FindRef("child_link");				// required value (urdf).

        float LimitUpper = 0.f;				// default value (urdf).
        float LimitLower = 0.f;				// default value (urdf).

        FVector Location(0.f, 0.f, 0.f);			// default value (urdf).
        FRotator Rotation(0.f, 0.f, 0.f);			// default value (urdf).
        FVector Axis(1.f, 0.f, 0.f);				// default value (urdf).

        // If a required value is missing.
        if (Parent.IsEmpty() || Child.IsEmpty() || JointName.IsEmpty() || Type.IsEmpty()) {

            UE_LOG(LogTemp, Warning, TEXT("Missing required data for a joint in robot description.\n"));
            return false;
        }

        // For Location and Rotation...if not, then default value.
        if (AttMap.Contains("origin_xyz"))
        {
            Location = GenerateVectors("origin_xyz");
        }
        if (AttMap.Contains("origin_rpy"))
        {
            Rotation = GenerateRotaters("origin_rpy");
        }
        if (AttMap.Contains("axis_xyz"))
        {
            Axis = GenerateVectors("axis_xyz");
        }
        if (AttMap.Contains("limit_lower"))
        {
            LimitLower = FCString::Atof(*(AttMap.FindRef("limit_lower")));
        }
        if (AttMap.Contains("limit_upper"))
        {
            LimitUpper = FCString::Atof(*(AttMap.FindRef("limit_upper")));
        }

        // Required at prismatic and revolute
        if ((Type.Equals("prismatic") || Type.Equals("revolute")) &&
                (AttMap.FindRef("limit_effort").IsEmpty() || AttMap.FindRef("limit_velocity").IsEmpty()))
        {

            UE_LOG(LogTemp, Warning, TEXT("Missing required data for prismatic/revolute joint.\n"));
            return false;
        }

        // Creates the joint and set the values of the struct.
        FRJoint NewJoint;
        NewJoint.Name = JointName;
        NewJoint.Type = Type;
        NewJoint.Location = Location;
        NewJoint.Rotation = Rotation;
        NewJoint.Parent = Parent;
        NewJoint.Child = Child;
        NewJoint.Axis = Axis;
        NewJoint.LowerLimit = LimitLower;
        NewJoint.UpperLimit = LimitUpper;

        // for safety limits...
        NewJoint.Effort = FCString::Atof(*AttMap.FindRef("limit_effort"));
        NewJoint.Velocity = FCString::Atof(*AttMap.FindRef("limit_velocity"));

        // for safety controller...
        NewJoint.k_velocity = FCString::Atof(*AttMap.FindRef("safety_controller_k_velocity"));
        NewJoint.damping = FCString::Atof(*AttMap.FindRef("dynamics_damping"));
        NewJoint.friction = FCString::Atof(*AttMap.FindRef("dynamics_friction"));

        // Add the joint data to the Robot
        Robot->AddJoint(NewJoint);

        //Makes the AttMap empty and deletes the stack element (joint).
        AttMap.Empty();
        Stack.Pop();

        return true;

    }
    else if (Stack.Top().Equals("link"))
    {

        FString LinkName = AttMap.FindRef("link_name");		// required value (urdf).

        // for Link Interial Elements
        FVector InertialLocation(0.f, 0.f, 0.f);		    // default value (urdf).
        FRotator InertialRotation(0.f, 0.f, 0.f);			// default value (urdf).

        // for Link Visual Elements
        FVector VisualLocation(0.f, 0.f, 0.f);			// default value (urdf).
        FRotator VisualRotation(0.f, 0.f, 0.f);			// default value (urdf).
        FString VisualMesh;							// required value (urdf).
        FVector VisualScale;						// required value, but optional for custom meshes.

        FString VisualMaterialName;
        FColor VisualMaterialColor;
        FString VisualMaterialTexture;

        // for Link Collision Elements
        FVector CollisionLocation(0.f, 0.f, 0.f);			// default value (urdf).
        FRotator CollisionRotation(0.f, 0.f, 0.f);		// default value (urdf).

        FString CollisionMesh;
        FVector CollisionScale;

        // set the optional origin xyz values under visual, inertial and collision
        if (AttMap.Contains("inertial_origin_xyz")) {
            InertialLocation = GenerateVectors("inertial_origin_xyz");
        }
        if (AttMap.Contains("visual_origin_xyz")) {
            VisualLocation = GenerateVectors("visual_origin_xyz");
        }
        if (AttMap.Contains("collision_origin_xyz")) {
            CollisionLocation = GenerateVectors("collision_origin_xyz");
        }

        // set the optional origin rpy values of visual, inertial and collision
        if (AttMap.Contains("inertial_origin_rpy")) {
            InertialRotation = GenerateRotaters("inertial_origin_rpy");
        }
        if (AttMap.Contains("visual_origin_rpy")) {
            VisualRotation = GenerateRotaters("visual_origin_rpy");
        }
        if (AttMap.Contains("collision_origin_rpy")) {
            CollisionRotation = GenerateRotaters("collision_origin_rpy");
        }

        // For Mesh loading
        TPair<FString, FVector> VisualMeshScale = MeshAndScale("visual_");
        TPair<FString, FVector> CollisionMeshScale = MeshAndScale("collision_");;
        VisualMesh = VisualMeshScale.Key;
        VisualScale = VisualMeshScale.Value;
        CollisionMesh = CollisionMeshScale.Key;
        CollisionScale = CollisionMeshScale.Value;

        // If a required value is missing.
        if (LinkName.IsEmpty()) {
            UE_LOG(LogTemp, Warning, TEXT("Missing link name in robot description.\n"));
            return false;
        }
        if (VisualMesh.IsEmpty() && CollisionMesh.IsEmpty()) {
            UE_LOG(LogTemp, Warning, TEXT("Missing mesh name for custom mesh in robot description.%s \n"), *LinkName);
            //return false;
        }

        // Create a new Link for the RLink struct.
        FRLink NewLink;

        NewLink.Name = LinkName;
        NewLink.Inertial.Location = InertialLocation;
        NewLink.Inertial.Rotation = InertialRotation;
        NewLink.Visual.Location = VisualLocation;
        NewLink.Visual.Rotation = VisualRotation;
        NewLink.Visual.Scale = VisualScale;
        NewLink.Visual.Mesh = VisualMesh;

        // for Color.
        if (AttMap.Contains("visual_color_rgba")) {
            TArray<FString> ColorArray;

            AttMap.FindRef("visual_color_rgba").ParseIntoArray(ColorArray, TEXT(" "), true);
            uint8 R = FCString::Atof(*ColorArray[0]);
            uint8 G = FCString::Atof(*ColorArray[1]);
            uint8 B = FCString::Atof(*ColorArray[2]);
            uint8 A = FCString::Atof(*ColorArray[3]);

            NewLink.Visual.Material.Color = FColor(R, G, B, A);
        }

        NewLink.Inertial.Mass = FCString::Atof(*AttMap.FindRef("inertial_mass_value"));

        NewLink.Visual.Material.Name = VisualMaterialName;
        NewLink.Visual.Material.Texture = VisualMaterialTexture;

        NewLink.Collision.Location = CollisionLocation;
        NewLink.Collision.Rotation = CollisionRotation;
        NewLink.Collision.Mesh = CollisionMesh;
        NewLink.Collision.Scale = CollisionScale;

        // Call to create a Link.
        Robot->AddLink(NewLink);

        //Makes the AttMap empty and deletes the stack element (link).
        AttMap.Empty();
        Stack.Pop();

        return true;
    }
    else {
        // If the element in the stack is not joint or link.
        Stack.Pop();
        return true;
    }
}

bool FRURDFParser::ProcessComment(const TCHAR* Comment)
{
    return true;
}

bool FRURDFParser::ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber)
{
    FString ElemName = FString(ElementName);

    if (!ElemName.IsEmpty()) {

        // Element to Stack.
        Stack.Push(ElemName);

        return true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to read an element in robot description.\n"));
        return false;
    }
}

bool FRURDFParser::ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber)
{
    return true;
}

FVector FRURDFParser::GenerateVectors(const FString Element)
{

    // For the conversion.
    TArray<FString> Helper;

    // The FVector is different for spheres.
    if (Element.Contains("sphere_radius", ESearchCase::IgnoreCase)) {

        AttMap.FindRef(Element).ParseIntoArray(Helper, TEXT(" "), true);
        float Val = (FCString::Atof(*Helper[0])) * 2.f;

        return FVector(Val, Val, Val);
    }
    AttMap.FindRef(Element).ParseIntoArray(Helper, TEXT(" "), true);
    float X = FCString::Atof(*Helper[0]);
    float Y = FCString::Atof(*Helper[1]);
    float Z = FCString::Atof(*Helper[2]);

    return FVector(X, Y, Z);
}

FRotator FRURDFParser::GenerateRotaters(const FString Element)
{

    // For the conversion.
    TArray<FString> Helper;

    //**********************************************************************
    // In URDF the order of axes might be different to the order in UE4
    //
    // radius: x*2,y*2   length: z
    // size z, y, x
    // rotation (rpy) roll, pitch, yaw (UE4 pitch, yaw, roll)
    //**********************************************************************
    AttMap.FindRef(Element).ParseIntoArray(Helper, TEXT(" "), true);
    float R = FMath::RadiansToDegrees(FCString::Atof(*Helper[0]));
    float P = FMath::RadiansToDegrees(FCString::Atof(*Helper[1]));
    float Y = FMath::RadiansToDegrees(FCString::Atof(*Helper[2]));

    return FRotator(P, Y, R);
}

FVector FRURDFParser::GenerateCylinderScale(const FString ElementRadius, const FString ElementLength)
{

    TArray<FString> HelperRadius, HelperLength;

    // To get the konext (collision or visual).
    FString Element = ElementRadius.Contains("visual") ? "visual_" : "collision_";

    AttMap.FindRef(Element + "cylinder_radius").ParseIntoArray(HelperRadius, TEXT(" "), true);
    AttMap.FindRef(Element + "cylinder_length").ParseIntoArray(HelperLength, TEXT(" "), true);

    // radius * 2 == diameter
    float Radius = FCString::Atof(*HelperRadius[0]) * 2.f;
    float Length = FCString::Atof(*HelperLength[0]);

    return FVector(Radius, Radius, Length);
}

TPair<FString, FVector> FRURDFParser::MeshAndScale(FString Element)
{
    FString VisualMesh = "";
    FVector VisualScale(1.f);

    // For Mesh loading
    if (AttMap.Contains(Element + "box_size"))
    {

        VisualMesh = "box";
        VisualScale = GenerateVectors(Element + "box_size");
    }
    else if (AttMap.Contains(Element + "cylinder_radius"))
    {

        VisualMesh = "cylinder";
        VisualScale = GenerateCylinderScale(Element + "cylinder_radius", Element + "cylinder_length");
    }
    else if (AttMap.Contains(Element + "sphere_radius"))
    {

        VisualMesh = "sphere";
        VisualScale = GenerateVectors(Element + "sphere_radius");
    }
    else if (AttMap.Contains(Element + "mesh_filename"))
    {

        VisualMesh = AttMap.FindRef(Element + "mesh_filename");

        // optional for own meshes.
        if (AttMap.Contains(Element + "mesh_scale"))
        {
            VisualScale = GenerateVectors(Element + "mesh_scale");
        }

    }
    TPair<FString, FVector> MeshScale;
    MeshScale.Key = VisualMesh;
    MeshScale.Value = VisualScale;

    return MeshScale;
}

void  FRURDFParser::ParseURDF(FString XmlUrdf)
{

    // Callback Object for parser
    // FRURDFParser Call(this);

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

    Success = FFastXml::ParseXmlFile((IFastXmlCallback*)this, TEXT(""), StringToParse.GetCharArray().GetData(), nullptr, false, false, Error, LineNumb);


    UE_LOG(LogTemp, Warning, TEXT("Use Paser function to parse URDF"));
    if (!Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create Robot\n"));
    }

}
