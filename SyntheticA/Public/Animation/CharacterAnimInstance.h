// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraViewPerspectiveTypes.h"

#include "Enumerations/ActorRotationTypes.h"
#include "Weapon/WeaponTypes.h"
#include "CharacterAnimInstance.generated.h"

class ABaseCharacter;
class AWeapon;

class UEnhancedMovementComponent;

UCLASS()
class SYNTHETICA_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
//UBaseAnimInstance(const FObjectInitializer& ObjectInitializer); De-Activated
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[General Functions]*---------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region Constructor && Intializer
public:
	UCharacterAnimInstance();

	// Function called when the animation instance is initialized
	virtual void NativeInitializeAnimation() override;
	// Function called to update the animation instance each frame
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void InitializationCapsule();

#pragma endregion 

/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------*[Other Classes && Components]*-----------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region Other Classes && Components
protected:
	UPROPERTY(BlueprintReadOnly, Category = Classes) ABaseCharacter*			 BaseCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Classes) UEnhancedMovementComponent* EnhancedMovementComponent;
	UPROPERTY()										 AWeapon*					 EquippedWeapon;

#pragma endregion

/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*----------------------------------------------------*[Variables]*-------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region Variables

protected:
	// Enums
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"))
	ECameraViewPerspective CameraViewPerspective;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"))
	EWeaponType WeaponTypes;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool Equip_Weapon;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanReload;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanSwapWeapon;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool HoldingFlag;


	/* "Variable calls from other classes" */
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float GroundVelocitySize;

	/* Movement Variables */
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanSprint;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanCrouch;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanAim;

	/*Ground and Air */
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsFalling;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsMovingOnGround;


	/* Input Properties */
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float HorizontalDirection;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float VerticalDirection;

	/* Aim Properties*/
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float AimOffsetYaw;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float AimOffsetPitch;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float RotateYaw;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float YawDelta;


	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) ECharacterRotateDirection CharacterRotateDirection;

/* Aim Offset varb. */
private:
	// AimOffset Rotators variables
	FRotator StartingAimRotation;

	// AimYawDelta Rotators variables
	FRotator RotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

#pragma endregion


#pragma region Functions
protected:
	void AimOffset();
	void OffSetYawForLean(float ClampNegative, float ClampPositive);
	void RotateCharacter();

public:
	bool AimOffSetIsHigh_DisableAim();
#pragma endregion

#pragma region HandSocketsAdjustment
private:
	void HandSocketTransforms(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool RightHandRotbLocallyControlled;
#pragma endregion 

#pragma region Turn In Place
#pragma endregion
};
