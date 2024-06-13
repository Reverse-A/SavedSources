/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponStateData.generated.h"

 /*
  * EWeaponState Enumeration:
  * -------------------------
  * Enumeration defining the different states of a weapon in the game.
  * Each state represents a specific condition or situation a weapon can be in during gameplay.
  */
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    Select              UMETA(DisplayName = "Select"),              // Selection
    Initial             UMETA(DisplayName = "Initial"),             // Initial state of the weapon
    Equipped            UMETA(DisplayName = "Equipped"),            // Weapon is equipped as the primary weapon
    EquippedSecondary   UMETA(DisplayName = "Equipped Secondary"),  // Weapon is equipped as a secondary weapon
    Dropped             UMETA(DisplayName = "Dropped"),             // Weapon is dropped on the ground
    Tradeable           UMETA(DisplayName = "Tradeable"),           // Weapon is tradeable
    Purchasable         UMETA(DisplayName = "Purchasable"),         // Weapon is tradeable
    Lootable            UMETA(DisplayName = "Loot-able"),           // Weapon is tradeable
    Craftable           UMETA(DisplayName = "Craft-able"),          // Weapon is available for crafting
    Upgradable          UMETA(DisplayName = "Upgradable"),          // Weapon can be upgraded
    Reward              UMETA(DisplayName = "Reward"),              // Weapon can be upgraded
    Broken              UMETA(DisplayName = "Broken"),              // Weapon is broken and cannot be used
    InInventory         UMETA(DisplayName = "In Inventory"),        // Weapon is in the player's inventory
    InStorage           UMETA(DisplayName = "In Storage"),          // Weapon is stored in a safe place
    MAX                 UMETA(DisplayName = "Default MAX")          // Default maximum value
};

// Data asset class representing the state of a weapon.
UCLASS()
class WEAPONMASTERY_API UWeaponStateData : public UDataAsset
{
	GENERATED_BODY()
	
protected:
    // Property representing the state of the weapon,
    // editable only for default values.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    EWeaponState WeaponStateType = EWeaponState::Select;
public:
    // Inline function to retrieve the current weapon state.
    FORCEINLINE EWeaponState GetWeaponState() const;

    // Function to set a new weapon state for this instance.
    void SetWeaponState(EWeaponState InWeaponState);
};
