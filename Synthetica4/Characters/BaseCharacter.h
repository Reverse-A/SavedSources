// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "InputActionValue.h"
#include "Synthetica/EnumTypes/CharacterTypes.h"
#include "TimerManager.h"
#include "BaseCharacter.generated.h"


UCLASS()
class SYNTHETICA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	/* Camera Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat)
	class UCombatComponent* Combat;

	class UCapsuleComponent* BaseCapsuleCompenent;
	class USkeletalMeshComponent* BaseMeshCompenent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* BaseCharacterMovementCompenent;
	class APlayerController* BaseInputController;


private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* MappingContext;

	/* Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* SwitchWeaponAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* InteractionAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* SwitchCameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	class UInputAction* ThrowGrenadeAction;

	float OutSlopePitchDegreeAngle;
	float OutSlopeRollDegreeAngle;

	bool bSprintButtonPressed;
	bool bIsWeaponEquipped;
	bool bJumpIsDone = true;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	void PlayFireMontage(bool bAiming);

	float InterpolateSpeed(float CurrentSpeed, float TargetSpeed, float DeltaTime, float InterpolationSpeed);

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	EWeaponEquippedStatus CurrentCharacterState;
	ERotateStatement RotateStatementEnum;

	/** Called for movement and looking input */
	void Move(const FInputActionValue& Value);
	void MoveNegative(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpFunction(const FInputActionValue& Value);

	void CharacterGeneralCondition(float DeltaTime);
	void CharacterMovementState(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SecondJumpTrigger();
	void AimButtonPressed(const FInputActionValue& Value);
	void AimButtonReleased(const FInputActionValue& Value);
	void InteractionButtonPressed(const FInputActionValue& Value);
	void InteractionButtonReleased(const FInputActionValue& Value);
	void ToggleWeapon();
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void StartSprinting();
	void StopSprinting();
	void FireButtonPressed();
	void FireButtonReleased();
	void ReloadButton();
	void ThrowGrenadeButton();

	UFUNCTION(BlueprintCallable)
	void ReloadFinished();

	//UFUNCTION(BlueprintCallable)
	//void ShotGunReloadFinished();

	//Camera
	FTimerHandle CameraSwitchTimerHandle;
	void SwitchCameraMethod();
	void SwitchCamera();

	// General Actions outside of the character but belongs to
	void RespawnPlayer(FVector SpawnLocation, FRotator SpawnRotation);
	void RagDoll();
	void IfActorRagDoll(float DeltaTime);

	FVector LastRagdollPosition;
	float RagDollTimerDelay;

	bool bcanStartJog = true;
	bool bIsMoving;
	bool bIsSprinting;
	bool bIsCrouching;
	bool bIsInAir;
	bool bIsJumping = false;
	bool bCanJump = true;
	bool bCanMove = true;
	bool bCanMoveAfterJump = true;
	bool bNormalJumping;
	bool bSprintJump;

	bool bSlopeHitInRange;
	bool InSlopeMovement;
	bool bVeryHighSlope;


	float WaitForMovementAfterJumpDone_Delay;
	float CrouchWalkSpeed = 75.f;

	UPROPERTY(EditAnywhere, Category = "PlayerStats")
	float MaxHealth = 200.f;
	float Health	= 200.f;

	class ABasePlayerController* BasePlayerController;

	void WallAndObstacleDetactor(float DeltaTime);
	void WallSlide();
	bool bIsActorHitFrontWall = false;
	bool bIsHoldingWall = false;
	bool bJumpingFromWallSlide = false;
	bool bCanRagDoll = false;
	bool bRagDolled = false;

	bool bIsNotTouchTheGroundAfterJump;
	float JumpingAfterTimerDelay;
	float canFallAfterJumpingWallDelay;


private:
	class AWeapon* OverlappingWeapon;
	AWeapon* DisarmedWeapon;
	class ABaseHUD* HUD;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	// Mathematical Functions and Logics
	void CalculatingPitchSlope(float DeltaTime);
	void CalculateFootSocketsFromGround(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void DistanceFromGround();
	float CharacterAndGroundBetweenDistance;

	float CanFallTimerDelay;
	bool bCanFall = false;


	float SlopeDegreeResetTimerDelay;
	float LeftDistanceToGround;
	float RightDistanceToGround;
	float DifferenceBetweenTwoFeet;

	float CurrentMovementSpeed;
	float CurrentWalkSpeed;
	float InitialWalkSpeed;
	float JogWalkSpeed;
	float SprintWalkSpeed;

	bool bCanSprint_Disarmed;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwitchWeaponMontage;
	FTimerHandle WeaponToggleTimerHandle;


	void ResetWeaponToggleTimer();
	void AimOffset(float DeltaTime);
	float InterpAO_Yaw;
	float AO_Yaw = 0.f;
	float AO_Pitch = 0.f;
	FRotator StartingAimRotation;

	void RotateCharacterInPlace(float DeltaTime);
	FRotator TargetRotation;
	FVector RotateForwardLocation;
	FVector RotateLeftLocation;

	bool bRotationInProgress;
	bool bIsReload;
	bool bIsFireButtonTriggered;
	bool bThrowningGrenade = false;

	float MovementX;
	float MovementY;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* AttachedHandGrenade;
public:
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UCombatComponent* GetCombat()	    const { return Combat; }
	FORCEINLINE USkeletalMeshComponent* GetAttachedHandGrenade() const { return AttachedHandGrenade; }
	FORCEINLINE EWeaponEquippedStatus GetCurrentCharacterState() const;
	FORCEINLINE ERotateStatement GetRotateCharacterInPlace() const { return RotateStatementEnum; }

	AWeapon* GetEquippedWeapon();

	float GetSlopePitchDegreeAngle() const;
	float GetSlopRollDegreeAngle() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	FORCEINLINE float GetLeftDistanceToGround() const { return LeftDistanceToGround; }
	FORCEINLINE float GetRightDistanceToGround() const { return RightDistanceToGround; }
	FORCEINLINE float GetDifferenceBetweenTwoFeet() const { return DifferenceBetweenTwoFeet; }
	FORCEINLINE float GetCurrentWalkSpeed() const { return CurrentWalkSpeed; }
	FORCEINLINE float GetCurrentMovementSpeed() const { return CurrentMovementSpeed; }


	FORCEINLINE bool GetIsMoving() const { return bIsMoving; }
	FORCEINLINE bool GetIsHoldingWall() const { return bIsActorHitFrontWall; }
	FORCEINLINE bool GetCrouch() const { return bIsCrouching; }
	FORCEINLINE bool GetIsJumping() const { return bIsJumping; }
	FORCEINLINE bool GetIsNormalJumping() const { return bNormalJumping; }
	FORCEINLINE bool GetSprinting() const { return bIsSprinting; }
	FORCEINLINE bool GetSprintJump() const { return bSprintJump; }
	FORCEINLINE bool GetIsInAir() const { return bIsInAir; }
	FORCEINLINE bool GetCanFall() const { return bCanFall; }
	FORCEINLINE bool GetIsReloading() const { return bIsReload; }
	FORCEINLINE bool SetIsReloading(bool IsReloading) { return bIsReload = IsReloading; }

	FORCEINLINE bool GetThrowGrenade() const { return bThrowningGrenade; }
	FORCEINLINE bool SetThrowningGrenade(bool bIsThrowGrenade) { return bThrowningGrenade = bIsThrowGrenade; }

	void SetCurrentCharacterState(EWeaponEquippedStatus NewCharacterState);
	void SetOverLappingWeapon(class AWeapon* Weapon);

	bool IsWeaponEquipped();
	bool IsAiming();

	FVector GetHitTarget() const;

	FORCEINLINE float GetMovementX() const { return MovementX; }
	FORCEINLINE float GetMovementY() const { return MovementY; }

private:
	FVector RotateForLeftVector;
	FVector RotateForRightVector;
	FVector RotateForForwardVector;

	float StartDelay;

};
