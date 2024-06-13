// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "BaseWeapon.h"
#include "MeleeWeapon.generated.h"

#pragma region Pre-called
class UBoxComponent;

#pragma endregion
#pragma region MeleeWeaponProperties
USTRUCT(BlueprintType)
struct FMeleeWeaponProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERarityCategory RarityCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMeleeWeaponType MeleeWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PenetrationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpecialEffects;

public:
	// Getters
	ERarityCategory GetRarityCategory()			const { return RarityCategory; }
	EMeleeWeaponType GetMeleeWeaponType()		const { return MeleeWeaponType; }
	float GetDamageValue()						const { return DamageValue; }
	float GetPenetrationValue()					const { return PenetrationValue; }
	float GetAttackTime()						const { return AttackTime; }
	float GetDurability()						const { return Durability; }
	float GetWeight()							const { return Weight; }
	float GetPrice()							const { return Price; }
	const TArray<AActor*>& GetSpecialEffects()	const { return SpecialEffects; }

	// Setters
	void SetRarityCategory(ERarityCategory InRarityCategory)		{ RarityCategory = InRarityCategory; }
	void SetMeleeWeaponType(EMeleeWeaponType InMeleeWeaponType)		{ MeleeWeaponType = InMeleeWeaponType; }
	void SetDamageValue(float InDamageValue)						{ DamageValue = InDamageValue; }
	void SetPenetrationValue(float InPenetrationValue)				{ PenetrationValue = InPenetrationValue; }
	void SetAttackTime(float InAttackTime)							{ AttackTime = InAttackTime; }
	void SetDurability(float InDurability)							{ Durability = InDurability; }
	void SetWeight(float InWeight)									{ Weight = InWeight; }
	void SetPrice(float InPrice)									{ Price = InPrice; }
	void SetSpecialEffects(const TArray<AActor*>& InSpecialEffects) { SpecialEffects = InSpecialEffects; }
};
#pragma endregion

UCLASS()
class ASCEND_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

#pragma region Weapon-Main
public:
	AMeleeWeapon();
	virtual void BeginPlay() override;

	void EditMeleeWeaponsByType();

#pragma endregion

#pragma region Class-Component-Structures
protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* DamageCollision;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DeveloperProperties)
    FMeleeWeaponProperties MeleeWeaponProperties;

public:
	FMeleeWeaponProperties GetMeleeWeaponProperties() const { return MeleeWeaponProperties; }
#pragma endregion

#pragma region WeaponOverlaps
private:
	float LastDamageTime = 0.0f;

protected:
	// Timer handle to manage the delay between consecutive damage dealing actions of the melee weapon
	FTimerHandle DamageTimerHandle;

	// Declare a boolean variable to track whether the weapon is currently dealing damage
	bool bDealDamageAgain = true;

	UFUNCTION()
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void ActivateWeaponOverlapDynamics(bool bActivate);
#pragma endregion
};
