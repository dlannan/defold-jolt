

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <Newton.h>
#include <vector>
#include <map>

#include <dMatrix.h>

#ifndef _EXTENSION_NEWTON_COLLISION_HEADER_
#define _EXTENSION_NEWTON_COLLISION_HEADER_

// External collision 
extern int addCollisionSphere( lua_State * L );
extern int addCollisionPlane( lua_State * L ); 
extern int addCollisionCube( lua_State * L );
extern int addCollisionCone( lua_State * L );
extern int addCollisionCapsule( lua_State * L );
extern int addCollisionCylinder( lua_State * L );
extern int addCollisionChamferCylinder( lua_State * L );
extern int addCollisionConvexHull( lua_State * L );
extern int destroyCollision(lua_State *L);

extern int createMeshFromCollision( lua_State *L );

extern int worldRayCast( lua_State *L );

#endif // _EXTENSION_NEWTON_COLLISION_HEADER_