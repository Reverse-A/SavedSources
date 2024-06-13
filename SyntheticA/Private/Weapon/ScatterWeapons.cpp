// @2023 All rights reversed by Reverse-Alpha Studios


#include "Weapon/ScatterWeapons.h"

#include "Characters/BaseCharacter.h"
#include "Components/LagCompensationComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Sound/SoundCue.h"

#include "DrawDebugHelpers.h"

void AScatterWeapons::FireScatterWeapons(const TArray<FVector_NetQuantize>& HitTargets)
{
	AWeapon::Fire(FVector());

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController	 = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{

		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector Start				 = SocketTransform.GetLocation();

		// Maps hit character to number of times hit
		TMap<ABaseCharacter*, uint32> HitMap;
		TMap<ABaseCharacter*, uint32> HeadShotHitMap;

		for (FVector_NetQuantize HitTarget : HitTargets)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(FireHit.GetActor());
			if (BaseCharacter)
			{
				const bool IsHeadShotHit = FireHit.BoneName.ToString() == FString("head");

				if (IsHeadShotHit)
				{
					if (HeadShotHitMap.Contains(BaseCharacter))
					{
						HeadShotHitMap[BaseCharacter]++;
					}
					else
					{
						HeadShotHitMap.Emplace(BaseCharacter, 1);
					}
				}
				else
				{
					if (HitMap.Contains(BaseCharacter))
					{
						HitMap[BaseCharacter]++;
					}
					else
					{
						HitMap.Emplace(BaseCharacter, 1);
					}
				}

				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation
					(
						GetWorld(),
						ImpactParticles,
						FireHit.ImpactPoint,
						FireHit.ImpactNormal.Rotation()
					);
				}

				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation
					(
						this,
						HitSound,
						FireHit.ImpactPoint,
						0.5f,
						FMath::FRandRange(-0.5f, 0.5f)
					);
				}
			}
		}

		TArray<ABaseCharacter*> HitCharacters;	
		TMap<ABaseCharacter*, float> DamageMap;		//Maps Character hit to total damage

		// Calculate body shot damage by multiplying times hit x Damage - store in DamageMap
		for (auto HitPair : HitMap)
		{
			if (HitPair.Key)
			{
				DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);
				HitCharacters.AddUnique(HitPair.Key);
			}
		}

		// Calculate body shot damage by multiplying times hit x HeadShotDamage - store in DamageMap
		for (auto HeadShotHitPair : HeadShotHitMap)
		{
			if (HeadShotHitPair.Key)
			{
				if (DamageMap.Contains(HeadShotHitPair.Key))
				{
					DamageMap[HeadShotHitPair.Key] += HeadShotHitPair.Value * HeadShotDamage;
				}
				else
				{
					DamageMap.Emplace(HeadShotHitPair.Key, HeadShotHitPair.Value * HeadShotDamage);
				}
				HitCharacters.AddUnique(HeadShotHitPair.Key);
			}
		}

		// Loop through DamageMap to get total damage for each character
		for (auto DamagePair : DamageMap)
		{
			if (DamagePair.Key && InstigatorController)
			{
				bool bCauseAuthDamage = !ServerSideRewind() || OwnerPawn->IsLocallyControlled();

				if (HasAuthority() && bCauseAuthDamage)
				{
					UGameplayStatics::ApplyDamage
					(
						DamagePair.Key,
						DamagePair.Value,
						InstigatorController,
						this,
						UDamageType::StaticClass()
					);
				}
			}
		}

		if (!HasAuthority() && ServerSideRewind())
		{
			BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(OwnerPawn) : BaseOwnerCharacter;
			PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APlayerControllerManager>(InstigatorController) : PlayerOwnerController;

			if (PlayerOwnerController && BaseOwnerCharacter && BaseOwnerCharacter->GetLagCompensationComponent() && BaseOwnerCharacter->IsLocallyControlled())
			{
				BaseOwnerCharacter->GetLagCompensationComponent()->Server_ShotgunScoreRequest
				(
					HitCharacters,
					Start,
					HitTargets,
					PlayerOwnerController->GetServerTime() - PlayerOwnerController->GetSingleTripTime()
				);		
			}
		}
	}


	/*For Aerodyne Types of Vehicles*/
	if (MuzzleFlashSocket)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector Start = SocketTransform.GetLocation();

		/*Maps hit Aerodyne to number of times hit*/
		TMap<AAerodyne*, uint32> AeroHitMap;

		for (FVector_NetQuantize AeroHitTarget : HitTargets)
		{
			FHitResult AeroHit;
			WeaponTraceHit(Start, AeroHitTarget, AeroHit);

			AAerodyne* Aerodyne = Cast<AAerodyne>(AeroHit.GetActor());
			if (Aerodyne)
			{
				if (AeroHitMap.Contains(Aerodyne))
				{
					AeroHitMap[Aerodyne]++;
				}
				else
				{
					AeroHitMap.Emplace(Aerodyne, 1);
				}

				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation
					(
						GetWorld(),
						ImpactParticles,
						AeroHit.ImpactPoint,
						AeroHit.ImpactNormal.Rotation()
					);
				}

				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation
					(
						this,
						HitSound,
						AeroHit.ImpactPoint,
						0.5f,
						FMath::FRandRange(-0.5f, 0.5f)
					);
				}
			}
		}

		TArray<AAerodyne*> HitAerodyne;
		TMap<AAerodyne*, float> AerodyneDamageMap;

		float ScatterDamagePerPiece = FMath::FRandRange(15.0f, 25.0f);
		for (auto AeroHitPair : AeroHitMap)
		{
			if (AeroHitPair.Key)
			{
				
				AerodyneDamageMap.Emplace(AeroHitPair.Key, AeroHitPair.Value * ScatterDamagePerPiece);
				HitAerodyne.AddUnique(AeroHitPair.Key);
			}
		}

		// Loop through DamageMap to get total damage for each Aerodyne
		for (auto AeroDamagePair : AerodyneDamageMap)
		{
			if (AeroDamagePair.Key && InstigatorController)
			{
				if (HasAuthority())
				{
					AActor* AerodyneActor = Cast<AActor>(AeroDamagePair.Key);
					UGameplayStatics::ApplyDamage
					(
						AerodyneActor,
						AeroDamagePair.Value,
						InstigatorController,
						this,
						UDamageType::StaticClass()
					);
					TotalVehicleDamageAmount = AeroDamagePair.Value;
				}
			}
		}
	}
}



void AScatterWeapons::ScatterTypeWeaponsTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket == nullptr) return;

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart		 = SocketTransform.GetLocation();

	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter		 = TraceStart + ToTargetNormalized * DistanceToSphere;

	for (uint32 i = 0; i < NumberOfPellets; i++)
	{
		const FVector RandomVector = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
		const FVector EndLocation = SphereCenter + RandomVector;
		FVector ToEndLocation = EndLocation - TraceStart;
		ToEndLocation		  = TraceStart + ToEndLocation * TRACE_MAX_LENGHT / ToEndLocation.Size();

		HitTargets.Add(ToEndLocation);
	}
}

float AScatterWeapons::GetVehicleTotalDamageAmountWithScatters()
{
	return TotalVehicleDamageAmount;
}