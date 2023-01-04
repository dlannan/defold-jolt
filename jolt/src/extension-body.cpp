#include "extension.h"
#include "extension-body.h"

#include <joltc.h>

// Define a custom data structure to store a body ID.
struct UserData {
    uint32_t bodyID=0;
};

// void cb_applyForce(const NewtonBody* const body, float timestep, int threadIndex)
// {
//     // Fetch user data and body position.
    //UserData *mydata = (UserData*)NewtonBodyGetUserData(body);
    //float pos[4];
    //NewtonBodyGetPosition(body, pos);

    // Apply force.
    // float force[3] = {0, -9.8, 0};
    // NewtonBodySetForce(body, force);

    // Print info to terminal.
    //printf("BodyID=%d, Sleep=%d, %.2f, %.2f, %.2f\n",
    //mydata->bodyID, NewtonBodyGetSleepState(body), pos[0], pos[1], pos[2]);
// }

int bodyGetMass( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 2);
	std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    
    lua_pushnumber(L, bodyit->second->mass );
    return 1;
}

int bodySetMassProperties( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
	std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end())
    {
        lua_pushnil(L);
        return 1;
    }

    float mass = lua_tonumber(L, 2);
    bodyit->second->mass = mass;
    
    lua_pushnumber(L, 1);
    return 1;
}

int bodyGetUserData( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end())
    {
        lua_pushnil(L);
        return 1;
    }
    uint32_t udata = JPH_Body_GetUserData(bodyit->second->body);
    lua_pushnumber(L, udata);
    return 1;
}

int bodySetUserData( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    uint32_t udata = lua_tonumber(L, 2);
    JPH_Body_SetUserData(bodyit->second->body, udata);
    lua_pushnumber(L, 1);
    return 1;
}

int bodySetRestitution( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }    
    float restitution = lua_tonumber(L, 2);
    JPH_Body_SetRestitution( gBodies[bodyindex]->body, restitution );
    lua_pushnumber(L, 1);
    return 1;
}

int bodySetFriction( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }    
    float friction = (float)lua_tonumber(L, 2);
    JPH_Body_SetFriction( gBodies[bodyindex]->body, friction );
    lua_pushnumber(L, 1);
    return 1;
}

int bodySetLinearVelocity( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }    
    float Ixx = (float)lua_tonumber(L, 2);
    float Iyy = (float)lua_tonumber(L, 3);
    float Izz = (float)lua_tonumber(L, 4);
    JPH_Vec3 velocity = {Ixx, Iyy, Izz};
    JPH_Body_SetLinearVelocity( gBodies[bodyindex]->body,  &velocity );
    lua_pushnumber(L, 1);
    return 1;
}

int bodySetAngularVelocity( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }    
    float Ixx = (float)lua_tonumber(L, 2);
    float Iyy = (float)lua_tonumber(L, 3);
    float Izz = (float)lua_tonumber(L, 4);
    JPH_Vec3 velocity = {Ixx, Iyy, Izz};
    JPH_Body_SetAngularVelocity( gBodies[bodyindex]->body,  &velocity );
    lua_pushnumber(L, 1);
    return 1;
}

int bodyGetCentreOfMass( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }   
    JPH_Vec3 center = JPH_Body_GetCenterOfMassPosition(bodyit->second->body);
    lua_pushnumber(L, center.x);
    lua_pushnumber(L, center.y);
    lua_pushnumber(L, center.z);
    return 3;
}

int bodyIsActive( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    } 
    lua_pushnumber(L, JPH_Body_IsActive(bodyit->second->body));
    return 1;
}

int bodyIsStatic( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    } 
    lua_pushnumber(L, JPH_Body_IsStatic(bodyit->second->body));
    return 1;
}

int bodyIsKinematic( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    } 
    lua_pushnumber(L, JPH_Body_IsKinematic(bodyit->second->body));
    return 1;
}

int bodyIsDynamic( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    } 
    lua_pushnumber(L, JPH_Body_IsDynamic(bodyit->second->body));
    return 1;
}

int bodyIsSensor( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    } 
    lua_pushnumber(L, JPH_Body_IsSensor(bodyit->second->body));
    return 1;
}

// TODO - Extract verts from shape!
int bodyGetShape( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    bodyit->second->shape = JPH_Body_GetShape(bodyit->second->body);
    lua_pushnumber(L, 1);
    return 1;
}

int bodyGetPosition( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    JPH_Vec3 pos = JPH_Body_GetPosition(bodyit->second->body);
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    lua_pushnumber(L, pos.z);
    return 3;
}

int bodyGetRotation( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    JPH_Quat rot = JPH_Body_GetRotation(bodyit->second->body);
    lua_pushnumber(L, rot.x);
    lua_pushnumber(L, rot.y);
    lua_pushnumber(L, rot.z);
    lua_pushnumber(L, rot.w);
    return 4;
}

int bodySetPosition( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);

    JPH_Vec3    pos;
    pos.x = x; pos.y = y; pos.z = z;
    JPH_Body_SetPosition(bodyit->second->body, &pos);
    lua_pushnumber(L, 1);
    return 1;
}

int bodySetRotation( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);
    float w = (float)lua_tonumber(L, 5);
    JPH_Quat rot;
    rot.x = x; rot.y = y; rot.z = z; rot.w = w;
    JPH_Body_SetRotation(bodyit->second->body, &rot);
    lua_pushnumber(L, 1);
    return 1;
}

void __applyForceAndTorqueCallback(const JoltBody *body, float timestep, int threadIndex) 
{
    uint32_t bodyindex = 0;

    std::map<uint32_t, JoltBody *>::iterator bodyit;
    for( bodyit = gBodies.begin(); bodyit != gBodies.end(); ++bodyit ) {
        if(bodyit->second == body) {
            bodyindex = bodyit->first;
            break;
        }
    }

    if( bodyindex == 0 ) {
        printf("error no body index found.");
    }

//     auto result = bodyCallback.find((JPH_BodyID)body);
//     int function_ref = result->second;
//     lua_rawgeti(gCbL, LUA_REGISTRYINDEX, function_ref);
// 
//     lua_pushnumber(gCbL, bodyindex);
//     lua_pushnumber(gCbL, timestep);
//     lua_pushnumber(gCbL, threadIndex);
//     if (lua_pcall(gCbL, 3, 0, 0) != 0 )
//     {
//         printf("error running function `f': %s", lua_tostring(gCbL, -1));
//     }
}

int bodySetForceAndTorqueCallback( lua_State *L )
{
    uint32_t bodyindex = lua_tonumber(L, 1);
    std::map<uint32_t, JoltBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }   
//    int function_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    //bodyCallback[(NewtonBody *)gBodies[bodyindex]] = function_ref;
    //NewtonBodySetForceAndTorqueCallback( gBodies[bodyindex], __applyForceAndTorqueCallback);
    lua_pushnumber(L, 1);
    return 1;
}
