#include "Data/WeaponStateData.h"


// Inline function implementation to retrieve the current weapon state.
// This function returns the value of the WeaponStateType property.
EWeaponState UWeaponStateData::GetWeaponState() const
{
    return WeaponStateType;
}

// Function implementation to set a new weapon state for this instance.
// This function assigns the provided value (InWeaponState) to the WeaponStateType property.
void UWeaponStateData::SetWeaponState(EWeaponState InWeaponState)
{
    WeaponStateType = InWeaponState;
}
