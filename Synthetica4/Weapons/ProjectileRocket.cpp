// ©2023 Reverse-A. All rights reserved.


#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Synthetica/Components/RocketMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"

AProjectileRocket::AProjectileRocket()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileRocketMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovement"));
	RocketMovementComponent->bRotationFollowsVelocity = true;
}


void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();


	// Attach a dynamic function to the OnComponentHit event of the CollisionBox. When the collision occurs, it will call the OnHit function of the AProjectileRocket class.

	if (TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailSystem,							// The Niagara particle system to spawn
			GetRootComponent(),						// The component to which the system will be attached
			FName(),								// The socket or bone name to attach to (empty for root component)
			GetActorLocation(),						// The location where the system will be spawned
			GetActorRotation(),						// The rotation of the system
			EAttachLocation::KeepWorldPosition,		// The attachment rule
			false									// Whether to auto-destroy the system when it completes
		);
	}

	// Spawn and attach a Niagara particle system to the root component of the AProjectileRocket object. The system will be placed at the actor's location with its initial rotation. The attachment is set to keep the world position, and the system will not be auto-destroyed when it completes.

	if (ProjectileLoop && LoopingSoundAttenuation)
	{
		ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(
			ProjectileLoop,							// The sound to play
			GetRootComponent(),						// The component to which the sound will be attached
			FName(),								// The socket or bone name to attach to (empty for root component)
			GetActorLocation(),						// The location where the sound will be played
			EAttachLocation::KeepWorldPosition,			// The attachment rule
			false,									// Whether to follow the sound's spatialization rules
			1.f,									// The volume multiplier
			1.f,									// The pitch multiplier
			0.f,									// The start time of the sound in seconds (0 for the beginning)
			LoopingSoundAttenuation,					// The sound attenuation settings to apply
			(USoundConcurrency*) nullptr,			// The sound concurrency settings (nullptr for default)
			false									// Whether to override the concurrency settings
		);
	}

	// Spawn and attach a looping sound to the root component of the AProjectileRocket object. The sound will be played at the actor's location. The attachment is set to keep the world position. The sound's volume and pitch can be adjusted with the provided multipliers. The sound will start from the beginning (0 seconds) and use the specified attenuation settings. Concurrency settings can be overridden if needed.

	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileRocket::OnHit);

}


void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	if (OtherActor == GetOwner())
	{
		return;
	}

	ExplodeDamage();

	StartDestroyTimer();

	// Destroy the projectile object
	ProjectileDestroy();

	// Hide the rocket mesh
	if (ProjectileMesh)
	{
		ProjectileMesh->SetVisibility(false);
	}

	// Disable collision on the collision box
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Deactivate the trail particle system component
	if (TrailSystemComponent && TrailSystemComponent->GetSystemInstanceController())
	{
		TrailSystemComponent->GetSystemInstanceController()->Deactivate();
	}

	// Stop the looping sound component if it's currently playing
	if (ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
	{
		ProjectileLoopComponent->Stop();
	}
}