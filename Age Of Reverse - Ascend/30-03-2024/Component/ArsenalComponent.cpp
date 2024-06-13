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
	
}

void UArsenalComponent::ArrangeWeaponByTypesAndClasses(ABaseWeapon* InsertOverlapWeapon)
{
	if (InsertOverlapWeapon == nullptr) return;

	if (InsertOverlapWeapon->GetWeaponType() == EWeaponType::EWT_Melee)
	{
		MeleeWeapon = Cast<AMeleeWeapon>(InsertOverlapWeapon);
		MeleeWeapons.Add(MeleeWeapon);
		EquipMeleeWeaponOnGround(MeleeWeapon);
	}
	else if (InsertOverlapWeapon->GetWeaponType() == EWeaponType::EWT_Ranged)
	{
		RangedWeapon = Cast<ARangedWeapon>(InsertOverlapWeapon);
		RangedWeapons.Add(RangedWeapon);
		EquipRangeWeaponOnGround(RangedWeapon);
	}
}


void UArsenalComponent::EquipMeleeWeaponOnGround(AMeleeWeapon* InsertMeleeWeapon)
{
	if (InsertMeleeWeapon == nullptr || (PrimaryEquipWeapon != nullptr && SecondaryEquipWeapon != nullptr)) return;

	if (PrimaryEquipWeapon == nullptr && SecondaryEquipWeapon == nullptr)
	{	
		AttachWeaponToActor(InsertMeleeWeapon);
		PrimaryEquipWeapon = InsertMeleeWeapon;
		BaseCharacter->GetCharacterAnimInstance()->SetWeaponType(EWeaponType::EWT_Melee);
		InsertMeleeWeapon->SetOwner(BaseCharacter->GetOwner());
		InsertMeleeWeapon->SetInstigator(BaseCharacter->GetInstigator());		
	}
	else if (PrimaryEquipWeapon != nullptr && SecondaryEquipWeapon == nullptr)
	{	
		SecondaryEquipWeapon = InsertMeleeWeapon;
		InsertMeleeWeapon->Destroy();
	}
}

void UArsenalComponent::EquipRangeWeaponOnGround(ARangedWeapon* InsertRangeWeapon)
{
	if (RangedWeapon == nullptr || (PrimaryEquipWeapon != nullptr && SecondaryEquipWeapon != nullptr)) return;

	if (PrimaryEquipWeapon == nullptr && SecondaryEquipWeapon == nullptr)
	{
		AttachWeaponToActor(InsertRangeWeapon);
		PrimaryEquipWeapon   = InsertRangeWeapon;
		
	}
	else if (PrimaryEquipWeapon != nullptr && SecondaryEquipWeapon == nullptr)
	{
		SecondaryEquipWeapon = InsertRangeWeapon;
		InsertRangeWeapon->Destroy();
	}
}

void UArsenalComponent::AttachWeaponToActor(ABaseWeapon* Weapon)
{
	if (Weapon == nullptr) return;

	if (Weapon->GetWeaponType() == EWeaponType::EWT_Melee)
	{
		Weapon->SetWeaponState(EWeaponState::EWS_Equipped);
		AMeleeWeapon* WeaponToAttach = Cast<AMeleeWeapon>(Weapon);
		EMeleeWeaponType CheckWeaponType = WeaponToAttach->GetMeleeWeaponProperties().MeleeWeaponType;

		const USkeletalMeshSocket* WeaponHandSocket = nullptr;

		switch (CheckWeaponType)
		{
		case EMeleeWeaponType::EMT_Katana:
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
