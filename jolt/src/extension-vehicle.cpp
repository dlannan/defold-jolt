
/* Copyright (c) <2003-2019> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#include "extension.h"
#include "extension-vehicle.h"


static dVehicleManager* gManager = NULL;
std::map<uint32_t, dMultiBodyVehicle *> gVehicles;

void VehiclesInit() {
    gManager = new dVehicleManager( gWorld );
	gVehicles.clear();
}

void VehiclesShutdown() {
	gVehicles.clear();
    delete gManager;
}

int VehicleAdd( lua_State *L ) {

    uint32_t bodyindex = lua_tonumber(L, 1);
	std::map<uint32_t, NewtonBody *>::iterator bodyit = gBodies.find(bodyindex);
    if(bodyit == gBodies.end()) {
        lua_pushnil(L);
        return 1;
    }

    dMatrix frame(dGetIdentityMatrix());
    frame.m_posit.m_x = lua_tonumber(L, 2);
    frame.m_posit.m_y = lua_tonumber(L, 3);
    frame.m_posit.m_z = lua_tonumber(L, 4);

    NewtonBody *body = gBodies[bodyindex];

    dMultiBodyVehicle *shape  = new dMultiBodyVehicle( body, frame, 9.8 );
    gManager->AddRoot(shape);

	uint32_t index = GetId();
    gVehicles[index] = shape;
    lua_pushnumber(L, index);
    return 1;
}

int VehicleAddTire( lua_State *L ) {

    uint32_t vehicleindex = lua_tonumber(L, 1);
	std::map<uint32_t, dMultiBodyVehicle *>::iterator it = gVehicles.find(vehicleindex);
    if(it == gVehicles.end())
	{
        lua_pushnil(L);
        return 1;
    }
    dMultiBodyVehicle *vehicle = it->second;

    dFloat width = (dFloat)lua_tonumber(L, 2);
    dFloat radius = (dFloat)lua_tonumber(L, 3);

    dMatrix mtx(dGetIdentityMatrix());
    mtx.m_posit.m_x = lua_tonumber(L, 4);
    mtx.m_posit.m_y = lua_tonumber(L, 5);
    mtx.m_posit.m_z = lua_tonumber(L, 6);

    dFloat gravity = 9.8f;
    dFloat vehicleMass = 1000.0f;

    dTireInfo tireInfo;
    tireInfo.m_mass = 20.0f;
    tireInfo.m_radio = radius;
    // for debugging
    //tireInfo.m_radio *= 1.25f; 
    tireInfo.m_width = width;
    tireInfo.m_pivotOffset = 0.01f;
    tireInfo.m_steerRate = 0.5f * dPi;
    tireInfo.m_frictionCoefficient = 0.8f;
    tireInfo.m_maxSteeringAngle = 40.0f * dDegreeToRad;
    tireInfo.m_suspensionLength = 0.3f;
    tireInfo.m_dampingRatio = 15.0f * vehicleMass;
    tireInfo.m_springStiffness = dAbs(vehicleMass * gravity * 8.0f/ tireInfo.m_suspensionLength);
    tireInfo.m_corneringStiffness = dAbs(vehicleMass * gravity * 10.0f);
    tireInfo.m_longitudinalStiffness = dAbs(vehicleMass * gravity * 1.0f);

    dMultiBodyVehicleTire *tire = vehicle->AddTire( mtx, tireInfo );

    dVehicleBrakeControl* const brakeControl = vehicle->GetBrakeControl();
    brakeControl->SetBrakeTorque(1000.0f);
    brakeControl->AddTire(tire);

    uint32_t index = GetId();
    gColls[index] = tire->GetCollisionShape();
    lua_pushnumber(L, index);
    return 1;
}

int VehicleFinalize( lua_State *L )
{
    uint32_t vehicleindex = lua_tonumber(L, 1);
	std::map<uint32_t, dMultiBodyVehicle *>::iterator it = gVehicles.find(vehicleindex);
    if(it == gVehicles.end())
	{
        lua_pushnil(L);
        return 1;
    }
    dMultiBodyVehicle *vehicle = it->second;
    vehicle->Finalize();
    lua_pushnumber(L, 1);
    return 1;
}

int VehicleRemove( lua_State *L ) {

    int index = lua_tonumber(L, 1);
	std::map<uint32_t, dMultiBodyVehicle *>::iterator it = gVehicles.find(index);
    if(it != gVehicles.end())
	{
        gManager->RemoveRoot(it->second);
        delete it->second;
        lua_pushnumber(L, 1);
    } else {
        printf("[Newton] Vehicle index %d not found.\n", index);
        lua_pushnil(L);
    }
    return 1;
}