#include "Data/WeaponData.h"


void UWeaponData::UpdateWeaponAttainmentType(EAttainmentType InWeaponAttainmentType)
{
    WeaponInformation.WeaponAttainmentType = InWeaponAttainmentType;
}

void UWeaponData::UpdateWeaponAttainmentTypeAsString(FString InWeaponAttainmentTypeAsString)
{
    WeaponInformationStrings.WeaponAttainmentType = InWeaponAttainmentTypeAsString;
}

void UWeaponData::UpdateWeaponStateType(EWeaponState InWeaponStateType)
{
    WeaponInformation.WeaponStateType = InWeaponStateType;
}


void UWeaponData::UpdateWeaponStateTypeAsString(FString InWeaponStateTypeAsString)
{
    WeaponInformationStrings.WeaponStateType = InWeaponStateTypeAsString;
}

void UWeaponData::UpdateWeaponType(EWeaponType InWeaponType)
{
    WeaponInformation.WeaponType = InWeaponType;
}

void UWeaponData::UpdateWeaponTypeAsString(FString InWeaponTypeAsString)
{
    WeaponInformationStrings.WeaponTypeName = InWeaponTypeAsString;
}

void UWeaponData::UpdateRarityEffectProcess(ERarityEffectProcess InRarityEffectProcess)
{
    WeaponInformation.RarityEffectProcess = InRarityEffectProcess;
}

void UWeaponData::UpdateRarityEffectProcessAsString(FString InRarityEffectProcessAsString)
{
    WeaponInformationStrings.RarityEffectProcess = InRarityEffectProcessAsString;
}

void UWeaponData::UpdateWeaponRarityType(ERarityType InWeaponRarityType)
{
    WeaponInformation.WeaponRarityType = InWeaponRarityType;
}

void UWeaponData::UpdateWeaponRarityTypeAsString(FString InWeaponRarityTypeAsString)
{
    WeaponInformationStrings.WeaponRarityTypeName = InWeaponRarityTypeAsString;
}

void UWeaponData::UpdateDamageType(EWeaponDamageType InDamageType)
{
    WeaponInformation.WeaponDamageType = InDamageType;
}

void UWeaponData::UpdateDamageTypeAsString(FString InDamageTypeAsString)
{
    WeaponInformationStrings.DamageTypeName = InDamageTypeAsString;
}

void UWeaponData::UpdateDamage(int32 InDamage)
{
    WeaponInformation.Damage = InDamage;
}

void UWeaponData::UpdateDamageAsString(FString InDamageAsString)
{
    WeaponInformationStrings.WeaponDamage = InDamageAsString;
}

void UWeaponData::UpdateRandomDamage(int32 InRandomDamageMin, int32 InRandomDamageMax)
{
    WeaponInformation.RandomDamageMin = InRandomDamageMin;
    WeaponInformation.RandomDamageMax = InRandomDamageMax;
}

void UWeaponData::UpdateRandomDamageAsString(FString InRandomDamageAsString)
{
    WeaponInformationStrings.WeaponRandomDamage = InRandomDamageAsString;
}

void UWeaponData::UpdatePenetration(float InPenetration)
{
    WeaponInformation.Penetration = InPenetration;
}

void UWeaponData::UpdatePenetrationAsString(FString InPenetrationAsString)
{
    WeaponInformationStrings.WeaponPenetration = InPenetrationAsString;
}

void UWeaponData::UpdateRange(float InRange)
{
    WeaponInformation.Range = InRange;
}

void UWeaponData::UpdateRangeAsString(FString InRangeAsString)
{
    WeaponInformationStrings.WeaponRange = InRangeAsString;
}

void UWeaponData::UpdateWeight(float InWeight)
{
    WeaponInformation.Weight = InWeight;
}

void UWeaponData::UpdateWeightAsString(FString InWeightAsString)
{
    WeaponInformationStrings.WeaponWeight = InWeightAsString;
}

void UWeaponData::UpdateDurability(float InDurability)
{
    WeaponInformation.Durability = InDurability;
}

void UWeaponData::UpdateDurabilityAsString(FString InDurabilityAsString)
{
    WeaponInformationStrings.WeaponDurability = InDurabilityAsString;
}

void UWeaponData::UpdatePrice(float InPrice)
{
    WeaponInformation.Price = InPrice;
}

void UWeaponData::UpdatePriceAsString(FString InPriceAsString)
{
    WeaponInformationStrings.WeaponPrice = InPriceAsString;
}

void UWeaponData::UpdateLongRangeWeaponType(ELongRangeWeaponType InLongRangeWeaponType)
{
    WeaponInformation.LongRangeWeaponType = InLongRangeWeaponType;
}

void UWeaponData::UpdateLongRangeWeaponTypeAsString(FString InLongRangeWeaponTypeAsString)
{
    WeaponInformationStrings.LongRangeWeaponTypeName = InLongRangeWeaponTypeAsString;
}

void UWeaponData::UpdateAmmunitionType(EAmmunitionType InAmmunitionType)
{
    WeaponInformation.AmmunitionType = InAmmunitionType;
}

void UWeaponData::UpdateAmmunitionTypeAsString(FString InAmmunitionTypeAsString)
{
    WeaponInformationStrings.WeaponAmmunitionTypeName = InAmmunitionTypeAsString;
}

void UWeaponData::UpdateFireType(EFireType InFireType)
{
    WeaponInformation.FireType = InFireType;
}

void UWeaponData::UpdateFireTypeAsString(FString InFireTypeAsString)
{
    WeaponInformationStrings.FireType = InFireTypeAsString;
}

void UWeaponData::UpdateFullAutomatic(bool InFullAutomatic)
{
    WeaponInformation.bFullAutomatic = InFullAutomatic;
}

void UWeaponData::UpdateFullAutomaticAsString(FString InFullAutomaticAsString)
{
    WeaponInformationStrings.bIsFullAutomatic = InFullAutomaticAsString;
}

void UWeaponData::UpdateFireModeType(EFireModeType InFireModeType)
{
    WeaponInformation.FireModeType = InFireModeType;
}

void UWeaponData::UpdateFireModeTypeAsString(FString InFireModeTypeAsString)
{
    WeaponInformationStrings.FireModeType = InFireModeTypeAsString;
}

void UWeaponData::UpdateAccuracy(float InAccuracy)
{
    WeaponInformation.Accuracy = InAccuracy;
}

void UWeaponData::UpdateAccuracyAsString(FString InAccuracyAsString)
{
    WeaponInformationStrings.WeaponAccuracy = InAccuracyAsString;
}

void UWeaponData::UpdateMagazineCapacity(int32 InMagazineCapacity)
{
    WeaponInformation.MagazineCapacity = InMagazineCapacity;
}

void UWeaponData::UpdateMagazineCapacityAsString(FString InMagazineCapacityAsString)
{
    WeaponInformationStrings.MagazineCapacity = InMagazineCapacityAsString;
}

void UWeaponData::UpdateFireRate(float InFireRate)
{
    WeaponInformation.FireRate = InFireRate;
}

void UWeaponData::UpdateFireRateAsString(FString InFireRateAsString)
{
    WeaponInformationStrings.WeaponFireRate = InFireRateAsString;
}

void UWeaponData::UpdateReloadTime(float InReloadTime)
{
    WeaponInformation.ReloadTime = InReloadTime;
}

void UWeaponData::UpdateReloadTimeAsString(FString InReloadTimeAsString)
{
    WeaponInformationStrings.WeaponReloadTime = InReloadTimeAsString;
}

void UWeaponData::UpdateShortRangeWeaponType(EShortRangeWeaponType InShortRangeWeaponType)
{
    WeaponInformation.ShortRangeWeaponType = InShortRangeWeaponType;
}

void UWeaponData::UpdateShortRangeWeaponTypeAsString(FString InShortRangeWeaponTypeAsString)
{
    WeaponInformationStrings.ShortRangeWeaponTypeName = InShortRangeWeaponTypeAsString;
}

void UWeaponData::UpdateAttackSpeed(float InAttackSpeed)
{
    WeaponInformation.AttackSpeed = InAttackSpeed;
}

void UWeaponData::UpdateAttackSpeedAsString(FString InAttackSpeedAsString)
{
    WeaponInformationStrings.WeaponAttackSpeed = InAttackSpeedAsString;
}


void UWeaponData::UpdateWeaponNameAsString(FString InWeaponNameAsString)
{
    WeaponInformationStrings.WeaponName = InWeaponNameAsString;
}
