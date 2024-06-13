// @2023 All rights reversed by Reverse-Alpha Studios

#include "Vehicle/Wheel/VehicleWheelFront.h"

UVehicleWheelFront::UVehicleWheelFront()
{
	AxleType			= EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle		= 40.0f;
}
