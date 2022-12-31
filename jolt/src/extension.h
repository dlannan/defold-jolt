

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <vector>
#include <map>

#ifndef _EXTENSION_JOLT_HEADER_
#define _EXTENSION_JOLT_HEADER_

#include <joltc.h>

extern JPH_PhysicsSystem* gWorld;
extern JPH_BodyInterface* gInterface;

typedef struct JoltCollision {
    union {
        JPH_BoxShape *                      box;
        JPH_SphereShape *                   sphere;
        JPH_TriangleShapeSettings *         tri;
        JPH_CapsuleShapeSettings *          cap;
        JPH_CylinderShapeSettings *         cyl;
        JPH_ConvexHullShapeSettings *       cvHull;
        JPH_MeshShapeSettings *             mesh;
        JPH_HeightFieldShapeSettings *      hMap;
        JPH_TaperedCapsuleShapeSettings *   taper;
    } shape;

} JoltCollision;


extern std::map<uint32_t, JPH_BodyID >     gBodies;
extern std::map<uint32_t, JoltCollision>  gColls;
extern std::map<uint32_t, JoltMesh* >      gMeshes;

//extern std::map<uint32_t, int>   bodyUserData;
//extern std::map<NewtonBody* , int>   bodyCallback;

extern lua_State *gCbL;

extern uint32_t GetId();

extern int SetTableVector( lua_State *L, dFloat *data, const char *name );
extern void AddTableIndices( lua_State *L, int count, int *indices );
extern void AddTableVertices( lua_State *L, int count, const double *vertices );
extern void AddTableUVs( lua_State *L, int count, const dFloat *uvs );
extern void AddTableNormals( lua_State *L, int count, const dFloat *normals );

#endif // _EXTENSION_NEWTON_HEADER_