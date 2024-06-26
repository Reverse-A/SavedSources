// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CoreGameMode.generated.h"

namespace MatchState
{
	extern SYNTHETICA_API const FName Cooldown; // Match duration has been reached. Display winner and begin cooldown timer.

}
/**
 * 
 */
UCLASS()
class SYNTHETICA_API ACoreGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACoreGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnMatchStateSet() override;

	virtual void PlayerEliminated(class ABaseCharacter* EliminatedCharacter, class APlayerControllerManager* VictimController, APlayerControllerManager* AttackerController);
	virtual void RequestRespawn(class ACharacter* EliminatedCharacter, AController* EliminatedController);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 7.0f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 5.0f;

	float LevelStartingTime = 0.0f;


private:
	float CountdownTime = 0.0f;

public:
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
};
