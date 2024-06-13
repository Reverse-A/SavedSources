// ©2023 Reverse - A Company. All rights reserved.


#include "GameModes/BaseGameMode.h"

#include "Characters/BaseCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameState/GameStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerManager.h"
#include "PlayerState/PlayerStateManager.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}
#pragma region Main/Functions
ABaseGameMode::ABaseGameMode()
{
	bDelayedStart = true;
}


void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ABaseGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Handle_MatchStates();
}
#pragma endregion

#pragma region MatchState

void ABaseGameMode::Handle_MatchStates()
{
	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.0f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;

		if (CountdownTime <= 0.0f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
	else if (MatchState == MatchState::LeavingMap)
	{

	}
}

void ABaseGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerControllerManager* PlayerController = Cast<APlayerControllerManager>(*It);

		if (PlayerController)
		{
			PlayerController->OnMatchStateSet(MatchState, bTeamMatch);
		}
	}
}
#pragma endregion

#pragma region Elimination
void ABaseGameMode::PlayerEliminated(ABaseCharacter* EliminatedCharacter, APlayerControllerManager* VictimController, APlayerControllerManager* AttackerController)
{
	if (AttackerController	== nullptr || AttackerController->PlayerState	== nullptr) return;
	if (VictimController	== nullptr || VictimController	->PlayerState	== nullptr) return;

	APlayerStateManager* AttackerPlayerState	= AttackerController ? Cast<APlayerStateManager>(AttackerController	->PlayerState)	: nullptr;
	APlayerStateManager* VictimPlayerState		= VictimController	 ? Cast<APlayerStateManager>(VictimController	->PlayerState)	: nullptr;

	AGameStateManager* GameStateManager = GetGameState<AGameStateManager>();

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && GameStateManager)
	{
		TArray<APlayerStateManager*> PlayersCurrentlyInTheLead;

		for (auto LeadPlayer : GameStateManager->TopScoringPlayers)
		{
			PlayersCurrentlyInTheLead.Add(LeadPlayer);
		}

		AttackerPlayerState->AddToKills(1);
		GameStateManager->UpdateTopScore(AttackerPlayerState);

		if (GameStateManager->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			ABaseCharacter* Leader = Cast<ABaseCharacter>(AttackerPlayerState->GetPawn());
			if (Leader)
			{
				//TODO Leader 
			}
		}

		for (int32 i = 0; i < PlayersCurrentlyInTheLead.Num(); i++)
		{
			if (!GameStateManager->TopScoringPlayers.Contains(PlayersCurrentlyInTheLead[i]))
			{
				ABaseCharacter* NonLeader = Cast<ABaseCharacter>(PlayersCurrentlyInTheLead[i]->GetPawn());

				if (NonLeader)
				{
					//TODO NonLeader 
				}
			}
		}
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDeaths(1);
	}

	if (EliminatedCharacter)
	{
		EliminatedCharacter->Elimination(false);
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		APlayerControllerManager* PlayerControllerIT = Cast<APlayerControllerManager>(*Iterator);

		if (PlayerControllerIT && AttackerPlayerState && VictimPlayerState)
		{
			PlayerControllerIT->Broadcast_Elimination(AttackerPlayerState, VictimPlayerState);
		}
	}
}

void ABaseGameMode::RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController)
{
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Reset();
		EliminatedCharacter->Destroy();
	}

	if (EliminatedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);

		RestartPlayerAtPlayerStart(EliminatedController, PlayerStarts[Selection]);
	}
}
#pragma region 
void ABaseGameMode::PlayerLeftGame(class APlayerStateManager* PlayerLeaving)
{
	if (PlayerLeaving == nullptr) return;

	AGameStateManager* GameStateManager = GetGameState<AGameStateManager>();
	
	if (GameStateManager && GameStateManager->TopScoringPlayers.Contains(PlayerLeaving))
	{
		GameStateManager->TopScoringPlayers.Remove(PlayerLeaving);
	}

	ABaseCharacter* CharacterLeaving = Cast<ABaseCharacter>(PlayerLeaving->GetPawn());
	if (CharacterLeaving)
	{
		CharacterLeaving->Elimination(true);
	}
}
#pragma endregion
#pragma region Damage
float ABaseGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	return BaseDamage;
}
#pragma endregion