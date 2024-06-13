// @2023 All rights reversed by Reverse-Alpha Studios


#include "ProjectileModule.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

#include "Ascend/Character/BaseCharacter.h"
#include "Ascend/Weapon/BaseWeapon.h"
#include "Ascend/AI/AIEntityModule.h"

#include "PhysicalMaterials/PhysicalMaterial.h"


AProjectileModule::AProjectileModule()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionBox);

	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
}


void AProjectileModule::BeginPlay()
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

	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileModule::OnHit);
}



void AProjectileModule::SpawnTrailSystem()
{
	if (TrailSystem)
	{
		TrailSystemComponent = 
		UNiagaraFunctionLibrary::SpawnSystemAttached
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


void AProjectileModule::ReceiveAndApplyDamageAmount(AActor* DamagedActor, float BaseDamage, AController* DamageSender)
{
	OnHitDamagedActor		= DamagedActor;
	WeaponBaseDamage		= BaseDamage;
	OnHitEventInstigator	= DamageSender;

	UGameplayStatics::ApplyDamage
	(
		OnHitDamagedActor,
		WeaponBaseDamage,
		OnHitEventInstigator,
		this,
		UDamageType::StaticClass()
	);
}

void AProjectileModule::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (OtherActor->IsA<ABaseCharacter>() || OtherActor->IsA<AAIEntityModule>())
		{
			PlaySurfaceSoundEffect(ESurfaceType::Flesh);
			SpawnSurfaceParticleEffects(ESurfaceType::Flesh);
		}
		else
		{
			// Define a map to associate actor tags with surface types
			TMap<FName, ESurfaceType> SurfaceTypeMap;

			SurfaceTypeMap.Add("Metal",		ESurfaceType::Metal);
			SurfaceTypeMap.Add("Wood",		ESurfaceType::Wood);
			SurfaceTypeMap.Add("Glass",		ESurfaceType::Glass);
			SurfaceTypeMap.Add("Concrete",	ESurfaceType::Concrete);
			SurfaceTypeMap.Add("Dirt",		ESurfaceType::Dirt);
			SurfaceTypeMap.Add("Water",		ESurfaceType::Water);
			SurfaceTypeMap.Add("Stone",		ESurfaceType::Stone);
			SurfaceTypeMap.Add("Flesh",		ESurfaceType::Flesh);
			SurfaceTypeMap.Add("Asphalt",	ESurfaceType::Asphalt);

			// Default surface type
			SurfaceType = ESurfaceType::Default;

			// Check if OtherActor has any matching tags in the map
			for (const auto& TagSurfacePair : SurfaceTypeMap)
			{
				if (OtherActor->ActorHasTag(TagSurfacePair.Key))
				{
					SurfaceType = TagSurfacePair.Value;
					break;
					// Exit loop if a matching tag is found
				}
			}
			// Play sound effect and spawn particle effects based on the determined surface type
			PlaySurfaceSoundEffect(SurfaceType);
			SpawnSurfaceParticleEffects(SurfaceType);
		}
	}

	ReceiveAndApplyDamageAmount(OnHitDamagedActor, WeaponBaseDamage, OnHitEventInstigator);
	Destroy();
}

void AProjectileModule::OnExplodeDamage()
{
	UGameplayStatics::ApplyRadialDamageWithFalloff
	(
		this,
		ExplosiveDamage,
		10.0f,
		GetActorLocation(),
		InnerRadiusDamage,
		OuterRadiusDamage,
		1.0f,
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this,
		GetInstigatorController()
	);
}

void AProjectileModule::Destroyed()
{
	Super::Destroyed();
}

void AProjectileModule::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer
	(
		DestroyTimer,
		this,
		&AProjectileModule::DestroyTimerFinished,
		DestroyTime
	);
}

void AProjectileModule::DestroyTimerFinished()
{
	Destroy();
}


void AProjectileModule::SpawnSurfaceParticleEffects(ESurfaceType SelectSurfaceType)
{
	switch (SelectSurfaceType)
	{
		case ESurfaceType::Metal:
			if (SurfaceImpactParticle_Metal)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Metal,
					GetActorLocation()
				);
			}
				break;

		case ESurfaceType::Wood:
			if (SurfaceImpactParticle_Wood)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Wood,
					GetActorLocation()
				);
			}
				break;

		case ESurfaceType::Glass:
			if (SurfaceImpactParticle_Glass)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Glass,
					GetActorLocation()
				);
			}
				break;

		case ESurfaceType::Concrete:
			if (SurfaceImpactParticle_Concrete)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Concrete,
					GetActorLocation()
				);
			}
				break;

		case ESurfaceType::Dirt:
			if (SurfaceImpactParticle_Dirt)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Dirt,
					GetActorLocation()
				);
			}
				break;

		case ESurfaceType::Water:
			if (SurfaceImpactParticle_Water)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Water,
					GetActorLocation()
				);
			}
				break;

		case ESurfaceType::Stone:
			if (SurfaceImpactParticle_Stone)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Stone,
					GetActorLocation()
				);
			}
				break;

		case ESurfaceType::Flesh:
			if (SurfaceImpactParticle_Flesh)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Flesh,
					GetActorLocation()
				);
			}
				break;

		case ESurfaceType::Asphalt:
			if (SurfaceImpactParticle_Asphalt)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					SurfaceImpactParticle_Asphalt,
					GetActorLocation()
				);
			}
				break;

		default:
			UE_LOG
			(
				LogTemp,
				Warning,
				TEXT("Unhandled surface type encountered")
			);
				break;
	}
}

void AProjectileModule::PlaySurfaceSoundEffect(ESurfaceType SelectSurfaceType)
{
	switch (SelectSurfaceType)
	{
		case ESurfaceType::Metal:
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				SurfaceImpactSound_Metal,
				GetActorLocation()
			);
			break;

		case ESurfaceType::Wood:
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				SurfaceImpactSound_Wood,
				GetActorLocation()
			);
			break;

		case ESurfaceType::Glass:
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				SurfaceImpactSound_Glass,
				GetActorLocation()
			);
			break;

		case ESurfaceType::Concrete:
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				SurfaceImpactSound_Concrete,
				GetActorLocation()
			);
			break;

		case ESurfaceType::Dirt:
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				SurfaceImpactSound_Dirt,
				GetActorLocation()
			);
			break;

		case ESurfaceType::Water:
			UGameplayStatics::PlaySoundAtLocation
			(
				this, 
				SurfaceImpactSound_Water,
				GetActorLocation()
			);
			break;

		case ESurfaceType::Stone:
			UGameplayStatics::PlaySoundAtLocation(
				this, 
				SurfaceImpactSound_Stone, 
				GetActorLocation()
			);
			break;

		case ESurfaceType::Flesh:
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				SurfaceImpactSound_Flesh,
				GetActorLocation()
			);
			break;

		case ESurfaceType::Asphalt:
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				SurfaceImpactSound_Asphalt,
				GetActorLocation()
			);
			break;
		// Add cases for additional surface types as needed
	}
}