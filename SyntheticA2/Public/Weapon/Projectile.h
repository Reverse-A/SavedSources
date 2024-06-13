// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;
	void StartDestroyTimer();
	void DestroyTimerFinished();
	void SpawnTrailSystem();
	void ExplodeDamage();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;

	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	UParticleSystem* ProjectileTracer;

	class UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float InnerRadiusDamage = 200.f;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float OuterRadiusDamage = 500.f;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float FalloffDamage = 1.0f;

private:
	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float DestroyTime = 3.0f;

};
