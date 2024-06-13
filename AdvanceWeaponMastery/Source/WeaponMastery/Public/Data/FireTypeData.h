/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FireTypeData.generated.h"

 /*
  * EFireType Enumeration:
  * ----------------------
  * This enumeration represents different types of firing mechanisms for weapons.
  * Each enumeration value corresponds to a specific type of firing method, with a display name provided for clarity.
  *
  * Enumeration Values:
  *  - Select: Default type for unspecified firing mechanisms.
  *  - Projectile: Represents a firing mechanism that uses projectiles (e.g., bullets, arrows).
  *  - HitScan: Represents a firing mechanism that uses the hit scan method (instant hit detection).
  */
UENUM(BlueprintType)
enum class EFireType : uint8
{
	Select	        UMETA(DisplayName = "Select"),           // Selection
	Projectile      UMETA(DisplayName = "ProjectileType"),   // Uses projectiles (e.g., bullets, arrows)
	HitScan         UMETA(DisplayName = "HitScanType")       // Hit scan method (instant hit detection)
};

// Data asset class representing the firing type of a weapon.
UCLASS()
class WEAPONMASTERY_API UFireTypeData : public UDataAsset
{
	GENERATED_BODY()

protected:
	// Property representing the firing type of the weapon,
	// editable only for default values.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    EFireType FireType = EFireType::Select;

public:
	// Inline function to retrieve the current firing type.
    FORCEINLINE EFireType GetFireType() const;

	// Function to set a new firing type for this instance.
    void SetFireType(EFireType InFireType);

};
