/**
 * Copyright (c) [2024] Reverse-Alpha-Studios
 * This plugin is designed for use with the Unreal Engine system.
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RangeWeaponInfoWidget.generated.h"

// This widget class will be considered for directly reflecting the data 
// from the FRangedWeaponProperties structure located within the ARangeWeaponModule class to the widget.
class UTextBlock;

UCLASS()
class WEAPONMASTERY_API URangeWeaponInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* FireType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* FireMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Ammunition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Penetration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Range;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Accuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* MagazineCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* ReloadTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Weight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Durability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "LongRangeWeaponInformation")
    UTextBlock* Price;
};
