// ©2023 Reverse-A. All rights reserved.


#include "ExplosiveGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Synthetica/Weapons/Projectile.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"




AExplosiveGrenade::AExplosiveGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AExplosiveGrenade::Destroyed()
{
	ExplodeDamage();
	Super::Destroyed();
}

void AExplosiveGrenade::BeginPlay()
{
	AActor::BeginPlay();
	StartExplosiveTimer();
}

void AExplosiveGrenade::ExplodeDamage()
{
	// Get the pawn that fired the projectile
	APawn* FiringPawn = GetInstigator();
	if (FiringPawn)
	{

		// Get the controller of the firing pawn
		AController* FiringController = FiringPawn->GetController();
		if (FiringController)
		{
			// Apply radial damage with falloff
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this,						// The current projectile object (context)
				ExplosiveDamage,			// The base damage value of the projectile
				MinimumRadialFallOffDamage,	// The minimum damage to apply
				GetActorLocation(),			// The location where the projectile hit
				DamageInnerRadius,			// The inner radius of the damage area
				DamageOuterRadius,			// The outer radius of the damage area
				DamageFallOff,				// The falloff value for the damage
				UDamageType::StaticClass(),	// The class of the damage type
				TArray<AActor*>(),			// An empty array of actors to ignore
				this,						// The object causing the damage (the projectile itself)
				FiringController			// The controller of the pawn that fired the projectile
			);
		}
	}
}

void AExplosiveGrenade::StartExplosiveTimer()
{
	GetWorldTimerManager().SetTimer(
		DestroyObjectTimer,
		this,
		&AExplosiveGrenade::ExplosiveTimerFinished,
		DestroyObjectTime
	);
}


void AExplosiveGrenade::ExplosiveTimerFinished()
{
	ExplodeDamage();
	// Spawn impact particles if defined
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}

	// Play impact sound if defined
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	
	Destroy();
}