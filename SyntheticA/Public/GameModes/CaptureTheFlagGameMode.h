// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameModes/TeamMatchGameMode.h"
#include "CaptureTheFlagGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ACaptureTheFlagGameMode : public ATeamMatchGameMode
{
	GENERATED_BODY()

public:
	virtual void PlayerEliminated(class ABaseCharacter* EliminatedCharacter, class APlayerControllerManager* VictimController, APlayerControllerManager* AttackerController) override; 

	void FlagCapturedScore(class AFlag* Flag);

public:		
};