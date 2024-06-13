// @2023 All rights reversed by Reverse-Alpha Studios


#include "Vehicle/Wheel/VehicleWheelRear.h"

UVehicleWheelRear::UVehicleWheelRear()
{
	AxleType			 = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine	 = true;
}