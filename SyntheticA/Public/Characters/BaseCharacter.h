// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "Interfaces/InteractWithCrosshairsInterface.h"

#include "NewTypes/TeamTypes.h"
#include "NewTypes/TroopTypes.h"

#include "BaseCharacter.generated.h"

#pragma region Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);
#pragma endregion
#pragma region Macros
#define ZERO 0.0f;

#define LOG_WARNING(Format)						UE_LOG(LogTemp, Warning, TEXT(Format));
#define LOG_WARNING_FLOAT(FloatValue)			UE_LOG(LogTemp, Warning, TEXT("%f"), FloatValue);
#define LOG_WARNING_TEXTANDFLOAT(Format, ...)	UE_LOG(LogTemp, Warning, TEXT(Format), __VA_ARGS__)
#define LOG_SCREEN(Message, ...) \
    if (GEngine) \
    { \
        FString Msg = FString::Printf(TEXT(Message), ##__VA_ARGS__); \
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, Msg); \
    }

#define LINETRACE(World, StartPoint, EndPoint, Color) DrawDebugLine(World, StartPoint, EndPoint, Color);
#pragma endregion
#pragma region Class&Components Pre-Store
class AAerodyne;
class ABaseGameMode;
class AFlag;
class AGroundVehiclePawn;
class APlayerControllerManager;
class APlayerStateManager;
class AWeapon;

class UAnimMontage;
class UAttributeComponent;
class UBoxComponent;
class UCameraComponent;
class UCameraManager;
class UCombatComponent;
class UEnhancedMovementComponent;
class USpringArmComponent;
class ULagCompensationComponent;
class UGameOption;

class UCoreAnimProcessor;
class UInputMappingContext;
class UInputAction;
class UScoreboard;
class USoundCue;
#pragma endregion
#pragma region BaseCharacter
UCLASS()
class SYNTHETICA_API ABaseCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

#pragma region General & OtherClasses
public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Destroyed() override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	UPROPERTY()	
	APlayerControllerManager*	PlayerControllerManager;

	UPROPERTY()	
	APlayerStateManager*		PlayerStateManager;

	UPROPERTY()
	ABaseGameMode*				BaseGameMode;
public:
	FORCEINLINE APlayerControllerManager* Get_PlayerControllerManager() const { return PlayerControllerManager; }

#pragma endregion
#pragma region Components
protected:
	UPROPERTY(VisibleAnywhere,  Category = "Components")
	UEnhancedMovementComponent* EnhancedMovementComponent;

	UPROPERTY(VisibleAnywhere,  Category = "Components")
	UAttributeComponent*	    AttributeComponent;

	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly, Category = "Components")
	UCombatComponent*		    CombatComponent;

	UPROPERTY(VisibleAnywhere,  Category = "Components")
	UCameraManager*			    CameraManager;

	UPROPERTY(VisibleAnywhere,  Category = "Components")
	ULagCompensationComponent*  LagCompensantionComponent;

	UPROPERTY(VisibleAnywhere,  Category = "Components")
	UGameOption*				InGameOption;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UScoreboard*				ScoreboardWidget;

public:
	FORCEINLINE UEnhancedMovementComponent* GetEnhancedMovementComponent()  const { return EnhancedMovementComponent; }
	FORCEINLINE UAttributeComponent*		GetAttributeComponent()			const { return AttributeComponent; }
	FORCEINLINE UCameraManager*				GetCameraManager()				const { return CameraManager; }
	FORCEINLINE UCombatComponent*			Get_CombatComponent()			const { return CombatComponent; }
	FORCEINLINE ULagCompensationComponent*	GetLagCompensationComponent()	const { return LagCompensantionComponent; }

#pragma endregion
#pragma region Camera
private:
	void EliminationCamera(float DeltaTime);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	USpringArmComponent* ThirdPersonSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent* DeathCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent* ThirdPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent* FirstPersonCamera;
public:			/* Getter And Setters*/
	FORCEINLINE USpringArmComponent* GetThirdPersonSpringArm() { return ThirdPersonSpringArm; }
	FORCEINLINE UCameraComponent* GetThirdPersonCamera() const { return ThirdPersonCamera; }
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }
#pragma endregion
#pragma region HitCollision
public:
	TMap<FName, UBoxComponent*> HitCollisionBoxes;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* Head_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Pelvis_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Spine_02_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Spine_03_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* UpperArm_R_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* UpperArm_L_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* LowerArm_R_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* LowerArm_L_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Hand_R_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Hand_L_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Thigh_R_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Thigh_L_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Calf_R_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Calf_L_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Foot_R_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Foot_L_Box;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BackPack_Box;

public:
	//void ActivePlayerCollisions(ECollisionChannel CollisionChannel);
#pragma endregion
#pragma region SkeletalMeshes
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Hair;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Head;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Legs;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Arms;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Hands;

	FORCEINLINE USkeletalMeshComponent* GetHairMesh()	const { return Hair;  }
	FORCEINLINE USkeletalMeshComponent* GetHeadMesh()	const { return Head;  }
	FORCEINLINE USkeletalMeshComponent* GetLegsMesh()	const { return Legs;  }
	FORCEINLINE USkeletalMeshComponent* GetArmsMesh()	const { return Arms;  }
	FORCEINLINE USkeletalMeshComponent* GetHandsMesh()	const { return Hands; }

#pragma endregion
#pragma region Inputs
protected:
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputMappingContext* BaseCharacterMappingContext;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* CameraSwitchAction;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* MoveInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* LookInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* InteractionInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* EnterVehicleInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SwapWeaponInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ReloadInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* AimInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* FireInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* JumpInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* CrouchInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ProneInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SprintInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SlideInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* DashInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* AutoRunInputAction;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* InGameOptionInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ScoreboardInputAction;


public:
	FORCEINLINE UInputMappingContext* GetBaseCharacterMappingContext() const { return BaseCharacterMappingContext; }
#pragma endregion
#pragma region InputsFunctions
private:
void EnhancedInputLocalPlayerSubsystem();

void Input_SwitchCamera				(const FInputActionValue& Value);
void Input_Movement					(const FInputActionValue& Value);
void Input_Look						(const FInputActionValue& Value);

void Input_Sprint					(const FInputActionValue& Value);

void Input_Crouch_Activation		(const FInputActionValue& Value);
void Input_Crouch_DeActivation		(const FInputActionValue& Value);

void Input_Jump						(const FInputActionValue& Value);

void Input_Fire_Activation			(const FInputActionValue& Value);
void Input_Fire_DeActivation		(const FInputActionValue& Value);

void Input_Interaction				(const FInputActionValue& Value);
void Input_EnterVehicleAction		(const FInputActionValue& Value);

void Input_Reload					(const FInputActionValue& Value);
void Input_SwapWeapon				(const FInputActionValue& Value);

void Input_AimButton_Activation		(const FInputActionValue& Value);
void Input_AimButton_DeActivation	(const FInputActionValue& Value);

void InputAction_InGameOptionMenu();
void InputAction_Scoreboard();

#pragma endregion
#pragma region Widgets
/*InGameOption*/
private:
	void CreateInGameOptionMenu();
	void CheckInGameOptionMenu();
	void CloseGameOptionMenuOnHit();

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UUserWidget> InGameOptionMenuWidgetClass;

protected:
	bool bInGameOptionMenuOpened = false;
	bool bCacheInGameOptionMenuOpened = false;

public:
	FORCEINLINE void SetInGameOptionMenuOpened(bool Value) { bInGameOptionMenuOpened = Value; }
	FORCEINLINE bool GetInGameOptionMenuOpened() const { return bInGameOptionMenuOpened; }


/*Scoreboard*/
private:
	void CreateScoreboardWidget();
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UUserWidget> ScoreBoardWidgetClass;

protected:
	bool bScoreboardOpened = false;

public:
	FORCEINLINE void SetScoreBoardOpened(bool bOpened) { bScoreboardOpened = bOpened; }
	FORCEINLINE bool GetScoreBoardOpened() const { return bScoreboardOpened; }
#pragma endregion
#pragma region Camera Perspective Props

private:
	UPROPERTY(Replicated) 
	float MovementDirectionX;

	UPROPERTY(Replicated) 
	float MovementDirectionY;
		
	UFUNCTION(Server, Reliable)
	void Server_SendMovemenntDirections(float DirectionX, float DirectionY);
public:
	FORCEINLINE float GetMovementDirectionX() const { return MovementDirectionX; }
	FORCEINLINE float GetMovementDirectionY() const { return MovementDirectionY; }
#pragma endregion
#pragma region Weapon
/*Weapon properties*/
private:
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void Server_EquipButtonPressed();


/*Drop & Destroy*/
	void DropOrDestroy_Weapon(AWeapon* Weapon);
public:
	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();

public:
	AWeapon* GetEquippedWeapon();

/* Aim */
public:
	bool IsAiming();

/*Spawn Default Weapon*/
private:
	// Flag to determine whether the default weapon should be spawned initially
	UPROPERTY(EditAnywhere)
	bool bStartWithWeapon = true;
protected:
	// The class of the weapon that will be spawned at the beginning, determined by the blueprint selection
	// If true, the default weapon will be spawned accordingly
	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	TSubclassOf<AWeapon> DefaultWeaponClass;

public:
	// Function to spawn the default weapon for the character 
	void SpawnDefaultWeapon();

/*CancelShotgunReload*/
private:
	void CheckAndCancelShotgunReloadingIfAttempted();
private:
	bool bAttemptToCancelShotgunReloading = false;

public:
	bool CanCancelShotgunReloading() const { return bAttemptToCancelShotgunReloading; }
	void SetCancelShotgunReloading(bool Value) { bAttemptToCancelShotgunReloading = Value; }

#pragma endregion
#pragma region Vehicles & Troops

/*Player's Situations*/
private:
	ETroopTypes TroopTypes = ETroopTypes::ETT_Infantry;

	bool bPlayerInVehicle = false;

public:
	FORCEINLINE bool GetPlayerInVehicle() const { return bPlayerInVehicle; }
	FORCEINLINE void SetPlayerInVehicle(bool bInVehicle) { bPlayerInVehicle = bInVehicle; }

	void SetTroopTypes(ETroopTypes NewTroopTypes) { TroopTypes = NewTroopTypes; }
	FORCEINLINE ETroopTypes GetTroopTypes()  const { return TroopTypes; }
/*Aerodynes*/
private:
	UPROPERTY(Replicated)
	AAerodyne* AerodyneClass;

private:
	void EnterVehicle_Aerodyne(AAerodyne* VehicleClass);
public:
	void Set_AerodynClass(AAerodyne* VehicleClass) { AerodyneClass = VehicleClass; }

	/*AeroRep.*/
	UFUNCTION(Server, Reliable)
	void Server_EnterVehicle_Aerodyne(AAerodyne* VehicleClass);

/*Ground Vehicles*/
private:
	UPROPERTY(Replicated)
	AGroundVehiclePawn* GroundVehiclePawn;

private:
	void EnterVehicle_GroundVehicle(AGroundVehiclePawn * GroundVehicle);
public:
	void SetGroundVehicleClass(AGroundVehiclePawn* GroundVehicle) { GroundVehiclePawn = GroundVehicle; }


	// 
	////UFUNCTION(NetMulticast, Reliable)
	////void Multicast_VehicleAction_Aerodyne(AAerodyne* VehicleClass, APlayerController* PC);

	//UFUNCTION(NetMulticast, Reliable)
	//void Server_VehicleAction();
#pragma endregion
#pragma region CollisionProps
public:
FCollisionQueryParams GetIgnoreCharacterParams() const;

void SetCharacterMeshCollisionEnabled(ECollisionEnabled::Type NewType);
#pragma endregion
#pragma region Player-Attributes/HUD
protected:
	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	float MaxHealth = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "DeveloperProperties", BlueprintReadOnly)
	float Health	= 100.0f;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	float MaxShield = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, VisibleAnywhere, Category = "DeveloperProperties")
	float Shield	= 100.0f;

protected:
	UFUNCTION()
	void OnRep_Health(float LastHealth);

	UFUNCTION()
	void OnRep_Shield(float LastShield);

public:
	void UpdateHUDHealth(bool bDead);
	void UpdateHUDShield(bool bShieldBreak);
	void UpdateHUDAmmunition();

	void DeathCondition(float DeltaTime);
public:
/*Getters*/
	FORCEINLINE bool CharacterIsDead()			const { return Health <= 0.0f; }
	FORCEINLINE bool CharacterShieldIsBreak()	const { return Shield <= 0.0f; }

	FORCEINLINE float GetHealth()				const { return Health; }
	FORCEINLINE float GetMaxHealth()			const { return MaxHealth; }

	FORCEINLINE float GetShield()				const { return Shield; }
	FORCEINLINE float GetMaxShield()			const { return MaxShield; }

	FORCEINLINE void  SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE void  SetShield(float Amount) { Shield = Amount; }

#pragma endregion
#pragma region Damage 
public:
	FVector GetHitTarget() const;


protected:
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

/*Shield Props*/
private:
	bool bShieldBreakPlayable;
#pragma endregion
#pragma region Elimation & Disolve
public:
	UFUNCTION(BlueprintCallable)
	void Elimination(bool bPlayerLeftGame);
private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Elimination(bool bPlayerLeftGame);

	void SetRagdollValues(bool SimulatePhysics, ECollisionEnabled::Type NewType);

//virtual void Destroyed() override;
private:
	bool bDeathProgressOver		= false;
	bool bPlayerEliminated = false;
	
	UPROPERTY(EditDefaultsOnly, Category = EliminationProperties)
	float EliminationTimerDelay = 3.0f;

	FTimerHandle EliminationTimer;

	void EliminationTimerFinished();

public:
	void SetPlayerIsEliminated(bool bEliminated) { bPlayerEliminated = bEliminated; }
	bool GetPlayerIsEliminated() const { return bPlayerEliminated; }
	
/* Dissolve Character Props */
private:
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;

	// Dynamic Mat Instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category = EliminationProperties)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// Mat Instance set on the blueprint, used with the dynamic material inst. 
	UPROPERTY(VisibleAnywhere, Category = EliminationProperties)
	UMaterialInstance* DissolveMaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = EliminationProperties)
	UMaterialInstance* DissolveHeadMaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = EliminationProperties)
	UMaterialInstance* DissolveHairMaterialInstance;

	/*TeamColors*/

	/*Default*/
	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* DefaultHairMaterial;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* DefaultHeadMaterial;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* DefaultBodyMaterial;

	/*Default VanguardSovereigntyTeam*/
	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* BlackHairMaterial;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* BlackHeadMaterial;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* BlackBodyMaterial;

	/*Default GenesisPactTeam*/
	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* WhiteHairMaterial;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* WhiteHeadMaterial;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* WhiteBodyMaterial;

	/*White*/
	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* WhiteDissolveHairMaterialInstance;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* WhiteDissolveHeadMaterialInstance;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* WhiteDissolveBodyMaterialInstance;

	/*Black*/
	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* BlackDissolveHairMaterialInstance;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* BlackDissolveHeadMaterialInstance;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* BlackDissolveBodyMaterialInstance;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UCurveFloat* DissolveCurve;
#pragma endregion
#pragma region Teams
public:
	void SetTeamColor(ETeam Team);

	ETeam GetTeam();

private:
	void CheckOnPlayerStateInitialized();
	void SetSpawnPoint();
#pragma endregion
#pragma region CaptureTheFlag
private:
	UPROPERTY(Replicated)
	AFlag* OverlappingFlag;

	void DropOrDestroy_Flag(AFlag* DestroyOrDropFlag);


public:
	void Set_HoldingFlag(bool bHolding);
	void Set_OverlappingFlag(AFlag* Flag);
	FORCEINLINE bool IsHoldingTheFlag() const;
#pragma endregion
#pragma region BlueprintImplementables
/* Implementable events and other properties for base characters */
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Show Sniper Scope Widget", Keywords = "Sniper Scope Widget Show"))
	void ShowSniperScopeWidget(bool bShowScope);
#pragma endregion
#pragma region Gameplay
private:
/*DisableGameplay*/
	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

/*LeftingGame*/
	bool bLeftGame = false;

public:
	FOnLeftGame OnLeftGame;

public:
	UFUNCTION(Server, Reliable)
	void Server_LeaveGame();

public:
	FORCEINLINE void SetDisableGameplay(bool bDisable) { bDisableGameplay = bDisable; }
	FORCEINLINE bool GameplayIsDisabled() const { return bDisableGameplay; }

#pragma endregion
#pragma region GameMode-Props
private:
	FString GameModeClassName;

	FString GetGameModeClassName();
#pragma endregion
};
#pragma endregion