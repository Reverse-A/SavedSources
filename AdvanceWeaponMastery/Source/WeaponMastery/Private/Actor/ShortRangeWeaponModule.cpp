#include "Actor/ShortRangeWeaponModule.h"
AShortRangeWeaponModule::AShortRangeWeaponModule()
{
	// Check if the WeaponMasteryComponent is valid
	if (WeaponMasteryComponent != nullptr)
	{
		// Set the weapon type to ShortRange
		EWeaponType ShortRangeWeaponType;
		ShortRangeWeaponType = EWeaponType::ShortRange;

		// Set the weapon type in WeaponMasteryComponent
		WeaponMasteryComponent->SetWeaponType(ShortRangeWeaponType);

		// Update the weapon type in the associated weapon data
		WeaponMasteryComponent->GetWeaponData()->UpdateWeaponType(ShortRangeWeaponType);

		// Get the string representation of the short range weapon type
		FString ShortRangeWeaponTypeString = WeaponMasteryComponent->GetWeaponTypeAsString(ShortRangeWeaponType);

		// Update the string representation of the weapon type in the weapon data
		WeaponMasteryComponent->GetWeaponData()->UpdateWeaponTypeAsString(ShortRangeWeaponTypeString);
	}
}
