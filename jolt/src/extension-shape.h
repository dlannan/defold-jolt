

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <vector>
#include <map>

#ifndef _EXTENSION_JOLT_COLLISION_HEADER_
#define _EXTENSION_JOLT_COLLISION_HEADER_

// External collision 
extern int addShapeSphere( lua_State * L );
extern int addShapePlane( lua_State * L ); 
extern int addShapeCube( lua_State * L );
extern int addShapeCone( lua_State * L );
extern int addShapeCapsule( lua_State * L );
extern int addShapeCylinder( lua_State * L );
extern int addShapeChamferCylinder( lua_State * L );
extern int addShapeConvexHull( lua_State * L );
extern int destroyShape(lua_State *L);

extern int createMeshFromShape( lua_State *L );

extern int worldRayCast( lua_State *L );

#endif // _EXTENSION_JOLT_COLLISION_HEADER_