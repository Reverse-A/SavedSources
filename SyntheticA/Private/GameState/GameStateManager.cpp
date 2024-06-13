// @2023 All rights reversed by Reverse-Alpha Studios


#include "GameState/GameStateManager.h"
#include "PlayerController/PlayerControllerManager.h"
#include "PlayerState/PlayerStateManager.h"

#include "Net/UnrealNetwork.h"

void AGameStateManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateManager, TopScoringPlayers);
	DOREPLIFETIME(AGameStateManager, VanguardSovereigntyScore);
	DOREPLIFETIME(AGameStateManager, GenesisPactScore);

}

void AGameStateManager::UpdateTopScore(APlayerStateManager* ScoringPlayer)
{
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if(ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void AGameStateManager::VanguardSovereigntyScores()
{
	VanguardSovereigntyScore++;

	APlayerControllerManager* PlayerControllerManager= Cast<APlayerControllerManager>(GetWorld()->GetFirstPlayerController());

	if (PlayerControllerManager)
	{
		PlayerControllerManager->Set_HUDVanguardSovereigntyScore(VanguardSovereigntyScore);
	}
}

void AGameStateManager::GenesisPactScores()
{
	GenesisPactScore++;

	APlayerControllerManager* PlayerControllerManager = Cast<APlayerControllerManager>(GetWorld()->GetFirstPlayerController());

	if (PlayerControllerManager)
	{
		PlayerControllerManager->Set_HUDGenesisPactScore(GenesisPactScore);
	}
}

void AGameStateManager::OnRep_VanguardSovereigntyScore()
{
	APlayerControllerManager* PlayerControllerManager = Cast<APlayerControllerManager>(GetWorld()->GetFirstPlayerController());

	if (PlayerControllerManager)
	{
		PlayerControllerManager->Set_HUDVanguardSovereigntyScore(VanguardSovereigntyScore);
	}
}

void AGameStateManager::OnRep_GenesisPactScore()
{
	APlayerControllerManager* PlayerControllerManager = Cast<APlayerControllerManager>(GetWorld()->GetFirstPlayerController());

	if (PlayerControllerManager)
	{
		PlayerControllerManager->Set_HUDGenesisPactScore(GenesisPactScore);
	}
}