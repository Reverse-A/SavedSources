// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
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
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
