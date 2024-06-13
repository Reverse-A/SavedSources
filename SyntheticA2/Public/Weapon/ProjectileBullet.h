// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()

public:
	AProjectileBullet();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;
};
