#pragma once

#define TRACE_MAX_LENGHT 80000.0f

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Pistol			UMETA(DisplayName = "Pistol"),
	EWT_AssaultRifle	UMETA(DisplayName = "Assault Rifle"),
	EWT_SubmachineGun   UMETA(DisplayName = "Sub-MachineGun"),
	EWT_ScatterWeapons	UMETA(DisplayName = "ScatterWeapons"),
	EWT_SniperRifle		UMETA(DisplayName = "SniperRifle"),

	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
	EWT_RocketLauncher	UMETA(DisplayName = "Rocket Launcher"),

	EWT_MAX				UMETA(DisplayName = "Default MAX")
};