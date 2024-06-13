// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 *
 */
UCLASS()
class SYNTHETICA_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& HitTarget) override;

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);

	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

protected:
	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	USoundCue* HitSound;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float Damage = 20.f;

private:


	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	USoundCue* FireSound;

	/*
	* Trace end with scatter
	*/

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	bool bUseScatter = false;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float DistanceToSphere = 800.0f;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float SphereRadius = 75.0f;
};
