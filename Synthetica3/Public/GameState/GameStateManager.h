// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStateManager.generated.h"

class APlayerStateManager;

UCLASS()
class SYNTHETICA_API AGameStateManager : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateTopScore(APlayerStateManager* ScoringPlayer);

	UPROPERTY(Replicated)
	TArray<APlayerStateManager*> TopScoringPlayers;

protected:

private:
	float TopScore = 0.0f;
};
