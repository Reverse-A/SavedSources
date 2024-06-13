// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

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

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(VisibleAnywhere)
	class URocketMovementComponent* RocketMovementComponent;

	UPROPERTY(EditAnywhere)
	USoundCue* ProjectileSoundLoop;

	UPROPERTY()
	UAudioComponent* ProjectileSoundLoopComponent;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* LoopingSoundAttenuation;
};
