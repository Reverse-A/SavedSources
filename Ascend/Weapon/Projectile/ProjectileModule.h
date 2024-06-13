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

UENUM(BlueprintType)
enum class ESurfaceType : uint8
{
	Default,
	Metal,
	Wood,
	Glass,
	Concrete,
	Dirt,
	Water,
	Stone,
	Flesh,
	Asphalt,
	Max // Optionally add Max for iterating through all surface types
	// Add more surface types as needed
};


#define ENABLE_RANGE_WEAPON_TYPE_DAMAGE 0

UCLASS()
class ASCEND_API AProjectileModule : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileModule();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESurfaceType SurfaceType;

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
private:
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


/*SFX, VFX*/
protected:
	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Metal;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Wood;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Glass;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Concrete;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Dirt;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Water;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Stone;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Flesh;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	UParticleSystem* SurfaceImpactParticle_Asphalt;


	/*Sound Effects*/
	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Metal;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Wood;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Glass;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Concrete;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Dirt;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Water;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Stone;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Flesh;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties")
	USoundCue* SurfaceImpactSound_Asphalt;

protected:
	void PlaySurfaceSoundEffect(ESurfaceType SelectSurfaceType);
	void SpawnSurfaceParticleEffects(ESurfaceType SelectSurfaceType);
};
