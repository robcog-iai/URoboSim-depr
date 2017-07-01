Every milestone should be updated with the description of the project


Our fourth milestone as described in Trello from 2017-03-10:
    
- drag and drop urdf, generate primitive meshes from it (import meshes in case of 3d mesh)
- meshes organized in corresponding folders, with corresponding names 
- generating the robots (either at load time, or runtime)


Our implementation:

- Drag and drop urdf, generate primitive meshes from it (import meshes in case of 3d mesh) 
  - XML files can be dragged into the content browser or imported via the import button. Custom meshes are imported. Collision of primitive meshes (box, cylinder, sphere) are realised with shapes, however for cylinder a capsule shape is used, which has slightly different properties and may cause problems for very short cylinders.  
- Meshes organized in corresponding folders, with corresponding names 
  - Meshes are imported accordingly into a folder with the name of the robot (folder is created if not present).  
- Generating the robots (either at load time, or runtime) 
  - Robots are created in the editor (and recreated in runtime due technical issues with prismatic links)
- Rotate attached objects (from first milestone) 
  - Links that are constrained by a joint type that supports rotation can be rotated to a target rotation with the method `RotateJoint` (`MovePrismaticJoint` to move a link that is constrained by a prismatic joint)

- - -

Necessary Software:

Unreal Engine 4 - 2017-02-03 (Tested with Unreal Engine 4.15).  

- - -

Execution:

The XML file can be imported to the content browser via the Import button "Import" or drag & drop.
An asset is then created with the urdf robot information, which can be dragged and dropped into the world to create the robot described by the URDF.
There are some example robots in the XML folder.
Re-import allows you to overwrite the asset (via Import button "Import" or drag & drop).  

Custom meshes must be present in .fbx format and are imported in a new folder (named after the robot in the same directory).
The meshes can be reimported by reimporting the robot description file (URDF).


The XML file must adhere to URDF standards as they are stated here:  
http://wiki.ros.org/urdf/XML/model  
http://wiki.ros.org/urdf/XML/joint  
http://wiki.ros.org/urdf/XML/link  

- URDF standards apply unless stated otherwise.  
- Only definitions of Links and Joints of a robot are supported. Additional content in the XML may cause unexpected behaviour.  
- Material and texture elements are currently not supported and will be ignored.  
- Every link and joint must have a unique name.  
- Double definitions of elements and attributes within a link/joint are not supported and may cause unexpected behaviour.  
- Global definitions for reusing purposes are not supported and may cause unexpected behaviour.  
- The path to custom meshes (.fbx) in the XML must be full paths. Otherwise the meshes cannot be imported (list of all faulty paths will be displayed onscreen).


	

