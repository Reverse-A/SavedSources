// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameModes/BaseGameMode.h"
#include "TeamMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ATeamMatchGameMode : public ABaseGameMode
{
	GENERATED_BODY()

protected:
	ATeamMatchGameMode();
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(class ABaseCharacter* EliminatedCharacter, class APlayerControllerManager* VictimController, APlayerControllerManager* AttackerController) override;
protected:
	virtual void HandleMatchHasStarted() override;
};
