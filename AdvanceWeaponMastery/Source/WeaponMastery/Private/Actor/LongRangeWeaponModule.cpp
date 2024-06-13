#include "Actor/LongRangeWeaponModule.h"
ALongRangeWeaponModule::ALongRangeWeaponModule()
{
	// Check if the WeaponMasteryComponent is valid
	if (WeaponMasteryComponent != nullptr)
	{
		// Set the weapon type to LongRange
		EWeaponType LongRangeWeaponType;
		LongRangeWeaponType = EWeaponType::LongRange;
		WeaponMasteryComponent->SetWeaponType(LongRangeWeaponType);

		// Update the weapon type in the weapon data
		WeaponMasteryComponent->GetWeaponData()->UpdateWeaponType(LongRangeWeaponType);

		// Get the string representation of the long range weapon type
		FString LongRangeWeaponTypeString = WeaponMasteryComponent->GetWeaponTypeAsString(LongRangeWeaponType);

		// Update the string representation of the weapon type in the weapon data
		WeaponMasteryComponent->GetWeaponData()->UpdateWeaponTypeAsString(LongRangeWeaponTypeString);
	}
}
