// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeapon.h"

#include "Characters/BaseCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/Projectile.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	UWorld* World = GetWorld();
	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform		= MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		FVector TargetLocation			= HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation			= TargetLocation.Rotation();

		FActorSpawnParameters			  SpawnParameters;
		SpawnParameters.Owner			= GetOwner();
		SpawnParameters.Instigator		= InstigatorPawn;

		AProjectile* SpawnedProjectile	= nullptr;
		if (ServerSideRewind())
		{
			if (InstigatorPawn->HasAuthority())	// Server
			{
				if (InstigatorPawn->IsLocallyControlled())	// Server, host - use replicated projectile
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>
						(
							ProjectileClass, 
							SocketTransform.GetLocation(),
							TargetRotation,
							SpawnParameters
						);

					SpawnedProjectile->SetServerSideRewind(false);
					SpawnedProjectile->Damage = Damage;
					SpawnedProjectile->HeadShotDamage = HeadShotDamage;
				}
				else // Server, not locally controlled - spawn non-replicated projectile, SSR
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>
						(
							ServerSideRewindProjectileClass, 
							SocketTransform.GetLocation(),
							TargetRotation, 
							SpawnParameters
						);
					SpawnedProjectile->SetServerSideRewind(true);
				}
			}
			else // client, using SSR
			{
				if (InstigatorPawn->IsLocallyControlled())  // Client, locally controlled - spawn non-replicated projectile, use SSR
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParameters);
					SpawnedProjectile->SetServerSideRewind(true);
					SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
					SpawnedProjectile->InitialVelocity = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->GetProjectileSpeed();
				}
				else // client, not locally controlled - spawn non-replicated projectile, no SSR
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParameters);
					SpawnedProjectile->SetServerSideRewind(false);
				}
			}
		}
		else // weapon not using SSR
		{
			if (InstigatorPawn->HasAuthority())
			{
				SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParameters);
				SpawnedProjectile->SetServerSideRewind(false);
				SpawnedProjectile->Damage = Damage;
				SpawnedProjectile->HeadShotDamage = HeadShotDamage;
			}
		}
	}
}
