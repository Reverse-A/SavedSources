// @2023 All rights reversed by Reverse-Alpha Studios


#include "Animation/CharacterAnimInstance.h"

#include "Characters/BaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponTypes.h"
#include "Net/UnrealNetwork.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	CharacterRotateDirection = ECharacterRotateDirection::ECRD_None;
	
}

void UCharacterAnimInstance::InitializationCapsule()
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(TryGetPawnOwner()) : BaseCharacter;
	if (BaseCharacter)
	{
		AimOffsetPitch				= 0.0f;
		AimOffsetYaw				= 0.0f;
		YawDelta					= 0.0f;

		EnhancedMovementComponent	= BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();
	}
	CameraViewPerspective = ECameraViewPerspective::ECVP_ThirdPerson;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	InitializationCapsule();
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(TryGetPawnOwner()) : BaseCharacter;

	if (BaseCharacter)
	{
		// Class
		EquippedWeapon			= BaseCharacter->GetEquippedWeapon();
		Equip_Weapon			= BaseCharacter->IsWeaponEquipped();

		CanCrouch			= BaseCharacter->bIsCrouched;
		CanAim				= BaseCharacter->IsAiming();

		if (BaseCharacter->IsAiming() || GroundVelocitySize <= 100.0f)
		{
			AimOffset();
		}

		if (EquippedWeapon)
		{
			WeaponTypes = EquippedWeapon->Get_WeaponType();

			if (BaseCharacter->IsAiming())
			{
				HorizontalDirection = BaseCharacter->GetMovementDirectionX();
				VerticalDirection   = BaseCharacter->GetMovementDirectionY();
			}

			UCombatComponent* CombatComponent = BaseCharacter->Get_CombatComponent();
			if (CombatComponent)
			{
				CanReload	  = CombatComponent->IsReloading();
				CanSwapWeapon = CombatComponent->IsSwapingWeapon();

				HoldingFlag   = CombatComponent->Get_HoldingFlag();
			}
			else
			{
				CombatComponent = CombatComponent == nullptr ? BaseCharacter->Get_CombatComponent() : CombatComponent;
			}
		}
	}
	else{BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());}


	if (EnhancedMovementComponent)
	{
		if (EnhancedMovementComponent->IsMovingOnGround() && !EnhancedMovementComponent->IsFalling())
		{
			if (BaseCharacter)
			{
				GroundVelocitySize = BaseCharacter->GetVelocity().Size();


				if (!EnhancedMovementComponent->IsFalling())
				{
					if (GroundVelocitySize > 0.0f && GroundVelocitySize <= 400.0f)
					{
						OffSetYawForLean(-15.0f, 15.0f);
					}

					if (GroundVelocitySize > 400.0f)
					{
						OffSetYawForLean(-35.0f, 35.0f);
					}
				}
			}
		}
		float SprintThresholdValue = 400.0f;

		CanSprint		  = EnhancedMovementComponent->GetIsSprinting() && GroundVelocitySize > SprintThresholdValue;
		IsFalling		  = EnhancedMovementComponent->IsFalling();
		IsMovingOnGround  = EnhancedMovementComponent->IsMovingOnGround();
	}
	else{if (BaseCharacter){EnhancedMovementComponent = BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();}}	

	if (EquippedWeapon != nullptr)
	{
		HandSocketTransforms(DeltaTime);
	}
	else
	{
	}

}


void UCharacterAnimInstance::AimOffset()
{
	bool CanReturn =
		GroundVelocitySize > 400.0f ||
		EnhancedMovementComponent->IsFalling() ||
		EnhancedMovementComponent->IsSwimming() ||
		BaseCharacter == nullptr;

	if (CanReturn) return;


	FVector Velocity = BaseCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	float GroundVelocity = Velocity.Size();
	bool IsInAir = EnhancedMovementComponent->IsFalling();

	if (GroundVelocity == 0.0f && IsInAir == false)
	{
		FRotator CurrentAimRotation = FRotator(0.0f, BaseCharacter->GetBaseAimRotation().Yaw, 0.0f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AimOffsetYaw = DeltaAimRotation.Yaw;
	}

	if (GroundVelocity > 0.0f || IsInAir)
	{
		StartingAimRotation = FRotator(0.0f, BaseCharacter->GetBaseAimRotation().Yaw, 0.0f);
		AimOffsetYaw = 0.0f;
	}

	AimOffsetPitch = BaseCharacter->GetBaseAimRotation().Pitch;
	if (AimOffsetPitch > 90.0f && !BaseCharacter->IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AimOffsetPitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AimOffsetPitch);
	}
}


void UCharacterAnimInstance::OffSetYawForLean(float ClampNegative, float ClampPositive)
{
	RotationLastFrame					= CharacterRotation;
	CharacterRotation					= BaseCharacter->GetActorRotation();

	float TargetThreshold				= 5.0f;
	const FRotator Delta				= UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, RotationLastFrame);

	float DeltaTime						= GetWorld()->GetDeltaSeconds();

	const float Target					= (Delta.Yaw / DeltaTime) / TargetThreshold;
	const float Interp					= FMath::FInterpTo(YawDelta, Target, DeltaTime, 5.0f);

	YawDelta							= FMath::Clamp(Interp, ClampNegative, ClampPositive);
}

void UCharacterAnimInstance::RotateCharacter()
{
	bool ReturnTypes =
		EnhancedMovementComponent->IsFalling()			||
		!EnhancedMovementComponent->IsMovingOnGround()	||
		EnhancedMovementComponent->IsSwimming()			||
		EnhancedMovementComponent->IsFlying();
	if (ReturnTypes) return;
}

bool UCharacterAnimInstance::AimOffSetIsHigh_DisableAim()
{
	if ((AimOffsetYaw > 90.0f || AimOffsetYaw < -90) || (AimOffsetPitch > 50.0f || AimOffsetPitch < -50.0f))
	{
		return true;
	}
	else
	{
		return false;
	}
}

#pragma region HandSocketsAdjustment
void UCharacterAnimInstance::HandSocketTransforms(float DeltaTime)
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr || EnhancedMovementComponent->IsFalling() || CanReload == true) return;

	if (EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BaseCharacter->GetMesh())
	{
		if (BaseCharacter->IsLocallyControlled())
		{
			RightHandRotbLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation
			(
				RightHandTransform.GetLocation(),
				RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BaseCharacter->GetHitTarget())
			);

			UCombatComponent* CombatComponentClass = BaseCharacter->Get_CombatComponent();
			if (CombatComponentClass)
			{
				if (CombatComponentClass->IsActorTargetedbyCrosshair())
				{
					RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 5.0f);
				}
				else
				{
					RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 30.0f);
				}
			}
		}



		if (WeaponTypes == EWeaponType::EWT_AssaultRifle)
		{
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LHS_AssaultRifle"), ERelativeTransformSpace::RTS_World);
		}
		else if (WeaponTypes == EWeaponType::EWT_ScatterWeapons)
		{
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LHS_Shotgun"), ERelativeTransformSpace::RTS_World);
		}
		else if (WeaponTypes == EWeaponType::EWT_SubmachineGun)
		{
			return;
		}
		else if (WeaponTypes == EWeaponType::EWT_Pistol)
		{
			return;
		}
		else if (WeaponTypes == EWeaponType::EWT_SniperRifle)
		{
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LHS_SniperRifle"), ERelativeTransformSpace::RTS_World);
		}
		else if (WeaponTypes == EWeaponType::EWT_GrenadeLauncher)
		{
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LHS_GrenadeLauncher"), ERelativeTransformSpace::RTS_World);
		}

		FVector OutPosition;
		FRotator OutRotation;
		BaseCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		FVector InPosition = OutPosition;
		FRotator InRotation = OutRotation;
		LeftHandTransform.SetLocation(InPosition);
		LeftHandTransform.SetRotation(FQuat(InRotation));


		bool RunDebug = false;
		if (RunDebug == true)
		{
			FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World);
			FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000.0f, FColor::Red);
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), BaseCharacter->GetHitTarget(), FColor::Cyan);
		}
	}
}







#pragma endregion