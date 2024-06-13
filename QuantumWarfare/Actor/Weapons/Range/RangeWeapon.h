// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "QuantumWarfare/Actor/Weapons/WeaponModule.h"
#include "RangeWeapon.generated.h"



UCLASS()
class QUANTUMWARFARE_API ARangeWeapon : public AWeaponModule
{
	GENERATED_BODY()
	
public:
	ARangeWeapon();

protected:
	UPROPERTY(EditAnywhere, Category = "RangeWeaponProperties|VisualEffects")
	TObjectPtr<UParticleSystem> MuzzleFlash;

	// Function created for future use 
	UFUNCTION(BlueprintCallable, Category = "RangeWeaponProperties|Functions")
	void SpawnMuzzleFlashEmitter(FVector MuzzleSocketLocation);
};
