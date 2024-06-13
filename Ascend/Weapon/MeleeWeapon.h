// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "BaseWeapon.h"
#include "MeleeWeapon.generated.h"

#pragma region Pre-called
class UBoxComponent;
class ABaseCharacter;
class AAIEntityModule;

#pragma endregion
USTRUCT(BlueprintType)
struct FMeleeWeaponProperties
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	ERarityCategory RarityCategory;

	UPROPERTY(EditAnywhere)
	EMeleeWeaponType MeleeWeaponType;

	UPROPERTY(VisibleAnywhere)
	float DamageValue;

	UPROPERTY(VisibleAnywhere)
	float PenetrationValue;

	UPROPERTY(VisibleAnywhere)
	float AttackTime;

	UPROPERTY(VisibleAnywhere)
	float Weight;

	UPROPERTY(VisibleAnywhere)
	float Durability;

	UPROPERTY(VisibleAnywhere)
	float Price;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SpecialEffects;

public:
	FMeleeWeaponProperties()
		:	RarityCategory(ERarityCategory::Common),
			MeleeWeaponType(EMeleeWeaponType::Default),
			DamageValue(0.0f),
			PenetrationValue(0.0f),
			AttackTime(0.0f),
			Weight(0.0f),
			Durability(0.0f),
			Price(0.0f)
	{
	}


	// Define a struct to hold weapon properties
	struct FWeaponProperties {
		float BaseDamage;
		float BasePenetration;
		float BaseAttackTime;
		float BaseDurability;
		float BasePrice;
		float BaseWeight;
	};

	// Define a map to store properties for each weapon type
	TMap<EMeleeWeaponType, FWeaponProperties> WeaponTypeProperties = 
	{
		{EMeleeWeaponType::Sword,       {25.0f, 1.0f, 1.5f, 1000.0f, 30.0f, 1200.0f}}, // Example weight value for Sword
		{EMeleeWeaponType::Katana,      {25.0f, 1.0f, 1.5f, 1000.0f, 30.0f, 1200.0f}}, // Example weight value for Katana
		{EMeleeWeaponType::Axe,         {30.0f, 1.2f, 2.0f, 1500.0f, 40.0f, 1600.0f}}, // Example weight value for Axe
		{EMeleeWeaponType::Mace,        {30.0f, 1.2f, 2.0f, 1500.0f, 40.0f, 2000.0f}}, // Example weight value for Mace
		{EMeleeWeaponType::Club,        {30.0f, 1.2f, 2.0f, 1500.0f, 40.0f, 1800.0f}}, // Example weight value for Club
		{EMeleeWeaponType::Spear,       {28.0f, 1.1f, 1.8f, 1200.0f, 35.0f, 2200.0f}}, // Example weight value for Spear
		{EMeleeWeaponType::Shield,      {10.0f, 0.5f, 2.0f, 2000.0f, 25.0f, 2500.0f}}, // Example weight value for Shield
		{EMeleeWeaponType::Whip,        {30.0f, 1.2f, 2.0f, 1500.0f, 40.0f, 1500.0f}}, // Example weight value for Whip
		{EMeleeWeaponType::Dagger,      {30.0f, 1.2f, 2.0f, 1500.0f, 40.0f, 1100.0f}}, // Example weight value for Dagger
		{EMeleeWeaponType::HiddenBlade, {30.0f, 1.2f, 2.0f, 1500.0f, 40.0f, 1050.0f}}  // Example weight value for HiddenBlade
	};

	// Define a map to store properties for each rarity category
	TMap<ERarityCategory, FWeaponProperties> RarityProperties =
	{
		{ERarityCategory::Common, {5.0f, 0.15f, 0.20f, 100.0f, 5.0f, 1000.0f}}, // Example weight value for Common
		{ERarityCategory::Rare,   {10.0f, 0.30f, 0.30f, 150.0f, 10.0f, 1100.0f}}, // Example weight value for Rare
		{ERarityCategory::Epic,   {15.0f, 0.45f, 0.45f, 200.0f, 15.0f, 1200.0f}}, // Example weight value for Epic
		{ERarityCategory::Unique, {20.0f, 0.55f, 0.60f, 250.0f, 20.0f, 1300.0f}}  // Example weight value for Unique
	};

	void UpdateWeaponProperties(EMeleeWeaponType WeaponType, ERarityCategory Rarity)
	{
		// Get base properties for the weapon type
		FWeaponProperties* BaseProperties = WeaponTypeProperties.Find(WeaponType);

		// Get additional properties based on rarity
		FWeaponProperties* RarityBonus = RarityProperties.Find(Rarity);

		if (BaseProperties && RarityBonus) // Ensure both pointers are valid
		{
			// Apply rarity bonus to base properties
			FWeaponProperties FinalProperties;
			FinalProperties.BaseDamage		= BaseProperties->BaseDamage		+ RarityBonus->BaseDamage;
			FinalProperties.BasePenetration = BaseProperties->BasePenetration	+ RarityBonus->BasePenetration;
			FinalProperties.BaseAttackTime	= BaseProperties->BaseAttackTime	- RarityBonus->BaseAttackTime;
			FinalProperties.BaseDurability	= BaseProperties->BaseDurability	+ RarityBonus->BaseDurability;
			FinalProperties.BasePrice		= BaseProperties->BasePrice			+ RarityBonus->BasePrice;
			FinalProperties.BaseWeight		= BaseProperties->BaseWeight		+ RarityBonus->BaseWeight;

			// Update weapon properties
			DamageValue						= FinalProperties.BaseDamage;
			PenetrationValue				= FinalProperties.BasePenetration;
			AttackTime						= FinalProperties.BaseAttackTime;
			Durability						= FinalProperties.BaseDurability;
			Price							= FinalProperties.BasePrice;
			Weight							= FinalProperties.BaseWeight;


			// Calculate the damage value by multiplying the base damage with the base penetration
			DamageValue						= FinalProperties.BaseDamage * FinalProperties.BasePenetration;
		}
	}

public:
	ERarityCategory GetRarityCategory() const
	{
		return RarityCategory;
	}

	EMeleeWeaponType GetMeleeWeaponType() const
	{
		return MeleeWeaponType;
	}

	float GetDamageValue() const
	{
		return DamageValue;
	}

	float GetPenetrationValue() const
	{
		return PenetrationValue;
	}

	float GetAttackTime() const
	{
		return AttackTime;
	}

	float GetDurability() const
	{
		return Durability;
	}

	float GetWeight() const
	{
		return Weight;
	}

	float GetPrice() const
	{
		return Price;
	}

	const TArray<AActor*>& GetSpecialEffects() const
	{
		return SpecialEffects;
	}

	// Setters
	void SetRarityCategory(ERarityCategory InRarityCategory)
	{
		RarityCategory = InRarityCategory;
	}

	void SetMeleeWeaponType(EMeleeWeaponType InMeleeWeaponType)
	{
		MeleeWeaponType = InMeleeWeaponType;
	}

	void SetDamageValue(float InDamageValue)
	{
		DamageValue = InDamageValue;
	}

	void SetPenetrationValue(float InPenetrationValue)
	{
		PenetrationValue = InPenetrationValue;
	}

	void SetAttackTime(float InAttackTime)
	{
		AttackTime = InAttackTime;
	}

	void SetDurability(float InDurability)
	{
		Durability = InDurability;
	}

	void SetWeight(float InWeight)
	{
		Weight = InWeight;
	}

	void SetPrice(float InPrice)
	{
		Price = InPrice;
	}

	void SetSpecialEffects(const TArray<AActor*>& InSpecialEffects)
	{
		SpecialEffects = InSpecialEffects;
	}
};

USTRUCT(BlueprintType)
struct FMeleeWeaponImages
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UTexture2D* SwordImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* KatanaImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* AxeImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* MaceImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* DaggerImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* ClubImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* SpearImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* WhipImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* ShieldImage;

	UPROPERTY(EditAnywhere)
	UTexture2D* HiddenBladeImage;

public:
	UTexture2D* GetSwordImage() const
	{
		return SwordImage;
	}

	UTexture2D* GetKatanaImage() const
	{
		return KatanaImage;
	}

	UTexture2D* GetAxeImage() const
	{
		return AxeImage;
	}

	UTexture2D* GetMaceImage() const
	{
		return MaceImage;
	}

	UTexture2D* GetDaggerImage() const
	{
		return DaggerImage;
	}

	UTexture2D* GetClubImage() const
	{
		return ClubImage;
	}

	UTexture2D* GetSpearImage() const
	{
		return SpearImage;
	}

	UTexture2D* GetWhipImage() const
	{
		return WhipImage;
	}

	UTexture2D* GetShieldImage() const
	{
		return ShieldImage;
	}

	UTexture2D* GetHiddenBladeImage() const
	{
		return HiddenBladeImage;
	}
};

UCLASS()
class ASCEND_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

#pragma region Main
public:
	AMeleeWeapon();
	virtual void BeginPlay() override;

	void InitialMeleeWeaponProperties();

#pragma endregion

#pragma region Class-Component-Structures
protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* DamageCollision;

	UPROPERTY(EditAnywhere, Category = "DevSettings|WeaponProperties")
    FMeleeWeaponProperties MeleeWeaponProperties;

	UPROPERTY(EditAnywhere, Category = "DevSettings|WeaponImages")
	FMeleeWeaponImages MeleeWeaponImages;

	UPROPERTY()
	AAIEntityModule* AIEntity;

public:
	FMeleeWeaponProperties GetMeleeWeaponProperties() const { return MeleeWeaponProperties; }

	void SetMeleeAIEntityOwner(AAIEntityModule* InAIEntity)
	{
		AIEntity = InAIEntity;
	}

#pragma endregion

#pragma region WeaponOverlaps
private:
	float LastDamageTime = 0.0f;

protected:
	// Timer handle to manage the delay between consecutive damage dealing actions of the melee weapon
	FTimerHandle DamageTimerHandle;

	// Declare a boolean variable to track whether the weapon is currently dealing damage
	bool bMeleeWeaponDamageAgain = true;

public:
	void SetMeleeWeaponDamageAgain(bool bDamageAgain)
	{
		bMeleeWeaponDamageAgain = bDamageAgain;
	}
public:
	UFUNCTION()
	virtual void OnBeginOverlapDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OverlapDamageEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void DeActivateDamageCollision();

public:
	void ActivateWeaponOverlapDynamics(bool bActivate);
#pragma endregion
};
