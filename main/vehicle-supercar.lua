local TIRE_SHAPE_SIZE =                12

local VehicleRaycastType =
{
	rctWorld = 0,
	rctConvex = 1,
}

local VehicleTireSteer =
{
	tsNoSteer = 0,
	tsSteer = 1
}

local VehicleSteerSide =
{
	tsSteerSideA = -1,
	tsSteerSideB = 1
}

local VehicleTireAccel =
{
	tsNoAccel = 0,
	tsAccel = 1
}

local VehicleTireBrake =
{
	tsNoBrake = 0,
	tsBrake = 1
}

local VehicleTireSide =
{
	tsTireSideA = 0,
	tsTireSideB = 1
}

local function VehicleRayCastFilter(body, collisionHit, contact, normal, collisionID, userData, intersetParam)

end 

local function VehicleRayPrefilterCallback(body, collision, userData)

end 

local function TireJointDeleteCallback(me)

end

local RayCastTireInfo =
{
    dFloat m_param;
	dVector m_normal;
	const NewtonBody* m_me;
	const NewtonBody* m_hitBody;
	int m_contactID;
};
