#pragma once

#define TRACE_LENGTH 80000.f

UENUM(BlueprintType)
enum class ERangedWeaponTypes : uint8
{
	ERWT_AssaultRifle	 UMETA(DisplayName = "Assault Rifle"),
	ERWT_RocketLauncher  UMETA(DisplayName = "Rocket Launcher"),
	ERWT_Pistol			 UMETA(DisplayName = "Pistol"),
	ERWT_SubmachineGun	 UMETA(DisplayName = "SubMachineGun"),
	ERWT_Shotgun		 UMETA(DisplayName = "Shotgun"),
	ERWT_SniperRifle	 UMETA(DisplayName = "SniperRifle"),
	ERWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),

	ERWT_MAX			 UMETA(DisplayName = "MAX")
};