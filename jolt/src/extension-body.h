

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <vector>
#include <map>

#ifndef _EXTENSION_JOLT_BODY_HEADER_
#define _EXTENSION_JOLT_BODY_HEADER_

// External Body
extern int bodyGetMass( lua_State *L );
extern int bodySetMassProperties( lua_State *L );
extern int bodyGetUserData( lua_State *L );
extern int bodySetUserData( lua_State *L );
extern int bodySetRestitution( lua_State *L );
extern int bodySetFriction( lua_State *L );
extern int bodySetLinearVelocity( lua_State *L );
extern int bodySetAngularVelocity( lua_State *L );
extern int bodyIsActive( lua_State *L );
extern int bodyIsStatic( lua_State *L );
extern int bodyIsKinematic( lua_State *L );
extern int bodyIsDynamic( lua_State *L );
extern int bodyIsSensor( lua_State *L );
extern int bodyGetShape( lua_State *L );
extern int bodyGetPosition( lua_State *L );
extern int bodyGetRotation( lua_State *L );
extern int bodySetPosition( lua_State *L );
extern int bodySetRotation( lua_State *L );
extern int bodyGetCentreOfMass( lua_State *L );
extern int bodySetForceAndTorqueCallback( lua_State *L );
extern int createMeshFromShape( lua_State *L );

#endif // _EXTENSION_JOLT_BODY_HEADER_