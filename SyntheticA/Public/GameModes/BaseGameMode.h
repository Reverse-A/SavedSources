// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseGameMode.generated.h"

namespace MatchState
{
	extern SYNTHETICA_API const FName Cooldown; // Match duration has been reached. Display winner and begin cooldown timer.
}
#pragma region BaseGameMode
UCLASS()
class SYNTHETICA_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()
	
#pragma region Main/Functions
public:
	ABaseGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void Handle_MatchStates();
#pragma endregion
#pragma region Match/Statement
public:
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 90.0f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.0f;

	float LevelStartingTime = 0.0f;

	float CountdownTime = 0.0f;

/*Funcs*/
	virtual void OnMatchStateSet() override;

	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }

#pragma endregion
#pragma region Elimination
	virtual void PlayerEliminated(class ABaseCharacter* EliminatedCharacter, class APlayerControllerManager* VictimController, APlayerControllerManager* AttackerController);
	virtual void RequestRespawn(class ACharacter* EliminatedCharacter, AController* EliminatedController);
#pragma endregion
#pragma region Gameplay
	void PlayerLeftGame(class APlayerStateManager* PlayerLeaving);

protected:
	bool bTeamMatch = false;

#pragma endregion
#pragma region Damage
public:
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

#pragma endregion
};
#pragma endregion