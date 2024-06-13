/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <Data/AttainmentData.h>
#include <Data/AmmunitionTypeData.h>
#include <Data/FireTypeData.h>
#include <Data/FireModeData.h>
#include <Data/RarityData.h>
#include <Data/WeaponStateData.h>
#include <Data/WeaponTypeData.h>

#include <Data/ShortRangeWeaponTypeData.h>
#include <Data/LongRangeWeaponTypeData.h>

#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponDamageType : uint8
{
	Select		  UMETA(DisplayName = "Select"),
	SingleDamage  UMETA(DisplayName = "Single Damage"),
	RandomDamage  UMETA(DisplayName = "Random Damage")
};

USTRUCT(BlueprintType)
struct FWeaponInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	EAttainmentType			WeaponAttainmentType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	EWeaponState			WeaponStateType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	EWeaponType				WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	ERarityEffectProcess	RarityEffectProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	ERarityType				WeaponRarityType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	EWeaponDamageType		WeaponDamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	int32					Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	int32					RandomDamageMin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	int32					RandomDamageMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					Penetration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					Range;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	ELongRangeWeaponType	LongRangeWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	EAmmunitionType			AmmunitionType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	EFireType				FireType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	bool					bFullAutomatic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	EFireModeType			FireModeType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					Accuracy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	int32					MagazineCapacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					FireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					ReloadTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	EShortRangeWeaponType	ShortRangeWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					AttackSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					Weight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					Durability;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData|WeaponInformation")
	float					Price;

public:
	FWeaponInformation()
	: WeaponAttainmentType		(EAttainmentType::Select)
	, WeaponStateType			(EWeaponState::Select)
	, WeaponType				(EWeaponType::Select)
	, RarityEffectProcess		(ERarityEffectProcess::Select)
	, WeaponRarityType			(ERarityType::Select)
	, WeaponDamageType			(EWeaponDamageType::Select)
	, Damage					(0)
	, RandomDamageMin			(0)
	, RandomDamageMax			(0)
	, Penetration				(0.0f)
	, Range						(0.0f)
	, LongRangeWeaponType		(ELongRangeWeaponType::Select)
	, AmmunitionType			(EAmmunitionType::Select)
	, FireType					(EFireType::Select)
	, bFullAutomatic			(false)
	, FireModeType				(EFireModeType::Select)
	, Accuracy					(0.0f)
	, MagazineCapacity			(0)
	, FireRate					(0.0f)
	, ReloadTime				(0.0f)
	, ShortRangeWeaponType		(EShortRangeWeaponType::Select)
	, AttackSpeed				(0.0f)
	, Weight					(0.0f)
	, Durability				(0.0f)
	, Price						(0.0f)
	{}
};

USTRUCT(BlueprintType)
struct FWeaponInformationStrings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponAttainmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponStateType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString RarityEffectProcess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponRarityTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString DamageTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponRandomDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponPenetration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString LongRangeWeaponTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString ShortRangeWeaponTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponAmmunitionTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString FireType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString bIsFullAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString FireModeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponAccuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FString WeaponName;
};

UCLASS(BlueprintType)
class WEAPONMASTERY_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()

#pragma region Structure Objects
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponInformation")
	FWeaponInformation WeaponInformation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponInformation|String")
	FWeaponInformationStrings WeaponInformationStrings;


public:

	FORCEINLINE FWeaponInformation GetWeaponInformation()
	{
		return WeaponInformation;
	}

	FORCEINLINE FWeaponInformationStrings GetWeaponInformationStrings()
	{
		return WeaponInformationStrings;
	}
#pragma endregion

	void UpdateWeaponAttainmentType			(EAttainmentType InWeaponAttainmentType);
	void UpdateWeaponAttainmentTypeAsString (FString InWeaponAttainmentTypeAsString);

	void UpdateWeaponStateType				(EWeaponState InWeaponStateType);
	void UpdateWeaponStateTypeAsString		(FString InWeaponStateTypeAsString);

	void UpdateWeaponType					(EWeaponType InWeaponType);
	void UpdateWeaponTypeAsString			(FString InWeaponTypeAsString);

	void UpdateRarityEffectProcess			(ERarityEffectProcess InRarityEffectProcess);
	void UpdateRarityEffectProcessAsString	(FString InRarityEffectProcessAsString);

	void UpdateWeaponRarityType				(ERarityType InWeaponRarityType);
	void UpdateWeaponRarityTypeAsString		(FString InWeaponRarityTypeAsString);

	void UpdateDamageType					(EWeaponDamageType InDamageType);
	void UpdateDamageTypeAsString			(FString InDamageTypeAsString);

	void UpdateDamage						(int32 InDamage);
	void UpdateDamageAsString				(FString InDamageAsString);

	void UpdateRandomDamage					(int32 InRandomDamageMin, int32 InRandomDamageMax);
	void UpdateRandomDamageAsString			(FString InRandomDamageAsString);

	void UpdatePenetration					(float InPenetration);
	void UpdatePenetrationAsString			(FString InPenetrationAsString);

	void UpdateRange						(float InRange);
	void UpdateRangeAsString				(FString InRangeAsString);

	void UpdateWeight						(float InWeight);
	void UpdateWeightAsString				(FString InWeightAsString);

	void UpdateDurability					(float InDurability);
	void UpdateDurabilityAsString			(FString InDurabilityAsString);

	void UpdatePrice						(float InPrice);
	void UpdatePriceAsString				(FString InPriceAsString);

	void UpdateLongRangeWeaponType			(ELongRangeWeaponType InLongRangeWeaponType);
	void UpdateLongRangeWeaponTypeAsString	(FString InLongRangeWeaponTypeAsString);

	void UpdateAmmunitionType				(EAmmunitionType InAmmunitionType);
	void UpdateAmmunitionTypeAsString		(FString InAmmunitionTypeAsString);

	void UpdateFireType						(EFireType InFireType);
	void UpdateFireTypeAsString				(FString InFireTypeAsString);

	void UpdateFullAutomatic				(bool InFullAutomatic);
	void UpdateFullAutomaticAsString		(FString InFullAutomaticName);

	void UpdateFireModeType					(EFireModeType InFireModeType);
	void UpdateFireModeTypeAsString			(FString InFireModeTypeAsString);

	void UpdateAccuracy						(float InAccuracy);
	void UpdateAccuracyAsString				(FString InAccuracyAsString);

	void UpdateMagazineCapacity				(int32 InMagazineCapacity);
	void UpdateMagazineCapacityAsString		(FString InMagazineCapacityAsString);

	void UpdateFireRate						(float InFireRate);
	void UpdateFireRateAsString				(FString InFireRateAsString);

	void UpdateReloadTime					(float InReloadTime);
	void UpdateReloadTimeAsString			(FString InReloadTimeAsString);

	void UpdateShortRangeWeaponType			(EShortRangeWeaponType InShortRangeWeaponType);
	void UpdateShortRangeWeaponTypeAsString	(FString InShortRangeWeaponTypeAsString);

	void UpdateAttackSpeed					(float InAttackSpeed);
	void UpdateAttackSpeedAsString			(FString InAttackSpeedAsString);

	void UpdateWeaponNameAsString			(FString InWeaponNameAsString);
};
