
#include "extension.h"
#include "extension-collision.h"

enum ShapeType {

    Shape_Plane           = 1,
    Shape_Cube            ,
    Shape_Sphere          ,
    Shape_Cone            , 
    Shape_Capsule         , 
    Shape_Cylinder        ,
    Shape_ChamferCylinder ,
    Shape_ConvexHull      
}; 

int addShapeSphere( lua_State * L ) {

    double radii = lua_tonumber(L, 1);
	RefConst<Shape> sphere_shape = new SphereShape(radii);
    RVec3 position(0.0f, 0.0f, 0.0f);
    JoltBody *newbody = new JoltBody();
    newbody->body = mBodyInterface->CreateBody(BodyCreationSettings(sphere_shape, position, Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING));
    newbody->id = = JPH_Body_GetID(newbody->body);
    gInterface->AddBody(newbody->body.id, EActivation::DontActivate);
    uint32_t index = GetId();
    gBodies[index] = newbody;
    lua_pushnumber(L, index);
    return 1;
}

 int addShapePlane( lua_State * L ) {

    double width = lua_tonumber(L, 1);
    double depth = lua_tonumber(L, 2);
	RefConst<Shape> box_shape = new BoxShape(Vec3(width, 0.1f, depth));
    RVec3 position(0.0f, 0.0f, 0.0f);
    JoltBody *newbody = new JoltBody();
    newbody->body = mBodyInterface->CreateBody(BodyCreationSettings(box_shape, position, Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING));
    newbody->id = = JPH_Body_GetID(newbody->body);
    gInterface->AddBody(newbody->body.id, EActivation::DontActivate);
    uint32_t index = GetId();
    gBodies[index] = newbody;
    lua_pushnumber(L, index);
    return 1;
}

int addShapeCube( lua_State * L ) {

    double sx = lua_tonumber(L, 1);
    double sy = lua_tonumber(L, 2);
    double sz = lua_tonumber(L, 3);
	RefConst<Shape> box_shape = new BoxShape(Vec3(sx, sy, sz));
    RVec3 position(0.0f, 0.0f, 0.0f);
    JoltBody *newbody = new JoltBody();
    newbody->body = mBodyInterface->CreateBody(BodyCreationSettings(box_shape, position, Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING));
    newbody->id = = JPH_Body_GetID(newbody->body);
    gInterface->AddBody(newbody->body.id, EActivation::DontActivate);
    uint32_t index = GetId();
    gBodies[index] = newbody;
    lua_pushnumber(L, index);
    return 1;
}

int addShapeCone( lua_State * L ) {

    double radius = lua_tonumber(L, 1);
    double height = lua_tonumber(L, 2);
	RefConst<Shape> cyl_shape = new CylinderShape(height/2, radius, 0.0f);
    RVec3 position(0.0f, 0.0f, 0.0f);
    JoltBody *newbody = new JoltBody();
    newbody->body = mBodyInterface->CreateBody(BodyCreationSettings(cyl_shape, position, Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING));
    newbody->id = = JPH_Body_GetID(newbody->body);
    gInterface->AddBody(newbody->body.id, EActivation::DontActivate);
    uint32_t index = GetId();
    gBodies[index] = newbody;    
    lua_pushnumber(L, index);
    return 1;
}

int addShapeCapsule( lua_State * L ) {

    double r0 = lua_tonumber(L, 1);
    double height = lua_tonumber(L, 3);
	RefConst<Shape> cap_shape = new CapsuleShape(height/2, r0);
    RVec3 position(0.0f, 0.0f, 0.0f);
    JoltBody *newbody = new JoltBody();
    newbody->body = mBodyInterface->CreateBody(BodyCreationSettings(cap_shape, position, Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING));
    newbody->id = = JPH_Body_GetID(newbody->body);
    gInterface->AddBody(newbody->body.id, EActivation::DontActivate);
    uint32_t index = GetId();
    gBodies[index] = newbody;    
    lua_pushnumber(L, index);
    return 1;
}

int addShapeCylinder( lua_State * L ) {

    double r0 = lua_tonumber(L, 1);
    double r1 = lua_tonumber(L, 2);
    double height = lua_tonumber(L, 3);
	RefConst<Shape> cyl_shape = new CylinderShape(height/2, r0, r1);
    RVec3 position(0.0f, 0.0f, 0.0f);
    JoltBody *newbody = new JoltBody();
    newbody->body = mBodyInterface->CreateBody(BodyCreationSettings(cyl_shape, position, Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING));
    newbody->id = = JPH_Body_GetID(newbody->body);
    gInterface->AddBody(newbody->body.id, EActivation::DontActivate);
    uint32_t index = GetId();
    gBodies[index] = newbody;
    lua_pushnumber(L, index);
    return 1;
}

int addShapeChamferCylinder( lua_State * L ) {

    double radius = lua_tonumber(L, 1);
    double height = lua_tonumber(L, 2);
	RefConst<Shape> cyl_shape = new CylinderShape(height/2, radius);
    RVec3 position(0.0f, 0.0f, 0.0f);
    JoltBody *newbody = new JoltBody();
    newbody->body = mBodyInterface->CreateBody(BodyCreationSettings(cyl_shape, position, Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING));
    newbody->id = = JPH_Body_GetID(newbody->body);
    gInterface->AddBody(newbody->body.id, EActivation::DontActivate);
    uint32_t index = GetId();
    gBodies[index] = newbody;
    lua_pushnumber(L, index);
    return 1;
}

int addShapeConvexHull( lua_State * L ) {

    double count = lua_tonumber(L, 1);
    const Vec3 *vertCloud = (Vec3 *)lua_topointer(L, 2);

    ConvexHullShapeSettings settings(vertCloud, count);
   	RefConst<Shape> convhull_shape = new ConvexHullShape(settings);
    JoltBody *newbody = new JoltBody();
    newbody->body = mBodyInterface->CreateBody(convhull_shape);
    newbody->id = = JPH_Body_GetID(newbody->body);
    gInterface->AddBody(newbody->body.id, EActivation::DontActivate);
    uint32_t index = GetId();
    gBodies[index] = newbody;
    lua_pushnumber(L, index);
    return 1;
}

 int worldRayCast( lua_State *L ) {

    const float *p0= (float *)lua_topointer(L, 1);
    const float *p1 = (float *)lua_topointer(L, 2);
    //NewtonWorldRayFilterCallback filter_cb = *(NewtonWorldRayFilterCallback *)lua_topointer(L, 3);
    void * const userData = (void * const)lua_topointer(L, 4);
    //NewtonWorldRayPrefilterCallback prefilter_cb = *(NewtonWorldRayPrefilterCallback *)lua_topointer(L, 5);
        
    //NewtonWorldRayCast( gWorld, p0, p1, filter_cb, userData, prefilter_cb, 0);
    return 0;
}

int destroyShape(lua_State *L)
{
    uint32_t collindex = lua_tonumber(L, 1);
	std::map<uint32_t, JoltShape>::iterator it = gBodies.find(collindex);
    if(it == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    JPH_Shape_Destroy(it->second->body->GetShape());
    gBodies.erase(it);
    lua_pushnumber(L, 1);
    return 1;
}

int createMeshFromShape( lua_State *L )
{
    uint32_t collindex = lua_tonumber(L, 1);
	std::map<uint32_t, JoltShape>::iterator it = gBodies.find(collindex);
    if(it == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }

    uint32_t mapping = lua_tonumber(L, 2);
    const Shape *collision = gBodies[collindex]->body->GetShape();

    //NewtonMesh *mesh = NewtonMeshCreateFromShape( collision );
    // if(mesh) {
    //     uint32_t index = GetId();
    //     gMeshes[index] = mesh;
    //     lua_pushnumber(L, index);

       // NewtonMeshTriangulate(mesh);
       // NewtonMeshCalculateVertexNormals(mesh, 0.3f);

//         dMatrix aligmentUV(dGetIdentityMatrix());
//         if(mapping == 1)
//             NewtonMeshApplySphericalMapping(mesh, 0, &aligmentUV[0][0]);
//         else if(mapping == 2)
//             NewtonMeshApplyCylindricalMapping(mesh, 0, 0, &aligmentUV[0][0]);
//         else
//             NewtonMeshApplyBoxMapping(mesh, 0, 0, 0, &aligmentUV[0][0]);
//         
//         int faceCount = NewtonMeshGetTotalFaceCount (mesh); 
//         int indexCount = NewtonMeshGetTotalIndexCount (mesh); 
//         int pointCount = NewtonMeshGetPointCount (mesh);
//         int vertexStride = NewtonMeshGetVertexStrideInByte(mesh) / sizeof (float);
// 
//         // extract vertex data  from the newton mesh		
//         float *vertices = new float[3 * indexCount];
//         float *normals = new float[3 * indexCount];
//         float *uvs = new float[2 * indexCount];
// 
//         memset (vertices, 0, 3 * indexCount * sizeof (float));
//         memset (normals, 0, 3 * indexCount * sizeof (float));
//         memset (uvs, 0, 2 * indexCount * sizeof (float));
//        
//         int* faceArray = new int [faceCount];
//         void** indexArray = new void* [indexCount];
//         int* materialIndexArray = new int [faceCount];
//         int* remapedIndexArray = new int [indexCount];
//                  
//         NewtonMeshGetFaces (mesh, faceArray, materialIndexArray, indexArray); 
//         NewtonMeshGetUV0Channel(mesh, 2 * sizeof (float), uvs);
//         NewtonMeshGetNormalChannel(mesh, 3 * sizeof (float), normals);
// 
//         float *mappedNormals = new float[3 * indexCount];
//         
//         for (int i = 0; i < indexCount; i ++) {
// 
//             // int fcount = faceArray[i];
//             int index = NewtonMeshGetVertexIndex (mesh, indexArray[i]);            
//             //int pindex = NewtonMeshGetPointIndex(mesh, indexArray[i]);
// 
//             mappedNormals[i * 3] = normals[index * 3];
//             mappedNormals[i * 3+1] = normals[index * 3 + 1];
//             mappedNormals[i * 3+2] = normals[index * 3 + 2];
//                         
//             remapedIndexArray[i] = index;
//          }
//         
//         AddTableIndices(L, indexCount, remapedIndexArray);
//         AddTableVertices(L, NewtonMeshGetVertexCount(mesh)*4, NewtonMeshGetVertexArray(mesh));
// 
//         AddTableUVs( L, indexCount * 2, uvs );
//         AddTableNormals( L, indexCount * 3, mappedNormals );

        // Cleanup
        // delete [] faceArray;
        // delete [] indexArray;
        // delete [] materialIndexArray;
        // delete [] remapedIndexArray;

    //     return 5;
    // }
    // else {
        lua_pushnil(L);
        return 1;
    // }
}