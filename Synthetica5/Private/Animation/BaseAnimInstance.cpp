// ©2023 Reverse - A Company.All rights reserved.

#include "Animation/BaseAnimInstance.h"

#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();


	BaseCharacter= Cast<ABaseCharacter>(TryGetPawnOwner());

	if (BaseCharacter)
	{
		BaseCharacterMovementComponent = BaseCharacter->FindComponentByClass<UBaseCharacterMovementComponent>();
	}
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BaseCharacterIsValid())
	{
		UpdateCharacterAnimationProperties();
	}
}

#pragma region CheckIsValid
bool UBaseAnimInstance::BaseCharacterIsValid()
{
	if (BaseCharacter == NULL)
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
		LOG_WARNING("BaseAnimInstance.cpp | BaseCharacter == NULL-PTR")
	}
	return BaseCharacter != nullptr;
}
#pragma endregion

#pragma region Functions

void UBaseAnimInstance::UpdateCharacterAnimationProperties()
{
	if (BaseCharacter)
	{
		MovementInputIsActive	= BaseCharacter->GetMovementInputActive();
		CanJump					= BaseCharacter->GetJumpPressed() && BaseCharacterMovementComponent->IsFalling();
		CameraViewPerspective	= BaseCharacter->CustomCameraComponent->GetCameraViewPerspective();


		/* Character Rotation */
		IsTurningRight			= BaseCharacter->GetTurningRight();
		IsTurningLeft			= BaseCharacter->GetTurningLeft();

		///* First Person Perspective Directions */ This booleans reserved for future uses

		HorizontallyDirectionsValue_FPP	= BaseCharacter->GetHorizontallyDirections_FPP();
		VerticallyDirectionsValue_FPP	= BaseCharacter->GetVerticallyDirections_FPP();
		//CharacterMovingForward  = BaseCharacter->GetCharacterIsMovingForward();
		//CharacterMovingBackward = BaseCharacter->GetCharacterIsMovingBackward();
		//CharacterMovingRight    = BaseCharacter->GetCharacterIsMovingRight();
		//CharacterMovingLeft     = BaseCharacter->GetCharacterIsMovingLeft();



	}
	else { LOG_WARNING("BaseAnimInstance.cpp | BaseCharacter is  <Invalid>"); }

	if (BaseCharacterMovementComponent)
	{
		VelocitySize			= BaseCharacterMovementComponent->GetCurrentVelocitySize();


		IsInAir					= BaseCharacterMovementComponent->IsFalling();
		IsOnGround				= BaseCharacterMovementComponent->IsMovingOnGround();

		CanCrouch				= BaseCharacterMovementComponent->IsCrouchingCustom();
		CanSprint				= BaseCharacterMovementComponent->GetSprintButtonPressedOrNot();
		CanSlide				= BaseCharacterMovementComponent->GetIsSliding();
		CanProne				= BaseCharacterMovementComponent->GetIsProne();

		// WallRun
		IsWallRunning		= BaseCharacterMovementComponent->IsWallRunning();
		IsWallRunningRight  = BaseCharacterMovementComponent->WallRunningIsRight();
	}
	else { LOG_WARNING("BaseAnimInstance.cpp | BaseCharacterMovementComponent is  <Invalid>"); }

	if (VelocitySize == 0 && !IsInAir)
	{
		AimOffset_Yaw	= BaseCharacter->GetAimOffSetYaw();
		AimOffset_Pitch = BaseCharacter->GetAimOffSetPitch();
	}


	if (VelocitySize > 0.f && !IsInAir)
	{
		float DeltaSecond = GetWorld()->GetDeltaSeconds();
		if (BaseCharacterMovementComponent)
		{
			if (BaseCharacterMovementComponent->IsCrouching() == false)
			{

				if (VelocitySize > 300.f)
				{
					OffSetYawForLean(DeltaSecond, 6.f, -90, 90.f);
				}
				else
				{
					fixFrameRefreshTime += DeltaSecond;
				}

				if (fixFrameRefreshTime >= 0.2f && VelocitySize <= 300.0f)
				{
					OffSetYawForLean(DeltaSecond, 6.f, -45.0f, 45.0f);
					fixFrameRefreshTime = 0.f;
				}
			}
			else
			{
				OffSetYawForLean(DeltaSecond, 6.f, -90, 90.f);
			}
		}
	}
}

void UBaseAnimInstance::OffSetYawForLean(float DeltaTime, float Interpolate, float ClampNegative, float ClampPositive)
{
	RotationLastFrame = CharacterRotation;
	CharacterRotation = BaseCharacter->GetActorRotation();

	float TargetThreshold = 5.0f;
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, RotationLastFrame);

	const float Target = (Delta.Yaw / DeltaTime) / TargetThreshold;
	const float Interp = FMath::FInterpTo(YawDelta, Target, DeltaTime, Interpolate);

	YawDelta = FMath::Clamp(Interp, ClampNegative, ClampPositive);
}

#pragma endregion