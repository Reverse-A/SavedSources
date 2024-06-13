#include "Data/WeaponTypeData.h"

EWeaponType UWeaponTypeData::GetWeaponType() const
{
	return WeaponType;
}

void UWeaponTypeData::SetWeaponType(EWeaponType InWeaponType)
{
	WeaponType = InWeaponType;
}
