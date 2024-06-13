// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|


#include "Weapon/Projectile.h"

#include "Components/BoxComponent.h"
//#include "Components/CombatComponent.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionBox);

	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileTracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached
		(
			ProjectileTracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}
	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectile::SpawnTrailSystem()
{
	if (TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached
		(
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
	APawn* InstigatorPawn = GetInstigator();
	if (InstigatorPawn && HasAuthority())
	{
		AController* InstigatorController = InstigatorPawn->GetController();
		if (InstigatorController)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff
			(
				this,
				Damage,
				10.0f,
				GetActorLocation(),
				InnerRadiusDamage,
				OuterRadiusDamage,
				FalloffDamage,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				this,
				InstigatorController
			);
		}
	}
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	// Check if "ImpactParticles" is set
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
}

void AProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer
	(
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