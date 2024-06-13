// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Ascend/Weapon/WeaponStructureBase.h"
#include "BaseCharacter.generated.h"

class ABaseWeapon;
class AMeleeWeapon;
class ARangedWeapon;

class UArsenalComponent;
class UAIPerceptionStimuliSourceComponent;
class UAttributeComponent;
class UCameraComponent;
class UCapsuleComponent;
class UCharacterDefaultAnimInstance;
class UCharacterMovementComponent;
class UCombatSystemComponent;
class USkeletalMeshComponent;
class UViewInterfaceComponent;

class UInputMappingContext;
class UInputAction;

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
	
public:
	FORCEINLINE UArsenalComponent* GetArsenalSystem() const						{ return ArsenalSystem; }
	FORCEINLINE UCameraComponent* GetPlayerCamera() const						{ return PlayerCamera; }
	FORCEINLINE UCharacterDefaultAnimInstance* GetCharacterAnimInstance() const { return CharacterAnimInstance; }
	FORCEINLINE UViewInterfaceComponent* GetViewInterface() const				{ return ViewInterface; }
	FORCEINLINE UCombatSystemComponent* GetCombatSystem() const					{ return CombatSystem; }

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
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* InteractionInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* AttackInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* FMSwitchInputAction;


	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* EnterVehicleInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SwapWeaponInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ReloadInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* AimInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SprintInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SlideInputAction;
	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* DashInputAction;

	UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* OptionInputAction;
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
	/*Subfunction*/
	void SwitchWeaponFiringMode(ABaseWeapon* InputWeapon);

/*Vehicles*/
	void InputFunctionEnterVehicle();
	void InputFunctionSwapWeapon();
	void InputFunctionReload();
	void InputFunctionSlide();
	void InputFunctionDash();

	void InputFunctionOption();
	void InputFunctionScore();


private:
	bool bWantsToCrouch = false;
	bool bWantsToProne	= false;
	bool bWantsToSprint = false;
	bool bWantsToLean	= false;

	bool bAttackButtonPressed = false;
	bool bAimButtonPressed	  = false;

	bool bCharacterCrouching	= false;
	bool bCharacterProne		= false;
	bool bCharacterSprinting		= false;
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
private:
	float DamageAmount;

public:
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ReceiveDamage(float Damage);

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
};
