// @2023 All rights reversed by Reverse-Alpha Studios


#include "GameModes/CaptureTheFlagGameMode.h"

#include "Characters/BaseCharacter.h"
#include "GameModes/CaptureTheFlag/Objects/Flag.h"
#include "GameState/GameStateManager.h"

void ACaptureTheFlagGameMode::PlayerEliminated(ABaseCharacter* EliminatedCharacter, APlayerControllerManager* VictimController, APlayerControllerManager* AttackerController)
{
	ABaseGameMode::PlayerEliminated(EliminatedCharacter, VictimController, AttackerController);
}

void ACaptureTheFlagGameMode::FlagCapturedScore(AFlag* Flag)
{
	AGameStateManager* GameStateManager = Cast<AGameStateManager>(GameState);

	if (GameStateManager)
	{
		if (Flag != nullptr)
		{
			if (Flag->GetTeamFlagColour() == ETeamFlagColour::ETFC_VanguardSovereigntyFlag)
			{
				GameStateManager->VanguardSovereigntyScores();
				Flag->SetTeamFlagColour(ETeamFlagColour::ETFC_NoTeamFlag);
			}

			if (Flag->GetTeamFlagColour() == ETeamFlagColour::ETFC_GenesisPactFlag)
			{
				GameStateManager->GenesisPactScores();
				Flag->SetTeamFlagColour(ETeamFlagColour::ETFC_NoTeamFlag);
			}
		}
	}
}
