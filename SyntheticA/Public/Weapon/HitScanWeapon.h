// @2023 All rights reversed by Reverse-Alpha Studios

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
	virtual float CalculateDamageForVehicles(float DamageAmount);

protected:
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	USoundCue* HitSound;

private:

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	USoundCue* FireSound;
};
