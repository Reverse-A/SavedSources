// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <QuantumWarfare/Interaction/CombatInterface.h>
#include "WeaponModule.generated.h"

/*Actors*/
class APlayerCharacter;

class AMeleeWeapon;
class ARangeWeapon;

/*Objects*/
class USphereComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class ERarityCategory : uint8
{
	Common,
	Rare,
	Epic,
	Unique
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Purchasable,
	Acquired,
	Dropped,
	Destroyed,
	Sold,
	StateMax
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Initial,
	CloseRange,
	LongRange
};

USTRUCT(BlueprintType)
struct FWeaponProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERarityCategory RarityCategory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

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

	FWeaponProperties()
	{
		InitializeDefaults();
	}

public:
	void InitializeDefaults()
	{
		RarityCategory   = ERarityCategory::Common;
		WeaponState		 = EWeaponState::Purchasable;
		WeaponType		 = EWeaponType::Initial;
		DamageValue		 = 0.0f;
		PenetrationValue = 0.0f;
		AttackTime		 = 0.0f;
		Durability		 = 0.0f;
		Weight			 = 0.0f;
		Price			 = 0.0f;
	}

public:
	/*Getters*/
	ERarityCategory GetRarityCategory()			const { return RarityCategory; }
	EWeaponState	GetWeaponState()			const { return WeaponState; }
	EWeaponType		GetWeaponType()				const { return WeaponType; }
	float			GetDamageValue()			const { return DamageValue; }
	float			GetPenetrationValue()		const { return PenetrationValue; }
	float			GetAttackTime()				const { return AttackTime; }
	float			GetDurability()				const { return Durability; }
	float			GetWeight()					const { return Weight; }
	float			GetPrice()					const { return Price; }

	/*Setters*/
	void SetRarityCategory(ERarityCategory InRarityCategory) { RarityCategory = InRarityCategory; }
	void SetWeaponState(EWeaponState InWeaponState)			 { WeaponState = InWeaponState; }
	void SetWeaponType(EWeaponType InWeaponType)			 { WeaponType = InWeaponType; }
	void SetDamageValue(float InDamageValue)				 { DamageValue = InDamageValue; }
	void SetPenetrationValue(float InPenetrationValue)		 { PenetrationValue = InPenetrationValue; }
	void SetAttackTime(float InAttackTime)					 { AttackTime = InAttackTime; }
	void SetDurability(float InDurability)					 { Durability = InDurability; }
	void SetWeight(float InWeight)							 { Weight = InWeight; }
	void SetPrice(float InPrice)							 { Price = InPrice; }
};

UCLASS(Abstract)
class QUANTUMWARFARE_API AWeaponModule : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponModule();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;

public:
	UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
#pragma region Overlap
protected:
	UFUNCTION()
	void OnSphereBeginOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// Function to handle when something ends overlapping with the sphere
	UFUNCTION()
	void OnSphereEndOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
#pragma endregion

protected:
	UPROPERTY(EditDefaultsOnly, Category = "WeaponProperties|Structure")
	FWeaponProperties WeaponProperties;

#pragma region WeaponState Functions
private:
	void WeaponStatement(); 
	void WeaponStatePurchasable();
	void WeaponStateAcquired();
	void WeaponStateDropped();
	void WeaponStateDestroyed();
	void WeaponStateSold();

public:
	void SetWeaponState(EWeaponState NewState);
#pragma endregion
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> KatanaDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> AssaultRiffleDissolveMaterialInstance;

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);
};
