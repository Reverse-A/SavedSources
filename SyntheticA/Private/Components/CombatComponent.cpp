// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Characters/BaseCharacter.h"
#include "Camera/CameraManager.h"
#include "Components/AttributeComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameModes/CaptureTheFlag/Objects/Flag.h"
#include "HUD/DisplayManager.h"
#include "HUD/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerManager.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Weapon/ScatterWeapons.h"
#include "Weapon/Weapon.h"

//#include "Weapons/WeaponTypes.h"

#pragma region Initialization
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BaseCharacter->HasAuthority())
	{
		InitializeCarriedAmmo();
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
	WeaponSwapEnd();
}


void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, EquippedFlag);
	DOREPLIFETIME(UCombatComponent, SecondaryWeapon);
	DOREPLIFETIME(UCombatComponent, bIsAiming);
	DOREPLIFETIME(UCombatComponent, bIsReloading);
	DOREPLIFETIME(UCombatComponent, bWeaponSwaping);
	DOREPLIFETIME(UCombatComponent, bHoldingTheFlag);

	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
}
#pragma endregion

#pragma region Equip_Weapon
void UCombatComponent::Equip_Weapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr)
	{
		return;
	}
	if (EquippedWeapon != nullptr && SecondaryWeapon == nullptr)
	{
		Equip_SecondaryWeapon(WeaponToEquip);
	}
	else
	{
		Equip_PrimaryWeapon(WeaponToEquip);
	}
}

void UCombatComponent::Equip_PrimaryWeapon(AWeapon* WeaponToEquip)
{
	CheckAndDrop_PrimariyWeapon();

	if (WeaponToEquip == nullptr)
	{
		return;
	}

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_PrimaryEquipped);
	Attach_WeaponToCharactersHand(EquippedWeapon);
	EquippedWeapon->SetOwner(BaseCharacter);
	EquippedWeapon->SetHUDAmmo();
	UpdateCarriedAmmunition();
	PlaySound_EquipWeapon(WeaponToEquip);
}

void UCombatComponent::Equip_SecondaryWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr)
	{
		return;
	}

	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_SecondaryEquipped);
	AttachWeaponToCharactersBackup(WeaponToEquip);
	PlaySound_EquipWeapon(WeaponToEquip);
	SecondaryWeapon->SetOwner(BaseCharacter);
}

void UCombatComponent::CheckAndDrop_PrimariyWeapon()
{
	if (EquippedWeapon != nullptr)
	{
		EquippedWeapon->Drop_Weapon();
	}
}

void UCombatComponent::Attach_WeaponToCharactersHand(AActor* Actor)
{
	bool ReturnTypes =
		BaseCharacter == nullptr				 ||
		BaseCharacter->GetHandsMesh() == nullptr ||
		Actor == nullptr;

	if (ReturnTypes)
	{
		return;
	}

	const USkeletalMeshSocket* HandSocket = BaseCharacter->GetHandsMesh()->GetSocketByName(FName("RH_WeaponSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(Actor, BaseCharacter->GetHandsMesh());
	}
}

void UCombatComponent::AttachWeaponToCharactersBackup(AActor* Actor)
{
	bool ReturnTypes =
		BaseCharacter == nullptr				||
		BaseCharacter->GetMesh() == nullptr ||
		Actor == nullptr;

	if (ReturnTypes) return;

	const USkeletalMeshSocket* BackupSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("SecondaryWeaponSocket"));

	if (BackupSocket)
	{
		BackupSocket->AttachActor(Actor, BaseCharacter->GetMesh());
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && BaseCharacter)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_PrimaryEquipped);
		Attach_WeaponToCharactersHand(EquippedWeapon);
		PlaySound_EquipWeapon(EquippedWeapon);
		EquippedWeapon->SetHUDAmmo();
	}
}

void UCombatComponent::OnRep_SecondaryWeapon()
{
	if (SecondaryWeapon && BaseCharacter)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_SecondaryEquipped);
		AttachWeaponToCharactersBackup(SecondaryWeapon);
		PlaySound_EquipWeapon(SecondaryWeapon);
	}
}

bool UCombatComponent::IsWeaponEquipped()
{
	return EquippedWeapon != nullptr;
}

#pragma endregion

#pragma region Equip Flag
void UCombatComponent::Equip_Flag(AFlag* FlagToEquip)
{
	if (FlagToEquip == nullptr)
	{
		return;
	}
	BaseCharacter->Set_OverlappingFlag(nullptr);

	EquippedFlag = FlagToEquip;
	Attach_FlagToCharacters(EquippedFlag);
	EquippedFlag->SetOwner(BaseCharacter);
	if (bHoldingTheFlag == false)
	{
		bHoldingTheFlag = true;
	}

	if (BaseCharacter->GetTeam() == ETeam::ET_VanguardSovereignty)
	{
		EquippedFlag->SetTeamFlagColour(ETeamFlagColour::ETFC_VanguardSovereigntyFlag);
	}
	else if (BaseCharacter->GetTeam() == ETeam::ET_GenesisPact)
	{
		EquippedFlag->SetTeamFlagColour(ETeamFlagColour::ETFC_GenesisPactFlag);
	}
	else if (BaseCharacter->GetTeam() == ETeam::ET_NoTeam)
	{
		EquippedFlag->SetTeamFlagColour(ETeamFlagColour::ETFC_NoTeamFlag);
	}

	//PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	//if (PlayerControllerManager)
	//{
	//	PlayerControllerManager->SetHUDFlag(false);
	//}

	EquippedFlag->Set_FlagState(EFlagState::EFS_Equiped);
}

void UCombatComponent::OnRep_EquippedFlag()
{
	if (EquippedFlag != nullptr && BaseCharacter)
	{
		Attach_FlagToCharacters(EquippedFlag);

		EquippedFlag->SetOwner(BaseCharacter);

		if (bHoldingTheFlag == false)
		{
			bHoldingTheFlag = true;
		}		
	}

	if (BaseCharacter->GetTeam() == ETeam::ET_VanguardSovereignty)
	{
		EquippedFlag->SetTeamFlagColour(ETeamFlagColour::ETFC_VanguardSovereigntyFlag);
	}
	else if (BaseCharacter->GetTeam() == ETeam::ET_GenesisPact)
	{
		EquippedFlag->SetTeamFlagColour(ETeamFlagColour::ETFC_GenesisPactFlag);
	}
	else
	{
		EquippedFlag->SetTeamFlagColour(ETeamFlagColour::ETFC_NoTeamFlag);
	}

	//if (PlayerControllerManager)
	//{
	//	PlayerControllerManager->SetHUDFlag(false);
	//}
	EquippedFlag->Set_FlagState(EFlagState::EFS_Equiped);
}

void UCombatComponent::Attach_FlagToCharacters(AActor* Flag)
{
	bool ReturnTypes =
		BaseCharacter == nullptr ||
		BaseCharacter->GetMesh() == nullptr ||
		Flag == nullptr;

	if (ReturnTypes) return;

	const USkeletalMeshSocket* FlagSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("FlagSocket"));
	if (FlagSocket)
	{
		FlagSocket->AttachActor(Flag, BaseCharacter->GetMesh());
	}
}

void UCombatComponent::Detach_FlagFromCharacters(AActor* Flag)
{
	bool ReturnTypes =
		BaseCharacter == nullptr ||
		BaseCharacter->GetMesh() == nullptr ||
		Flag == nullptr ||
		Flag->GetAttachParentActor() != BaseCharacter;

	if (ReturnTypes) 
		return;

	Flag->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

#pragma endregion

#pragma region Pick-up

void UCombatComponent::PickupAmmunition(EWeaponType WeaponType, int32 AmmunitionAmount)
{
	if (CarriedAmmoMap.Contains(WeaponType))
	{
		CarriedAmmoMap[WeaponType] = FMath::Clamp(CarriedAmmoMap[WeaponType] + AmmunitionAmount, 0, MaxCarriedAmmunition);
		Update_AmmoValues();
	}

	if (EquippedWeapon && EquippedWeapon->IsEmpty() && EquippedWeapon->Get_WeaponType() == WeaponType)
	{
		Reload();
	}
}
#pragma endregion

#pragma region Ammunition

/* Ammunition */
void UCombatComponent::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol,				StartingPistolAmmunition);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_ScatterWeapons,		StartingScatterWeaponAmmunation);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SubmachineGun,		StartingSubMachhineGunAmmunition);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle,		StartingAssaultRifleAmmunition);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle,		StartingSniperRifleAmmunition);

	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher,		StartingRocketLauncherAmmunition);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_GrenadeLauncher,	StartingGrenadeLauncherAmmunition);
}

void UCombatComponent::UpdateCarriedAmmunition()
{
	if (EquippedWeapon == nullptr) return;

	if (CarriedAmmoMap.Contains(EquippedWeapon->Get_WeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->Get_WeaponType()];
	}

	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}
}
void UCombatComponent::OnRep_CarriedAmmo()
{
	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}

	bool NoScatterWeaponTypesAmmunitionCarried =
		EquippedWeapon != nullptr &&
		EquippedWeapon->Get_WeaponType() == EWeaponType::EWT_ScatterWeapons &&
		IsReloading() &&
		CarriedAmmo == 0;

	if (NoScatterWeaponTypesAmmunitionCarried)
	{
		if (IsReloading())
		{
			SetReload(false);
		}
	}
}

void UCombatComponent::Update_AmmoValues()
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;

	int32 ReloadAmount = AmountToReload();
	if (CarriedAmmoMap.Contains(EquippedWeapon->Get_WeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->Get_WeaponType()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->Get_WeaponType()];
	}

	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}

	EquippedWeapon->AddAmmo(ReloadAmount);
}

void UCombatComponent::Update_AmmoValues_Shotgun()
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;

	if (CarriedAmmoMap.Contains(EquippedWeapon->Get_WeaponType()))
	{
		CarriedAmmoMap[EquippedWeapon->Get_WeaponType()] -= 1;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->Get_WeaponType()];
	}

	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}

	EquippedWeapon->AddAmmo(1);
	if (EquippedWeapon->IsFull() || CarriedAmmo == 0 || BaseCharacter->CanCancelShotgunReloading())
	{
		if (bIsReloading)
			bIsReloading = false;

		BaseCharacter->SetCancelShotgunReloading(false);
	}
}
#pragma endregion

#pragma region Reloading-System
bool UCombatComponent::CanReload()
{
	return (!EquippedWeapon->IsFull() && CarriedAmmo > 0) || (CarriedAmmo == 0 && EquippedWeapon->Get_Ammo() == 0);
}

void UCombatComponent::Reload()
{
	bool ReturnTypes = EquippedWeapon->IsFull() || BaseCharacter == nullptr || EquippedWeapon == nullptr || BaseCharacter->Get_CombatComponent()->Get_HoldingFlag() || bIsReloading ;

	
	if (ReturnTypes) return;

	if (IsAiming() || IsFireButtonActive())
	{
		Set_Aiming(false);
		Set_FireButton(false);
	}

	if (!bIsReloading && BaseCharacter->IsLocallyControlled())
	{
		bIsReloading = true;
	}

	if (CarriedAmmo > 0)
	{
		Server_Reload();
	}

	if (PlayerHUD && PlayerHUD->DisplayManager)
	{
		FString ReloadingText = FString::Printf(TEXT("Reloading..."));
		PlayerHUD->DisplayManager->WeaponCrosshairAmmoAmount->SetText(FText::FromString(ReloadingText));
	}
}

void UCombatComponent::Server_Reload_Implementation()
{
	if (BaseCharacter == nullptr ||	EquippedWeapon == nullptr || BaseCharacter->IsLocallyControlled())	return;

	if (!bIsReloading)	
		bIsReloading = true;
}

void UCombatComponent::FinishReload()
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;

	if (bIsReloading)
		bIsReloading = false;

	if (BaseCharacter->HasAuthority())
	{
		Update_AmmoValues();
	}

	if (bFireButtonActive)
	{
		Fire();
	}
}

void UCombatComponent::ShotgunShellReload()
{
	bool ReturnTypes = 	BaseCharacter  == nullptr  ||	EquippedWeapon == nullptr  ||
		EquippedWeapon->Get_Ammo() == EquippedWeapon->Get_MagCapacity() ||
		Get_HoldingFlag();

	if (ReturnTypes)
	{
		if (bIsReloading)
			bIsReloading = false;
		return;
	}

	if (BaseCharacter->HasAuthority())
	{
		Update_AmmoValues_Shotgun();
	}
}

int32 UCombatComponent::AmountToReload()
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return 0;

	int32 RoomInMagazine = EquippedWeapon->Get_MagCapacity() - EquippedWeapon->Get_Ammo();

	if (CarriedAmmoMap.Contains(EquippedWeapon->Get_WeaponType()))
	{
		int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->Get_WeaponType()];
		int32 Least = FMath::Min(RoomInMagazine, AmountCarried);
		return FMath::Clamp(RoomInMagazine, 0, Least);
	}
	return 0;
}
#pragma endregion


#pragma region Weapon-Swap
void UCombatComponent::DoSwap()
{
	if (BaseCharacter == nullptr) return;

	if (BaseCharacter->HasAuthority())
	{
		bWeaponSwaping = true;
		CanSwap		   = false;
	}
	else
	{
		Server_SetWeaponSwap(true);
	}
}

bool UCombatComponent::CanSwapWeapons()
{
	if (IsReloading() || IsSwapingWeapon() || IsFireButtonActive() || CanSwap == false) return false;
	
	return (EquippedWeapon != nullptr && SecondaryWeapon != nullptr);
}


void UCombatComponent::Server_SetWeaponSwap_Implementation(bool InsertValue)
{
	if (BaseCharacter == nullptr) return;

	bWeaponSwaping = InsertValue;
}

void UCombatComponent::WeaponSwapEnd()
{
	if (bWeaponSwaping)
	{
		SwapTimer += GetWorld()->GetDeltaSeconds();

		float SwapThreshold = 0.65f;
		if (SwapTimer >= SwapThreshold)
		{
			if (BaseCharacter->HasAuthority())
			{		
				AWeapon* TemporaryWeapon = EquippedWeapon;
				EquippedWeapon = SecondaryWeapon;
				SecondaryWeapon = TemporaryWeapon;

				EquippedWeapon->SetWeaponState(EWeaponState::EWS_PrimaryEquipped);
				Attach_WeaponToCharactersHand(EquippedWeapon);
				EquippedWeapon->SetHUDAmmo();
				UpdateCarriedAmmunition();
				PlaySound_EquipWeapon(EquippedWeapon);

				SecondaryWeapon->SetWeaponState(EWeaponState::EWS_PrimaryEquipped);
				AttachWeaponToCharactersBackup(SecondaryWeapon);
				bWeaponSwaping = false;
			}
			else
			{
				Server_SetWeaponSwap(false);
			}
		}
	}
	else
	{
		if (SwapTimer != 0.0f)
			SwapTimer = 0.0f;
	}

	CanSwapTimer += GetWorld()->GetDeltaSeconds();
	float LastThreshold = 1.5f;
	if (CanSwapTimer >= LastThreshold && CanSwap == false)
	{
		CanSwap = true;
	}
}
#pragma endregion

#pragma region Aim-System


void UCombatComponent::OnRep_Aiming()
{
	if (BaseCharacter && BaseCharacter->IsLocallyControlled())
	{
		bIsAiming = bAimButtonPressed;
	}
}

void UCombatComponent::Set_Aiming(bool SetValue)
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr) return;

	bIsAiming = SetValue;

	Server_SetAiming(bIsAiming);

	if (BaseCharacter->IsLocallyControlled() && EquippedWeapon->Get_WeaponType() == EWeaponType::EWT_SniperRifle)
	{
		BaseCharacter->ShowSniperScopeWidget(SetValue);
	}

	if (BaseCharacter->IsLocallyControlled())
	{
		bAimButtonPressed = SetValue;
	}
}

void UCombatComponent::Server_SetAiming_Implementation(bool SetValue)
{
	bIsAiming = SetValue;
}
#pragma endregion
#pragma region Fire-System

bool UCombatComponent::CanFire()
{
	if (EquippedWeapon == nullptr || IsReloading()) return false;

	if (EquippedWeapon && EquippedWeapon->Get_WeaponType() == EWeaponType::EWT_ScatterWeapons && bCanFire) return true;
	return !EquippedWeapon->IsEmpty() && bCanFire && !IsReloading();
}

void UCombatComponent::Set_FireButton(bool bActive)
{
	if (BaseCharacter == nullptr || EquippedWeapon == nullptr || BaseCharacter->GetPlayerIsEliminated() || IsReloading()) return;
	bFireButtonActive = bActive;
	if (bFireButtonActive)
	{
		Fire();
	}
}

void UCombatComponent::Fire()
{
	if (CanFire())
	{
		bCanFire = false;
		if (EquippedWeapon)
		{
			switch (EquippedWeapon->GetFireType())
			{
				case EFireType::EFT_Projectile:
					Fire_ProjectileWeapon();
						break;
				case EFireType::EFT_HitScan:
					Fire_HitScanWeapon();
						break;
				case EFireType::EFT_ScatterType:
					Fire_ScatterTypeWeapon();
						break;
			}
		}

		if (BaseCharacter)
		{
			if (BaseCharacter->GetCameraManager())
			{
				BaseCharacter->GetCameraManager()->SetCrosshairShootingFactor(2.25);
			}
			if (BaseCharacter->GetHealth() >= 0.0f)
			{
				StartFireTimer();
			}
		}
	}
}

void UCombatComponent::Fire_ProjectileWeapon()
{
	if (EquippedWeapon && BaseCharacter)
	{
		HitTarget = EquippedWeapon->GetIsScatterWeaponType() ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		if (!BaseCharacter->HasAuthority())
		{
			Local_Fire(HitTarget);
		}
		Server_Fire(HitTarget, EquippedWeapon->GetFireDelay());
	}
}

void UCombatComponent::Fire_HitScanWeapon()
{
	if (EquippedWeapon && BaseCharacter)
	{
		HitTarget = EquippedWeapon->GetIsScatterWeaponType() ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;

		if (!BaseCharacter->HasAuthority())
		{
			Local_Fire(HitTarget);
		}
		Server_Fire(HitTarget, EquippedWeapon->GetFireDelay());
	}
}

void UCombatComponent::Fire_ScatterTypeWeapon()
{
	AScatterWeapons* ScatterWeapons = Cast<AScatterWeapons>(EquippedWeapon);

	if (ScatterWeapons && BaseCharacter)
	{
		TArray<FVector_NetQuantize> HitTargets;
		ScatterWeapons->ScatterTypeWeaponsTraceEndWithScatter(HitTarget, HitTargets);
		
		if (!BaseCharacter->HasAuthority())
		{
			Local_ScatterTypeFire(HitTargets);
		}
		Server_ScatterTypeFire(HitTargets, EquippedWeapon->GetFireDelay());
	}	
}

/* Automatic Fire System */
void UCombatComponent::StartFireTimer()
{
	bool ReturnTypes = EquippedWeapon == nullptr || BaseCharacter == nullptr;
	if (ReturnTypes) return;

	BaseCharacter->GetWorldTimerManager().SetTimer
	(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		EquippedWeapon->GetFireDelay()
	);
}

void UCombatComponent::FireTimerFinished()
{
	if (EquippedWeapon == nullptr) return;

	bCanFire = true;
	if (bFireButtonActive && EquippedWeapon->GetIsAutomaticWeapon())
	{
		Fire();
	}
}

void UCombatComponent::Local_Fire(const FVector_NetQuantize& TraceHitTarget)
{
	if (EquippedWeapon == nullptr || BaseCharacter == nullptr) return;

	PlayFireMontageAndCameraShake();
	EquippedWeapon->Fire(TraceHitTarget);
}

void UCombatComponent::Local_ScatterTypeFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	AScatterWeapons* ScatterWeapons = Cast<AScatterWeapons>(EquippedWeapon);
	if (ScatterWeapons == nullptr || BaseCharacter == nullptr) return;
	ScatterWeapons->FireScatterWeapons(TraceHitTargets);
	PlayFireMontageAndCameraShake();
}

void UCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	Multicast_Fire(TraceHitTarget);
}

bool UCombatComponent::Server_Fire_Validate(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	if (EquippedWeapon)
	{
		bool bNearlyEqual = FMath::IsNearlyEqual(EquippedWeapon->GetFireDelay(), FireDelay, 0.001f);
		return bNearlyEqual;
	}
	return true;
}

void UCombatComponent::Multicast_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (BaseCharacter && BaseCharacter->IsLocallyControlled() && !BaseCharacter->HasAuthority()) return;
	Local_Fire(TraceHitTarget);
}

void UCombatComponent::Server_ScatterTypeFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	Multicast_ScatterTypeFire(TraceHitTargets);
}

bool UCombatComponent::Server_ScatterTypeFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	if (EquippedWeapon)
	{
		bool bNearlyEqual = FMath::IsNearlyEqual(EquippedWeapon->GetFireDelay(), FireDelay, 0.001f);
		return bNearlyEqual;
	}
	return true;
}

void UCombatComponent::Multicast_ScatterTypeFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if (BaseCharacter && BaseCharacter->IsLocallyControlled() && !BaseCharacter->HasAuthority()) return;
	Local_ScatterTypeFire(TraceHitTargets);
}

void UCombatComponent::PlayFireMontageAndCameraShake()
{
	if (EquippedWeapon == nullptr || BaseCharacter == nullptr) return;

	UAnimInstance* AnimInstance = BaseCharacter->GetMesh()->GetAnimInstance();

	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = BaseCharacter->IsAiming() ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}

	/*CameraShake*/
	if (FireCameraShake)
	{
		BaseCharacter->GetCameraManager()->PlayCameraShake(FireCameraShake);
	}
}

#pragma endregion
#pragma region Crosshair&Trace-System
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
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.0f);
		}
		FVector End = Start + CrosshairWorldDirection * TRACE_MAX_LENGHT;

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
				if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())
				{
					bIsActorTargetedByCrosshair = true;
					PlayerHUD->SetCrosshairColor(FLinearColor::Red);
				}
				else
				{
					bIsActorTargetedByCrosshair = false;
					PlayerHUD->SetCrosshairColor(FLinearColor::White);
				}
			}
		}
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
	}
}
#pragma endregion
#pragma region Hit Reaction
void UCombatComponent::PlayMontage_HitReaction()
{
	if (BaseCharacter == nullptr) return;

	UAnimInstance* AnimInstance = BaseCharacter->GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactionMontage)
	{
		AnimInstance->Montage_Play(HitReactionMontage);
		FString Selection;

		if (IsWeaponEquipped())
		{
			Selection = "FromFront";
		}
		else
		{
			Selection = "FromFrontUA";
		}

		FName SectionName(Selection);

		AnimInstance->Montage_JumpToSection(SectionName);
	}

	if (BaseCharacter->GetCameraManager())
	{
		if (FireCameraShake)
		{
			BaseCharacter->GetCameraManager()->PlayCameraShake(FireCameraShake);
		}
	}
}
#pragma endregion
#pragma region ShieldBreak
void UCombatComponent::ShieldBreak()
{
	if (BaseCharacter == nullptr || ShieldBreakParticle == nullptr || ShieldBreakSound == nullptr) return;
	Server_PlayShieldBreak();
}

void UCombatComponent::Server_PlayShieldBreak_Implementation()
{
	Multicast_PlayShieldBreak();
}
void UCombatComponent::Multicast_PlayShieldBreak_Implementation()
{
	PlayShieldBreakEffect();
	PlaySound_ShieldBreak();
}
void UCombatComponent::PlayShieldBreakEffect()
{
	if (ShieldBreakParticle)
	{
		FVector CharacterLocation = BaseCharacter->GetActorLocation();
		CharacterLocation.Z += 35.0f;
		UGameplayStatics::SpawnEmitterAtLocation
		(

			GetWorld(),
			ShieldBreakParticle,
			CharacterLocation
		);
	}
}

#pragma endregion

#pragma region SoundPlay
void UCombatComponent::PlaySound_ShieldBreak()
{
	if (ShieldBreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			GetWorld(),
			ShieldBreakSound,
			BaseCharacter->GetActorLocation()
		);
	}
}

void UCombatComponent::PlaySound_EquipWeapon(AWeapon* Weapon)
{
	if (BaseCharacter && Weapon && Weapon->GetEquipSound())
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			Weapon->GetEquipSound(),
			BaseCharacter->GetActorLocation()
		);
	}
}

#pragma endregion