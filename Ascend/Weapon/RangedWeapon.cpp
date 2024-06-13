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

	InitialRangeWeaponProperties();
}

void ARangedWeapon::InitialRangeWeaponProperties()
{
	// This sets the weapon type as Ranged and prevents it from being changed dynamically.
	// Do not attempt to change this setting; it is static.
	SetWeaponType(EWeaponType::EWT_Ranged);

	// Update the properties of the ranged weapon based on its current type.
	RangedWeaponProperties.UpdateWeaponProperties(RangedWeaponProperties.GetRangedWeaponType());

	FName ItemName;
	FString ItemDescription;

	if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Pistol)
	{
		SetInventoryItemImage(RangedWeaponImages.GetPistolImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Pistol (Common)";
			ItemDescription = "A common pistol.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Pistol (Rare)";
			ItemDescription = "A rare pistol with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Pistol (Epic)";
			ItemDescription = "An epic pistol imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Pistol (Unique)";
			ItemDescription = "A one-of-a-kind pistol, legendary among firearms.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Rifle)
	{
		SetInventoryItemImage(RangedWeaponImages.GetRifleImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Rifle (Common)";
			ItemDescription = "A common rifle.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Rifle (Rare)";
			ItemDescription = "A rare rifle with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Rifle (Epic)";
			ItemDescription = "An epic rifle imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Rifle (Unique)";
			ItemDescription = "A one-of-a-kind rifle, legendary among firearms.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Submachine)
	{
		SetInventoryItemImage(RangedWeaponImages.GetSubmachineGunImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Submachine Gun (Common)";
			ItemDescription = "A common submachine gun.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Submachine Gun (Rare)";
			ItemDescription = "A rare submachine gun with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Submachine Gun (Epic)";
			ItemDescription = "An epic submachine gun imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Submachine Gun (Unique)";
			ItemDescription = "A one-of-a-kind submachine gun, legendary among firearms.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Heavymachine)
	{
		SetInventoryItemImage(RangedWeaponImages.GetHeavyMachineGunImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Heavy-machine Gun (Common)";
			ItemDescription = "A common heavy-machine gun.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Heavy-machine Gun (Rare)";
			ItemDescription = "A rare heavy-machine gun with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Heavy-machine Gun (Epic)";
			ItemDescription = "An epic heavy-machine gun imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Heavy-machine Gun (Unique)";
			ItemDescription = "A one-of-a-kind heavy-machine gun, legendary among firearms.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Shotgun)
	{
		SetInventoryItemImage(RangedWeaponImages.GetShotgunImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Shotgun (Common)";
			ItemDescription = "A common shotgun.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Shotgun (Rare)";
			ItemDescription = "A rare shotgun with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Shotgun (Epic)";
			ItemDescription = "An epic shotgun imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Shotgun (Unique)";
			ItemDescription = "A one-of-a-kind shotgun, legendary among firearms.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Sniper)
	{
		SetInventoryItemImage(RangedWeaponImages.GetSniperRifleImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Sniper Rifle (Common)";
			ItemDescription = "A common sniper rifle.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Sniper Rifle (Rare)";
			ItemDescription = "A rare sniper rifle with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Sniper Rifle (Epic)";
			ItemDescription = "An epic sniper rifle imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Sniper Rifle (Unique)";
			ItemDescription = "A one-of-a-kind sniper rifle, legendary among firearms.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Bow)
	{
		SetInventoryItemImage(RangedWeaponImages.GetBowImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Bow (Common)";
			ItemDescription = "A common bow.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Bow (Rare)";
			ItemDescription = "A rare bow with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Bow (Epic)";
			ItemDescription = "An epic bow imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Bow (Unique)";
			ItemDescription = "A one-of-a-kind bow, legendary among ranged weapons.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Crossbow)
	{
		SetInventoryItemImage(RangedWeaponImages.GetCrossbowImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Crossbow (Common)";
			ItemDescription = "A common crossbow.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Crossbow (Rare)";
			ItemDescription = "A rare crossbow with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Crossbow (Epic)";
			ItemDescription = "An epic crossbow imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Crossbow (Unique)";
			ItemDescription = "A one-of-a-kind crossbow, legendary among ranged weapons.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Throwing)
	{
		SetInventoryItemImage(RangedWeaponImages.GetThrowingWeaponImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Throwing Weapon (Common)";
			ItemDescription = "A common throwing weapon.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Throwing Weapon (Rare)";
			ItemDescription = "A rare throwing weapon with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Throwing Weapon (Epic)";
			ItemDescription = "An epic throwing weapon imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Throwing Weapon (Unique)";
			ItemDescription = "A one-of-a-kind throwing weapon, legendary among ranged weapons.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Launcher)
	{
		SetInventoryItemImage(RangedWeaponImages.GetLauncherImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Launcher (Common)";
			ItemDescription = "A common launcher.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Launcher (Rare)";
			ItemDescription = "A rare launcher with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Launcher (Epic)";
			ItemDescription = "An epic launcher imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Launcher (Unique)";
			ItemDescription = "A one-of-a-kind launcher, legendary among ranged weapons.";
		}
	}
	else if (RangedWeaponProperties.GetRangedWeaponType() == ERangedWeaponType::ERT_Cannon)
	{
		SetInventoryItemImage(RangedWeaponImages.GetCannonImage());

		if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Cannon (Common)";
			ItemDescription = "A common cannon.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Cannon (Rare)";
			ItemDescription = "A rare cannon with unique craftsmanship.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Cannon (Epic)";
			ItemDescription = "An epic cannon imbued with mystical powers.";
		}
		else if (RangedWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Cannon (Unique)";
			ItemDescription = "A one-of-a-kind cannon, legendary among ranged weapons.";
		}
	}

	float Price = RangedWeaponProperties.GetPrice();

	SetInventoryItemName(ItemName);
	SetInventoryItemPrice(Price);
	SetInventoryItemDescription(ItemDescription);
}


FRangedWeaponProperties ARangedWeapon::GetRangedWeaponProperties()
{
	// Assuming RangedWeaponProperties is a member variable of ABaseCharacter
	return RangedWeaponProperties;
}
ERangedWeaponType ARangedWeapon::GetRangedWeaponType()
{
	return RangedWeaponProperties.GetRangedWeaponType();
}

void ARangedWeapon::SetWeaponFireMode(EWeaponFiringMode newFireMode)
{
	RangedWeaponProperties.SetWeaponFireMode(newFireMode);
}
void ARangedWeapon::SetWeaponFireType(EFireType newFireType)
{
	RangedWeaponProperties.SetFireType(newFireType);
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

		FString ActorName = HitActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("ActorName: %s"), *ActorName);

		if (HitActor && HitActor->IsA<AAIEntityModule>())
		{
			AAIEntityModule* AIEntity = Cast<AAIEntityModule>(HitActor);
			if (AIEntity)
			{

				// First, let's align the LevelSystem we have defined with the Level system on the AI
				if (BaseCharacter)
				{
			
				}

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
