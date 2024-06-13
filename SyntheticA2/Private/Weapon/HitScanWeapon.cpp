// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|


#include "Weapon/HitScanWeapon.h"

#include "Characters/BaseCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

#include "Weapon/WeaponTypes.h"

#include "DrawDebugHelpers.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();


	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		UWorld* World = GetWorld();

		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(FireHit.GetActor());
		if (BaseCharacter && HasAuthority() && InstigatorController)
		{
			UGameplayStatics::ApplyDamage
			(
				BaseCharacter,
				Damage,
				InstigatorController,
				this,
				UDamageType::StaticClass()
			);
		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation
			(
				World,
				ImpactParticles,
				FireHit.ImpactPoint,
				FireHit.Normal.Rotation()
			);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				HitSound,
				FireHit.ImpactPoint
			);
		}
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation
			(
				World,
				MuzzleFlash,
				SocketTransform
			);
		}
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				FireSound,
				GetActorLocation()
			);
		}
	}
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) : TraceStart + (HitTarget - TraceStart) * 1.25f;

		World->LineTraceSingleByChannel
		(
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
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation
			(
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


FVector AHitScanWeapon::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget)
{
	FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	FVector RandomVector = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.0f, SphereRadius);
	FVector EndLocation = SphereCenter + RandomVector;
	FVector ToEndLocation = EndLocation - TraceStart;

	/*
	DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, true);
	DrawDebugSphere(GetWorld(), EndLocation, 4.0f, 12, FColor::Cyan, true);
	DrawDebugLine
	(
		GetWorld(),
		TraceStart,
		FVector(TraceStart + ToEndLocation * MAX_TRACE_LENGTH / ToEndLocation.Size()),
		FColor::Orange,
		true
	);*/

	return FVector(TraceStart + ToEndLocation * MAX_TRACE_LENGTH / ToEndLocation.Size());
}

