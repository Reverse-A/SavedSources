/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShortRangeWeaponTypeData.generated.h"

 /**
  * EShortRangeWeaponType Enum:
  * ---------------------------
  * Enumerates the possible types of short-range melee weapons.
  *
  * Values:
  *  - Select: Default option for unspecified melee weapons.
  *  - Sword: Represents a sword melee weapon.
  *  - Dagger: Represents a dagger melee weapon.
  *  - Mace: Represents a mace melee weapon.
  *  - GreatHammer: Represents a great hammer melee weapon.
  *  - Axe: Represents an axe melee weapon.
  *  - GreatAxe: Represents a great axe melee weapon.
  *  - Spear: Represents a spear melee weapon.
  *  - Sickle: Represents a sickle melee weapon.
  *  - HiddenBlade: Represents a hidden blade melee weapon.
  */
UENUM(BlueprintType)
enum class EShortRangeWeaponType : uint8
{
    // Default type for unspecified melee weapons.
    Select         UMETA(DisplayName = "Select"),       // Selection

    // Bladed Weapons
    Sword          UMETA(DisplayName = "Sword"),         // Sword
    Dagger         UMETA(DisplayName = "Dagger"),        // Dagger

    // Blunt Weapons
    Mace           UMETA(DisplayName = "Mace"),          // Mace
    GreatHammer    UMETA(DisplayName = "Great Hammer"),  // Great hammer

    // Axes
    Axe            UMETA(DisplayName = "Axe"),           // Axe
    GreatAxe       UMETA(DisplayName = "Great Axe"),     // Great axe

    // Polearms
    Spear          UMETA(DisplayName = "Spear"),         // Spear
    Sickle         UMETA(DisplayName = "Sickle"),        // Sickle

    // Special Weapons
    HiddenBlade    UMETA(DisplayName = "Hidden Blade")   // Hidden blade
};

// Data asset class representing the type of a short range weapon.
UCLASS()
class WEAPONMASTERY_API UShortRangeWeaponTypeData : public UDataAsset
{
    GENERATED_BODY()

protected:
    // Property representing the type of the short range weapon,
    // editable only for default values.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    EShortRangeWeaponType MeleeWeaponType = EShortRangeWeaponType::Select;

public:
    // Inline function to retrieve the current short range weapon type.
    FORCEINLINE EShortRangeWeaponType GetShortRangeWeaponType() const;

    // Function to set a new short range weapon type for this instance.
    void SetShortRangeWeaponType(EShortRangeWeaponType InMeleeWeaponType);
};
