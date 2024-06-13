// ©2023 Reverse - A Company. All rights reserved.

#include "PlayerController/PlayerControllerManager.h"

#include "Net/UnrealNetwork.h"

#include "Characters/BaseCharacter.h"
#include "Components/Button.h"
#include "Components/CombatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "HUD/Announcement.h"
#include "HUD/PlayerHUD.h"
#include "HUD/DisplayManager.h"
#include "GameModes/BaseGameMode.h"
#include "GameState/GameStateManager.h"
#include "PlayerState/PlayerStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Weapon.h"

#include "NewTypes/Announcement.h"

#pragma region Main/Functions

APlayerControllerManager::APlayerControllerManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

//#if USING_CHEAT_MANAGER
//	// TODO ADD CHEAT CLASS INSIDE IT
//#endif // #if USING_CHEAT_MANAGER
}
APlayerStateManager* APlayerControllerManager::GetPlayerstateManager() const
{
	return CastChecked<APlayerStateManager>(PlayerState, ECastCheckedType::NullAllowed);
}
APlayerHUD* APlayerControllerManager::GetPlayerHUD() const
{
	return CastChecked<APlayerHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

bool APlayerControllerManager::TryToRecordClientReplay()
{
	if (ShouldRecordClientReplay())
	{

	}
	return false;
}

bool APlayerControllerManager::ShouldRecordClientReplay()
{
	return false;
}

void APlayerControllerManager::Server_Cheat_Implementation(const FString& Message)
{
//#if USING_CHEAT_MANAGER
//	if (CheatManager)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ServerCheat: %s"), *Message);
//		ClientMessage(ConsoleCommand(Message));
//	}
//#endif
}

bool APlayerControllerManager::Server_Cheat_Validate(const FString& Message)
{
	return true;
}

void APlayerControllerManager::Server_CheatAll_Implementation(const FString& Message)
{

}

bool APlayerControllerManager::Server_CheatAll_Validate(const FString& Message)
{
	return true;
}

void APlayerControllerManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerHUD		= Cast<APlayerHUD>(GetHUD());
	BaseCharacter	= Cast<ABaseCharacter>(GetPawn());
	Server_CheckMatchState();
}

void APlayerControllerManager::Tick(float DeltaTime)
{
	Super::Tick	(DeltaTime);

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;

	InitializePool();
	CheckTimeSync(DeltaTime);
	CheckPing(DeltaTime);
	SetHUDTime();
}

void APlayerControllerManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerControllerManager, MatchState);
	DOREPLIFETIME(APlayerControllerManager, bShowTeamScores);
}

void APlayerControllerManager::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(InPawn) : BaseCharacter;
	if (BaseCharacter)
	{
		SetHUDHealth(BaseCharacter->GetHealth(), BaseCharacter->GetMaxHealth());
		SetHUDShield(BaseCharacter->GetShield(), BaseCharacter->GetShield());
		LOG_SCREEN("DONE")
	}
}


void APlayerControllerManager::InitializePool()
{
	if (DisplayManager == nullptr)
	{
		PlayerHUD = PlayerHUD == nullptr ? PlayerHUD = Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD)
		{
			DisplayManager = PlayerHUD->DisplayManager;
			if (DisplayManager)
			{
				if (bInitializeHealth == true)
				{
					SetHUDHealth(HUDHealth, HUDMaxHealth);
				}
				if (bInitializeShield == true)
				{
					SetHUDShield(HUDShield, HUDMaxShield);
				}
				if (bInitializeScore == true)
				{
					SetHUDScore(HUDScore);
				}
				if (bInitializeDeaths == true)
				{
					SetHUDDeaths(HUDDeaths);
				}
				if (bInitializeWeaponAmmunition == true)
				{
					SetHUDWeaponAmmo(HUDWeaponAmmunition);
				}
				if (bInitializeCarriedAmmunition == true)
				{
					SetHUDCarriedAmmo(HUDCarriedAmmunition);
				}
			}
		}
	}
}
#pragma endregion

#pragma region HUDFunctions

void APlayerControllerManager::ResetHUDElementsToDefault()
{
	SetHUDHealth(0.0f, 0.0f);
	SetHUDShield(0.0f, 0.0f);
	SetHUDWeaponAmmo(0);
	SetHUDCarriedAmmo(0);
	SetHUDWeaponType(EWeaponType::EWT_MAX, false);
}

void APlayerControllerManager::SetHUDIfPlayerInVehicle(bool bHide)
{

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter->GetPlayerIsEliminated())
	{
		return;
	}

	if (bHide)
	{
		if (bHUDHideInVehicle)
		{
			return;
		}
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			if (PlayerHUD->DisplayManager->HeaderHealth &&	PlayerHUD->DisplayManager->HeaderHealth->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HeaderHealth->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->HealthBar &&	PlayerHUD->DisplayManager->HealthBar->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HealthBar->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->HealthText && PlayerHUD->DisplayManager->HealthText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HealthText->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->HeaderShield &&	PlayerHUD->DisplayManager->HeaderShield->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HeaderShield->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->ShieldBar &&	PlayerHUD->DisplayManager->ShieldBar->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->ShieldBar->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->ShieldText && PlayerHUD->DisplayManager->ShieldText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->ShieldText->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->ScoreAmount && PlayerHUD->DisplayManager->ScoreAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->ScoreAmount->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->ScoreText &&	PlayerHUD->DisplayManager->ScoreText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->ScoreText->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->DeathsAmount && PlayerHUD->DisplayManager->DeathsAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->DeathsAmount->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->DeathsText && PlayerHUD->DisplayManager->DeathsText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->DeathsText->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->WeaponAmmoAmount &&	PlayerHUD->DisplayManager->WeaponAmmoAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->WeaponAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->AmmoMiddle && PlayerHUD->DisplayManager->AmmoMiddle->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->AmmoMiddle->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->CarriedAmmoAmount &&	PlayerHUD->DisplayManager->CarriedAmmoAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->CarriedAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->WeaponTypeText && PlayerHUD->DisplayManager->WeaponTypeText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->WeaponTypeText->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount &&	PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			}
			bHUDHideInVehicle = true;
		}
	}
	else
	{
		if (BaseCharacter->GetPlayerInVehicle() == false)
		{
			if (bHUDHideInVehicle == false) return;

			if (PlayerHUD->DisplayManager->HeaderHealth &&	PlayerHUD->DisplayManager->HeaderHealth->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HeaderHealth->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->HealthBar &&	PlayerHUD->DisplayManager->HealthBar->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HealthBar->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->HealthText && PlayerHUD->DisplayManager->HealthText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HealthText->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->HeaderShield &&	PlayerHUD->DisplayManager->HeaderShield->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HeaderShield->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->ScoreAmount && PlayerHUD->DisplayManager->ScoreAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->ScoreAmount->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->ScoreText &&	PlayerHUD->DisplayManager->ScoreText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->ScoreText->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->DeathsAmount && PlayerHUD->DisplayManager->DeathsAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->DeathsAmount->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->DeathsText && PlayerHUD->DisplayManager->DeathsText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->DeathsText->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->WeaponAmmoAmount && PlayerHUD->DisplayManager->WeaponAmmoAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->WeaponAmmoAmount->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->AmmoMiddle && PlayerHUD->DisplayManager->AmmoMiddle->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->AmmoMiddle->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->CarriedAmmoAmount &&	PlayerHUD->DisplayManager->CarriedAmmoAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->CarriedAmmoAmount->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->WeaponTypeText && PlayerHUD->DisplayManager->WeaponTypeText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->WeaponTypeText->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount && PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetVisibility(ESlateVisibility::Visible);
			}
			bHUDHideInVehicle = false;
		}
	}
}

void APlayerControllerManager::SetHUDHealth(float Health, float MaxHealth)
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter->GetHealth() >= 0)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			if (PlayerHUD->DisplayManager->HeaderHealth && PlayerHUD->DisplayManager->HeaderHealth->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HeaderHealth->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->HealthBar &&	PlayerHUD->DisplayManager->HealthBar->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HealthBar->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->HealthText && PlayerHUD->DisplayManager->HealthText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HealthText->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->ScoreAmount && PlayerHUD->DisplayManager->ScoreAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->ScoreAmount->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->ScoreText &&	PlayerHUD->DisplayManager->ScoreText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->ScoreText->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->DeathsAmount && PlayerHUD->DisplayManager->DeathsAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->DeathsAmount->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->DeathsText && PlayerHUD->DisplayManager->DeathsText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->DeathsText->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->DeathNote &&	PlayerHUD->DisplayManager->DeathNote->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->DeathNote->SetVisibility(ESlateVisibility::Hidden);
			}
			if (PlayerHUD->DisplayManager->HealhtShieldBackgroundFrame->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HealhtShieldBackgroundFrame->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->WeaponBackgroundFrame->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->WeaponBackgroundFrame->SetVisibility(ESlateVisibility::Visible);
			}

			FString HealthText = FString::Printf(TEXT("Health"));
			PlayerHUD->DisplayManager->HeaderHealth->SetText(FText::FromString(HealthText));

			FLinearColor UIBaseColour = FLinearColor(
				(float)(0x02 & 0xFF) / 255.0f,
				(float)(0xD8 & 0xFF) / 255.0f,
				(float)(0xDC & 0xFF) / 200.0f,
				(float)(0xFF & 0xFF) / 050.0f
			);
			PlayerHUD->DisplayManager->HeaderHealth->SetColorAndOpacity(UIBaseColour);

			const float HealthPercent = Health / MaxHealth;
			PlayerHUD->DisplayManager->HealthBar->SetFillColorAndOpacity(UIBaseColour);
			PlayerHUD->DisplayManager->HealthBar->SetPercent(HealthPercent);

			PlayerHUD->DisplayManager->HealthText->SetColorAndOpacity(UIBaseColour);
			FString HealthValueText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
			PlayerHUD->DisplayManager->HealthText->SetText(FText::FromString(HealthValueText));
		}
		else
		{
			bInitializeHealth	= true;
			HUDHealth			= Health;
			HUDMaxHealth		= MaxHealth;
		}
		
	}
}

void APlayerControllerManager::SetHUDShield(float Shield, float MaxShield)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			if (PlayerHUD->DisplayManager->ShieldBar &&	PlayerHUD->DisplayManager->ShieldBar->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->ShieldBar->SetVisibility(ESlateVisibility::Visible);
			}

			if (PlayerHUD->DisplayManager->ShieldText && PlayerHUD->DisplayManager->ShieldText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->ShieldText->SetVisibility(ESlateVisibility::Visible);
			}

			if (PlayerHUD->DisplayManager->HeaderShield && PlayerHUD->DisplayManager->HeaderShield->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->HeaderShield->SetVisibility(ESlateVisibility::Visible);
			}
		}

		FLinearColor UIBaseColour = FLinearColor(
			(float)(0x02 & 0x18) / 255.0f,
			(float)(0xD8 & 0xED) / 255.0f,
			(float)(0xDC & 0xEF) / 200.0f,
			(float)(0xFF & 0xFF) / 050.0f
		);
		PlayerHUD->DisplayManager->HeaderShield->SetColorAndOpacity(UIBaseColour);

		FString ShieldText = FString::Printf(TEXT("Shield"));
		PlayerHUD->DisplayManager->HeaderShield->SetText(FText::FromString(ShieldText));

		PlayerHUD->DisplayManager->ShieldBar->SetFillColorAndOpacity(UIBaseColour);
		const float ShieldPercent = Shield / MaxShield;
		PlayerHUD->DisplayManager->ShieldBar->SetPercent(ShieldPercent);

		PlayerHUD->DisplayManager->ShieldText->SetColorAndOpacity(UIBaseColour);
		FString ShieldValuesText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Shield), FMath::CeilToInt(MaxShield));
		PlayerHUD->DisplayManager->ShieldText->SetText(FText::FromString(ShieldValuesText));
	}
	else
	{
		bInitializeShield	= true;
		HUDShield			= Shield;
		HUDMaxShield		= MaxShield;
	}		
}

void APlayerControllerManager::SetHUDShieldOff()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD && PlayerHUD->DisplayManager)
	{				
		if (PlayerHUD->DisplayManager->ShieldBar && PlayerHUD->DisplayManager->ShieldBar->GetVisibility() != ESlateVisibility::Hidden)
		{
			PlayerHUD->DisplayManager->ShieldBar->SetVisibility(ESlateVisibility::Hidden);
		}			
				
		if (PlayerHUD->DisplayManager->ShieldText && PlayerHUD->DisplayManager->ShieldText->GetVisibility() != ESlateVisibility::Hidden)
		{
			PlayerHUD->DisplayManager->ShieldText->SetVisibility(ESlateVisibility::Hidden);
		}
		FString ShieldText = FString::Printf(TEXT("Shield Break"));
		PlayerHUD->DisplayManager->HeaderShield->SetText(FText::FromString(ShieldText));

		FLinearColor RedColor = FLinearColor::Red;
		PlayerHUD->DisplayManager->HeaderShield->SetColorAndOpacity(RedColor);
	}
	else
	{
	}
}

void APlayerControllerManager::SetHUDScore(float Score)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD && PlayerHUD->DisplayManager)
	{	
		if (PlayerHUD->DisplayManager->ScoreAmount && PlayerHUD->DisplayManager->ScoreAmount->GetVisibility() != ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->ScoreAmount->SetVisibility(ESlateVisibility::Visible);
		}
			
		if (PlayerHUD->DisplayManager->ScoreText && PlayerHUD->DisplayManager->ScoreText->GetVisibility() != ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->ScoreText->SetVisibility(ESlateVisibility::Visible);
		}
		
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		PlayerHUD->DisplayManager->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializeScore = true;
		HUDScore		 = Score;
	}	
}

void APlayerControllerManager::SetHUDKills(int32 Kills)
{

}

void APlayerControllerManager::SetHUDAssists(int32 Assists)
{

}

void APlayerControllerManager::SetHUDDeaths(int32 Deaths)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		if (PlayerHUD->DisplayManager->DeathsAmount && PlayerHUD->DisplayManager->DeathsAmount->GetVisibility() != ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->DeathsAmount->SetVisibility(ESlateVisibility::Visible);
		}
		if (PlayerHUD->DisplayManager->DeathsText && PlayerHUD->DisplayManager->DeathsText->GetVisibility() != ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->DeathsText->SetVisibility(ESlateVisibility::Visible);
		}

		FString DeathText = FString::Printf(TEXT("%d"), Deaths);
		PlayerHUD->DisplayManager->DeathsAmount->SetText(FText::FromString(DeathText));
	}
	else
	{
		bInitializeDeaths = true;
		HUDDeaths = Deaths;
	}	
}

void APlayerControllerManager::SetHUDWeaponAmmo(int32 Ammo)
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;

	AWeapon* EquippedWeapon = BaseCharacter->GetEquippedWeapon();

	if (EquippedWeapon)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			if (PlayerHUD->DisplayManager->WeaponAmmoAmount && PlayerHUD->DisplayManager->WeaponAmmoAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->WeaponAmmoAmount->SetVisibility(ESlateVisibility::Visible);
			}
			if (PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount && PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetVisibility(ESlateVisibility::Visible);
			}

			if (PlayerHUD->DisplayManager->AmmoMiddle && PlayerHUD->DisplayManager->AmmoMiddle->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->AmmoMiddle->SetVisibility(ESlateVisibility::Visible);
			}

			if (PlayerHUD->DisplayManager->WeaponTypeText && PlayerHUD->DisplayManager->WeaponTypeText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->WeaponTypeText->SetVisibility(ESlateVisibility::Visible);
			}

			if (BaseCharacter->GetEquippedWeapon()->IsEmpty())
			{
				FString Ammotext = FString::Printf(TEXT("%d"), Ammo);
				PlayerHUD->DisplayManager->WeaponAmmoAmount->SetText(FText::FromString(Ammotext));
				FString EmptyText = FString::Printf(TEXT("Emtpy"));
				PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetText(FText::FromString(EmptyText));
			}
			else
			{
				FString Ammotext = FString::Printf(TEXT("%d"), Ammo);
				PlayerHUD->DisplayManager->WeaponAmmoAmount->SetText(FText::FromString(Ammotext));
				PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetText(FText::FromString(Ammotext));
			}

			FString WeaponTypeText = UEnum::GetDisplayValueAsText(BaseCharacter->GetEquippedWeapon()->Get_WeaponType()).ToString();
			FString FormattedText = FString::Printf(TEXT("%s"), *WeaponTypeText, LINE_TERMINATOR);

			PlayerHUD->DisplayManager->WeaponTypeText->SetText(FText::FromString(FormattedText));
		}
		else
		{
			bInitializeWeaponAmmunition = true;
			HUDWeaponAmmunition			= Ammo;
		}
	}
	else
	{
		if (BaseCharacter->GetPlayerIsEliminated()) return;

		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD->DisplayManager->WeaponTypeText->GetVisibility() != ESlateVisibility::Hidden)
		{
			PlayerHUD->DisplayManager->WeaponTypeText->SetVisibility(ESlateVisibility::Hidden);
		}

		if (PlayerHUD->DisplayManager->AmmoMiddle->GetVisibility() != ESlateVisibility::Hidden)
		{
			PlayerHUD->DisplayManager->AmmoMiddle->SetVisibility(ESlateVisibility::Hidden);
		}
	}	
}

void APlayerControllerManager::SetHUDCarriedAmmo(int32 CarriedAmmo)
{

	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		PlayerHUD->DisplayManager->CarriedAmmoAmount->SetVisibility(ESlateVisibility::Visible);
		FString CarriedAmmoText = FString::Printf(TEXT("%d"), CarriedAmmo);
		PlayerHUD->DisplayManager->CarriedAmmoAmount->SetText(FText::FromString(CarriedAmmoText));
	}
	else
	{
		bInitializeCarriedAmmunition = true;
		HUDCarriedAmmunition = CarriedAmmo;
	}	
}

void APlayerControllerManager::SetHUDWeaponType(EWeaponType WeaponType, bool bVisible)
{
	if (bVisible)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			FString WeaponTypeText = UEnum::GetDisplayValueAsText(WeaponType).ToString();
			FString FormattedText = FString::Printf(TEXT("%s"), *WeaponTypeText, LINE_TERMINATOR);
			if (PlayerHUD->DisplayManager->EquipWeaponText)
			{
				PlayerHUD->DisplayManager->EquipWeaponText->SetText(FText::FromString(FormattedText));
			}
		}
	}
	else
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			FString HideWeaponText = FString::Printf(TEXT(""));
			PlayerHUD->DisplayManager->EquipWeaponText->SetText(FText::FromString(HideWeaponText));
		}
	}
}

void APlayerControllerManager::SetHUDFlag(bool bShow)
{
	if (bShow)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			FString FlagText = FString::Printf(TEXT("CaptureTheFlag"));
			PlayerHUD->DisplayManager->EquipWeaponText->SetText(FText::FromString(FlagText));
		}
	}
	else
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			FString FlagText = FString::Printf(TEXT(""));
			PlayerHUD->DisplayManager->EquipWeaponText->SetText(FText::FromString(FlagText));
		}
	}
}

void APlayerControllerManager::SetHUDMatchCountdown(float CountdownTime)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		if (CountdownTime < 0.0f)
		{
			PlayerHUD->DisplayManager->MatchCountDownText->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.0f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		PlayerHUD->DisplayManager->MatchCountDownText->SetText(FText::FromString(CountdownText));
	}
}

void APlayerControllerManager::SetHUDAnnouncementCountdown(float CountdownTime)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		if (CountdownTime < 0.0f)
		{
			PlayerHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.0f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		PlayerHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

void APlayerControllerManager::SetHUDTime()
{
	float TimeLeft = 0.f;

	if (MatchState == MatchState::WaitingToStart)
	{
		TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	}
	else if (MatchState == MatchState::InProgress)
	{
		TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	}
	else if (MatchState == MatchState::Cooldown)
	{
		TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	}

	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (HasAuthority())
	{
		if (BaseGameMode == nullptr)
		{
			BaseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
			LevelStartingTime = BaseGameMode->LevelStartingTime;
		}

		BaseGameMode = BaseGameMode == nullptr ? Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)) : BaseGameMode;
		if (BaseGameMode)
		{
			SecondsLeft = FMath::CeilToInt(BaseGameMode->GetCountdownTime() + LevelStartingTime);
		}
	}

	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}
void APlayerControllerManager::AddAnnouncementWidgetToScreen()	/*Located: BeginPlay*/
{
	if (PlayerHUD && MatchState == MatchState::WaitingToStart)
	{
		PlayerHUD->AddAnnouncementToScreen();
	}
}

#pragma endregion
#pragma region Elimination

void APlayerControllerManager::Broadcast_Elimination(APlayerState* Attacker, APlayerState* Victim)
{
	Client_EliminationAnnouncement(Attacker, Victim);
}

void APlayerControllerManager::Client_EliminationAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
	APlayerState* Self = GetPlayerState<APlayerState>();
	if (Attacker && Victim && Self)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

		if (PlayerHUD)
		{
			if (Attacker == Self && Victim != Self)
			{
				PlayerHUD->AddEliminationAnnoucementToScreen("You", Victim->GetPlayerName());
				return;
			}
			if (Victim == Self && Attacker != Self)
			{
				PlayerHUD->AddEliminationAnnoucementToScreen(Attacker->GetPlayerName(), "You");
				return;
			}
			if (Attacker == Victim && Attacker == Self)
			{
				PlayerHUD->AddEliminationAnnoucementToScreen("You", "yourself");
				return;
			}
			if (Attacker == Victim && Attacker != Self)
			{
				PlayerHUD->AddEliminationAnnoucementToScreen(Attacker->GetPlayerName(), "themselves");
				return;
			}
			PlayerHUD->AddEliminationAnnoucementToScreen(Attacker->GetPlayerName(), Victim->GetPlayerName());
		}
	}
}
#pragma endregion

#pragma region Match/Statement
void APlayerControllerManager::OnMatchStateSet(FName State, bool bTeamMatch)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		Handle_MatchHasStarted(bTeamMatch);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		Handle_MatchStateCooldown();
	}
}

void APlayerControllerManager::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		Handle_MatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		Handle_MatchStateCooldown();
	}
}

void APlayerControllerManager::Server_CheckMatchState_Implementation()
{
	BaseGameMode = BaseGameMode == nullptr ? Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)) : BaseGameMode;

	if (BaseGameMode)
	{
		WarmupTime		  = BaseGameMode->WarmupTime;
		MatchTime		  = BaseGameMode->MatchTime;
		CooldownTime	  = BaseGameMode->CooldownTime;
		LevelStartingTime = BaseGameMode->LevelStartingTime;

		MatchState	      = BaseGameMode->GetMatchState();

		Client_JoinMidGame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void APlayerControllerManager::Client_JoinMidGame_Implementation(FName StateOfMatch, float WarmupTimeInput, float MatchTimeInput, float CooldownInput, float StartingTimeInput)
{
	WarmupTime			= WarmupTimeInput;
	MatchTime			= MatchTimeInput;
	CooldownTime		= CooldownInput;
	LevelStartingTime	= StartingTimeInput;

	MatchState			= StateOfMatch;
	OnMatchStateSet(MatchState);

	AddAnnouncementWidgetToScreen();
}

void APlayerControllerManager::Handle_MatchHasStarted(bool bTeamMatch)
{
	if (HasAuthority())
	{
		bShowTeamScores = bTeamMatch;
	}

	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD)
	{
		if (PlayerHUD->DisplayManager == nullptr)
		{
			PlayerHUD->AddPlayerInterfaceToScreen();
		}

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController && PlayerController->HasAuthority())
		{

		}
		if (PlayerHUD->Announcement)
		{
			PlayerHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}

		if (!HasAuthority())
		{
			return;
		}

		if (bTeamMatch)
		{
			Initialize_TeamScores();
		}
		else
		{
			Hide_TeamScores();
		}
	}
}

void APlayerControllerManager::Handle_MatchStateCooldown()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD)
	{
		PlayerHUD->DisplayManager->RemoveFromParent();

		bool IsValid =
			PlayerHUD->Announcement &&
			PlayerHUD->Announcement->AnnouncementText &&
			PlayerHUD->Announcement->InfoText;

		if (IsValid)
		{
			PlayerHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText = Announcement::MatchStartString;
			PlayerHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));

			AGameStateManager*	 GameStateManager = Cast<AGameStateManager>(UGameplayStatics::GetGameState(this));
			APlayerStateManager* PlayerStateManager = GetPlayerState<APlayerStateManager>();
			if (GameStateManager && PlayerStateManager)
			{
				TArray<APlayerStateManager*> TopPlayers = GameStateManager->TopScoringPlayers;
				FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(GameStateManager) : GetInfoText(TopPlayers);

				PlayerHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
			}
		}
	}

	//BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter && BaseCharacter->Get_CombatComponent())
	{
		BaseCharacter->SetDisableGameplay(true);
		if (BaseCharacter->Get_CombatComponent()->IsFireButtonActive())
		{
			BaseCharacter->Get_CombatComponent()->Set_FireButton(false);
		}
		if (BaseCharacter->Get_CombatComponent()->IsAiming())
		{
			BaseCharacter->Get_CombatComponent()->Set_Aiming(false);
		}
		if (BaseCharacter->Get_CombatComponent()->IsReloading())
		{
			BaseCharacter->Get_CombatComponent()->SetReload(false);
		}
	}
}


/* TeamMatches */
void APlayerControllerManager::Initialize_TeamScores()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		FString Zero("0");
		FString Spacer("|");
		PlayerHUD->DisplayManager->BlackTeamScore->SetText(FText::FromString(Zero));
		PlayerHUD->DisplayManager->WhiteTeamScore->SetText(FText::FromString(Zero));
		PlayerHUD->DisplayManager->ScoreSpacerText->SetText(FText::FromString(Spacer));
	}
}

void APlayerControllerManager::Hide_TeamScores()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		PlayerHUD->DisplayManager->BlackTeamScore->SetText(FText());
		PlayerHUD->DisplayManager->WhiteTeamScore->SetText(FText());
		PlayerHUD->DisplayManager->ScoreSpacerText->SetText(FText());
	}
}

void APlayerControllerManager::Set_HUDVanguardSovereigntyScore(int32 VanguardTeamScore)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), VanguardTeamScore);
		PlayerHUD->DisplayManager->BlackTeamScore->SetText(FText::FromString(ScoreText));
	}
}

void APlayerControllerManager::Set_HUDGenesisPactScore(int32 GenesisTeamScore)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), GenesisTeamScore);
		PlayerHUD->DisplayManager->WhiteTeamScore->SetText(FText::FromString(ScoreText));
	}
}

void APlayerControllerManager::OnRep_ShowTeamScores()
{
	if (bShowTeamScores)
	{
		Initialize_TeamScores();
	}
	else
	{
		Hide_TeamScores();
	}
}
#pragma endregion
#pragma region Checks

#pragma region Time Section
void APlayerControllerManager::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		Server_RequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.0f;
	}
}

void APlayerControllerManager::Server_RequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();

	Client_ReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void APlayerControllerManager::Client_ReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime		= GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime = 0.5f * RoundTripTime;
	float CurrentServerTime = TimeServerReceivedClientRequest + SingleTripTime;

	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float APlayerControllerManager::GetServerTime()
{
	if (HasAuthority())
	{
		return GetWorld()->GetTimeSeconds();
	}
	else
	{
		return GetWorld()->GetTimeSeconds() + ClientServerDelta;
	}
}

void APlayerControllerManager::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		Server_RequestServerTime(GetWorld()->GetTimeSeconds());

	}
}
#pragma endregion

#pragma region Ping Section
void APlayerControllerManager::CheckPing(float DeltaTime)
{
	HighPingRunningTime += DeltaTime;
	if (HighPingRunningTime > CheckPingFrequency)
	{
		PlayerState = GetPlayerState<APlayerState>();
		if (PlayerState)
		{
			if (PlayerState->GetPingInMilliseconds() > HighPingThreshold)
			{
				HighPingWarning();
				PingAnimationRunningTime = 0.0f;
				Server_ReportPingStatus(true);
			}
			else
			{
				Server_ReportPingStatus(false);
			}

		}
		HighPingRunningTime = 0.0f;

	}
	bool bHighPingAnimationIsPlaying =
		PlayerHUD &&
		PlayerHUD->DisplayManager &&
		PlayerHUD->DisplayManager->HighPingAnimation &&
		PlayerHUD->DisplayManager->IsAnimationPlaying(PlayerHUD->DisplayManager->HighPingAnimation);

	if (bHighPingAnimationIsPlaying)
	{
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration)
		{
			StopHighPingWarning();
		}
	}
}

void APlayerControllerManager::HighPingWarning()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		PlayerHUD->DisplayManager->HighPingImage->SetOpacity(1.0f);
		PlayerHUD->DisplayManager->PlayAnimation
		(
			PlayerHUD->DisplayManager->HighPingAnimation,
			0.0f,
			5
		);
	}
}

void APlayerControllerManager::StopHighPingWarning()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		PlayerHUD->DisplayManager->HighPingImage->SetOpacity(0.0f);
		if (PlayerHUD->DisplayManager->IsAnimationPlaying(PlayerHUD->DisplayManager->HighPingAnimation))
		{
			PlayerHUD->DisplayManager->StopAnimation(PlayerHUD->DisplayManager->HighPingAnimation);
		}
	}
}
#pragma endregion

#pragma region ServerFuncs
// Is the ping too high?
void APlayerControllerManager::Server_ReportPingStatus_Implementation(bool bHighPing)
{
	HighPingDelegate.Broadcast(bHighPing);
}
#pragma endregion

#pragma region PlayerInfo
FString APlayerControllerManager::GetInfoText(const TArray<class APlayerStateManager*>& Players)
{
	APlayerStateManager* PlayerStateManager = GetPlayerState<APlayerStateManager>();
	if (PlayerStateManager == nullptr) return FString();
	FString InfoTextString;

	if (Players.Num() == 0)
	{
		InfoTextString = Announcement::LoseString;
	}
	else if (Players.Num() == 1 && Players[0] == PlayerStateManager)
	{
		InfoTextString = Announcement::WinString;
	}
	else if (Players.Num() == 1)
	{
		InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *Players[0]->GetPlayerName());
	}
	else if (Players.Num() > 1)
	{
		InfoTextString = Announcement::TiedString;
		InfoTextString.Append(FString("\n"));
		for (auto TiedPlayer : Players)
		{
			InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
		}
	}
	return InfoTextString;
}

FString APlayerControllerManager::GetTeamsInfoText(AGameStateManager* GameStateManager)
{
	if (GameStateManager == nullptr) return FString();
	FString InfoTextString;

	const int32 BlackTeamScore = GameStateManager->VanguardSovereigntyScore;
	const int32 WhiteTeamScore = GameStateManager->GenesisPactScore;

	if (BlackTeamScore == 0 && WhiteTeamScore == 0)
	{
		InfoTextString = Announcement::LoseString;
	}
	else if (BlackTeamScore == WhiteTeamScore)
	{
		InfoTextString = FString::Printf(TEXT("%s\n"), *Announcement::TeamTiedString);
		InfoTextString.Append(Announcement::WhiteTeam);
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(Announcement::BlackTeam);
		InfoTextString.Append(TEXT("\n"));
	}
	else if (BlackTeamScore > WhiteTeamScore)
	{
		InfoTextString = Announcement::BlackTeamWinString;
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlackTeam, BlackTeamScore));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::WhiteTeam, WhiteTeamScore));
	}
	else if (WhiteTeamScore > BlackTeamScore)
	{
		InfoTextString = Announcement::WhiteTeamWinString;
		InfoTextString.Append(TEXT("\n"));

		InfoTextString.Append(FString::Printf(TEXT("%s :%d\n"), *Announcement::WhiteTeam, WhiteTeamScore));
		InfoTextString.Append(FString::Printf(TEXT("%s :%d\n"), *Announcement::BlackTeam, BlackTeamScore));
	}

	return InfoTextString;
}



void APlayerControllerManager::SetHUDEliminatedWindow()
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter && BaseCharacter->GetPlayerIsEliminated())
	{	
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
		if (PlayerHUD && PlayerHUD->DisplayManager)
		{
			// HealthBar
			if (PlayerHUD->DisplayManager->HealthBar->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HealthBar->SetVisibility(ESlateVisibility::Hidden);
			}

			// ShieldBar
			if (PlayerHUD->DisplayManager->ShieldBar->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->ShieldBar->SetVisibility(ESlateVisibility::Hidden);
			}

			// HealthText
			if (PlayerHUD->DisplayManager->HealthText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HealthText->SetVisibility(ESlateVisibility::Hidden);
			}

			// HeaderHealth
			if (PlayerHUD->DisplayManager->HeaderHealth->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HeaderHealth->SetVisibility(ESlateVisibility::Hidden);
			}

			// ShieldText
			if (PlayerHUD->DisplayManager->ShieldText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->ShieldText->SetVisibility(ESlateVisibility::Hidden);
			}

			// HeaderShield
			if (PlayerHUD->DisplayManager->HeaderShield->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HeaderShield->SetVisibility(ESlateVisibility::Hidden);
			}

			// ScoreAmount
			if (PlayerHUD->DisplayManager->ScoreAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->ScoreAmount->SetVisibility(ESlateVisibility::Hidden);
			}

			// ScoreText
			if (PlayerHUD->DisplayManager->ScoreText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->ScoreText->SetVisibility(ESlateVisibility::Hidden);
			}

			// DeathsAmount
			if (PlayerHUD->DisplayManager->DeathsAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->DeathsAmount->SetVisibility(ESlateVisibility::Hidden);
			}

			// DeathsText
			if (PlayerHUD->DisplayManager->DeathsText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->DeathsText->SetVisibility(ESlateVisibility::Hidden);
			}

			// WeaponTypeText
			if (PlayerHUD->DisplayManager->WeaponTypeText->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->WeaponTypeText->SetVisibility(ESlateVisibility::Hidden);
			}

			// WeaponAmmoAmount
			if (PlayerHUD->DisplayManager->WeaponAmmoAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->WeaponAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			}

			// WeaponCrosshairAmmoAmount
			if (PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			}

			// AmmoMiddle
			if (PlayerHUD->DisplayManager->AmmoMiddle->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->AmmoMiddle->SetVisibility(ESlateVisibility::Hidden);
			}

			// CarriedAmmoAmount
			if (PlayerHUD->DisplayManager->CarriedAmmoAmount->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->CarriedAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			}

			// DeathNote
			if (PlayerHUD->DisplayManager->DeathNote->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->DeathNote->SetVisibility(ESlateVisibility::Visible);
			}

			// VanguardSovereigntyScore
			if (PlayerHUD->DisplayManager->BlackTeamScore->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->BlackTeamScore->SetVisibility(ESlateVisibility::Visible);
			}

			if (PlayerHUD->DisplayManager->WhiteTeamScore->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->WhiteTeamScore->SetVisibility(ESlateVisibility::Visible);
			}

			if (PlayerHUD->DisplayManager->ScoreSpacerText->GetVisibility() != ESlateVisibility::Visible)
			{
				PlayerHUD->DisplayManager->ScoreSpacerText->SetVisibility(ESlateVisibility::Visible);
			}

			if (PlayerHUD->DisplayManager->HealhtShieldBackgroundFrame->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->HealhtShieldBackgroundFrame->SetVisibility(ESlateVisibility::Hidden);
			}

			if (PlayerHUD->DisplayManager->WeaponBackgroundFrame->GetVisibility() != ESlateVisibility::Hidden)
			{
				PlayerHUD->DisplayManager->WeaponBackgroundFrame->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void APlayerControllerManager::SetHUDInGameMenuOpened()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD)
	{
		if (PlayerHUD->DisplayManager->HealthBar->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->HealthBar->SetVisibility(ESlateVisibility::Hidden);
			bCacheHealthBar = true;
		}
		if (PlayerHUD->DisplayManager->ShieldBar->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->ShieldBar->SetVisibility(ESlateVisibility::Hidden);
			bCacheShieldBar = true;
		}

		if (PlayerHUD->DisplayManager->HealthText->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->HealthText->SetVisibility(ESlateVisibility::Hidden);
			bCacheHealthText = true;
		}

		if (PlayerHUD->DisplayManager->HeaderHealth->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->HeaderHealth->SetVisibility(ESlateVisibility::Hidden);
			bCacheHeaderHealth = true;
		}

		if (PlayerHUD->DisplayManager->ShieldText->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->ShieldText->SetVisibility(ESlateVisibility::Hidden);
			bCacheShieldText = true;
		}

		if (PlayerHUD->DisplayManager->HeaderShield->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->HeaderShield->SetVisibility(ESlateVisibility::Hidden);
			bCacheHeaderShield = true;

		}

		if (PlayerHUD->DisplayManager->ScoreAmount->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->ScoreAmount->SetVisibility(ESlateVisibility::Hidden);
			bCacheScoreAmount = true;

		}

		if (PlayerHUD->DisplayManager->ScoreText->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->ScoreText->SetVisibility(ESlateVisibility::Hidden);
			bCacheScoreText = true;

		}

		if (PlayerHUD->DisplayManager->DeathsAmount->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->DeathsAmount->SetVisibility(ESlateVisibility::Hidden);
			bCacheDeathsAmount = true;

		}

		if (PlayerHUD->DisplayManager->DeathsText->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->DeathsText->SetVisibility(ESlateVisibility::Hidden);
			bCacheDeathsText = true;

		}

		if (PlayerHUD->DisplayManager->WeaponTypeText->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->WeaponTypeText->SetVisibility(ESlateVisibility::Hidden);
			bCacheWeaponTypeText = true;

		}

		if (PlayerHUD->DisplayManager->WeaponAmmoAmount->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->WeaponAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			bCacheWeaponAmmoAmount = true;
		}

		if (PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			bCacheWeaponCrosshairAmmoAmount = true;
		}

		if (PlayerHUD->DisplayManager->AmmoMiddle->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->AmmoMiddle->SetVisibility(ESlateVisibility::Hidden);
			bCacheAmmoMiddle = true;
		}

		if (PlayerHUD->DisplayManager->CarriedAmmoAmount->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->CarriedAmmoAmount->SetVisibility(ESlateVisibility::Hidden);
			bCacheCarriedAmmoAmount = true;
		}

		if (PlayerHUD->DisplayManager->MatchCountDownText->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->MatchCountDownText->SetVisibility(ESlateVisibility::Hidden);
			bCacheMatchCountDownText = true;
		}

		if (PlayerHUD->DisplayManager->EquipWeaponText->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->EquipWeaponText->SetVisibility(ESlateVisibility::Hidden);
			bCacheEquipWeaponText = true;
		}

		if (PlayerHUD->DisplayManager->BlackTeamScore->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->BlackTeamScore->SetVisibility(ESlateVisibility::Hidden);
			bCacheBlackTeamScore = true;
		}

		if (PlayerHUD->DisplayManager->WhiteTeamScore->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->WhiteTeamScore->SetVisibility(ESlateVisibility::Hidden);
			bCacheWhiteTeamScore = true;
		}

		if (PlayerHUD->DisplayManager->ScoreSpacerText->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->ScoreSpacerText->SetVisibility(ESlateVisibility::Hidden);
			bCacheScoreSpacerText = true;
		}

		if (PlayerHUD->DisplayManager->HealhtShieldBackgroundFrame->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->HealhtShieldBackgroundFrame->SetVisibility(ESlateVisibility::Hidden);
			bCacheHealhtShieldBackgroundFrame = true;
		}

		if (PlayerHUD->DisplayManager->WeaponBackgroundFrame->GetVisibility() == ESlateVisibility::Visible)
		{
			PlayerHUD->DisplayManager->WeaponBackgroundFrame->SetVisibility(ESlateVisibility::Hidden);
			bCacheWeaponBackgroundFrame = true;
		}
	}
}

void APlayerControllerManager::SetHUDInGameMenuClosed()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD)
	{
		if (bCacheHealthBar)
		{
			PlayerHUD->DisplayManager->HealthBar->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheShieldBar)
		{
			PlayerHUD->DisplayManager->ShieldBar->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheHealthText)
		{
			PlayerHUD->DisplayManager->HealthText->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheHeaderHealth)
		{
			PlayerHUD->DisplayManager->HeaderHealth->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheShieldText)
		{
			PlayerHUD->DisplayManager->ShieldText->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheHeaderShield)
		{
			PlayerHUD->DisplayManager->HeaderShield->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheScoreAmount)
		{
			PlayerHUD->DisplayManager->ScoreAmount->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheScoreText)
		{
			PlayerHUD->DisplayManager->ScoreText->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheDeathsAmount)
		{
			PlayerHUD->DisplayManager->DeathsAmount->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheDeathsText)
		{
			PlayerHUD->DisplayManager->DeathsText->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheWeaponTypeText)
		{
			PlayerHUD->DisplayManager->WeaponTypeText->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheWeaponAmmoAmount)
		{
			PlayerHUD->DisplayManager->WeaponAmmoAmount->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheWeaponCrosshairAmmoAmount)
		{
			PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheAmmoMiddle)
		{
			PlayerHUD->DisplayManager->AmmoMiddle->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheCarriedAmmoAmount)
		{
			PlayerHUD->DisplayManager->CarriedAmmoAmount->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheMatchCountDownText)
		{
			PlayerHUD->DisplayManager->MatchCountDownText->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheEquipWeaponText)
		{
			PlayerHUD->DisplayManager->EquipWeaponText->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheBlackTeamScore)
		{
			PlayerHUD->DisplayManager->BlackTeamScore->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheWhiteTeamScore)
		{
			PlayerHUD->DisplayManager->WhiteTeamScore->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheScoreSpacerText)
		{
			PlayerHUD->DisplayManager->ScoreSpacerText->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheHealhtShieldBackgroundFrame)
		{
			PlayerHUD->DisplayManager->HealhtShieldBackgroundFrame->SetVisibility(ESlateVisibility::Visible);
		}
		if (bCacheWeaponBackgroundFrame)
		{
			PlayerHUD->DisplayManager->WeaponBackgroundFrame->SetVisibility(ESlateVisibility::Visible);
		}
		LOG_WARNING("DONE");
		bCacheHealthBar = false;
		bCacheShieldBar = false;
		bCacheHealthText = false;
		bCacheHeaderHealth = false;
		bCacheShieldText = false;
		bCacheHeaderShield = false;
		bCacheScoreAmount = false;
		bCacheScoreText = false;
		bCacheDeathsAmount = false;
		bCacheDeathsText = false;
		bCacheWeaponTypeText = false;
		bCacheWeaponAmmoAmount = false;
		bCacheWeaponCrosshairAmmoAmount = false;
		bCacheAmmoMiddle = false;
		bCacheCarriedAmmoAmount = false;
		bCacheMatchCountDownText = false;
		bCacheEquipWeaponText = false;
		bCacheDeathNote = false;
		bCacheBlackTeamScore = false;
		bCacheWhiteTeamScore = false;
		bCacheScoreSpacerText = false;
		bCacheHealhtShieldBackgroundFrame = false;
		bCacheWeaponBackgroundFrame = false;
	}
}
#pragma endregion

#pragma region ReplayPlayerController
void AReplayPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// The state may go invalid at any time due to scrubbing during a replay
	if (!IsValid(FollowedPlayerState))
	{
		UWorld* World = GetWorld();

		// Listen for changes for both recording and playback
		if (AGameStateManager* GameState = Cast<AGameStateManager>(World->GetGameState()))
		{
			
		}

	}
}

void AReplayPlayerController::SmoothTargetViewRotation(APawn* TargetPawn, float DeltaSeconds)
{
	// Default behavior is to interpolate to TargetViewRotation which is set from APlayerController::TickActor but it's not very smooth
	Super::SmoothTargetViewRotation(TargetPawn, DeltaSeconds);
}

bool AReplayPlayerController::ShouldRecordClientReplay()
{
	return false;
}
void AReplayPlayerController::RecorderPlayerStateUpdated(APlayerState* NewRecorderPlayerState)
{
	if (NewRecorderPlayerState)
	{
		FollowedPlayerState = NewRecorderPlayerState;

		NewRecorderPlayerState->OnPawnSet.AddUniqueDynamic(this, &AReplayPlayerController::OnPlayerStatePawnSet);
		OnPlayerStatePawnSet(NewRecorderPlayerState, NewRecorderPlayerState->GetPawn(), nullptr);
	}
}
void AReplayPlayerController::OnPlayerStatePawnSet(APlayerState* ChangedPlayerState, APawn* NewPlayerPawn, APawn* OldPlayerPawn)
{
	if (ChangedPlayerState == FollowedPlayerState)
	{
		SetViewTarget(NewPlayerPawn);
	}
}
#pragma endregion
