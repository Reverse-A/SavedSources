// ©2023 Reverse-A. All rights reserved.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Synthetica/Interfaces/HitInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}
	if(HasAuthority())
	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileDestroy();
	
	// Check if the hit result has an actor associated with it
	if (Hit.GetActor())			
	{
		// Apply damage to the hit actor
		UGameplayStatics::ApplyDamage(				
			Hit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		// Check if the hit actor implements the IHitInterface
		IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor());
		if (HitInterface)
		{
			// Call the GetHit function on the hit actor
			HitInterface->GetHit(Hit.ImpactPoint);				
		}
	}
	Destroy();	// Destroy the projectile
}


void AProjectile::ProjectileDestroy()
{
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
}

void AProjectile::SpawnTrailSystem()
{
	if (TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailSystem,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			false
		);
	}
}

void AProjectile::ExplodeDamage()
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
				Damage,						// The base damage value of the projectile
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

void AProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(
		DestroyTimer,
		this,
		&AProjectile::DestroyTimerFinished,
		DestroyTime
	);
}

void AProjectile::DestroyTimerFinished()
{
	Destroy();
}