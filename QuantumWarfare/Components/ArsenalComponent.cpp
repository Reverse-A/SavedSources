// @2023 All rights reversed by Reverse-Alpha Studios


#include "ArsenalComponent.h"

#include <QuantumWarfare/Character/PlayerCharacter.h>
#include <QuantumWarfare/Actor/Weapons/WeaponModule.h>
#include <QuantumWarfare/Actor/Weapons/Melee/MeleeWeapon.h>
#include <QuantumWarfare/Actor/Weapons/Range/RangeWeapon.h>
#include "Engine/SkeletalMeshSocket.h"


UArsenalComponent::UArsenalComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UArsenalComponent::SetOverlappingWeapon(AWeaponModule* InWeapon)
{
	OverlapWeapon = InWeapon;
}

void UArsenalComponent::SetEquippedWeaponRenderCustomDepth(bool InValue)
{
	if (EquippedWeapon != nullptr)
	{
		//TODO : Fix the issues about SetRenderCustomDepth including or accessing this function somehow it is red squiggling
		//EquippedWeapon->SetRenderCustomDepth(InValue);
	}
}

void UArsenalComponent::SetWeaponDissolveDynamicMaterialInstance(UMaterialInstanceDynamic* DynamicMatInst)
{
	if (EquippedWeapon != nullptr)
	{
		EquippedWeapon->GetWeaponMesh()->SetMaterial(0, DynamicMatInst);
	}
}


void UArsenalComponent::SetPlayerCharacter(APlayerCharacter* InPC)
{
	PlayerCharacter = InPC;
}

void UArsenalComponent::SetModuleCharacter(ACharacterModule* InModCharacter)
{
	CharacterModule = InModCharacter;
}

void UArsenalComponent::SpawnWeapon()
{
	if (CharacterModule)
	{
		FTransform SocketTransform = CharacterModule->GetMesh()->GetSocketTransform(FName("RH_AssaultRifle"), ERelativeTransformSpace::RTS_World);
		AWeaponModule* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponModule>(AssaultRifle);

		if (SpawnedWeapon)
		{
			SpawnedWeapon->AttachToComponent(CharacterModule->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RH_AssaultRifle"));

			SpawnedWeapon->SetWeaponState(EWeaponState::Acquired);
			CharacterModule->ABPSetWeaponEquipped();
			EquippedWeapon = SpawnedWeapon;
		}
	}
}

void UArsenalComponent::DropWeapon()
{
	if (EquippedWeapon != nullptr)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		EquippedWeapon->SetWeaponState(EWeaponState::Dropped);
	}
}

void UArsenalComponent::DestroyEnemyWeapon()
{
	if (EquippedWeapon != nullptr)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		EquippedWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EquippedWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
		EquippedWeapon->GetWeaponMesh()->SetEnableGravity(true);
		EquippedWeapon->GetWeaponMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		EquippedWeapon->Dissolve();
		GetWorld()->GetTimerManager().SetTimer
		(
			DestroyTimerHandler,
			this,
			&UArsenalComponent::DestroyEquippedWeapon, // Pass the function pointer to the timer
			2.9f,
			false
		);
	}
}

void UArsenalComponent::DestroyEquippedWeapon()
{
	EquippedWeapon->SetWeaponState(EWeaponState::Destroyed);
}

FVector UArsenalComponent::GetWeaponMuzzleFlashSocketLocation()
{
	return 	EquippedWeapon->GetWeaponMesh()->GetSocketLocation("MuzzleFlash");
}

void UArsenalComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
}
