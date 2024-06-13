// // @2023 All rights reversed by Reverse-Alpha Studios


#include "RangedWeapon.h"
#include "Engine/StaticMeshActor.h"


#include "Components/CapsuleComponent.h"
#include "Ascend/Tools/LogMacros.h"
#include "Ascend/AI/AIEntityModule.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

#include "Projectile/ProjectileModule.h"

ARangedWeapon::ARangedWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();

	StaticRangeWeaponPropertiesInitializer();
}

void ARangedWeapon::StaticRangeWeaponPropertiesInitializer()
{
	// This sets the weapon type as Ranged and prevents it from being changed dynamically.
	// Do not attempt to change this setting; it is static.
	SetWeaponType(EWeaponType::EWT_Ranged);

	// Update the properties of the ranged weapon based on its current type.
	RangedWeaponProperties.UpdateWeaponProperties(RangedWeaponProperties.GetRangedWeaponType());
}


FRangedWeaponProperties ARangedWeapon::GetRangedWeaponProperties()
{
	// Assuming RangedWeaponProperties is a member variable of ABaseCharacter
	return RangedWeaponProperties;
}

void ARangedWeapon::SetWeaponFireMode(EWeaponFiringMode newFireMode)
{
	RangedWeaponProperties.SetWeaponFireMode(newFireMode);
}

void ARangedWeapon::Fire(const FVector& HitPoint, FHitResult& HitResult)
{
	// Retrieves the location and rotation of the muzzle flash socket on the weapon mesh,
	// and sets the end point of the shot.
	const FName MuzzleFlashSocketName("MuzzleFlashSocket");
	FVector		MuzzleFlashSocketLocation	= WeaponMesh->GetSocketLocation(MuzzleFlashSocketName);
	FRotator	MuzzleFlashSocketRotation	= WeaponMesh->GetSocketRotation(MuzzleFlashSocketName);
	FVector		EndPoint					= HitPoint;

#define DEBUG_LINE 1
#if DEBUG_LINE == 0
	DrawDebugLine
	(
		GetWorld(),
		MuzzleFlashSocketLocation,
		EndPoint,
		FColor::Red
	);
#endif
	// Play Fire Sound to player
	if (WeaponFireSound)
	{
		InsertAndPlaySound2D(WeaponFireSound);
	}

	// Spawn Muzzle-Flash at its socket
	if (MuzzleFlash)
	{
		if (WeaponMesh->DoesSocketExist(MuzzleFlashSocketName))
		{
			UGameplayStatics::SpawnEmitterAttached
			(
				MuzzleFlash,
				WeaponMesh,
				MuzzleFlashSocketName,
				MuzzleFlashSocketLocation,
				MuzzleFlashSocketRotation,
				EAttachLocation::KeepWorldPosition
			);
		}
	}
#pragma region FireTrace
	// Set up the trace parameters
	FCollisionQueryParams CollisionParams;
	// Ignore the projectile itself during the trace
	CollisionParams.AddIgnoredActor(this);
	// Trace against complex collision, including detailed collision geometry
	CollisionParams.bTraceComplex = true;
	// Assign a trace tag for debugging or identification purposes
	CollisionParams.TraceTag = "ProjectileTrace";

	// Perform a line trace from the muzzle flash socket location to the end point
	FHitResult WeaponHitResult;
	bool bHit =
	GetWorld()->LineTraceSingleByChannel
	(
		WeaponHitResult,
		MuzzleFlashSocketLocation,
		EndPoint,
		ECC_Visibility,
		CollisionParams
	);
#pragma region Damage
	// Retrieve the current equipped weapon's damage value.
	float WeaponDamage = RangedWeaponProperties.GetDamage();

	// Retrieve the current equipped weapon's penetration value.
	float WeaponPenetration = RangedWeaponProperties.GetPenetration();

	// Calculate the total damage by multiplying the weapon's damage by its penetration.
	float TotalDamage = WeaponDamage * WeaponPenetration;

	// Calculate the durability damage by dividing the total damage by a fixed divisor.
	// Default durability divisor
	float DurabilityDivisor;
	// Adjust divisor as needed for balancing
	if (RangedWeaponProperties.GetWeaponFireMode() == EWeaponFiringMode::SingleShot)
	{
		DurabilityDivisor = 2.0f;
	}
	else if (RangedWeaponProperties.GetWeaponFireMode() == EWeaponFiringMode::Automatic)
	{
		DurabilityDivisor = 5.50f;
	}
	else if (RangedWeaponProperties.GetWeaponFireMode() == EWeaponFiringMode::SemiAutomatic)
	{
		DurabilityDivisor = 5.50f;
	}

	float DurabilityDamage = TotalDamage / DurabilityDivisor;

	// Retrieve the current durability value of the ranged weapon.
	float CurrentWeaponDurability = RangedWeaponProperties.GetDurability();

	// Update the durability property by subtracting the calculated durability damage
	// This simulates wear and tear on the weapon due to its usage
	RangedWeaponProperties.SetDurability(CurrentWeaponDurability - DurabilityDamage);

#pragma region FireType
	if (RangedWeaponProperties.GetFireType() == EFireType::EFT_HitScan)
	{
		if (RangedWeaponOptions.bSpawnTrail == true)
		{
			if (BulletTrail)
			{
				UParticleSystemComponent* Tracer =
					UGameplayStatics::SpawnEmitterAtLocation
					(
						GetWorld(),
						BulletTrail,
						HitResult.TraceStart,
						FRotator::ZeroRotator,
						true
					);
			}
		}

		// Get the actor that was hit by a trace or collision
		AActor* HitActor = WeaponHitResult.GetActor();
		if (HitActor && HitActor->IsA<AAIEntityModule>())
		{
			AAIEntityModule* AIEntity = Cast<AAIEntityModule>(HitActor);
			if (AIEntity)
			{
				// Get the mesh component from the AIEntity
				// Replace "UMeshComponent" with the actual type of your mesh component
				UMeshComponent* MeshComponent = AIEntity->GetMesh();

				if (MeshComponent)
				{		
					// Spawn particle emitters at impact point
					if (ImpactBodyParticle)
					{
						UGameplayStatics::SpawnEmitterAtLocation
						(
							GetWorld(),
							ImpactBodyParticle,
							WeaponHitResult.ImpactPoint,
							WeaponHitResult.ImpactNormal.Rotation()
						);
					}

					if (ImpactBodySound)
					{
						InsertAndPlaySoundAtLocation(ImpactBodySound, WeaponHitResult.ImpactPoint);
					}

					UGameplayStatics::ApplyDamage
					(
						AIEntity,
						TotalDamage,
						BaseCharacter->GetInstigatorController(),
						this,
						UDamageType::StaticClass()
					);	
				}
			}
		}
		else if (HitActor && HitActor->IsA<AStaticMeshActor>())
		{
			// Surface Hit

			//Spawn Surface particle
			if (ImpactMetalParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					ImpactMetalParticle,
					WeaponHitResult.ImpactPoint,
					WeaponHitResult.ImpactNormal.Rotation()
				);
			}

			if (ImpactSurfaceSound)
			{
				InsertAndPlaySoundAtLocation(ImpactSurfaceSound, WeaponHitResult.ImpactPoint);
			}
		}
	}
	else if (RangedWeaponProperties.GetFireType() == EFireType::EFT_Projectile)
	{
		APawn* InstigatorPawn = Cast<APawn>(GetOwner());
		AProjectileModule* ProjectileModuleInstance = nullptr;

		FVector TargetLocation = HitPoint - MuzzleFlashSocketLocation;
		FRotator TargetRotation = TargetLocation.Rotation();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner		= GetOwner();
		SpawnParameters.Instigator  = InstigatorPawn;

		AProjectileModule* ProjectileClass = 
		GetWorld()->SpawnActor<AProjectileModule>
		(
			ProjectModule,
			MuzzleFlashSocketLocation,
			TargetRotation,
			SpawnParameters
		);

		// Get the actor that was hit by a trace or collision
		AActor* FindActor = WeaponHitResult.GetActor();
		if (FindActor)
		{
			if (FindActor->IsA<AAIEntityModule>())
			{
				AAIEntityModule* AIEntity = Cast<AAIEntityModule>(FindActor);
				
				if (AIEntity)
				{
					if (ProjectileClass)
					{
						ProjectileClass->ReceiveAndApplyDamageAmount
						(
							AIEntity,
							TotalDamage,
							BaseCharacter->GetInstigatorController()
						);
					}
				}

				if (AIEntity->IsAIDead() && WeaponHitResult.IsValidBlockingHit())
				{
					UPrimitiveComponent* Target = WeaponHitResult.GetComponent();
					AActor* OwnerActor = Target->GetOwner();

					if (OwnerActor)
					{
						FVector NewVelocity = FVector(500.0f, 0.0f, 0.0f);

						Target->SetPhysicsLinearVelocity(NewVelocity);
					}
				}
			}
		}
	}

#define LOG_WEAPON_PROPERTIES 0
#if LOG_WEAPON_PROPERTIES == 1
	LW_TF("WeaponDamage: %f", WeaponDamage);
	LW_TF("CurrentDurability: %f", CurrentWeaponDurability);
	LW_TF("Durability Damage: %f", DurabilityDamage);
#endif

}

void ARangedWeapon::HandleHitScanFireMode()
{
	if (RangedWeaponProperties.GetWeaponFireMode() == EWeaponFiringMode::SingleShot)
	{
		if (bCanFire_HitScan)
		{
			bCanFire_HitScan = false;
		}
	}
	else if (RangedWeaponProperties.GetWeaponFireMode() == EWeaponFiringMode::SemiAutomatic)
	{
		if (bCanFire_HitScan)
		{
			bCanFire_HitScan = false;
		}
	}
	else if (RangedWeaponProperties.GetWeaponFireMode() == EWeaponFiringMode::Automatic)
	{
		while (bCanFire_HitScan)
		{

			bCanFire_HitScan = false;
		}
	}
}
