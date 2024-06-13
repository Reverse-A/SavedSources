// // @2023 All rights reversed by Reverse-Alpha Studios


#include "CombatSystemComponent.h"
#include "Ascend/Character/ShortDebugMacros.h"

#include "Ascend/Character/BaseCharacter.h"

#include "Ascend/Component/ArsenalComponent.h"

#include "Ascend/Weapon/BaseWeapon.h"
#include "Ascend/Weapon/MeleeWeapon.h"
#include "Ascend/Weapon/RangedWeapon.h"

#include "TimerManager.h"

#include "Kismet/GameplayStatics.h"

UCombatSystemComponent::UCombatSystemComponent()
{
	PrimaryComponentTick.bCanEverTick				 = true;
	PrimaryComponentTick.bStartWithTickEnabled		 = true;
}

void UCombatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PerformCrosshairTrace(CrosshairTraceHitResult);
}

#pragma region AttackSystem
void UCombatSystemComponent::MeleeAttack(bool bMeleeAttack)
{
	if (bMeleeAttack)
	{
		if (BaseCharacter != nullptr)
		{
			BaseCharacter->GetArsenalSystem()->GetMeleeWeapon()->ActivateWeaponOverlapDynamics(true);
			BaseCharacter->GetWorldTimerManager().SetTimer
			(
				WeaponTimerHandle,
				this,
				&UCombatSystemComponent::MeleeAttackFinished,
				1.5
			);
		}
	}
}

void UCombatSystemComponent::MeleeAttackFinished()
{
	if (BaseCharacter)
	{
		BaseCharacter->SetAttackButtonPressed(false);
		BaseCharacter->GetArsenalSystem()->GetMeleeWeapon()->ActivateWeaponOverlapDynamics(false);
	}
}

void UCombatSystemComponent::RangeAttack()
{
	if (bFire == false) return;

	if (BaseCharacter != nullptr)
	{
		RangeWeapon = RangeWeapon == nullptr ? BaseCharacter->GetArsenalSystem()->GetRangedWeapon() : RangeWeapon;

		if (RangeWeapon)
		{
			EWeaponFiringMode WeaponFireMode = RangeWeapon->GetRangedWeaponProperties().GetWeaponFireMode();
			WeaponFireRate = RangeWeapon->GetRangedWeaponProperties().GetFireRate();
			switch (WeaponFireMode)
			{
			case EWeaponFiringMode::SingleShot:
				HandleSingleShot();
				break;
			case EWeaponFiringMode::SemiAutomatic:
				HandleSemiAutomaticFire();
				break;		
			case EWeaponFiringMode::Automatic:
				HandleAutomaticFire();
				break;
			}
		}		
	}
}

void UCombatSystemComponent::HandleSingleShot()
{
	RangeWeapon->Fire(CrosshairImpactPoint, CrosshairTraceHitResult);

	GetWorld()->GetTimerManager().SetTimer
	(
		WeaponTimerHandle,
		this,
		&UCombatSystemComponent::SingleShotTimerFinished,
		WeaponFireRate
	);
}

void UCombatSystemComponent::SingleShotTimerFinished()
{
	bFire = false;
	BaseCharacter->SetAttackButtonPressed(false);
}

/*SemiAutomatic*/
void UCombatSystemComponent::HandleSemiAutomaticFire()
{
	if (SemiShotCount == 0)
	{
		bFire = false;
		SemiShotCount = 3;
		AutomaticFireTimerFinished();
	}
	else
	{
		SemiShotCount--;
		RangeWeapon->Fire(CrosshairImpactPoint, CrosshairTraceHitResult);
		GetWorld()->GetTimerManager().SetTimer
		(
			WeaponTimerHandle,
			this,
			&UCombatSystemComponent::HandleSemiAutomaticFire,
			WeaponFireRate
		);
	}
}

/*Automatic*/
void UCombatSystemComponent::HandleAutomaticFire()
{
	RangeWeapon->Fire(CrosshairImpactPoint, CrosshairTraceHitResult);
	GetWorld()->GetTimerManager().SetTimer
	(
		WeaponTimerHandle,
		this,
		&UCombatSystemComponent::AutomaticFireTimerFinished,
		WeaponFireRate
	);
}

void UCombatSystemComponent::AutomaticFireTimerFinished()
{
	if (bFire == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(WeaponTimerHandle);
		return;
	}
	HandleAutomaticFire();
}

bool UCombatSystemComponent::CanAttack()
{
	if (BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() == nullptr) return false;
	if (BaseCharacter->GetAttackButtonPressed() == true) return false;
	if (BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon()->GetWeaponType() == EWeaponType::EWT_Melee) 	return true;

	return false;	
}
bool UCombatSystemComponent::CanFire()
{
	if (BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() == nullptr) return false;
	if (BaseCharacter->GetAttackButtonPressed() == true) return false;
	if (BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon()->GetWeaponType() == EWeaponType::EWT_Ranged)	return true;

	return false;
}
bool UCombatSystemComponent::CanAim()
{
	if (BaseCharacter->GetAimButtonPressed() == true || BaseCharacter->DisableAim())
	{
		return false;
	}
	return true;
}
bool UCombatSystemComponent::IsAiming()
{
	return BaseCharacter->GetAimButtonPressed();
}
#pragma endregion


void UCombatSystemComponent::PerformCrosshairTrace(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.0f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld
	(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if (BaseCharacter)
		{
			float DistanceToCharacter = (BaseCharacter->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 75.0f);
		}

		float TRACE_MAX_LENGTH = 80000;
		FVector End = Start + CrosshairWorldDirection * TRACE_MAX_LENGTH;

		GetWorld()->LineTraceSingleByChannel
		(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);

		CrosshairTraceHitResult = TraceHitResult;
		CrosshairImpactPoint = End;
	}
}