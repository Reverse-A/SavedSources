// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseThrowableGrenades.h"
#include "ExplosiveGrenade.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AExplosiveGrenade : public ABaseThrowableGrenades
{
	GENERATED_BODY()

public:
	AExplosiveGrenade();
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;


	// Explosive objects
	void ExplodeDamage();

	void StartExplosiveTimer();
	void ExplosiveTimerFinished();

	UPROPERTY(EditAnywhere)
	float ExplosiveDamage = FMath::RandRange(20.f, 45.f);

	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 500.f;

	UPROPERTY(EditAnywhere)
	float DamageOuterRadius = 250.f;

	UPROPERTY(EditAnywhere)
	float DamageFallOff = 150.f;

	UPROPERTY(EditAnywhere)
	float MinimumRadialFallOffDamage = 150.f;
};
