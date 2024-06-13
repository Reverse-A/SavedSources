// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "ProjectileModule.h"
#include "ProjectileBullet.generated.h"

UCLASS()
class ASCEND_API AProjectileBullet : public AProjectileModule
{
	GENERATED_BODY()
	
public:
	AProjectileBullet();

public:
	virtual void OnHit
	(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;
};