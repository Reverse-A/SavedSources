// ©2023 Reverse - A Company. All rights reserved.


#include "Animation/CoreAnimProcessor.h"

#include "Synthetica/EnumTypes/CombatTypes.h"

#include "Net/UnrealNetwork.h"


#include "Character/BaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/Weapon.h"




UCoreAnimProcessor::UCoreAnimProcessor()
{
	CharacterRotationType = ECharacterRotation::ECR_None;
	
}

void UCoreAnimProcessor::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());

	if (BaseCharacter)
	{
		EnhancedMovement	= BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();
		CombatComponent		= BaseCharacter->FindComponentByClass<UCombatComponent>();
		CameraSettings		= BaseCharacter->FindComponentByClass<UCameraSettings>();
	}
}


bool UCoreAnimProcessor::CharacterValidation()
{
	if (BaseCharacter == NULL)
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}

	return BaseCharacter != nullptr;
}

void UCoreAnimProcessor::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterValidation())
	{
		UpdateAndStoreData();
	}
}

void UCoreAnimProcessor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCoreAnimProcessor, RightHandRotation);
	DOREPLIFETIME(UCoreAnimProcessor, LeftHandTransform);
	DOREPLIFETIME(UCoreAnimProcessor, FirstPersonHorizontalDirectionValue);
	DOREPLIFETIME(UCoreAnimProcessor, FirstPersonVerticalDirectionValue);

}

void UCoreAnimProcessor::UpdateAndStoreData()
{
	AimOffset();
	LeftHandSocketAdjustment();

	// Retrieve information from the BaseCharacter
	if (BaseCharacter)
	{
		if (BaseCharacter->GameplayIsDisabled())
		{
			FirstPersonHorizontalDirectionValue = 0.0f;
			FirstPersonVerticalDirectionValue	= 0.0f;
			CurrentVelocityMagnitude			= 0.0f;
			bMovementInputActive				= false;
			CanCrouch							= false;
			CanAim								= false;
			CanSprint							= false;
			return;
		}
		EquippedWeapon = BaseCharacter->GetEquippedWeapon();
		CameraViewPerspective = BaseCharacter->GetCameraSettingsComponent()->GetCameraViewPerspective();
		bMovementInputActive  = BaseCharacter->GetMovementInputIsActivated();

		// Check if the character is in first person or aiming
		if (CameraViewPerspective == ECameraViewPerspective::ECVP_FirstPerson || BaseCharacter->IsAiming())
		{
			FirstPersonHorizontalDirectionValue = BaseCharacter->GetMovementDirectionX();
			FirstPersonVerticalDirectionValue	= BaseCharacter->GetMovementDirectionY();
		}		
		//bCanJump							= BaseCharacter->GetJumpPressed() && EnhancedMovement->IsFalling();
		CanCrouch							= BaseCharacter->bIsCrouched;
		CanAim								= BaseCharacter->IsAiming();
	}

	// Retrieve information from EnhancedMovement
	if (EnhancedMovement)
	{
		CurrentVelocityMagnitude = EnhancedMovement->Velocity.Size();
		bIsMovingOnGround		 = EnhancedMovement->IsMovingOnGround();
		bIsFalling				 = EnhancedMovement->IsFalling();
		bIsInAir				 = EnhancedMovement->GetLastUpdateVelocity().Z > 0.f;
		CanSprint				 = EnhancedMovement->GetIsSprinting();
		CanProne				 = EnhancedMovement->GetIsProning();
		CanSlide				 = EnhancedMovement->GetIsSliding();
		IsWallRunning			 = EnhancedMovement->IsWallRunning();
		IsWallRunningRight		 = EnhancedMovement->WallRunningIsRight();
		CanMantle				 = EnhancedMovement->GetCanMantle();
		CanTallMantle			 = EnhancedMovement->GetTallMantle();
		SlopePitchDegreeAngle	 = EnhancedMovement->GetSlopePitchDegreeAngle();
	} 

	// Retrieve information from CombatComponent
	if (CombatComponent)
	{
		bWeaponEquipped = CombatComponent->IsWeaponEquipped();
		CanReload		= CombatComponent->GetIsReloading();
	}

	bUseFABRIK     = BaseCharacter->GetCombatTypes() != ECombatTypes::ECT_Reloading;
}


void UCoreAnimProcessor::AimOffset()
{
	FVector Velocity = BaseCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	float Speed = Velocity.Size();
	AimOffsetPitch = BaseCharacter->GetBaseAimRotation().Pitch;

	if (Speed == 0.0 && !bIsInAir)
	{
		FRotator CurrentAimRotation = FRotator(0.0f, BaseCharacter->GetBaseAimRotation().Yaw, 0.0f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AimOffsetYaw = FMath::Clamp(DeltaAimRotation.Yaw, -180.f, 180.f);
	}

	if (Speed > 0.f || bIsInAir)
	{
		StartingAimRotation = FRotator(0.f, BaseCharacter->GetBaseAimRotation().Yaw, 0.f);
		AimOffsetYaw = 0.f;
	}

	if (AimOffsetPitch > 90.f && !BaseCharacter->IsLocallyControlled())
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AimOffsetPitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AimOffsetPitch);
	}
	TemporarilyRotation();
}

void UCoreAnimProcessor::TemporarilyRotation()
{
	if (BaseCharacter && BaseCharacter->GameplayIsDisabled())
	{
		return;
	}
	if (AimOffsetYaw > 90.0f || AimOffsetYaw < -90.0f)
	{
		BaseCharacter->SetAutoRun(true);
		RotateCharacter = true;
	}

	if (RotateCharacter)
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		AutoRunTriggerDelay += DeltaTime;

		if (AutoRunTriggerDelay >= 0.5f)
		{
			BaseCharacter->SetAutoRun(false);
			AutoRunTriggerDelay = 0.0f;
			RotateCharacter = false;
		}
	}
}



//void UCoreAnimProcessor::TurnInPlace()
//{
//	if (AimOffsetYaw >= 90.0f)
//	{
//		LOG_WARNING("TurningRight")
//		CharacterRotationType = ECharacterRotation::ECR_Right;
//		bIsRotating = true;
//	}
//	else if (AimOffsetYaw <= -90.0f)
//	{
//		LOG_WARNING("TurningLeft")
//		CharacterRotationType = ECharacterRotation::ECR_Left;
//		bIsRotating = true;
//	}
//
//	if (CharacterRotationType != ECharacterRotation::ECR_None)
//	{
//		float DeltaTime			= GetWorld()->GetDeltaSeconds();
//		CharacterRotationTimer += DeltaTime;
//		float CharacterRotationDelay = 0.5f;
//		LOG_WARNING("Turning")
//
//		if (CharacterRotationTimer >= CharacterRotationDelay)
//		{
//			LOG_WARNING("TurningDone")
//			CharacterRotationType	= ECharacterRotation::ECR_None;
//			bIsRotating = false;
//			CharacterRotationTimer = 0.0f;
//		}
//	}
//}

void UCoreAnimProcessor::OffSetYawForLean(float DeltaTime, float Interpolate, float ClampNegative, float ClampPositive)
{
	RotationLastFrame = CharacterRotation;
	CharacterRotation = BaseCharacter->GetActorRotation();

	float TargetThreshold = 5.0f;
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, RotationLastFrame);

	const float Target = (Delta.Yaw / DeltaTime) / TargetThreshold;
	const float Interp = FMath::FInterpTo(YawDelta, Target, DeltaTime, Interpolate);

	YawDelta = FMath::Clamp(Interp, ClampNegative, ClampPositive);


	float DeltaSecond = GetWorld()->GetDeltaSeconds();
	if (EnhancedMovement->IsCrouching())
	{
		OffSetYawForLean(DeltaSecond, 6.f, -90.0f, 90.0f);
	}
	else
	{
		if (CurrentVelocityMagnitude > 300.0f)
		{
			OffSetYawForLean(DeltaSecond, 4.0f, -90.0f, 90.0f);
		}
		else
		{
			YawDeltaRefreshTimer += DeltaSecond;
		}

		if (YawDeltaRefreshTimer >= 0.2f && CurrentVelocityMagnitude <= 300.0f)
		{
			OffSetYawForLean(DeltaSecond, 6.f, -45.0f, 45.0f);
			YawDeltaRefreshTimer = 0.f;
		}
	}
}


#pragma region | Control |
bool UCoreAnimProcessor::IsMoving()
{
	return	CurrentVelocityMagnitude >= 0.1f;
}
bool UCoreAnimProcessor::IsVelocityZero()
{
	return CurrentVelocityMagnitude == 0.f;
}
#pragma endregion

#pragma region |Fabrik|
void UCoreAnimProcessor::LeftHandSocketAdjustment()
{
	if (EquippedWeapon == nullptr) return;
	if (EquippedWeapon->GetWeaponMesh() && BaseCharacter->GetHandsMesh())
	{
		FName LeftHandSocketName;
		EWeaponType WeaponType = BaseCharacter->GetEquippedWeapon()->GetWeaponType();
		switch (WeaponType)
		{
		case EWeaponType::EWT_Pistol:
			LeftHandSocketName = "LHS_Pistol";
			break;
		case EWeaponType::EWT_SMG:
			LeftHandSocketName = "LHS_SMG";
			break;
		case EWeaponType::EWT_Shotgun:
			LeftHandSocketName = "LHS_Shotgun";
			break;
		case EWeaponType::EWT_AssaultRifle:
			LeftHandSocketName = "LHS_Assault";
			break;
		case EWeaponType::EWT_SniperRifle:
			LeftHandSocketName = "LHS_Sniper";
			break;
		case EWeaponType::EWT_GrenadeLauncher:
			LeftHandSocketName = "LHS_GrenadeLauncher";
			break;
		case EWeaponType::EWT_RocketLauncher:
			LeftHandSocketName = "LHS_Rocket";
			break;
		}
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(LeftHandSocketName, ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		BaseCharacter->GetHandsMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));



		if (BaseCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation
			(
				RightHandTransform.GetLocation(),
				RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BaseCharacter->GetHitTarget())
			);

			float DeltaTime = GetWorld()->GetDeltaSeconds();
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 10.f);
		}
	}
}
#pragma endregion

#pragma region | Setters |
#pragma endregion

#pragma region | NETWORKING FUNCTIONS |
#pragma endregion