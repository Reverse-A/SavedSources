// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Weapon/WeaponTypes.h"
#include "PlayerControllerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

class ABaseCharacter;
class ABaseGameMode;
class AFlag;
class APlayerHUD;
class APawn;
class APlayerState;
class APlayerStateManager;
class AGameStateManager;

class UDisplayManager;

#pragma region APlayerControllerManager
UCLASS()
class SYNTHETICA_API APlayerControllerManager : public APlayerController
{
	GENERATED_BODY()

#pragma region OtherClass
protected:
	APlayerControllerManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "SyntheticA|PlayerController")
	APlayerStateManager* GetPlayerstateManager() const;

	/*TODO: AbilitySystem*/

	UFUNCTION(BlueprintCallable, Category = "SyntheticA|PlayerController")
	APlayerHUD* GetPlayerHUD() const;

	bool TryToRecordClientReplay();

	virtual bool ShouldRecordClientReplay();

	UFUNCTION(Reliable, Server, WithValidation)
	void Server_Cheat(const FString& Message);

	UFUNCTION(Reliable, Server, WithValidation)
	void Server_CheatAll(const FString& Message);


private:
	UPROPERTY()
	APlayerHUD* PlayerHUD;

	UPROPERTY()
	UDisplayManager* DisplayManager;

	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	ABaseGameMode* BaseGameMode;
#pragma endregion
#pragma region Main/Functions
public:
	virtual void InitializePool();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* InPawn) override;



#pragma endregion
#pragma region HUD/Functions
public:
	void ResetHUDElementsToDefault();
	void SetHUDIfPlayerInVehicle(bool bHide);

	void SetHUDHealth				(float Health, float MaxHealth			);
	void SetHUDShield				(float Shield, float MaxShield			);
	void SetHUDShieldOff();
		
	void SetHUDScore				(float Score							);
	void SetHUDKills				(int32 Kills							);
	void SetHUDAssists				(int32 Assists							);
	void SetHUDDeaths				(int32 Deaths							);

	void SetHUDWeaponAmmo			(int32 Ammo								);
	void SetHUDCarriedAmmo			(int32 CarriedAmmo						);
	void SetHUDWeaponType			(EWeaponType WeaponType, bool bVisible	);
	void SetHUDFlag					(bool bShow);

	void SetHUDTime();
	void SetHUDMatchCountdown		(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);

	void SetHUDEliminatedWindow();
	void SetHUDInGameMenuOpened();
	void SetHUDInGameMenuClosed();

private:
	void AddAnnouncementWidgetToScreen();

	bool bHUDHideInVehicle = false;
#pragma endregion
#pragma region Elimination
public:
	void Broadcast_Elimination(APlayerState* Attacker, APlayerState* Victim);

	UFUNCTION(Client, Reliable)
	void Client_EliminationAnnouncement(APlayerState* Attacker, APlayerState* Victim);
#pragma endregion
#pragma region Match/Statement
private:
	float LevelStartingTime = 0.0f;
	float MatchTime			= 0.0f;
	float CooldownTime		= 0.0f;
	float WarmupTime		= 0.0f;

	uint32 CountdownInt		= 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UFUNCTION(Server, Reliable)
	void Server_CheckMatchState();

	UFUNCTION(Client, Reliable)
	void Client_JoinMidGame(FName StateOfMatch, float WarmupTimeInput, float MatchTimeInput,float CooldownInput, float StartingTimeInput);
public:
	void OnMatchStateSet(FName State, bool bTeamMatch = false);

	void Handle_MatchHasStarted(bool bTeamMatch = false);
	void Handle_MatchStateCooldown();

/*TeamMatchs*/
	void Hide_TeamScores();
	void Initialize_TeamScores();
	void Set_HUDVanguardSovereigntyScore(int32 VanguardTeamScore);
	void Set_HUDGenesisPactScore(int32 GenesisTeamScore);

private:
	UPROPERTY(ReplicatedUsing = OnRep_ShowTeamScores)
	bool bShowTeamScores = false;

	UFUNCTION()
	void OnRep_ShowTeamScores();

#pragma endregion
#pragma region Re-InitializeProps
private:
	bool bInitializeHealth = false;
	float HUDHealth;
	float HUDMaxHealth;

	bool bInitializeScore = false;
	float HUDScore;

	bool bInitializeDeaths = false;
	int32 HUDDeaths;

	bool bInitializeGrenades = false;
	int32 HUDGrenades;

	bool bInitializeShield = false;
	float HUDShield;
	float HUDMaxShield;

	bool bInitializeWeaponAmmunition = false;
	int32 HUDWeaponAmmunition;

	bool bInitializeCarriedAmmunition = false;
	int32 HUDCarriedAmmunition;
#pragma endregion
#pragma region Ping&Time
public:
	FHighPingDelegate HighPingDelegate;

/*Ping*/
private:
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;

	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 5.0f;

	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.0f;

	float HighPingRunningTime = 0.0f;
	float PingAnimationRunningTime = 0.0f;
protected:
	void CheckPing(float DeltaTime);
	void HighPingWarning();
	void StopHighPingWarning();
public:
	UFUNCTION(Server, Reliable)
	void Server_ReportPingStatus(bool bHighPing);


/*Time synchronization
  Sync time between cliend and server*/
private:
	float SingleTripTime = 0.0f;
public:
	FORCEINLINE float GetSingleTripTime() const { return SingleTripTime; }
private:
	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.0f;

	float TimeSyncRunningTime = 0.0f;

	// Difference between client and server time
	float ClientServerDelta = 0.0f;

protected:
	void CheckTimeSync(float DeltaTime);

	// Requests the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to Server_RequestServerTime
	UFUNCTION(Client, Reliable)
	void Client_ReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

public:
	// Synched with server world clock 
	virtual float GetServerTime();

	// Sync with server clock asap
	virtual void  ReceivedPlayer() override;
#pragma endregion
#pragma region PlayerInfo
private:
	FString GetInfoText(const TArray<APlayerStateManager*>& Players);
	FString GetTeamsInfoText(AGameStateManager* GameStateManager);
#pragma endregion
#pragma region InGameMenuOption
private:
	bool bCacheHealthBar = false;
	bool bCacheShieldBar = false;
	bool bCacheHealthText = false;
	bool bCacheHeaderHealth = false;
	bool bCacheShieldText = false;
	bool bCacheHeaderShield = false;
	bool bCacheScoreAmount = false;
	bool bCacheScoreText = false;
	bool bCacheDeathsAmount = false;
	bool bCacheDeathsText = false;
	bool bCacheWeaponTypeText = false;
	bool bCacheWeaponAmmoAmount = false;
	bool bCacheWeaponCrosshairAmmoAmount = false;
	bool bCacheAmmoMiddle = false;
	bool bCacheCarriedAmmoAmount = false;
	bool bCacheMatchCountDownText = false;
	bool bCacheEquipWeaponText = false;
	bool bCacheDeathNote = false;
	bool bCacheBlackTeamScore = false;
	bool bCacheWhiteTeamScore = false;
	bool bCacheScoreSpacerText = false;
	bool bCacheHealhtShieldBackgroundFrame = false;
	bool bCacheWeaponBackgroundFrame = false;	 
#pragma endregion
};
#pragma endregion

#pragma region ReplayPlayerController
/*
* A player controlelr used for replay capture and playback  "From LyraGame"
*/

UCLASS()
class AReplayPlayerController : public APlayerControllerManager
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;
	virtual void SmoothTargetViewRotation(APawn* TargetPawn, float DeltaSeconds) override;
	virtual bool ShouldRecordClientReplay();

	// Callback for when the game state's RecorderPlayerState gets replicated during replay playback
	void RecorderPlayerStateUpdated(APlayerState* NewRecorderPlayerState);

	//Callback for when the followed player state changes pawn
	UFUNCTION()
	void OnPlayerStatePawnSet(APlayerState* ChangedPlayerState, APawn* NewPlayerPawn, APawn* OldPlayerPawn);

	// The player state we are currently following
	UPROPERTY(Transient)
	TObjectPtr<APlayerState> FollowedPlayerState;
};

#pragma endregion