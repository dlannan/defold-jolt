#include "extension.h"
#include "extension-body.h"


// Define a custom data structure to store a body ID.
struct UserData {
    uint32_t bodyID=0;
};

void cb_applyForce(const NewtonBody* const body, dFloat timestep, int threadIndex)
{
    // Fetch user data and body position.
    //UserData *mydata = (UserData*)NewtonBodyGetUserData(body);
    //dFloat pos[4];
    //NewtonBodyGetPosition(body, pos);

    // Apply force.
    // dFloat force[3] = {0, -9.8, 0};
    // NewtonBodySetForce(body, force);

    // Print info to terminal.
    //printf("BodyID=%d, Sleep=%d, %.2f, %.2f, %.2f\n",
    //mydata->bodyID, NewtonBodyGetSleepState(body), pos[0], pos[1], pos[2]);
}


 int addBody( lua_State *L ) {

    // Neutral transform matrix.
    float	tm[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t idx = lua_tonumber(L, 1);
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);
    double z = lua_tonumber(L, 4);
    double mass = lua_tonumber(L, 5);

//     tm[12] = x; tm[13] = y; tm[14] = z;
//     // NewtonBody *body = NewtonCreateDynamicBody(gWorld, gColls[idx], tm);
//     JPH_BodyID body = JPH_BodyInterface_CreateAndAddBody();
// 
//     gBodies[idx] = body;
// 
//     // NewtonBodySetForceAndTorqueCallback(body, cb_applyForce);
// 
//     // Assign non-zero mass to sphere to make it dynamic.
//     NewtonBodySetMassMatrix(body, mass, 1, 1, 1);
//     
//     UserData *myData = new UserData[2];
//     myData[0].bodyID = idx;
//     NewtonBodySetUserData(body, (void *)&myData[0]);
    
    lua_pushnumber(L, idx);
    return 1;
}

 int bodyGetMass( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 2);
	std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    
    dFloat mass = 0.0f;
    dFloat Ixx = 0.0f;
    dFloat Iyy = 0.0f;
    dFloat Izz = 0.0f;
    // NewtonBodyGetMass( gBodies[bodyindex], &mass, &Ixx, &Iyy, &Izz);
    lua_pushnumber(L, mass );
    lua_pushnumber(L, Ixx );
    lua_pushnumber(L, Iyy );
    lua_pushnumber(L, Izz );
    return 4;
}

int bodySetMassProperties( lua_State *L )
{
    uint32_t collindex = lua_tonumber(L, 1);
    std::map<uint32_t, NewtonCollision *>::iterator collit = gColls.find(collindex);
    if(collit == gColls.end())
	{
        lua_pushnil(L);
        return 1;
    }
    uint32_t bodyindex = lua_tonumber(L, 2);
	std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end())
    {
        lua_pushnil(L);
        return 1;
    }

    float mass = lua_tonumber(L, 3);

    // NewtonBodySetMassProperties( bodyit->second, mass, collit->second );
    lua_pushnumber(L, 1);
    return 1;
}

int bodyGetUserData( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end())
    {
        lua_pushnil(L);
        return 1;
    }

    // int *tablerefptr = NewtonBodyGetUserData(gBodies[bodyindex]);
    lua_rawgeti(L, LUA_REGISTRYINDEX, bodyUserData[bodyindex]);
    return 1;
}

int bodySetUserData( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }

    // Grab the ref to the table passed in as the second arg
    int tableref = luaL_ref(L, LUA_REGISTRYINDEX);
//     std::pair<uint32_t, int> tuple(bodyindex, tableref);
//     bodyUserData.insert(tuple);
// 
    //NewtonBodySetUserData(gBodies[bodyindex], &tuple.second());
    lua_pushnumber(L, 1);
    return 1;
}

int bodySetLinearDamping( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }    
    float damping = lua_tonumber(L, 2);
    // NewtonBodySetLinearDamping( gBodies[bodyindex], damping );
    lua_pushnumber(L, 1);
    return 1;
}

int bodySetAngularDamping( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }    
    dFloat damping[4] = { (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4), (float)lua_tonumber(L, 5) };
    //NewtonBodySetAngularDamping( gBodies[bodyindex], (dFloat *)&damping );
    lua_pushnumber(L, 1);
    return 1;
}

int bodySetMassMatrix( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }    
    dFloat mass = (dFloat)lua_tonumber(L, 2);
    dFloat Ixx = (dFloat)lua_tonumber(L, 3);
    dFloat Iyy = (dFloat)lua_tonumber(L, 4);
    dFloat Izz = (dFloat)lua_tonumber(L, 5);
    //NewtonBodySetMassMatrix( gBodies[bodyindex],  mass, Ixx, Iyy, Izz );
    lua_pushnumber(L, 1);
    return 1;
}

int bodyGetCentreOfMass( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }   
    float center[3];
   // NewtonBodyGetCentreOfMass( gBodies[bodyindex], center);

    lua_pushnumber(L, center[0]);
    lua_pushnumber(L, center[1]);
    lua_pushnumber(L, center[2]);
    return 3;
}

void __applyForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex) 
{
    uint32_t bodyindex = 0;

    std::map<uint32_t, NewtonBody *>::iterator bodyit;
    for( bodyit = gBodies.begin(); bodyit != gBodies.end(); ++bodyit ) {
        if(bodyit->second == body) {
            bodyindex = bodyit->first;
            break;
        }
    }

    if( bodyindex == 0 ) {
        printf("error no body index found.");
    }

    auto result = bodyCallback.find((NewtonBody *)body);
    int function_ref = result->second;
    lua_rawgeti(gCbL, LUA_REGISTRYINDEX, function_ref);

    lua_pushnumber(gCbL, bodyindex);
    lua_pushnumber(gCbL, timestep);
    lua_pushnumber(gCbL, threadIndex);
    if (lua_pcall(gCbL, 3, 0, 0) != 0 )
    {
        printf("error running function `f': %s", lua_tostring(gCbL, -1));
    }
}

int bodySetForceAndTorqueCallback( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }   
    int function_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    //bodyCallback[(NewtonBody *)gBodies[bodyindex]] = function_ref;
    //NewtonBodySetForceAndTorqueCallback( gBodies[bodyindex], __applyForceAndTorqueCallback);
    lua_pushnumber(L, 1);
    return 1;
}
