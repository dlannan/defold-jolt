

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <Newton.h>
#include <vector>
#include <map>

#include <dMatrix.h>

#ifndef _EXTENSION_NEWTON_BODY_HEADER_
#define _EXTENSION_NEWTON_BODY_HEADER_

// External Body
extern int addBody( lua_State *L );
extern int bodyGetMass( lua_State *L );
extern int bodySetMassProperties( lua_State *L );
extern int bodyGetUserData( lua_State *L );
extern int bodySetUserData( lua_State *L );
extern int bodySetLinearDamping( lua_State *L );
extern int bodySetAngularDamping( lua_State *L );
extern int bodySetMassMatrix( lua_State *L );
extern int bodyGetCentreOfMass( lua_State *L );
extern int bodySetForceAndTorqueCallback( lua_State *L );

#endif // _EXTENSION_NEWTON_BODY_HEADER_