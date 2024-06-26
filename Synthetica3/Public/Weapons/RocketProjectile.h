// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectile.h"
#include "RocketProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ARocketProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	ARocketProjectile();

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
