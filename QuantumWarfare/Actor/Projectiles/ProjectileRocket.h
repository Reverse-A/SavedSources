// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "ProjectileModule.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMWARFARE_API AProjectileRocket : public AProjectileModule
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

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
