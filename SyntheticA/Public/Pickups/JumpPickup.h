// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "JumpPickup.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AJumpPickup : public APickup
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
	UPROPERTY(EditAnywhere)
	float JumpZVelocityIncrease = 1500.0f;

	UPROPERTY(EditAnywhere)
	float JumpIncreaseTime = 5.0f;
};
