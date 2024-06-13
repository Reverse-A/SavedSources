#include "Data/FireModeData.h"

 // Inline function implementation to retrieve the current firing mode.
EFireModeType UFireModeData::GetFireModeType() const
{
    return FireModeType;
}

// Function implementation to set a new firing mode for this instance.
void UFireModeData::SetFireModeType(EFireModeType InFireModeType)
{
    FireModeType = InFireModeType;
}