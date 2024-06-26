// ©2023 Reverse-A. All rights reserved.


#include "BaseThrowableGrenades.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Synthetica/Weapons/Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


ABaseThrowableGrenades::ABaseThrowableGrenades()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereCollision"));
	SetRootComponent(CapsuleCollisionComponent);

	ThrowableMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	ThrowableMesh->SetupAttachment(CapsuleCollisionComponent);

	GrenadesProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	GrenadesProjectileMovementComponent->bRotationFollowsVelocity = true;
	GrenadesProjectileMovementComponent->bShouldBounce = true;
}

void ABaseThrowableGrenades::BeginPlay()
{
	Super::BeginPlay();
	GrenadesProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ABaseThrowableGrenades::OnBounce);
}

void ABaseThrowableGrenades::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseThrowableGrenades::Destroyed()
{
	Super::Destroyed();
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}



void ABaseThrowableGrenades::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (GrenadeBounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			GrenadeBounceSound,
			GetActorLocation()
		);
	}
}

void ABaseThrowableGrenades::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(
		DestroyObjectTimer,
		this,
		&ABaseThrowableGrenades::DestroyTimerFinished,
		DestroyObjectTime
	);
}

void ABaseThrowableGrenades::DestroyTimerFinished()
{
	Destroy();
}

