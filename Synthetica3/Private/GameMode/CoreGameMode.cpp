// ©2023 Reverse - A Company. All rights reserved.


#include "GameMode/CoreGameMode.h"

#include "Character/BaseCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerManager.h"
#include "PlayerState/PlayerStateManager.h"
#include "GameState/GameStateManager.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ACoreGameMode::ACoreGameMode()
{
	bDelayedStart = true;	
}

void ACoreGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ACoreGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		if (CountdownTime <= 0.0f)
		{
			RestartGame();
		}
	}
}

void ACoreGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerControllerManager* PlayerController = Cast<APlayerControllerManager>(*It);
		if (PlayerController)
		{
			PlayerController->OnMatchStateSet(MatchState);
		}
	}
}

void ACoreGameMode::PlayerEliminated(ABaseCharacter* EliminatedCharacter, APlayerControllerManager* VictimController, APlayerControllerManager* AttackerController)
{
	if (AttackerController	== nullptr || AttackerController->PlayerState	== nullptr) return;
	if (VictimController	== nullptr || VictimController->PlayerState		== nullptr) return;

	APlayerStateManager* AttackerPlayerState = AttackerController ? Cast<APlayerStateManager>(AttackerController->PlayerState) : nullptr;
	APlayerStateManager* VictimPlayerState   = VictimController   ? Cast<APlayerStateManager>(VictimController->PlayerState)   : nullptr;

	AGameStateManager* GameStateManager = GetGameState<AGameStateManager>();

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && GameStateManager)
	{
		AttackerPlayerState->AddToScore(1.0f);
		GameStateManager->UpdateTopScore(AttackerPlayerState);
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

	if (EliminatedCharacter)
	{
		EliminatedCharacter->Elimination();
	}
}

void ACoreGameMode::RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController)
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