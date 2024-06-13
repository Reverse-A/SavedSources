// @2023 All rights reversed by Reverse-Alpha Studios


#include "Weapon/WeaponSpawnPoint.h"

#include "Weapon/Weapon.h"

AWeaponSpawnPoint::AWeaponSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AWeaponSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnWeaponTimer((AActor*)nullptr);
}

void AWeaponSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponSpawnPoint::SpawnWeapons()
{
	int32 NumberOfWeaponClasses = WeaponClasses.Num();

	if (NumberOfWeaponClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumberOfWeaponClasses - 1);
		SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClasses[Selection], GetActorTransform());

		if (HasAuthority() && SpawnedWeapon)
		{
			SpawnedWeapon->OnDestroyed.AddDynamic(this, &AWeaponSpawnPoint::StartSpawnWeaponTimer);
		}
	}
}

void AWeaponSpawnPoint::StartSpawnWeaponTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnWeaponTimeMin, SpawnWeaponTimeMax);
	GetWorldTimerManager().SetTimer
	(
		SpawnWeaponTimer,
		this,
		&AWeaponSpawnPoint::SpawnWeaponTimerFinished,
		SpawnTime
	);
}

void AWeaponSpawnPoint::SpawnWeaponTimerFinished()
{
	if (HasAuthority())
	{
		SpawnWeapons();
	}
}