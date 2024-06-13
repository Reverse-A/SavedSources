/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FireModeData.generated.h"

 /**
  * EFireModeType Enum:
  * -------------------
  * Enumerates the possible firing modes for a weapon.
  *
  * Values:
  *  - Select: Default option for unspecified fire mode.
  *  - SingleShot: Represents a single fire mode.
  *  - SemiAutomatic: Represents a semi-automatic fire mode.
  *  - Automatic: Represents a fully automatic fire mode.
  */
UENUM(BlueprintType, Meta = (DisplayName = "Weapon Firing Mode", ToolTip = "Specifies the firing mode of a weapon"))
enum class EFireModeType : uint8
{
    Select,           // Selection
    SingleShot,       // Single fire mode
    SemiAutomatic,    // Semi-automatic fire mode
    Automatic         // Fully automatic fire mode
};

// Data asset class representing the firing mode of a weapon.
UCLASS()
class WEAPONMASTERY_API UFireModeData : public UDataAsset
{
	GENERATED_BODY()

protected:
    // Property representing the firing mode of the weapon,
    // editable only for default values.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Settings|Data")
    EFireModeType FireModeType = EFireModeType::Select;
public:
    // Inline function to retrieve the current firing mode.
    FORCEINLINE EFireModeType GetFireModeType() const;

    // Function to set a new firing mode for this instance.
    void SetFireModeType(EFireModeType InFireModeType);
};