// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerManager.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API APlayerControllerManager : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth (float Health, float MaxHealth);
	void SetHUDScore  (float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);

	virtual void OnPossess(APawn* InPawn) override;
	virtual void ReceivedPlayer() override; // Sync with server clock asap

	virtual float GetServerTime(); // Synced with server world clock.

	void HandleMatchStarted();
	void OnMatchStateSet(FName State);
	void HandleCooldown();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	void SetHUDTime();
	void PollInit();

	/* Sync time between client and server */

	// Requests the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void Client_ReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	void CheckTimeSync(float DeltaTime);

	float ClientServerDelta = 0.0f; // Difference between client and server time

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.0f;

	UFUNCTION(Server, Reliable)
	void Server_CheckMatchState();

	UFUNCTION(Client, Reliable)
	void Client_JoinMiddleGame(FName StateOfMatch, float InputWarmup, float InputMatchTime, float Cooldown, float InputStartingTime);

private:
	UPROPERTY()
	class APlayerHUD*		PlayerHUD;
	UPROPERTY()
	class UPlayerHUDPanel*	PlayerHUDPanel;
	UPROPERTY()
	class ACoreGameMode*	CoreGameMode;

	bool bInitializePlayerHUDPanel = false;

	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDefeats;

	float LevelStartingTime = 0.0f;
	float MatchTime			= 0.0f;
	float WarmupTime		= 0.0f;
	float CooldownTime		= 0.0f;
	uint32 CountdownInt		= 0.0f;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();



};
