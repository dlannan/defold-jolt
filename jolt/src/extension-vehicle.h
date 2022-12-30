

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <vector>
#include <map>

#ifndef _EXTENSION_JOLT_VEHICLES_HEADER_
#define _EXTENSION_JOLT_VEHICLES_HEADER_

// External collision 
extern void VehiclesInit();
extern void VehiclesShutdown();
extern int VehicleAdd( lua_State *L );
extern int VehicleRemove( lua_State *L );
extern int VehicleAddTire( lua_State *L );
extern int VehicleFinalize( lua_State *L );

#endif // _EXTENSION_JOLT_VEHICLES_HEADER_