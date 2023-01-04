

#ifndef _EXTENSION_JOLT_HEADER_
#define _EXTENSION_JOLT_HEADER_

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <vector>
#include <map>

#include <joltc.h>

extern JPH_PhysicsSystem* gWorld;
extern JPH_BodyInterface* gInterface;

typedef struct JoltBody {
    JPH_BodyID  id;
    JPH_Body *  body;
    JPH_Shape * shape;
    float       mass;
} JoltBody;

typedef struct JoltMesh {
    JPH_Triangle    triangles;
    uint32_t        count;
} JoltMesh;

extern std::map<uint32_t, JoltBody* >      gBodies;
extern std::map<uint32_t, JoltMesh* >      gMeshes;

//extern std::map<uint32_t, int>   bodyUserData;
//extern std::map<NewtonBody* , int>   bodyCallback;

extern lua_State *gCbL;

extern uint32_t GetId();

extern int SetTableVector( lua_State *L, float *data, const char *name );
extern void AddTableIndices( lua_State *L, int count, int *indices );
extern void AddTableVertices( lua_State *L, int count, const double *vertices );
extern void AddTableUVs( lua_State *L, int count, const float *uvs );
extern void AddTableNormals( lua_State *L, int count, const float *normals );

#endif // _EXTENSION_NEWTON_HEADER_