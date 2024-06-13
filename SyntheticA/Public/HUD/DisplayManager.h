// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DisplayManager.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;

UCLASS()
class SYNTHETICA_API UDisplayManager : public UUserWidget
{
	GENERATED_BODY()

public:
#pragma region ProgressBar

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldBar;

#pragma endregion 
#pragma region TextBlock
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HeaderHealth;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShieldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HeaderShield;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathsAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathsText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponTypeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponCrosshairAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoMiddle;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchCountDownText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EquipWeaponText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathNote;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BlackTeamScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WhiteTeamScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreSpacerText;

	UPROPERTY(meta = (BindWidget))
	UImage* HealhtShieldBackgroundFrame;

	UPROPERTY(meta = (BindWidget))
	UImage* WeaponBackgroundFrame;

#pragma endregion

#pragma region Images
	UPROPERTY(meta = (BindWidget))
	UImage* HighPingImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HighPingAnimation;
#pragma endregion 
};
