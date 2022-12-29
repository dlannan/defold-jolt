
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

int addCollisionSphere( lua_State * L ) {

    double radii = lua_tonumber(L, 1);
    // Collision shapes: sphere (our ball), and large box (our ground plane).
    NewtonCollision* cs_object = NewtonCreateSphere(gWorld, radii, Shape_Sphere, NULL);
    uint32_t index = GetId();
    gColls[index] = cs_object;
    lua_pushnumber(L, index);
    return 1;
}

 int addCollisionPlane( lua_State * L ) {

    double width = lua_tonumber(L, 1);
    double depth = lua_tonumber(L, 2);
    // Collision shapes: sphere (our ball), and large box (our ground plane).
    NewtonCollision* cs_object = NewtonCreateBox(gWorld, width, 0.1, depth, Shape_Plane, NULL);
    uint32_t index = GetId();
    gColls[index] = cs_object;
    lua_pushnumber(L,index);
    return 1;
}

int addCollisionCube( lua_State * L ) {

    double sx = lua_tonumber(L, 1);
    double sy = lua_tonumber(L, 2);
    double sz = lua_tonumber(L, 3);
    NewtonCollision* cs_object = NewtonCreateBox(gWorld, sx, sy, sz, Shape_Cube, NULL);
    uint32_t index = GetId();
    gColls[index] = cs_object;
    lua_pushnumber(L, index);
    return 1;
}

int addCollisionCone( lua_State * L ) {

    double radius = lua_tonumber(L, 1);
    double height = lua_tonumber(L, 2);
    NewtonCollision* cs_object = NewtonCreateCone(gWorld, radius, height, Shape_Cone, NULL);
    uint32_t index = GetId();
    gColls[index] = cs_object;
    lua_pushnumber(L, index);
    return 1;
}

int addCollisionCapsule( lua_State * L ) {

    double r0 = lua_tonumber(L, 1);
    double r1 = lua_tonumber(L, 2);
    double height = lua_tonumber(L, 3);
    NewtonCollision* cs_object = NewtonCreateCapsule(gWorld, r0, r1, height, Shape_Capsule, NULL);
    uint32_t index = GetId();
    gColls[index] = cs_object;
    lua_pushnumber(L, index);
    return 1;
}

int addCollisionCylinder( lua_State * L ) {

    double r0 = lua_tonumber(L, 1);
    double r1 = lua_tonumber(L, 2);
    double height = lua_tonumber(L, 3);
    NewtonCollision* cs_object = NewtonCreateCylinder(gWorld, r0, r1, height, Shape_Cylinder, NULL);
    uint32_t index = GetId();
    gColls[index] = cs_object;
    lua_pushnumber(L, index);
    return 1;
}

int addCollisionChamferCylinder( lua_State * L ) {

    double radius = lua_tonumber(L, 1);
    double height = lua_tonumber(L, 2);
    NewtonCollision* cs_object = NewtonCreateChamferCylinder(gWorld, radius, height,Shape_ChamferCylinder, NULL);
    uint32_t index = GetId();
    gColls[index] = cs_object;
    lua_pushnumber(L, index);
    return 1;
}

int addCollisionConvexHull( lua_State * L ) {

    double count = lua_tonumber(L, 1);
    int stride = lua_tonumber(L, 2);
    double tolerance = lua_tonumber(L, 3);
    const float *vertCloud = (float *)lua_topointer(L, 4);
    NewtonCollision* cs_object = NewtonCreateConvexHull(gWorld, count, vertCloud, stride, tolerance, Shape_ConvexHull, NULL);
    uint32_t index = GetId();
    gColls[index] = cs_object;
    lua_pushnumber(L, index);
    return 1;
}

 int worldRayCast( lua_State *L ) {

    const dFloat *p0= (dFloat *)lua_topointer(L, 1);
    const dFloat *p1 = (dFloat *)lua_topointer(L, 2);
    NewtonWorldRayFilterCallback filter_cb = *(NewtonWorldRayFilterCallback *)lua_topointer(L, 3);
    void * const userData = (void * const)lua_topointer(L, 4);
    NewtonWorldRayPrefilterCallback prefilter_cb = *(NewtonWorldRayPrefilterCallback *)lua_topointer(L, 5);
        
    NewtonWorldRayCast( gWorld, p0, p1, filter_cb, userData, prefilter_cb, 0);
    return 0;
}

int destroyCollision(lua_State *L)
{
    uint32_t collindex = lua_tonumber(L, 1);
	std::map<uint32_t, NewtonCollision*>::iterator it = gColls.find(collindex);
    if(it == gColls.end()) {
        lua_pushnil(L);
        return 1;
    }
    NewtonDestroyCollision(it->second);
    gColls.erase(it);
    lua_pushnumber(L, 1);
    return 1;
}

int createMeshFromCollision( lua_State *L )
{
    uint32_t collindex = lua_tonumber(L, 1);
	std::map<uint32_t, NewtonCollision*>::iterator it = gColls.find(collindex);
    if(it == gColls.end()) {
        lua_pushnil(L);
        return 1;
    }

    uint32_t mapping = lua_tonumber(L, 2);
    const NewtonCollision *collision = gColls[collindex];

    NewtonMesh *mesh = NewtonMeshCreateFromCollision( collision );
    if(mesh) {
        uint32_t index = GetId();
        gMeshes[index] = mesh;
        lua_pushnumber(L, index);

        NewtonMeshTriangulate(mesh);
        NewtonMeshCalculateVertexNormals(mesh, 0.3f);

        dMatrix aligmentUV(dGetIdentityMatrix());
        if(mapping == 1)
            NewtonMeshApplySphericalMapping(mesh, 0, &aligmentUV[0][0]);
        else if(mapping == 2)
            NewtonMeshApplyCylindricalMapping(mesh, 0, 0, &aligmentUV[0][0]);
        else
            NewtonMeshApplyBoxMapping(mesh, 0, 0, 0, &aligmentUV[0][0]);
        
        int faceCount = NewtonMeshGetTotalFaceCount (mesh); 
        int indexCount = NewtonMeshGetTotalIndexCount (mesh); 
        int pointCount = NewtonMeshGetPointCount (mesh);
        int vertexStride = NewtonMeshGetVertexStrideInByte(mesh) / sizeof (dFloat);

        // extract vertex data  from the newton mesh		
        dFloat *vertices = new dFloat[3 * indexCount];
        dFloat *normals = new dFloat[3 * indexCount];
        dFloat *uvs = new dFloat[2 * indexCount];

        memset (vertices, 0, 3 * indexCount * sizeof (dFloat));
        memset (normals, 0, 3 * indexCount * sizeof (dFloat));
        memset (uvs, 0, 2 * indexCount * sizeof (dFloat));
       
        int* faceArray = new int [faceCount];
        void** indexArray = new void* [indexCount];
        int* materialIndexArray = new int [faceCount];
        int* remapedIndexArray = new int [indexCount];
                 
        NewtonMeshGetFaces (mesh, faceArray, materialIndexArray, indexArray); 
        NewtonMeshGetUV0Channel(mesh, 2 * sizeof (dFloat), uvs);
        NewtonMeshGetNormalChannel(mesh, 3 * sizeof (dFloat), normals);

        dFloat *mappedNormals = new dFloat[3 * indexCount];
        
        for (int i = 0; i < indexCount; i ++) {

            // int fcount = faceArray[i];
            int index = NewtonMeshGetVertexIndex (mesh, indexArray[i]);            
            //int pindex = NewtonMeshGetPointIndex(mesh, indexArray[i]);

            mappedNormals[i * 3] = normals[index * 3];
            mappedNormals[i * 3+1] = normals[index * 3 + 1];
            mappedNormals[i * 3+2] = normals[index * 3 + 2];
                        
            remapedIndexArray[i] = index;
         }
        
        AddTableIndices(L, indexCount, remapedIndexArray);
        AddTableVertices(L, NewtonMeshGetVertexCount(mesh)*4, NewtonMeshGetVertexArray(mesh));

        AddTableUVs( L, indexCount * 2, uvs );
        AddTableNormals( L, indexCount * 3, mappedNormals );

        // Cleanup
        delete [] faceArray;
        delete [] indexArray;
        delete [] materialIndexArray;
        delete [] remapedIndexArray;

        return 5;
    }
    else {
        lua_pushnil(L);
        return 1;
    }
}