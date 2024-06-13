/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LongRangeWeaponTypeData.generated.h"

 /*
  * ELongRangeWeaponType Enum:
  * -------------------------
  * This enumeration defines the various types of long-range weapons that can be used in the game.
  * Each type represents a specific category or classification of long-range weapons.
  *
  * Enum Values:
  *  - Select: Default value indicating no specific long-range weapon type.
  *  - Pistol: Handguns.
  *  - AssaultRifle: Assault rifles.
  *  - Sniper: Sniper rifles.
  *  - BattleRifle: Battle rifles.
  *  - Submachine: Submachine guns.
  *  - PDW: Personal Defense Weapons (PDWs).
  *  - LightMachine: Light machine guns.
  *  - HeavyMachine: Heavy machine guns.
  *  - Shotgun: Shotguns.
  *  - RocketLauncher: Rocket launchers.
  *  - GrenadeLauncher: Grenade launchers.
  *  - Bow: Bows.
  *  - Crossbow: Crossbows.
  *  - HiddenBlade: Hidden blade weapons (e.g., knives).
  *  - ThrowingKnife: Throwing knives.
  */

UENUM(BlueprintType)
enum class ELongRangeWeaponType : uint8
{
    // Default type for unspecified ranged weapons.
    Select          UMETA(DisplayName = "Select"),           // Selection

    // Handguns
    Pistol          UMETA(DisplayName = "Pistol"),            // Pistol

    // Rifles
    AssaultRifle    UMETA(DisplayName = "Assault Rifle"),     // Assault rifle
    Sniper          UMETA(DisplayName = "Sniper Rifle"),      // Sniper rifle
    BattleRifle     UMETA(DisplayName = "Battle Rifle"),      // Battle rifle

    // Submachine Guns
    Submachine      UMETA(DisplayName = "Submachine Gun"),    // Submachine gun
    PDW             UMETA(DisplayName = "Personal Defense Weapon"), // Personal Defense Weapon (PDW)

    // Machine Guns
    LightMachine    UMETA(DisplayName = "Light Machine Gun"), // Light machine gun
    HeavyMachine    UMETA(DisplayName = "Heavy Machine Gun"), // Heavy machine gun

    // Shotguns
    Shotgun         UMETA(DisplayName = "Shotgun"),           // Shotgun

    // Launchers
    RocketLauncher  UMETA(DisplayName = "Rocket Launcher"),   // Rocket launcher
    GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),  // Grenade launcher

    // Bows and Crossbows
    Bow             UMETA(DisplayName = "Bow"),               // Bow
    Crossbow        UMETA(DisplayName = "Crossbow"),          // Crossbow

    // Throwables and Hidden Weapons
    HiddenBlade     UMETA(DisplayName = "Hidden Blade"),      // Hidden blade weapons (e.g., knives)
    ThrowingKnife   UMETA(DisplayName = "Throwing Knife")     // Throwing knife
};

// Data asset class representing the type of a long-range weapon.
UCLASS()
class WEAPONMASTERY_API ULongRangeWeaponTypeData : public UDataAsset
{
	GENERATED_BODY()

protected:
    // Property representing the type of the long-range weapon,
    // editable only for default values.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    ELongRangeWeaponType LongRangeWeaponType = ELongRangeWeaponType::Select;

public:
    // Inline function to retrieve the current weapon type.
	FORCEINLINE ELongRangeWeaponType GetLongRangeWeaponType() const;

    // Function to set a new weapon type for this instance.
	void SetLongRangeWeaponType(ELongRangeWeaponType InLongRangeWeaponType);
};
