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

#pragma region Weapon
private:
	/* Arrays to store collected weapons, categorized by their respective class types:(AMeleeWeapon)*/
	UPROPERTY()
	TArray<AMeleeWeapon*>  MeleeWeapons;

	/* Arrays to store collected weapons, categorized by their respective class types:(ARangedWeapon)*/
	UPROPERTY()
	TArray<ARangedWeapon*> RangedWeapons;

	// Pointer to the primary equipped weapon.
	UPROPERTY()
	ABaseWeapon* PrimaryEquipWeapon = nullptr;

	// Pointer to the secondary equipped weapon.
	UPROPERTY()
	ABaseWeapon* SecondaryEquipWeapon = nullptr;

	UPROPERTY()
	AMeleeWeapon* MeleeWeapon;

	UPROPERTY()
	ARangedWeapon* RangedWeapon;

public:
	ABaseWeapon* GetPrimaryEquipWeapon()   const { return PrimaryEquipWeapon; }
	ABaseWeapon* GetSecondaryEquipWeapon() const { return SecondaryEquipWeapon; }

	TArray<AMeleeWeapon*>  GetMeleeWeaponsIndex()  const { return MeleeWeapons; }
	TArray<ARangedWeapon*> GetRangedWeaponsIndex() const { return RangedWeapons; }

	AMeleeWeapon* GetMeleeWeapon() const { return MeleeWeapon; }
	ARangedWeapon* GetRangedWeapon() const { return RangedWeapon; }

public:
	void ArrangeWeaponByTypesAndClasses(ABaseWeapon* ArrangeableWeapon);
	void EquipMeleeWeaponOnGround(AMeleeWeapon* InsertMeleeWeapon);
	void EquipRangeWeaponOnGround(ARangedWeapon* InsertRangeWeapon);

private:
	void AttachWeaponToActor(ABaseWeapon* Weapon);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> PurchasableWeaponsList;

	void BuyWeapon(TSubclassOf<ABaseWeapon> InsertSubclassWeapon, USkeletalMeshComponent* SkeletalMesh);

public:

#pragma endregion
};
