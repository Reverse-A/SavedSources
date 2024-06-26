// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RangedWeaponTypes.h"
#include "RaycastWeapons.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ARaycastWeapons : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& HitTarget) override;

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);

	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

	UPROPERTY(EditAnywhere)
	float RaycastWeaponDamage = FMath::RandRange(15.f, 35.f);

	UPROPERTY(EditAnywhere)
	float SniperWeaponDamage = FMath::RandRange(300.f, 450.f);

	ERangedWeaponTypes RangedWeaponTypes;

private:
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, Category = "Bullet Spread")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Bullet Spread")
	float SphereRadius = 75.f;

	UPROPERTY(EditAnywhere, Category = "Bullet Spread")
	bool bUseScatter = false;
};
