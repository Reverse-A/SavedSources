// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileModule.generated.h"

class ARangedWeapon;

class UBoxComponent;
class UParticleSystem;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USoundCue;
class UNiagaraSystem;
class UNiagaraComponent;


#define ENABLE_RANGE_WEAPON_TYPE_DAMAGE 0

UCLASS()
class ASCEND_API AProjectileModule : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileModule();

protected:
	virtual void BeginPlay() override;

protected:
	/*ProjectileMovement*/
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY()
	ARangedWeapon* RangedWeapon;

	UPROPERTY(EditAnywhere)
	float ProjectileInitialSpeed;

	UPROPERTY(EditAnywhere)
	float ProjectileMaxSpeed;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY()
	UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	UParticleSystem* ProjectileTracer;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	UParticleSystem* ImpactSurfaceParticles;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	USoundCue* ImpactSurfaceSound;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	UParticleSystem* ImpactBodyParticles;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	USoundCue* ImpactBodySound;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TrailSystem;

	UPROPERTY()
	UNiagaraComponent* TrailSystemComponent;

	UPROPERTY(EditAnywhere)
	float ExplosiveDamage   = FMath::FRandRange(50.0f, 100.0f);

	UPROPERTY(EditAnywhere)
	float InnerRadiusDamage = FMath::FRandRange(50.0f, 100.0f);

	UPROPERTY(EditAnywhere)
	float OuterRadiusDamage = FMath::FRandRange(50.0f, 100.0f);
private:
	UPROPERTY()
	AActor* OnHitDamagedActor = nullptr;

	UPROPERTY()
	float WeaponBaseDamage = 0.0f;

	UPROPERTY()
	AController* OnHitEventInstigator = nullptr;
public:
	virtual void ReceiveAndApplyDamageAmount(AActor* DamagedActor, float BaseDamage, AController* DamageSender);
protected:
	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.0f;

	FTimerHandle DestroyTimer;

	void SpawnTrailSystem();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	virtual void OnExplodeDamage();

	virtual void Destroyed() override;
	void StartDestroyTimer();
	void DestroyTimerFinished();



};
