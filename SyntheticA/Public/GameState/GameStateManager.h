// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStateManager.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AGameStateManager : public AGameState
{
	GENERATED_BODY()

private:
	float TopScore = 0.0f;


protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void UpdateTopScore(class APlayerStateManager* ScoringPlayer);

	UPROPERTY(Replicated)
	TArray<APlayerStateManager*> TopScoringPlayers;

	/*
	*  Teams
	*/

	UPROPERTY()
	TArray<APlayerStateManager*> VanguardSovereigntyTeam;
	UPROPERTY()
	TArray<APlayerStateManager*> GenesisPactTeam;

	void VanguardSovereigntyScores();
	void GenesisPactScores();
	
	UPROPERTY(ReplicatedUsing = OnRep_VanguardSovereigntyScore)
	float VanguardSovereigntyScore = 0.0f;

	UPROPERTY(ReplicatedUsing = OnRep_GenesisPactScore)
	float GenesisPactScore = 0.0f;

	UFUNCTION()
	void OnRep_VanguardSovereigntyScore();

	UFUNCTION()
	void OnRep_GenesisPactScore();
};
