// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Ascend/Weapon/WeaponStructureBase.h"
#include "BaseCharacter.generated.h"

#define CONTROL_AND_DEBUG_CLASSCOMPONENTS_CHECK 0

#pragma region Pre-Call
#pragma region Pre-Classes
class ABaseWeapon;
class AMeleeWeapon;
class ARangedWeapon;

class APlayerControllerModule;
class APlayerStateModule;

#pragma endregion

#pragma region Pre-Components
class UArsenalComponent;
class UAIPerceptionStimuliSourceComponent;
class UAttributeComponent;
class UCameraComponent;
class UCapsuleComponent;
class UCharacterDefaultAnimInstance;
class UCharacterMovementComponent;
class UCombatSystemComponent;
class UGameSaveData;
class USkeletalMeshComponent;
class ULevelSystemComponent;
class UViewInterfaceComponent;
class UPlayerInventory;

class UInputMappingContext;
class UInputAction;
#pragma endregion
#pragma endregion
UCLASS()
class ASCEND_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

#pragma region ConstructorFunctionalities
	// Sets up the player's camera component, including creation, attachment, positioning, and settings such as field of view and frustum visibility.
	void SetupPlayerCameraComponent();

	// Sets up the character's capsule component, including initialization, attachment, and collision settings.
	void SetupCharacterCapsuleComponent();

	// Sets up the skeletal mesh components for the character, including initialization, attachment, visibility settings, and relative positioning.
	void SetupCharacterMeshComponents();

	// Configures the character's movement parameters such as crouched height, maximum walk speed, swim speed, and related settings.
	void SetupCharacterMovementComponent();

	//	Create the components as character's sub-objects.
	void CreateSubobjectComponents();
#pragma endregion
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

#pragma region Classes
protected:
	UPROPERTY()
	APlayerControllerModule* PlayerControllerModule;

	UPROPERTY()
	APlayerStateModule* PlayerStateModule;

	void ControlClassesAndComponentsCast();
#pragma endregion
#pragma region Components
protected:
	UPROPERTY(VisibleAnywhere)
	UArsenalComponent* ArsenalSystem;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CharacterCapsule;

	UPROPERTY(VisibleAnywhere)
	UCharacterMovementComponent* CharacterMovementComp;

	UPROPERTY()
	UCharacterDefaultAnimInstance* CharacterAnimInstance;

	UPROPERTY(VisibleAnywhere)
	UCombatSystemComponent* CombatSystem;

	UPROPERTY(EditAnywhere)
	UViewInterfaceComponent* ViewInterface;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	ULevelSystemComponent* LevelSystem;
	
public:
	FORCEINLINE UArsenalComponent*				GetArsenalSystem() const				{ return ArsenalSystem; }
	FORCEINLINE UCameraComponent*				GetPlayerCamera() const					{ return PlayerCamera; }
	FORCEINLINE UCharacterDefaultAnimInstance*	GetCharacterAnimInstance() const		{ return CharacterAnimInstance; }
	FORCEINLINE UViewInterfaceComponent*		GetViewInterface() const				{ return ViewInterface; }
	FORCEINLINE UCombatSystemComponent*			GetCombatSystem() const					{ return CombatSystem; }
	FORCEINLINE ULevelSystemComponent*			GetLevelSystem() const					{ return LevelSystem; }

#pragma endregion
#pragma region Save/Load-Game

private:
	UPROPERTY(EditAnywhere, Category = "Developer Settings")
	TSubclassOf<UGameSaveData> SaveGameClass;

#pragma endregion
#pragma region Data-Receiver& Transmitter
private:
	UFUNCTION()
	void DataTransceiverBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function to handle actor end overlap events
	UFUNCTION()
	void DataTransceiverEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion
#pragma region SkeletalMeshes
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* CharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Hair;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Head;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Legs;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Arms;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Hands;

public:
	FORCEINLINE USkeletalMeshComponent* GetHairMesh()	const { return Hair; }
	FORCEINLINE USkeletalMeshComponent* GetHeadMesh()	const { return Head; }
	FORCEINLINE USkeletalMeshComponent* GetLegsMesh()	const { return Legs; }
	FORCEINLINE USkeletalMeshComponent* GetArmsMesh()	const { return Arms; }
	FORCEINLINE USkeletalMeshComponent* GetHandsMesh()	const { return Hands; }

#pragma endregion
#pragma region PlayerInputs
private:
/*Main*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/*Variables*/
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* MoveInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* LookInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* JumpInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* CrouchInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ProneInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* LeanRightInputAction;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* AttackInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* AimInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* WeaponSwitchInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* FireModeSwitchInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* FireTypeSwitchInputAction;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ReloadInputAction;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* InteractionInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* EnterVehicleInputAction;


	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SprintInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SlideInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* DashInputAction;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* OptionInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* InventoryInputAction;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ScoreboardInputAction;


/*TEMPORARILY*/
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* DebugInputAction;
	void DebugInput();

private:
/*Movement*/
	void InputFunctionMovement	(const FInputActionValue& Value);
	void InputFunctionMovementC (const FInputActionValue& Value);
	void ControlCharacterMovementSpeed();

/*LookAround*/
	void InputFunctionLook		(const FInputActionValue& Value);

/*Jump*/
	void InputFunctionJump();

/*Crouch*/
	void InputFunctionCrouch();

/*Prone*/
	void InputFunctionProne();	
	void InputFunctionProneEnd();


/*Sprint*/
	void InputFunctionSprint();
	void InputFunctionSprintEnd();
/*Leaning*/
	void InputFunctionLean(const FInputActionValue& Value);			 
	void InputFunctionLeanCompleted();
	void ResetLean();
	void InputFunctionInteraction();

/*Attack(Melee, Range)*/
	void InputFunctionAttack(); 
	void InputFunctionAttackEnd();

/*Aiming*/
	void InputFunctionAim();
	void InputFunctionAimEnd();

/*WeaponSwitchMode*/
	void InputFunctionWeaponFiringModeSwitch();
	/*Subs*/
	void SwitchWeaponFiringMode(ARangedWeapon* InputWeapon);

	void InputFunctionWeaponFireTypeSwitch();
	void SwitchWeaponFireType(ARangedWeapon* InputWeapon);

/*Reloading*/
	void InputFunctionReload();

/*Vehicles*/
	void InputFunctionEnterVehicle();
	void InputFunctionSwapWeapon();
	void InputFunctionSlide();
	void InputFunctionDash();

	void InputFunctionOption();
	void InputFunctionInventoryMenu(); bool bInventory = false;

	void InputFunctionScore();

private:
	bool bWantsToCrouch			= false;
	bool bWantsToProne			= false;
	bool bWantsToSprint			= false;
	bool bWantsToLean			= false;
	bool bWantsToReload			= false;

	bool bAttackButtonPressed	= false;
	bool bAimButtonPressed		= false;

	bool bCharacterCrouching	= false;
	bool bCharacterProne		= false;
	bool bCharacterSprinting	= false;
	bool bCharacterLeaning		= false;
	bool bCharacterAttacking	= false;
	bool bCharacterAiming		= false;
/*SupFunctions*/
public:
	bool DisableAim();

/*GetterSetters*/
public:
	FORCEINLINE bool GetAttackButtonPressed() const { return bAttackButtonPressed; }
	FORCEINLINE void SetAttackButtonPressed(bool bPressed) { bAttackButtonPressed = bPressed; }

	FORCEINLINE bool GetAimButtonPressed() const { return bAimButtonPressed; }
	FORCEINLINE void SetAimButtonPressed(bool bPressed) { bAimButtonPressed = bPressed; }
#pragma endregion
#pragma region Movement
private:
	float CharacterForwardDirection;
	float CharacterRightDirection;
	float ForwardSpeed;
	float RightSpeed;

protected:
	void UpdateSpeedAndDirections();
	void ResetSpeedAndDirections();

public:
	float GetForwardSpeed() const { return ForwardSpeed; }
	float GetRightSpeed() const { return RightSpeed; }
#pragma endregion
#pragma region Camera&Viewport
private:
	float CameraAimYaw;
	float CameraAimPitch;

	float HorizontalSensitivity = 0.50f;
	float VerticalSensitivity   = 0.50f;

	float RotateBonePitch;
	float RotateBoneYaw;

	float LeanRightRoll;
public:
	FORCEINLINE float GetRotateBonePitch()	const { return RotateBonePitch; }
	FORCEINLINE float GetRotateBoneYaw()	const { return RotateBoneYaw; }
	FORCEINLINE float GetLeanRight()		const { return LeanRightRoll; }

#pragma endregion
#pragma region Damage-System
/*Damage*/
private:
	float DamageAmount;

public:
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ReceiveDamage(float Damage);

#pragma endregion
#pragma region Death-State
private:
	bool bPlayerIsDead = false;
	void Death();
	void SetRagDoll(bool SimulatePhysics, ECollisionEnabled::Type NewType);

protected:
	FTimerHandle RespawnTimerHandler;
	void RespawnPlayer();
public:
	bool PlayerIsDead() const { return bPlayerIsDead; }
#pragma endregion
#pragma region HUD
protected:
	void HUDInitialSetup();
private:
	void InitializeHUDHealth();
	void InitializeHUDShield();
	void InitializeHUDStamina();
#pragma endregion
#pragma region UI/IX/Widgets
private:
	void CreateInventoryWidget();

	bool bIsInInventoryWidget = false;
protected:
	// The class of the user widget to use for the inventory UI.
	// This widget will be instantiated to represent the inventory interface.
	UPROPERTY(EditAnywhere, Category = "Developer settings")
	TSubclassOf<class UUserWidget> InventoryWidget;

	UPROPERTY()
	UPlayerInventory* PlayerInventoryWidget;

public:
	UPlayerInventory* GetPlayerInventoryWidget() const
	{
		return PlayerInventoryWidget;
	}

#pragma endregion
#pragma region Attributes
private:
	void HandleAttributes();

	void HandleHealth();
	void HandleShield();
	void HandleStamina();
#pragma endregion
#pragma region Level-System
public:
	void UpdateHUDExperience();
#pragma endregion
#pragma region Weapon-System
private:
	UPROPERTY()
	ABaseWeapon* OverlapWeapon = nullptr;

	UPROPERTY()
	ARangedWeapon* RangeWeaponClass;

	UPROPERTY()
	AMeleeWeapon* MeleeWeaponClass;

public:
	void SetOverlapWeapon(ABaseWeapon* Weapon) { OverlapWeapon = Weapon; }
#pragma endregion
#pragma region AI-System
private:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();
#pragma endregion
#pragma region Reloading
public:
	void PlayReloadMontage();

	void SetReloading(bool bReload);
#pragma endregion
#pragma region SoundFX-Lib
private:

#pragma endregion
#pragma region VisualFX-Lib
private:

#pragma endregion
#pragma region Animation-Lib
	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Montages")
	UAnimMontage* ReloadMontage;
#pragma endregion
};
