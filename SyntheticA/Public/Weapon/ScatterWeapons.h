// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Weapon/HitScanWeapon.h"
#include "ScatterWeapons.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AScatterWeapons : public AHitScanWeapon
{
	GENERATED_BODY()
	
public:
	virtual void FireScatterWeapons(const TArray<FVector_NetQuantize>& HitTargets);

	void ScatterTypeWeaponsTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets);

private:
	UPROPERTY(EditAnywhere, Category = "WeaponSettings/ScatterTypes")
	uint32 NumberOfPellets = 7;


private:
	float TotalVehicleDamageAmount;
public:
	float GetVehicleTotalDamageAmountWithScatters();
};
