// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ascend/Weapon/WeaponStructureBase.h"
#include "ArsenalComponent.generated.h"

class ABaseCharacter;
class ABaseWeapon;
class AMeleeWeapon;
class ARangedWeapon;

class UPlayerInventory;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCEND_API UArsenalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UArsenalComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

public:
	void SetBaseCharacter(ABaseCharacter* GetBaseCharacter) { BaseCharacter = GetBaseCharacter; }

#pragma region Weapon-Classes
private:
	/* Arrays to store collected weapons, categorized by their respective class types:(AMeleeWeapon)*/
	UPROPERTY()
	TArray<AMeleeWeapon*>  MeleeWeapons;

	/* Arrays to store collected weapons, categorized by their respective class types:(ARangedWeapon)*/
	UPROPERTY()
	TArray<ARangedWeapon*> RangedWeapons;

	UPROPERTY()
	TArray<ABaseWeapon*> InventoryWeapons;

	// Pointer to the primary equipped weapon.
	UPROPERTY()
	ABaseWeapon* PrimaryEquipWeapon = nullptr;

	UPROPERTY()
	AMeleeWeapon* MeleeWeapon;

	UPROPERTY()
	ARangedWeapon* RangedWeapon;

public:
	ABaseWeapon* GetPrimaryEquipWeapon()   const { return PrimaryEquipWeapon; }

	TArray<AMeleeWeapon*>  GetMeleeWeaponsIndex()  const { return MeleeWeapons; }
	TArray<ARangedWeapon*> GetRangedWeaponsIndex() const { return RangedWeapons; }
	TArray<ABaseWeapon*> GetInventoryWeapons() { return InventoryWeapons; }

	AMeleeWeapon* GetMeleeWeapon() const { return MeleeWeapon; }
	ARangedWeapon* GetRangedWeapon() const { return RangedWeapon; }

public:
	void SelectWeaponTypes(ABaseWeapon* ArrangeableWeapon);
	void EquipMeleeWeaponOnGround(AMeleeWeapon* InsertMeleeWeapon);
	void EquipRangeWeaponOnGround(ARangedWeapon* InsertRangeWeapon);

	ERangedWeaponType GetRangeWeaponType();
private:
	void AttachWeaponToActor(ABaseWeapon* Weapon);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> PurchasableWeaponsList;

	void BuyWeapon(TSubclassOf<ABaseWeapon> InsertSubclassWeapon, USkeletalMeshComponent* SkeletalMesh);

public:
#pragma endregion

#pragma region Ammunition
private:
	TMap<ERangedWeaponType, int32> CarriedAmmunitionMap;

	UPROPERTY(EditAnywhere)
	int32 MAxCarriedAmmunition = 60.0f;

	int32 CarriedAmmunition;

	void InitializeCarriedAmmunition();

	void UpdateCarriedAmmunition();
	void Update_AmmunitionValue();
	void Update_AmmunitionValues_Shotgun();


	/*The initial ammunition quantity allocated for each category of weapons is contingent upon the specific type and model, with varying starting values designated based on the firearm's intended use and design specifications.*/
	UPROPERTY(EditAnywhere)
	int32 InitialBowAmmunition				= 24;

	UPROPERTY(EditAnywhere)
	int32 InitialCrossBowAmmunition			= 16;

	UPROPERTY(EditAnywhere)
	int32 InitialHeavyMachineAmmunition		= 120;

	UPROPERTY(EditAnywhere)
	int32 InitialCannonAmmunition			= 4;

	UPROPERTY(EditAnywhere)
	int32 InitialLauncherAmmunition			= 4;
		
	UPROPERTY(EditAnywhere)
	int32 InitialRifleAmmunition			= 60;

	UPROPERTY(EditAnywhere)
	int32 InitialPistolAmmunition			= 32;

	UPROPERTY(EditAnywhere)
	int32 InitialSubMachineGunAmmunition	= 40;

	UPROPERTY(EditAnywhere)
	int32 InitialShotgunWeaponAmmunition	= 16;

	UPROPERTY(EditAnywhere)
	int32 InitialSniperRifleAmmunition		= 8;

	UPROPERTY(EditAnywhere)
	int32 InitialThrowingAmmunition			= 8;
#pragma endregion
};
