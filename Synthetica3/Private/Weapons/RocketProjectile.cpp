// ©2023 Reverse - A Company. All rights reserved.


#include "Weapons/RocketProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystemInstance.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "Weapons/RocketMovementComponent.h"

ARocketProjectile::ARocketProjectile()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComponent"));
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(true);
}



void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &ARocketProjectile::OnHit);
	}

	SpawnTrailSystem();

	if (ProjectileSoundLoop && LoopingSoundAttenuation)
	{
		ProjectileSoundLoopComponent = UGameplayStatics::SpawnSoundAttached
		(
			ProjectileSoundLoop,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			EAttachLocation::KeepWorldPosition,
			false,
			1.0f,
			1.0f,
			0.0f,
			LoopingSoundAttenuation,
			(USoundConcurrency*) nullptr,
			false
		);
	}
}

void ARocketProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
	{
		return;
	}
	ExplodeDamage();

	StartDestroyTimer();

	if (ImpactParticles)
	{
		// Spawn particle effects at the location of the hit.
		UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			ImpactParticles,
			GetActorTransform()
		);
	}
	// Check if "ImpactSound" is set.
	if (ImpactSound)
	{
		// Play a sound at the location of the hit.
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			ImpactSound,
			GetActorLocation()
		);
	}
	if (ProjectileMesh)
	{
		ProjectileMesh->SetVisibility(false);
	}
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (TrailSystemComponent && TrailSystemComponent->GetSystemInstance())
	{
		TrailSystemComponent->GetSystemInstance()->Deactivate();
	}
	if (ProjectileSoundLoopComponent && ProjectileSoundLoopComponent->IsPlaying())
	{
		ProjectileSoundLoopComponent->Stop();
	}
}

void ARocketProjectile::Destroyed()
{

}