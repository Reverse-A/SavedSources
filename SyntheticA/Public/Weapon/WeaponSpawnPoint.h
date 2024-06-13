// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSpawnPoint.generated.h"

UCLASS()
class SYNTHETICA_API AWeaponSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponSpawnPoint();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")
	TArray<TSubclassOf<class AWeapon>> WeaponClasses;
	
	UPROPERTY()
	AWeapon* SpawnedWeapon;

	void SpawnWeapons();

	UFUNCTION() 
	void StartSpawnWeaponTimer(AActor* DestroyedActor);

	void SpawnWeaponTimerFinished();

private:
	FTimerHandle SpawnWeaponTimer;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")	float SpawnWeaponTimeMin;
	UPROPERTY(EditAnywhere, Category = "ClassProperties")	float SpawnWeaponTimeMax;
};
