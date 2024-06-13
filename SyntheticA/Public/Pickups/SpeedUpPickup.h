// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "SpeedUpPickup.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ASpeedUpPickup : public APickup
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
	);

private:

	UPROPERTY(EditAnywhere, Category = "ClassProperties")	float WalkSpeedIncreaseValue	= 3000.0f;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")	float CrouchSpeedIncreaseValue	= 400.0f;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")	float SpeedIncrementTime		= 5.0f;
};
