// // @2023 All rights reversed by Reverse-Alpha Studios


#include "ArsenalComponent.h"
 
#include "Ascend/Character/BaseCharacter.h"
#include "Ascend/Character/CharacterDefaultAnimInstance.h"
#include "Ascend/Character/ShortDebugMacros.h"

#include "Ascend/Weapon/BaseWeapon.h"
#include "Ascend/Weapon/MeleeWeapon.h"
#include "Ascend/Weapon/RangedWeapon.h"

#include "Engine/SkeletalMeshSocket.h"


UArsenalComponent::UArsenalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UArsenalComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeCarriedAmmunition();
}

void UArsenalComponent::SelectWeaponTypes(ABaseWeapon* InsertOverlapWeapon)
{
	if (InsertOverlapWeapon == nullptr) return;

	// Determine the type of the overlapping weapon and handle it accordingly
	if (InsertOverlapWeapon->GetWeaponType() == EWeaponType::EWT_Melee)
	{
		// If the overlapping weapon is of melee type, handle it accordingly
		MeleeWeapon = Cast<AMeleeWeapon>(InsertOverlapWeapon);
		MeleeWeapons.Add(MeleeWeapon);
		InventoryWeapons.Add(MeleeWeapon);

		EquipMeleeWeaponOnGround(MeleeWeapon);
	}
	else if (InsertOverlapWeapon->GetWeaponType() == EWeaponType::EWT_Ranged)
	{
		// If the overlapping weapon is of ranged type, handle it accordingly
		RangedWeapon = Cast<ARangedWeapon>(InsertOverlapWeapon);
		RangedWeapons.Add(RangedWeapon);
		InventoryWeapons.Add(RangedWeapon);

		EquipRangeWeaponOnGround(RangedWeapon);
	}
}

void UArsenalComponent::EquipMeleeWeaponOnGround(AMeleeWeapon* InsertMeleeWeapon)
{
	if (InsertMeleeWeapon == nullptr) return;

	// Equip the melee weapon on the ground based on the current equipment state
	if (PrimaryEquipWeapon == nullptr)
	{	
		// If neither primary nor secondary weapon is equipped, equip this as the primary weapon
		AttachWeaponToActor(InsertMeleeWeapon);
		PrimaryEquipWeapon = InsertMeleeWeapon;
		BaseCharacter->GetCharacterAnimInstance()->SetWeaponType(EWeaponType::EWT_Melee);
		InsertMeleeWeapon->SetOwner(BaseCharacter->GetOwner());
		InsertMeleeWeapon->SetInstigator(BaseCharacter->GetInstigator());		
	}
	else
	{	
		if (InsertMeleeWeapon != nullptr)
		{
			InventoryWeapons.Add(InsertMeleeWeapon);
			for (auto Weapon : InventoryWeapons)
			{
				if (Weapon)
				{
					UE_LOG(LogTemp, Warning, TEXT("Added weapon: %s"), *Weapon->GetName());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Attempted to add a null weapon to InventoryWeapons!"));
				}
			} 
			InsertMeleeWeapon->Destroy();
		}
	}
}

void UArsenalComponent::EquipRangeWeaponOnGround(ARangedWeapon* InsertRangeWeapon)
{
	// Check if the weapon to be inserted is valid
	if (InsertRangeWeapon == nullptr) return;

	// Equip the range weapon on the ground based on the current equipment state
	if (PrimaryEquipWeapon == nullptr)
	{
		// If neither primary nor secondary weapon is equipped, equip this as the primary ranged weapon
		AttachWeaponToActor(InsertRangeWeapon);
		PrimaryEquipWeapon   = InsertRangeWeapon;
		BaseCharacter->GetCharacterAnimInstance()->SetWeaponType(EWeaponType::EWT_Ranged);
		InsertRangeWeapon->SetOwner(BaseCharacter->GetOwner());
		InsertRangeWeapon->SetInstigator(BaseCharacter->GetInstigator());
	}
	else
	{
		if (InsertRangeWeapon != nullptr)
		{
			InventoryWeapons.Add(InsertRangeWeapon);
			for (auto Weapon : InventoryWeapons)
			{
				if (Weapon)
				{
					UE_LOG(LogTemp, Warning, TEXT("Added weapon: %s"), *Weapon->GetName());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Attempted to add a null weapon to InventoryWeapons!"));
				}
			}
			InsertRangeWeapon->Destroy();
		}
	}
}

ERangedWeaponType UArsenalComponent::GetRangeWeaponType()
{
	if (RangedWeapon == nullptr) return ERangedWeaponType();

	return RangedWeapon->GetRangedWeaponProperties().GetRangedWeaponType();
}

void UArsenalComponent::AttachWeaponToActor(ABaseWeapon* Weapon)
{
	if (Weapon == nullptr) return;

	if (Weapon->GetWeaponType() == EWeaponType::EWT_Melee)
	{
		Weapon->SetWeaponState(EWeaponState::EWS_Equipped);
		AMeleeWeapon* WeaponToAttach = Cast<AMeleeWeapon>(Weapon);
		EMeleeWeaponType CheckWeaponType = WeaponToAttach->GetMeleeWeaponProperties().GetMeleeWeaponType();

		const USkeletalMeshSocket* WeaponHandSocket = nullptr;

		switch (CheckWeaponType)
		{
			case EMeleeWeaponType::Katana:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::Axe:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::Club:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::Dagger:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::HiddenBlade:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::Mace:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::Shield:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::Spear:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::Sword:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
			case EMeleeWeaponType::Whip:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_KatanaSocket"));
				break;
		}
		if (WeaponHandSocket)
		{
			WeaponHandSocket->AttachActor(WeaponToAttach, BaseCharacter->GetMesh());
		}
		WeaponToAttach->SetWeaponState(EWeaponState::EWS_Equipped);
	}
	else if (Weapon->GetWeaponType() == EWeaponType::EWT_Ranged)
	{
		ARangedWeapon* WeaponToAttach = Cast<ARangedWeapon>(Weapon);
		ERangedWeaponType CheckRangedWeaponType = WeaponToAttach->GetRangedWeaponProperties().GetRangedWeaponType();
		BaseCharacter->GetCharacterAnimInstance()->SetWeaponType(EWeaponType::EWT_Ranged);
		BaseCharacter->GetCharacterAnimInstance()->SetRangedWeaponType(WeaponToAttach->GetRangedWeaponProperties().GetRangedWeaponType());
		WeaponToAttach->SetWeaponState(EWeaponState::EWS_Equipped);

		const USkeletalMeshSocket* WeaponHandSocket = nullptr;
		switch (CheckRangedWeaponType)
		{
			case ERangedWeaponType::ERT_Pistol:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_PistolSocket"));
				LW("ERT_Pistol")
				break;
			case ERangedWeaponType::ERT_Rifle:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_AssaultRifleSocket"));
				LW("ERT_Rifle")
				break;
			case ERangedWeaponType::ERT_Shotgun:
				WeaponHandSocket = BaseCharacter->GetMesh()->GetSocketByName(FName("RH_ShotgunSocket"));
				LW("ERT_Shotgun")
				break;
		}
		if (WeaponHandSocket)
		{
			WeaponHandSocket->AttachActor(WeaponToAttach, BaseCharacter->GetMesh());
		}

	}
}

void UArsenalComponent::BuyWeapon(TSubclassOf<ABaseWeapon> InsertSubclassWeapon, USkeletalMeshComponent* SkeletalMesh)
{
	if (SkeletalMesh == nullptr || InsertSubclassWeapon == nullptr) return;

	const USkeletalMeshSocket* WeaponHandSocket = SkeletalMesh->GetSocketByName(FName("RightHandWeaponSocket"));

	if (PrimaryEquipWeapon == nullptr)
	{
		ABaseWeapon* PurchasedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(InsertSubclassWeapon, FVector::ZeroVector, FRotator::ZeroRotator);

		PrimaryEquipWeapon = PurchasedWeapon;
		PrimaryEquipWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

		//AttachWeaponToActor(PrimaryEquipWeapon, BaseCharacter->GetMesh());
		//CharacterAnimInstance->SetWeaponType(EWeaponType::EWT_Melee);
	}
}

void UArsenalComponent::InitializeCarriedAmmunition()
{
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Bow,			InitialBowAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Crossbow,		InitialCrossBowAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Cannon,			InitialCannonAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Heavymachine,	InitialHeavyMachineAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Launcher,		InitialLauncherAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Pistol,			InitialPistolAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Rifle,			InitialRifleAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Submachine,		InitialSubMachineGunAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Shotgun,		InitialShotgunWeaponAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Sniper,			InitialSniperRifleAmmunition);
	CarriedAmmunitionMap.Emplace(ERangedWeaponType::ERT_Throwing,		InitialThrowingAmmunition);

#define DEBUG_INITIALIZE_CARRIED_AMMUNITION 0
#if DEBUG_INITIALIZE_CARRIED_AMMUNITION == 1

	for (auto& Pair : CarriedAmmunitionMap)
	{
		const ERangedWeaponType WeaponType = Pair.Key;
		const int32 AmmunitionCount = Pair.Value;

		FString WeaponTypeName;
		switch (WeaponType)
		{
			case ERangedWeaponType::ERT_Bow:
				UE_LOG(LogTemp, Warning, TEXT("Bow Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Bow]);
				break;
			case ERangedWeaponType::ERT_Crossbow:
				UE_LOG(LogTemp, Warning, TEXT("Crossbow Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Crossbow]);
				break;
			case ERangedWeaponType::ERT_Cannon:
				UE_LOG(LogTemp, Warning, TEXT("Cannon Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Cannon]);
				break;
			case ERangedWeaponType::ERT_Heavymachine:
				UE_LOG(LogTemp, Warning, TEXT("Heavy Machine Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Heavymachine]);
				break;
			case ERangedWeaponType::ERT_Launcher:
				UE_LOG(LogTemp, Warning, TEXT("Launcher Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Launcher]);
				break;
			case ERangedWeaponType::ERT_Pistol:
				UE_LOG(LogTemp, Warning, TEXT("Pistol Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Pistol]);
				break;
			case ERangedWeaponType::ERT_Rifle:
				UE_LOG(LogTemp, Warning, TEXT("Rifle Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Rifle]);
				break;
			case ERangedWeaponType::ERT_Submachine:
				UE_LOG(LogTemp, Warning, TEXT("Submachine Gun Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Submachine]);
				break;
			case ERangedWeaponType::ERT_Shotgun:
				UE_LOG(LogTemp, Warning, TEXT("Shotgun Weapon Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Shotgun]);
				break;
			case ERangedWeaponType::ERT_Sniper:
				UE_LOG(LogTemp, Warning, TEXT("Sniper Rifle Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Sniper]);
				break;
			case ERangedWeaponType::ERT_Throwing:
				UE_LOG(LogTemp, Warning, TEXT("Throwing Ammunition Count: %d"), CarriedAmmunitionMap[ERangedWeaponType::ERT_Throwing]);
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Unknown Weapon Type"));
		}
	}
#endif

}

void UArsenalComponent::UpdateCarriedAmmunition()
{
	if (RangedWeapon == nullptr) return;

	if (CarriedAmmunitionMap.Contains(RangedWeapon->GetRangedWeaponType()))
	{
		CarriedAmmunition = CarriedAmmunitionMap[RangedWeapon->GetRangedWeaponType()];
	}

	//PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	//if (PlayerControllerManager)
	//{
	//	PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	//}
}

void UArsenalComponent::Update_AmmunitionValue()
{
	if (BaseCharacter == nullptr || PrimaryEquipWeapon == nullptr) return;

	if (CarriedAmmunitionMap.Contains(RangedWeapon->GetRangedWeaponType()))
	{
		CarriedAmmunitionMap[RangedWeapon->GetRangedWeaponType()] -= 1;
		CarriedAmmunition = CarriedAmmunitionMap[RangedWeapon->GetRangedWeaponType()];
	}

	//PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	//if (PlayerControllerManager)
	//{
	//	PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	//}

	//EquippedWeapon->AddAmmo(ReloadAmount);
}

void UArsenalComponent::Update_AmmunitionValues_Shotgun()
{
	if (BaseCharacter == nullptr || PrimaryEquipWeapon == nullptr || RangedWeapon == nullptr) return;

	if (CarriedAmmunitionMap.Contains(RangedWeapon->GetRangedWeaponType()))
	{
		CarriedAmmunitionMap[RangedWeapon->GetRangedWeaponType()] -= 1;
		CarriedAmmunition = CarriedAmmunitionMap[RangedWeapon->GetRangedWeaponType()];
	}

	/*PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDCarriedAmmo(CarriedAmmo);
	}*/

	//EquippedWeapon->AddAmmo(1);
	//if (EquippedWeapon->IsFull() || CarriedAmmo == 0 || BaseCharacter->CanCancelShotgunReloading())
	//{
	//	if (bIsReloading)
	//		bIsReloading = false;

	//	BaseCharacter->SetCancelShotgunReloading(false);
	//}
}