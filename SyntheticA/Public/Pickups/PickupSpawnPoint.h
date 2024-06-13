// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPoint.generated.h"

UCLASS()
class SYNTHETICA_API APickupSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupSpawnPoint();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)	
	TArray<TSubclassOf<class APickup>> PickupClasses;

	UPROPERTY()
	APickup* SpawnedPickup;

	void SpawnPickups();

	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);
	void SpawnPickupTimerFinished();

private:	
	FTimerHandle SpawnPickupTimer;

	UPROPERTY(EditAnywhere)	float SpawnPickupTimeMin;
	UPROPERTY(EditAnywhere)	float SpawnPickupTimeMax;

};