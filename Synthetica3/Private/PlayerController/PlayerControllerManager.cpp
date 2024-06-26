// ©2023 Reverse - A Company. All rights reserved.


#include "PlayerController/PlayerControllerManager.h"

#include "Character/BaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerHUDPanel.h"
#include "HUD/Announcement.h"
#include "GameMode/CoreGameMode.h"
#include "GameState/GameStateManager.h"
#include "PlayerState/PlayerStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void APlayerControllerManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = Cast<APlayerHUD>(GetHUD());
	Server_CheckMatchState();
}

void APlayerControllerManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerControllerManager, MatchState);
}

void APlayerControllerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
}

void APlayerControllerManager::PollInit()
{
	if (PlayerHUDPanel == nullptr)
	{
		if (PlayerHUD && PlayerHUD->PlayerHUDPanel)
		{
			PlayerHUDPanel = PlayerHUD->PlayerHUDPanel;
			if (PlayerHUDPanel)
			{
				SetHUDHealth(HUDHealth, HUDMaxHealth);
				SetHUDScore(HUDScore);
				SetHUDDefeats(HUDDefeats);
			}
		}
	}
}

void APlayerControllerManager::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(InPawn);

	if (BaseCharacter)
	{
		SetHUDHealth(BaseCharacter->GetHealth(), BaseCharacter->GetMaxHealth());
	}
}

void APlayerControllerManager::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;

	if (IsLocalController() && TimeSyncRunningTime >= TimeSyncFrequency)
	{
		Server_RequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.0f;
	}
}

void APlayerControllerManager::Server_CheckMatchState_Implementation()
{
	ACoreGameMode* GameMode = Cast<ACoreGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		WarmupTime			= GameMode->WarmupTime;
		MatchTime			= GameMode->MatchTime;
		CooldownTime		= GameMode->CooldownTime;
		LevelStartingTime	= GameMode->LevelStartingTime;
		MatchState			= GameMode->GetMatchState();

		Client_JoinMiddleGame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void APlayerControllerManager::Client_JoinMiddleGame_Implementation(FName StateOfMatch, float InputWarmup, float InputMatchTime, float Cooldown, float InputStartingTime)
{
	WarmupTime			= InputWarmup;
	MatchTime			= InputMatchTime;
	CooldownTime		= Cooldown;
	LevelStartingTime	= InputStartingTime;
	MatchState			= StateOfMatch;
	
	OnMatchStateSet(MatchState);

	if (PlayerHUD && MatchState == MatchState::WaitingToStart)
	{
		PlayerHUD->AddAnnouncement();
	}
}

#pragma region | HUD Settings Functions |
void APlayerControllerManager::SetHUDHealth(float Health, float MaxHealth)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	bool HUDValid = 
		PlayerHUD								&& 
		PlayerHUD->PlayerHUDPanel				&& 
		PlayerHUD->PlayerHUDPanel->HealthBar	&& 
		PlayerHUD->PlayerHUDPanel->HealthText;

	if (HUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		PlayerHUD->PlayerHUDPanel->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		PlayerHUD->PlayerHUDPanel->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializePlayerHUDPanel	= true;
		HUDHealth					= Health;
		HUDMaxHealth				= MaxHealth;
	}
}

void APlayerControllerManager::SetHUDScore(float Score)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool HUDValid =
		PlayerHUD &&
		PlayerHUD->PlayerHUDPanel &&
		PlayerHUD->PlayerHUDPanel->ScoreAmount;

	if (HUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		PlayerHUD->PlayerHUDPanel->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializePlayerHUDPanel	= true;
		HUDScore					= Score;
	}
}

void APlayerControllerManager::SetHUDDefeats(int32 Defeats)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool HUDValid =
		PlayerHUD &&
		PlayerHUD->PlayerHUDPanel &&
		PlayerHUD->PlayerHUDPanel->DefeatsAmount;

	if (HUDValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		PlayerHUD->PlayerHUDPanel->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		bInitializePlayerHUDPanel	= true;
		HUDDefeats					= Defeats;
	}
}

void APlayerControllerManager::SetHUDWeaponAmmo(int32 Ammo)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool HUDValid =
		PlayerHUD &&
		PlayerHUD->PlayerHUDPanel &&
		PlayerHUD->PlayerHUDPanel->WeaponAmmoAmount;

	if (HUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		PlayerHUD->PlayerHUDPanel->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void APlayerControllerManager::SetHUDCarriedAmmo(int32 Ammo)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool HUDValid =
		PlayerHUD &&
		PlayerHUD->PlayerHUDPanel &&
		PlayerHUD->PlayerHUDPanel->CarriedAmmoAmount;

	if (HUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		PlayerHUD->PlayerHUDPanel->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void APlayerControllerManager::SetHUDTime()
{
	float TimeLeft = 0.f;
	if		(MatchState == MatchState::WaitingToStart)	TimeLeft = WarmupTime	- GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress)		TimeLeft = WarmupTime	+ MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown)		TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (HasAuthority())
	{
		CoreGameMode = CoreGameMode == nullptr ? Cast<ACoreGameMode>(UGameplayStatics::GetGameMode(this)) : CoreGameMode;

		if (CoreGameMode)
		{
			SecondsLeft = FMath::CeilToInt(CoreGameMode->GetCountdownTime() + LevelStartingTime);
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
		SetHUDMatchCountdown(MatchTime - GetServerTime());
	}
	CountdownInt = SecondsLeft;
}

void APlayerControllerManager::SetHUDMatchCountdown(float CountdownTime)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	bool HUDValid =
		PlayerHUD &&
		PlayerHUD->PlayerHUDPanel &&
		PlayerHUD->PlayerHUDPanel->MatchCountDownText;

	if (HUDValid)
	{
		if (CountdownTime < 0.0f)
		{
			PlayerHUD->PlayerHUDPanel->MatchCountDownText->SetText(FText());
			return;
		}
		
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountDownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

		PlayerHUD->PlayerHUDPanel->MatchCountDownText->SetColorAndOpacity(FLinearColor::White);
		PlayerHUD->PlayerHUDPanel->MatchCountDownText->SetText(FText::FromString(CountDownText));
	}
}

void APlayerControllerManager::SetHUDAnnouncementCountdown(float CountdownTime)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;

	bool HUDValid =
		PlayerHUD &&
		PlayerHUD->Announcement &&
		PlayerHUD->Announcement->WarmupTime;

	if (HUDValid)
	{
		if (CountdownTime < 0.f)
		{
			PlayerHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountDownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

		PlayerHUD->Announcement->WarmupTime->SetText(FText::FromString(CountDownText));
	}
}
#pragma endregion

void APlayerControllerManager::Server_RequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	Client_ReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void APlayerControllerManager::Client_ReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime		= GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);

	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}


float APlayerControllerManager::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}


void APlayerControllerManager::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		Server_RequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void APlayerControllerManager::HandleMatchStarted()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD)
	{
		if(PlayerHUD->PlayerHUDPanel == nullptr) PlayerHUD->AddPlayerHUDPanel();
		if (PlayerHUD->Announcement)
		{
			PlayerHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APlayerControllerManager::HandleCooldown()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(GetHUD()) : PlayerHUD;
	if (PlayerHUD)
	{
		PlayerHUD->PlayerHUDPanel->RemoveFromParent();

		bool HUDValid =
			PlayerHUD->Announcement						&&
			PlayerHUD->Announcement->AnnouncementText	&&
			PlayerHUD->Announcement->InfoText;

		if (HUDValid)
		{
			PlayerHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText("New Match starts in:");
			PlayerHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));
			AGameStateManager* GameStateManager			= Cast<AGameStateManager>(UGameplayStatics::GetGameState(this));
			APlayerStateManager* PlayerStateManager		= GetPlayerState<APlayerStateManager>();
			if (GameStateManager && PlayerStateManager)
			{
				TArray<APlayerStateManager*> TopPlayers = GameStateManager->TopScoringPlayers;
				FString InfoTextString;
				if (TopPlayers.Num() == 0)
				{
					InfoTextString = FString("No Winner.");
				}
				else if (TopPlayers.Num() == 1 && TopPlayers[0] == PlayerStateManager)
				{
					InfoTextString = FString("You Win!.");
				}
				else if (TopPlayers.Num() == 1)
				{
					InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *TopPlayers[0]->GetPlayerName());
				}
				else if (TopPlayers.Num() > 1)
				{
					InfoTextString = FString("Players are Tied!. \n");
					for (auto TiedPlayer : TopPlayers)
					{
						InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
					}
				}
				PlayerHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
			}
		}
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	if (BaseCharacter && BaseCharacter->GetCombatComponent())
	{
		if (BaseCharacter->GameplayIsDisabled() != true)
		{
			BaseCharacter->DisableGameplay(true);
			BaseCharacter->GetCombatComponent()->SetFireButtonTrigger(false);
		}
	}
}

void APlayerControllerManager::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		HandleMatchStarted();
	}
	else if(MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}



void APlayerControllerManager::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}


