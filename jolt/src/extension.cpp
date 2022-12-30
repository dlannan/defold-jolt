// myextension.cpp
// Extension lib defines
#define LIB_NAME "Jolt"
#define MODULE_NAME "jolt"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <stdlib.h>
#include <vector>
#include <map>

#include <extension.h>

JPH_PhysicsSystem* gWorld = NULL;
JPH_BodyInterface* gInterface = NULL;

std::map<uint32_t, JPH_BodyID >     gBodies;
std::map<uint32_t, JoltCollision*>  gColls;
std::map<uint32_t, JoltMesh* >      gMeshes;

std::map<uint32_t, int>             bodyUserData;
std::map<JoltBody* , int>           bodyCallback;

lua_State *gCbL = NULL;

// utils
int SetTableVector( lua_State *L, dFloat *data, const char *name );
void CollisionShutdown();
void BodyShutdown();

static void Close();

static int Create( lua_State *L )
{
    // Cleanup if this is called again during a run.
    Close();

    // Print the library version.
    printf("[Jolt] Version %d\n", JoltWorldGetVersion());

    JPH_Init();

    // Create the Jolt world.
    gWorld = JPH_PhysicsSystem_Create();
    gInterface = JPH_PhysicsSystem_GetBodyInterface(gWorld);

    VehiclesInit();
    return 0;
}

static int Update( lua_State *L )
{
    double timestep = luaL_checknumber(L, 1);

    // Callbacks use this state to run in - not sure how good/bad/crazy this is
    gCbL = L;

    if(gWorld) JoltUpdate(gWorld, (float)timestep);

    lua_newtable(L);
    
    std::map<uint32_t, JoltBody*>::iterator bodyit = gBodies.begin();
    for ( ; bodyit != gBodies.end(); ++bodyit ) 
    {       
        JoltBody *body = bodyit->second;

        // After update, build the table and set all the pos and quats.
        dFloat rot[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        JoltBodyGetRotation(body, rot);

        dFloat pos[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        JoltBodyGetPosition(body, pos);

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
    std::map<uint32_t, JoltMesh*>::iterator meshit = gMeshes.begin();
    for ( ; meshit != gMeshes.end(); ++meshit )
        JoltMeshDestroy(meshit->second);
    std::map<uint32_t, JoltCollision*>::iterator collit = gColls.begin();
    for ( ; collit != gColls.end(); ++collit )
        JoltDestroyCollision(collit->second);
    // std::map<uint32_t, JoltBody*>::iterator bodyit = gBodies.begin();
    // for ( ; bodyit != gBodies.end(); ++bodyit )        
    //     JoltDestroyBody(bodyit->second);

    //gColls.clear();
    //gBodies.clear();
    //gMeshes.clear();

    if(gWorld) JoltDestroy(gWorld);
    gWorld = NULL;

    JPH_Shutdown();
}


// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"create", Create}, 
    {"update", Update}, 
    
    {"collision_addplane", addCollisionPlane },
    {"collision_addcube", addCollisionCube },
    {"collision_addsphere", addCollisionSphere },
    {"collision_addcone", addCollisionCone },
    {"collision_addcapsule", addCollisionCapsule },
    {"collision_addcylinder", addCollisionCylinder },
    {"collision_addchamfercylinder", addCollisionChamferCylinder },
    {"collision_addconvexhull", addCollisionConvexHull },
    {"collision_destroy", destroyCollision },

    {"body_add", addBody },
    {"body_getmass", bodyGetMass },
    {"body_setmassproperties", bodySetMassProperties },
    {"body_getuserdata", bodyGetUserData },
    {"body_setuserdata", bodySetUserData },
    {"body_setlineardamping", bodySetLinearDamping },
    {"body_setangulardamping", bodySetAngularDamping },
    {"body_setmassmatrix", bodySetMassMatrix },
    {"body_getcentreofmass", bodyGetCentreOfMass },
    {"body_setforceandtorquecallback", bodySetForceAndTorqueCallback },

    {"createmeshfromcollision", createMeshFromCollision },

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
