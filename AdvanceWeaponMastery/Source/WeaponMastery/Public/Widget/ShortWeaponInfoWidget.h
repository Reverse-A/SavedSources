/**
 * Copyright (c) [2024] Reverse-Alpha-Studios
 * This plugin is designed for use with the Unreal Engine system.
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShortWeaponInfoWidget.generated.h"

// This widget class has been incorrectly named. It is actually a UI widget prepared for melee-type weapons. 
// Please consider it as MeleeWeaponType. 
// The variables below have been matched with the values in the AMeleeWeaponModule class.
class UTextBlock;

UCLASS()
class WEAPONMASTERY_API UShortWeaponInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* Penetration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* Range;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* Weight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* Durability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "ShortRangeWeaponInformation")
    UTextBlock* Price;
};
