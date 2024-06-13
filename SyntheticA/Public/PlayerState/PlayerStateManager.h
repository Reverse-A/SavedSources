// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NewTypes/TeamTypes.h"
#include "PlayerStateManager.generated.h"

class ABaseCharacter;
class APlayerControllerManager;

UENUM()
enum class EPlayerConnectionType : uint8
{
	/* An active participant in the game */
	Player = 0,
	
	/* A viewer connected to a game currently in progress */
	LiveSpectator,

	/* A viewer watching a recorded playback of a game offline */
	ReplaySpectator,

	/* A player who has disconnected or is otherwise inactive */
	InactivePlayer
};

UCLASS(Config = Game)
class SYNTHETICA_API APlayerStateManager : public APlayerState
{
	GENERATED_BODY()
	
#pragma region General
public:
	APlayerStateManager(const FObjectInitializer& OBjectInitializer = FObjectInitializer::Get());
	UFUNCTION(BlueprintCallable, Category = "Synthetica/PlayerState")
	APlayerControllerManager* GetPlayerControllerManager() const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	/* Actor Interface*/
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	/*APlayerState interface*/
	virtual void Reset() override;
	virtual void CopyProperties(APlayerState* PlayerState) override;

private:
	UPROPERTY()
	ABaseCharacter*	BaseCharacter;

	UPROPERTY()	
	APlayerControllerManager*	PlayerControllerManager;


#pragma endregion

#pragma region PlayerConnectionType
private:
	UPROPERTY(Replicated)
	EPlayerConnectionType PlayerConnectionType;

protected:
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;

public:
	void SetPlayerConnectionType(EPlayerConnectionType NewType);
	EPlayerConnectionType GetPlayerConnectionType() const { return PlayerConnectionType; }
#pragma endregion
#pragma region Teams
private:
	UPROPERTY(ReplicatedUsing = OnRep_Team) 
	ETeam Team = ETeam::ET_NoTeam;

	UFUNCTION()
	void OnRep_Team();

public:
	FORCEINLINE ETeam GetTeam() const { return Team; }
	void SetTeam(ETeam TeamToSet);
#pragma endregion

#pragma region Stats
public:
	virtual void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_Kills();
	UFUNCTION()
	virtual void OnRep_Assists();
	UFUNCTION()
	virtual void OnRep_Deaths();

	void AddToScore(float ScoreAmount);
	void AddToKills(int32 KillAmount);
	void AddToAssists(int32 AssistAmount);
	void AddToDeaths(int32 DeathAmount);

private:
	UPROPERTY(ReplicatedUsing = OnRep_Kills)
	int32 Kills;

	UPROPERTY(ReplicatedUsing = OnRep_Assists)
	int32 Assists;

	UPROPERTY(ReplicatedUsing = OnRep_Deaths)
	int32 Deaths;
#pragma endregion

#pragma region CaptureTheFlag

#pragma endregion

};
