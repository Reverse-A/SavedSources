// ©2023 Reverse - A Company. All rights reserved.


#include "Weapons/ProjectileWeapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Weapons/Projectile.h"


void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	if (!HasAuthority()) return;

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	if (MuzzleFlashSocket)
	{
		FTransform WeaponSocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		// From muzzle flash socket to hit location from trace under crosshairs.
		FVector TargetLocation = HitTarget - WeaponSocketTransform.GetLocation();
		FRotator TargetRotation = TargetLocation.Rotation();

		if (ProjectileClass && InstigatorPawn)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner		= GetOwner();
			SpawnParameters.Instigator	= InstigatorPawn;

			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AProjectile>(
					ProjectileClass,
					WeaponSocketTransform.GetLocation(),
					TargetRotation,
					SpawnParameters
					);
			}
		}
	}
}
