// ©2023 Reverse - A Company. All rights reserved.


#include "Components/CombatComponent.h"

#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

#include "Character/BaseCharacter.h"
#include "Camera/CameraSettings.h"
#include "Components/CombatComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "HUD/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerManager.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Weapons/Weapon.h"
#include "Weapons/WeaponTypes.h"

#pragma region | Constructors |
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
	DOREPLIFETIME(UCombatComponent, CombatTypes);
	DOREPLIFETIME(UCombatComponent, bIsReloading);
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterMesh = BaseCharacter->GetHeadMesh();

	if (BaseCharacter)
	{
		EnhancedMovement = BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();

		if (BaseCharacter->HasAuthority())
		{
			InitializeCarriedAmmo();
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BaseCharacter && BaseCharacter->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	FVector CrosshairWorldPosition;
	FVector	CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
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
			Start += CrosshairWorldDirection * (DistanceToCharacter);
		}

		FVector End = Start + CrosshairWorldDirection * MAX_TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			BaseCharacter->GetIgnoreCharacterParams()
		);

		PlayerControllerManager = Cast<APlayerControllerManager>(BaseCharacter->Controller);
		if (PlayerControllerManager)
		{
			PlayerHUD = Cast<APlayerHUD>(PlayerControllerManager->GetHUD());

			if (PlayerHUD)
			{
				if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UCrosshairInterface>())
				{
					PlayerHUD->SetCrosshairColor(FLinearColor::Red);
				}
				else
				{
					PlayerHUD->SetCrosshairColor(FLinearColor::White);
				}
			}
			else
			{
				LOG_WARNING("PlayerHUD NULL");
			}
		}

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
	}
}

#pragma endregion

#pragma region | Equip System |

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (BaseCharacter == nullptr || WeaponToEquip == nullptr) return;
	if (EquippedWeapon)
	{
		EquippedWeapon->DropWeapon();
	}
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = BaseCharacter->GetHandsMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, BaseCharacter->GetHandsMesh());
	}
	EquippedWeapon->SetOwner(BaseCharacter);
	EquippedWeapon->SetHUDAmmo();

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}

	if (EquippedWeapon->GetEquipSound())
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			EquippedWeapon->GetEquipSound(),
			BaseCharacter->GetActorLocation()
		);
	}

	if (EquippedWeapon->IsEmpty())
	{
		Reload();
	}

	// TODO: Disable Player'S HUD overlay "equip weapon" Text
}

void UCombatComponent::Server_EquipButtonPressed_Implementation()
{
	EquipWeapon(BaseCharacter->OverlappingWeapon);
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && BaseCharacter)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		const USkeletalMeshSocket* HandSocket = BaseCharacter->GetHandsMesh()->GetSocketByName(FName("RightHandSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(EquippedWeapon, BaseCharacter->GetHandsMesh());
		}

		if (EquippedWeapon->GetEquipSound())
		{
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				EquippedWeapon->GetEquipSound(),
				BaseCharacter->GetActorLocation()
			);
		}
	}
}

bool UCombatComponent::IsWeaponEquipped()
{
	return EquippedWeapon != nullptr;
}
#pragma endregion

#pragma region | Fire System |
void UCombatComponent::SetFireButtonTrigger(bool bTriggered)
{
	bFireButtonPressed = bTriggered;
	if (bFireButtonPressed)
	{
		Fire();
	}
}

bool UCombatComponent::CanFire()
{
	if (EquippedWeapon == nullptr) return false;
	return !EquippedWeapon->IsEmpty() &&
			bCanFire &&
			CombatTypes == ECombatTypes::ECT_Unoccupied ;
}

void UCombatComponent::Fire()
{
	if (CanFire())
	{
		bCanFire = false;
		Server_Fire(HitTarget);
		if (EquippedWeapon)
		{
			BaseCharacter->GetCameraSettingsComponent()->SetCrosshairShootingFactor(0.75f);
		}
		StartFireTimer();
	}
}

void UCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || BaseCharacter == nullptr) return;

	BaseCharacter->GetWorldTimerManager().SetTimer
	(
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
	if (bFireButtonPressed && EquippedWeapon->bAutomaticWeapon)
	{
		Fire();
	}
	if (EquippedWeapon->IsEmpty())
	{
		Reload();
	}
}

void UCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	Multicast_Fire(TraceHitTarget);
}

void UCombatComponent::Multicast_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon == nullptr) return;

	if (BaseCharacter && CombatTypes == ECombatTypes::ECT_Unoccupied)
	{
		PlayFireMontage(bAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}
#pragma endregion

#pragma region | Weapon Reload |
uint32 UCombatComponent::AmountToReload()
{
	if (EquippedWeapon == nullptr) return 0;
	int32 RoomInMagazine = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();

	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		int32 Least = FMath::Min(RoomInMagazine, AmountCarried);
		return FMath::Clamp(RoomInMagazine, 0, Least);
	}
	return 0;
}

void UCombatComponent::Reload()
{
	if (CarriedAmmo > 0 && CombatTypes != ECombatTypes::ECT_Reloading)
	{
		Server_Reload();
	}
}

void UCombatComponent::Server_Reload_Implementation()
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;
	bIsReloading = true;
	CombatTypes = ECombatTypes::ECT_Reloading;
	//HandleReload();
}

void UCombatComponent::OnRep_CombatTypes()
{
	switch (CombatTypes)
	{
	case ECombatTypes::ECT_Reloading:
		bIsReloading = true;
		//HandleReload();
		break;
	case ECombatTypes::ECT_Unoccupied:
		if (IsFireButtonPressed())
		{
			Fire();
		}
		break;
	}
}

void UCombatComponent::HandleReload()
{
	PlayReloadMontage();
}

void UCombatComponent::PlayReloadMontage()
{
	if (EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = BaseCharacter->GetHeadMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;
		switch (EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_Pistol:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_SMG:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_Shotgun:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_AssaultRifle:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_SniperRifle:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_GrenadeLauncher:
			SectionName = FName("Grenade");
			break;
		case EWeaponType::EWT_RocketLauncher:
			SectionName = FName("Rifle");
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void UCombatComponent::FinishReloading()
{
	if (BaseCharacter == nullptr) return;
	if (BaseCharacter->HasAuthority())
	{
		CombatTypes = ECombatTypes::ECT_Unoccupied;
		bIsReloading = false;
		UpdateAmmoValues();
	}
	if (IsFireButtonPressed())
	{
		Fire();
	}
}

void UCombatComponent::UpdateAmmoValues()
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;
	 
	int32 ReloadAmount = AmountToReload();
	if (CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetWeaponType()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
	}

	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}
	EquippedWeapon->AddAmmo(-ReloadAmount);
}

#pragma endregion

#pragma region | Aiming |
void UCombatComponent::SetAiming(bool bIsAiming)
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;
	bAiming = bIsAiming;
	Server_SetAiming(bIsAiming);

	if (BaseCharacter->IsLocallyControlled() && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		BaseCharacter->ShowSniperScopeWidget(bIsAiming);
	}
}

void UCombatComponent::Server_SetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}
#pragma endregion

#pragma region | Animation |
void UCombatComponent::PlayFireMontage(bool bIsAiming)
{
	if (EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = BaseCharacter->GetHeadMesh()->GetAnimInstance();

	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
	else
	{
		LOG_WARNING("ANIMINSTANCE NULL");
	}
}

void UCombatComponent::PlayHitReactionMontage()
{
	if (EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = BaseCharacter->GetHeadMesh()->GetAnimInstance();

	if (AnimInstance && HitReactionMontage)
	{
		AnimInstance->Montage_Play(HitReactionMontage);
		FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
	else
	{
		LOG_WARNING("ANIMINSTANCE NULL");
	}
}

void UCombatComponent::OnRep_CarriedAmmo()
{
	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}
}

void UCombatComponent::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol,			 StartingPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Shotgun,		 StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SMG,			 StartingSMGAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle,	 StartingAssaultRifleAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle,	 StartingSniperRileAmmo);	
	CarriedAmmoMap.Emplace(EWeaponType::EWT_GrenadeLauncher, StartingGrenadeLauncherAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher,  StartingRocketLauncherAmmo);
}

#pragma endregion