/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttainmentData.generated.h"

// Define an enumeration named EAttainmentType to represent different methods of acquiring weapons.
UENUM(BlueprintType)
enum class EAttainmentType : uint8
{
    Select,         // Selection
    None,           // No acquisition method - the weapon cannot be obtained through conventional means.
    Pickable,       // Can be picked up in the game world by the player.
    Purchasable,    // Can be purchased within the game using virtual currency or resources.
    Lootable,       // Can be obtained as loot from defeated enemies, treasure chests, or other interactive elements.
    Craftable,      // Can be crafted by the player using in-game crafting mechanics and materials.
    Tradeable,      // Can be crafted by the player using in-game crafting mechanics and materials.
    Reward,         // Can be obtained as a reward for completing quests, achieving milestones, or fulfilling criteria.
    Upgradable      // Can be upgraded to enhance its attributes or functionality over time.
};

UCLASS()
class WEAPONMASTERY_API UAttainmentData : public UDataAsset
{
	GENERATED_BODY()
    
protected:
    // Define a property named AttainmentType that determines how the weapon is obtained,
    // accessible within this class and editable only for default values.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    EAttainmentType AttainmentType = EAttainmentType::Select;
	
public:
    // Inline function to retrieve the current AttainmentType value.
    FORCEINLINE EAttainmentType GetAttainmentType() const;

    // Function to set a new AttainmentType value for this instance.
    void SetAttainmentType(EAttainmentType InNewAttainmentType);
};
