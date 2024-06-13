// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "ShieldPickup.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AShieldPickup : public APickup
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


#pragma region Shield Regeneration
private:
	UPROPERTY(EditAnywhere, Category = "ClassProperties")	float ShieldAmount			 = 100.0f;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")	float ShieldRegenerationTime = 5.0f;

#pragma endregion
};
