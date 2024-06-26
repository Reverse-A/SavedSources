// ©2023 Reverse-A. All rights reserved.


#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Synthetica/AI/BaseEnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"

void AShotgun::Fire(const FVector& HitTarget)
{
	AWeapon::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		TMap<ABaseEnemyAI* , uint32> HitMap;

		for (uint32 i = 0; i < NumberOfPellets; i++)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			IHitInterface* HitInterface = Cast<IHitInterface>(FireHit.GetActor());
			if (HitInterface)
			{
				HitInterface->GetHit(FireHit.ImpactPoint);
			}
			ABaseEnemyAI* BaseEnemyAI = Cast<ABaseEnemyAI>(FireHit.GetActor());
			if (BaseEnemyAI && InstigatorController)
			{
				if (HitMap.Contains(BaseEnemyAI))
				{
					HitMap[BaseEnemyAI]++;
				}
				else
				{
					HitMap.Emplace(BaseEnemyAI, 1);
				}
			}

			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation()
				);
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					FireHit.ImpactPoint,
					.5f,
					FMath::FRandRange(-.5f, .5f)
				);
			}
		}
		for (auto HitPair : HitMap)
		{
			if (HitPair.Key && InstigatorController)
			{
				UGameplayStatics::ApplyDamage(
					HitPair.Key,
					RaycastWeaponDamage * HitPair.Value,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}