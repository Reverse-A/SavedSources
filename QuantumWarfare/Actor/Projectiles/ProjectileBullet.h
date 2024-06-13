// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "ProjectileModule.h"
#include "ProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMWARFARE_API AProjectileBullet : public AProjectileModule
{
	GENERATED_BODY()
	
public:
	AProjectileBullet();

protected:
	virtual void OnBoxOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;
};
