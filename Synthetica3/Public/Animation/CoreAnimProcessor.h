// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Synthetica/EnumTypes/RotationTypes.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraSettings.h"

#include "CoreAnimProcessor.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API UCoreAnimProcessor : public UAnimInstance
{
	GENERATED_BODY()
	
//UBaseAnimInstance(const FObjectInitializer& ObjectInitializer); De-Activated
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[General Functions]*---------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	UCoreAnimProcessor();
	// Function called when the animation instance is initialized
	virtual void NativeInitializeAnimation() override;

	// Function called to update the animation instance each frame
	virtual void NativeUpdateAnimation(float DeltaTime) override;


/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*----------------------------------------------------*[Networking]*------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Character)
	class ABaseCharacter*				BaseCharacter;
	class UEnhancedMovementComponent*	EnhancedMovement;
	class UCombatComponent*				CombatComponent;
	class UCameraSettings*				CameraSettings;

	class AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"))
	ECharacterRotation CharacterRotationType;

private:
	bool CharacterValidation();
	void UpdateAndStoreData();



protected:
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"), Replicated) ECameraViewPerspective CameraViewPerspective;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"), Replicated) float AimOffsetPitch;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"), Replicated) float AimOffsetYaw;

	float Interp_AimoffsetYaw;
	FRotator StartingAimRotation;


	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float YawDelta;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float CurrentVelocityMagnitude;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float SlopePitchDegreeAngle;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bMovementInputActive;

	// Movement
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bCanJump;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bIsJogging;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanSprint;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanCrouch;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanProne;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanSlide;

	// Aim
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanAim;

	// Mantle
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanMantle;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanTallMantle;

	// Reloading
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanReload;

	// Wall-Run Statement
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsWallRunning;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsWallRunningRight;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bIsFalling;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bIsMovingOnGround;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bRotateRootBone;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bUseFABRIK;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bUseAimOffsets;


	// First person perspective Directions
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"), Replicated) float FirstPersonHorizontalDirectionValue;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"), Replicated) float FirstPersonVerticalDirectionValue;

	// Weapon properties
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool bWeaponEquipped;

public:
	bool IsMoving();
	bool IsVelocityZero();

	bool IsCharacterRotation() const { return bIsRotating; }


private:
	void AimOffset();
	void TemporarilyRotation();
	float AutoRunTriggerDelay;
	bool RotateCharacter;

	void  OffSetYawForLean(float DeltaTime, float Interpolate, float ClampNegative, float ClampPositive);

	void AimOffsetUseage();

	float YawDeltaRefreshTimer;	
	FRotator RotationLastFrame;	
	FRotator CharacterRotation;

	void LeftHandSocketAdjustment();

	void TurnInPlace();
	float CharacterRotationTimer;

	bool bIsRotating;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"), Category = Movement, Replicated)
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"), Category = Movement, Replicated)
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"), Category = Movement)
	bool bLocallyControlled;


public:
	//FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; } not used for now
	FORCEINLINE bool IsInAir() const { return bIsInAir; }
};
