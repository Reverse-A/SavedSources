// @2023 All rights reversed by Reverse-Alpha Studios


#include "PCharacterAnimInstance.h"
#include "QuantumWarfare/Character/CharacterModule.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CharacterModule = CharacterModule == nullptr ? Cast<ACharacterModule>(TryGetPawnOwner()) : CharacterModule;
}

void UPCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterModule)
	{
		MovementSpeed = CharacterModule->GetVelocity().Size();

		bShouldMove = MovementSpeed <= 3.0f ? false : true;

		if (CharacterModule->GetMovementComponent())
		{
			bIsFalling = CharacterModule->GetMovementComponent()->IsFalling();
		}	 
	}
}

void UPCharacterAnimInstance::AimOffSet()
{
	if (CharacterModule == nullptr)
	{
		CharacterModule = Cast<ACharacterModule>(TryGetPawnOwner());
	}

	FVector Velocity = CharacterModule->GetVelocity();
	Velocity.Z = 0.0f;

	float GroundVelocitySize = Velocity.Size();
	bool bIsInAir = CharacterModule->GetCharacterMovement()->IsFalling();

	if (GroundVelocitySize > 0.0f || bIsInAir)
	{
		AimOffsetYaw = 0.0f;
	}

	AimOffsetPitch = CharacterModule->GetBaseAimRotation().Pitch;

	if (AimOffsetPitch > 90.0f)
	{
		FVector2D InRange(270.0f, 360.0f);
		FVector2D OutRange(-90.0f, 0.0f);

		AimOffsetPitch = FMath::GetMappedRangeValueClamped
		(
			InRange, 
			OutRange, 
			AimOffsetPitch
		);
	}
}

void UPCharacterAnimInstance::YawOffsetForLeaning(float ClampNegativeThreshold, float ClampPositiveThreshold)
{
	/*	(UPCharacterAnimInstance::YawOffsetForLeaning)
	* This function calculates the yaw offset for leaning of the character based on its rotation changes between frames.
	*/ 


	// Check if CharacterModule is nullptr, if so, try to get it from the PawnOwner
	CharacterModule = CharacterModule == nullptr ? Cast<ACharacterModule>(TryGetPawnOwner()) : CharacterModule;

	// Store the current rotation as the last frame's rotation
	RotationLastFrame = CharacterRotation;

	// Get the current rotation of the character
	CharacterRotation = CharacterModule->GetActorRotation();

	// Define a threshold for the target value used in interpolation
	float TargetThreshold = 5.0f;

	// Calculate the delta rotation between the current frame and the last frame
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator
	(
		CharacterRotation,
		RotationLastFrame
	);

#pragma region NormalizedDeltaRotator Operation Explanation
	/*
		Normalized Delta Rotator operation explaining.

		1. We start by subtracting the components of Rotator B from the corresponding components of Rotator A:
		Delta=CharacterRotation?RotationLastFrame
		So, for each component:
		

		First step formula
		DeltaYaw   = Yaw.A - Yaw.B = ?
		DeltaPitch = Yaw.A - Yaw.B = ?
		DeltaRoll  = Yaw.A - Yaw.B = ?

		Random example values of CharacterRotation and Rotation last Frame

		CharacterRotation  = A: Yaw = 300, Pitch = 200, Roll = -50
		RotationLastFrame = B: Yaw = 301, Pitch = 202, Roll = -55

		DeltaYaw   = Yaw.A - Yaw.B = 300 - 301   = -1 
		DeltaPitch = Yaw.A - Yaw.B = 200 - 202   = -2
		DeltaRoll  = Yaw.A - Yaw.B = -50 - (-55) = 5
	

		2. Normalization: 
		After subtracting the components, we normalize the resulting Delta Rotator. Normalizing means scaling the components so that the resulting vector has a length of 1. This ensures that the rotation represents a unit rotation.

		Delta.Normalize()
		The normalization process divides each component of Delta by the magnitude of Delta:

		Magnitude of Delta= DeltaYaw^2 + DeltaPitch^2 + DeltaRoll^2

		In this case, the magnitude would be calculated as:

		?(?1)^2 + (-2)^2 + 5^2 = 1 + 4+ 25 = 30

		After normalization, each component of Delta will be divided by the magnitude:

		NormalizedDeltaYaw   = DeltaYaw   / 30
		NormalizedDeltaPitch = DeltaPitch / 30
		NormalizedDeltaRoll  = DeltaRoll  / 30

		These values represent the normalized delta rotation.

		So, the final result would be a normalized delta rotation with components:

		NormalizedDeltaYaw   = ?0.1826
		NormalizedDeltaPitch = ?0.3651
		NormalizedDeltaRoll  = 0.9129

?
		These values indicate the unit rotation required to go from the rotation represented by CharacterRotation to the rotation represented by RotationLastFrame.
	*/
#pragma endregion

	// Get the delta time for this frame
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// Calculate the target value based on the yaw component of the delta rotation and the delta time
	const float Target = (Delta.Yaw / DeltaTime) / TargetThreshold;

	// Interpolate between the current YawDelta and the target value over time
	const float Interp = FMath::FInterpTo(YawDelta, Target, DeltaTime, 5.0f);

	// Clamp the interpolated value to ensure it stays within the specified thresholds
	YawDelta = FMath::Clamp(Interp, ClampNegativeThreshold, ClampPositiveThreshold);
}
