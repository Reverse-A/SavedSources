// ©2023 Reverse-A. All rights reserved.

#pragma once

// This is the header file for the UBaseAnimInstance class, which is a derived class of UAnimInstance.
// It contains declarations for various member variables and functions.


#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Synthetica/Characters/BaseCharacter.h"
#include "Synthetica/EnumTypes/CharacterTypes.h"
#include "BaseAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EIKType : uint8
{
	EIKT_Leave	 UMETA(DisplayName = "IKFoot Leaving State"),
	EIKT_Process UMETA(DisplayName = "IKFoot Processing State"),

	ECS_MAX		 UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class SYNTHETICA_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Function called when the animation instance is initialized
	virtual void NativeInitializeAnimation() override;

	// Function called to update the animation instance each frame
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// Function for the FABRIK algorithm used in inverse kinematics
	void FabrikMethod();

	// BaseCharacter reference
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class ABaseCharacter* BaseCharacter;

	// CharacterMovement component reference
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UCharacterMovementComponent* CharacterMovement;

	// Movement speed of the character
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bCharacterIsMoving;

	// Flag indicating if the character is accelerating
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	// Flag indicating if the character is in the air
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsNormalJump;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bSprintJump;

	// Flag indicating if the character is crouched
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsJumping;

	// Flag indicating if the character is throwing a grenade
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bGrenadeThrowning;

	// Velocity of the character
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FVector Velocity;

	// Yaw rotation of the character
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Yaw;

	// Pitch rotation of the character
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Pitch;

	// Roll rotation of the character
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Roll;

	// Delta yaw rotation of the character
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawDelta;

	// Yaw offset used for leaning animation
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawOffset;

	// Yaw rotation for additive overlay calculations
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;

	// Pitch rotation for additive overlay calculations
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Pitch;

	// Movement input in the X-axis
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Movement_X;

	// Movement input in the Y-axis
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Movement_Y;

	// Distance to the ground on the right foot for foot IK
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RightDistanceToGround;

	// Distance to the ground on the left foot for foot IK
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LeftDistanceToGround;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool HoldingWall;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsGround;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool CharacterIsFalling;

protected:
	// Function to calculate pitch, yaw, and roll rotations
	virtual void PitchYawRollReturn(float DeltaTime);

	// Function to offset yaw rotation for leaning animation
	virtual void OffSetYawForLean(float DeltaTime, float Interpolate, float ClampNegative, float ClampPositive);

private:
	// Store the delta time for animations
	float DeltaTimeStore;

	// Reference to the equipped weapon class
	class AWeapon* WeaponClass;

	// Current state of the character
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		EWeaponEquippedStatus CurrentCharacterState;

	// Rotate statement enumeration for rotating the character in place
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		ERotateStatement RotateStatementEnum;

	// Ranged weapon type enumeration for animation purposes
	UPROPERTY(BlueprintReadOnly, Category = WeaponType, meta = (AllowPrivateAccess = "true"))
		ERangedWeaponTypes RangeWeaponTypes;

	// IK type enumeration for foot IK state
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		EIKType IKType;

	// Last frame rotation of the character
	FRotator RotationLastFrame;

	// Rotation of the character
	FRotator CharacterRotation;

	// Rotation of the right hand
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FRotator RightHandRotation;

	// Delta rotation of the character
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FRotator DeltaRotation;

	// Transform of the left hand for IK calculations
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FTransform LeftHandTransform;

	// Slope pitch angle for slope-based animations
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float SlopePitchDegreeAngle;

	// Flag indicating if foot IK should be applied
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool bShouldDoIK;

	// Alpha value for foot IK blend
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float IKAlphaValue;

	// Difference in height between the character's feet
	float DifferenceBetweenTwoFeet;

	// Time lapse for flagged foot IK state
	float FlaggedTimeLapse;

	// Timer for reaching zero movement speed
	float CharacterATZeroMovementSpeedTimer;

	// Alpha blend value for foot IK
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float FootIKAplhaBlend;

	// Flag indicating if the character is sprinting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting;

	// Flag indicating if a weapon is equipped
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped;

	// Flag indicating if the character is aiming
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	// Flag indicating if the character is idle (not moving)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsIdle;

	// Flag indicating if the character is reloading
	UPROPERTY(BlueprintreadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsReloading;

	UPROPERTY(BlueprintreadOnly, meta = (AllowPrivateAccess = "true"))
	bool bDoFabrik;

	// Function to calculate yaw for aim offset
	void YawForAimCalculation(float DeltaTime);

	// Function to activate foot IK
	void ActiveFootIK(float DeltaTime);

	// Function to disable foot IK
	void DisableFootIK();

	// Function for foot IK progress
	void FootIKProgress(float DeltaTime);


	void SelectSocketName();
	FName LeftHandSocketName;
	FName LeftHandSocketNameAim;
	FName LefHandSocketNameWhileSprinting;

public:
	// Add additional public member functions and variables as required for the class
};