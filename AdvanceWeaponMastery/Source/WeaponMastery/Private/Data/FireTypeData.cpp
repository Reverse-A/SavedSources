#include "Data/FireTypeData.h"

// Inline function implementation to retrieve the current firing type.
// This function returns the value of the FireType property.
EFireType UFireTypeData::GetFireType() const
{
    return FireType;
}

// Function implementation to set a new firing type for this instance.
// This function assigns the provided value (InFireType) to the FireType property.
void UFireTypeData::SetFireType(EFireType InFireType)
{
    FireType = InFireType;
}