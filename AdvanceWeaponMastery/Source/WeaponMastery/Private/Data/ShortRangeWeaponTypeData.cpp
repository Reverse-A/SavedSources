#include "Data/ShortRangeWeaponTypeData.h"

// Inline function implementation to retrieve the current ShortRange weapon type.
EShortRangeWeaponType UShortRangeWeaponTypeData::GetShortRangeWeaponType() const
{
    return MeleeWeaponType;
}

// Function implementation to set a new melee weapon type for this instance.
void UShortRangeWeaponTypeData::SetShortRangeWeaponType(EShortRangeWeaponType InMeleeWeaponType)
{
    MeleeWeaponType = InMeleeWeaponType;
}
