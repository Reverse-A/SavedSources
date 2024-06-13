// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArsenalComponent.generated.h"

class ACharacterModule;
class AWeaponModule;
class AMeleeWeapon;
class ARangeWeapon;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUANTUMWARFARE_API UArsenalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UArsenalComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY() ACharacterModule* CharacterModule = nullptr;
	UPROPERTY() APlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY()	AWeaponModule* OverlapWeapon = nullptr;
	UPROPERTY()	AMeleeWeapon*  MeleeWeapon	 = nullptr;
	UPROPERTY()	ARangeWeapon*  RangeWeapon	 = nullptr;

	UPROPERTY()	AWeaponModule* EquippedWeapon = nullptr;

public:
	AWeaponModule* GetEquippedWeapon() const { return EquippedWeapon; }

	FVector GetWeaponMuzzleFlashSocketLocation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawnRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMeleeWeapon> Katana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARangeWeapon> AssaultRifle;

	void SpawnWeapon();
	void DropWeapon();
	void DestroyEnemyWeapon();
	void DestroyEquippedWeapon();

	FTimerHandle DestroyTimerHandler;
public:
	void SetPlayerCharacter(APlayerCharacter* InPC);
	void SetModuleCharacter(ACharacterModule* InModCharacter);
	void SetOverlappingWeapon(AWeaponModule* InWeapon);

	AWeaponModule* GetOverlapWeapon() const { return OverlapWeapon; }
	/*CustomRender*/
public:
	void SetEquippedWeaponRenderCustomDepth(bool InValue = false);

public:
	void SetWeaponDissolveDynamicMaterialInstance(UMaterialInstanceDynamic* DynamicMatInst);
};
