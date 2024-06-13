#include "Data/LongRangeWeaponTypeData.h"

 // Inline function implementation to retrieve the current weapon type.
ELongRangeWeaponType ULongRangeWeaponTypeData::GetLongRangeWeaponType() const
{
    return LongRangeWeaponType;
}

// Function implementation to set a new weapon type for this instance.
void ULongRangeWeaponTypeData::SetLongRangeWeaponType(ELongRangeWeaponType InLongRangeWeaponType)
{
    LongRangeWeaponType = InLongRangeWeaponType;
}