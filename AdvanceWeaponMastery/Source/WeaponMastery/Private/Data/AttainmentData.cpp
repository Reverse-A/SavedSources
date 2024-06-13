#include "Data/AttainmentData.h"

EAttainmentType UAttainmentData::GetAttainmentType() const
{
	// Return the current value of the AttainmentType property.
	return AttainmentType;
}

void UAttainmentData::SetAttainmentType(EAttainmentType InNewAttainmentType)
{
	// Set a new value for the AttainmentType property.
	AttainmentType = InNewAttainmentType;
}
