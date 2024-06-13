/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RarityData.generated.h"

 /**
  * @enum ERarityEffectProcess
  *
  * @brief Enum representing the process of applying or removing rarity effects.
  *
  * This enum is used to define the actions that can be performed on rarity effects within the game or application.
  * The enum provides two values: Apply and Remove, which correspond to the operations of adding a rarity effect
  * to an object or entity and removing an existing rarity effect from an object or entity, respectively.
  */
UENUM(BlueprintType)
enum class ERarityEffectProcess : uint8
{
    /*Selection*/
    Select,
    /**
     * @brief Apply a rarity effect.
     *
     * This value indicates that a rarity effect should be applied.
     * Use this when you need to enhance an object or entity by assigning it a specific rarity effect.
     */
    Apply,

    /**
     * @brief Remove a rarity effect.
     *
     * This value indicates that a rarity effect should be removed.
     * Use this when you need to revert an object or entity to its state prior to having a rarity effect, or to change its effect.
     */
    Remove
};


/**
 * @enum ERarityType
 *
 * @brief Enumeration defining the rarity categories of weapons.
 *
 * This enum is used to categorize weapons based on their rarity. Each category represents a different level of
 * quality and scarcity, ranging from the most common and basic weapons to the rarest and most powerful ones.
 * These categories help to classify and manage weapons within the game, making it easier to implement rarity-based
 * logic and mechanics.
 */
UENUM(BlueprintType)
enum class ERarityType : uint8
{
    /**
     * @brief Default weapon rarity.
     *
     * This value represents weapons with no specific rarity. These weapons may serve as placeholders or generic items.
     */

    /*Common*/
    /**
    * @brief Common weapons - basic and easily found.
    *
    * Common weapons are the most basic type of weapons. They are plentiful and easy to find, often serving as
    * starting equipment or easily obtainable gear.
    */

    /*Rare*/
    /**
     * @brief Rare weapons - higher quality and harder to find.
     *
     * Rare weapons are of higher quality than common weapons and are less frequently encountered. They offer better
     * performance and often have enhanced attributes.
     */

    /*Epic*/
    /**
    * @brief Epic weapons - very high quality with significant rarity.
    *
    * Epic weapons are significantly better in quality and power compared to common and rare weapons. They are
    * considerably harder to find and provide substantial advantages in combat.
    */

    /*Legendary*/
    /**
     * @brief Legendary weapons - exceptional quality and extremely rare.
     *
     * Legendary weapons are of exceptional quality, offering outstanding performance and unique abilities. They are
     * extremely rare and are often sought after by players for their powerful attributes and unique features.
     */

    /*Mystic*/
    /**
    * @brief Mystic weapons - almost mythical in nature, extraordinarily rare.
    *
    * Mystic weapons are nearly mythical, with extraordinary attributes and powers. They are extraordinarily rare and
    * often come with lore or special significance within the game world.
    */

    /*Unique*/
    /**
     * @brief Unique weapons - one-of-a-kind items with special attributes.
     *
     * Unique weapons are one-of-a-kind items with special attributes that set them apart from all other weapons.
     * These weapons are exceptionally rare and often have custom appearances and abilities.
     */
    Select,
    Common,
    Rare,
    Epic,
    Legendary,
    Mystic,
    Unique
};


// Data asset class representing the rarity type of a weapon.
UCLASS()
class WEAPONMASTERY_API URarityData : public UDataAsset
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    ERarityEffectProcess RarityEffectProcess = ERarityEffectProcess::Select;
    // Property representing the rarity type of the weapon,
    // editable only for default values.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    ERarityType RarityType = ERarityType::Select;

public:
    FORCEINLINE ERarityEffectProcess GetRarityEffectProcess() const;

    void SetRarityEffectProcess(ERarityEffectProcess InRarityEffectProcess);

    // Inline function to retrieve the current rarity type.
    FORCEINLINE ERarityType GetRarityType() const;

    // Function to set a new rarity type for this instance.
    void SetRarityType(ERarityType InRarityType);
};
