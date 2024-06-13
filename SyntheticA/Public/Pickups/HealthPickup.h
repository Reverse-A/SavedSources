// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AHealthPickup : public APickup
{
	GENERATED_BODY()
	
public:
	AHealthPickup();

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


#pragma region Healing
private:
	UPROPERTY(EditAnywhere, Category = "ClassProperties")
	float HealingAmount = 100.0f;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")
	float HealingTime = 5.0f;

#pragma endregion

};
