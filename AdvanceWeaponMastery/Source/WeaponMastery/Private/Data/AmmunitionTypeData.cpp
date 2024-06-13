#include "Data/AmmunitionTypeData.h"

 // Inline function implementation to retrieve the current ammunition type.
EAmmunitionType UAmmunitionTypeData::GetAmmunitionType() const
{
    return AmmunitionType;
}

// Function implementation to set a new ammunition type for this instance.
void UAmmunitionTypeData::SetAmmunitionType(EAmmunitionType InAmmunitionType)
{
    AmmunitionType = InAmmunitionType;
}