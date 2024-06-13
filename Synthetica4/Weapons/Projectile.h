// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SYNTHETICA_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ProjectileSkeletalMesh;
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void ProjectileDestroy();
	void ExplodeDamage();


protected:
	virtual void BeginPlay() override;
	void SpawnTrailSystem();
	void StartDestroyTimer();
	void DestroyTimerFinished();

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere)
	float Damage = FMath::RandRange(20.f, 45.f);

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;

	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 500.f;

	UPROPERTY(EditAnywhere)
	float DamageOuterRadius = 250.f;

	UPROPERTY(EditAnywhere)
	float DamageFallOff = 150.f;

	UPROPERTY(EditAnywhere)
	float MinimumRadialFallOffDamage = 150.f;

private:


	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	class UParticleSystemComponent* TracerComponent;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 1.5f;

public:	


};
