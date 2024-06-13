// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|


#include "Components/CombatComponent.h"

#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

#include "Characters/BaseCharacter.h"
#include "Camera/CameraConfiguration.h"
#include "Components/CombatComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "HUD/PlayerHUD.h"
#include "HUD/CrosshairInterface.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerManager.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponTypes.h"

#pragma region Constructor
UCombatComponent::UCombatComponent()
{

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
	if (BaseCharacter)
	{
		EnhancedMovementComponent = BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();

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
#pragma endregion

#pragma region Weapon
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (BaseCharacter == nullptr || WeaponToEquip == nullptr) return;
	if (EquippedWeapon)
	{
		EquippedWeapon->DropWeapon();
	}
	LOG_WARNING(" EquipWeapon");

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

	if (EquippedWeapon != nullptr)
	{
		LOG_WARNING("EquippedWeapon - nullptr!=")
	}
	else
	{
		LOG_WARNING("EquippedWeapon - nullptr")
	}
	// TODO: Disable Player'S HUD overlay "equip weapon" Text
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

void UCombatComponent::Server_EquipButtonPressed_Implementation()
{
	EquipWeapon(OverlappingWeapon);
}

void UCombatComponent::SetOverlappingWeapon(AWeapon* Weapon)
{
	OverlappingWeapon = Weapon;
}

void UCombatComponent::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->SetDisplayEquipWeapon(true);
	}
	if (LastWeapon)
	{
		LastWeapon->SetDisplayEquipWeapon(false);
	}
}

AWeapon* UCombatComponent::GetEquippedWeapon()
{
	return EquippedWeapon;
}
#pragma endregion

#pragma region Fire System
bool UCombatComponent::CanFire()
{
	if (EquippedWeapon == nullptr) return false;

	return !EquippedWeapon->IsEmpty() &&
			bCanFire;
}

void UCombatComponent::Fire()
{

}
void UCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{

}
void UCombatComponent::Multicast_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
}

void UCombatComponent::SetFireButtonTrigger(bool bTriggered)
{

}



#pragma endregion

#pragma region Reload
void UCombatComponent::Reload()
{
	if (CarriedAmmo > 0 )
	{
		Server_Reload();
	}
}
void UCombatComponent::Server_Reload_Implementation()
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;
	bIsReloading = true;
}
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
#pragma endregion

#pragma region Ammunation

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
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol, StartingPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Shotgun, StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SMG, StartingSMGAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, StartingAssaultRifleAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle, StartingSniperRileAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_GrenadeLauncher, StartingGrenadeLauncherAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, StartingRocketLauncherAmmo);
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
#pragma region Hit Reaction
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
#pragma endregion
#pragma region Aim

void UCombatComponent::SetAiming(bool bIsAiming)
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;
	bAiming = bIsAiming;
	Server_SetAiming(bIsAiming);

	if (BaseCharacter->IsLocallyControlled() && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
	{
		ShowSniperScopeWidget(bIsAiming);
	}
}


void UCombatComponent::Server_SetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void UCombatComponent::ShowSniperScopeWidget(bool bShowScope)
{

}

#pragma endregion

#pragma region HUD
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

void UCombatComponent::OnRep_CombatTypes()
{
	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}
}
#pragma endregion