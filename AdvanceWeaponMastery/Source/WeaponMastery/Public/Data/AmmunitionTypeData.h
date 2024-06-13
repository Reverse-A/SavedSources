/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AmmunitionTypeData.generated.h"

 /*
  * EAmmunitionType Enumeration:
  * ----------------------------
  * This enumeration represents different types of ammunition that can be used in weapons.
  * Each enumeration value corresponds to a specific type of ammunition, with a display name provided for clarity.
  *
  * Enumeration Values:
  *  - Select: Default type for unspecified ammunition.
  *  - EAT_9mm: Represents 9mm ammunition.
  *  - EAT_45ACP: Represents .45 ACP ammunition.
  *  - EAT_556mm: Represents 5.56mm ammunition.
  *  - EAT_762mm: Represents 7.62mm ammunition.
  *  - EAT_50Caliber: Represents .50 caliber ammunition.
  *  - EAT_12Gauge: Represents 12 gauge ammunition.
  *  - EAT_40mm: Represents 40mm grenade ammunition.
  *  - EAT_Rocket: Represents rocket ammunition for launchers.
  *  - EAT_Grenade: Represents general grenade ammunition.
  *  - EAT_Arrow: Represents arrow ammunition for bows.
  *  - EAT_Bolt: Represents bolt ammunition for crossbows.
  *  - EAT_Cartridge: Represents a general cartridge type.
  */
UENUM(BlueprintType)
enum class EAmmunitionType : uint8
{
    // Default type for unspecified ammunition.
    Select              UMETA(DisplayName = "Select"),           // Selection

    // Handgun Ammunition
    EAT_9mm             UMETA(DisplayName = "9mm"),               // 9mm ammunition
    EAT_45ACP           UMETA(DisplayName = ".45 ACP"),           // .45 ACP ammunition

    // Rifle Ammunition
    EAT_556mm           UMETA(DisplayName = "5.56mm"),            // 5.56mm ammunition
    EAT_762mm           UMETA(DisplayName = "7.62mm"),            // 7.62mm ammunition
    EAT_50Caliber       UMETA(DisplayName = ".50 Caliber"),       // .50 caliber ammunition

    // Shotgun Ammunition
    EAT_12Gauge         UMETA(DisplayName = "12 Gauge"),          // 12 gauge ammunition

    // Explosive Ammunition
    EAT_40mm            UMETA(DisplayName = "40mm Grenade"),      // 40mm grenade
    EAT_Rocket          UMETA(DisplayName = "Rocket"),            // Rockets for launchers
    EAT_Grenade         UMETA(DisplayName = "Grenade"),           // General grenade

    // Archery Ammunition
    EAT_Arrow           UMETA(DisplayName = "Arrow"),             // Arrows for bows
    EAT_Bolt            UMETA(DisplayName = "Bolt"),              // Bolts for crossbows

    // General Ammunition
    EAT_Cartridge       UMETA(DisplayName = "Cartridge")          // General cartridge type
};

// Data asset class representing the type of ammunition.
UCLASS()
class WEAPONMASTERY_API UAmmunitionTypeData : public UDataAsset
{
	GENERATED_BODY()
	
protected:
    // Property representing the type of the ammunition,
    // editable only for default values.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    EAmmunitionType AmmunitionType = EAmmunitionType::Select;
public:
    // Inline function to retrieve the current ammunition type.
    FORCEINLINE EAmmunitionType GetAmmunitionType() const;

    // Function to set a new ammunition type for this instance.
    void SetAmmunitionType(EAmmunitionType InAmmunitionType);
};
