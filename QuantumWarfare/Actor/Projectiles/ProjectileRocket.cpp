// @2023 All rights reversed by Reverse-Alpha Studios


#include "ProjectileRocket.h"

void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	if (LoopingSoundEffect != nullptr)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached
		(
			LoopingSoundEffect,
			GetRootComponent()
		);
	}
}

void AProjectileRocket::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		LoopingSoundComponent->Stop();
	}
	Super::Destroyed();
}

void AProjectileRocket::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (LoopingSoundComponent != nullptr)
	{
		LoopingSoundComponent->Stop();
	}
}
