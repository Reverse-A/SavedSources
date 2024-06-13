// ©2023 Reverse-A. All rights reserved.

#include "BasePlayerController.h"
#include "Synthetica/Characters/BaseCharacter.h"
#include "Synthetica/HUD/BaseHUD.h"
#include "Synthetica/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Widgets/DeclarativeSyntaxSupport.h"



void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	BaseHUD = Cast<ABaseHUD>(GetHUD());
	//BaseHUD->CharacterOverlay->WeaponAmmoAmount1->SetVisibility(ESlateVisibility::Hidden);
}

void ABasePlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;

	bool bHudValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->HealthBar &&
		BaseHUD->CharacterOverlay->HealthText;

	if (bHudValid)
	{
		const float HealthPercent = Health / MaxHealth;
		BaseHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);

		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BaseHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void ABasePlayerController::SetHUDAmmunation(int32 Ammo)
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;

	bool bHudValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->WeaponAmmoAmount;

	if (bHudValid)
	{

		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);

		BaseHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));

		if (Ammo != 0.f)
		{
			BaseHUD->CharacterOverlay->WeaponAmmoAmount1->SetText(FText::FromString(AmmoText));
		}
		else
		{		
			FString ReloadText = FString::Printf(TEXT("Empty"));
			BaseHUD->CharacterOverlay->WeaponAmmoAmount1->SetText(FText::FromString(ReloadText));
		}
	}
}


void ABasePlayerController::SetHUDCarriedAmmunation(int32 Ammo)
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;

	bool bHudValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->CarriedAmmoAmount;

	if (bHudValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BaseHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void ABasePlayerController::SetHUDGrenades(int32 Grenades)
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;

	bool bHudValid = BaseHUD &&
		BaseHUD->CharacterOverlay &&
		BaseHUD->CharacterOverlay->GrenadesText;

	if (bHudValid)
	{
		FString GrenadesText = FString::Printf(TEXT("%d"), Grenades);
		BaseHUD->CharacterOverlay->GrenadesText->SetText(FText::FromString(GrenadesText));
	}
	else
	{
		HUDGrenades = Grenades;
	}
}
