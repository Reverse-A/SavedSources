// @2023 All rights reversed by Reverse-Alpha Studios


#include "ProjectileModule.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include <QuantumWarfare/QuantumWarfare.h>
#include <QuantumWarfare/Character/EnemyCharacter.h>
AProjectileModule::AProjectileModule()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);

	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionObjectType(ECC_Projectile);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed			= ProjectileMovement_InitialSpeed;
	ProjectileMovement->MaxSpeed				= ProjectileMovement_MaxSpeed;
	ProjectileMovement->ProjectileGravityScale	= Projectile_GravityScale;
}

void AProjectileModule::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(8.0f);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileModule::OnBoxOverlap);
}

void AProjectileModule::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		if (ImpactBodySound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation
			(
				this,
				ImpactBodySound.Get(),
				GetActorLocation(),
				FRotator::ZeroRotator
			);
		}

		if (ImpactBodyEffect != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation
			(
				GetWorld(),
				ImpactBodyEffect.Get(),
				GetActorLocation(),
				FRotator::ZeroRotator
			);
		}
	}
	Super::Destroyed();
}

void AProjectileModule::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamageEffectSpecHandle.Data.IsValid() && DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}

	if (bOverlapExecuted == false)
	{
		if (OtherActor->IsA<AEnemyCharacter>())
		{
			if (ImpactBodySound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation
				(
					this,
					ImpactBodySound.Get(),
					GetActorLocation(),
					FRotator::ZeroRotator
				);
			}

			if (ImpactBodyEffect != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					ImpactBodyEffect.Get(),
					GetActorTransform()
				);
			}

			if (HasAuthority())
			{
				if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
				{
					TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
				}
				Destroy();
			}
			else
			{
				bHit = true;
			}
		}
		else if (OtherActor->IsA<UStaticMeshComponent>())
		{
			if (ImpactSurfaceSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation
				(
					this,
					ImpactSurfaceSound.Get(),
					GetActorLocation(),
					FRotator::ZeroRotator
				);
			}

			if (ImpactSurfaceEffect != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					ImpactSurfaceEffect.Get(),
					GetActorLocation(),
					FRotator::ZeroRotator
				);
			}
		}
		else if (OtherActor->IsA<APlayerCharacter>())
		{
			if (ImpactBodySound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation
				(
					this,
					ImpactBodySound.Get(),
					GetActorLocation(),
					FRotator::ZeroRotator
				);
			}

			if (ImpactBodyEffect != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation
				(
					GetWorld(),
					ImpactBodyEffect.Get(),
					GetActorTransform()
				);
			}

			if (HasAuthority())
			{
				if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
				{
					TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
				}
				Destroy();
			}
			else
			{
				bHit = true;
			}
		}
		bOverlapExecuted = true;
	}
}