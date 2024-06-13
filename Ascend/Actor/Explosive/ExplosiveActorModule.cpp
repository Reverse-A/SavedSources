// @2023 All rights reversed by Reverse-Alpha Studios


#include "ExplosiveActorModule.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

#include "Ascend/Character/BaseCharacter.h"
#include "Ascend/AI/AIEntityModule.h"
#include "Ascend/Weapon/BaseWeapon.h"
#include "Ascend/Weapon/RangedWeapon.h"
#include "Ascend/Weapon/MeleeWeapon.h"


void AExplosiveActorModule::BeginPlay()
{
	Super::BeginPlay();
	
	Explode();
}

void AExplosiveActorModule::Explode()
{
	if (ExplosionParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			ExplosionParticleSystem,
			GetActorLocation()
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

	TArray<FOverlapResult> OverlapResults;
	FVector Center = GetActorLocation();
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(ExplosionRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore self
	GetWorld()->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECC_Pawn, CollisionSphere, QueryParams);


	// Location at the explosion
	FVector ExplosionLocation = GetActorLocation();


	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		// Access the actor pointer from the overlap result
		AActor* OverlappingActor = OverlapResult.GetActor();


		// Now you can use OverlappingActor as needed
		if (OverlappingActor)
		{
			if (OverlappingActor->IsA<ABaseCharacter>())
			{
				// ABaseCharacter handling
				ABaseCharacter* Character = Cast<ABaseCharacter>(OverlappingActor);
				if (Character)
				{
					// Calculate distance between character and explosion location
					float Distance = FVector::Distance(Character->GetActorLocation(), ExplosionLocation);

					// Calculate damage based on distance
					float Damage = MaximumExplosionDamage * FMath::Square(1.0f - FMath::Clamp(Distance / ExplosionRadius, 0.0f, 1.0f));

					Character->ReceiveDamage(Damage);
				}
			}

			if (OverlappingActor->IsA<AAIEntityModule>())
			{
				// AAIEntityModule handling
				AAIEntityModule* AIEntity = Cast<AAIEntityModule>(OverlappingActor);
				if (AIEntity)
				{
					// Calculate distance between AI and explosion location
					float Distance = FVector::Distance(AIEntity->GetActorLocation(), ExplosionLocation);

					// Calculate damage based on distance
					float Damage = MaximumExplosionDamage * FMath::Square(1.0f - FMath::Clamp(Distance / ExplosionRadius, 0.0f, 1.0f));
					
					AIEntity->ReceiveDamage(Damage);
				}
			}

			//if (OverlappingActor->IsA<ARangedWeapon>())
			//{
			//	// ABaseWeapon handling
			//	ARangedWeapon* RangedWeapon = Cast<ARangedWeapon>(OverlappingActor);
			//	if (RangedWeapon)
			//	{
			//		if (RangedWeapon)
			//		{
			//			float Distance = FVector::Distance(RangedWeapon->GetActorLocation(), ExplosionLocation);

			//			float DurabilityDamageFromExplosion = FMath::Clamp((1.0f - (Distance / ExplosionRadius)) * 1000.0f, 0.0f, 1000.0f);

			//			RangedWeapon->GetRangedWeaponProperties().SetDurability(DurabilityDamageFromExplosion);
			//		}
			//	}
			//}
			//		

			//if (OverlappingActor->IsA<AMeleeWeapon>())
			//{
			//	AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(OverlappingActor);
			//	if (MeleeWeapon)
			//	{
			//		float Distance = FVector::Distance(MeleeWeapon->GetActorLocation(), ExplosionLocation);

			//		// Calculate durability damage based on distance

			//		float DurabilityDamageFromExplosion = FMath::Clamp((1.0f - (Distance / ExplosionRadius)) * 1000.0f, 0.0f, 1000.0f);
			//		// Apply durability damage to melee weapon
			//		MeleeWeapon->GetMeleeWeaponProperties().SetDurability(DurabilityDamageFromExplosion);

			//		UE_LOG(LogTemp, Warning, TEXT("MeleeWeapon - Durability Damage: %f"), DurabilityDamageFromExplosion);
			//	}
			//}
					
		}
	}

	//for (AActor* Actor : OverlappingActors)
	//{
	//	FVector ActorLocation = Actor->GetActorLocation();
	//	float Distance = FVector::Dist(ActorLocation, GetActorLocation());
	//	if (Distance <= ExplosionRadius)
	//	{
	//		// Apply damage to nearby actors in a radial area
	//		FDamageEvent		DamageEvent;
	//		FRadialDamageEvent	RadialDamageEvent;
	//		// Assuming you have a default damage type class
	//		RadialDamageEvent.DamageTypeClass		= UDamageType::StaticClass(); 
	//		RadialDamageEvent.Origin				= GetActorLocation();
	//		RadialDamageEvent.Params.BaseDamage		= ExplosionDamage;
	//		RadialDamageEvent.Params.OuterRadius	= ExplosionRadius;

	//		TArray<AActor*> IgnoredActors;
	//		UGameplayStatics::ApplyRadialDamage
	//		(
	//			this,
	//			ExplosionDamage, 
	//			GetActorLocation(), 
	//			ExplosionRadius, 
	//			UDamageType::StaticClass(), 
	//			IgnoredActors,
	//			this,
	//			nullptr, 
	//			true, 
	//			ECC_Visibility
	//		);
	//	}
	//}
	Destroy();
}
