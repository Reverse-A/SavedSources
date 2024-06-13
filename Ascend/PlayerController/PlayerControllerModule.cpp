// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerControllerModule.h"
#include "Ascend/HUD/PlayerHUD.h"
#include "Ascend/UI/Player/PlayerMainScreen.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


APlayerControllerModule::APlayerControllerModule(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

APlayerHUD* APlayerControllerModule::GetPlayerHUD() const
{
	return CastChecked<APlayerHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

void APlayerControllerModule::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerControllerModule::SetHUDHealth(float Health, float MaxHealth, bool bAlive)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD)
	{
		if (bAlive)
		{
			if (PlayerHUD->GetPlayerMainScreen()->HealthProgressBar->IsVisible() == false)
			{
				PlayerHUD->GetPlayerMainScreen()->HealthProgressBar->SetVisibility(ESlateVisibility::Visible);
			}
			const float HealthPercent = Health / MaxHealth;
			PlayerHUD->GetPlayerMainScreen()->HealthProgressBar->SetPercent(HealthPercent);
		}
		else
		{
			if (PlayerHUD->GetPlayerMainScreen()->HealthProgressBar->IsVisible())
			{
				PlayerHUD->GetPlayerMainScreen()->HealthProgressBar->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void APlayerControllerModule::SetHUDShield(float Shield, float MaxShield, bool bAlive)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD)
	{
		if (bAlive)
		{

			if (PlayerHUD->GetPlayerMainScreen()->ShieldProgressBar->IsVisible() == false)
			{
				PlayerHUD->GetPlayerMainScreen()->ShieldProgressBar->SetVisibility(ESlateVisibility::Visible);
			}

			const float ShieldPercent = Shield / MaxShield;
			PlayerHUD->GetPlayerMainScreen()->ShieldProgressBar->SetPercent(ShieldPercent);

		}
		else
		{
			if (PlayerHUD->GetPlayerMainScreen()->ShieldProgressBar->IsVisible())
			{
				PlayerHUD->GetPlayerMainScreen()->ShieldProgressBar->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void APlayerControllerModule::SetHUDStamina(float Health, float MaxHealth, bool bAlive)
{
	
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (bAlive)
	{
		if (PlayerHUD)
		{
			if (PlayerHUD->GetPlayerMainScreen()->StaminaProgressBar->IsVisible() == false)
			{
				PlayerHUD->GetPlayerMainScreen()->StaminaProgressBar->SetVisibility(ESlateVisibility::Visible);
			}
			const float StaminaPercent = Health / MaxHealth;
			PlayerHUD->GetPlayerMainScreen()->StaminaProgressBar->SetPercent(StaminaPercent);
		}
	}
	else
	{
		if (PlayerHUD)
		{
			if (PlayerHUD->GetPlayerMainScreen()->StaminaProgressBar->IsVisible())
			{
				PlayerHUD->GetPlayerMainScreen()->StaminaProgressBar->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void APlayerControllerModule::SetHUDCredits(float Credits)
{

}

void APlayerControllerModule::SetHUDWeaponAmmunition(int32 Ammunition)
{

}

void APlayerControllerModule::SetHUDCarriedAmmunition(int32 CarriedAmmunition)
{

}

void APlayerControllerModule::SetHUDRangedWeaponType(ERangedWeaponType RangedWeaponType)
{

}

void APlayerControllerModule::SetHUDMeleeWeaponType(EMeleeWeaponType MeleeWeaponType)
{

}

void APlayerControllerModule::SetHUDExperience(float Experience, float LevelExperienceCap, int32 CurrentPlayerLevel, bool bAlive)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (bAlive)
	{
		if (PlayerHUD)
		{
			if (PlayerHUD->GetPlayerMainScreen()->ExperienceProgressBar->IsVisible() == false)
			{
				PlayerHUD->GetPlayerMainScreen()->ExperienceProgressBar->SetVisibility(ESlateVisibility::Visible);
				PlayerHUD->GetPlayerMainScreen()->ExperienceAndLevelText->SetVisibility(ESlateVisibility::Visible);
			}
			float ExperiencePercent = Experience / LevelExperienceCap;
			PlayerHUD->GetPlayerMainScreen()->ExperienceProgressBar->SetPercent(ExperiencePercent);

			FString PlayerLevelText = FString::Printf(TEXT("Lvl.%d"), CurrentPlayerLevel);
			FString ExperienceText = FString::Printf(TEXT("%.0f / %.0f"), Experience, LevelExperienceCap);
			FString FullText = FString::Printf(TEXT("%s\n%s"), *PlayerLevelText, *ExperienceText);

			PlayerHUD->GetPlayerMainScreen()->ExperienceAndLevelText->SetText(FText::FromString(FullText));
		}
	}
	else
	{
		if (PlayerHUD)
		{
			if (PlayerHUD->GetPlayerMainScreen()->StaminaProgressBar->IsVisible())
			{
				PlayerHUD->GetPlayerMainScreen()->ExperienceProgressBar->SetVisibility(ESlateVisibility::Hidden);
				PlayerHUD->GetPlayerMainScreen()->ExperienceAndLevelText->SetVisibility(ESlateVisibility::Hidden);
			}
		}	
	}
}

void APlayerControllerModule::SetHUDQuests()
{

}
