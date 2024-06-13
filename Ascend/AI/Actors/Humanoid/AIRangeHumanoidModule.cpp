// @2023 All rights reversed by Reverse-Alpha Studios


#include "AIRangeHumanoidModule.h"

#include "Components/SphereComponent.h"

void AAIRangeHumanoidModule::BeginPlay()
{
	Super::BeginPlay();
	SpawnAndAttachRangeWeapon();
}

void AAIRangeHumanoidModule::SpawnAndAttachRangeWeapon()
{
	if (RangeWeapons.Num() == 0) return;

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (CharacterMesh == nullptr) return;

	int32 RandomIndex = FMath::RandRange(0, RangeWeapons.Num() - 1);

	TSubclassOf<ARangedWeapon> SelectedRangeWeaponClass = RangeWeapons[RandomIndex];


	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();

	ARangedWeapon* SpawnedRangeWeapon = GetWorld()->SpawnActor<ARangedWeapon>
		(
			SelectedRangeWeaponClass,
			GetActorLocation(),
			GetActorRotation(),
			SpawnParameters
		);

	FName SocketName = "AI_RangedWeaponSlot";

	if (SpawnedRangeWeapon)
	{
		SpawnedRangeWeapon->GetAreaCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedRangeWeapon->GetWeaponMesh()->SetSimulatePhysics(false);
		SpawnedRangeWeapon->GetWeaponMesh()->SetEnableGravity(false);
		SpawnedRangeWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedRangeWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		SpawnedRangeWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		SetRangedWeaponClass(SpawnedRangeWeapon);
	}
}
