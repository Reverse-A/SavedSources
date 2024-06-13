// ©2023 Reverse-A. All rights reserved.

#include "CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Synthetica/AI/BaseEnemyAI.h"
#include "Synthetica/Characters/BaseCharacter.h"
#include "Synthetica/EnumTypes/CharacterTypes.h"
#include "Synthetica/PlayerController/BasePlayerController.h"
#include "Synthetica/HUD/BaseHUD.h"
#include "Synthetica/Weapons/RangedWeaponTypes.h"
#include "Synthetica/Weapons/Weapon.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if (BaseCharacter)
	{
		{
			DefaultFOV = BaseCharacter->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
		InitializeCarriedAmmo();
		UpdateHUDGrenades();
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (BaseCharacter)
	{
		bIsSprinting = BaseCharacter->GetSprinting();
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;
		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	Fire();
}

void UCombatComponent::Fire()
{
	if (BaseCharacter->IsAiming() && !BaseCharacter->GetIsInAir() && !BaseCharacter->GetSprinting() && EquippedWeapon)
	{
		if(CanFire())
		{
			bCanFire = false;
			BaseCharacter->PlayFireMontage(bAiming);
			EquippedWeapon->Fire(HitTarget);
			CrosshairShootingFactor = .75;
			StartFireTimer();
		}
	}
}

void UCombatComponent::MeleeAttackPressed(bool bPressed)
{
	bAttackButtonPressed = bPressed;
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

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
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		}
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if (TraceHitResult.GetActor()  && TraceHitResult.GetActor()->Implements<UInteractableCrosshairInterface>())
		{
			HUDPackage.CrosshairsColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairsColor = FLinearColor::White;
		}

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		else
		{
			HitTarget = TraceHitResult.ImpactPoint;
		}
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (BaseCharacter == nullptr || BaseCharacter->Controller == nullptr) return;
	BaseController = BaseController == nullptr ? Cast<ABasePlayerController>(BaseCharacter->Controller) : BaseController;
	if (BaseController)
	{
		HUD = HUD == nullptr ? Cast<ABaseHUD>(BaseController->GetHUD()) : HUD;
		if (HUD)
		{
			if (EquippedWeapon && BaseCharacter->GetCurrentMovementSpeed() < 600.f)
			{
				if (EquippedWeapon == nullptr) return;
				HUDPackage.CrosshairCenter = EquippedWeapon->GetCrosshairTextures().CrosshairCenter;
				HUDPackage.CrosshairLeft = EquippedWeapon->GetCrosshairTextures().CrosshairLeft;
				HUDPackage.CrosshairRight = EquippedWeapon->GetCrosshairTextures().CrosshairRight;
				HUDPackage.CrosshairTop = EquippedWeapon->GetCrosshairTextures().CrosshairTop;
				HUDPackage.CrosshairBottom = EquippedWeapon->GetCrosshairTextures().CrosshairBottom;
			}
			else if (EquippedWeapon && BaseCharacter->GetCurrentMovementSpeed() > 600.f)
			{
				HUDPackage.CrosshairCenter	= EquippedWeapon->GetCrosshairTextures().CrosshairCenter;
				HUDPackage.CrosshairLeft	= nullptr;
				HUDPackage.CrosshairRight	= nullptr;
				HUDPackage.CrosshairTop		= nullptr;
				HUDPackage.CrosshairBottom	= nullptr;
			}

			FVector2D WalkSpeedRange(0.f, BaseCharacter->GetCurrentWalkSpeed());
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = BaseCharacter->GetVelocity();
			Velocity.Z = 0.f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (BaseCharacter->GetIsInAir())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);

			}
			if (bAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.58f, DeltaTime, 20.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 20.f);
			}
			const float NeutralSpreadSize = 0.15f;

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);

			HUDPackage.CrosshairSpread = 
				NeutralSpreadSize + 
				CrosshairVelocityFactor + 
				CrosshairInAirFactor - 
				CrosshairAimFactor + 
				CrosshairShootingFactor;

			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr) return;
	const float JogWalkSpeed = 300.f;
	const float AvarageSprintSpeed = 550.f;
	if (bAiming)
	{
		if (bIsSprinting) return;
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime * 0.4f, EquippedWeapon->GetZoomedInterpSpeed());
	}
	else if (!bAiming && bIsSprinting && BaseCharacter->GetVelocity().Size() > AvarageSprintSpeed)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, 130.f, DeltaTime * 0.2f, EquippedWeapon->GetZoomedInterpSpeed());
	}
	else if (!bAiming && !bIsSprinting && BaseCharacter->GetVelocity().Size() < AvarageSprintSpeed && BaseCharacter->bJumpIsDone)
	{
		const float ZoomInterpSpeed = 4.f;
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime * .5f, ZoomInterpSpeed);
	}
	else if (BaseCharacter->bJumpIsDone || !bAiming)
	{
		const float ZoomInterpSpeed = 2.f;
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime * .5f, ZoomInterpSpeed);
	}

	if (BaseCharacter && BaseCharacter->GetFollowCamera())
	{
		BaseCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}

void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || BaseCharacter == nullptr) return;
	BaseCharacter->GetWorldTimerManager().SetTimer(
		FireTimer, 
		this, 
		&UCombatComponent::FireTimerFinished, 
		EquippedWeapon->FireDelay
	);
}

void UCombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;
	bCanFire = true;

	if (bFireButtonPressed && EquippedWeapon->bAutomatic)
	{
		Fire();
	}

	ReloadEmptyMag();
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;

	bAiming = bIsAiming;
	if (BaseCharacter && EquippedWeapon->GetRangedWeaponTypes() == ERangedWeaponTypes::ERWT_SniperRifle)
	{
		BaseCharacter->ShowSniperScopeWidget(bIsAiming);
	}
}

void UCombatComponent::EquipWeapon(class AWeapon* WeaponToEquip)
{
	if (BaseCharacter == nullptr || WeaponToEquip == nullptr) return;

	if (CombatState != ECombatState::ECS_Null) return;

	IfWeaponEquipped_DropEquippedWeapon();

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	AttachActorToRightHand(EquippedWeapon);

	EquippedWeapon->SetOwner(BaseCharacter);
	EquippedWeapon->SetHUDAmmo();

	UpdateCarriedAmmo();
	PlayEquippedWeaponSound();

	if(!EquippedWeapon->IsAmmoEmpty() && CarriedAmmo != 0)ReloadEmptyMag();

	EquippedWeapon->ShowPickupWidget(false);
	BaseCharacter->SetCurrentCharacterState(EWeaponEquippedStatus::ECS_Armed);
	BaseCharacter->bIsWeaponEquipped = true;
}


void UCombatComponent::IfWeaponEquipped_DropEquippedWeapon()
{
	if (EquippedWeapon)
	{
		DropWeapon();
	}
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
	if (BaseCharacter == nullptr || BaseCharacter->GetMesh() == nullptr || ActorToAttach == nullptr) return;

	const USkeletalMeshSocket* HandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));

	if (HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, BaseCharacter->GetMesh());
	}
}

void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach)
{
	if (BaseCharacter == nullptr || BaseCharacter->GetMesh() == nullptr || ActorToAttach == nullptr || EquippedWeapon == nullptr) return;

	FName LeftHandSocketName;
	switch (EquippedWeapon->GetRangedWeaponTypes())
	{
	case ERangedWeaponTypes::ERWT_Pistol:
		LeftHandSocketName = "LeftHandPistolSocket";
		break;
	case ERangedWeaponTypes::ERWT_AssaultRifle:
		LeftHandSocketName = "LeftHandAssaultRifleSocket";
		break;
	case ERangedWeaponTypes::ERWT_RocketLauncher:
		LeftHandSocketName = "LeftHandSocket";
		break;
	case ERangedWeaponTypes::ERWT_SubmachineGun:
		LeftHandSocketName = "LeftHandSocket";
		break;
	case ERangedWeaponTypes::ERWT_Shotgun:
		LeftHandSocketName = "LeftHandSocket";
		break;
	case ERangedWeaponTypes::ERWT_SniperRifle:
		LeftHandSocketName = "LeftHandSocket";
		break;
	}
	const USkeletalMeshSocket* HandSocket = BaseCharacter->GetMesh()->GetSocketByName(LeftHandSocketName);

	if (HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, BaseCharacter->GetMesh());
	}
}

void UCombatComponent::UpdateCarriedAmmo()
{
	if (EquippedWeapon == nullptr) return;

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetRangedWeaponTypes()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetRangedWeaponTypes()];
	}

	BaseController = BaseController == nullptr ? Cast<ABasePlayerController>(BaseCharacter->Controller) : BaseController;
	if (BaseController)
	{
		BaseController->SetHUDCarriedAmmunation(CarriedAmmo);
	}
}

void UCombatComponent::PlayEquippedWeaponSound()
{
	if (BaseCharacter && EquippedWeapon && EquippedWeapon->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquippedWeapon->EquipSound,
			BaseCharacter->GetActorLocation()
		);
	}
}

void UCombatComponent::ReloadEmptyMag()
{
	if (EquippedWeapon && EquippedWeapon->IsAmmoEmpty() && CarriedAmmo > 0)
	{
		BaseCharacter->SetIsReloading(true);
	}
}

void UCombatComponent::UnEquipWeapon(class AWeapon* WeaponToUnEquip)
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;

	DisarmedWeapon->SetWeaponState(EWeaponState::EWS_Disarmed);
	const USkeletalMeshSocket* HandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("UnEquippedRifleSocket"));

	DisarmedWeapon = EquippedWeapon;
	BaseCharacter->SetCurrentCharacterState(EWeaponEquippedStatus::ECS_Disarmed);
	FTimerHandle TimerHandle;

	if (HandSocket)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, HandSocket]() {
			HandSocket->AttachActor(DisarmedWeapon, BaseCharacter->GetMesh());
			}, 0.8f, false);
	}
	EquippedWeapon->SetOwner(nullptr);
}

void UCombatComponent::DropWeapon()
{
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	EquippedWeapon->GetWeaponMesh()->DetachFromComponent(DetachRules);
	EquippedWeapon->SetOwner(nullptr);
	EquippedWeapon->BaseOwnerCharacter = nullptr;
	EquippedWeapon->BaseOwnerController = nullptr;
}

bool UCombatComponent::CanFire()
{
	if (EquippedWeapon == nullptr)
		return false;

	return !EquippedWeapon->IsAmmoEmpty() && bCanFire;
}



void UCombatComponent::UpdateCarriedAmmunition()
{
	if (EquippedWeapon == nullptr) return;

	int32 ReloadAmount = AmountToReload();
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetRangedWeaponTypes()))
	{
		CarriedAmmoMap[EquippedWeapon->GetRangedWeaponTypes()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetRangedWeaponTypes()];
	}
	BaseController = BaseController == nullptr ? Cast<ABasePlayerController>(BaseCharacter->Controller) : BaseController;
	if (BaseController)
	{
		BaseController->SetHUDCarriedAmmunation(CarriedAmmo);
	}
	EquippedWeapon->AddAmmunation(-ReloadAmount);
}


void UCombatComponent::ThrowGrenade()
{
	if (CombatState != ECombatState::ECS_Null || EquippedWeapon == nullptr ||
		BaseCharacter->GetThrowGrenade() == false) return;

	if (BaseCharacter)
	{
		CombatState = ECombatState::ECS_ThrowingGrenade;
		AttachActorToLeftHand(EquippedWeapon);
		ShowAttachedGrenade(true);
	}
}

void UCombatComponent::LaunchGrenade()
{
	ShowAttachedGrenade(false);

	if (BaseCharacter && BaseGrenadeClass && !bGrenadeIsLaunched)
	{
		bGrenadeIsLaunched = true;
		const FVector StartingLocation = BaseCharacter->GetAttachedHandGrenade()->GetComponentLocation();
		FVector ToTarget = HitTarget - StartingLocation;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = BaseCharacter;
		SpawnParams.Instigator = BaseCharacter;
		UWorld* World = GetWorld();

		if (World)
		{
			World->SpawnActor<ABaseThrowableGrenades>(
				BaseGrenadeClass,
				StartingLocation,
				ToTarget.Rotation(),
				SpawnParams
			);
		}
		Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);
		UpdateHUDGrenades();
	}
}

void UCombatComponent::ThrowGrenadeFinishedTimer()
{
	if (!BaseCharacter->GetThrowGrenade()) return;
	BaseCharacter->SetThrowningGrenade(false);
	bGrenadeIsLaunched = false;
	CombatState = ECombatState::ECS_Null;
	AttachActorToRightHand(EquippedWeapon);
}

void UCombatComponent::UpdateHUDGrenades()
{
	BaseController = BaseController == nullptr ? Cast<ABasePlayerController>(BaseCharacter->Controller) : BaseController;
	if (BaseController)
	{
		BaseController->SetHUDGrenades(Grenades);
	}
}

void UCombatComponent::ThrowGrenadeFinished()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(
		ThrowGrenadeTimerHandle,
		this,
		&UCombatComponent::ThrowGrenadeFinishedTimer,
		.1f
	);
}


void UCombatComponent::ShowAttachedGrenade(bool bShowGrenade)
{
	if (BaseCharacter && BaseCharacter->GetAttachedHandGrenade())
	{
		BaseCharacter->GetAttachedHandGrenade()->SetVisibility(bShowGrenade);
	}
}

int32 UCombatComponent::AmountToReload()
{
	if (EquippedWeapon == nullptr) return 0;

	int32 RoomInMagazine = EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo();

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetRangedWeaponTypes()))
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetRangedWeaponTypes()];
		int32 Least = FMath::Min(RoomInMagazine, AmountCarried);
		return FMath::Clamp(RoomInMagazine, 0, Least);
	}
	return 0;
}

void UCombatComponent::CombatStateLogic()
{
	switch (CombatState)
	{
	case ECombatState::ECS_Null:
		break;
	case ECombatState::ECS_Reloading:
		break;
	case ECombatState::ECS_ThrowingGrenade:
		break;
	case ECombatState::ECS_MAX:
		break;
	}
}

void UCombatComponent::PickupAmmunition(ERangedWeaponTypes RangeWeaponType, int32 AmmunationAmount)
{
	if (CarriedAmmoMap.Contains(RangeWeaponType))
	{
		CarriedAmmoMap[RangeWeaponType] = FMath::Clamp(CarriedAmmoMap[RangeWeaponType] + AmmunationAmount, 0 , MaxCarriedAmmo);
		UpdateCarriedAmmunition();
	}
	if (EquippedWeapon && EquippedWeapon->IsAmmoEmpty() && EquippedWeapon->GetRangedWeaponTypes() == RangeWeaponType)
	{
		BaseCharacter->SetIsReloading(true);
	}
}

void UCombatComponent::InitializeCarriedAmmo()
{		
	CarriedAmmoMap.Emplace(ERangedWeaponTypes::ERWT_AssaultRifle,		AssaultRiffleInitialAmmunation);
	CarriedAmmoMap.Emplace(ERangedWeaponTypes::ERWT_RocketLauncher,		RocketLauncherInitialAmmunation);
	CarriedAmmoMap.Emplace(ERangedWeaponTypes::ERWT_Pistol,				PistolInitialAmmunation);
	CarriedAmmoMap.Emplace(ERangedWeaponTypes::ERWT_SubmachineGun,		SMGInitialAmmunation);
	CarriedAmmoMap.Emplace(ERangedWeaponTypes::ERWT_Shotgun,			ShotgunInitialAmmunation);
	CarriedAmmoMap.Emplace(ERangedWeaponTypes::ERWT_SniperRifle,		SniperInitialAmmunation);
	CarriedAmmoMap.Emplace(ERangedWeaponTypes::ERWT_GrenadeLauncher,    GrenadeLauncherInitialAmmunation);
}