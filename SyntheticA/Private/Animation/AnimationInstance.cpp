// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|


#include "Animation/AnimationInstance.h"

#include "Camera/CameraConfiguration.h"
#include "Characters/BaseCharacter.h"
#include "Components/EnhancedMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Net/UnrealNetwork.h"

UAnimationInstance::UAnimationInstance()
{

}

void UAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Initialize();

	if (BaseCharacter)
	{
		CameraConfigurationComponent = BaseCharacter->FindComponentByClass<UCameraConfiguration>();
		EnhancedMovementComponent	 = BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();
	}
}

void UAnimationInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BaseCharacter)
	{
		HorizontalDirection		= BaseCharacter->GetMovementDirectionX();
		VerticalDirection		= BaseCharacter->GetMovementDirectionY();
		CurrentVelocity			= BaseCharacter->GetVelocity().Size();

		CanCrouch				= BaseCharacter->bIsCrouched;
		AimOffset();
	}

	if (EnhancedMovementComponent)
	{
		CanSprint				= EnhancedMovementComponent->GetIsSprinting();
	}

	if (CameraConfigurationComponent)
	{
		CameraViewPerspective	= CameraConfigurationComponent->GetCameraViewPerspective();
	}

	//AimOffset();
}

void UAnimationInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UAnimationInstance::Initialize()
{
	BaseCharacter	= BaseCharacter == nullptr ? Cast<ABaseCharacter>(TryGetPawnOwner()) : BaseCharacter;

	AimOffsetPitch	= 0.0f;
	AimOffsetYaw	= 0.0f;
	YawDelta		= 0.0f;
}

void UAnimationInstance::AimOffset()
{
	FRotator AimRotation				= BaseCharacter->GetBaseAimRotation();
	FRotator CurrentCharacterRotation	= BaseCharacter->GetActorRotation();
	FRotator AimOffset					= AimRotation - CurrentCharacterRotation;

	AimOffset.Normalize();

	AimOffsetPitch						= AimOffset.Pitch;
	AimOffsetYaw						= AimOffset.Yaw;

	AimOffsetYaw						= FMath::ClampAngle(AimOffsetYaw, -90.0f, 90.0f);
	AimOffsetPitch						= FMath::ClampAngle(AimOffsetPitch, -90.0f, 90.0f);
}

void UAnimationInstance::OffSetYawForLean(float DeltaTime, float Interpolate, float ClampNegative, float ClampPositive)
{
	RotationLastFrame					= CharacterRotation;
	CharacterRotation					= BaseCharacter->GetActorRotation();

	float TargetThreshold				= 5.0f;
	const FRotator Delta				= UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, RotationLastFrame);

	const float Target					= (Delta.Yaw / DeltaTime) / TargetThreshold;
	const float Interp					= FMath::FInterpTo(YawDelta, Target, DeltaTime, Interpolate);

	YawDelta							= FMath::Clamp(Interp, ClampNegative, ClampPositive);
}
