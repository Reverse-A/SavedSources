
// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileRocket();

	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


private:
	UPROPERTY(VisibleAnywhere)
	class URocketMovementComponent* RocketMovementComponent;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	USoundCue* RocketProjectileLoop;

	UPROPERTY()
	UAudioComponent* RocketProjectileLoopComponent;
	
	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	USoundAttenuation* RocketProjectileLoopingSoundAttenuation;

public:
	FORCEINLINE URocketMovementComponent* GetRocketMovementComponent() const { return RocketMovementComponent; }
};
