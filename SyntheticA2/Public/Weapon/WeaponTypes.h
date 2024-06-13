#pragma once

#define MAX_TRACE_LENGTH 80000.f

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Pistol			UMETA(DisplayName = "Pistol"),
	EWT_Shotgun			UMETA(DisplayName = "Shotgun"),
	EWT_SMG				UMETA(DisplayName = "SubmachineGun"),
	EWT_AssaultRifle	UMETA(DisplayName = "Assault Rifle"),
	EWT_SniperRifle		UMETA(DisplayName = "Sniper Rifle"),
	EWT_GrenadeLauncher	UMETA(DisplayName = "Grenade Launcher"),
	EWT_RocketLauncher  UMETA(DisplayName = "Rocket Launcher"),

	EWT_MAX				UMETA(DisplayName = "DefaultMax"),
};