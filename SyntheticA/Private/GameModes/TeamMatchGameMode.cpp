// @2023 All rights reversed by Reverse-Alpha Studios


#include "GameModes/TeamMatchGameMode.h"
#include "GameState/GameStateManager.h"
#include "PlayerController/PlayerControllerManager.h"
#include "PlayerState/PlayerStateManager.h"
#include "Kismet/GameplayStatics.h"

#include "NewTypes/TeamTypes.h"

ATeamMatchGameMode::ATeamMatchGameMode()
{
	bTeamMatch = true;
}

void ATeamMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AGameStateManager* GameStateManager = Cast<AGameStateManager>(UGameplayStatics::GetGameState(this));

	if (GameStateManager)
	{
		APlayerStateManager* PlayerStateManager = NewPlayer->GetPlayerState<APlayerStateManager>();

		if (PlayerStateManager && PlayerStateManager->GetTeam() == ETeam::ET_NoTeam)
		{
			if (GameStateManager->GenesisPactTeam.Num() >= GameStateManager->VanguardSovereigntyTeam.Num())
			{
				GameStateManager->VanguardSovereigntyTeam.AddUnique(PlayerStateManager);
				PlayerStateManager->SetTeam(ETeam::ET_GenesisPact);
			}
			else
			{
				GameStateManager->GenesisPactTeam.AddUnique(PlayerStateManager);
				PlayerStateManager->SetTeam(ETeam::ET_VanguardSovereignty);
			}
		}
	}
}

void ATeamMatchGameMode::Logout(AController* Exiting)
{
	AGameStateManager* GameStateManager = Cast<AGameStateManager>(UGameplayStatics::GetGameState(this));

	APlayerStateManager* PlayerStateManager = Exiting->GetPlayerState<APlayerStateManager>();

	if (GameStateManager && PlayerStateManager)
	{

		if (GameStateManager->GenesisPactTeam.Contains(PlayerStateManager))
		{
			GameStateManager->GenesisPactTeam.Remove(PlayerStateManager);
		}

		if (GameStateManager->VanguardSovereigntyTeam.Contains(PlayerStateManager))
		{
			GameStateManager->VanguardSovereigntyTeam.Remove(PlayerStateManager);
		}
	}
}

void ATeamMatchGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	AGameStateManager* GameStateManager = Cast<AGameStateManager>(UGameplayStatics::GetGameState(this));

	if (GameStateManager)
	{
		for (auto PLayerState : GameStateManager->PlayerArray)
		{
			APlayerStateManager* PlayerStateManager = Cast<APlayerStateManager>(PLayerState.Get());

			if (PlayerStateManager && PlayerStateManager->GetTeam() == ETeam::ET_NoTeam)
			{
				if (GameStateManager->GenesisPactTeam.Num() >= GameStateManager->VanguardSovereigntyTeam.Num())
				{
					GameStateManager->GenesisPactTeam.AddUnique(PlayerStateManager);
					PlayerStateManager->SetTeam(ETeam::ET_GenesisPact);
				}
				else
				{
					GameStateManager->VanguardSovereigntyTeam.AddUnique(PlayerStateManager);
					PlayerStateManager->SetTeam(ETeam::ET_VanguardSovereignty);
				}
			}
		}
	}
}

float ATeamMatchGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	APlayerStateManager* AttackerPlayerState = Attacker->GetPlayerState<APlayerStateManager>();
	APlayerStateManager* VictimPlayerState   = Victim->GetPlayerState<APlayerStateManager>();

	if (AttackerPlayerState == nullptr || VictimPlayerState == nullptr) return BaseDamage;

	if (VictimPlayerState == AttackerPlayerState)
	{
		return BaseDamage;
	}

	if (AttackerPlayerState->GetTeam() == VictimPlayerState->GetTeam())
	{
		return 0.0f;
	}

	return BaseDamage;
}

void ATeamMatchGameMode::PlayerEliminated(ABaseCharacter* EliminatedCharacter, APlayerControllerManager* VictimController, APlayerControllerManager* AttackerController)
{
	Super::PlayerEliminated(EliminatedCharacter, VictimController, AttackerController);

	AGameStateManager* GameStateManager = Cast<AGameStateManager>(UGameplayStatics::GetGameState(this));
	APlayerStateManager* AttackerPlayerState = AttackerController ? Cast<APlayerStateManager>(AttackerController->PlayerState) : nullptr;

	if (GameStateManager && AttackerPlayerState)
	{
		if (AttackerPlayerState->GetTeam() == ETeam::ET_VanguardSovereignty)
		{
			GameStateManager->VanguardSovereigntyScores();
		}

		if (AttackerPlayerState->GetTeam() == ETeam::ET_GenesisPact)
		{
			GameStateManager->GenesisPactScores();
		}
	}
}
