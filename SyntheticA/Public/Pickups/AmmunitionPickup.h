// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "Weapon/WeaponTypes.h"
#include "AmmunitionPickup.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AAmmunitionPickup : public APickup
{
	GENERATED_BODY()
	

protected:
	virtual void OnSphereOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

private:
	UPROPERTY(EditAnywhere)
	int32 AmmunitionAmount = 30;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
};
