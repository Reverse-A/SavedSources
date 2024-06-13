/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponTypeData.generated.h"

 /*
  * EWeaponType Enum:
  * -----------------
  * This enumeration defines the various types of weapons that can be used in the game.
  * Each type represents a category or classification of weapons.
  *
  * Enum Values:
  *  - Select: Default value indicating no specific weapon type.
  *  - Unarmed: No weapon, using fists.
  *  - ShortRange: Close combat weapons.
  *  - LongRange: Long range weapons.
  *  - Throwable: Weapons that can be thrown.
  *  - Explosive: Explosive weapons.
  *  - Heavy: Heavy weapons with significant firepower.
  */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Select           UMETA(DisplayName = "Select"),                 // Selection
    Unarmed          UMETA(DisplayName = "Unarmed"),                // No weapon, using fists
    ShortRange       UMETA(DisplayName = "Short Range Weapon"),     // Close combat weapons
    LongRange        UMETA(DisplayName = "Long Range Weapon"),      // Long range weapons
    Throwable        UMETA(DisplayName = "Throwable"),              // Weapons that can be thrown
    Explosive        UMETA(DisplayName = "Explosive"),              // Explosive weapons
    Heavy            UMETA(DisplayName = "Heavy Weapon")            // Heavy weapons with significant firepower
};

// Data asset class representing the type of weapon.
UCLASS()
class WEAPONMASTERY_API UWeaponTypeData : public UDataAsset
{
	GENERATED_BODY()
	
protected:
    // Property representing the type of weapon, editable only for default values.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    EWeaponType WeaponType = EWeaponType::Select;

public:
    // Function to retrieve the current weapon type.
    FORCEINLINE EWeaponType GetWeaponType() const;

    // Function to set a new weapon type for this instance.
    void SetWeaponType(EWeaponType InWeaponType);
};
