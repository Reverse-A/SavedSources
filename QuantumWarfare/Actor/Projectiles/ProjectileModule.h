// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "GameplayEffectTypes.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectileModule.generated.h"

class UBoxComponent;
class UParticleSystem;
class UProjectileMovementComponent;
class USoundBase;

UCLASS()
class QUANTUMWARFARE_API AProjectileModule : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileModule();
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnBoxOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UPROPERTY(EditAnywhere, Category = "ProjectileMovement|Properties")
	float ProjectileMovement_InitialSpeed = 550.f;

	UPROPERTY(EditAnywhere, Category = "ProjectileMovement|Properties")
	float ProjectileMovement_MaxSpeed = 550.0f;

	UPROPERTY(EditAnywhere, Category = "ProjectileMovement|Properties")
	float Projectile_GravityScale = 0.01f;

protected:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.0f;

	bool bHit = false;
	bool bOverlapExecuted = false;

	UPROPERTY(EditAnywhere, Category = "ProjectileProperties|VisualEffects")
	TObjectPtr<UParticleSystem> ImpactBodyEffect;

	UPROPERTY(EditAnywhere, Category = "ProjectileProperties|SoundEffects")
	TObjectPtr<USoundBase> ImpactBodySound;

	UPROPERTY(EditAnywhere, Category = "ProjectileProperties|VisualEffects")
	TObjectPtr<UParticleSystem> ImpactSurfaceEffect;

	UPROPERTY(EditAnywhere, Category = "ProjectileProperties|SoundEffects")
	TObjectPtr<USoundBase> ImpactSurfaceSound;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(EditAnywhere, Category = "ProjectileProperties|SoundEffects")
	TObjectPtr<USoundBase> LoopingSoundEffect;
};
