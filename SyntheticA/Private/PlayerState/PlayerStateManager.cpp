// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/PlayerStateManager.h"

#include "Characters/BaseCharacter.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Net/UnrealNetwork.h"

APlayerStateManager::APlayerStateManager(const FObjectInitializer& OBjectInitializer)
	: Super(OBjectInitializer)
{

}

APlayerControllerManager* APlayerStateManager::GetPlayerControllerManager() const
{
	return Cast<APlayerControllerManager>(GetOwner());
}

void APlayerStateManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerStateManager, Kills);
	DOREPLIFETIME(APlayerStateManager, Assists);
	DOREPLIFETIME(APlayerStateManager, Deaths);
	DOREPLIFETIME(APlayerStateManager, Team);

}

void APlayerStateManager::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void APlayerStateManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		// TODO: Initialize components or classes in there, if needed.!
	}
}

void APlayerStateManager::Reset()
{
	Super::Reset();
}
void APlayerStateManager::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

#pragma region AddScore
void APlayerStateManager::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDScore(GetScore());
		}
	}
}

void APlayerStateManager::OnRep_Score()
{
	Super::OnRep_Score();

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDScore(GetScore());
		}
	}
}
#pragma endregion

#pragma region Kills
void APlayerStateManager::AddToKills(int32 KillAmount)
{
	Kills += KillAmount;
	float KillScore = 1000;
	AddToScore(KillAmount * KillScore);

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDKills(Kills);
		}

	}
}


void APlayerStateManager::OnRep_Kills()
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDKills(Kills);
		}
	}
}
#pragma endregion

#pragma region Assists	
void APlayerStateManager::AddToAssists(int32 AssistAmount)
{
	Assists += AssistAmount;
	float AssistScore = 500;

	AddToScore(AssistAmount * AssistScore);

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDAssists(Assists);
		}
	}
}
void APlayerStateManager::OnRep_Assists()
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDAssists(Assists);
		}
	}
}
#pragma endregion

#pragma region Deaths

void APlayerStateManager::AddToDeaths(int32 DeathAmount)
{
	Deaths += DeathAmount;
	float DeathScore = -500;
	AddToScore(DeathAmount * DeathScore);

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDDeaths(Deaths);
		}
	}
}




void APlayerStateManager::OnRep_Deaths()
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDDeaths(Deaths);
		}
	}
}
#pragma endregion
void APlayerStateManager::SetTeam(ETeam TeamToSet)
{
	Team = TeamToSet;

	BaseCharacter = Cast<ABaseCharacter>(GetPawn());

	if (BaseCharacter)
	{
		BaseCharacter->SetTeamColor(Team);
	}
}

void APlayerStateManager::OnRep_Team()
{
	BaseCharacter = Cast<ABaseCharacter>(GetPawn());

	if (BaseCharacter)
	{
		BaseCharacter->SetTeamColor(Team);
	}
}


#pragma region PlayerConnectionType
void APlayerStateManager::OnDeactivated()
{
	bool bDestroyDeactivatedPlayerState = false;

	switch (GetPlayerConnectionType())
	{
	case EPlayerConnectionType::Player:
	case EPlayerConnectionType::InactivePlayer:
		//@TODO: Ask the experience if we should destroy disconnecting players immediately or leave them around
		// (e.g., for long running servers where they might build up if lots of players cycle through)
		// This PlayerConnectionType took from Lyragame example for future use, can be checked from there.
		bDestroyDeactivatedPlayerState = true;
		break;
	default:
		bDestroyDeactivatedPlayerState = true;
		break;
	}

	SetPlayerConnectionType(EPlayerConnectionType::InactivePlayer);

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void APlayerStateManager::OnReactivated()
{
	if (GetPlayerConnectionType() == EPlayerConnectionType::InactivePlayer)
	{
		SetPlayerConnectionType(EPlayerConnectionType::Player);
	}
}

void APlayerStateManager::SetPlayerConnectionType(EPlayerConnectionType NewType)
{
	PlayerConnectionType = NewType;
}
#pragma endregion
