// myextension.cpp
// Extension lib defines
#define LIB_NAME "Jolt"
#define MODULE_NAME "jolt"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <joltc.h>
#include <vector>
#include <map>

#include "extension.h"
#include "extension-shape.h"

JPH_PhysicsSystem* gWorld = NULL;
JPH_BodyInterface* gInterface = NULL;

std::map<uint32_t, JoltBody* >      gBodies;
std::map<uint32_t, JoltMesh* >      gMeshes;

std::map<uint32_t, int>             bodyUserData;
std::map<JoltBody *, int>           bodyCallback;

lua_State *gCbL = NULL;

// utils
int SetTableVector( lua_State *L, float *data, const char *name );
void CollisionShutdown();
void BodyShutdown();

static void Close();

static int Create( lua_State *L )
{
    // Cleanup if this is called again during a run.
    Close();

    // Print the library version.
    printf("[Jolt] Initialized.\n");

    JPH_Init();

    // Create the Jolt world.
    gWorld = JPH_PhysicsSystem_Create();
//    BPLayerInterfaceImpl broadPhaseLayer = new();

//    JPH_PhysicsSystem_Init(gWorld, maxBodies, numBodyMutexes, maxBodyPairs, maxConstraints, layer, layerFilter, pairFilter);
    gInterface = JPH_PhysicsSystem_GetBodyInterface(gWorld);

//    VehiclesInit();
    return 0;
}

static int Update( lua_State *L )
{
    double timestep = luaL_checknumber(L, 1);

    // Callbacks use this state to run in - not sure how good/bad/crazy this is
    gCbL = L;

    if(gWorld) JPH_PhysicsSystem_Update(gWorld, (float)timestep, 100, 2, NULL, NULL);

    lua_newtable(L);
    
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.begin();
    for ( ; bodyit != gBodies.end(); ++bodyit ) 
    {       
        JPH_Body * body = bodyit->second->body;

        // After update, build the table and set all the pos and quats.
        JPH_Quat jrot = JPH_Body_GetRotation(body);
        float rot[4] = {jrot.x, jrot.y, jrot.z, jrot.w};
        
        JPH_Vec3 jpos = JPH_Body_GetPosition(body, pos);
        float pos[4] = {jpos.x, jpos.y, jpos.z, 0.0f};
        
        lua_pushnumber(L, bodyit->first); 
        lua_newtable(L);
        
        SetTableVector(L, pos, "pos");
        SetTableVector(L, rot, "rot");
        lua_settable(L, -3);
    }

    return 1;
}

void Close( void )
{
    // Clean up.
    // std::map<uint32_t, JoltMesh*>::iterator meshit = gMeshes.begin();
    // for ( ; meshit != gMeshes.end(); ++meshit )
    //     JoltMeshDestroy(meshit->second);
    // std::map<uint32_t, JoltCollision>::iterator collit = gShapes.begin();
    // for ( ; collit != gShapes.end(); ++collit )
    //     JoltDestroyCollision(collit->second);
    // std::map<uint32_t, JoltBody*>::iterator bodyit = gBodies.begin();
    // for ( ; bodyit != gBodies.end(); ++bodyit )        
    //     JoltDestroyBody(bodyit->second);

    gBodies.clear();
    gMeshes.clear();

    if(gWorld) JPH_PhysicsSystem_Destroy(gWorld);
    gWorld = NULL;

    JPH_Shutdown();
}


// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"create", Create}, 
    {"update", Update}, 
    
    {"shape_addplane", addShapePlane },
    {"shape_addcube", addShapeCube },
    {"shape_addsphere", addShapeSphere },
    {"shape_addcone", addShapeCone },
    {"shape_addcapsule", addShapeCapsule },
    {"shape_addcylinder", addShapeCylinder },
    {"shape_addchamfercylinder", addShapeChamferCylinder },
    {"shape_addconvexhull", addShapeConvexHull },
    {"shape_destroy", destroyShape },

    {"body_getmass", bodyGetMass },
    {"body_setmassproperties", bodySetMassProperties },
    {"body_getuserdata", bodyGetUserData },
    {"body_setuserdata", bodySetUserData },
    {"body_setrestitution", bodySetRestitution },
    {"body_setfriction", bodySetFriction },
    {"body_setlinearvelocity", bodySetLinearVelocity },
    {"body_setangularvelocity", bodySetAngularVelocity },

    {"body_isactive", bodyIsActive },
    {"body_isstatic", bodyIsStatic },
    {"body_iskinematic", bodyIsKinematic },
    {"body_isdynamic", bodyIsDynamic },
    {"body_issensor", bodyIsSensor },

    {"body_getshape", bodyGetShape },
    {"body_getposition", bodyGetPosition },
    {"body_getrotation", bodyGetRotation },
    {"body_setposition", bodySetPosition },
    {"body_setrotation", bodySetRotation },

    {"body_getcentreofmass", bodyGetCentreOfMass },
    {"body_setforceandtorquecallback", bodySetForceAndTorqueCallback },

    {"createmeshfromshape", createMeshFromShape },

    {"vehicle_add", VehicleAdd },
    {"vehicle_addtire", VehicleAddTire },
    {"vehicle_finalize", VehicleFinalize },
    {"vehicle_remove", VehicleRemove },
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeJoltExtension(dmExtension::AppParams* params)
{
    dmLogInfo("AppInitializeJoltExtension\n");
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeJoltExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    dmLogInfo("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeJoltExtension(dmExtension::AppParams* params)
{
    dmLogInfo("AppFinalizeJoltExtension\n");
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeJoltExtension(dmExtension::Params* params)
{
    dmLogInfo("FinalizeJoltExtension\n");

    VehiclesShutdown();
    Close();
    return dmExtension::RESULT_OK;
}

dmExtension::Result OnUpdateJoltExtension(dmExtension::Params* params)
{
    // dmLogInfo("OnUpdateJoltExtension\n");
    return dmExtension::RESULT_OK;
}

void OnEventJoltExtension(dmExtension::Params* params, const dmExtension::Event* event)
{
    switch(event->m_Event)
    {
        case dmExtension::EVENT_ID_ACTIVATEAPP:
            dmLogInfo("OnEventJoltExtension - EVENT_ID_ACTIVATEAPP\n");
            break;
        case dmExtension::EVENT_ID_DEACTIVATEAPP:
            dmLogInfo("OnEventJoltExtension - EVENT_ID_DEACTIVATEAPP\n");
            break;
        case dmExtension::EVENT_ID_ICONIFYAPP:
            dmLogInfo("OnEventJoltExtension - EVENT_ID_ICONIFYAPP\n");
            break;
        case dmExtension::EVENT_ID_DEICONIFYAPP:
            dmLogInfo("OnEventJoltExtension - EVENT_ID_DEICONIFYAPP\n");
            break;
        default:
            dmLogWarning("OnEventJoltExtension - Unknown event id\n");
            break;
    }
}

// Defold SDK uses a macro for setting up extension entry points:
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)

// JoltExtension is the C++ symbol that holds all relevant extension data.
// It must match the name field in the `ext.manifest`
DM_DECLARE_EXTENSION(Jolt, LIB_NAME, AppInitializeJoltExtension, AppFinalizeJoltExtension, InitializeJoltExtension, OnUpdateJoltExtension, OnEventJoltExtension, FinalizeJoltExtension)
