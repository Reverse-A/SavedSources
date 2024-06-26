// ©2023 Reverse-A. All rights reserved.


#include "RaycastWeapons.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Synthetica/AI/BaseEnemyAI.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "RangedWeaponTypes.h"

void ARaycastWeapons::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		UWorld* World = GetWorld();

		ABaseEnemyAI* BaseEnemyAI = Cast<ABaseEnemyAI>(FireHit.GetActor());
		if (BaseEnemyAI && InstigatorController)
		{
			if (GetRangedWeaponTypes() == ERangedWeaponTypes::ERWT_SniperRifle)
			{
				UE_LOG(LogTemp, Warning, TEXT("Flag1"));
				UGameplayStatics::ApplyDamage(
					BaseEnemyAI,
					SniperWeaponDamage,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Flag2"));
				UGameplayStatics::ApplyDamage(
					BaseEnemyAI,
					RaycastWeaponDamage,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}
		}
		IHitInterface* HitInterface = Cast<IHitInterface>(FireHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(FireHit.ImpactPoint);
		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				World,
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
				FireHit.ImpactPoint
			);
		}
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				World,
				MuzzleFlash,
				SocketTransform
			);
		}
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				FireSound,
				GetActorLocation()
			);
		}
	}

}

void ARaycastWeapons::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) : TraceStart + (HitTarget - TraceStart) * 1.25f;

		World->LineTraceSingleByChannel(
			OutHit,
			TraceStart,
			End,
			ECollisionChannel::ECC_Visibility
		);

		FVector BeamEnd = End;
		if (OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam =
				UGameplayStatics::SpawnEmitterAtLocation(
					World,
					BeamParticles,
					TraceStart,
					FRotator::ZeroRotator,
					true
				);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}


FVector ARaycastWeapons::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget)
{
	FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	FVector RandVector = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	FVector EndLocation = SphereCenter + RandVector;
	FVector ToEndLocation = EndLocation - TraceStart;

	// DebugSphere
	/*DrawDebugSphere(
		GetWorld(),
		SphereCenter,
		SphereRadius,
		12,
		FColor::Cyan,
		true
	);

	DrawDebugSphere(
		GetWorld(),
		EndLocation,
		4.f,
		12,
		FColor::Red,
		true
	);

	DrawDebugLine(
		GetWorld(),
		TraceStart,
		FVector(TraceStart + ToEndLocation * TRACE_LENGTH / ToEndLocation.Size()),
		FColor::Orange,
		true
	);*/ 

	return FVector(TraceStart + ToEndLocation * TRACE_LENGTH / ToEndLocation.Size());
}


